#ifndef USER_H
#define USER_H

#include <string>
class UserManager
{
public:
    UserManager(){}
    ~UserManager(){}

};
struct UserDetail
{
    int userid_;
    std::string nickname_;
    std::string phone;

};
class User
{
public:
    User(){}
    ~User(){}

private:
    UserDetail detail_;
};

#endif
