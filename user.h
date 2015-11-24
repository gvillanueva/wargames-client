#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    static User& instance();
    QString authToken() const;
    void setAuthToken(const QString& value);

private:
    User();
    User(const User&);
    void operator=(const User&);

private:
    QString m_AuthToken;
    static User* s_Instance;
};

#endif // USER_H
