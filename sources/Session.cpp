/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:43:31 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 18:55:54 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

Session::Session(void)
{
	return;
}

Session::~Session(void)
{
	return;
}

// Session * Session::make_new_session(int fd, struct sockaddr_in *from)
// {
// 	this->fd = fd;
// 	this->from_ip = ntohl(from->sin_addr.s_addr);
// 	this->from_port = ntohs(from->sin_port);
// 	this->buf_used = 0;
// 	this->state = fsm_start;

// 	send_string("HTTP/1.1 200 OK\r\n");
// 	return this;
// }

void Session::send_string(const char *str)
{
	write(this->fd, str, strlen(str));
}

int Session::do_read(void)
{
	int rc, bufp = this->buf_used;
	ParseRequest parseRequest;
	rc = read(this->fd, this->buf + bufp, INBUFSIZE - bufp);
	if(rc <= 0) {
		this->state = fsm_error;
		return 0;
	}

	//parse
	parseRequest.addToBuffer((std::string)this->buf);
	// printf("%s\n", this->buf); // for debug
		
	this->buf_used += rc;
	// check_lf();
	if(this->buf_used >= INBUFSIZE) {
		send_string("Line too long! Good bye...\n");
		this->state = fsm_error;
		return 0;
	}
	if(this->state == fsm_finish)
		return 0;
	return 1;
}

// void Session::check_lf(void)
// {
// 	int pos = -1;
// 	int i;
// 	char *line;
	
// 	for(i = 0; i < this->buf_used; i++) {
// 		if(this->buf[i] == '\n') {
// 			pos = i;
// 			break;
// 		}
// 	}
// 	if(pos == -1)
// 		return;
// 	line = (char *)malloc(pos+1);
// 	memcpy(line, this->buf, pos);
// 	line[pos] = 0;
// 	this->buf_used -= (pos+1);
// 	memmove(this->buf, this->buf+pos+1, this->buf_used);
// 	if(line[pos-1] == '\r')
// 		line[pos-1] = 0;
// 	// session_fsm_step(this, line);  /* we transfer ownership! */
// }

void Session::commit(FILE *f)
{
	// неразрешенные функции
	unsigned int ip = this->from_ip;
	fprintf(f, "From %d.%d.%d.%d:%d\n",
			(ip>>24 & 0xff), (ip>>16 & 0xff), (ip>>8 & 0xff), (ip & 0xff),
			this->from_port);
	fflush(f);
	
}