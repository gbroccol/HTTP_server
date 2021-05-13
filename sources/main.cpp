/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbroccol <gbroccol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:27:49 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/13 15:50:38 by gbroccol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
# include "Server.hpp"
# include "Config.hpp"


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