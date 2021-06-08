#include "Authentication.hpp"

Authentication::Authentication()
{
    addNewUser("gbroccol", "1");
    addNewUser("ssnowbir", "1");
    addNewUser("pvivian", "1");
}

Authentication::~Authentication() {}

void Authentication::addNewUser(std::string login, std::string password) // call checkUserExist() -> create new user
{
    try
    {
        std::string er1 = "Error. User with this login exists";
        if (checkUserExist(login))
            throw er1;
    }
    catch (std::string message)
    {
//        std::cerr << message << '\n';
        std::cout << RED << message << std::endl;
        return;
    }

    _credentials.insert(std::pair<std::string, std::string> (login, password));
//    std::map<std::string, std::string>::iterator it = _credentials.find(login);
//    std::cout << PURPLE << " New user for testing is LOGIN - " << it->first << " PASSWORD - " << it->second << BW << std::endl;
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

//    try {
        std::string _loginNotExists = "Login is not exists";
        std::string _passwordNotCorrect = "Password is not correct";

        if (it == _credentials.end())
        {
//            throw _loginNotExists;
            return false;
        }
        if (it->second != password)
        {
//            throw _passwordNotCorrect;
            return false;
        }
//    }
//    catch (std::string message)
//    {
//        std::cerr << message << '\n';
//        std::cout << RED << message << std::endl;
//        return false;
//    }
//    std::cout << GREEN << "TRUE - user exists" << std::endl;
    return true;
}