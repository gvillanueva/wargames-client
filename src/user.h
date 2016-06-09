/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    user.h
 * \brief   Defines the structure of a User object.
 */

#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    static User& instance();
    QString authToken() const;
    void setAuthToken(const QString& value);
    QString name() const;
    void setName(const QString& value);

private:
    User();
    User(const User&);
    void operator=(const User&);

private:
    QString m_Name;
    QString m_AuthToken;
    static User* s_Instance;
};

#endif // USER_H
