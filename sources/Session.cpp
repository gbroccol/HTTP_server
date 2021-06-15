/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:43:31 by pvivian           #+#    #+#             */

/*   Updated: 2021/05/10 13:27:45 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp" 

Session::Session(void) { return; } // private

Session::Session(std::vector<configServer*> config, Authentication * authentication, int fd)
{
    this->confServer = config;
	this->parseRequest = new ParseRequest;
  	this->handler      = new Handler(fd);
  	this->_user.signIn = false;
  	this->authentication = authentication;
	this->fd = fd;
//	fcntl(this->fd, F_SETFL, O_NONBLOCK);
	return; 
}

Session::~Session(void) 
{
	delete this->handler;
	delete this->parseRequest;
	return; 
}

int Session::send_message(void)
{
//	if ((send(this->fd, wr_buf.c_str(), wr_buf.length(), MSG_NOSIGNAL)) < 0)
    if ((write(this->fd, wr_buf.c_str(), wr_buf.length())) < 0)
	{
		this->state = fsm_error;
		wr_buf.clear();
		std::cout << "Client disconnected" << std::endl;
		return 0;
	}
	wr_buf.clear();
	return 1;
}

int Session::do_read(void)
{
	ssize_t read_res;
	char tmp[INBUFSIZE];

	this->buf.clear();

	read_res = read(this->fd, tmp, INBUFSIZE);
	if (read_res <= 0) {
		if (read_res < 0) {
		// internal server error?
		; }
		else
			std::cout << "Client disconnected" << std::endl;
		return 0;
	}
	this->buf.append(tmp, read_res);
	return 1;
}

//void Session::commit(FILE *f)
//{
//	// неразрешенные функции
//	unsigned int ip = this->from_ip;
//	fprintf(f, "From %d.%d.%d.%d:%d\n",
//			(ip>>24 & 0xff), (ip>>16 & 0xff), (ip>>8 & 0xff), (ip & 0xff),
//			this->from_port);
//	fflush(f);
//
//}

void Session::checkAuthentication(void)
{
    data dataRequest = parseRequest->getData();

    if (dataRequest.path == "/home_page/index.html" || dataRequest.path == "/")
        this->_user.signIn = false;

    if (dataRequest.formData->size() != 0) // есть данные для обработки их формы
    {
        if (this->_user.signIn == false) // && dataRequest.path.find("authorize"))
        {
            if (authentication->checkAuthenticationData(dataRequest.formData->find("login")->second, dataRequest.formData->find("password")->second))
            {
                this->_user.signIn = true;
                this->_user.login = dataRequest.formData->find("login")->second;
            }
            else if (authentication->addNewUser(dataRequest.formData->find("loginSignUp")->second, dataRequest.formData->find("passwordSignUp")->second))
            {
                this->_user.signIn = true;
                this->_user.login = dataRequest.formData->find("loginSignUp")->second;
            }
        }
    }
}

void Session::handle_request(fd_set * writefds)
{
    this->request_left = this->parseRequest->addToBuffer((std::string) this->buf);
    if (parseRequest->isRequestReady()) 
    {
        configServer *config = getConfig();
        checkAuthentication();
        this->wr_buf = this->handler->handle(*(config), parseRequest->getData(), this->_user);
        FD_SET(this->fd, writefds); // готовы ли некоторые из их дескрипторов к чтению, готовы к записи или имеют ожидаемое исключительное состояние,
    }
}

void Session::handle_cgi(fd_set * writefds)
{
		this->wr_buf = this->handler->handle();
		FD_SET(this->fd, writefds);
}

bool Session::isRequestLeft(void)
{
	return this->request_left;
}

int  Session::getCgiFd(void) const
{
	return this->handler->getCgiFd();
}

configServer *Session::getConfig(void)
{
    std::string host = parseRequest->getHost();
    int firstConf = -1;
    std::string serverName = host.substr(0, host.find(":"));
    for(size_t i = 0; i < confServer.size(); i++)
    {
        if(confServer[i]->ip == ip)
        {
            for(size_t j = 0; j < confServer[i]->port.size(); j++)
            {
                if(firstConf == -1)
                    firstConf = (int)i;
                if(this->port == confServer[i]->port[j] && serverName == confServer[i]->server_name)
                    return confServer[i];
            }

        }

    }
    return confServer[firstConf];
}
