#include "account.hpp"

using namespace std;


Account::Account(const string &path) {
    ifstream file(path);
    if(file) {
        json file_data;
        file >> file_data;
        file.close();
        service = file_data["service"];
    }
    if(service == "Google") {
        session = make_shared<GoogleAPI>(path);
        session->get_access_token();
        json info = session->get_account_info();
        mail = info["user"]["emailAddress"];
        storage_size = stoull(info["storageQuota"]["limit"].get<std::string>());
        used_size = stoull(info["storageQuota"]["usage"].get<std::string>());
    } else if(service == "Yandex") {
        session = make_shared<YandexAPI>(path);
        session->get_access_token();
        json info = session->get_account_info();
        mail = info["user"]["login"].get<string>() + "@yandex.ru";
        storage_size = info["total_space"].get<unsigned long long>();
        used_size = info["used_space"].get<unsigned long long>();
    }
    
    session_file_name = fs::path(path).filename().string();
    files = make_shared<Item>("acc", mail, "", 0, service, get_now(), get_now(), true, false, false, nullptr);
}

string Account::remove_extension(const string& name, const string& extension) {
    string suffix = "." + extension;
    if (name.size() >= suffix.size() && name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0) {
        return name.substr(0, name.size() - suffix.size());
    }
    return name;
}

void Account::init_files() {
    if(service == "Google") {
        json list_files = session->get_list_files();

        if(!fs::exists("files_cache")) {
            fs::create_directory("files_cache");
        }
        ofstream file("files_cache/"+session_file_name);
        file << list_files.dump();
        file.close();

        files->id = list_files["root_id"];
        google_init_files(list_files, files);
    } else if(service == "Yandex") {
        json list_files = session->get_list_files();

        if(!fs::exists("files_cache")) {
            fs::create_directory("files_cache");
        }
        ofstream file("files_cache/"+session_file_name);
        file << list_files.dump();
        file.close();

        files->id = "disk:/";
        yandex_init_files(list_files, files);
    }
}

void Account::init_files_from_file() {
    if(service == "Google") {
        if(fs::exists("files_cache/"+session_file_name)) {
            json list_files;
            ifstream file("files_cache/"+session_file_name);
            file >> list_files;
            file.close();
            files->id = list_files["root_id"];
            google_init_files(list_files, files);
        } else {
            init_files();
        }
    } else if(service == "Yandex") {
        if(fs::exists("files_cache/"+session_file_name)) {
            json list_files;
            ifstream file("files_cache/"+session_file_name);
            file >> list_files;
            file.close();
            files->id = "disk:/";
            yandex_init_files(list_files, files);
        } else {
            init_files();
        }
    }
}


void Account::google_init_files(json list_files, shared_ptr<Item> item_ptr) {
    for(const auto &file : list_files["files"]) {
        for(const auto &parent : file["parents"]) {
            if(parent == item_ptr->id) {
                if(file["mimeType"] != "application/vnd.google-apps.folder" && file["trashed"].get<bool>() != true) {
                    if(starts_with(file["mimeType"], "application/vnd.google-apps") && !ends_with(file["mimeType"], "site")) {
                        item_ptr->add_item(
                            file["id"],
                            file.contains("fileExtension") ? remove_extension(file["name"], file["fileExtension"]) : file["name"].get<string>(),
                            ends_with(file["mimeType"], "document") ? "docx" : ends_with(file["mimeType"], "spreadsheet") ? "xlsx" : ends_with(file["mimeType"], "presentation") ? "pptx" : "",
                            stoull(file["size"].get<string>()),
                            file["mimeType"],
                            iso8601string_to_timepoint(file["createdTime"]),
                            iso8601string_to_timepoint(file["modifiedTime"]),
                            false,
                            file["shared"].get<bool>(),
                            file["trashed"].get<bool>(),
                            item_ptr
                        );
                    } else if(file["mimeType"] != "application/vnd.google-apps.site") 
                        item_ptr->add_item(
                            file["id"],
                            file.contains("fileExtension") ? remove_extension(file["name"], file["fileExtension"]) : file["name"].get<string>(),
                            file.contains("fileExtension") ? file["fileExtension"] : "",
                            stoull(file["size"].get<string>()),
                            file["mimeType"],
                            iso8601string_to_timepoint(file["createdTime"]),
                            iso8601string_to_timepoint(file["modifiedTime"]),
                            false,
                            file["shared"].get<bool>(),
                            file["trashed"].get<bool>(),
                            item_ptr
                        );
                } else if(file["mimeType"] == "application/vnd.google-apps.folder" && file["trashed"].get<bool>() != true) {
                    shared_ptr<Item> new_folder = item_ptr->add_item(
                        file["id"],
                        file["name"],
                        "",
                        0,
                        file["mimeType"],
                        iso8601string_to_timepoint(file["createdTime"]),
                        iso8601string_to_timepoint(file["modifiedTime"]),
                        true,
                        file["shared"].get<bool>(),
                        file["trashed"].get<bool>(),
                        item_ptr
                    );
                    google_init_files(list_files, new_folder);
                }
                
            }
        }
        
    }
}

