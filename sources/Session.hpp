/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/06/10 17:06:15 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"
# include "Handler.hpp"
# include "Authentication.hpp"

enum states {
    fsm_start, fsm_finish, fsm_error, fsm_request
};

class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    std::string buf;
	std::string wr_buf;
    enum states state;

private:
	ParseRequest *      parseRequest;
	Handler *           handler;
    Authentication *    authentication;
	bool                request_left;
	

public:

   Session(configServer config, Authentication * authentication);
    ~Session(void);
	int send_message(void);
	int do_read(void);
	void commit(FILE *f);
	void handle_request(fd_set * writefds);
	void handle_cgi(fd_set * writefds);
	bool isRequestLeft(void);
	bool isCgi(void) const;


private:

	Session(void);
    user    _user;
};

#endif
