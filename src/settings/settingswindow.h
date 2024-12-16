#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QFileDialog>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <QMessageBox>
#include "account_table_view.h"
#include "../main/itemlist.hpp"
#include "../utils/focus_delegate.hpp"

namespace Ui {
    class Settings;
}

class Settings : public QDialog {
    Q_OBJECT
public:
    Settings(std::shared_ptr<ItemList> itemlist, std::shared_ptr<std::string> download_path, QWidget *parent = nullptr);
    ~Settings();

private slots:
    void add_account();
    void choose_path();

signals:
    void refresh_all_signal();

private:
    std::shared_ptr<ItemList> itemlist;
    std::shared_ptr<std::string> download_path;
    QTableViewAccountsModel *model;
    Ui::Settings *ui;
};

#endif // SETTINGSWINDOW_H