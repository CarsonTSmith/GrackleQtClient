#include "mainwindow.h"

#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<MainWindow> w(new MainWindow());
    w->show();
    return a.exec();
}
