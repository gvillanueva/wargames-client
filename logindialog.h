#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSettings>
#include <qjsonrpc/qjsonrpchttpclient.h>

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

private slots:
    void processLoginResponse(const QJsonRpcMessage& response);
    void registerUser();

private:
    Ui::LoginDialog *ui;
    QSettings m_Settings;
    QJsonRpcHttpClient m_Client;
};

#endif // LOGINDIALOG_H
