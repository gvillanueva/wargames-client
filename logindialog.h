#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

public slots:
    virtual void accept();
    virtual void reject();

private:
    Ui::LoginDialog *ui;
    QSettings m_Settings;
};

#endif // LOGINDIALOG_H
