#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), itemlist(make_shared<ItemList>()), history_index(0), network_manager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);
    QAction* iconAction = new QAction(QIcon(":drive/static/search.png"), "");
    ui->search_line->addAction(iconAction, QLineEdit::LeadingPosition);

    model_table = new QTableViewFilesModel(itemlist);
    ui->files_view->setModel(model_table);
    ui->files_view->setDragEnabled(true);
    ui->files_view->setAcceptDrops(true);
    history.push_back(itemlist->current_dir);

    ui->files_view->setColumnWidth(0, 280);
    ui->files_view->setColumnWidth(1, 110);
    ui->tasks_table->setColumnWidth(0, 150);
    ui->tasks_table->setColumnWidth(4, 130);
    ui->left_menu->installEventFilter(this);
    ui->files_view->installEventFilter(this);
    ui->left_menu->setItemDelegate(new FocusDelegate(ui->left_menu));
    ui->files_view->setItemDelegate(new FocusDelegate(ui->files_view));
    ui->tasks_table->setItemDelegate(new FocusDelegate(ui->tasks_table));
    
    ui->files_view->setContextMenuPolicy(Qt::CustomContextMenu);

    init_left_menu(ui->left_menu->topLevelItem(1), itemlist->root);
    check_storage_label();
    init_settings();

    connect(ui->files_view, &QTableView::doubleClicked, this, &MainWindow::double_click_to_dir);
    connect(ui->back, &QPushButton::clicked, this, &MainWindow::go_back);
    connect(ui->forward, &QPushButton::clicked, this, &MainWindow::go_forward);
    connect(ui->refresh, &QPushButton::clicked, this, [this]() {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not refresh in the account list");
            return;
        }
        refresh_files();
    });

    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::open_settings);
    connect(ui->upload, &QPushButton::clicked, this, &MainWindow::upload_button);

    connect(ui->left_menu, &QTreeWidget::itemClicked, this, &MainWindow::click_left_menu_item);
    connect(ui->files_view, &QTableView::customContextMenuRequested, this, &MainWindow::show_context_menu);

    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionabout, &QAction::triggered, this, &MainWindow::open_about);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::open_settings);
    connect(ui->actionCreate_folder, &QAction::triggered, this, [this]() {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not create a folder in the account list");
            return;
        }
        create_folder();
    });
    connect(ui->actionUpload, &QAction::triggered, this, [this]() {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not upload file in the account list");
            return;
        }
        upload_file();
    });
    connect(ui->actionrefresh, &QAction::triggered, this, [this]() {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not refresh in the account list");
            return;
        }
        refresh_files();
    });

    connect(ui->search_line, &QLineEdit::editingFinished, this, [this]() {
        string search_text = (ui->search_line->text()).toStdString();
        if(search_text != "") {
            itemlist->search(search_text);
            refresh_history(itemlist->current_dir);
            clear_subtree(ui->left_menu->topLevelItem(1));
            model_table->refresh();
        } else {
            if(itemlist->search_current_dir != nullptr) {
                itemlist->current_dir = itemlist->search_current_dir;
                itemlist->search_current_dir = nullptr;
            }
            finish_refresh_files();
        }
    });
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if(event->type() == QEvent::FocusOut) {
        if(object == ui->left_menu) {
            ui->left_menu->clearSelection();
        }else if(object == ui->files_view) {
            ui->files_view->clearSelection();
        }
    }
    return false;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::open_settings() {
    Settings w_settings(itemlist, download_path, this);
    w_settings.exec();
}

void MainWindow::open_about() {
    About w_about(this);
    w_about.exec();
}

void MainWindow::upload_button() {
    if(itemlist->current_dir == itemlist->root) {
        QMessageBox::warning(this, "Error", "You can not upload file in the account list");
        return;
    }
    upload_file();
}

void MainWindow::double_click_to_dir(const QModelIndex &index) {
    if(itemlist->current_dir->items[index.row()]->is_dir == true) {
        if(history_index+1 < history.size() && history[history_index+1] != itemlist->current_dir->items[index.row()])
            while(history.size()-1 != history_index) history.removeLast();

        itemlist->current_dir = itemlist->current_dir->items[index.row()];
        if(history_index+1 == history.size()) {
            history.push_back(itemlist->current_dir);
            history_index++;
        } else {
            history_index++;
        }

        model_table->refresh();

        check_storage_label();
    }
}

