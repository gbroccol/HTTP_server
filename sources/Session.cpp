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

Session::Session(configServer config, Authentication * authentication)
{
	this->parseRequest = new ParseRequest;
    this->handler      = new Handler(config);
    this->_signIn      = false;
    this->authentication = authentication;
	return; 
}

Session::~Session(void) { return; }

int Session::send_message(void)
{
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
	if ((write(this->fd, wr_buf.c_str(), wr_buf.length())) < 0)
	{
		this->state = fsm_error;
		return 0;
	}
	wr_buf.clear();
	// this->state = fsm_finish;
	// return 0;
	return 1;
}

int Session::do_read(void)
{
	int read_res;
	read_res = read(this->fd, this->buf, INBUFSIZE);
	if(read_res < 0) {
		this->state = fsm_error; // internal server error
		return 0;
	}
	else if (read_res == 0)
	{
		// передать в парсер информацию о том, что сообщение закончено
		// this->state = fsm_finish;
		return 0;
	}
	this->buf[read_res] = 0;

//  if(this->state == fsm_finish)
//    return 0;
	return 1;
}

void Session::commit(FILE *f)
{
	// неразрешенные функции
	unsigned int ip = this->from_ip;
	fprintf(f, "From %d.%d.%d.%d:%d\n",
			(ip>>24 & 0xff), (ip>>16 & 0xff), (ip>>8 & 0xff), (ip & 0xff),
			this->from_port);
	fflush(f);
	
}

void Session::handle_request(fd_set * writefds)
{
//    data tmp = parseRequest->getData();

    this->request_left = this->parseRequest->addToBuffer((std::string) this->buf);
    if (parseRequest->getData().status == REQUEST_READY)
    {
//        std::cout << GREEN << "_________________  REQUEST_READY  _________________________" << BW << std::endl << std::endl;
//
//        std::cout << GREEN << tmp.path << BW << std::endl << std::endl;
//
        std::multimap <std::string, std::string>::iterator itCL = parseRequest->getData().headers->find("Authorization");
//
//        std::multimap <std::string, std::string>::iterator printB = parseRequest->getData().headers->begin();
//        std::multimap <std::string, std::string>::iterator printEND = parseRequest->getData().headers->end();
//
//        while (printB != printEND)
//        {
//            std::cout << "FIRST -> " << printB->first << " SECOND -> " << printB->second << std::endl;
//            printB++;
//        }

        if (itCL != parseRequest->getData().headers->end())
        {
            std::cout << "_signIn ON" << std::endl;
            this->_signIn = true;
        }
        this->wr_buf = this->handler->handle(parseRequest->getData(), this->env, this->_signIn);
        FD_SET(this->fd, writefds); // готовы ли некоторые из их дескрипторов к чтению, готовы к записи или имеют ожидаемое исключительное состояние,
    }

//    data tmp = parseRequest->getData();
//
//	this->request_left = this->parseRequest->addToBuffer((std::string) this->buf);
//	if (tmp.status == REQUEST_READY)
//	{
//
//        this->wr_buf = this->handler->handle(tmp, this->env, this->_signIn);
//        FD_SET(this->fd, writefds); // готовы ли некоторые из их дескрипторов к чтению, готовы к записи или имеют ожидаемое исключительное состояние,
//    }
}

bool Session::isRequestLeft(void)
{
	return this->request_left;
}
