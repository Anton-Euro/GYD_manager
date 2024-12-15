/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *download_path;
    QPushButton *select_path;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *add;
    QComboBox *acc_type;
    QSpacerItem *horizontalSpacer;
    QTableView *acc_list_view;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName("Settings");
        Settings->resize(667, 475);
        verticalLayout = new QVBoxLayout(Settings);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        tabWidget = new QTabWidget(Settings);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
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
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(tab);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        download_path = new QLineEdit(tab);
        download_path->setObjectName("download_path");
        download_path->setEnabled(false);

        horizontalLayout_2->addWidget(download_path);

        select_path = new QPushButton(tab);
        select_path->setObjectName("select_path");
        select_path->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        horizontalLayout_2->addWidget(select_path);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName("horizontalLayout");
        add = new QPushButton(tab_2);
        add->setObjectName("add");
        add->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/settings/static/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        add->setIcon(icon);

        horizontalLayout->addWidget(add);

        acc_type = new QComboBox(tab_2);
        acc_type->addItem(QString());
        acc_type->addItem(QString());
        acc_type->addItem(QString());
        acc_type->addItem(QString());
        acc_type->setObjectName("acc_type");
        acc_type->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(acc_type);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        acc_list_view = new QTableView(tab_2);
        acc_list_view->setObjectName("acc_list_view");
        acc_list_view->setStyleSheet(QString::fromUtf8("QTableView {\n"
"	background-color: #ffffff;\n"
"	border: 1px solid #d1d1d6;\n"
"	border-radius: 3px;\n"
"	color: #000000;\n"
"	selection-background-color: #007aff;\n"
"	selection-color: #ffffff;\n"
"}"));
        acc_list_view->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        acc_list_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        acc_list_view->setShowGrid(false);
        acc_list_view->setWordWrap(false);
        acc_list_view->horizontalHeader()->setDefaultSectionSize(150);
        acc_list_view->horizontalHeader()->setStretchLastSection(true);
        acc_list_view->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(acc_list_view);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(Settings);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QCoreApplication::translate("Settings", "Settings", nullptr));
        label->setText(QCoreApplication::translate("Settings", "Download path:", nullptr));
        download_path->setText(QString());
        select_path->setText(QCoreApplication::translate("Settings", "Choose", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Settings", "General", nullptr));
        add->setText(QCoreApplication::translate("Settings", "Add account", nullptr));
        acc_type->setItemText(0, QCoreApplication::translate("Settings", "--", nullptr));
        acc_type->setItemText(1, QCoreApplication::translate("Settings", "Google", nullptr));
        acc_type->setItemText(2, QCoreApplication::translate("Settings", "Yandex", nullptr));
        acc_type->setItemText(3, QCoreApplication::translate("Settings", "Dropbox", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Settings", "Accounts", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
