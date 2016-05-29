#include "lobbydialog.h"
#include "ui_lobbydialog.h"
#include "../user.h"
#include <QGraphicsPixmapItem>
#include <QTextTable>
#include <QScrollBar>

const int SQ_SIZE = 48;
const int SQ_ROWS = 8;
const int SQ_COLS = 8;

void MyPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    emit moved(this->data(0).toInt(), pos());
}

/*!
 * \brief Create a new LobbyDialog instance.
 * \param gameName The name of the game we are joining.
 * \param parent The parent QWidget object.
 */
LobbyDialog::LobbyDialog(const QString& gameName, QWidget *parent) :
    QDialog(parent), ui(new Ui::LobbyDialog),
    m_Client("ws://localhost:8000/", this),
    m_GameName(gameName),
    m_Scene(0)
{
    ui->setupUi(this);

    // Configure handlers list
    m_Handlers["joined"] = joined;
    m_Handlers["left"] = left;
    m_Handlers["chatted"] = chatted;
    m_Handlers["moved"] = moved;

    // Configure formatter for chat log
    m_TableFormat.setBorder(0);

    // Set up our game scene
    m_Scene = new QGraphicsScene(0, 0, SQ_SIZE * SQ_ROWS, SQ_SIZE * SQ_COLS);

    // Set up JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", gameName);
    QJsonArray connectParams = QJsonArray() << userObj << gameObj;

    // Make a request to join the game/subscribe to its events
    QJsonRpcMessage request = QJsonRpcMessage::createRequest("Game.join", connectParams);
    m_Client.openBlocking();
    m_Client.sendMessage(request);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));
    connect(&m_Client, SIGNAL(notificationReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));

    ui->graphicsView->setScene(m_Scene);
}

/*!
 * \brief Destroys this LobbyDialog instance.
 */
LobbyDialog::~LobbyDialog()
{
    m_Client.close();
    delete ui;
}

/*!
 * \brief Entry point to game events.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::parseLobbyMessage(const QJsonRpcMessage& message)
{
    // Call the method appropriate for handling the message
    if (!m_Handlers.contains(message.method())) {
        qCritical() << "Received unknown method: " << message.method() << message;
        return;
    }

    // Call handler for known method
    (this->*m_Handlers[message.method()])(message);
}

/*!
 * \brief Handles the user joined game event.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::joined(const QJsonRpcMessage &message)
{
    if (!message.params().isObject() && !message.params().isArray()) {
        qWarning() << __func__ << ": unexpected message format: " << message;
        return;
    }

    // Convert between by-position/by-name request parameter
    QJsonObject obj = message.params().isArray() ?
                message.params().toArray()[0].toObject() :
                message.params().toObject();
    QString name = obj["name"].toString();
    QString id = obj["id"].toString();

    if (name.isEmpty() || id.isEmpty()) {
        qWarning() << __func__ << ": message contents empty: " << message;
        return;
    }

    QColor color = ui->teHistory->textColor();
    ui->teHistory->setTextColor(Qt::gray);
    ui->teHistory->append(tr("* %1 has joined").arg(name));
    ui->teHistory->setTextColor(color);
    ui->lvUsers->addItem(name);
}

/*!
 * \brief Handles the user left game event.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::left(const QJsonRpcMessage &message)
{
}

/*!
 * \brief Handles the chatted game event.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::chatted(const QJsonRpcMessage &message)
{
    if (!message.params().isObject() && !message.params().isArray()) {
        qWarning() << __func__ << ": unexpected message format: " << message;
        return;
    }

    // Convert between by-position/by-name request parameter
    QJsonObject obj = message.params().isArray() ?
                message.params().toArray()[0].toObject() :
                message.params().toObject();
    QString user = obj["user"].toString();
    QString text = obj["message"].toString();

    if (user.isEmpty() || text.isEmpty()) {
        qWarning() << __func__ << ": message contents empty: " << message;
        return;
    }

    // Insert chat message into history
    QTextCursor cursor(ui->teHistory->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, m_TableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + user + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(text);
    QScrollBar *bar = ui->teHistory->verticalScrollBar();
    bar->setValue(bar->maximum());
}

/*!
 * \brief Handles the moved game event.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::moved(const QJsonRpcMessage &message)
{
    QJsonArray params = message.params().toArray();
    QJsonObject userObj = params[0].toObject();
    QJsonObject unitObj = params[1].toObject();
    QJsonObject moveObj = params[2].toObject();

    foreach(QGraphicsItem* item, m_Scene->items())
    {
        if (item->data(0).toInt() == unitObj["id"].toInt())
        {
            item->setPos(QPointF(moveObj["x"].toDouble(),
                                 moveObj["y"].toDouble()));
            break;
        }
    }

}

/*!
 * \brief Transmits a movement message to the server.
 * \param id The id of the unit being moved.
 * \param pos The proposed new position of the unit.
 */
void LobbyDialog::sendMoved(int id, const QPointF pos)
{
    // Construct the JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", m_GameName);
    QJsonObject unitObj;
    unitObj.insert("id", id);
    QJsonObject moveObj;
    moveObj.insert("x", pos.x());
    moveObj.insert("y", pos.y());
    QJsonArray moveParams = QJsonArray() << userObj << gameObj << unitObj << moveObj;

    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.move", moveParams);
    m_Client.sendMessage(message);
}

/*!
 * \brief Sends a chat message to the server.
 */
void LobbyDialog::on_pbSend_clicked()
{
    // Construct the JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("name", User::instance().name());
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", m_GameName);
    QJsonObject messageObj;
    messageObj.insert("text", ui->leMessage->text());
    ui->leMessage->clear();
    QJsonArray chatParams = QJsonArray() << userObj << gameObj << messageObj;

    // Make the call (notification)
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.chat", chatParams);
    m_Client.sendMessage(message);
}
