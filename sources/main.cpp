
#include "Webserv.hpp"
# include "Server.hpp"
# include "Config.hpp"

int main(int argc,  char **argv)
{
	std::vector<Server *> servers;
	Server * server;
	Config config;
	configServer *confServer;
	std::string configFile = "./config/default.conf";

	try 
	{
		if ( argc == 2)
			configFile = argv[1];
		else if (argc > 2)
		{
			std::cout << "Incorrect argc"<<std::endl;
			return -1;
		}

		config.getFile(configFile);
		
		for (size_t  i = 0; i < config.getSize(); i++)
		{
			confServer = config.getconfigServer((int)i);
			server = new Server;
			server->init(*confServer);
			servers.push_back(server);
		}
		for (size_t i = 0; i < servers.size(); i++)
			servers[i]->run();
	}
    catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}