void MainWindow::go_back() {
    if(history_index-1 >= 0) {
        itemlist->current_dir = history[--history_index];
        model_table->refresh();
        check_storage_label();
    }
}

void MainWindow::go_forward() {
    if(history_index+1 < history.size()) {
        itemlist->current_dir = history[++history_index];
        model_table->refresh();
        check_storage_label();
    }
}

void MainWindow::click_left_menu_item(QTreeWidgetItem *item, int column) {
    if(item) {
        QVariant raw_data = item->data(0, Qt::UserRole);
        if(raw_data.isValid()) {
            ui->content_widget->setCurrentIndex(1);
            shared_ptr<Item> cur_item = raw_data.value<std::shared_ptr<Item>>();
            refresh_history(cur_item);
            model_table->refresh();
        } else if(item->text(column) == "Tasks") {
            ui->content_widget->setCurrentIndex(0);
        } else if(item->text(column) == "Drive") {
            ui->content_widget->setCurrentIndex(1);
            history.clear();
            history.push_back(itemlist->root);
            history_index = 0;
            itemlist->current_dir = itemlist->root;
            model_table->refresh();
        }
        check_storage_label();
    }
}

void MainWindow::init_left_menu(QTreeWidgetItem *root, shared_ptr<Item> root_item) {
    for(shared_ptr<Item> &item : root_item->items) {
        QTreeWidgetItem *new_tree_item = new QTreeWidgetItem(root);
        new_tree_item->setData(0, Qt::UserRole, QVariant::fromValue(item));
        if(item->file_extension != "")
            new_tree_item->setText(0, QString::fromStdString(item->name+"."+item->file_extension));
        else
            new_tree_item->setText(0, QString::fromStdString(item->name));
        if(item->parent->id == "drive" && item->mime_type == "Google") {
            new_tree_item->setIcon(0, QIcon(QPixmap(":drive/static/folder-google.png")));
            init_left_menu(new_tree_item, item);
        } else if(item->parent->id == "drive" && item->mime_type == "Yandex") {
            new_tree_item->setIcon(0, QIcon(QPixmap(":drive/static/folder-yandex.png")));
            init_left_menu(new_tree_item, item);
        } else if(item->parent->id == "drive" && item->mime_type == "Dropbox") {
            new_tree_item->setIcon(0, QIcon(QPixmap(":drive/static/folder-dropbox.png")));
            init_left_menu(new_tree_item, item);
        } else if(item->is_dir == true) {
            new_tree_item->setIcon(0, QIcon(QPixmap(":drive/static/folder.png")));
            init_left_menu(new_tree_item, item);
        } else {
            new_tree_item->setIcon(0, QIcon(QPixmap(":drive/static/file.png")));
        }
    }
}

void MainWindow::clear_subtree(QTreeWidgetItem *parent) {
    if (!parent) return;
    while (parent->childCount() > 0) {
        QTreeWidgetItem *child = parent->takeChild(0);
        delete child;
    }
}

void MainWindow::show_context_menu(const QPoint& pos) {
    QModelIndex index = ui->files_view->indexAt(pos);
    QMenu contextMenu(this);
    QAction* action1 = contextMenu.addAction("New folder");
    QAction* action2 = contextMenu.addAction("Upload");
    QAction* action3;
    QAction* action4;

    if(index.isValid()) {
        contextMenu.addSeparator();
        action3 = contextMenu.addAction("Download");
        action4 = contextMenu.addAction("Delete");
    }

    QAction* selectedAction = contextMenu.exec(ui->files_view->viewport()->mapToGlobal(pos));
    if (selectedAction == action1) {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not create a folder in the account list");
            return;
        }
        create_folder();
    } else if(selectedAction == action2) {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not upload file in the account list");
            return;
        }
        upload_file();
    } else if(selectedAction == action3) {
        if(itemlist->current_dir->items[index.row()]->is_dir == true) {
            QMessageBox::warning(this, "Error", "You can not download the folder. This feature is coming soon!");
            return;
        }
        download_file(itemlist->current_dir->items[index.row()]);
    } else if(selectedAction == action4) {
        if(itemlist->current_dir == itemlist->root) {
            QMessageBox::warning(this, "Error", "You can not delete the account folder");
            return;
        }
        itemlist->get_account_by_item(itemlist->current_dir->items[index.row()])->session->move_to_trash(itemlist->current_dir->items[index.row()]->id);
        itemlist->current_dir->items.erase(itemlist->current_dir->items.begin() + index.row());
        clear_subtree(ui->left_menu->topLevelItem(1));
        init_left_menu(ui->left_menu->topLevelItem(1), itemlist->root);
        model_table->refresh();

    }
}

