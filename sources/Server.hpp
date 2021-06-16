/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:28:09 by pvivian           #+#    #+#             */
/*   Updated: 2021/06/12 17:44:05 by pvivian          ###   ########.fr       */
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

	configServer			config;
//    Authentication *        _authentication;

public:
	Server(void);
	~Server(void);
	void init(const configServer & config);
	std::vector<int> getListenSockets(void) const;
    std::vector<struct sockaddr_in> getAddrs(void) const;

//    Authentication * getAuth(void) const;
};

#endif
