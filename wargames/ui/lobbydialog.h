/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-31
 * \file    lobbydialog.h
 * \brief   Defines the structure of the Lobby dialog.
 */

#ifndef LOBBYDIALOG_H
#define LOBBYDIALOG_H

#include <QDialog>
#include "jsonrpc/jsonrpcwebsocketclient.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QHash>
#include <QTextTableFormat>

namespace Ui {
class LobbyDialog;
}

/*!
 * \brief The LobbyDialog class provides the chatroom interface and the gameplay
 *        scene.
 */
class LobbyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LobbyDialog(const QString& gameName, QWidget *parent = 0);
    ~LobbyDialog();

private slots:
    void parseLobbyMessage(const QJsonRpcMessage& message);
    void parseMoveResponse();
    void joined(const QJsonRpcMessage& message);
    void left(const QJsonRpcMessage& message);
    void chatted(const QJsonRpcMessage& message);
    void moved(const QJsonRpcMessage& message);
    void initialState(const QJsonRpcMessage& message);
    void sendMoved(int id, const QPointF pos);
    void on_pbSend_clicked();

private:
    Ui::LobbyDialog *ui;
    QTextTableFormat m_TableFormat;
    JsonRpcWebSocketClient m_Client;
    QString m_GameName;
    QGraphicsScene *m_Scene;
    QHash<QString, void(LobbyDialog::*)(const QJsonRpcMessage&)> m_Handlers;
};

class MyPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MyPixmapItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap) {}
signals:
    void moved(int id, const QPointF& value);
protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
};

#endif // LOBBYDIALOG_H
