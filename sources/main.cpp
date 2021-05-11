/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:27:49 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/11 18:47:25 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
# include "Server.hpp"
# include "Session.hpp"
# include "Config.hpp"
# include "ParseRequest.hpp"

int isLocation(std::vector<location *> locations, std::string path)
{
	int theBestLocation = -1;
	for(size_t i = 0; i < locations.size(); i++)
	{
		if(locations[i]->path == path)
		{
			theBestLocation = i;
			std::cout << theBestLocation<<std::endl;
			return i;
		}
		else
		{
			for(size_t j = 0; j < path.length(); j++)
				if(path[j] == locations[i]->path[j])
					if(j == locations[i]->path.length() - 1)
						if(((path[j] == path[path.length() - 1]) || path[j + 1] == '/') || j == 0)
						{

							if(theBestLocation == -1 ||(theBestLocation >= 0 && locations[i]->path.length() > locations[theBestLocation]->path.length()))
								theBestLocation = i;
						}
		}
	}
	std::cout << theBestLocation<<std::endl;
	return theBestLocation;
}

int main(int argc,  char **argv)
{
	std::vector<Server *> servers;
	Server * server;
	Config config;
	configServer *confServer;
	// char *configFile;

	try 
	{
		// если аргументов нет (argc == 1), configFile = default path
		// В ином случае configFile = argv[1]
		if(argc != 2)
		{
			std::cout << "Incorrect argc"<<std::endl;
			return -1;
		}
		config.getFile(argv[1]);
		
		for (size_t i = 0; i < config.getSize(); i++)
		{
			confServer = config.getconfigServer((int)i);
			isLocation(confServer->locations, "/abc");
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