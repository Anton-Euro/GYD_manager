#include "account_table_view.h"

using namespace std;


// ButtonDelegate


void ButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QPushButton button;
    button.setIcon(QIcon(":settings/static/delete.png"));
    button.setIconSize(QSize(24, 24));
    button.setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 5px;\n"
"    color: #000000;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #e6e6eb;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #f2f2f7;\n"
"    color: #8e8e93;\n"
"    border-color: #d1d1d6;\n"
"}"));
    button.resize(option.rect.size());
    
    QPixmap pixmap(button.size());
    button.render(&pixmap);
    painter->drawPixmap(option.rect.topLeft(), pixmap);
}

bool ButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (event->type() == QEvent::MouseButtonPress) {
        emit buttonClicked(index);
        return true;
    }
    return false;
}


// QTableViewAccountsModel


QTableViewAccountsModel::QTableViewAccountsModel(std::shared_ptr<std::vector<std::shared_ptr<Account>>> accounts, QObject *parent): accounts(accounts), QAbstractTableModel(parent) {}

void QTableViewAccountsModel::refresh() {
    beginResetModel();
    endResetModel();
}

int QTableViewAccountsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return accounts->size();
}

int QTableViewAccountsModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

QVariant QTableViewAccountsModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() >= accounts->size()) {
        return QVariant();
    }
    if (role == Qt::CheckStateRole || role == Qt::DecorationRole) {
        if(index.column() == 0 && role == Qt::DecorationRole) {
            if((*accounts)[index.row()]->service == "Google")
                return QIcon(QPixmap(":settings/static/google.png"));
            else if ((*accounts)[index.row()]->service == "Yandex")
                return QIcon(QPixmap(":settings/static/google.png"));
        }
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignVCenter;
    }

    switch(index.column()) {
        case 0: return QString::fromStdString((*accounts)[index.row()]->service);
        case 1: return QString::fromStdString((*accounts)[index.row()]->mail);
        case 2: return QString::fromStdString(size_to_print((*accounts)[index.row()]->used_size)+"/"+size_to_print((*accounts)[index.row()]->storage_size));
        case 3: {
            return QString::fromStdString(" ");
        }
        default: return QVariant();
    }
}

QVariant QTableViewAccountsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    
    switch (section) {
        case 0: return QStringLiteral("service");
        case 1: return QStringLiteral("mail");
        case 2: return QStringLiteral("storage");
        case 3: return QStringLiteral(" ");
        default: return QVariant();
    }
}

bool QTableViewAccountsModel::removeRow(int row, const QModelIndex &parent) {
    if (row < 0 || row >= accounts->size()) return false;

        beginRemoveRows(parent, row, row);
        fs::remove("sessions/"+(*accounts)[row]->session_file_name);
        fs::remove("files_cache/"+(*accounts)[row]->session_file_name);
        (*accounts).erase((*accounts).begin() + row);
        endRemoveRows();
        return true;
}