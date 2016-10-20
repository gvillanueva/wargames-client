/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    user.cpp
 * \brief   Defines the behavior of a User object.
 */

#include "user.h"

User::User()
{
}

User& User::instance()
{
    static User instance;
    return instance;
}

QString User::authToken() const
{
    return m_AuthToken;
}

void User::setAuthToken(const QString& value)
{
    m_AuthToken = value;
}

QString User::name() const
{
    return m_Name;
}

void User::setName(const QString& value)
{
    m_Name = value;
}
