#include "itemlist.hpp"

using namespace std;


ItemList::ItemList(): accounts(make_shared<vector<shared_ptr<Account>>>()), search_current_dir(nullptr) {
    init();
}

void ItemList::init() {
    if(!fs::exists("sessions/")) {
        fs::create_directory("sessions/");
    }
    for(const auto &entry : fs::directory_iterator("sessions/")) {
        if (fs::is_regular_file(entry)) {
            try {
                accounts->push_back(make_shared<Account>(entry.path().string()));
            } catch(...) {
                cout << "bad account" << endl;
            }
        }
    }
    root = make_shared<Item>("drive", "Drive", "", 0, "", get_now(), get_now(), true, false, false, nullptr);
    
    for(int i = 0; i < accounts->size(); i++) {
        (*accounts)[i]->init_files_from_file();
        root->add_item_from_ptr((*accounts)[i]->files);
        (*accounts)[i]->files->parent = root;
    }
    current_dir = root;
}

unsigned long long ItemList::get_all_storage_size() {
    unsigned long long storage_size = 0;
    for(int i = 0; i < accounts->size(); i++) {
        storage_size += (*accounts)[i]->storage_size;
    }
    return storage_size;
}


unsigned long long ItemList::get_used_storage_size() {
    unsigned long long storage_size = 0;
    for(int i = 0; i < accounts->size(); i++) {
        storage_size += (*accounts)[i]->used_size;
    }
    return storage_size;
}

shared_ptr<Account> ItemList::get_account_by_item(shared_ptr<Item> item) {
    shared_ptr<Item> iter_item = item;
    while(iter_item->parent != root) {
        iter_item = iter_item->parent;
    }
    for(shared_ptr<Account> &acc : (*accounts)) {
        if(acc->files == iter_item)
            return acc;
    }
    return nullptr;
}

void ItemList::search(string search_text) {
    shared_ptr<Item> searched_items = make_shared<Item>("search", "Search", "", 0, "", get_now(), get_now(), true, false, false, nullptr);
    
    if(search_current_dir == nullptr) {
        search_req(search_text, current_dir, searched_items);
        search_current_dir = current_dir;
    } else {
        search_req(search_text, search_current_dir, searched_items);
    }
        
    current_dir = searched_items;
}

void ItemList::search_req(string search_text, shared_ptr<Item> root_ptr, shared_ptr<Item> search_ptr) {
    if(root_ptr->is_dir == false && starts_with(to_lower_case(root_ptr->file_extension != "" ? root_ptr->name+"."+root_ptr->file_extension : root_ptr->name), to_lower_case(search_text))) {
        search_ptr->add_item_from_ptr(root_ptr);
    }
    for(shared_ptr<Item> &pitem : root_ptr->items) {
        search_req(search_text, pitem, search_ptr);
    }
}