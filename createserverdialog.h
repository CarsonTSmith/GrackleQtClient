#ifndef CREATESERVERDIALOG_H
#define CREATESERVERDIALOG_H

#include "./ui_createserverdialog.h"
#include "mainwindow.h"

#include <QDialog>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CreateServerDialog; }
QT_END_NAMESPACE

class CreateServerDialog : public QDialog
{
    Q_OBJECT

public:
    CreateServerDialog(QWidget *parent = nullptr);
    ~CreateServerDialog();

private slots:
    void on_connectButton_clicked();

private:
    Ui::CreateServerDialog *ui;

    friend class MainWindow;
};

#endif // CREATESERVERDIALOG_H
