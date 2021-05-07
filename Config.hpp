/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:31:32 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/05 14:27:50 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
# include "Session.hpp"
# include "Config.hpp"
# include "ParseRequest.hpp"

#define BW "\033[0;0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define PURPLE "\033[0;35m"

#define SERVER 1
#define LOCATION 0

class Config
{
	struct 						location
	{
		std::string 			path;
		bool 					repeat_path;
		std::string 			index;
		bool 					repeat_index;
		std::string 			root;
		bool 					repeat_root;
		int 					maxBody;
		bool 					repeat_maxBody;
	};

	struct 						configServer
	{
		int 					port;
		bool 					repeat_port;
		std::string 			server_name;
		bool 					repeat_server_name;
		int 					error_page;
		bool 					repeat_error_page;
		std::vector<Config::location*> 	locations;
	};

private:
	long 						_port;
	char 						*_root;
	std::string					_server_name;
	
	std::vector<configServer*> 		servers;
	
public:
	Config();
	~Config();

	long 				getPort(void) const;
	configServer        *getconfigServer(int index);
	// {
	// 	return(servers[index]);
	// }
	char 				*getLogFile(void) const;
	void 				setPort(long const & p);
	void 				setLogFile(char * name);
	bool				checkMainValLoc(location *locNode);
	void				initLocNode(location *locNode);
	void				initServNode(configServer *servNode);
	void				getFile(std::string file);
	std::string			parseStr(std::string str);
	std::string			parseLocation(std::string str,  configServer *servNode);
	bool				checkTokens(std::string &save, std::string str, int config_part);
	bool				checkMainValServ(struct configServer *servNode);
	void 				serverTokenSearch(std::string save, std::string tmp, configServer *servNode);
	void 				locTokenSearch(std::string save, std::string tmp, location *locNode);



	class FileNotOpenException: public std::exception
	{
		const char    *what() const throw();
	};
	class FileIsNotCorrectException: public std::exception
	{
		const char    *what() const throw();
	};
	class IncorrectConfigException: public std::exception
	{
		const char    *what() const throw();
	};
};

#endif