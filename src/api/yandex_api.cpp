#include "yandex_api.hpp"

using namespace std;


YandexAPI::YandexAPI(
    const string &token_file_path,  
    const string &client_id, 
    const string &client_secret, 
    const int redirect_port
): client(), token_file_path(token_file_path), CLIENT_ID(client_id), CLIENT_SECRET(client_secret), REDIRECT_URI("http://127.0.0.1:"+to_string(redirect_port)), REDIRECT_PORT(redirect_port) {}


string YandexAPI::get_redirected_code() { // TODO: reget other port?
    HttpServer server("127.0.0.1", REDIRECT_PORT);
    server.start();
    string code = server.get_code();
    server.stop();
    return code;
}


string YandexAPI::get_authorization_url() {
    string auth_url = AUTH_URL + "?client_id=" + CLIENT_ID +
                           "&redirect_uri=" + REDIRECT_URI +
                           "&response_type=code&force_confirm=1";
    return auth_url;
}


void YandexAPI::refresh_access_token() {
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
    token_data["service"] = "Yandex";
    expired_time = string_to_timepoint(token_data["expired"]);
    
    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();
}


void YandexAPI::check_access_token() {
    if(get_now() > expired_time) {
        refresh_access_token();
    }
}


string YandexAPI::get_access_token_from_code(string& auth_code) {
    map<string, string> headers = {
        {"Content-type", "application/x-www-form-urlencoded"},
    };

    string data = "code=" + auth_code +
                    "&client_id=" + CLIENT_ID +
                    "&client_secret=" + CLIENT_SECRET +
                    "&grant_type=authorization_code";

    HttpResponse response = client.post(TOKEN_URL, headers, data);

    json token_data = json::parse(response.first);
    access_token = token_data["access_token"];
    refresh_token = token_data["refresh_token"];
    int expired_in = token_data["expires_in"];
    token_data["expired"] = timepoint_to_string(add_seconds_to_timepoint(get_now(), expired_in-10));
    token_data["service"] = "Yandex";
    expired_time = string_to_timepoint(token_data["expired"]);

    ofstream token_file(token_file_path);
    token_file << token_data.dump();
    token_file.close();

    return access_token;
}

string YandexAPI::get_access_token() {
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


json YandexAPI::get_account_info() {
    map<string, string> headers = {
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };
    string url = "https://cloud-api.yandex.net/v1/disk?fields=total_space,used_space,user.login";

    HttpResponse response = client.get(url, headers);
    return json::parse(response.first);
}

json YandexAPI::get_list_files() {
    map<string, string> headers = {
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };
    string url = "https://cloud-api.yandex.net/v1/disk/resources?limit=10000&fields=_embedded.items.path,_embedded.items.name,_embedded.items.size,_embedded.items.mime_type,_embedded.items.created,_embedded.items.modified,_embedded.items.type,&path=";
    
    unsigned int offset;
    HttpResponse response;
    json json_response;
    json data = json::array();
    vector<string> process = {"disk:/"};

    while(process.size() > 0) {
        string cur_path = process.back();
        process.pop_back();
        offset = 0;
        while(1) {
            response = client.get(url + cur_path + "&offset=" + to_string(offset), headers);
            json_response = json::parse(response.first);

            if(json_response["_embedded"]["items"].empty()) break;

            for (const auto& file : json_response["_embedded"]["items"]) {
                data.push_back(file);
                if(file["type"] == "dir") process.push_back(file["path"].get<string>());
            }
            offset += 10000;
        }
    }
    
    return data;
}

string YandexAPI::get_download_url(string &file_path) {
    map<string, string> headers = {
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };
    string url = "https://cloud-api.yandex.net/v1/disk/resources/download?path=" + file_path;
    HttpResponse response = client.get(url, headers);
    json response_json = json::parse(response.first);
    return response_json["href"].get<string>();
}

json YandexAPI::create_folder_yandex(const string &folder_path) {
    map<string, string> headers = {
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };

    string url = "https://cloud-api.yandex.net/v1/disk/resources?path=" + folder_path;

    HttpResponse response = client.put(url, headers);
    json json_respose = json::parse(response.first);
    if(json_respose.contains("error")) return json_respose;
    response = client.get(json_respose["href"].get<string>() + "&fields=name,created,modified,path,type", headers);
    return json::parse(response.first);
}

string YandexAPI::init_upload_file_yandex(const string& file_path) {
    string url = "https://cloud-api.yandex.net/v1/disk/resources/upload?path=" + file_path;
    
    map<string, string> headers = {
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };

    HttpResponse response = client.get(url, headers);
    json data = json::parse(response.first);
    return data["href"].get<string>();
}

void YandexAPI::send_upload_part(
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

void YandexAPI::move_to_trash(std::string &file_path) {
    string url = "https://cloud-api.yandex.net/v1/disk/resources?path=" + file_path;

    map<string, string> headers = {
        {"Accept", "application/json"},
        {"Authorization", "OAuth " + get_access_token()}
    };

    client.delete_(url, headers);
}