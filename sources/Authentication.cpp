#include "Authentication.hpp"

Authentication::Authentication()
{
//    addNewUser("gbroccol", "1");
}

Authentication::~Authentication() {}

bool Authentication::addNewUser(std::string login, std::string password) // call checkUserExist() -> create new user
{
    if (login.length() == 0 || password.length() == 0)
        return false;
    else if (checkUserExist(login))
        return false;

    _credentials.insert(std::pair<std::string, std::string> (login, password));

    return true;
}

bool Authentication::checkUserExist(std::string login)
{



    std::map<std::string, std::string>::iterator it;
    it = _credentials.find(login);

    if (it == _credentials.end())
        return false;
    return true;
}

bool Authentication::checkAuthenticationData(std::string login, std::string password)
{
    std::map<std::string, std::string>::iterator it;
    it = _credentials.find(login);

//    std::string _loginNotExists = "Login is not exists";
//    std::string _passwordNotCorrect = "Password is not correct";

    if (it == _credentials.end())
        return false;
    else if (it->second != password)
        return false;
    return true;
}