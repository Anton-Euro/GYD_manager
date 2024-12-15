/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *label;
    QLabel *label_3;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *version;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *close;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName("About");
        About->resize(400, 300);
        About->setSizeGripEnabled(false);
        label = new QLabel(About);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 10, 250, 160));
        label->setMinimumSize(QSize(250, 160));
        label->setMaximumSize(QSize(250, 160));
        label->setPixmap(QPixmap(QString::fromUtf8(":/about/static/logo.png")));
        label->setScaledContents(true);
        label_3 = new QLabel(About);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 230, 361, 21));
        label_3->setWordWrap(true);
        gridLayoutWidget = new QWidget(About);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(20, 180, 161, 41));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        version = new QLabel(gridLayoutWidget);
        version->setObjectName("version");

        gridLayout->addWidget(version, 0, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 1, 1, 1, 1);

        close = new QPushButton(About);
        close->setObjectName("close");
        close->setGeometry(QRect(309, 260, 71, 24));
        close->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QCoreApplication::translate("About", "About", nullptr));
        label->setText(QString());
        label_3->setText(QCoreApplication::translate("About", "A program for using different cloud services in one place", nullptr));
        label_2->setText(QCoreApplication::translate("About", "Version:", nullptr));
        version->setText(QCoreApplication::translate("About", "0.1.0", nullptr));
        label_4->setText(QCoreApplication::translate("About", "Author:", nullptr));
        label_5->setText(QCoreApplication::translate("About", "Sladikov Anton", nullptr));
        close->setText(QCoreApplication::translate("About", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
