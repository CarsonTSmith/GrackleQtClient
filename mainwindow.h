#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    QTcpSocket *socket;

    void format_msg(QString &str);
    void add_header(QString &str);
};
#endif // MAINWINDOW_H