void MainWindow::create_folder() {
    itemlist->current_dir->add_item(
        "temp_id",
        "New folder",
        "",
        0,
        "",
        get_now(),
        get_now(),
        true,
        false,
        false,
        itemlist->current_dir
    );
    model_table->refresh();
    QModelIndex new_folder_index = ui->files_view->model()->index(itemlist->current_dir->items.size()-1, 0);
    ui->files_view->scrollToBottom();
    ui->files_view->edit(new_folder_index);
}

void MainWindow::check_storage_label() {
    if(itemlist->current_dir != itemlist->root) {
        shared_ptr<Item> iter_item = itemlist->current_dir;
        while(iter_item->parent != itemlist->root)
            iter_item = iter_item->parent;
        for(shared_ptr<Account> &acc : *(itemlist->accounts)) {
            if(iter_item == acc->files) {
                ui->storage_status_label->setText(QString::fromStdString(
                    "Used " + size_to_print(acc->used_size) + " from " + size_to_print(acc->storage_size)
                ));

                ui->storage_status_progressbar->setValue((static_cast<double>(acc->used_size) / static_cast<double>(acc->storage_size)) * 100);
                break;
            }
        }
    } else {
        unsigned long long used_size = itemlist->get_used_storage_size();
        unsigned long long storage_size = itemlist->get_all_storage_size();
        
        ui->storage_status_label->setText(QString::fromStdString(
            "Used " + size_to_print(used_size) + " from " + size_to_print(storage_size)
        ));

        ui->storage_status_progressbar->setValue((static_cast<double>(used_size) / static_cast<double>(storage_size)) * 100);
    }
}

void MainWindow::init_settings() {
    json data;
    if(!fs::exists("settings.json")) {
        data = {{"download_path", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString()}};
        ofstream file("settings.json");
        file << data;
        file.close();
    }
    ifstream file("settings.json");
    file >> data;
    file.close();
    *download_path = data["download_path"].get<string>();
}

void MainWindow::download_file(shared_ptr<Item> item) {
    shared_ptr<Account> acc = itemlist->get_account_by_item(item);

    string download_url;
    if(starts_with(item->mime_type, "application/vnd.google-apps") && (ends_with(item->mime_type, "document") || ends_with(item->mime_type, "sheet") || ends_with(item->mime_type, "presentation"))) {
        download_url = acc->session->get_export_google_document_url(item->id, item->mime_type);
    } else {
        download_url = acc->session->get_download_url(item->id);
    }

    int row_count = ui->tasks_table->rowCount();
    ui->tasks_table->insertRow(row_count);
    QTableWidgetItem *table_item;

    table_item = new QTableWidgetItem(QString::fromStdString(item->file_extension == "" ? item->name : item->name+"."+item->file_extension));
    table_item->setIcon(QIcon(QPixmap(":drive/static/download_status.png")));
    ui->tasks_table->setItem(row_count, 0, table_item);
    table_item = new QTableWidgetItem(QString::fromStdString(size_to_print(item->size)));
    ui->tasks_table->setItem(row_count, 1, table_item);
    table_item = new QTableWidgetItem(QString("--"));
    ui->tasks_table->setItem(row_count, 2, table_item);
    table_item = new QTableWidgetItem(QString("Starting"));
    ui->tasks_table->setItem(row_count, 3, table_item);
    QProgressBar *progress_bar = new QProgressBar;
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    ui->tasks_table->setCellWidget(row_count, 4, progress_bar);
    table_item = new QTableWidgetItem(QString("--"));
    ui->tasks_table->setItem(row_count, 5, table_item);

    QUrl url(QString::fromStdString(download_url));
    QNetworkRequest *request = new QNetworkRequest(url);
    request->setRawHeader(QByteArray::fromStdString("Authorization"), QByteArray::fromStdString("Bearer " + acc->session->get_access_token()));

    const int chunk_size = 10 * 1024 * 1024;
    shared_ptr<unsigned long long> downloaded = make_shared<unsigned long long>(0);
    QFile *file = new QFile(QString::fromStdString(acc->get_saved_path(*download_path, item)+".download"));
    file->open(QIODevice::WriteOnly);

    shared_ptr<QElapsedTimer> timer = make_shared<QElapsedTimer>();
    timer->start();

    shared_ptr<qint64> last_time = make_shared<qint64>(timer->elapsed());

    download_chunk(request, file, 0, chunk_size, row_count, progress_bar, downloaded, item, timer, last_time);

}

