/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:27:49 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/06 14:48:02 by pvivian          ###   ########.fr       */
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