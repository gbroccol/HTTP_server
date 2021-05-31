#include "Handler.hpp"

Handler::Handler(void){ return; } // private

Handler::Handler(configServer const & config)
{
    this->config = config;
	this->isDir = false;
    this->_error401 = false;
	return;
}
Handler::~Handler(void)
{
	return;
}

std::string const & Handler::handle(data const & req, char **env, bool _signIn) // убрать конфиг и переменные окружения в отдельниый инит
{

    this->response.clear();                             // ответ для клиента
	this->response.append("HTTP/1.1 ");

	this->request = req;
	this->env = env;
    this->_signIn = _signIn;

	if (!isRequestCorrect())
    {
        std::cout << this->response << std::endl; //for debug
		return this->response;
    }

	makePath();

	if(config.locations[this->index_location]->autoIndex == ON)
        getFilesOrDirFromRoot(config.locations[this->index_location]->root);

    if (_error401)
        handle_401();
	else if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		handle_post();
	else if (request.method == "PUT")
		handle_put();

	std::cout << this->response << std::endl; //for debug

	this->path.clear();
	this->location_path.clear();
	return this->response;
}

int Handler::isRequestCorrect(void)
{
	std::string methods = "GET, HEAD, PUT, POST";
	int status_code = 0;

	if (request.headers->count("Host") > 1) // проверить, что заголовок и хедеры не пустые
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

    this->_error401 = false;
    if (config.locations[index_location]->authentication && _signIn == false)
    {
        this->path = "./content/home_page/authentication.html";
        this->location_path = "/home_page/authentication.html";
        this->_error401 = true;
    }
    else
    {
        this->path = ".";
        this->path.append(config.locations[index_location]->root);
        this->path.append("/");
        this->path.append(subpath());
        this->location_path.append(request.path);
    }

	dir = opendir(path.c_str());

    if (dir)
    {
		if (config.locations[index_location]->index.length() > 0) {
			this->path.append("/");
			this->path.append(config.locations[index_location]->index);                     // путь до странички
			this->location_path.append(config.locations[index_location]->index);          // путь до странички БЕЗ ДИРРЕКТОРИИ
		}
		else
			this->isDir = true;
        closedir(dir);
    }
}

void Handler::getFilesOrDirFromRoot(std::string LocPath)
{
    DIR *dir;
    struct dirent *dirStruct;
    std::string indexPath = "";

	this->arrDir.clear();

    if( LocPath[ LocPath.length() - 1] == '/')
        indexPath = '.' + config.locations[index_location]->path + LocPath;
    else
        indexPath =  '.' +  config.locations[index_location]->path + '/' + LocPath + '/';
   if((dir  = opendir(indexPath.c_str())) == nullptr)
		;													// error?
    while((dirStruct = readdir(dir)) != nullptr)
    {
        this->arrDir.push_back(dirStruct->d_name);
        if((std::string)dirStruct->d_name != ".")
            getLink(dirStruct->d_name);
    }
    closedir(dir);
}

void Handler::handle_401(void)
{
    std::string body;

    if (isDir && config.locations[this->index_location]->autoIndex == ON)
        makeAutoindexPage(&body);
    else if (checkFile() != 0)
        return;

    this->response.append("401 Unauthorized\r\n");
    this->response.append("Server: Webserv/1.1\r\n");

    this->response.append("Date: ");
    this->response.append(getPresentTime());
    this->response.append("\r\n");

    this->response.append("WWW-Authenticate: Basic realm=\"Access to the staging site\", charset=\"UTF-8\"\r\n");

    this->response.append("Content-Language: en\r\n"); //оставляем так или подтягиваем из файла?

    this->response.append("Content-Location: ");
    this->response.append(this->location_path);
    this->response.append("\r\n");

    this->response.append("Content-Type: ");
    this->response.append("text/html");
    this->response.append("\r\n");

    this->response.append("Content-Length: ");
    this->response.append(this->contentLength);
    this->response.append("\r\n");

    this->response.append("Last-Modified: ");
    this->response.append(this->lastModTime);
    this->response.append("\r\n");

    this->response.append("\r\n");

//    if (request.method == "GET") {
        if (body.length() == 0)
            loadBodyFromFile(&body);
        this->response.append(body);
//    }
}

