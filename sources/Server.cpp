#include "Server.hpp"


Server::Server(void)
{
    this->_authentication = new Authentication;
	return;
}

Server::~Server(void)
{
	delete this->_authentication;
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
        addr.sin_addr.s_addr = config.ip;
        if (addr.sin_addr.s_addr == (unsigned int)-1)
            throw std::runtime_error("Invalid server address");
        addr.sin_port = config.port[i];
        if(bind(sock[i], (struct sockaddr*) &addr, sizeof(addr)) == -1)
        {
            if (errno == EADDRINUSE)
                continue;
            throw std::runtime_error("Could not bind socket");
        }

        listen(sock[i], LISTEN_QLEN);
        this->listenSockets.push_back(sock[i]);
        this->addrs.push_back(addr);

        f = fopen((config.server_name + "_log").c_str(), "wb");  /// logfile name
        if(!f) {
            close(sock[i]);
            throw std::runtime_error("Could not create a log file");
        }
        this->res = f;
        this->config = config;
    }

}

Authentication * Server::getAuth(void) const
{
    return this->_authentication;
}


std::vector<int> Server::getListenSockets(void) const
{
	return this->listenSockets;
}

std::vector<struct sockaddr_in> Server::getAddrs(void) const
{
    return this->addrs;
}
