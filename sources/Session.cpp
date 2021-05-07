/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:43:31 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/06 18:16:39 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp" 

Session::Session(void) 
{
	flock.l_type = F_UNLCK;
   	flock.l_whence = SEEK_SET;
    // flock.l_start;   /* Начальное смещение блокировки */
    flock.l_len = 0;
	return; 
}

Session::~Session(void) { return; }

int Session::send_message(void)
{
	if ((write(this->fd, wr_buf.c_str(), wr_buf.length())) < 0)
	{
		this->state = fsm_error;
		return 0;
	}
	fcntl(this->fd, F_SETLK, this->flock);
	// this->state = fsm_finish;
	// return 0;
	return 1;
}

int Session::do_read(void)
{
	int read_res, bufp = this->buf_used;
	ParseRequest parseRequest;
	read_res = read(this->fd, this->buf + bufp, INBUFSIZE - bufp);
	if(read_res <= 0) {
		this->state = fsm_error;
		return 0;
	}

	//parse
	
	try {
	parseRequest.addToBuffer((std::string)this->buf);
	}
	catch (int const & e)
	{
		return 2;
	}
		
	this->buf_used += read_res;
	// if(this->buf_used >= INBUFSIZE) {
	// 	send_message("Line too long! Good bye...\n");
	// 	this->state = fsm_error;
	// 	return 0;
	// }
	if(this->state == fsm_finish)
		return 0;
	return 1;
}

void Session::do_write(const char *str, fd_set * writefds)
{
	this->wr_buf = (std::string)str;
	FD_SET(this->fd, writefds);
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