void Handler::handle_head(void)
{
	std::string body;

	if (isDir && config.locations[this->index_location]->autoIndex == ON)
		makeAutoindexPage(&body);
	else if (checkFile() != 0)
		return;
	
	this->response.append("200 OK\r\n");
	this->response.append("Server: Webserv/1.1\r\n");
		
	this->response.append("Date: ");
	this->response.append(getPresentTime());
	this->response.append("\r\n");
		
	this->response.append("Content-Language: en\r\n"); //оставляем так или подтягиваем из файла?
		
	this->response.append("Content-Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
		
	this->response.append("Content-Type: ");
	this->response.append("text/html");
	this->response.append("\r\n");
		
	this->response.append("Content-Length: ");
	this->response.append(this->contentLength);
	this->response.append("\r\n");

	this->response.append("Last-Modified: ");
	this->response.append(this->lastModTime);
	this->response.append("\r\n");

	this->response.append("\r\n");
	
	if (request.method == "GET") {
		if (body.length() == 0)
			loadBodyFromFile(&body);
		this->response.append(body);
	}
}

void Handler::makeAutoindexPage(std::string * body)
{
	
	body->append("<html>");
	for (int i = 1; i < (int)arrDir.size(); i++) {
		    body->append(getLink(arrDir[i]));
	}
	body->append("</html>");
	this->lastModTime = getPresentTime();
	this->contentLength = lltostr(body->length());
}

std::string Handler::getLink(std::string path)
{
    std::string link;
    link = "<div style=\"display: inline-block;width: 35%;\"></span><a href=\""+ path +"\">"+ path + "</a></div>"
	    "<div style=\"display: inline-block\">"+ getPresentTime() +"</div></br>";
    return (link);
}

int Handler::checkFile(void)
{
	int fd;
	struct stat file_stat;
	
	if ( (fd = open(this->path.c_str(), O_RDONLY)) == -1)
	{
		if (errno == ENOENT || errno == EFAULT)
			error_message(404);
		else
			error_message(500);
		return 1;
	}
	
	fstat(fd, &file_stat);
	this->contentLength = lltostr(file_stat.st_size);
	this->lastModTime = getLastModificationTime(file_stat.st_mtime);
	close(fd);
	return 0;
}

void Handler::loadBodyFromFile(std::string * body)
{	
	std::ifstream ifs(this->path.c_str());
	std::stringstream ss;
	ss << ifs.rdbuf();
	
	body->append(ss.str());
	ifs.close();
}

void Handler::handle_put(void)
{
	int fd;
	std::string status = "204 No Content\r\n";
	int status_code = 204;
	
	fd = open(this->path.c_str(), O_RDWR);
	if (fd < 0)
	{
		status = "201 Created\r\n";
		status_code = 201;
	}
	else
		close(fd);
	
	std::ofstream ofs(this->path.c_str(), std::ios_base::trunc);
	if (!ofs.good())
	{
		error_message(500);
		return;
	}
	ofs << request.body;
	ofs.close();
	
	this->response.append(status);
	this->response.append("Server: Webserv/1.1\r\n");
		
	this->response.append("Date: ");
	this->response.append(getPresentTime());
	this->response.append("\r\n");
		
	this->response.append("Content-Language: en\r\n"); //нужно подтянуть из хедера запроса, если он есть
		
	this->response.append("Content-Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
		
	this->response.append("Content-Type: ");
	 this->response.append("text/plain"); //нужно подтянуть из хедера запроса, если он есть
	this->response.append("\r\n");
		
	this->response.append("Content-Length: ");
	this->response.append(lltostr(request.body.size())); //нужно подтянуть из хедера запроса, если он есть
	this->response.append("\r\n");

	this->response.append("Location: ");
	this->response.append(this->location_path);
	this->response.append("\r\n");
    this->response.append("\r\n");

	if (status_code == 201)
		this->response.append(request.body);
}

void Handler::handle_post(void)
{
    // дополняем список переменных окружения (глобальная переменная g_env)
    char ** envPost = create_env();

   	if (!(envPost))
	{
		error_message(500);
       return ;
	}

    char *args[2] = {(char*)"./cgi_tester", NULL};
    std::string body;
    if (launch_cgi(args, envPost, &body) == 1)
    {
        ft_free_array(envPost);
        return;
    }
    std::cout << RED << "cgi done" << BW << std::endl;

    std::cout << "BODY from cgi: " << GREEN << body<< BW << std::endl;


//    ft_free_array(envPost);
    //формируем ответ
    //добавляем к нему тело
    // записываем тело в файл указанный в запросе
}

char **         Handler::add_headers(int len, int headersNmb, char **result)
{
    std::vector<std::string> headers;
    headers.push_back("REQUEST_METHOD=POST");
    headers.push_back("SERVER_PROTOCOL=HTTP/1.1");
    headers.push_back("PATH_INFO=/cgi_tester");
    headers.push_back("CONTENT_LENGTH=100000000");
    headers.push_back("CONTENT_TYPE=test/file");

	int j = 0;
    for (int i = len; i < (len + headersNmb) && j < (int)headers.size(); i++, j++)
    {
        if (!(result[i] = ft_strdup(headers[j].c_str())))
        {
            ft_free_array(result);
            return (NULL);
        }
    }
	return result;
}

char **         Handler::create_env(void)
{
    char **result;
    int len = 0;
    int headersNmb = 5;

    while (this->env[len] != NULL) {
        len++;
	}

	if (!(result = (char **)calloc(len + headersNmb + 1, sizeof(char*))))
        return NULL;


    for (int i = 0; i < len && this->env[i]; i++)
    {
        if (!(result[i] = ft_strdup(this->env[i])))
        {
            ft_free_array(result);
            return (NULL);
        }
    }
    add_headers(len, headersNmb, result);
    return result;
}


int Handler::launch_cgi(char **args, char **env, std::string * body)
{
//    int fd_to_write;
//    //create file to write body
//    std::ofstream fileWrite("filename.txt");
//    fd_to_write = fileWrite.fd();


//    FILE *stream;
    int  fd_to_write;

    if ((fd_to_write = creat("file.txt", S_IWUSR)) < 0)
        perror("creat() error");

    std::cout << "New FD - " << fd_to_write << std::endl;

//    else
//    {
//        if ((stream = fdopen(fd_my, "w")) == NULL) {
//            perror("fdopen() error");
//            close(fd_my);
//        }
//        else {
//            fputs("This is a test", stream);
//            fclose(stream);
//        }}




    int status = 0;
    int fd[2];
    pid_t pid;
    // int stat;

    if (pipe(fd) != 0)
    {
        error_message(500);
        status = 1;
        return status;
    }
    pid = fork(); //проверить на ошибку
    if (pid == 0) // дочерний процесс
    {
//        dup2(fd[1], 1); // fd to write
        dup2(fd[1], fd_to_write);
		fcntl(fd[1], F_SETFL, O_NONBLOCK);
        close(fd[0]);
        if (execve(args[0], args, env) == -1)
        {
            std::cout << "execve error" << std::endl;
            error_message(500);
            status = 1;
            exit(status);
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
		// fcntl(fd[0], F_SETFL, O_NONBLOCK);

		char buffer[INBUFSIZE];
        int res = 0;
        // waitpid(pid, &stat, WUNTRACED);
        // while (!WIFEXITED(stat) && !WIFSIGNALED(stat))
		// {
            // waitpid(pid, &stat, WUNTRACED);
        	while((res = read(fd[0], buffer, INBUFSIZE)) > 0)
        	{
//				std::cout << buffer << std::endl; //for debug
           		body->append(buffer);
            	bzero(buffer, INBUFSIZE);
        	}
			std::cerr << BLUE << strerror(errno) << BW << std::endl;
        	if (res < 0)
        	{
           		kill(pid, SIGKILL);
            	status = 1;
        	}
		// }
        if (status == 1 /*|| (status = WEXITSTATUS(stat)) == 1*/)
		{
			std::cerr << BLUE << strerror(errno) << BW << std::endl;
			error_message(500);
			return (status);
		}
        //читаем и записываем в строку
        // char buffer[INBUFSIZE];
        // int res = 0;
        // while((res = read(fd[0], buffer, INBUFSIZE)) > 0)
        // {
        //     body->append(buffer);
        //     bzero(buffer, INBUFSIZE);
        // }
        // if (res < 0)
        // {
        //     error_message(500);
        //     status = 1;
        // }
        close(fd[0]);
    }

    //    std::ifstream fileRead("filename.txt");
//    Close the file
//    fileWrite.close();
//    fileRead.close();

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

std::string Handler::subpath(void) // KATE
{
    size_t i = 0;
    std::string loc_path = config.locations[index_location]->path;
//    while (i < loc_path.size() && i < request.path.size() && loc_path[i] == request.path[i])
//        i++;
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

int Handler::putVal(std::string locPath,size_t j,size_t i, int theBestLocation, std::vector<location *> locations)
{
	if((theBestLocation == -1 && j == locPath.length() - 1) || (theBestLocation >= 0 &&
		locations[i]->path.length() > locations[theBestLocation]->path.length()))
		return(i);
	return (theBestLocation);
}
int Handler::searchlocPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i, int &theBestLocation,  std::vector<location *> locations, std::string &reqPath, int flag, std::string &locPath)
{
	flag = 0;
	reqPath.length();
	locPath.length();
	if(locTmp == reqTmp )
	{
		if(reqPath[j + 1] == '/' || j == reqPath.length() - 1 || locPath == "/")
		{
			theBestLocation = putVal(locTmp, j, i, theBestLocation, locations);
			return(1);
		}
	}
	return(0);
}

int Handler::searchreqPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i, int &theBestLocation, std::vector<location *> locations, std::string &reqPath, std::string &locPath)
{
	if(locTmp == reqTmp)
	{
		if(locPath[j + 1] == '/' || j == locPath.length() - 1 || reqPath == "/")
		{
			theBestLocation = putVal(locTmp, j, i, theBestLocation, locations);
			return(1);
		}
	}
	return(0);
}

void Handler::searchPath(std::string &locTmp, std::string &reqTmp, std::string &locPath,size_t &j,size_t i, int &theBestLocation, std::string &reqPath, std::vector<location *> locations)
{
	if(locTmp == reqTmp &&  (j == locPath.length() - 1 || j == reqPath.length() - 1))
		theBestLocation = putVal(locTmp, j, i, theBestLocation, locations);
	if(locTmp == reqTmp)
	{
		locTmp.clear();
		reqTmp.clear();
		reqPath.erase(0, j + 1);
		locPath.erase(0, j + 1);
		j = -1;
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
			std::string locPath = locations[i]->path;
			std::string reqPath = path;
			std::string locTmp = "";
			std::string reqTmp = "";
			int res = 1;
			for(size_t j = 0; j < locPath.length(); j++)
			{
					locTmp.push_back(locPath[j]);
					reqTmp.push_back(reqPath[j]);
				if(locPath[j] == '*' && flag > 0)		
				{
					flag = isFiles(reqPath, locPath);
					if((theBestLocation == -1 && flag != -1) || flag > tmp)
					{
						tmp = flag;
						theBestLocation = i;
					}
					break ;
				}
				if(locPath[j] == '/')
					searchPath(locTmp, reqTmp,locPath,j,i,theBestLocation,reqPath,locations);
				else if(j == locPath.length() - 1)
					res  = searchlocPath(locTmp, reqTmp, j, i, theBestLocation, locations, reqPath, flag, locPath);
				else if(j == reqPath.length() - 1)
					res  = searchreqPath(locTmp, reqTmp, j, i, theBestLocation, locations, reqPath, locPath);
				if(res == 0)
					break ;
			}
		}
	}
	return(theBestLocation);
}

/*
 * ------------------------------ libft ------------------------------
 */

int             Handler::ft_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

void		    Handler::ft_free_array(char **to_free)
{
    char	**tmp;

    tmp = to_free;
//    while (*tmp != NULL)
//    {
//        free(*tmp);
//        tmp++;
//    }
//    free(to_free);
//    to_free = NULL;
}

char *          Handler::ft_strdup(const char *s)
{
    char	*res;
    int		i;
    int		len;

    len = ft_strlen((char *)s);
    if ((res = (char *)malloc(len + 1)) == NULL)
        return (NULL);
    i = 0;
    while (len)
    {
        res[i] = s[i];
        i++;
        len--;
    }
    res[i] = '\0';
    return (res);
}

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
