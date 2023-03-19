#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "createserverdialog.h"

#include <memory>
#include <QAbstractSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <QThread>
#include <stdio.h>
#ifdef _WIN32
#include <utilapiset.h>
#include <processthreadsapi.h>
#elif _LINUX
#include <stdlib.h>
#endif



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket)
{
    ui->setupUi(this);
    do_connect_to_server();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
}

void MainWindow::do_connect_to_server()
{
    bool connected = false;

    while (!connected) {
        get_server_info();
        connected = ConnectToServer();
    }
}

void MainWindow::get_server_info()
{
    std::unique_ptr<CreateServerDialog> dialog(new CreateServerDialog());
    auto result = dialog->exec();
    if (result == QDialog::Rejected) {
#ifdef _WIN32
        ExitProcess(0);
#elif _LINUX
        exit(0);
#endif
    }

    ip       = dialog->ui->ip_address->text();
    port     = dialog->ui->port->text().toInt();
    username = dialog->ui->username->text();
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
    socket->connectToHost(ip, port, QIODevice::ReadWrite);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_from_server()));
    socket->waitForConnected(5000);
    if (socket->state() == QAbstractSocket::ConnectedState) {
        return true;
    } else {
        socket->abort();
        return false;
    }
}

void MainWindow::on_send_button_clicked()
{
    QString msg;

    if (!socket)
        return;

    if (!socket->isOpen())
        return;

    msg = ui->send_buffer->toPlainText();
    msg.prepend(username + ": ");
    format_msg(msg);
    if (socket->state() == QAbstractSocket::ConnectedState) {
        do_write(msg);
        ui->send_buffer->clear();
    }
}

void MainWindow::read_from_server()
{
    QString msgstr;
    int body_len;

    if (!socket)
        return;

    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    if (socket->bytesAvailable() == 0)
        return;

    body_len = read_header();
    msgstr   = read_body(body_len);

    ui->chat_messages->appendPlainText(msgstr);
    do_bell(msgstr);
}

void MainWindow::do_bell(const QString &msg)
{
    auto name_list = msg.split(":");
    if (name_list[0] != username) {
#ifdef _WIN32
    Beep(1000, 500);
#endif
    }
}

void MainWindow::do_write(const QString &msg)
{
    int total = 0, result = 0;

    while (total < msg.length()) {
        result = socket->write(msg.toStdString().c_str(), msg.length() - total);
        if (result == -1)
            return; // write error occurred

        total += result;
    }
}
