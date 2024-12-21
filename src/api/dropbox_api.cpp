#include "dropbox_api.hpp"

using namespace std;


DropboxAPI::DropboxAPI(
    const string &token_file_path,  
    const string &client_id, 
    const string &client_secret, 
    const int redirect_port
): client(), token_file_path(token_file_path), CLIENT_ID(client_id), CLIENT_SECRET(client_secret), REDIRECT_URI("http://127.0.0.1:"+to_string(redirect_port)), REDIRECT_PORT(redirect_port) {}


string DropboxAPI::get_redirected_code() { // TODO: reget other port?
    HttpServer server("127.0.0.1", REDIRECT_PORT);
    server.start();
    string code = server.get_code();
    server.stop();
    return code;
}


string DropboxAPI::get_authorization_url() {
    string auth_url = AUTH_URL + "?client_id=" + CLIENT_ID +
                           "&redirect_uri=" + REDIRECT_URI +
                           "&response_type=code&force_reauthentication=true&token_access_type=offline";
    return auth_url;
}


void DropboxAPI::refresh_access_token() {
    string data = "grant_type=refresh_token&refresh_token=" + refresh_token +
                "&client_id=" + CLIENT_ID +
                "&client_secret=" + CLIENT_SECRET;
    map<string, string> headers = {
        {"Content-type", "application/x-www-form-urlencoded"},
    };
    HttpResponse response = client.post(TOKEN_URL, headers, data);
    json token_data = json::parse(response.first);
    access_token = token_data["access_token"];
    int expired_in = token_data["expires_in"];
    token_data["expired"] = timepoint_to_string(add_seconds_to_timepoint(get_now(), expired_in-10));
    token_data["refresh_token"] = refresh_token;
    token_data["service"] = "Dropbox";
    expired_time = string_to_timepoint(token_data["expired"]);
    
    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();
}


void DropboxAPI::check_access_token() {
    if(get_now() > expired_time) {
        refresh_access_token();
    }
}


string DropboxAPI::get_access_token_from_code(string& auth_code) {
    map<string, string> headers = {
        {"Content-type", "application/x-www-form-urlencoded"},
    };

    string data = "code=" + auth_code +
                    "&client_id=" + CLIENT_ID +
                    "&client_secret=" + CLIENT_SECRET +
                    "&redirect_uri=" + REDIRECT_URI +
                    "&grant_type=authorization_code";

    HttpResponse response = client.post(TOKEN_URL, headers, data);

    json token_data = json::parse(response.first);
    access_token = token_data["access_token"];
    refresh_token = token_data["refresh_token"];
    int expired_in = token_data["expires_in"];
    token_data["expired"] = timepoint_to_string(add_seconds_to_timepoint(get_now(), expired_in-10));
    token_data["service"] = "Dropbox";
    expired_time = string_to_timepoint(token_data["expired"]);

    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();

    return access_token;
}

string DropboxAPI::get_access_token() {
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


json DropboxAPI::get_account_info() {
    json return_data;
    json data;
    
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()}
    };
    HttpResponse response = client.post("https://api.dropboxapi.com/2/users/get_current_account", headers);
    data = json::parse(response.first);
    return_data["email"] = data["email"];
    response = client.post("https://api.dropboxapi.com/2/users/get_space_usage", headers);
    data = json::parse(response.first);
    return_data["total_space"] = data["allocation"]["allocated"];
    return_data["used_space"] = data["used"];
    return return_data;
}

json DropboxAPI::get_list_files() {
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
        {"Content-Type", "application/json"}
    };
    json data = {
        {"include_deleted", false},
        {"include_has_explicit_shared_members", false},
        {"include_mounted_folders", true},
        {"include_non_downloadable_files", true},
        {"path", ""},
        {"recursive", true},
        {"limit", 1000}
    };
    string url = "https://api.dropboxapi.com/2/files/list_folder";
    
    HttpResponse response;
    json json_response;
    json return_data = json::array();

    response = client.post(url, headers, data.dump());
    json_response = json::parse(response.first);

    for(const auto& file : json_response["entries"]) {
        return_data.push_back(file);
    }

    while(json_response["has_more"] == true) {
        data = {
            {"cursor", json_response["cursor"]},
        };
        response = client.post(url + "/continue", headers, data.dump());
        json_response = json::parse(response.first);

        for(const auto& file : json_response["entries"]) {
            return_data.push_back(file);
        }
    }
    
    return return_data;
}

string DropboxAPI::get_download_url(string &file_path) {
    string url = "https://content.dropboxapi.com/2/files/download";
    return url;
}

json DropboxAPI::create_folder_yandex(const string &folder_path) {
    map<string, string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + get_access_token()}
    };
    json data = {
        {"autorename", true},
        {"path", folder_path}
    };
    string url = "https://api.dropboxapi.com/2/files/create_folder_v2";

    HttpResponse response = client.post(url, headers, data.dump());
    data = {
        {"include_deleted", false},
        {"include_has_explicit_shared_members", false},
        {"path", json::parse(response.first)["metadata"]["path_display"]},
    };
    response = client.post("https://api.dropboxapi.com/2/files/get_metadata", headers, data.dump());
    return json::parse(response.first);
}

string DropboxAPI::init_upload_file_dropbox(vector<char> &buffer, unsigned long long bytes_sent, streamsize gcount) {
    json headers_data = {
        {"close", false}
    };
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
        {"Dropbox-API-Arg", headers_data.dump()},
        {"Content-Type", "application/octet-stream"}
    };

    HttpResponse response = client.post("https://content.dropboxapi.com/2/files/upload_session/start", headers, string(buffer.data(), gcount));
    return json::parse(response.first)["session_id"];
}

void DropboxAPI::send_upload_part(
    string &upload_uri,
    unsigned long long toRead,
    unsigned long long bytes_sent,
    unsigned long long file_size,
    vector<char> &buffer,
    unsigned int chunk_size
) {
    json headers_data = {
        {"close", false},
        {"cursor", {
            {"offset", bytes_sent},
            {"session_id", upload_uri},
        }},
    };
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
        {"Dropbox-API-Arg", headers_data.dump()},
        {"Content-Type", "application/octet-stream"}
    };

    client.post("https://content.dropboxapi.com/2/files/upload_session/append_v2", headers, string(buffer.data(), toRead));
};

void DropboxAPI::finish_upload_file_dropbox(unsigned long long bytes_sent, string session_id, string file_path) {
    json headers_data = {
        {"cursor", {
            {"session_id", session_id},
            {"offset", bytes_sent},
        }},
        {"commit", {
            {"autorename", true},
            {"mode", "add"},
            {"mute", false},
            {"path", file_path},
            {"strict_conflict", false},
        }},
    };
    map<string, string> headers = {
        {"Authorization", "Bearer " + get_access_token()},
        {"Dropbox-API-Arg", headers_data.dump()},
        {"Content-Type", "application/octet-stream"}
    };

    client.post("https://content.dropboxapi.com/2/files/upload_session/finish", headers);
};

void DropboxAPI::move_to_trash(std::string &file_path) {
    string url = "https://api.dropboxapi.com/2/files/delete_v2";
    string file_p = file_path;
    if(file_p[file_p.length()-1] == '/')
        file_p = file_p.substr(0, file_p.length()-1);
    json data = {
        {"path", file_p}
    };
    map<string, string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + get_access_token()}
    };

    client.post(url, headers, data.dump());
}