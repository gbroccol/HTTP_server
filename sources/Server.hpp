/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:28:09 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/10 17:15:37 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Webserv.hpp"
# include "Session.hpp"
# include "Config.hpp"

class Server
{
private:
	int 					listenSocket;
	FILE 					*res;
	std::vector<Session *>	sessions;
	configServer			config;
	fd_set 					readfds;
	fd_set 					writefds;

public:
	Server(void);
	~Server(void);
	void init(const configServer & config);
	void run(void);
	void accept_client(void);
	Session * make_new_session(int fd, struct sockaddr_in *from);
	void remove_session(int sd);
	void close_session(int sd);
	void close_all_sessions(void);
};

#endif