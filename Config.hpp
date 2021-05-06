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

	struct 						server
	{
		int 					port;
		bool 					repeat_port;
		std::string 			server_name;
		bool 					repeat_server_name;
		int 					error_page;
		bool 					repeat_error_page;
		std::vector<location*> 	locations;
	};

private:
	long 						_port;
	char 						*_root;
	std::string					_server_name;
	
	std::vector<server*> 		servers;
	
public:
	Config();
	~Config();

	long 				getPort(void) const;
	char 				*getLogFile(void) const;
	void 				setPort(long const & p);
	void 				setLogFile(char * name);
	bool				checkMainValLoc(location *locNode);
	void				initLocNode(location *locNode);
	void				initServNode(server *servNode);
	void				getFile(std::string file);
	void				parseStr(std::string str);
	std::string			parseLocation(std::string str,  server *servNode);
	bool				checkTokens(std::string &save, std::string str, int config_part);
	bool				checkMainValServ(struct server *servNode);
	void 				serverTokenSearch(std::string save, std::string tmp, server *servNode);




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