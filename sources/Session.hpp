/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/30 18:12:58 by pvivian          ###   ########.fr       */
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
    char **env;

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
	bool isRequestLeft(void);


private:

	Session(void);
    user    _user;
};

#endif
