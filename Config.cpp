#include "Config.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config()
{
}

// Config::Config( const Config & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::~Config()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Config &				Config::operator=( Config const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, Config const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/
bool IsParenthesesOrDash(char c)
{
    switch(c)
    {
    case '\t':
    case '\r':
    case ' ':
	case '\n':
        return true;
    default:
        return false;
    }
}

void				Config::getFile(std::string file)
{
	std::string file_in_str;
	std::string tmp;

	if(file.length() != 0 && file == "example.conf")
	{
		std::ifstream fconfig(file);
		if (!fconfig.is_open())
			throw Config::FileNotOpenException();
		while (getline(fconfig, tmp))
		{
			file_in_str += tmp;
		}
		std::cout <<"first->str: "<<file<<std::endl;
		file_in_str.erase(std::remove_if(file_in_str.begin(), file_in_str.end(), &IsParenthesesOrDash), file_in_str.end());
		std::cout <<file_in_str<<std::endl;
		parseStr(file_in_str);
	}
	else
		throw Config::FileIsNotCorrectException();
	

}

void				Config::parseStr(std::string str)
{	
	struct server *servNode = new server;
	std::string tmp;
	std::string save;
	size_t pos = 0;
	initServNode(servNode);
	while(str.length() > 0)
	{
		if(str[pos] == '{')
		{
			if(tmp != "server")
			{
				std::cout << "tmp-->"<<tmp<<std::endl;
				throw Config::IncorrectConfigException();
			}
			str.erase(0, pos + 1);
			pos = 0;
			tmp.clear();
		}
		else if(checkTokens(save, tmp, SERVER) == true)
		{
			std::cout << save<<std::endl;
			if(save.empty())
				throw Config::IncorrectConfigException();
			str.erase(0, pos);
			pos = 0;
			tmp.clear();
			if(save == "location")
			{
				str = parseLocation(str, servNode);
			}
		}
		else if(str[pos] == ';')
		{
			serverTokenSearch(save, tmp, servNode);
			// if(save == "listen")
			// {
			// 	servNode->port = std::stoi(tmp);
			// 	if(servNode->port > 65535 || servNode->port < 0)
			// 		throw Config::IncorrectConfigException();
			// 	servNode->repeat_port = true;
			// }
			// else if(save == "server_name")
			// {
			// 	servNode->server_name.insert(0, tmp, 0, pos);
			// 	servNode->repeat_server_name = true;
			// }
			str.erase(0, pos + 1);
			tmp.clear();
			pos = 0;
	
		}
		else
			tmp.push_back(str[pos++]);

		if(str[pos] == '}')
		{
			str.erase(0, pos + 1);
			if(checkMainValServ(servNode) == true)
			{
				this->servers.push_back(servNode);
				initServNode(servNode);
				// break ;
			}
			else
				throw Config::IncorrectConfigException();
		}
	}
	std::cout <<servers.size()<<std::endl;
	for(unsigned int i = 0; i < servers.size(); i++)
	{
		std::cout <<"server data["<< i<< "]: "<<std::endl;
		std::cout <<"                       error_page ["<< i<< "]: "<<servers[i]->error_page<<" flag: "<<servers[i]->repeat_error_page<<std::endl;
		std::cout <<"                       port       ["<< i<< "]: "<<servers[i]->port<<" flag: "<<servers[i]->repeat_port<<std::endl;
		std::cout <<"                       server_name["<< i<< "]: "<<servers[i]->server_name<<" flag: "<<servers[i]->repeat_server_name<<std::endl;
		for(unsigned int j = 0; j < servers[i]->locations.size(); j++)
		{
			std::cout <<"location data["<< j<< "]: "<<std::endl;
			std::cout <<"                       index ["<< j<< "]: "<<servers[i]->locations[j]->index<< " flag: "<<servers[i]->locations[j]->repeat_index<<std::endl;
			std::cout <<"                       maxBody       ["<< j<< "]: "<<servers[i]->locations[j]->maxBody<<" flag: "<<servers[i]->locations[j]->repeat_maxBody<<std::endl;
			std::cout <<"                       path["<< j<< "]: "<<servers[i]->locations[j]->path<<" flag: "<<servers[i]->locations[j]->repeat_path<<std::endl;
			std::cout <<"                       root ["<< j<< "]: "<<servers[i]->locations[j]->root<<" flag: "<<servers[i]->locations[j]->repeat_root<<std::endl;
		}
	}
}

