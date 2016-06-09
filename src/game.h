/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-14
 * \file    game.h
 * \brief   Defines the structure of a Game object.
 */

#ifndef GAME_H
#define GAME_H

#include <QJsonObject>
#include <QUuid>

class Game
{
public:
    Game();
    Game(const QJsonObject& obj);

    QUuid guid() const;
    void setGuid(const QUuid& value);
    QString name() const;
    void setName(const QString& value);
    bool isPublic() const;
    void setIsPublic(bool value);
    int numUsers() const;
    int maxUsers() const;
    void setMaxUsers(int value);

private:
    QUuid m_Guid;
    QString m_Name;
    bool m_IsPublic;
    int m_NumUsers;
    int m_MaxUsers;
};

typedef QList<Game> GameList;

#endif // GAME_H