void MainWindow::download_chunk(QNetworkRequest *request, QFile *file, unsigned long long startByte, qint64 chunkSize, int row_count, QProgressBar *progress_bar, shared_ptr<unsigned long long> downloaded, shared_ptr<Item> item, shared_ptr<QElapsedTimer> timer, shared_ptr<qint64> last_time) {
    request->setRawHeader("Range", "bytes=" + QByteArray::number(startByte) + "-" + QByteArray::number(startByte + chunkSize - 1));
    
    QNetworkReply *reply = network_manager->get(*request);
    connect(reply, &QNetworkReply::readyRead, this, [=]() {
        QByteArray data = reply->readAll();
        file->write(data);
        (*downloaded) += data.size();

        update_download_progress(*downloaded, item->size, row_count, progress_bar, timer, last_time);
    });

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (*downloaded < item->size) {
            download_chunk(request, file, *downloaded, chunkSize, row_count, progress_bar, downloaded, item, timer, last_time);
        } else {
            handle_download_finished(row_count, reply, progress_bar, timer, downloaded, last_time, file);
        }
    });
}

void MainWindow::update_download_progress(qint64 bytesReceived, qint64 bytesTotal, int row, QProgressBar *progress_bar, shared_ptr<QElapsedTimer> timer, shared_ptr<qint64> last_time) {
    if(bytesTotal > 0) {
        qint64 elapsedTime = timer->elapsed();
        if (elapsedTime - *last_time >= 1000) {
            double val = static_cast<double>(bytesReceived) / static_cast<double>(bytesTotal) * 100.0;
            progress_bar->setValue(static_cast<int>(round(val)));
            QTableWidgetItem *item;
            qint64 bytesPerSecond = bytesReceived * 1000 / elapsedTime;
            item = ui->tasks_table->item(row, 2);
            item->setText(QString::fromStdString(size_to_print(static_cast<unsigned long long>(bytesPerSecond), "/s")));
            item = ui->tasks_table->item(row, 3);
            item->setText(QString("Downloading"));

            qint64 remainingBytes = bytesTotal - bytesReceived;
            double timeLeft = remainingBytes / static_cast<double>(bytesPerSecond);
            item = ui->tasks_table->item(row, 5);
            item->setText(QString::fromStdString(seconds_to_timeprint(static_cast<unsigned long long>(round(timeLeft)))));
            (*last_time) = elapsedTime;
        }
    }
}

void MainWindow::handle_download_finished(int row, QNetworkReply *reply, QProgressBar *progress_bar, shared_ptr<QElapsedTimer> timer, shared_ptr<unsigned long long> downloaded, shared_ptr<qint64> last_time, QFile *file) {
    QTableWidgetItem *item;
    if (reply->error() == QNetworkReply::NoError) {
        item = ui->tasks_table->item(row, 3);
        item->setText("Completed");
        QString file_name = file->fileName();
        file_name.chop(9);
        file->rename(file_name);
        file->close();
    } else {
        file->remove();
        file->close();
        item = ui->tasks_table->item(row, 3);
        item->setText("Error");
    }
    item = ui->tasks_table->item(row, 2);
    item->setText(" ");
    item = ui->tasks_table->item(row, 5);
    item->setText(" ");
    progress_bar->setValue(100);

    reply->deleteLater();
    delete file;
}

