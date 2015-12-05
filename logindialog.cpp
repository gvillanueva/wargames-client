#include "logindialog.h"
#include "ui_logindialog.h"
#include "user.h"
#include <QMessageBox>
#include <QDesktopServices>

/*!
 * \brief Creates a new LoginDialog instance.
 * \param parent A parent QObject for this dialog.
 */
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_Client("https://wargames.walkingtarget.com/api/user.php", this)
{
    ui->setupUi(this);

    // Configure the dialog window
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->txtPassword->setEchoMode(QLineEdit::Password);

    // Connect the buttons
    connect(ui->pbLogin, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pbRegister, SIGNAL(clicked(bool)), this, SLOT(registerUser()));

    // Recall application settings
    ui->txtUsername->setText(m_Settings.value("lastUser").toString());
    if (ui->txtUsername->text().isEmpty())
        ui->txtUsername->setFocus();
    ui->chkLastUser->setChecked(m_Settings.value("rememberLastUser").toBool());
}

/*!
 * \brief Cleans up the user interface widgets allocated on the heap.
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/*!
 * \brief Sends a User.login JSON-RPC request to the server.
 */
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
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("login", loginParams);
    m_Client.sendMessage(message);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(processLoginResponse(QJsonRpcMessage)));
}

/*!
 * \brief Parses the response to the User.login JSON-RPC request.
 * \param response The JSON-RPC response from a QJsonRpcHttpClient.
 */
void LoginDialog::processLoginResponse(const QJsonRpcMessage& response)
{
    this->setEnabled(true);
    if (response.errorCode() != QJsonRpc::NoError)
        QMessageBox::critical(this, "Login error", response.errorMessage());
    else {
        User::instance().setAuthToken(response.result().toString());
        QDialog::accept();
    }
}

/*!
 * \brief Opens a browser window the user can use to register.
 */
void LoginDialog::registerUser()
{
    QUrl registerUserUrl("http://wargames.walkingtarget.com/register.php");
    QDesktopServices::openUrl(registerUserUrl);
}