void Account::yandex_init_files(json list_files, shared_ptr<Item> item_ptr) {
    for(const auto &file : list_files) {
        if(file["type"] == "file" && good_path_for_yandex(item_ptr->id, file["path"].get<string>())) {
            item_ptr->add_item(
                file["path"],
                cut_fullname(file["name"].get<string>()).first,
                cut_fullname(file["name"].get<string>()).second,
                file["size"].get<unsigned long long>(),
                file["mime_type"],
                iso8601string_to_timepoint(file["created"]),
                iso8601string_to_timepoint(file["modified"]),
                false,
                false,
                false,
                item_ptr
            );
        } else if(file["type"] == "dir" && good_path_for_yandex(item_ptr->id, file["path"].get<string>(), true)) {
            shared_ptr<Item> new_folder = item_ptr->add_item(
                file["path"].get<string>() + "/",
                file["name"],
                "",
                0,
                "yandex/folder",
                iso8601string_to_timepoint(file["created"]),
                iso8601string_to_timepoint(file["modified"]),
                true,
                false,
                false,
                item_ptr
            );
            yandex_init_files(list_files, new_folder);
        }    
    }
}

string Account::get_saved_path(string saved_path, shared_ptr<Item> item) {
    string file_name = check_filename(item->name);
    unsigned int file_exists_count = 0;
    while(fs::exists(saved_path + "/" + file_name + (file_exists_count > 0 ? "(" + to_string(file_exists_count) + ")" : "") + (item->file_extension != "" ? "."+item->file_extension : ""))) {
        file_exists_count++;
    }
    return saved_path + "/" + file_name + (file_exists_count > 0 ? "(" + to_string(file_exists_count) + ")" : "") + (item->file_extension != "" ? "."+item->file_extension : "");
}

void Account::save_to_file(json data) {
    if(fs::exists("files_cache/"+session_file_name)) {
        json list_files;
        ifstream file("files_cache/"+session_file_name);
        file >> list_files;
        file.close();
        if(service == "Google")
            list_files["files"].push_back(data);
        else if(service == "Yandex")
            list_files.push_back(data);
        ofstream ofile("files_cache/"+session_file_name);
        ofile << list_files.dump();
        ofile.close();
    }
}

shared_ptr<Item> Account::find_item_by_id(string item_id) {
    return find_item_by_id_req(item_id, files);
}

shared_ptr<Item> Account::find_item_by_id_req(string& item_id, shared_ptr<Item> root) {
    if(root->id == item_id) return root;
    for(shared_ptr<Item>& item : root->items) {
        shared_ptr<Item> cur_item = find_item_by_id_req(item_id, item);
        if(cur_item != nullptr) return cur_item;
    }
    return nullptr;
}