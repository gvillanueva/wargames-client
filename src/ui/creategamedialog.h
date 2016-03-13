#ifndef CREATEGAMEDIALOG_H
#define CREATEGAMEDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>

namespace Ui {
class CreateGameDialog;
}

class CreateGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGameDialog(QWidget *parent = 0);
    ~CreateGameDialog();

public slots:
    virtual void accept();

private slots:
    void processCreateRequest();

private:
    Ui::CreateGameDialog *ui;
    QJsonRpcHttpClient m_Client;
};

#endif // CREATEGAMEDIALOG_H
