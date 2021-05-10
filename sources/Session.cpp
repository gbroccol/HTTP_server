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

Session::Session(void) 
{
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
	// this->state = fsm_finish;
	// return 0;
	return 1;
}

int Session::do_read(void)
{
	int read_res;
	ParseRequest parseRequest;
	read_res = read(this->fd, this->buf, INBUFSIZE);
	if(read_res < 0) {
		this->state = fsm_error;
		return 0;
	}
	else if (read_res == 0)
	{
		// передать в парсер информацию о том, что сообщение закончено
		// this->state = fsm_finish;
	}

	//parse
	
	try {
	parseRequest.addToBuffer((std::string)this->buf);
	}
	catch (int const & e)
	{
		// сообщить основному процессу, что можно запускать обработчик запроса
		return 2;
	}
	
	if(this->state == fsm_finish)
		return 0;
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
	// this->wr_buf = this->handler.handle(заполненная структура с запросом);
	this->wr_buf = this->handler.handle();
	FD_SET(this->fd, writefds);
}
