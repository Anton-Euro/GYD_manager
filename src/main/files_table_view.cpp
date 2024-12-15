#include "files_table_view.hpp"

using namespace std;

QTableViewFilesModel::QTableViewFilesModel(shared_ptr<ItemList> listptr, QObject *parent): itemlist(listptr), QAbstractTableModel(parent) {}

void QTableViewFilesModel::refresh() {
    beginResetModel();
    endResetModel();
} 


int QTableViewFilesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return itemlist->current_dir->items.size();
}

int QTableViewFilesModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

QVariant QTableViewFilesModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() >= itemlist->current_dir->items.size()) {
        return QVariant();
    }
    if (role == Qt::CheckStateRole || role == Qt::DecorationRole) {
        if(index.column() == 0 && role == Qt::DecorationRole) {
            if(itemlist->current_dir->id == "drive" && itemlist->current_dir->items[index.row()]->mime_type == "Google") {
                return QIcon(QPixmap(":drive/static/folder-google.png"));
            } else if(itemlist->current_dir->id == "drive" && itemlist->current_dir->items[index.row()]->mime_type == "Yandex") {
                return QIcon(QPixmap(":drive/static/folder-yandex.png"));
            } else if(itemlist->current_dir->id == "drive" && itemlist->current_dir->items[index.row()]->mime_type == "Dropbox") {
                return QIcon(QPixmap(":drive/static/folder-dropbox.png"));
            } else if(itemlist->current_dir->items[index.row()]->is_dir == true) {
                return QIcon(QPixmap(":drive/static/folder.png"));
            } else {
                return QIcon(QPixmap(":drive/static/file.png"));
            }
            
        }
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignVCenter;
    }

    std::shared_ptr<Item> cur_item = itemlist->current_dir->items[index.row()];

    switch(index.column()) {
        case 0: {
            if(cur_item->file_extension != "") {
                return QString::fromStdString(cur_item->name+"."+cur_item->file_extension);
            } else {
                return QString::fromStdString(cur_item->name);
            }
            
        }
        case 1: {
            if(cur_item->is_dir == true) {
                return QString::fromStdString("--");
            } else {
                return QString::fromStdString(size_to_print(cur_item->size));
            }
        }
        case 2: return QString::fromStdString(timepoint_to_string(cur_item->created_time));
        case 3: return QString::fromStdString(timepoint_to_string(cur_item->modified_time));
        default: return QVariant();
    }
}

QVariant QTableViewFilesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    
    switch(section) {
        case 0: return QStringLiteral("Name");
        case 1: return QStringLiteral("Size");
        case 2: return QStringLiteral("Date Created");
        case 3: return QStringLiteral("Date Modified");
        default: return QVariant();
    }
}

void QTableViewFilesModel::sort(int column, Qt::SortOrder order) {
    if(column == 0) {
        std::sort(itemlist->current_dir->items.begin(), itemlist->current_dir->items.end(), [order](const shared_ptr<Item>& a, const shared_ptr<Item>& b) {
            return order == Qt::AscendingOrder ? a->name < b->name : a->name > b->name;
        });
    } else if(column == 1) {
        std::sort(itemlist->current_dir->items.begin(), itemlist->current_dir->items.end(), [order](const shared_ptr<Item>& a, const shared_ptr<Item>& b) {
            return order == Qt::AscendingOrder ? a->size < b->size : a->size > b->size;
        });
    } else if(column == 2) {
        std::sort(itemlist->current_dir->items.begin(), itemlist->current_dir->items.end(), [order](const shared_ptr<Item>& a, const shared_ptr<Item>& b) {
            return order == Qt::AscendingOrder ? a->created_time < b->created_time : a->created_time > b->created_time;
        });
    } else if(column == 3) {
        std::sort(itemlist->current_dir->items.begin(), itemlist->current_dir->items.end(), [order](const shared_ptr<Item>& a, const shared_ptr<Item>& b) {
            return order == Qt::AscendingOrder ? a->modified_time < b->modified_time : a->modified_time > b->modified_time;
        });
    }
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

Qt::ItemFlags QTableViewFilesModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.column() == 0) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
bool QTableViewFilesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    if (role == Qt::EditRole) {
        string folder_name = value.toString().toStdString();
        shared_ptr<Account> acc = itemlist->get_account_by_item(itemlist->current_dir->items[index.row()]);
        if(acc->service == "Google") {
            json data = acc->session->create_folder(folder_name, itemlist->current_dir->id);
            acc->save_to_file(data);
            itemlist->current_dir->items[index.row()]->id = data["id"];
            itemlist->current_dir->items[index.row()]->name = data["name"];
            itemlist->current_dir->items[index.row()]->mime_type = data["mimeType"];
            itemlist->current_dir->items[index.row()]->created_time = iso8601string_to_timepoint(data["createdTime"]);
            itemlist->current_dir->items[index.row()]->modified_time = iso8601string_to_timepoint(data["modifiedTime"]);
            itemlist->current_dir->items[index.row()]->shared = data["shared"].get<bool>();
            itemlist->current_dir->items[index.row()]->trashed = data["trashed"].get<bool>();
        } else if(acc->service == "Yandex") {
            json data = acc->session->create_folder_yandex(itemlist->current_dir->id + folder_name);
            if(data.contains("error")) return false;
            acc->save_to_file(data);
            itemlist->current_dir->items[index.row()]->id = data["path"];
            itemlist->current_dir->items[index.row()]->name = data["name"];
            itemlist->current_dir->items[index.row()]->mime_type = "yandex/folder";
            itemlist->current_dir->items[index.row()]->created_time = iso8601string_to_timepoint(data["created"]);
            itemlist->current_dir->items[index.row()]->modified_time = iso8601string_to_timepoint(data["modified"]);
            itemlist->current_dir->items[index.row()]->shared = false;
            itemlist->current_dir->items[index.row()]->trashed = false;
        }
        emit dataChanged(index, index);
        return true;
        
    }

    return false;
}