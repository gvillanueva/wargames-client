#include "unit.h"
#include <qjsonrpc/qjsonrpchttpclient.h>
#include <QVector3D>
#include <qjsonrpc/qjsonrpcmessage.h>

Unit::Unit(QObject *parent) : QObject(parent)
{
}

/*!
 * \brief Gets the three-dimensional position of the unit.
 * \return
 */
QVector3D Unit::position() const
{
    return m_Position;
}

/*!
 * \brief Sets the three-dimensional position of the unit.
 * \param value
 */
void Unit::setPosition(QVector3D value)
{
    // TODO: Need GS validator to ensure non rule-breaking move
    m_Position = value;
}

/*!
 * \brief Gets the direction (y-rotation) of the unit.
 * \return
 */
qreal Unit::direction() const
{
    return m_Direction;
}

/*!
 * \brief Sets the direction (y-rotation) of the unit.
 * \param value
 */
void Unit::setDirection(qreal value)
{
    m_Direction = value;
}

/*!
 * \brief Unit::move
 * \param newPosition
 * \remarks Accessible to scripting.
 */
void Unit::move(QVector3D newPosition)
{
    //Game *game = Game::instance();

    QJsonObject obj;
//    obj["gameUuid"]. m_GameUuid;
//    obj["userUuid"] = m_UserUuid;
//    obj["unitUuid"] = m_UnitUuid;
//    obj["x"] = newPosition.x();
//    obj["y"] = newPosition.y();
//    obj["x"] = newPosition.z();

    // Movement requirest -- 1. UnitID, 2. New Position
    // Returns
    // JSON-RPC Success
    // Access Error, Unit does not belong to user
    // Access Error, Unit does not belong to game
    // Error invalid unit.
    // Error invalid position - outside of bounds, collision with terrain
    // Error, illegal move, determined by Game Rules

    QJsonRpcHttpClient client("http://wargames.walkingtarget.com/chess/unit.php");
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("move");
    QJsonRpcMessage response = client.sendMessageBlocking(message);

//    if (response.errorCode() == SUCCESS)
//        setDirection(value);
//    else
//        display error;
}

/*!
 * \brief Unit::turn
 * \param newDirection
 * \remarks Accessible to scripting.
 */
void Unit::turn(qreal newDirection)
{
    // Direction requires -- 1. UnitId, 2. New Position
    // Mainly determined by game rules, eg. activated units cannot turn after the fact
    // Error, illegal action
}
