/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 18:31:32 by pvivian           #+#    #+#             */
/*   Updated: 2021/06/07 15:01:40 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Webserv.hpp"

#define METHOD 2
#define SERVER 1
#define LOCATION 0


	struct 						location
	{
		std::string 			path;
		bool 					repeat_path;
		std::string 			index;
		bool 					repeat_index;
        std::string 			cgi;
        bool 					repeat_cgi;
		std::string 			root;
		bool 					repeat_root;
		int 					maxBody;
		bool 					repeat_maxBody;
		std::vector<std::string> method;
		bool 					repeat_method;
        bool                    autoIndex;
        bool 					repeat_autoIndex;
        bool                    authentication;
        bool 					repeat_authentication;
        std::string 			redirect;
        bool 					repeat_redirect;

	};

struct 	configServer
{
        std::vector<int> 		port;
		bool 					repeat_port;
        std::string 			ip;
		std::string 			server_name;
		bool 					repeat_server_name;
		std::string 			error_page;
		bool 					repeat_error_page;
		std::vector<location*> 	locations;
	};


class Config
{

private:
	
	std::vector<configServer*> 	servers;
	
public:
	Config();
	~Config();

    void						getFile(std::string file);
    std::string					parseStr(std::string str);
    std::string					parseLocation(std::string str,  configServer *servNode);
    void 						locTokenSearch(std::string save, std::string tmp, location *locNode);
    void 						serverTokenSearch(std::string save, std::string tmp, configServer *servNode);
    bool						checkTokens(std::string &save, std::string str, int config_part);

    void						initLocNode(location *locNode);
    void						initServNode(configServer *servNode);

	bool						checkMainValLoc(location *locNode);
	bool						checkMainValServ(struct configServer *servNode);
	unsigned int                ft_strlen(std::string str[]);
    bool                        checkIndex(std::string root, std::string indexPath);
    bool                        checkErrorPage(std::string path);
    bool                        checkLockPath(std::string path);
    void                        getPortsAndIP(configServer *servNode, std::string portsStr);
    bool                        check_repeat_ports(configServer *servNode);
    bool                        checkCgi(std::string cgiPath);

    configServer        		*getconfigServer(int index) const;
    size_t						getSize(void) const;


	class 						FileNotOpenException: public std::exception
	{
		const char    *what() const throw();
	};
	class 						FileIsNotCorrectException: public std::exception
	{
		const char    *what() const throw();
	};
	class 						IncorrectConfigException: public std::exception
	{
		const char    *what() const throw();
	};
};

#endif