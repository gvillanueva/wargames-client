#include "logindialog.h"
#include "ui_logindialog.h"
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "user.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Configure the dialog window
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->txtPassword->setEchoMode(QLineEdit::Password);

    // Connect the buttons
    connect(ui->pbLogin, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    // TODO: connect(ui->pbRegister, SIGNAL(clicked(bool)), this, SLOT(register()));

    // Recall application settings
    ui->txtUsername->setText(m_Settings.value("lastUser").toString());
    if (ui->txtUsername->text().isEmpty())
        ui->txtUsername->setFocus();
    ui->chkLastUser->setChecked(m_Settings.value("rememberLastUser").toBool());
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::accept()
{
    // Disable UI until response is returned
    this->setEnabled(false);

    // Save settings
    if (ui->chkLastUser->isChecked())
        m_Settings.setValue("lastUser", ui->txtUsername->text());
    else
        m_Settings.setValue("lastUser", QString());
    m_Settings.setValue("rememberLastUser", ui->chkLastUser->isChecked());

    // Set up JSON-RPC parameters
    QJsonArray loginParams;
    loginParams.insert(0, QJsonValue(ui->txtUsername->text()));
    loginParams.insert(1, QJsonValue(ui->txtPassword->text()));

    // Attempt to log in
    QJsonRpcHttpClient client("https://wargames.walkingtarget.com/chess/user.php");
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("login", loginParams);
    QJsonRpcMessage response = client.sendMessageBlocking(message);

    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical
        qDebug() << response.errorMessage();
    }
    else {
        qDebug() << response.result().toString();
        User::instance().setAuthToken(response.result().toString());
    }

    this->setEnabled(true);
}
