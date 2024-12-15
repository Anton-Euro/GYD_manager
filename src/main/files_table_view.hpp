#ifndef FILES_TABLE_VIEW_HPP
#define FILES_TABLE_VIEW_HPP

#include "itemlist.hpp"
#include "../utils/tools.hpp"
#include "../utils/datetime.hpp"
#include <QModelIndex>
#include <QAbstractTableModel>
#include <QString>
#include <algorithm>
#include <QIcon>
#include <QPixmap>

class QTableViewFilesModel: public QAbstractTableModel
{
public:
    QTableViewFilesModel(std::shared_ptr<ItemList> listptr, QObject *parent=nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void refresh();
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
    std::shared_ptr<ItemList> itemlist;

};

#endif // FILES_TABLE_VIEW_HPP