/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 15:29:15 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"

enum fsm_states {
    fsm_start, fsm_finish, fsm_error
};

class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    char buf[INBUFSIZE];
    int buf_used;
    enum fsm_states state;

public:
	Session(void)
	{
		return;
	}

	// Session * make_new_session(int fd, struct sockaddr_in *from)
	// {
	// 	this->fd = fd;
	// 	this->from_ip = ntohl(from->sin_addr.s_addr);
	// 	this->from_port = ntohs(from->sin_port);
	// 	this->buf_used = 0;
	// 	this->state = fsm_start;

	// 	send_string("HTTP/1.1 200 OK\r\n");
	// 	return this;
	// }

	void send_string(const char *str)
	{
   		write(this->fd, str, strlen(str));
	}

	int do_read()
	{
		int rc, bufp = this->buf_used;
		ParseRequest parseRequest;
		rc = read(this->fd, this->buf + bufp, INBUFSIZE - bufp);
		if(rc <= 0) {
			this->state = fsm_error;
			return 0;
		}

		//parse?
		parseRequest.addToBuffer((std::string)this->buf);
		// printf("%s\n", this->buf); // for debug
		
		this->buf_used += rc;
		check_lf();
		if(this->buf_used >= INBUFSIZE) {
			/* we can't read further, no room in the buffer, no whole line yet */
			send_string("Line too long! Good bye...\n");
			this->state = fsm_error;
			return 0;
		}
		if(this->state == fsm_finish)
			return 0;
		return 1;
	}

	void check_lf()
	{
		int pos = -1;
		int i;
		char *line;
		for(i = 0; i < this->buf_used; i++) {
			if(this->buf[i] == '\n') {
				pos = i;
				break;
			}
		}
		if(pos == -1)
			return;
		line = (char *)malloc(pos+1);
		memcpy(line, this->buf, pos);
		line[pos] = 0;
		this->buf_used -= (pos+1);
		memmove(this->buf, this->buf+pos+1, this->buf_used);
		if(line[pos-1] == '\r')
			line[pos-1] = 0;
		// session_fsm_step(this, line);  /* we transfer ownership! */
		// we should call parser
	}

	void commit(FILE *f)
	{
		unsigned int ip = this->from_ip;
		fprintf(f, "From %d.%d.%d.%d:%d\n",
				(ip>>24 & 0xff), (ip>>16 & 0xff), (ip>>8 & 0xff), (ip & 0xff),
				this->from_port);
		fflush(f);
	}

	
};

#endif