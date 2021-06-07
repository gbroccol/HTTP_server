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
;
	if(file.length() != 0)
	{
		std::ifstream fconfig(file);
		if (!fconfig.is_open())
			throw Config::FileNotOpenException();
		while (getline(fconfig, tmp))
			file_in_str += tmp;
		file_in_str.erase(std::remove_if(file_in_str.begin(), file_in_str.end(), &IsParenthesesOrDash), file_in_str.end());
		while(file_in_str.length() > 0)
			file_in_str =  parseStr(file_in_str);
	}
	else
		throw Config::FileIsNotCorrectException();
}

std::string				Config::parseStr(std::string str)
{
    struct configServer *servNode = new configServer;
    std::string tmp;
    std::string save;

    size_t pos = 0;
    initServNode(servNode);
    while(str.length() > 0)
    {
        if(str[pos] == '{')
        {
            if(tmp != "server")
                throw Config::IncorrectConfigException();
            str.erase(0, pos + 1);
            pos = 0;
            tmp.clear();
        }
        else if(checkTokens(save, tmp, SERVER) == true)
        {
            if(save.empty())
                throw Config::IncorrectConfigException();
            str.erase(0, pos);
            pos = 0;
            tmp.clear();
            if(save == "location")
                str = parseLocation(str, servNode);
        }
        else if(str[pos] == ';')
        {
            serverTokenSearch(save, tmp, servNode);
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
                break ;
            }
            else
                throw Config::IncorrectConfigException();
        }
    }
    // std::cout <<servers.size()<<std::endl;
    // for(unsigned int i = 0; i < servers.size(); i++)
    // {
    // 	std::cout <<"configServer data["<< i<< "]: "<<std::endl;
    // 	std::cout <<"                       error_page ["<< i<< "]: "<<servers[i]->error_page<<" flag: "<<servers[i]->repeat_error_page<<std::endl;
    // 	std::cout <<"                       port       ["<< i<< "]: "<<servers[i]->port<<" flag: "<<servers[i]->repeat_port<<std::endl;
    // 	std::cout <<"                       server_name["<< i<< "]: "<<servers[i]->server_name<<" flag: "<<servers[i]->repeat_server_name<<std::endl;
    // 	std::cout <<servers[i]->locations.size()<<std::endl;
    // 	for(unsigned int j = 0; j < servers[i]->locations.size(); j++)
    // 	{
    // 		std::cout <<std::setw(20) <<"location data["<< j<< "]: "<<std::endl;
    // 		std::cout <<std::setw(40) <<"index ["<< j<< "]: "<<servers[i]->locations[j]->index<< " flag: "<<servers[i]->locations[j]->repeat_index<<std::endl;
    // 		std::cout <<std::setw(40) <<"maxBody["<< j<< "]: "<<servers[i]->locations[j]->maxBody<<" flag: "<<servers[i]->locations[j]->repeat_maxBody<<std::endl;
    // 		std::cout <<std::setw(40) <<"path["<< j<< "]: "<<servers[i]->locations[j]->path<<" flag: "<<servers[i]->locations[j]->repeat_path<<std::endl;
    // 		std::cout <<std::setw(40) <<"root ["<< j<< "]: "<<servers[i]->locations[j]->root<<" flag: "<<servers[i]->locations[j]->repeat_root<<std::endl;
    // 		for(unsigned int h = 0; h < servers[i]->locations[j]->method.size(); h++)
    // 			std::cout <<std::setw(40) <<"method ["<< h<< "]: "<<servers[i]->locations[j]->method[h]<<" flag: "<<servers[i]->locations[j]->repeat_method<<std::endl;
    // 	}
    // }
    return (str);
}

bool checkLockPath(std::string path)
{
    std::string tmp = "";
    int index = path.find('*');
    if(index > 0)
    {
        tmp = path.substr(index, path.length() - 1);
        if(!tmp.empty() && tmp.length() > 1 &&  tmp[0] == '*' &&  tmp[1] != '.')
            return(false);
    }
    return (true);
}

