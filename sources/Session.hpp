/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbroccol <gbroccol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/13 15:55:10 by gbroccol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"
# include "Handler.hpp"


enum states {
    fsm_start, fsm_finish, fsm_error, fsm_request
};

class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    char buf[INBUFSIZE];
	std::string wr_buf;
    enum states state;
    char **env;

//private:
	ParseRequest *  parseRequest;
	Handler *       handler;
	bool            request_left;
	

public:

   Session(configServer config);
    ~Session(void);
	int send_message(void);
	int do_read(void);
	void commit(FILE *f);
	void handle_request(fd_set * writefds);


private:

	Session(void);
};

#endif
