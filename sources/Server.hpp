/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:28:09 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/07 13:05:01 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Webserv.hpp"
# include "Session.hpp"
# include "ParseConfig.hpp"
# include "Config.hpp"

class Server
{
private:
	int listenSocket;
	FILE *res;
	// std::vector<Session *> sessions;
	std::map<int, Session *> sessions;
	fd_set readfds;
	fd_set writefds;

public:
	Server(void);
	~Server(void);
	void init(const Config & config);
	void run(void);
	void accept_client(void);
	Session * make_new_session(int fd, struct sockaddr_in *from);
	// void remove_session(int sd);
	// void close_session(int sd);
	void close_session(std::map<int, Session *>::iterator it);
	void close_all_sessions(void);
};

#endif