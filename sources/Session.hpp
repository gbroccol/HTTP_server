/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:29:00 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/07 16:28:51 by pvivian          ###   ########.fr       */
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
	std::string wr_buf;
    enum fsm_states state;
	

public:
	Session(void);
	~Session(void);
	int send_message(void);
    std::string         getPresentTime();
	std::string getLastModificationFile(std::string fileName);
	void do_write(const char *str, fd_set * writefds);
	int do_read(void);
	void check_lf(void);
	void commit(FILE *f);
};

#endif
