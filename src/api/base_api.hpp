#ifndef BASE_API_HPP
#define BASE_API_HPP

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "../utils/datetime.hpp"

using json = nlohmann::json;

class BaseAPI {
public:
    virtual ~BaseAPI();
    virtual void check_access_token() = 0;
    virtual std::string get_authorization_url() = 0;
    virtual std::string get_redirected_code() = 0;
    virtual std::string get_access_token_from_code(std::string &auth_code) = 0;
    virtual std::string get_access_token() = 0;


    virtual json get_account_info() = 0;
    virtual json get_list_files() = 0;
    virtual std::string get_download_url(std::string &file_id) = 0;
    virtual std::string get_export_google_document_url(std::string &file_id, std::string &mime_type) = 0;
    virtual json create_folder(std::string &folder_name, std::string &parent_id) = 0;
    virtual json create_folder_yandex(const std::string &folder_path) = 0;
    virtual std::string init_upload_file_yandex(const std::string &file_path) = 0;
    virtual std::string init_upload_file(std::string &file_name, std::string &parent_id) = 0;
    virtual std::string init_upload_file_dropbox(std::vector<char> &buffer, unsigned long long bytes_sent, std::streamsize gcount) = 0;
    virtual void finish_upload_file_dropbox(unsigned long long bytes_sent, std::string session_id, std::string file_path) = 0;
    virtual void send_upload_part(
        std::string &upload_uri,
        unsigned long long toRead,
        unsigned long long bytes_sent,
        unsigned long long file_size,
        std::vector<char> &buffer,
        unsigned int chunk_size = 256 * 1024
    ) = 0;
    virtual void move_to_trash(std::string &file_id) = 0;

private:
    virtual void refresh_access_token() = 0;
};

#endif // GOOGLE_API_HPP