/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:37:19 by pvivian           #+#    #+#             */
/*   Updated: 2021/06/07 14:34:30 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
    this->_authentication = new Authentication;
	return;
}

Server::~Server(void)
{
	return;
}

void Server::init(const configServer & config)
{
	int opt;
	std::vector<int>sock;
    struct sockaddr_in addr;
	FILE *f;
    opt = 1;
	for(size_t i = 0; i < config.port.size(); i++)
    {
        sock.push_back(socket(AF_INET, SOCK_STREAM, 0));
        if(sock[i] == -1)
            throw std::runtime_error("Could not create a socket");
        setsockopt(sock[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(config.ip.c_str());
        if (addr.sin_addr.s_addr == (unsigned int)-1)
            throw std::runtime_error("Invalid server address");
        addr.sin_port = htons(config.port[i]);
        if(bind(sock[i], (struct sockaddr*) &addr, sizeof(addr)) == -1)
            throw std::runtime_error("Could not bind socket"); //// проверить ошибку (занят ли этот порт)

        listen(sock[i], LISTEN_QLEN);
        this->listenSockets.push_back(sock[i]);

        f = fopen((config.server_name + "_log").c_str(), "wb");  /// logfile name
        if(!f) {
            close(sock[i]);
            throw std::runtime_error("Could not create a log file");
        }
        this->res = f;
        this->config = config;
    }

}

Session * Server::make_new_session(int fd, struct sockaddr_in *from)
{
	Session *sess = new Session(this->config, this->_authentication);
	sess->fd = fd;
	sess->from_ip = ntohl(from->sin_addr.s_addr);
	sess->from_port = ntohs(from->sin_port);
	sess->state = fsm_start;

	return sess;
}

std::vector<int> Server::getListenSockets(void) const
{
	return this->listenSockets;
}
