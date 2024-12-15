#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QMenu>
#include <QStandardPaths>
#include <QMessageBox>
#include <algorithm>
#include <QMessageBox>
#include <QAction>
#include <QIcon>
#include <QProgressBar>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QFile>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <cmath>
#include "../settings/settingswindow.h"
#include "../about/aboutwindow.h"
#include "files_table_view.hpp"
#include "../utils/focus_delegate.hpp"
#include "../utils/tools.hpp"
#include <QDropEvent>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void double_click_to_dir(const QModelIndex &index);
    void go_back();
    void go_forward();

    void open_settings();
    void open_about();
    void upload_button();

    void click_left_menu_item(QTreeWidgetItem *item, int column);
    void show_context_menu(const QPoint& pos);

    void update_download_progress(qint64 bytesReceived, qint64 bytesTotal, int row, QProgressBar *progress_bar, std::shared_ptr<QElapsedTimer> timer, std::shared_ptr<qint64> last_time);
    void handle_download_finished(int row, QNetworkReply *reply, QProgressBar *progress_bar, std::shared_ptr<QElapsedTimer> timer, std::shared_ptr<unsigned long long> downloaded, std::shared_ptr<qint64> last_time, QFile *file);
    void update_upload_progress(double speed, double remaining_time, int progress_val, QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar);
    void handle_upload_finished(QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar);
    void finish_refresh_files();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ItemList> itemlist;
    QTableViewFilesModel *model_table;
    QVector<std::shared_ptr<Item>> history;
    int history_index;
    std::shared_ptr<std::string> download_path = std::make_shared<std::string>();
    QNetworkAccessManager *network_manager;
    

    bool eventFilter(QObject *object, QEvent *event) override;
    void init_left_menu(QTreeWidgetItem *root, std::shared_ptr<Item> root_item);
    void check_storage_label();
    void create_folder();
    void init_settings();
    void download_file(std::shared_ptr<Item> item);
    void download_chunk(QNetworkRequest *request, QFile *file, unsigned long long startByte, qint64 chunkSize, int row_count, QProgressBar *progress_bar, std::shared_ptr<unsigned long long> downloaded, std::shared_ptr<Item> item, std::shared_ptr<QElapsedTimer> timer, std::shared_ptr<qint64> last_time);
    void upload_file();
    void thread_upload_file(std::string file_path, QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar);
    void thread_refresh_files();
    void refresh_history(std::shared_ptr<Item> cur_item);
    void clear_subtree(QTreeWidgetItem *parent);
    void refresh_files();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // MAINWINDOW_H