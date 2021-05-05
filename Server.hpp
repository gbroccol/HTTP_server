/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:28:09 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 14:57:33 by pvivian          ###   ########.fr       */
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
	int listenSocket;
	FILE *res;
	// std::vector<Session *> sessions;
	Session ** sessions;
	int sessions_count;

public:
	void init(const Config & config)
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
		addr.sin_port = htons(config.getPort());
		if(bind(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1)
			throw std::runtime_error("Could not bind socket");

		listen(sock, LISTEN_QLEN);
		this->listenSocket = sock;

		f = fopen(config.getLogFile(), "wb");
		if(!f) {
			close(sock);
			throw std::runtime_error("Could not create a log file");
		}
		this->res = f;

		this->sessions = (Session **)malloc(sizeof(Session*) * INIT_SESS_ARR_SIZE);
   		this->sessions_count = INIT_SESS_ARR_SIZE;
   	 	for(int i = 0; i < INIT_SESS_ARR_SIZE; i++)
        	this->sessions[i] = NULL;
		// this->sessions = std::vector<Session *>(INIT_SESS_ARR_SIZE, NULL);
		// this->sessions_count = INIT_SESS_ARR_SIZE;
	}
	
	int run(void)
	{
		fd_set readfds;
		int i, sr, ssr, maxfd;
		for(;;) {
			FD_ZERO(&readfds);
			FD_SET(this->listenSocket, &readfds);
			maxfd = this->listenSocket;
			for(i = 0; i < this->sessions_count; i++) {
				if(this->sessions[i]) {
					FD_SET(i, &readfds);
					if(i > maxfd)
						maxfd = i;
				}
			}
			sr = select(maxfd+1, &readfds, NULL, NULL, NULL);
			if (sr == -1) {
				// perror("select");
				return 4;
			}
			if (FD_ISSET(this->listenSocket, &readfds))
				accept_client();
			for (i = 0; i < this->sessions_count; i++) {
				if (this->sessions[i] && FD_ISSET(i, &readfds)) {
					ssr = this->sessions[i]->do_read();
					if (!ssr)
						close_session(i);
				}
			}
		}
		return 0;
	}


	Session * make_new_session(int fd, struct sockaddr_in *from)
	{
		Session *sess = (Session *)malloc(sizeof(Session));
		sess->fd = fd;
		sess->from_ip = ntohl(from->sin_addr.s_addr);
		sess->from_port = ntohs(from->sin_port);
		sess->buf_used = 0;
		sess->state = fsm_start;

		sess->send_string("HTTP/1.1 200 OK\r\n");
		return sess;
	}

	void accept_client(void)
	{
		int sd, i;
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		sd = accept(this->listenSocket, (struct sockaddr*) &addr, &len);
		if (sd == -1) {
			// perror("accept");
			return;
		}

		if (sd >= this->sessions_count) 
		{
			// this->sessions_count.resize(this->sessions_count, NULL);
			int newlen = this->sessions_count;
			while(sd >= newlen)
				newlen += INIT_SESS_ARR_SIZE;
			this->sessions =
				(Session **)realloc(this->sessions, newlen * sizeof(Session *));
			for(i = this->sessions_count; i < newlen; i++)
				this->sessions[i] = NULL;
			this->sessions_count = newlen;
		}
		
		this->sessions[sd] = make_new_session(sd, &addr);
    	// this->sessions[sd]->make_new_session(sd, &addr);
	}
	
	void remove_session(int sd)
	{
		close(sd);
		this->sessions[sd]->fd = -1;
		free(this->sessions[sd]);
		this->sessions[sd] = NULL;
	}

	void close_session(int sd)
	{
		if(this->sessions[sd]->state == fsm_finish)
			this->sessions[sd]->commit(this->res);
		remove_session(sd);
	}
};

#endif