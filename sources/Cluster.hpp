#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Webserv.hpp"
# include "Server.hpp"
# include "Session.hpp"
# include "Config.hpp"
# include "Authentication.hpp"

class Cluster
{
private:

	fd_set 					readfds;
	fd_set 					writefds;
	std::vector<Server *> 	servers;
	std::vector<int> 		listenSockets;
	std::vector<Session *>	sessions;

public:
	Cluster(void);
	~Cluster(void);
	void init(const Config & config);
	void run(void);
	void updateSelectSets(int * maxfd);
	void acceptClient(int pos);
	int getServerNum(int pos);
	void closeSession(int sd);
	void closeAllSessions(void);
	// sighandler_t signalHandler(void);
};



#endif
