#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audio_player.h"

#include <memory>
#include <QByteArray>
#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

constexpr int PORT     = 8085;
constexpr int HEADERSZ = 8;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool ConnectToServer();

private slots:
    void on_send_button_clicked();
    void read_from_server();

private:
    Ui::MainWindow *ui;
    QTcpSocket     *socket;
    QString        ip;
    int            port;
    QString        username;
    audio_player   sound_player;

    void        get_server_info();
    void        format_msg(QString &str);
    void        add_header(QString &str);
    void        add_timestamp(QString &str);
    int         read_header();
    QString     read_body(const int body_len);
    void        do_connect_to_server();
    QJsonObject format_incoming_msg(const QString& body);
    QString     format_outgoing_msg(const QString& msg);
    void        do_write(const QString &msg);
    void        do_notification_sound(const QString &username_from_msg);
};
#endif // MAINWINDOW_H
