#include "aboutwindow.h"
#include "../ui/ui_about.h"

using namespace std;

About::About(QWidget *parent): QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);

    connect(ui->close, &QPushButton::clicked, this, &QDialog::close);
}

About::~About() {
    delete ui;
}