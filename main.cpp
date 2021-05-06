/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:27:49 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 14:30:03 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
# include "Server.hpp"
# include "Session.hpp"
# include "Config.hpp"
# include "ParseRequest.hpp"

int main(int argc,  char **argv)
{
    Server server;
	Config config;
	


// parse config and set attributes
	if(argc != 2)
	{
		std::cout << "Incorrect argc"<<std::endl;
		return -1;
	}
	config.getFile(argv[1]);
	
	char filename[4] = {'l', 'o', 'g'};
	config.setPort(8080);
	config.setLogFile(filename);

// init server
	try 
	{
		server.init(config);
	}
    catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
		
//run server
    return server.run();
}