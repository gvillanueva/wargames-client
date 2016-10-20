/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-27
 * \file    creategamedialog.h
 * \brief   Defines the structure for a Create Game dialog.
 */

#ifndef CREATEGAMEDIALOG_H
#define CREATEGAMEDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>

namespace Ui {
class CreateGameDialog;
}

/*!
 * \brief The CreateGameDialog class provides the interface for selecting
 *        parameters to create a new game.
 */
class CreateGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGameDialog(QWidget *parent = 0);
    ~CreateGameDialog();

public slots:
    virtual void accept();

private slots:
    void sendListSystemsRequest();
    void processListSystemsResponse();
    void processCreateRequest();

private:
    Ui::CreateGameDialog *ui;
    QJsonRpcHttpClient m_Client;
};

#endif // CREATEGAMEDIALOG_H
