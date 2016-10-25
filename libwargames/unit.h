/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    unit.h
 * \brief   Defines the structure of a Unit object.
 */

#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QUuid>
#include <QVector3D>
#include <QJsonObject>
#include <qjsonrpc/qjsonrpchttpclient.h>

/*!
 * \brief The Unit class defines a model to contain Unit information.
 */
class Unit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition)
    Q_PROPERTY(qreal direction READ direction WRITE setDirection)

public:
    explicit Unit(QObject *parent = 0);
    QVector3D position() const;
    void setPosition(QVector3D value);
    qreal direction() const;
    void setDirection(qreal value);

public slots:
    void move(QVector3D newPosition);
    void turn(qreal newDirection);

private:
    QJsonRpcHttpClient m_Client;
    QUuid m_GameUuid;
    QUuid m_UserUuid;
    QUuid m_UnitUuid;
    QVector3D m_Position;
    qreal m_Direction;
    QJsonObject m_JsonData;
};

#endif // UNIT_H
