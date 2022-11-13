#include "createserverdialog.h"
#include "./ui_createserverdialog.h"

#include <QWidget>

CreateServerDialog::CreateServerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateServerDialog)
{
    ui->setupUi(this);
}

CreateServerDialog::~CreateServerDialog()
{
    delete ui;
}
