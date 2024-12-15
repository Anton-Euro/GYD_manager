#include "item.hpp"

using namespace std;


Item::Item(const std::string &id,
    const std::string &name,
    const std::string &file_extension,
    unsigned long long size,
    const std::string &mime_type,
    datetime::time_point created_time,
    datetime::time_point modified_time,
    bool is_dir,
    bool shared,
    bool trashed,
    shared_ptr<Item> parent
): id(id), name(name), file_extension(file_extension), size(size), mime_type(mime_type), created_time(created_time), modified_time(modified_time), is_dir(is_dir), shared(shared), trashed(trashed), parent(parent) {}

shared_ptr<Item> Item::add_item(
    const std::string &id,
    const std::string &name,
    const std::string &file_extension,
    unsigned long long size,
    const std::string &mime_type,
    datetime::time_point created_time,
    datetime::time_point modified_time,
    bool is_dir,
    bool shared,
    bool trashed,
    shared_ptr<Item> parent
) {
    shared_ptr<Item> new_item = make_shared<Item>(id, name, file_extension, size, mime_type, created_time, modified_time, is_dir, shared, trashed, parent);
    items.push_back(new_item);
    return new_item;
}

void Item::add_item_from_ptr(shared_ptr<Item> item_ptr) {
    items.push_back(item_ptr);
}

bool Item::contains(shared_ptr<Item> item_ptr) {
    for(shared_ptr<Item> itemp : items) {
        if(itemp == item_ptr) return true;
    }
    return false;
}