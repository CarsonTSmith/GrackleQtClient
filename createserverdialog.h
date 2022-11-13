#ifndef CREATESERVERDIALOG_H
#define CREATESERVERDIALOG_H

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

private:
    Ui::CreateServerDialog *ui;
};

#endif // CREATESERVERDIALOG_H
