#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void    get_server_info();
    void    format_msg(QString &str);
    void    add_header(QString &str);
    void    add_timestamp(QString &str);
    int     read_header();
    QString read_body(const int body_len);
    void    do_connect_to_server();
    void    do_bell(const QString &msg);
    void    do_write(const QString &msg);
};
#endif // MAINWINDOW_H
