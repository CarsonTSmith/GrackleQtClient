#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAbstractSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <stdio.h>

#define PORT 8085
#define HEADERSZ 8

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket)
{
    ui->setupUi(this);
    ConnectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
}

bool MainWindow::ConnectToServer()
{
    socket->connectToHost(QHostAddress::LocalHost, PORT);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_from_server()));
    socket->open(QIODevice::ReadWrite);
    if (socket->isOpen()) {
        return true;
    }

    return false;
}

void MainWindow::on_send_button_clicked()
{
    QString msg;

    if (!socket)
        return;

    if (!socket->isOpen())
        return;

    msg = ui->send_buffer->toPlainText();
    if (socket->ConnectedState) {
        socket->write(msg.toStdString().c_str());
        ui->send_buffer->clear();
    }
}

void MainWindow::read_from_server()
{
    QByteArray data;
    QString msgstr;
    int msglen, bytesrd = 0;

    if (!socket)
        return;

    if (!socket->isOpen())
        return;

    data = socket->readAll();
    ui->chat_messages->appendPlainText(msgstr);
}
