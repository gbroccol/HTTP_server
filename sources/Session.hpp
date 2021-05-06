/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/06 17:42:15 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"

enum fsm_states {
    fsm_start, fsm_finish, fsm_error
};

struct Flock {
	short l_type;
   	short l_whence;
    off_t l_start;
    off_t l_len;
};

class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    char buf[INBUFSIZE];
    int buf_used;
	std::string wr_buf;
    enum fsm_states state;
	Flock flock;
	

public:
	Session(void);
	~Session(void);
	int send_message(void);
	void do_write(const char *str, fd_set * writefds);
	int do_read(void);
	void check_lf(void);
	void commit(FILE *f);	
};

#endif