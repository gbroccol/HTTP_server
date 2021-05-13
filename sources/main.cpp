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


// int isFiles(std::string path, std::string locPath)
// {
// 	std::string expansion = "";
// 	std::string tmp = "";
// 	int flag = 0;
// 	for(size_t i = 0; i < path.length(); i++)
// 	{
// 		if(path[i] == '.' || flag == 1)
// 		{
// 			expansion.push_back(path[i]);
// 			flag = 1;
// 		}
// 	}
// 	if(locPath == "*" && expansion.length() > 1)
// 		return(1);
// 	else if(locPath[1] == '.' && expansion.length() > 1)
// 	{
// 		tmp = locPath.substr(locPath.find('.'), locPath.length() - 1);
// 		if(tmp == expansion)
// 			return(2);
// 	}
// 	if(locPath.length() > 1 && locPath[0] == '*' && locPath[1] != '.')
// 		std::cout << "ERROR"<<std::endl; // make exception
// 	return (-1);
// }

// int putVal(std::string &locTmp, std::string &reqTmp, std::vector<location *> locations,size_t i, size_t j, int theBestLocation)
// {
// 	locTmp.clear();
// 	reqTmp.clear();
// 	if((theBestLocation == -1 && j == locations[i]->path.length() - 1) || (theBestLocation >= 0 && locations[i]->path.length() > locations[theBestLocation]->path.length()))
// 		return(i);
// 	return (theBestLocation);
// }

// void searchPath(std::string &locTmp, std::string &reqTmp, std::vector<location *> locations,size_t i, size_t &j, int &theBestLocation, std::string path)
// {
// 	if(locTmp == reqTmp &&  j == path.length() - 1 )
// 		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
// 	else if(locTmp == reqTmp && reqTmp[j + 1] == '/')
// 		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
// 	else if(j == 0 && j == locations[i]->path.length() - 1)
// 		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
// 	else if(locTmp != reqTmp && locations[i]->path[j] == '/')
// 		if(j + 1 == locations[i]->path.length() && j == path.length())
// 			theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
// 	if(j == 0 && j != locations[i]->path.length() - 1)
// 	{
// 		locTmp.clear();
// 		reqTmp.clear();
// 		j = 0;
// 	}
// }

// int isLocation(std::vector<location *> locations, std::string path)
// {
// 	int theBestLocation = -1;
// 	int tmp = -1;
// 	int flag = 1;
// 	for(size_t i = 0; i < locations.size(); i++)
// 	{
// 		if(locations[i]->path == path)
// 		{
// 			theBestLocation = i;
// 			std::cout << theBestLocation<<std::endl;
// 			return (theBestLocation);
// 		}
// 		else
// 		{
// 			std::string locTmp = "";
// 			std::string reqTmp = "";
// 			for(size_t j = 0; j < locations[i]->path.length(); j++)
// 			{
// 				locTmp.push_back(locations[i]->path[j]);
// 				reqTmp.push_back(path[j]);

// 				if(locations[i]->path[j] == '*' && flag > 0)		
// 				{
// 					flag = isFiles(path.substr(j, path.length() - 1), locations[i]->path.substr(j, path.length() - 1));
// 					if((theBestLocation == -1 && flag != -1) || flag > tmp)
// 					{
// 						tmp = flag;
// 						theBestLocation = i;
// 					}
// 					break ;
// 				}
// 				if(locations[i]->path[j] == '/' || j == locations[i]->path.length() - 1 )
// 					searchPath(locTmp, reqTmp, locations, i, j, theBestLocation, path);
// 			}
// 		}
// 	}
// 	std::cout << theBestLocation<<std::endl;
// 	return(theBestLocation);
// }

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
			isLocation(confServer->locations, "/i.html");
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