bool checkErrorPage(std::string path)
{

    // std::ifstream fconfig(path);
    std::ifstream fconfig("./config/example.conf");
    std::string tmp = "";
    int index = path.find('.');
    if(index > 0)
    {
        tmp = path.substr(index, path.length() - 1);
        if(tmp.length() == 1 || !fconfig.is_open())
            return(false);
    }
    else if(index < 0)
        return(false);
    return (true);
}

bool checkIndex(std::string root, std::string indexPath)
{

    std::string tmp = "";
    if(root[root.length() - 1] == '/')
        indexPath = '.' + root + indexPath;
    else
        indexPath =  '.' + root + '/' + indexPath;

    std::ifstream fconfig(indexPath);

    int index = 1;
    if(index > 0)
    {
        tmp = indexPath.substr(index,indexPath.length() - 1);
        if(tmp.length() == 1 || !fconfig.is_open())
            return(false);
    }
    else if(index < 0)
        return(false);
    return (true);
}

std::string				Config::parseLocation(std::string str,  configServer *servNode)
{
    struct location *locNode = new location;
    std::string tmp;
    std::string save;

    size_t pos = 0;
    initLocNode(locNode);
    while(str.length() > 0)
    {
        if(str[pos] == '{')
        {
            if(tmp.empty() || tmp[0] != '/' || checkLockPath(tmp) == false)
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
            if(save.empty() || (save == "method" && locNode->repeat_method == true))
                throw Config::IncorrectConfigException();
            str.erase(0, pos);
            pos = 0;
            tmp.clear();
        }
        else if(str[pos] == ';')
        {
            locTokenSearch(save, tmp, locNode);
            if(save == "method")
                locNode->repeat_method = true;
            str.erase(0, pos + 1);
            tmp.clear();
            pos = 0;
        }
        else if(checkTokens(save, tmp, METHOD) == true)
        {
            locTokenSearch(save, tmp, locNode);
            locNode->repeat_method = true;
            str.erase(0, pos);
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
                if(locNode->repeat_index == false) {
                    locNode->index = ""; }
                servNode->locations.push_back(locNode);
                break ;
            }
            else
                throw Config::IncorrectConfigException();
        }
    }
    return(str);
}

void Config::locTokenSearch(std::string save, std::string tmp, location *locNode)
{
    if(save == "maxBody")
    {
        if(locNode->repeat_maxBody == true)
            throw Config::IncorrectConfigException();
        locNode->maxBody = std::stoi(tmp); //check char?
        if(locNode->maxBody > 1000000 || locNode->maxBody < 0)
            throw Config::IncorrectConfigException();
        locNode->repeat_maxBody = true;
    }
    else if(save == "root")
    {
        if(tmp.empty() || tmp[0] != '/' || locNode->repeat_root == true)
            throw Config::IncorrectConfigException();
        locNode->root.assign(tmp);
        locNode->repeat_root = true;
    }
    else if(save == "index")
    {
        if(tmp.empty() || locNode->repeat_index == true || checkIndex(locNode->root,tmp) == false)
            throw Config::IncorrectConfigException();
        locNode->index.assign(tmp);
        locNode->repeat_index = true;
    }
    else if(save == "method")
    {
        if(tmp.empty() || checkTokens(save, tmp, METHOD) == false)
            throw Config::IncorrectConfigException();
        locNode->method.push_back(tmp);
    }
    else if(save == "autoindex")
    {
        if(tmp.empty() || locNode->repeat_autoIndex == true)
            throw Config::IncorrectConfigException();
        if(tmp == "on")
            locNode->autoIndex = ON;
        else if(tmp == "off")
            locNode->autoIndex = OFF;
        else
            throw Config::IncorrectConfigException();
        locNode->repeat_autoIndex = true;
    }
    else if(save == "authentication")
    {
        if(tmp.empty() || locNode->repeat_authentication == true)
            throw Config::IncorrectConfigException();
        if(tmp == "on")
            locNode->authentication = ON;
        else if(tmp == "off")
            locNode->authentication = OFF;
        else
            throw Config::IncorrectConfigException();
        locNode->repeat_authentication = true;
    }
}

