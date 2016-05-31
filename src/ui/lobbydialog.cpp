#include "lobbydialog.h"
#include "ui_lobbydialog.h"
#include "../user.h"
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTextTable>
#include <QScrollBar>

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
    setAttribute(Qt::WA_DeleteOnClose);

    // Configure handlers list
    m_Handlers["joined"] = joined;
    m_Handlers["left"] = left;
    m_Handlers["chatted"] = chatted;
    m_Handlers["moved"] = moved;
    m_Handlers["initialState"] = initialState;

    // Configure formatter for chat log
    m_TableFormat.setBorder(0);
    m_Scene = new QGraphicsScene(this);

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
    // Set up JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", m_GameName);
    QJsonArray connectParams = QJsonArray() << userObj << gameObj;

    QJsonRpcMessage request = QJsonRpcMessage::createRequest("Game.leave", connectParams);
    m_Client.sendMessage(request);
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

    QList<QListWidgetItem *> items = ui->lvUsers->findItems(name,
                                                            Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = ui->teHistory->textColor();
    ui->teHistory->setTextColor(Qt::gray);
    ui->teHistory->append(tr("* %1 has left").arg(name));
    ui->teHistory->setTextColor(color);
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
 * \brief Handles the initialState message.
 * \param message The JSON-RPC request message.
 */
void LobbyDialog::initialState(const QJsonRpcMessage& message)
{
    if (!message.params().isObject() && !message.params().isArray()) {
        qWarning() << __func__ << ": unexpected message format: " << message;
        return;
    }

    // Read expected parameters
    QJsonObject obj = message.params().isArray() ?
                message.params().toArray()[0].toObject() :
                message.params().toObject();
    QJsonArray users = obj["users"].toArray();
    QJsonObject bounds = obj["bounds"].toObject();
    QJsonObject units = obj["units"].toObject();

    // Read array of initially connected users
    foreach(QJsonValue user, users)
        ui->lvUsers->addItem(user.toString());

    // Set up our game scene
    QRectF sceneBounds(bounds["x"].toDouble(), bounds["y"].toDouble(),
                  bounds["w"].toDouble(), bounds["h"].toDouble());
    m_Scene->setSceneRect(sceneBounds);

    // Read dictionary of initial units
    foreach(QJsonValue unitVal, units) {
        QJsonObject unit = unitVal.toObject();
        MyPixmapItem *pixmap = new MyPixmapItem(QPixmap(unit["image"].toString()));
        pixmap->setPos(unit["x"].toDouble(), unit["y"].toDouble());
        pixmap->setFlag(QGraphicsItem::ItemIsMovable);
        pixmap->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        m_Scene->addItem(pixmap);
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

    // Make the call (check for response)
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.move", moveParams);
    QJsonRpcServiceReply *response = m_Client.sendMessage(message);
    connect(response, SIGNAL(finished()), this, SLOT(parseMoveResponse()));
}

/*!
 * \brief Checks Game.move responses for errors.
 * \param message The JSON-RPC response message.
 * Actual unit movement will occur as part of LobbyDialog::moved.
 */
void LobbyDialog::parseMoveResponse(const QJsonRpcMessage& message)
{
    if (message.errorCode() == QJsonRpc::NoError)
        return;
    QMessageBox::critical(this, "Login error", message.errorMessage());
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