void MainWindow::upload_file() {
    string file_path = QFileDialog::getOpenFileName(this, "Select file").toStdString();

    if(file_path.empty()) return;
    if(itemlist->current_dir == itemlist->root) return;
    string file_name = fs::path(file_path).filename().string();
    for(shared_ptr<Item> &pitem : itemlist->current_dir->items) {
        if((pitem->file_extension != "" ? pitem->name+"."+pitem->file_extension : pitem->name) == file_name) {
            QMessageBox::warning(this, "Error", "File already exists!");
            return;
        }
    }

    int row_count = ui->tasks_table->rowCount();
    ui->tasks_table->insertRow(row_count);
    QTableWidgetItem *table_item;

    table_item = new QTableWidgetItem(QString::fromStdString(file_name));
    table_item->setIcon(QIcon(QPixmap(":drive/static/upload_status.png")));
    ui->tasks_table->setItem(row_count, 0, table_item);
    table_item = new QTableWidgetItem(QString::fromStdString(size_to_print(fs::file_size(file_path))));
    ui->tasks_table->setItem(row_count, 1, table_item);
    QTableWidgetItem *speed_cell = new QTableWidgetItem(QString("--"));
    ui->tasks_table->setItem(row_count, 2, speed_cell);
    QTableWidgetItem *status_cell = new QTableWidgetItem(QString("Starting"));
    ui->tasks_table->setItem(row_count, 3, status_cell);
    QProgressBar *progress_bar = new QProgressBar;
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    ui->tasks_table->setCellWidget(row_count, 4, progress_bar);
    QTableWidgetItem *ETA_cell = new QTableWidgetItem(QString("--"));
    ui->tasks_table->setItem(row_count, 5, ETA_cell);

    QFutureWatcher<void> watcher;
    QFuture<void> future = QtConcurrent::run([=]() {
        thread_upload_file(file_path, speed_cell, status_cell, ETA_cell, progress_bar);
    });
    watcher.setFuture(future);
}

void MainWindow::thread_upload_file(string file_path, QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar) {
    shared_ptr<Account> acc = itemlist->get_account_by_item(itemlist->current_dir);
    string file_name = fs::path(file_path).filename().string();
    string upload_uri;

    if(acc->service == "Google") {
        upload_uri = acc->session->init_upload_file(file_name, itemlist->current_dir->id);
    } else if(acc->service == "Yandex") {
        upload_uri = acc->session->init_upload_file_yandex(itemlist->current_dir->id + file_name);
    }
    

    ifstream file(file_path, ios::binary);
    file.seekg(0, ios::end);
    unsigned long long file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    unsigned int chunk_size = 3 * 1024 * 1024;
    vector<char> buffer(chunk_size);
    unsigned long long bytes_sent = 0;
    QElapsedTimer timer;
    timer.start();
    qint64 last_time = timer.elapsed();
    unsigned long long last_bytes = 0;

    while(bytes_sent < file_size) {
        unsigned long long to_read = std::min(static_cast<unsigned long long>(chunk_size), file_size - bytes_sent);
        file.read(buffer.data(), to_read);

        acc->session->send_upload_part(
            upload_uri,
            to_read,
            bytes_sent,
            file_size,
            buffer,
            chunk_size
        );

        bytes_sent += to_read;

        double val = static_cast<double>(bytes_sent) / static_cast<double>(file_size) * 100.0;
        int progress_val = static_cast<int>(round(val));

        qint64 current_time = timer.elapsed();
        qint64 elapsed_time = current_time - last_time;
        if (elapsed_time >= 1000) {
            double speed = (bytes_sent - last_bytes) / (elapsed_time / 1000.0);

            double remaining_time = (file_size - bytes_sent) / speed;

            emit update_upload_progress(speed, remaining_time, progress_val, speed_cell, status_cell, ETA_cell, progress_bar);

            last_time = current_time;
            last_bytes = bytes_sent;
        }
    }

    emit handle_upload_finished(speed_cell, status_cell, ETA_cell, progress_bar);

}

void MainWindow::update_upload_progress(double speed, double remaining_time, int progress_val, QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar) {
    progress_bar->setValue(progress_val);
    speed_cell->setText(QString::fromStdString(size_to_print(static_cast<unsigned long long>(round(speed)), "/s")));
    ETA_cell->setText(QString::fromStdString(seconds_to_timeprint(static_cast<unsigned long long>(round(remaining_time)))));
    status_cell->setText("Uploading");
}
    
