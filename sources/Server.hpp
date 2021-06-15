/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:28:09 by pvivian           #+#    #+#             */
/*   Updated: 2021/06/07 14:34:42 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Webserv.hpp"
# include "Session.hpp"
# include "Config.hpp"
# include "Authentication.hpp"

class Server
{
private:
	std::vector<int> 		listenSockets;
    std::vector<struct sockaddr_in> addrs;
	FILE 					*res;
	configServer			config;
    Authentication *        _authentication;

public:
	Server(void);
	~Server(void);
	void init(const configServer & config);
	std::vector<int> getListenSockets(void) const;
    std::vector<struct sockaddr_in> getAddrs(void) const;

    Authentication * getAuth(void) const;

};

#endif
