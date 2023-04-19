#include "createserverdialog.h"
#include "./ui_createserverdialog.h"

#include <QWidget>

CreateServerDialog::CreateServerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateServerDialog)
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
}

CreateServerDialog::~CreateServerDialog()
{
    delete ui;
}

void CreateServerDialog::on_connectButton_clicked()
{
    done(QDialog::Accepted);
}

