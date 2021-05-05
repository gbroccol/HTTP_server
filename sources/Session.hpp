/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 18:36:38 by pvivian          ###   ########.fr       */
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
	Session(void);
	~Session(void);
	// Session * make_new_session(int fd, struct sockaddr_in *from);
	void send_string(const char *str);
	int do_read(void);
	void check_lf(void);
	void commit(FILE *f);	
};

#endif