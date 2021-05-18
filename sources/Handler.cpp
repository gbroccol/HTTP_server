#include "Handler.hpp"

Handler::Handler(void)
{
	return;
}
Handler::~Handler(void)
{
	return;
}

std::string const & Handler::handle(configServer const & config, data const & req, char **env) // убрать конфиг и переменные окружения в отдельниый инит
{

    this->response.clear();                             // ответ для клиента
	this->response.append("HTTP/1.1 ");
	this->config = config;
	this->request = req;
	this->env = env;

	if (!isRequestCorrect())
    {
        std::cout << this->response << std::endl; //for debug
		return this->response;
    }

	makePath();
	if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		handle_post();
	else if (request.method == "PUT")
		handle_put();

	std::cout << this->response << std::endl; //for debug
	return this->response;
}


int Handler::isRequestCorrect(void)
{
	std::string methods = "GET, HEAD, PUT, POST";
	int status_code = 0;

	if (request.headers.count("Host") > 1) // проверить, что заголовок и хедеры не пустые
		status_code = 400;
	else if (request.version != "HTTP/1.1")
		status_code = 505;
	else if ((index_location = isLocation(config.locations, request.path)) < 0)
		status_code = 404;
	else if (methods.find(request.method) == std::string::npos)
		status_code = 501;
	else if (!doesLocationAnswersMethod())
        status_code = 405;

	if (status_code != 0)
	{
		error_message(status_code);
		return 0;
	}
	return 1;
}

int Handler::doesLocationAnswersMethod(void)
{
    std::vector<std::string> methods = config.locations[index_location]->method;
    for (size_t i = 0; i < methods.size(); i++) {
        if (methods[i] == request.method)
            return 1;
    }
    return 0;
}

void Handler::makePath(void)
{
	DIR	*dir;
	
	this->path = ".";
	this->path.append(config.locations[index_location]->root);
	this->path.append(subpath());
	this->location_path.append(request.path);

    std::cout << "ROOT " << config.locations[index_location]->root << std::endl;
	std::cout << "PATH " << this->path << std::endl;

	dir = opendir(path.c_str());

    if (dir)
    {
        this->path.append("/");
        this->path.append(config.locations[index_location]->index);                     // путь до странички
        this->location_path.append(config.locations[index_location]->index);           // путь до странички БЕЗ ДИРРЕКТОРИИ
        closedir(dir);
    }
}

