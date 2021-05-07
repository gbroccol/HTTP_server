/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:37:19 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/07 14:47:33 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	this->sessions = std::vector<Session *>(INIT_SESS_ARR_SIZE, NULL);
	
	return;
}

Server::~Server(void)
{
	close_all_sessions();
	return;
}

void Server::init(const configServer & config)
{
	int sock, opt;
	struct sockaddr_in addr;
	FILE *f;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		throw std::runtime_error("Could not create a socket");

	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(config.port);
	if(bind(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1)
		throw std::runtime_error("Could not bind socket");

	listen(sock, LISTEN_QLEN);
	this->listenSocket = sock;

	f = fopen(config.server_name.c_str(), "wb");  /// logfile name
	if(!f) {
		close(sock);
		throw std::runtime_error("Could not create a log file");
	}
	this->res = f;
}
	
void Server::run(void)
{
	int i;
	int sr, ssr, maxfd;
	std::map<int, Session *>::iterator it;
	std::map<int, Session *>::iterator temp;
	
	for(;;) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(this->listenSocket, &readfds);
		maxfd = this->listenSocket;
		for(i = 0; i < (int)this->sessions.size(); i++) {
			if(this->sessions[i]) {
				FD_SET(i, &readfds);
				if(i > maxfd)
					maxfd = i;
			}
		}
		// for (it = this->sessions.begin(); it != this->sessions.end(); it++)
		// {
		// 	FD_SET(it->first, &readfds);
		// 	if (it->first > maxfd)
		// 			maxfd = it->first;
		// }
		
		sr = select(maxfd+1, &readfds, &writefds, NULL, NULL);
		if (sr == -1)
			throw std::runtime_error("Select error");
		if (FD_ISSET(this->listenSocket, &readfds))
			accept_client();
		for (i = 0; i < (int)this->sessions.size(); i++) 
		{
			if (this->sessions[i] && FD_ISSET(i, &readfds)) {
				ssr = this->sessions[i]->do_read();
				if (ssr == 2)
					this->sessions[i]->do_write("HTTP/1.1 200 OK\r\n\r\n<html><head>Welcome</head></html>\n", &writefds);
				else if (!ssr)
					close_session(i);
			}
			if (this->sessions[i] && FD_ISSET(i, &writefds)) {
				ssr = this->sessions[i]->send_message();
				if (!ssr)
					close_session(i);
			}
		}
		// for (it = this->sessions.begin(); it != this->sessions.end(); it++) 
		// {
		// 	if (FD_ISSET(it->first, &readfds)) {
		// 		ssr = it->second->do_read();
		// 		if (ssr == 2)
		// 			it->second->do_write("HTTP/1.1 200 OK\r\n\r\n<html><head>Welcome</head></html>\n", &writefds);
		// 		else if (!ssr)
		// 		{
		// 			temp = it;
		// 			++it;
		// 			close_session(temp);
		// 			--it;
		// 		}
		// 	}
		// 	if (FD_ISSET(it->first, &writefds)) {
		// 		ssr = it->second->send_message();
		// 		if (!ssr)
		// 		{
		// 			temp = it;
		// 			++it;
		// 			close_session(temp);
		// 			--it;
		// 		}
		// 	}
		// }
	}
}

void Server::accept_client(void)
{
	int sd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	sd = accept(this->listenSocket, (struct sockaddr*) &addr, &len);
	if (sd == -1)
		throw std::runtime_error("Accept error");

	if (sd >= (int)this->sessions.size()) 
	{
		int newlen = this->sessions.size();
		while(sd >= newlen)
			newlen += INIT_SESS_ARR_SIZE;
		this->sessions.resize(newlen, NULL);
	}
	
	this->sessions[sd] = make_new_session(sd, &addr);
	// this->sessions.insert(std::make_pair(sd, make_new_session(sd, &addr)));
}

Session * Server::make_new_session(int fd, struct sockaddr_in *from)
{
	Session *sess = (Session *)malloc(sizeof(Session));
	sess->fd = fd;
	sess->from_ip = ntohl(from->sin_addr.s_addr);
	sess->from_port = ntohs(from->sin_port);
	sess->buf_used = 0;
	sess->state = fsm_start;

	return sess;
}

void Server::remove_session(int sd)
{
	close(sd);
	this->sessions[sd]->fd = -1;
	free(this->sessions[sd]);
	this->sessions[sd] = NULL;
}

void Server::close_session(int sd)
{
	if (this->sessions[sd]->state == fsm_finish)
		this->sessions[sd]->commit(this->res);
	close(sd);
	free(this->sessions[sd]);
	this->sessions[sd] = NULL;
}


// void Server::close_session(std::map<int, Session *>::iterator it)
// {
// 	if (it->second->state == fsm_finish)
// 		it->second->commit(this->res);
// 	close(it->first);
// 	this->sessions.erase(it);
// }

void Server::close_all_sessions(void)
{
	for (size_t i = 0; i < this->sessions.size(); i++)
	{
		if (this->sessions[i])
			close_session(i);
	}
	// std::map<int, Session *>::iterator it;
	// std::map<int, Session *>::iterator temp;
	// for (it = this->sessions.begin(); it !=  this->sessions.end(); it++)
	// {
	// 	temp = it;
	// 	--it;
	// 	close_session(temp);
	// }
}