std::string				Config::parseLocation(std::string str,  server *servNode)
{
	struct location *locNode = new location;
	initLocNode(locNode);
	std::string tmp;
	std::string save;
	size_t pos = 0;
	while(str.length() > 0)
	{
		if(str[pos] == '{')
		{
			if(tmp.empty() && tmp[0] != '/')
				throw Config::IncorrectConfigException();
			locNode->path.assign(tmp);
			locNode->repeat_path = true;
			str.erase(0, pos + 1);
			pos = 0;
			tmp.clear();
		}
		else if(checkTokens(save, tmp, LOCATION) == true)
		{
			if(save == "index" && str[pos] == '.')
			{
				tmp.push_back(str[pos++]);
				continue ;
			}
			std::cout << save<<std::endl;
			if(save.empty())
				throw Config::IncorrectConfigException();
			str.erase(0, pos);
			pos = 0;
			tmp.clear();
		}
		else if(str[pos] == ';')
		{
			if(save == "maxBody")
			{
				if(locNode->repeat_maxBody == true)
					throw Config::IncorrectConfigException();
				locNode->maxBody = std::stoi(tmp); //check char?
				if(locNode->maxBody > LISTEN_QLEN || locNode->maxBody < 0)
					throw Config::IncorrectConfigException();
				locNode->repeat_maxBody = true;
			}
			else if(save == "root")
			{
				if(tmp.empty() && tmp[0] != '/' && locNode->repeat_root == true)
					throw Config::IncorrectConfigException();
				locNode->root.assign(tmp);
				locNode->repeat_root = true;
			}
			else if(save == "index")
			{
				if(tmp.empty() && locNode->repeat_index == true)
					throw Config::IncorrectConfigException();
				locNode->index.assign(tmp);
				locNode->repeat_index = true;
			}
			str.erase(0, pos + 1);
			tmp.clear();
			pos = 0;
	
		}
		else
			tmp.push_back(str[pos++]);
		
		if(str[pos] == '}')
		{
			str.erase(0, pos + 1);
			if(checkMainValLoc(locNode) == true)
			{
				servNode->locations.push_back(locNode);
				break ;
			}
			else
				throw Config::IncorrectConfigException();
		}
		
		
	}
	return(str);
}

void Config::serverTokenSearch(std::string save, std::string tmp, server *servNode)
{
	if(save == "listen")
	{
		servNode->port = std::stoi(tmp);
		if(servNode->port > 65535 || servNode->port < 0)
			throw Config::IncorrectConfigException();
		servNode->repeat_port = true;
	}
	else if(save == "server_name")
	{
		servNode->server_name.assign(tmp);
		servNode->repeat_server_name = true;
	}
}

void					Config::initLocNode(location *locNode)
{
	locNode->index = "";
	locNode->maxBody = 0;
	locNode->path = "";
	locNode->repeat_index = false;
	locNode->repeat_maxBody = false;
	locNode->repeat_path = false;
	locNode->repeat_root = false;
	locNode->root = "";
}

void					Config::initServNode(server *servNode)
{
	servNode->error_page = -1;
	servNode->port = -1;
	servNode->server_name = "";
	servNode->repeat_error_page = false;
	servNode->repeat_port = false;
	servNode->repeat_server_name = false;
}

bool				Config::checkMainValLoc(struct location *locNode)
{
	if(locNode->repeat_path == true && locNode->repeat_index == true &&
	   locNode->repeat_root == true)
	   return (true);
	else
		return (false);
}

bool				Config::checkMainValServ(struct server *servNode)
{
	if(/*servNode->repeat_error_page == true && */servNode->repeat_server_name == true &&
	   servNode->repeat_port == true)
	   return (true);
	else
		return (false);
}

bool				Config::checkTokens(std::string &save, std::string str, int config_part)
{
	std::string server_tokens[4] = {"listen", "server_name", "error_page", "location"};
	std::string location_tokens[] = {"index", "root", "MaxBody"};

	if(config_part == SERVER)
	{
		for(unsigned int i = 0; i < 4; i++)
		{
			if(server_tokens[i] == str)
			{
				save = server_tokens[i];
				return (true);
			}
		}
		return (false);
	}
	else
	{
		for(unsigned int i = 0; i < 3; i++)
		{
			if(location_tokens[i] == str)
			{
				save = location_tokens[i];
				return (true);
			}
		}
		return (false);
	}
}


/*
** --------------------------------- GETTERS AND SETTERS ---------------------------------
*/
	long Config::getPort(void) const
	{
		return this->_port;
	}

	char * Config::getLogFile(void) const
	{
		return this->_root;
	}
	
	void Config::setPort(long const & p)
	{
		this->_port = p;
	}

	void Config::setLogFile(char * name)
	{
		this->_root = name;
	}

/*
** --------------------------------- EXCEPTIONS ----------------------------------
*/
const char   *Config::FileNotOpenException::what() const throw()
{
	return ("File not open");
}

const char   *Config::FileIsNotCorrectException::what() const throw()
{
	return ("File is not correct");
}

const char   *Config::IncorrectConfigException::what() const throw()
{
	return ("Config is not correct");
}
/* ************************************************************************** */