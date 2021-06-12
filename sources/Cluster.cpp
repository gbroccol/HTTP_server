# include "Cluster.hpp"

Cluster::Cluster(void) 
{
	this->sessions = std::vector<Session *>(INIT_SESS_ARR_SIZE, NULL);
}
Cluster::~Cluster(void)
{
	close_all_sessions();
	return;
}
void Cluster::init(const Config & config) 
{
	configServer *confServer;
	Server * server;
	std::vector<int> sockets;

	for (size_t  i = 0; i < config.getSize(); i++)
	{
		confServer = config.getconfigServer((int)i);
		server = new Server;
		server->init(*confServer);
		servers.push_back(server);
		sockets = server->getListenSockets();
		listenSockets.insert(listenSockets.end(), sockets.begin(), sockets.end());
	}
}

void Cluster::run(void)
{
	int i, sr, ssr, maxfd;
	int cgiFd;

	// fcntl(1, F_SETFL, O_NONBLOCK);


	for(;;) {
		FD_ZERO(&readfds);                  // зачистить сет для чтения
		FD_ZERO(&writefds);                 // зачистить сет для записи
		for (size_t i = 0; i < this->listenSockets.size(); i++)
			FD_SET(this->listenSockets[i], &readfds);
		maxfd = this->listenSockets.back();  // sega
		for(i = 0; i < (int)sessions.size(); i++) {
			if(sessions[i]) {
				FD_SET(i, &readfds);
				if(i > maxfd)
					maxfd = i;
				if ((cgiFd = sessions[i]->getCgiFd()) > 0)
				{
					FD_SET(cgiFd, &readfds);
					if(cgiFd > maxfd)
						maxfd = cgiFd;
				}
			}
		}

		sr = select(maxfd+1, &readfds, &writefds, NULL, NULL);
		if (sr == -1)
		    throw std::runtime_error("Select error");
		for (size_t i = 0; i < this->listenSockets.size(); i++) 
			if (FD_ISSET(this->listenSockets[i], &readfds))
				accept_client(i);
		for (size_t i = 0; i < sessions.size(); i++) 
		{
			if (sessions[i]) {
				if (sessions[i]->getCgiFd() > 0 && FD_ISSET(sessions[i]->getCgiFd(), &readfds))
					sessions[i]->handle_cgi(&writefds);
				else if (FD_ISSET(i, &readfds)) {
					ssr = sessions[i]->do_read();
					if (ssr == 1 || sessions[i]->isRequestLeft()) {
						sessions[i]->handle_request(&writefds);
					}
					else if (!ssr)
						closeSession(i);
				}
				if (FD_ISSET(i, &writefds)) {
					ssr = sessions[i]->send_message();
					if (!ssr)
						closeSession(i);
				}
			}
		}
	}
}

void Cluster::accept_client(int pos)
{
	int sd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	sd = accept(this->listenSockets[pos], (struct sockaddr*) &addr, &len);
	if (sd == -1)
		throw std::runtime_error("Accept error");

	if (sd >= (int)this->sessions.size()) 
	{
		int newlen = this->sessions.size();
		while(sd >= newlen)
			newlen += INIT_SESS_ARR_SIZE;
		this->sessions.resize(newlen, NULL);
	}
	int serverNum = getServerNum(listenSockets[pos]);
	this->sessions[sd] = servers[serverNum]->make_new_session(sd, &addr);
}

int Cluster::getServerNum(int pos)
{
	std::vector<int> sockets;
	int serverNum;
	for (size_t i = 0; i < servers.size(); i++)
	{
		sockets = servers[i]->getListenSockets();
		for (size_t j = 0;  j < sockets.size(); j++)
		{
			if (sockets[j] == pos)
			{
				serverNum = i;
				break;
			}
		}
	}
	return serverNum;
}

void Cluster::closeSession(int sd)
{
	// if (this->sessions[sd]->state == fsm_finish)
	// 	this->sessions[sd]->commit(this->res);
	close(sd);
	delete this->sessions[sd];
	// free(this->sessions[sd]);
	this->sessions[sd] = NULL;
}

void Cluster::close_all_sessions(void)
{
	for (size_t i = 0; i < this->sessions.size(); i++)
	{
		if (this->sessions[i])
			closeSession(i);
	}
}