/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCreate_folder;
    QAction *actionUpload;
    QAction *actionSettings;
    QAction *actionTrash;
    QAction *actionExit;
    QAction *actionabout;
    QAction *actionSort;
    QAction *actionrefresh;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_3;
    QSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *upload;
    QSpacerItem *horizontalSpacer_2;
    QTreeWidget *left_menu;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QProgressBar *storage_status_progressbar;
    QLabel *storage_status_label;
    QPushButton *settings;
    QStackedWidget *content_widget;
    QWidget *tasks_widget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tasks_table;
    QWidget *drive_widget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *back;
    QPushButton *forward;
    QPushButton *refresh;
    QSpacerItem *horizontalSpacer;
    QLineEdit *search_line;
    QTableView *files_view;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menuhelp;
    QMenu *menuview;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(978, 672);
        actionCreate_folder = new QAction(MainWindow);
        actionCreate_folder->setObjectName("actionCreate_folder");
        actionUpload = new QAction(MainWindow);
        actionUpload->setObjectName("actionUpload");
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName("actionSettings");
        actionTrash = new QAction(MainWindow);
        actionTrash->setObjectName("actionTrash");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionabout = new QAction(MainWindow);
        actionabout->setObjectName("actionabout");
        actionSort = new QAction(MainWindow);
        actionSort->setObjectName("actionSort");
        actionrefresh = new QAction(MainWindow);
        actionrefresh->setObjectName("actionrefresh");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_3 = new QHBoxLayout(centralwidget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 3, 0, 3);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Horizontal);
        splitter->setHandleWidth(5);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(3, 3, 0, 0);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        upload = new QPushButton(verticalLayoutWidget);
        upload->setObjectName("upload");
        upload->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 8px;\n"
"	color: #000000;\n"
"	padding: 10px 20px 10px 20px;\n"
"	font-size: 20px;\n"
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
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/menu/static/upload.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        upload->setIcon(icon);
        upload->setIconSize(QSize(24, 24));

        horizontalLayout_11->addWidget(upload);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_11);

        left_menu = new QTreeWidget(verticalLayoutWidget);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/menu/static/swap_vert.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/menu/static/cloud.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(left_menu);
        __qtreewidgetitem->setIcon(0, icon1);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(left_menu);
        __qtreewidgetitem1->setIcon(0, icon2);
        left_menu->setObjectName("left_menu");
        left_menu->setStyleSheet(QString::fromUtf8("QTreeWidget {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 3px;\n"
"    color: #000000;\n"
"    selection-background-color: #007aff;\n"
"    selection-color: #ffffff;\n"
"}"));
        left_menu->setIndentation(15);
        left_menu->setHeaderHidden(true);

        verticalLayout_3->addWidget(left_menu);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        storage_status_progressbar = new QProgressBar(verticalLayoutWidget);
        storage_status_progressbar->setObjectName("storage_status_progressbar");
        storage_status_progressbar->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"	max-height: 5px;\n"
"}"));
        storage_status_progressbar->setValue(0);
        storage_status_progressbar->setTextVisible(false);

        verticalLayout_2->addWidget(storage_status_progressbar);

        storage_status_label = new QLabel(verticalLayoutWidget);
        storage_status_label->setObjectName("storage_status_label");

        verticalLayout_2->addWidget(storage_status_label);


        horizontalLayout_2->addLayout(verticalLayout_2);

        settings = new QPushButton(verticalLayoutWidget);
        settings->setObjectName("settings");
        settings->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 5px;\n"
"    color: #000000;\n"
"    padding: 2px 8px;\n"
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
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/menu/static/settings.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        settings->setIcon(icon3);
        settings->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(settings);


        verticalLayout_3->addLayout(horizontalLayout_2);

        splitter->addWidget(verticalLayoutWidget);
        content_widget = new QStackedWidget(splitter);
        content_widget->setObjectName("content_widget");
        content_widget->setMinimumSize(QSize(700, 0));
        tasks_widget = new QWidget();
        tasks_widget->setObjectName("tasks_widget");
        verticalLayout = new QVBoxLayout(tasks_widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 3, 3, 0);
        tasks_table = new QTableWidget(tasks_widget);
        if (tasks_table->columnCount() < 6)
            tasks_table->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tasks_table->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tasks_table->setObjectName("tasks_table");
        tasks_table->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid #d1d1d6;\n"