void MainWindow::handle_upload_finished(QTableWidgetItem *speed_cell, QTableWidgetItem *status_cell, QTableWidgetItem *ETA_cell, QProgressBar *progress_bar) {
    speed_cell->setText(" ");
    ETA_cell->setText(" ");
    status_cell->setText("Completed");
    progress_bar->setValue(100);
}

void MainWindow::refresh_history(shared_ptr<Item> cur_item) {
    history.clear();
    history_index = -1;
    shared_ptr<Item> iter_item;
    if(cur_item->is_dir == true)
        iter_item = cur_item;
    else
        iter_item = cur_item->parent;
    while(iter_item != nullptr) {
        history.push_back(iter_item);
        history_index++;
        iter_item = iter_item->parent;
    }
    reverse(history.begin(), history.end());
    
    if(cur_item->is_dir == true)
        itemlist->current_dir = cur_item;
    else
        itemlist->current_dir = cur_item->parent;
}

void MainWindow::thread_refresh_files() {
    shared_ptr<Account> acc = itemlist->get_account_by_item(itemlist->current_dir);
    string cur_dir_id = itemlist->current_dir->id;
    itemlist->current_dir = itemlist->root;
    acc->files->items.clear();
    acc->init_files();
    itemlist->current_dir = acc->find_item_by_id(cur_dir_id);
    QMetaObject::invokeMethod(this, "finish_refresh_files", Qt::QueuedConnection);
}

void MainWindow::finish_refresh_files() {
    refresh_history(itemlist->current_dir);
    clear_subtree(ui->left_menu->topLevelItem(1));
    init_left_menu(ui->left_menu->topLevelItem(1), itemlist->root);
    model_table->refresh();
    ui->refresh->setDisabled(false);
    ui->actionrefresh->setDisabled(false);
}

void MainWindow::refresh_files() {
    ui->refresh->setDisabled(true);
    ui->actionrefresh->setDisabled(true);
    QFutureWatcher<void> watcher;
    QFuture<void> future = QtConcurrent::run([=]() {
        thread_refresh_files();
    });
    watcher.setFuture(future);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        const QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url : urls) {
            QFileInfo fileInfo(url.toLocalFile());
            if (fileInfo.exists()) {
                if(itemlist->current_dir == itemlist->root) {
                    QMessageBox::warning(this, "Error", "You can not upload file in the account list");
                    return;
                }
                string file_path = fileInfo.filePath().toStdString();
                for(shared_ptr<Item> &pitem : itemlist->current_dir->items) {
                    if((pitem->file_extension != "" ? pitem->name+"."+pitem->file_extension : pitem->name) == fileInfo.fileName().toStdString()) {
                        QMessageBox::warning(this, "Error", "File already exists!");
                        return;
                    }
                }
                int row_count = ui->tasks_table->rowCount();
                ui->tasks_table->insertRow(row_count);
                QTableWidgetItem *table_item;

                table_item = new QTableWidgetItem(QString::fromStdString(fs::path(file_path).filename().string()));
                table_item->setIcon(QIcon(QPixmap(":drive/static/upload_status.png")));
                ui->tasks_table->setItem(row_count, 0, table_item);
                table_item = new QTableWidgetItem(QString::fromStdString(size_to_print(fs::file_size(file_path))));
                ui->tasks_table->setItem(row_count, 1, table_item);
                QTableWidgetItem *speed_cell = new QTableWidgetItem(QString("--"));
                ui->tasks_table->setItem(row_count, 2, speed_cell);
                QTableWidgetItem *status_cell = new QTableWidgetItem(QString("Starting"));
                ui->tasks_table->setItem(row_count, 3, status_cell);
                QProgressBar *progress_bar = new QProgressBar;
                progress_bar->setRange(0, 100);
                progress_bar->setValue(0);
                ui->tasks_table->setCellWidget(row_count, 4, progress_bar);
                QTableWidgetItem *ETA_cell = new QTableWidgetItem(QString("--"));
                ui->tasks_table->setItem(row_count, 5, ETA_cell);

                QFutureWatcher<void> watcher;
                QFuture<void> future = QtConcurrent::run([=]() {
                    thread_upload_file(file_path, speed_cell, status_cell, ETA_cell, progress_bar);
                });
                watcher.setFuture(future);
            }
        }
    }
}