void getPortsAndIP(configServer *servNode, std::string portsStr)
{
    int tmpPort;
    std::string tmp;

    for(size_t i = 0; i < portsStr.length(); i++)
    {
        if(portsStr[i] > '9' ||  portsStr[i] < '0')
        {
            if (portsStr[i] != ',' && portsStr[i] != ':')
                throw Config::IncorrectConfigException();
        }
        if(portsStr[i] == ':')
        {
            if(!tmp.empty())
            {
                tmpPort = std::stoi(tmp);
                if(tmpPort > 65535 || tmpPort < 0)
                    throw Config::IncorrectConfigException();
                servNode->port.push_back(tmpPort);
                tmp.clear();
            }
            while(++i < portsStr.length())
            {
                tmp.push_back(portsStr[i]);
            }
            if(tmp.empty())
                throw Config::IncorrectConfigException();
            servNode->ip.assign(tmp);
            continue;
        }
        if( portsStr[i] == ',')
        {
            tmpPort = std::stoi(tmp);
            if(tmpPort > 65535 || tmpPort < 0)
                throw Config::IncorrectConfigException();
            servNode->port.push_back(tmpPort);
            tmp.clear();
            continue;
        }
        tmp.push_back(portsStr[i]);
    }
}

void Config::serverTokenSearch(std::string save, std::string tmp, configServer *servNode)
{
    if(save == "listen")
    {
        getPortsAndIP(servNode, tmp);
        servNode->repeat_port = true;
    }
    else if(save == "server_name")
    {
        servNode->server_name.assign(tmp);
        servNode->repeat_server_name = true;
    }
    else if(save == "error_page")
    {
        if(tmp.empty() || tmp[0] != '/' || servNode->repeat_error_page == true || checkErrorPage(tmp) == false)
            throw Config::IncorrectConfigException();
        servNode->error_page.assign(tmp);
        servNode->repeat_error_page = true;
    }
}

void					Config::initLocNode(location *locNode)
{
    locNode->index = "";
    locNode->maxBody = 0;
    locNode->path = "";
    locNode->method.clear();
    locNode->repeat_method = false;
    locNode->repeat_index = false;
    locNode->repeat_maxBody = false;
    locNode->repeat_path = false;
    locNode->repeat_root = false;
    locNode->root = "";
    locNode->autoIndex = -1;
    locNode->repeat_autoIndex = false;
    locNode->authentication = false;
    locNode->repeat_authentication = false;
}

void					Config::initServNode(configServer *servNode)
{
    servNode->error_page = "";
    servNode->port.clear();
    servNode->server_name = "";
    servNode->repeat_error_page = false;
    servNode->repeat_port = false;
    servNode->repeat_server_name = false;
    servNode->ip.clear();
}

bool				Config::checkMainValLoc(struct location *locNode)
{
    if(locNode->repeat_path == true && locNode->repeat_root == true && locNode->repeat_method == true)
        return (true);
    else
        return (false);
}

bool				Config::checkMainValServ(struct configServer *servNode)
{
    if(servNode->repeat_error_page == true && !(servNode->ip.empty()) &&
       servNode->repeat_port == true)
        return (true);
    else
        return (false);
}

bool				Config::checkTokens(std::string &save, std::string str, int config_part)
{
	std::string server_tokens[] = {"listen", "server_name", "error_page", "location"};
	std::string location_tokens[] = {"index", "root", "maxBody", "method", "autoindex", "authentication"};
	std::string method_tokens[] = {"GET", "POST", "PUT", "HEAD"};

    if (config_part == SERVER)
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
    else if(config_part == METHOD)
    {
        for(unsigned int i = 0; i < 4; i++)
            if(method_tokens[i] == str)
                return (true);
        return (false);
    }
    else
    {
        for(unsigned int i = 0; i < 6; i++)
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
	configServer        *Config::getconfigServer(int index)
	{
		return(servers[index]);
	}

	size_t		Config::getSize(void)
	{
		return this->servers.size();
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