"	border-radius: 3px;\n"
"	color: #000000;\n"
"	selection-background-color: #007aff;\n"
"	selection-color: #ffffff;\n"
"}"));
        tasks_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tasks_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tasks_table->setShowGrid(false);
        tasks_table->setWordWrap(false);
        tasks_table->horizontalHeader()->setStretchLastSection(true);
        tasks_table->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tasks_table);

        content_widget->addWidget(tasks_widget);
        drive_widget = new QWidget();
        drive_widget->setObjectName("drive_widget");
        verticalLayout_5 = new QVBoxLayout(drive_widget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 3, 3, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName("horizontalLayout");
        back = new QPushButton(drive_widget);
        back->setObjectName("back");
        back->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 5px;\n"
"	color: #000000;\n"
"    padding: 3px 12px;\n"
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
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/drive/static/arrow_back.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        back->setIcon(icon4);
        back->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(back);

        forward = new QPushButton(drive_widget);
        forward->setObjectName("forward");
        forward->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 5px;\n"
"	color: #000000;\n"
"    padding: 3px 12px;\n"
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
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/drive/static/arrow_forward.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        forward->setIcon(icon5);
        forward->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(forward);

        refresh = new QPushButton(drive_widget);
        refresh->setObjectName("refresh");
        refresh->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 5px;\n"
"	color: #000000;\n"
"    padding: 3px 12px;\n"
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
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/drive/static/refresh.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        refresh->setIcon(icon6);
        refresh->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(refresh);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        search_line = new QLineEdit(drive_widget);
        search_line->setObjectName("search_line");
        search_line->setMinimumSize(QSize(0, 0));
        search_line->setMaximumSize(QSize(250, 16777215));
        search_line->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #d1d1d6;\n"
"    border-radius: 3px;\n"
"    color: #000000;\n"
"	height: 24px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 1px solid #007aff;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QLineEdit:disabled {\n"
"    background-color: #f2f2f7;\n"
"    color: #8e8e93;\n"
"    border-color: #d1d1d6;\n"
"}"));

        horizontalLayout->addWidget(search_line);


        verticalLayout_5->addLayout(horizontalLayout);

        files_view = new QTableView(drive_widget);
        files_view->setObjectName("files_view");
        files_view->setStyleSheet(QString::fromUtf8("QTableView {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid #d1d1d6;\n"
"	border-radius: 3px;\n"
"	color: #000000;\n"
"	selection-background-color: #007aff;\n"
"	selection-color: #ffffff;\n"
"}"));
        files_view->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        files_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        files_view->setShowGrid(false);
        files_view->setSortingEnabled(true);
        files_view->setWordWrap(false);
        files_view->horizontalHeader()->setDefaultSectionSize(150);
        files_view->horizontalHeader()->setStretchLastSection(true);
        files_view->verticalHeader()->setVisible(false);

        verticalLayout_5->addWidget(files_view);

        content_widget->addWidget(drive_widget);
        splitter->addWidget(content_widget);

        horizontalLayout_3->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setStyleSheet(QString::fromUtf8("QStatusBar {\n"
"    border-top: 1px solid #d1d1d6;\n"
"}"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 978, 23));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menuhelp = new QMenu(menubar);
        menuhelp->setObjectName("menuhelp");
        menuview = new QMenu(menubar);
        menuview->setObjectName("menuview");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menuview->menuAction());
        menubar->addAction(menuhelp->menuAction());
        menu->addAction(actionUpload);
        menu->addSeparator();
        menu->addAction(actionCreate_folder);
        menu->addSeparator();
        menu->addAction(actionSettings);
        menu->addAction(actionTrash);
        menu->addAction(actionExit);
        menuhelp->addAction(actionabout);
        menuview->addAction(actionSort);
        menuview->addSeparator();
        menuview->addAction(actionrefresh);

        retranslateUi(MainWindow);

        content_widget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GYDManager", nullptr));
        actionCreate_folder->setText(QCoreApplication::translate("MainWindow", "Create folder", nullptr));
        actionUpload->setText(QCoreApplication::translate("MainWindow", "Upload", nullptr));
        actionSettings->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        actionTrash->setText(QCoreApplication::translate("MainWindow", "Trash", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionabout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionSort->setText(QCoreApplication::translate("MainWindow", "Sort", nullptr));
        actionrefresh->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        upload->setText(QCoreApplication::translate("MainWindow", "Upload", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = left_menu->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "1", nullptr));

        const bool __sortingEnabled = left_menu->isSortingEnabled();
        left_menu->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = left_menu->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "Tasks", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = left_menu->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("MainWindow", "Drive", nullptr));
        left_menu->setSortingEnabled(__sortingEnabled);

        storage_status_label->setText(QCoreApplication::translate("MainWindow", "Used ? from ?", nullptr));
        settings->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tasks_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tasks_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tasks_table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Speed", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tasks_table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tasks_table->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Progress", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tasks_table->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "ETA", nullptr));
        back->setText(QString());
        forward->setText(QString());
        refresh->setText(QString());
        search_line->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuhelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuview->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
