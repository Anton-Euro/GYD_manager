#ifndef ITEMLIST_HPP
#define ITEMLIST_HPP

#include "item.hpp"
#include "account.hpp"
#include <memory>

class ItemList {
    friend class QTableViewFilesModel;
    friend class QTreeViewMenuModel;
    friend class Settings;
    friend class MainWindow;
public:
    ItemList();

    unsigned long long get_all_storage_size();
    unsigned long long get_used_storage_size();
    std::shared_ptr<Account> get_account_by_item(std::shared_ptr<Item> item);
    void search(std::string search_text);

private:
    std::shared_ptr<std::vector<std::shared_ptr<Account>>> accounts;
    std::shared_ptr<Item> root;
    std::shared_ptr<Item> current_dir;
    std::shared_ptr<Item> search_current_dir;
    void search_req(std::string search_text, std::shared_ptr<Item> root_ptr, std::shared_ptr<Item> search_ptr);
};

#endif // ITEMLIST_HPP