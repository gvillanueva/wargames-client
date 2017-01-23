/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-13
 * \file    logindialog.h
 * \brief   Defines the structure of the Login dialog.
 */

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class LoginDialog;
}

/*!
 * \brief The LoginDialog class provides the interface for user authentication.
 */
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

public slots:
    virtual void accept();

private slots:
    void processReply();
    void registerUser();

private:
    Ui::LoginDialog *ui;
    QSettings m_Settings;
};

#endif // LOGINDIALOG_H
