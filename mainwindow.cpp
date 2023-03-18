#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAbstractSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <stdio.h>

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

int MainWindow::read_header()
{
    int total = 0;
    QByteArray header;

    while (total < HEADERSZ) {
        header.append(socket->read(HEADERSZ - total));
        total += header.length() - total;
    }

    return header.toInt();
}

QString MainWindow::read_body(const int body_len)
{
    int total = 0;
    QByteArray body;

    while (total < body_len) {
        body.append(socket->read(body_len - total));
        total += body.length() - total;
    }

    return body;
}

void MainWindow::format_msg(QString &str)
{
    add_header(str);
}

void MainWindow::add_header(QString &str)
{
    QString header = QString::number(str.length()).rightJustified(HEADERSZ, '0');
    str.prepend(header);
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
    format_msg(msg);
    if (socket->ConnectedState) {
        socket->write(msg.toStdString().c_str());
        ui->send_buffer->clear();
    }
}

void MainWindow::read_from_server()
{
    QByteArray data, result;
    QString msgstr;
    int body_len, bytesrd = 0;

    if (!socket)
        return;

    if (!socket->isOpen())
        return;

    body_len = read_header();
    msgstr   = read_body(body_len);
    /*
    while (bytesrd < msglen) {
        result = socket->read(msglen - bytesrd);
        data.append(result);
        bytesrd += result.length();
    }
    msgstr = data;
    */
    ui->chat_messages->appendPlainText(msgstr);
}
