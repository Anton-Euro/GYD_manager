#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <vector>
#include <memory>
#include "../utils/datetime.hpp"

namespace fs = std::filesystem;


class Item {
    // friend class ItemList;
    // friend class QTableViewFilesModel;
public:
    std::string id;
    std::string name;
    std::string file_extension;
    unsigned long long size;
    std::string mime_type;
    datetime::time_point created_time;
    datetime::time_point modified_time;
    bool is_dir;
    bool shared;
    bool trashed;

    std::vector<std::shared_ptr<Item>> items;
    std::shared_ptr<Item> parent;

    Item(const std::string &id,
        const std::string &name,
        const std::string &file_extension,
        unsigned long long size,
        const std::string &mime_type,
        datetime::time_point created_time,
        datetime::time_point modified_time,
        bool is_dir,
        bool shared,
        bool trashed,
        std::shared_ptr<Item> parent
    );

    std::shared_ptr<Item> add_item(const std::string &id,
        const std::string &name,
        const std::string &file_extension,
        unsigned long long size,
        const std::string &mime_type,
        datetime::time_point created_time,
        datetime::time_point modified_time,
        bool is_dir,
        bool shared,
        bool trashed,
        std::shared_ptr<Item> parent
    );

    void add_item_from_ptr(std::shared_ptr<Item> item_ptr);

    bool contains(std::shared_ptr<Item> item_ptr);
};

#endif // ITEM_HPP