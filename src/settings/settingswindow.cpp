#include "settingswindow.h"
#include "../ui/ui_settings.h"

using namespace std;

Settings::Settings(shared_ptr<ItemList> itemlist, shared_ptr<string> download_path, QWidget *parent): QDialog(parent), itemlist(itemlist), download_path(download_path), ui(new Ui::Settings) {
    ui->setupUi(this);

    model = new QTableViewAccountsModel(itemlist->accounts);
    ui->acc_list_view->setModel(model);

    ui->acc_list_view->setColumnWidth(0, 100);
    ui->acc_list_view->setColumnWidth(1, 300);
    ui->acc_list_view->setColumnWidth(2, 200);
    ui->acc_list_view->setColumnWidth(3, 20);

    ui->download_path->setText(QString::fromStdString(*download_path));

    ButtonDelegate* buttonDelegate = new ButtonDelegate(this);
    ui->acc_list_view->setItemDelegateForColumn(3, buttonDelegate);
    
    ui->acc_list_view->setItemDelegate(new FocusDelegate(ui->acc_list_view));

    connect(buttonDelegate, &ButtonDelegate::buttonClicked, this, [=](const QModelIndex& index) {
        model->removeRow(index.row());
    });

    connect(ui->add, &QPushButton::clicked, this, &Settings::add_account);
    connect(ui->select_path, &QPushButton::clicked, this, &Settings::choose_path);
}

Settings::~Settings() {
    delete ui;
}


string async_wait_code(const shared_ptr<BaseAPI>& api) {
    return api->get_redirected_code();
}

void Settings::add_account() {
    int index = ui->acc_type->currentIndex();
    if(index == 1) {
        string new_file_path;
        do {
            boost::uuids::random_generator generator;
            new_file_path = "sessions/"+boost::uuids::to_string(generator()) + ".json";
        } while(fs::exists(new_file_path));
        
        shared_ptr<BaseAPI> new_acc = make_shared<GoogleAPI>(new_file_path);

        QUrl url(QString::fromStdString(new_acc->get_authorization_url()));
        QDesktopServices::openUrl(url);

        QFuture<string> future = QtConcurrent::run(async_wait_code, new_acc);

        QFutureWatcher<string>* watcher = new QFutureWatcher<string>(this);
        connect(watcher, &QFutureWatcher<std::string>::finished, [this, watcher, new_acc, new_file_path]() mutable {
            std::string code = watcher->result();
            new_acc->get_access_token_from_code(code);
            shared_ptr<Account> new_acc = make_shared<Account>(new_file_path);
            for(int i = 0; i < this->model->accounts->size(); i++) {
                if(new_acc->mail == this->model->accounts->at(i)->mail && new_acc->service == this->model->accounts->at(i)->service) {
                    fs::remove(new_file_path);
                    QMessageBox::warning(this, "Warning", "This account has already been added!");
                    watcher->deleteLater();
                    return;
                }
            }
            this->model->accounts->push_back(new_acc);
            this->model->refresh();

            watcher->deleteLater();
        });

        watcher->setFuture(future);
    } else if(index == 2) {
        string new_file_path;
        do {
            boost::uuids::random_generator generator;
            new_file_path = "sessions/"+boost::uuids::to_string(generator()) + ".json";
        } while(fs::exists(new_file_path));
        
        shared_ptr<BaseAPI> new_acc = make_shared<YandexAPI>(new_file_path);

        QUrl url(QString::fromStdString(new_acc->get_authorization_url()));
        QDesktopServices::openUrl(url);

        QFuture<string> future = QtConcurrent::run(async_wait_code, new_acc);

        QFutureWatcher<string>* watcher = new QFutureWatcher<string>(this);
        connect(watcher, &QFutureWatcher<std::string>::finished, [this, watcher, new_acc, new_file_path]() mutable {
            std::string code = watcher->result();
            new_acc->get_access_token_from_code(code);
            shared_ptr<Account> new_acc = make_shared<Account>(new_file_path);
            for(int i = 0; i < this->model->accounts->size(); i++) {
                if(new_acc->mail == this->model->accounts->at(i)->mail && new_acc->service == this->model->accounts->at(i)->service) {
                    fs::remove(new_file_path);
                    QMessageBox::warning(this, "Warning", "This account has already been added!");
                    watcher->deleteLater();
                    return;
                }
            }
            this->model->accounts->push_back(new_acc);
            this->model->refresh();

            watcher->deleteLater();
        });

        watcher->setFuture(future);
    }
}

void Settings::choose_path() {
    QString folder_path = QFileDialog::getExistingDirectory(this, "Choose folder");
    if(!folder_path.isEmpty()) {
        std::string folder_path_std = folder_path.toStdString();
        json data;
        ifstream file("settings.json");
        file >> data;
        file.close();
        data["download_path"] = folder_path_std;
        *download_path = folder_path_std;
        ofstream fileo("settings.json");
        fileo << data;
        fileo.close();
        ui->download_path->setText(folder_path);
    }
}