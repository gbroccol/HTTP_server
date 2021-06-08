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
	void accept_client(int pos);
	int getServerNum(int pos);
	void closeSession(int sd);
	void close_all_sessions(void);
};

#endif
