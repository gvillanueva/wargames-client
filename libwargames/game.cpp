/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-14
 * \file    game.cpp
 * \brief   Defines the behaviors of a Game object.
 */

#include "game.h"

Game::Game()
{
}

Game::Game(const QJsonObject& obj)
{
    m_Guid = QUuid(obj["id"].toString());
    m_Name = obj["name"].toString();
    m_IsPublic = obj["public"].toBool();
    m_NumUsers = obj["numUsers"].toInt();
    m_MaxUsers = obj["maxUsers"].toInt();
}

QUuid Game::guid() const
{
    return m_Guid;
}

void Game::setGuid(const QUuid& value)
{
    m_Guid = value;
}

QString Game::name() const
{
    return m_Name;
}

void Game::setName(const QString& value)
{
    m_Name = value;
}

bool Game::isPublic() const
{
    return m_IsPublic;
}

void Game::setIsPublic(bool value)
{
    m_IsPublic = value;
}

int Game::numUsers() const
{
    return m_NumUsers;
}

int Game::maxUsers() const
{
    return m_MaxUsers;
}

void Game::setMaxUsers(int value)
{
    m_MaxUsers = value;
}
