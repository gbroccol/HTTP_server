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

int main(void)
{
    Server server;
	Config config;


// parse config and set attributes
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