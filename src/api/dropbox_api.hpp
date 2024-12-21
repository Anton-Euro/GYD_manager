#ifndef DROPBOX_API_HPP
#define DROPBOX_API_HPP

#include <fstream>
#include <nlohmann/json.hpp>
#include "base_api.hpp"
#include "http/client.hpp"
#include "http/server.hpp"
#include "../utils/datetime.hpp"

using json = nlohmann::json;

class DropboxAPI : public BaseAPI {
public:
    DropboxAPI(
        const std::string &token_file_path,
        const std::string &client_id = "", 
        const std::string &client_secret = "",
        const int redirect_port = 15333
    );

    void check_access_token() override;
    std::string get_authorization_url() override;
    std::string get_redirected_code() override;
    std::string get_access_token_from_code(std::string &auth_code) override;
    std::string get_access_token() override;


    json get_account_info() override;
    json get_list_files() override;
    std::string get_download_url(std::string &file_path) override;
    json create_folder_yandex(const std::string &folder_path) override;
    std::string init_upload_file_dropbox(std::vector<char> &buffer, unsigned long long bytes_sent, std::streamsize gcount) override;
    void finish_upload_file_dropbox(unsigned long long bytes_sent, std::string session_id, std::string file_path) override;
    void send_upload_part(
        std::string &upload_uri,
        unsigned long long toRead,
        unsigned long long bytes_sent,
        unsigned long long file_size,
        std::vector<char> &buffer,
        unsigned int chunk_size = 256 * 1024
    ) override;
    void move_to_trash(std::string &file_path) override;

    std::string get_export_google_document_url(std::string &file_id, std::string &mime_type) override {return "";};
    json create_folder(std::string &folder_name, std::string &parent_id) override {return json::array();};
    std::string init_upload_file(std::string &file_name, std::string &parent_id) override {return "";};
    std::string init_upload_file_yandex(const std::string &file_path) override {return "";};

private:
    void refresh_access_token() override;

    HttpClient client;
    const std::string token_file_path;
    const std::string CLIENT_ID;
    const std::string CLIENT_SECRET;
    const std::string REDIRECT_URI;
    const int REDIRECT_PORT;

    std::string access_token;
    std::string refresh_token;
    datetime::time_point expired_time;

    const std::string AUTH_URL = "https://www.dropbox.com/oauth2/authorize";
    const std::string TOKEN_URL = "https://api.dropboxapi.com/oauth2/token";
};

#endif // DROPBOX_API_HPP