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
	std::vector<int>sock;
    struct sockaddr_in addr;
    int opt = 1;

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
		errno = 0;
        if(bind(sock[i], (struct sockaddr*) &addr, sizeof(addr)) == -1)
        {
            if (errno == EADDRINUSE)
            {
                std::cout<<"Address already in use"<<std::endl;
                continue;
            }
            throw std::runtime_error("Could not bind socket");
        }

        listen(sock[i], LISTEN_QLEN);
        this->listenSockets.push_back(sock[i]);
        this->config = config;
    }

}

Session * Server::make_new_session(int fd)
{
	Session *sess = new Session(this->config, this->_authentication, fd);
	return sess;
}

std::vector<int> Server::getListenSockets(void) const
{
	return this->listenSockets;
}
