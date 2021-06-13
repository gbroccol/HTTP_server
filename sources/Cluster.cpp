# include "Cluster.hpp"

Cluster::Cluster(void) 
{
	this->sessions = std::vector<Session *>(INIT_SESS_ARR_SIZE, NULL);
	return;
}
Cluster::~Cluster(void)
{
	closeAllSessions();
	for (size_t i = 0; i < servers.size(); i++)
		delete servers[i];
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
	return;
}

void Cluster::run(void)
{
	size_t i;
	int sr, ssr, maxfd;

	// fcntl(1, F_SETFL, O_NONBLOCK);
	// signal(SIGINT, signalHandler());

	for(;;) {
		updateSelectSets(&maxfd);
		sr = select(maxfd+1, &readfds, &writefds, NULL, NULL);
		if (sr == -1)
		    throw std::runtime_error("Select error");
		for (i = 0; i < this->listenSockets.size(); i++) 
			if (FD_ISSET(this->listenSockets[i], &readfds))
				acceptClient(i);
		for (i = 0; i < sessions.size(); i++) 
		{
			if (sessions[i]) {
				if (sessions[i]->getCgiFd() > 0 && FD_ISSET(sessions[i]->getCgiFd(), &readfds))
					sessions[i]->handle_cgi(&writefds);
				else if (FD_ISSET(i, &readfds)) {
					ssr = sessions[i]->do_read();
					if (ssr == 1 || sessions[i]->isRequestLeft())
						sessions[i]->handle_request(&writefds);
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
	return;
}

void Cluster::updateSelectSets(int * maxfd)
{
	int cgiFd = 0;
	size_t i = 0;

	FD_ZERO(&readfds);                  // зачистить сет для чтения
	FD_ZERO(&writefds);                 // зачистить сет для записи
	for (; i < this->listenSockets.size(); i++)
		FD_SET(this->listenSockets[i], &readfds);
	*maxfd = this->listenSockets.back();

	for(i = 0; i < sessions.size(); i++) {
		if(sessions[i]) {
			FD_SET(i, &readfds);
			if((int)i > *maxfd)
				*maxfd = i;
			if ((cgiFd = sessions[i]->getCgiFd()) > 0)
			{
				FD_SET(cgiFd, &readfds);
				if(cgiFd > *maxfd)
					*maxfd = cgiFd;
			}
		}
	}
	return;
}

void Cluster::acceptClient(int pos)
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
	this->sessions[sd] = servers[serverNum]->make_new_session(sd);
	return;
}

int Cluster::getServerNum(int pos)
{
	std::vector<int> sockets;
	int serverNum = 0;
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
	close(sd);
	delete this->sessions[sd];
	this->sessions[sd] = NULL;
	return;
}

void Cluster::closeAllSessions(void)
{
	for (size_t i = 0; i < this->sessions.size(); i++)
	{
		if (this->sessions[i])
			closeSession(i);
	}
	return;
}

// sighandler_t Cluster::signalHandler()
// {
// 	closeAllSessions();
// 	for (size_t i = 0; i < servers.size(); i++)
// 		delete servers[i];
// 	return SIG_DFL;
// 	// exit(EXIT_SUCCESS);
// }