void Handler::handle_head(void)
{
	int fd;
	struct stat file_stat;
	
	if ( (fd = open(this->path.c_str(), O_RDONLY)) == -1)
	{
		if (errno == ENOENT || errno == EFAULT)
			error_message(404);
		else
			error_message(500);
		return;
	}
	
	fstat(fd, &file_stat);
	this->response.append("200 OK\r\n");
	this->response.append("Server: Webserv/1.1\r\n");
		
	this->response.append("Date: ");
	this->response.append(getPresentTime());
	this->response.append("\r\n");
		
	this->response.append("Content-Language: en\r\n"); //оставляем так или подтягиваем из файла?
		
	this->response.append("Content-Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
		
	// как определяем тип файла ??
	this->response.append("Content-Type: ");
	this->response.append("text/html");
	this->response.append("\r\n");
		
	this->response.append("Content-Length: ");
	this->response.append(lltostr(file_stat.st_size));
	this->response.append("\r\n");

	this->response.append("Last-Modified: ");
	this->response.append(getLastModificationTime(file_stat.st_mtime));
	this->response.append("\r\n");

	this->response.append("\r\n");
	
	close(fd);
	
	if (request.method == "GET")
		append_body();
}

void Handler::append_body(void)
{
	//возможно нужно будет заменить на функции из других библиотек
	
	std::ifstream ifs(this->path.c_str());
	std::stringstream ss;
	ss << ifs.rdbuf();
	
	this->response.append(ss.str());
	ifs.close();
}

void Handler::handle_put(void)
{
	std::string status_code;
	int fd;
	status_code = "204 No Content\r\n";
	
	fd = open(this->path.c_str(), O_RDWR);
	if (fd < 0)
		status_code = "201 Created\r\n";
	close(fd);
	
	std::ofstream ofs(this->path.c_str(), std::ios_base::trunc);
	if (!ofs.good())
	{
		error_message(500);
		return;
	}
	ofs << request.body;
	ofs.close();
	
	this->response.append(status_code);
	this->response.append("Server: Webserv/1.1\r\n");
		
	this->response.append("Date: ");
	this->response.append(getPresentTime());
	this->response.append("\r\n");
		
	this->response.append("Content-Language: en\r\n"); //нужно подтянуть из хедера запроса, если он есть
		
	this->response.append("Content-Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
		
	this->response.append("Content-Type: ");
	// this->response.append(this->path); //нужно подтянуть из хедера запроса, если он есть
	this->response.append("\r\n");
		
	this->response.append("Content-Length: ");
	// this->response.append(lltostr(file_stat.st_size)); //нужно подтянуть из хедера запроса, если он есть
	this->response.append("\r\n");

	this->response.append("Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
}

void Handler::handle_post(void)
{
    // дополняем список переменных окружения (глобальная переменная g_env)
    char ** env = create_env();
    char *args[2] = {(char*)"./cgi_tester", NULL};
    std::string body;
    if (launch_cgi(args, env, &body) == 1)
    {
//        free whole env
        return;
    }

    //формируем ответ
    //добавляем к нему тело
}

char ** Handler::create_env(void)
{
    char **result;
    int len = 0;
    int headersNmb = 0;

    while (this->env[len] != NULL)
        len++;

    result = (char **)malloc((sizeof(char *) * len) + headersNmb + 1);

    for (int i = 0; i < len; i++)
    {
        result[i] = this->;
    }

//    for (int i = len; i < (len + headersNmb); i++)
//    {
//
//    }

//    for param in os.environ.keys():
//    print("<b>%20s</b>: %s<br>" % (param, os.environ[param]))



    return result;
}

int Handler::launch_cgi(char **args, char **env, std::string * body)
{
    int status = 0;
    int fd[2];
    pid_t pid;
    int stat;


    if (pipe(fd) != 0)
    {
        error_message(500);
        status = 1;
        return status;
    }
    pid = fork();
    if (pid == 0) // дочерний процесс
    {
        dup2(fd[1], 1);
        close(fd[0]);
        if (execve(args[0], args, env) == -1)
        {
            error_message(500);
            status = 1;
        }
    }
    else if (pid < 0)
    {
        error_message(500);
        close(fd[0]);
        close(fd[1]);
        status = 1;
    }
    else
    {
        dup2(fd[0], 0);
        close(fd[1]);
        waitpid(pid, &stat, WUNTRACED);
        while (!WIFEXITED(stat) && !WIFSIGNALED(stat))
            waitpid(pid, &stat, WUNTRACED);
        //читаем и записываем в строку
        char buffer[INBUFSIZE];
        int res = 0;
        while((res = read(fd[0], buffer, INBUFSIZE)) > 0)
        {
            body->append(buffer);
            bzero(buffer, INBUFSIZE);
        }
        if (res < 0)
        {
            error_message(500);
            status = 1;
        }

        close(fd[0]);
    }
    return status;
}

std::string Handler::getPresentTime(void)
{
    char buffer[80];
    time_t seconds = time(NULL);
    std::string format = "%a, %d %b %Y %I:%M:%S";
    
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, format.c_str(), timeinfo);
    std::string resultTime = std::string(buffer);
    resultTime = resultTime + " GMT";
    return (resultTime);
}

std::string Handler::getLastModificationTime(time_t const & time)
{
	tm *ltm = std::localtime(&time); 
	char mbstr[100];
	std::strftime(mbstr, 100, "%a, %d %b %Y %I:%M:%S", ltm);
	std::string lastModifTime(mbstr);
	lastModifTime += " GMT";
	return (lastModifTime);
}

void Handler::error_message(int const & status_code)
{
	switch(status_code)
	{
		case 400:
			this->response.append("400 Bad Request\r\n");
			break;
		case 404:
			this->response.append("404 Not found\r\n");
			break;
		case 405:
			this->response.append("405 Method Not Allowed\r\n");
			allow_header();
			break;
		case 500:
			this->response.append("500 Internal Server Error\r\n");
			break;
		case 501:
			this->response.append("501 Not Implemented\r\n");
			allow_header();
			break;
		case 505:
			this->response.append("505 HTTP Version Not Supported\r\n");
			break;
	}
    this->response.append("Content-Length: 0\r\n\r\n");
	return;
}

void Handler::allow_header(void)
{
	location * loc = config.locations[index_location];
	size_t size = loc->method.size();
	
	this->response.append("Allow: ");
	for (size_t i = 0; i < size; i++)
	{
		this->response.append(loc->method[i].c_str());
		if (i != size - 1)
			this->response.append(", ");
	}
	this->response.append("\r\n");
}

// Additional functions

std::string Handler::lltostr(long long number)
{
	std::string res;
	
	if (number >= 10)
	{
		while (number > 0)
		{
			res.insert(res.begin(), (number % 10 + '0'));
			number = number / 10;
		}
	}
	else
		res.insert(res.begin(), (number + '0'));
	return res;
}

std::string Handler::subpath(void)
{
    size_t i = 0;
    std::string loc_path = config.locations[index_location]->path;
    while (i < loc_path.size() && i < request.path.size() && loc_path[i] == request.path[i])
        i++;
    return (request.path.substr(i));
}

int Handler::isFiles(std::string path, std::string locPath)
{
	std::string expansion = "";
	std::string tmp = "";
	int flag = 0;
	for(size_t i = 0; i < path.length(); i++)
	{
		if(path[i] == '.' || flag == 1)
		{
			expansion.push_back(path[i]);
			flag = 1;
		}
	}
	if(locPath == "*" && expansion.length() > 1)
		return(1);
	else if(locPath[1] == '.' && expansion.length() > 1)
	{
		tmp = locPath.substr(locPath.find('.'), locPath.length() - 1);
		if(tmp == expansion)
			return(2);
	}
	return (-1);
}

int Handler::putVal(std::string &locTmp, std::string &reqTmp, std::vector<location *> locations,size_t i, size_t j, int theBestLocation)
{
	locTmp.clear();
	reqTmp.clear();
	if((theBestLocation == -1 && j == locations[i]->path.length() - 1) || (theBestLocation >= 0 && locations[i]->path.length() > locations[theBestLocation]->path.length()))
		return(i);
	return (theBestLocation);
}

void Handler::searchPath(std::string &locTmp, std::string &reqTmp, std::vector<location *> locations,size_t i, size_t &j, int &theBestLocation, std::string path)
{
	if(locTmp == reqTmp &&  j == path.length() - 1 )
		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
	else if(locTmp == reqTmp && reqTmp[j + 1] == '/')
		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
	else if(j == 0 && j == locations[i]->path.length() - 1)
		theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
	else if(locTmp != reqTmp && locations[i]->path[j] == '/')
		if(j + 1 == locations[i]->path.length() && j == path.length())
			theBestLocation = putVal(locTmp, reqTmp, locations, i, j, theBestLocation);
	if(j == 0 && j != locations[i]->path.length() - 1)
	{
		locTmp.clear();
		reqTmp.clear();
		j = 0;
	}
}

int Handler::isLocation(std::vector<location *> locations, std::string path)
{
	int theBestLocation = -1;
	int tmp = -1;
	int flag = 1;
	for(size_t i = 0; i < locations.size(); i++)
	{
		if(locations[i]->path == path)
		{
			theBestLocation = i;
			return (theBestLocation);
		}
		else
		{
			std::string locTmp = "";
			std::string reqTmp = "";
			for(size_t j = 0; j < locations[i]->path.length(); j++)
			{
				locTmp.push_back(locations[i]->path[j]);
				reqTmp.push_back(path[j]);

				if(locations[i]->path[j] == '*' && flag > 0)		
				{
					flag = isFiles(path.substr(j, path.length() - 1), locations[i]->path.substr(j, path.length() - 1));
					if((theBestLocation == -1 && flag != -1) || flag > tmp)
					{
						tmp = flag;
						theBestLocation = i;
					}
					break ;
				}
				if(locations[i]->path[j] == '/' || j == locations[i]->path.length() - 1 )
					searchPath(locTmp, reqTmp, locations, i, j, theBestLocation, path);
			}
		}
	}
	return(theBestLocation);
}