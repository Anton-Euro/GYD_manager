#ifndef ACCOUNT_TABLE_VIEW_H
#define ACCOUNT_TABLE_VIEW_H

#include <QModelIndex>
#include <QAbstractTableModel>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QPainter>
#include <QApplication>
#include <vector>
#include <filesystem>
#include "../main/account.hpp"
#include "../utils/tools.hpp"

namespace fs = std::filesystem;

class ButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void buttonClicked(const QModelIndex& index) const;
};


class QTableViewAccountsModel: public QAbstractTableModel {
    friend class Settings;
public:
    QTableViewAccountsModel(std::shared_ptr<std::vector<std::shared_ptr<Account>>> accounts, QObject *parent=nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    void refresh();

private:
    std::shared_ptr<std::vector<std::shared_ptr<Account>>> accounts;
};

#endif // ACCOUNT_TABLE_VIEW_H