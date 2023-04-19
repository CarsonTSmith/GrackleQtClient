#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "createserverdialog.h"

#include <QDebug>
#include <iostream>
#include <memory>
#include <QAbstractSocket>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QHostAddress>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QThread>
#include <stdio.h>
#include <unistd.h>
#ifdef _WIN32
#include <utilapiset.h>
#include <processthreadsapi.h>
#elif __linux__
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
#elif __linux__
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
    str = format_outgoing_msg(str);
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
    if (msg.isEmpty())
        return;

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

    while (socket->bytesAvailable() > 0) {
        body_len              = read_header();
        msgstr                = read_body(body_len);
        QJsonObject json_body = format_incoming_msg(msgstr);
        ui->chat_messages->appendPlainText("\n" + json_body["timestamp"].toString() + "\n" + json_body["username"].toString() + ": " + json_body["message"].toString());
        do_notification_sound(json_body["username"].toString());
    }
}

QString MainWindow::format_outgoing_msg(const QString& msg)
{
    QJsonObject obj {
        {"path", "/chat/send"},
        {"client", "Grackle Desktop"},
        {"username", username},
        {"message", msg}
    };
    QJsonDocument doc(obj);
    QString str_json(doc.toJson(QJsonDocument::Compact));
    return str_json;
}

QJsonObject MainWindow::format_incoming_msg(const QString& body)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(body.toUtf8());

    // check validity of the document
    if(!doc.isNull()) {
        if(doc.isObject()) {
            obj = doc.object();
        }
    }

    return obj;
}

void MainWindow::do_write(const QString &msg)
{
    int total = 0, result = 0;

    // for testing the server non-blocking tcp socket
    // simulates sending the tcp bytes in two parts
    /*
    auto w1 = socket->write(msg.toStdString().c_str(), 10);
    socket->flush();
    qDebug() << w1;
    sleep(10);
    socket->write(msg.toStdString().c_str() + 10, msg.length() - 10);
    */

    while (total < msg.length()) {
        result = socket->write(msg.toStdString().c_str() + total, msg.length() - total);
        if (result == -1)
            return; // write error occurred

        total += result;
    }
}

void MainWindow::do_notification_sound(const QString &username_from_msg)
{
    if (username_from_msg == username)
        return; // you sent the message, dont notify on your own msgs

    QString notification_sound_path = QDir::current().absolutePath() + "/sounds/notification_sound.mp3";
    sound_player.play(notification_sound_path);
}
