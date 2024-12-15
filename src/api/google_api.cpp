#include "google_api.hpp"

using namespace std;


GoogleAPI::GoogleAPI(
    const string &token_file_path,  
    const string &client_id, 
    const string &client_secret, 
    const int redirect_port
): client(), token_file_path(token_file_path), CLIENT_ID(client_id), CLIENT_SECRET(client_secret), REDIRECT_URI("http://127.0.0.1:"+to_string(redirect_port)), REDIRECT_PORT(redirect_port) {}


string GoogleAPI::get_redirected_code() { // TODO: reget other port?
    HttpServer server("127.0.0.1", REDIRECT_PORT);
    server.start();
    string code = server.get_code();
    server.stop();
    return code;
}


string GoogleAPI::get_authorization_url() {
    string auth_url = AUTH_URL + "?client_id=" + CLIENT_ID +
                           "&redirect_uri=" + REDIRECT_URI +
                           "&response_type=code" +
                           "&scope=https://www.googleapis.com/auth/drive";
    return auth_url;
}


void GoogleAPI::refresh_access_token() {
    json data = {
        {"grant_type", "refresh_token"},
        {"refresh_token", refresh_token},
        {"client_id", CLIENT_ID},
        {"client_secret", CLIENT_SECRET},
    };
    HttpResponse response = client.post(TOKEN_URL, {}, data.dump());
    json token_data = json::parse(response.first);
    access_token = token_data["access_token"];
    int expired_in = token_data["expires_in"];
    token_data["expired"] = timepoint_to_string(add_seconds_to_timepoint(get_now(), expired_in-10));
    token_data["refresh_token"] = refresh_token;
    token_data["service"] = "Google";
    expired_time = string_to_timepoint(token_data["expired"]);
    
    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();
}


void GoogleAPI::check_access_token() {
    if(get_now() > expired_time) {
        refresh_access_token();
    }
}


string GoogleAPI::get_access_token_from_code(string &auth_code) {
    string post_fields = "code=" + auth_code +
                        "&client_id=" + CLIENT_ID +
                        "&client_secret=" + CLIENT_SECRET +
                        "&redirect_uri=" + REDIRECT_URI +
                        "&grant_type=authorization_code";

    HttpResponse response = client.post(TOKEN_URL+"?"+post_fields);

    json token_data = json::parse(response.first);
    access_token = token_data["access_token"];
    refresh_token = token_data["refresh_token"];
    int expired_in = token_data["expires_in"];
    token_data["expired"] = timepoint_to_string(add_seconds_to_timepoint(get_now(), expired_in-10));
    token_data["service"] = "Google";
    expired_time = string_to_timepoint(token_data["expired"]);

    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();

    return access_token;
}

string GoogleAPI::get_access_token() {
    ifstream file(token_file_path);
    if(file) {
        json file_data;
        file >> file_data;
        file.close();
        access_token = file_data["access_token"];
        refresh_token = file_data["refresh_token"];
        expired_time = string_to_timepoint(file_data["expired"]);
        check_access_token();
        return access_token;
    }
    return NULL;
}


json GoogleAPI::get_account_info() {
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
    };
    string url = "https://www.googleapis.com/drive/v3/about?fields=user,storageQuota";

    HttpResponse response = client.get(url, headers);
    return json::parse(response.first);
}

json GoogleAPI::get_list_files() {
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
    };
    string url = "https://www.googleapis.com/drive/v3/files?q=%27me%27%20in%20owners&fields=files(id,name,fileExtension,size,mimeType,createdTime,modifiedTime,shared,trashed,trashedTime,parents),nextPageToken";
    
    HttpResponse response = client.get("https://www.googleapis.com/drive/v3/files/root?fields=id", headers);
    json json_response = json::parse(response.first);
    
    json data = {
        {"root_id", json_response["id"].get<string>()},
        {"files", json::array()}
    };
    string next_page_token = "";

    while(1) {
        if(next_page_token != "")
            response = client.get(url + "&pageToken=" + next_page_token, headers);
        else
            response = client.get(url, headers);
        json_response = json::parse(response.first);
        for(const auto& file : json_response["files"]) {
            data["files"].push_back(file);
        }
        if(json_response.contains("nextPageToken"))
            next_page_token = json_response["nextPageToken"];
        else break;
    }
    
    return data;
}

string GoogleAPI::get_download_url(string &file_id) {
    return "https://www.googleapis.com/drive/v3/files/" + file_id + "?alt=media";
}

string GoogleAPI::get_export_google_document_url(string &file_id, string &mime_type) {
    if(mime_type == "application/vnd.google-apps.document") {
        return "https://www.googleapis.com/drive/v3/files/" + file_id + "/export?mimeType=application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    } else if(mime_type == "application/vnd.google-apps.spreadsheet") {
        return "https://www.googleapis.com/drive/v3/files/" + file_id + "/export?mimeType=application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    } else if(mime_type == "application/vnd.google-apps.presentation") {
        return "https://www.googleapis.com/drive/v3/files/" + file_id + "/export?mimeType=application/vnd.openxmlformats-officedocument.presentationml.presentation";
    } else {
        return "https://www.googleapis.com/drive/v3/files/" + file_id + "/export";
    }
}

json GoogleAPI::create_folder(string &folder_name, string &parent_id) {
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
    };

    json data = {
        {"name", folder_name},
        {"mimeType", "application/vnd.google-apps.folder"},
        {"parents", json::array({parent_id})}
    };

    string url = "https://www.googleapis.com/drive/v3/files?fields=id,name,mimeType,createdTime,modifiedTime,parents,shared,trashed,trashedTime";

    HttpResponse response = client.post(url, headers, data.dump());
    return json::parse(response.first);
}

string GoogleAPI::init_upload_file(string &file_name, string &parent_id) {
    string url = "https://www.googleapis.com/upload/drive/v3/files?uploadType=resumable";
    
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
        {"Content-Type", "application/json; charset=UTF-8"},
    };

    json data = {
        {"name", file_name},
        {"parents", json::array({parent_id})}
    };
    HttpResponse response = client.post(url, headers, data.dump());
    return response.second["Location"];
}

void GoogleAPI::send_upload_part(
    string &upload_uri,
    unsigned long long to_read,
    unsigned long long bytes_sent,
    unsigned long long file_size,
    vector<char> &buffer,
    unsigned int chunk_size
) {
    map<string, string> headers = {
        {"Content-Length", to_string(to_read)},
        {"Content-Range", "bytes " + to_string(bytes_sent) + "-" + to_string(bytes_sent + to_read - 1) + "/" + to_string(file_size)},
    };
    client.put(upload_uri, headers, string(buffer.data(), to_read));
}

void GoogleAPI::move_to_trash(std::string &file_id) {
    string url = "https://www.googleapis.com/drive/v3/files/" + file_id;

    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
    };

    json data = {
        {"trashed", true}
    };

    client.patch(url, headers, data.dump());
}