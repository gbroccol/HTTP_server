/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:27:49 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/07 13:54:41 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "ParseConfig.hpp"

// int main(int argc,  char **argv)
int main(void)
{
	Server server;
	// std::vector<Server *> servers;
	// Server * server;
	Config config;
	ParseConfig parse;
	
// parse config and set attributes
	// if(argc != 2)
	// 	return -1;
	// parse.parseFile(argv[1]);
	char filename[4] = {'l', 'o', 'g'};
	config.setPort(8080);
	config.setLogFile(filename);

	try 
	{
		// for (std::vector<>::iterator it = .begin(); it != .end(); it++)
		// {
		// 	server = new Server;
		// 	server->init(config);
		// 	servers.push_back(server);
		// }
		// for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); it++)
		// 	&(*it)->run();
		server.init(config);
		server.run();
	}
    catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}