
#ifndef CLION_TEST_AUTHENTICATION_HPP
#define CLION_TEST_AUTHENTICATION_HPP

#include "Webserv.hpp"

class Authentication {

    public:
        Authentication();
        ~Authentication();

        void addNewUser(std::string login, std::string password); // call checkUserExist() -> create new user
        bool checkUserExist(std::string login);

        bool checkAuthenticationData(std::string login, std::string password);

    private:

        std::map <std::string, std::string> _credentials; // loginPassword
//        std::map<std::string, std::string>::iterator _itUser;
};

#endif //CLION_TEST_AUTHENTICATION_HPP
