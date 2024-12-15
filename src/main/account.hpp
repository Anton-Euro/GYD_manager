#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "../api/base_api.hpp"
#include "../api/google_api.hpp"
#include "../api/yandex_api.hpp"
#include "item.hpp"
#include "../utils/tools.hpp"

class Account {
public:
    std::string mail;
    std::string service;
    unsigned long long storage_size;
    unsigned long long used_size;

    std::shared_ptr<Item> files;
    std::shared_ptr<BaseAPI> session;
    std::string session_file_name;

    Account(const std::string &path);
    Account() = default;

    void init_files();
    void init_files_from_file();
    void save_to_file(json data);
    std::shared_ptr<Item> find_item_by_id(std::string item_id);

    std::string get_saved_path(std::string saved_path, std::shared_ptr<Item> item);

private:
    void google_init_files(json list_files, std::shared_ptr<Item> item_ptr);
    void yandex_init_files(json list_files, std::shared_ptr<Item> item_ptr);

    std::string remove_extension(const std::string& name, const std::string& extension);
    std::shared_ptr<Item> find_item_by_id_req(std::string& item_id, std::shared_ptr<Item> root);

};

#endif // ACCOUNT_HPP