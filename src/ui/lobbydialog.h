#ifndef LOBBYDIALOG_H
#define LOBBYDIALOG_H

#include <QDialog>
#include "../jsonrpc/jsonrpcwebsocketclient.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QHash>

namespace Ui {
class LobbyDialog;
}

class LobbyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LobbyDialog(const QString& gameName, QWidget *parent = 0);
    ~LobbyDialog();

private slots:
    void parseLobbyMessage(const QJsonRpcMessage& message);
    void joined(const QJsonRpcMessage& message);
    void left(const QJsonRpcMessage& message);
    void chatted(const QJsonRpcMessage& message);
    void moved(const QJsonRpcMessage& message);
    void sendMoved(int id, const QPointF pos);
    void on_pbSend_clicked();

private:
    Ui::LobbyDialog *ui;
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
