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
	this->response.clear();
	return;
}

std::string const & Handler::handle(data const & req, user & userData)
{

  this->response.clear();                             // ответ для клиента
	this->response.append("HTTP/1.1 ");

	this->request = req;
  this->_userData = userData;

	if (!isRequestCorrect())
    {
        std::cout << this->response << std::endl; //for debug
		return this->response;
    }

	makePath();

	if(config.locations[this->index_location]->autoIndex == ON)
        getFilesOrDirFromRoot(config.locations[this->index_location]->root);

	if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		handle_post();
	else if (request.method == "PUT")
		handle_put();
	else if (request.method == "DELETE")
        handle_delete();

  
  
	if (request.method != "POST")
      std::cout << PURPLE << "RESPONSE" << BW << std::endl << this->response << std::endl; //for debug

	this->path.clear();
	this->location_path.clear();
	this->arrDir.clear();
	this->contentLength.clear();
	this->lastModTime.clear();
	return this->response;
}

int Handler::isRequestCorrect(void)  // errors
{
	std::string methods = "GET, HEAD, PUT, POST, DELETE";
	int status_code = 0;

	if (request.headers->count("Host") > 1) // проверить, что заголовок и хедеры не пустые
		status_code = 400;
//	else if (request.version != "HTTP/1.1")
//		status_code = 505;
    else if ((index_location = isLocation(config.locations, request.path)) < 0)
	 	status_code = 404;
	else if (methods.find(request.method) == std::string::npos)
		status_code = 501;
	else if (!doesLocationAnswersMethod())
        status_code = 405;
	else if (config.locations[index_location]->maxBody > 0 && (int)request.body.length() > config.locations[index_location]->maxBody)
		status_code = 413;
	else if (config.locations[index_location]->authentication && _userData.signIn == false)
        status_code = 511;

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
    this->path.append("/");
    this->path.append(subpath());
//        this->location_path.append("./content"); // change otnosit pyt // Kate // test download
    this->location_path.append(request.path);

    size_t pos = this->path.find("//", 0);
    if (pos != std::string::npos)
        this->path.erase(pos, 1);

	dir = opendir(path.c_str());

    if (dir)
    {
		if (config.locations[index_location]->index.length() > 0) {
			this->path.append("/");
			this->location_path.append("/");
			this->path.append(config.locations[index_location]->index);                     // путь до странички
			this->location_path.append(config.locations[index_location]->index);          // путь до странички БЕЗ ДИРРЕКТОРИИ
		}
		else
			this->isDir = true; // ???
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
        indexPath = '.' + LocPath;
    else
        indexPath =  '.' +  LocPath + '/';
//    if( LocPath[ LocPath.length() - 1] == '/')
//        indexPath = '.' + config.locations[index_location]->path + LocPath;
//    else
//        indexPath =  '.' +  config.locations[index_location]->path + '/' + LocPath + '/';
   if((dir  = opendir(indexPath.c_str())) == nullptr)
		std::cout << RED<< "ERROR OPEN DIR"<< BW<<std::endl;

    while((dirStruct = readdir(dir)) != nullptr)
    {
        this->arrDir.push_back(dirStruct->d_name);
        if((std::string)dirStruct->d_name != ".")
            getLink(dirStruct->d_name);
    }
    closedir(dir);
}

/*
 * ----------------------------------------- HEAD --------------------------------------
 */

void Handler::handle_head(void)
{
	std::string body;
	int status_code = 200;

	if (isDir && config.locations[this->index_location]->autoIndex == ON)
		makeAutoindexPage(&body);
	else if (checkFile() != 0)
		return;

    if (config.locations[index_location]->repeat_redirect == true)
        status_code = 301;
    else if (config.locations[index_location]->repeat_redirect == true)
        status_code = 308;

    addHeaderStatus(status_code);
    addHeaderServer();
    addHeaderDate();
    addHeaderContentLanguage();
    addHeaderContentLocation();
    if (status_code == 301 || status_code == 308)
        addHeaderLocation();
    addHeaderContentType();
    addHeaderContentLength(this->contentLength);
    addHeaderLastModified();

	this->response.append("\r\n");
	
	if (request.method == "GET") {
		if (body.length() == 0)
			loadBodyFromFile(&body);
		this->response.append(body);
	}
}

/*
 * ----------------------------------------- POST --------------------------------------
 */

void Handler::handle_post(void)
{
    /*
     *  Upload files
     */
//    std::multimap <std::string, std::string>::iterator itCL = this->request.headers->find("Content-Type");
//    size_t pos = itCL->second.find("multipart/form-data; boundary=", 0);
//    if (pos != std::string::npos && pos == 0)
//    {
//        pos = itCL->second.find("=", 0);
//        std::string boundary;
//        boundary.append(itCL->second, pos + 1, std::string::npos);
//
//        addHeaderStatus(updateFile(boundary));
//        addHeaderServer();
//        addHeaderDate();
//        addHeaderContentLanguage();
//        addHeaderContentLocation();
//        addHeaderContentLength("0");
//        addHeaderLocation();
//        this->response.append("\r\n");
//        std::cout << PURPLE << "RESPONSE" << BW << std::endl << this->response << std::endl; //for debug
//        return;
//    }

    if (this->request.formData->size() != 0) // регистрация
    {
        int status = 200;
        std::string body;

        loadBodyFromFile(&body);
        addHeaderStatus(status); // другой статус если такого пользователя нет
        addHeaderServer();
        addHeaderDate();
        addHeaderContentLanguage();
        addHeaderContentLocation();
        addHeaderContentLength(std::to_string(body.length()));
        addHeaderLocation();
        this->response.append("\r\n");
        this->response.append(body);
        return;
    }
    char ** envPost = create_env(); // all headers
   	if (!(envPost))
        return error_message(500);

   	// hardcode
   	std::string pathBodyFile;
    if (config.locations[index_location]->cgi_name == "python_upload.py")
        pathBodyFile= "./content/website1/users/bodyCGI.txt";
    else
        pathBodyFile = this->path.c_str();

    std::ofstream ofs(pathBodyFile, std::ios_base::trunc);
	if (!ofs.good())
        return error_message(500);
	ofs << request.body;
	ofs.close();

    const char *pathToCGI = config.locations[index_location]->cgi.c_str();

	char *args[3] = { (char *)pathToCGI, (char *)path.c_str(), NULL};
    std::string body;
    if (launch_cgi(args, envPost, &body) == 1)
    {
        ft_free_array(envPost);
        return;
    }

    addHeaderStatus(200);
    addHeaderServer();
    addHeaderDate();
    addHeaderContentLanguage();
    addHeaderContentLocation();
    this->response.append("Transfer-Encoding: chunked\r\n");
    this->response.append("Location: ");
    this->response.append(this->location_path);
    this->response.append("\r\n");
    this->response.append("\r\n");

//	std::cout << PURPLE << "RESPONSE" << BW << std::endl << this->response << std::endl; //for debug

	this->response.append(body);
   	ft_free_array(envPost);
}

int Handler::updateFile(std::string & boundary)
{
    int status = 200;
    size_t pos;

    std::string type;
    std::string name;
    std::string fileName;
    std::string mime;
    std::string content;
    /*
     * пропустить boundary
     */
    request.body.erase(0, boundary.length() + 2 + 2);
    /*
     * прочитать хедоры
     */
    int separate = 0;
    std::string header;
    while ((pos = request.body.find("\r\n", 0)) != std::string::npos)
    {
        if (pos == 0)
        {
            request.body.erase(0, 2);
            break;
        }
        separate = request.body.find(":", 0);
        header = request.body.substr(0, separate);
        if (header == "Content-Disposition")
        {
            request.body.erase(0, separate + 2);
            /* form-data */
            separate = request.body.find(";", 0);
            type = request.body.substr(0, separate);
            request.body.erase(0, separate + 2);
            /* name=\"photo\" */
            if (request.body.find("name", 0) == 0)
            {
                separate = request.body.find(";", 0);
                name = request.body.substr(6, separate - 1 - 6);
                request.body.erase(0, separate + 2);
            }
            if ((separate = request.body.find("filename", 0)) == 0)
            {
                separate = request.body.find("\r\n", 0);
                fileName = request.body.substr(10, separate - 1 - 10);
                request.body.erase(0, separate + 2);
            }
        }
        else if (header == "Content-Type")
        {
            separate = request.body.find("\r\n", 0);
            mime = request.body.substr(12 + 2, separate - 12 - 2);
            request.body.erase(0, separate + 2);
        }
    }
    /*
     * прочитать содержимое файла
     */
    if ((pos = request.body.find(boundary, 0)) != std::string::npos)
    {
        content = request.body.substr(0, pos - 2);
        request.body.erase(0, pos - 2);
        request.body.clear();

        status = createNewFile("avatar", content, "png");
    }
    return status;
}

int Handler::createNewFile(std::string fileName, std::string content, std::string fileExtension)
{

    if (fileName.length() == 0 && fileExtension.length() == 0 )
        return 1;

    std::string path = "." + config.locations[index_location]->root + "/"; // "./content/website1/users/";
    path += _userData.login;
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    path += "/";
    path += "avatar.png";


    std::ofstream outfile (path);
    outfile << content << std::endl;
    outfile.close();

    return 200;
}

/*
 * ----------------------------------------- PUT --------------------------------------
 */

void Handler::handle_put(void)
{
    int fd;
    int status_code = 204;

    fd = open(this->path.c_str(), O_RDWR);
    if (fd < 0)
        status_code = 201;
    else
        close(fd);

    std::ofstream ofs(this->path.c_str(), std::ios_base::trunc);
    if (!ofs.good())
        return error_message(500);

    ofs << request.body;
    ofs.close();

    addHeaderStatus(status_code);
    addHeaderServer();
    addHeaderDate();
    addHeaderContentLanguage();
    addHeaderContentLocation();
    this->response.append("Content-Type: ");
    this->response.append("text/plain"); //нужно подтянуть из хедера запроса, если он есть
    this->response.append("\r\n");
    addHeaderContentLength(lltostr(request.body.size(), 10));
    addHeaderLocation();
    this->response.append("\r\n");

    if (status_code == 201)
        this->response.append(request.body);
}

/*
 * ---------------------------------------- DELETE --------------------------------------
 */

void Handler::handle_delete(void)
{
    int     status = 200;
    bool    deleteAccess = OFF;

    std::string truePath = "./content/users/";
    truePath += _userData.login;
    truePath += "/";

    size_t pos = this->path.find(truePath, 0);

    if (_userData.signIn == true && pos == 0)
        deleteAccess = ON;
    else
        status = 403;

    if (deleteAccess == OFF)
        error_message(status);
    else
    {
        if (remove(this->path.c_str()) != 0 ) // this->path
            return error_message(404);


        std::string body = "<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n</html>";

//        if (isDir && config.locations[this->index_location]->autoIndex == ON)
//            makeAutoindexPage(&body);
//        else if (checkFile() != 0)
//            return;

        addHeaderStatus(status);
        addHeaderServer();
        addHeaderDate();
        addHeaderContentLanguage();
        addHeaderContentLocation();
        this->response.append("Content-Type: text/html\r\n");
        addHeaderContentLength(std::to_string(body.length()));
        addHeaderLocation();
        addHeaderLastModified();
        this->response.append("\r\n");
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
	this->contentLength = lltostr(body->length(), 10);
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
		if (errno == ENOENT || errno == EFAULT || errno == ENOTDIR)
			error_message(404);
		else
			error_message(500);
		return 1;
	}
	
	fstat(fd, &file_stat);
	this->contentLength = lltostr(file_stat.st_size, 10);
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
	ss.clear();
	ifs.close();
}

void Handler::loadBodyFromFile(std::string * body, std::string path)
{
    std::ifstream ifs(path.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();

    body->append(ss.str());
    ss.clear();
    ifs.close();
}

void         Handler::add_headers(std::vector<std::string> * headers)
{
	std::string contentType = request.headers->find("Content-Type")->second;

	headers->push_back("AUTH_TYPE=Anonymous");
	headers->push_back("CONTENT_LENGTH=" + lltostr(request.body.length(), 10));
	headers->push_back("CONTENT_TYPE=" + contentType);
	headers->push_back("GATEWAY_INTERFACE=CGI/1.1");
	headers->push_back("PATH_INFO=" + request.path);
	headers->push_back("PATH_TRANSLATED=" + this->path);
	headers->push_back("QUERY_STRING=");
	headers->push_back("REMOTE_ADDR=");
	headers->push_back("REMOTE_IDENT=");
	headers->push_back("REMOTE_USER=");
    headers->push_back("REQUEST_METHOD=" + request.method);
	headers->push_back("REQUEST_URI=" + request.path);
	headers->push_back("SCRIPT_NAME=" + config.locations[index_location]->cgi_name);
	headers->push_back("SERVER_NAME=" + config.server_name);
	headers->push_back("SERVER_PORT=" + lltostr(config.port[0], 10)); //// hardcode
    headers->push_back("SERVER_PROTOCOL=HTTP/1.1");
	headers->push_back("SERVER_SOFTWARE=Webserv/1.1");

	std::multimap<std::string, std::string>::iterator it = request.headers->begin();
	for (; it != request.headers->end(); it++)
		headers->push_back("HTTP_" + it->first + "=" + it->second);
}

char **         Handler::create_env(void)
{
    char **result; // all headers
	std::vector<std::string> headers;
	int headersNmb;

	add_headers(&headers);
    headersNmb = headers.size();

	if (!(result = (char **)calloc(headersNmb + 1, sizeof(char*))))
        return NULL;

    for (int i = 0; i < headersNmb; i++)
    {
        if (!(result[i] = ft_strdup(headers[i].c_str())))
        {
            ft_free_array(result);
            return (NULL);
        }
    }
    return result;
}

int Handler::launch_cgi(char **args, char **env, std::string * body)
{
int std_input = dup(0);
    int status = 0;
    int fd[2];
    pid_t pid;

	errno = 0;

    if (pipe(fd) != 0)
    {
        error_message(500);
        status = 1;
        return status;
    }
    pid = fork();
    if (pid == 0) // дочерний процесс
    {
       	dup2(fd[1], 1); // fd to write
		int in = open(this->path.c_str(), O_RDWR);
		// int out = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
		// dup2(out, 1);
        close(fd[0]);
		dup2(in, 0);
        if (execve(args[0], args, env) == -1)
        {
			std::cerr << RED << strerror(errno) << BW << std::endl;
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

		char buffer[INBUFSIZE];
        int res = 0;
		size_t offset = 0;

        while((res = read(fd[0], buffer, INBUFSIZE - 1)) > 0) {
			buffer[res] = 0;
			if (body->length() == 0) {
				std::string temp(buffer);
				offset = temp.find("\r\n\r\n");
				body->append(temp, 0, offset);
				body->append("\r\n\r\n");
				offset += 4;
				body->append(lltostr(res - offset, 16));
				body->append("\r\n");
				body->append(temp, offset, res - offset);
				body->append("\r\n");
			}
			else
			{
				body->append(lltostr(res, 16));
				body->append("\r\n");
				body->append(buffer, 0, res);
				body->append("\r\n");
			}
		}
		body->append("0\r\n\r\n");
        if (res < 0)
        {
        	kill(pid, SIGKILL);
        	status = 1;
        }
        if (status == 1)
			error_message(500);
		dup2(std_input, 0);
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
    std::string message;

	switch(status_code)
	{
		case 400:
            message = "Bad Request";
			break;
		case 401:
            message = "Unauthorized";
            break;
	    case 403:
            message = "Forbidden";
            break;
		case 404:
            message = "Not found";
			break;
		case 405:
            message = "Method Not Allowed";
			allow_header();
			break;
		case 413:
            message = "Payload Too Large";
			break;
		case 500:
            message = "Internal Server Error";
			break;
		case 501:
            message = "Not Implemented";
			allow_header();
			break;
		case 505:
            message = "HTTP Version Not Supported";
			break;
        case 511:
            message = "Network Authentication Required";
            break;
	}
    this->response.append(std::to_string(status_code));
    this->response.append(" ");
    this->response.append(message);
    this->response.append("\r\n");

    if (status_code == 401 || status_code == 511)
    {
        this->response.append("WWW-Authenticate: Basic realm=\"Access to the staging site\", charset=\"UTF-8\"\r\n");
        this->response.append("Content-Disposition: inline\r\n");
    }

    std::string body;
    std::string errorPagePath = "." + config.error_page;
    loadBodyFromFile(&body, errorPagePath);

    size_t pos = body.find("<?php errorStatus(); ?>", 0);
    if (pos != std::string::npos)
    {
        std::string errorStatus = "<h1></h1><h2></h2>";
        errorStatus.insert(13, message);
        errorStatus.insert(4, std::to_string(status_code));

        body.erase(pos, 23);
        body.insert(pos, errorStatus);
    }

    addHeaderContentLength(std::to_string(body.length()));

    //    addHeaderServer();
//    addHeaderDate();
//    addHeaderContentLanguage();
    //    this->response.append("Content-Type: text/html\r\n");
//    addHeaderLastModified();

    this->response.append("Location: /error_page/");
    this->response.append("\r\n");
//    addHeaderContentLocation();


    this->response.append("Content-Location: /error_page/");
    this->response.append("\r\n");

    this->response.append("\r\n");
    this->response.append(body);

    std::cout << this->response  << std::endl;
	return ;
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

std::string Handler::subpath(void)
{
    size_t i = 0;
    std::string loc_path = config.locations[index_location]->path;
    if (loc_path != "/close" || loc_path != "/download") // delete
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
 * ------------------------- Add headers ----------------------------
 */

void Handler::addHeaderStatus(int status) // no errors here
{
    switch (status) {
        case 200:
            this->response.append("200 OK\r\n");
            break;
        case 201:
            this->response.append("201 Created\r\n");
            break;
        case 204:
            this->response.append("204 No Content\r\n");
            break;
        case 301:
            this->response.append("301 Moved Permanently\r\n");
            break;
        case 308:
            this->response.append("308 Permanent Redirect\r\n");
            break;
        case 404:
            this->response.append("404 Not Found\r\n");
            break;
    }
}
void Handler::addHeaderServer() { this->response.append("Server: Webserv/1.1\r\n"); }
void Handler::addHeaderDate()
{
    this->response.append("Date: ");
    this->response.append(getPresentTime());
    this->response.append("\r\n");
}
void Handler::addHeaderContentLanguage() { this->response.append("Content-Language: en\r\n"); }
void Handler::addHeaderContentLocation()
{
    this->response.append("Content-Location: ");
    this->response.append(this->location_path);
    this->response.append("\r\n");
}
void Handler::addHeaderContentLength(std::string size)
{
    this->response.append("Content-Length: ");
    this->response.append(size);
    this->response.append("\r\n");
}
void Handler::addHeaderLocation(void)
{
    this->response.append("Location: ");

    if (config.locations[index_location]->repeat_redirect == true)
        this->response.append(config.locations[index_location]->redirect);
    else
        this->response.append(this->location_path);
    this->response.append("\r\n");
}

void Handler::addHeaderContentType(void) // hardcode
{
//    this->response.append("Content-Type: ");
//    if (this->request.path.length() > 0)
//    {
//        size_t pos = this->request.path.find(".", 1);
//        std::string fileExtension = this->request.path.substr(pos, std::string::npos);
//        if (fileExtension == "png")
//            this->response.append("image/png");
//        else if (fileExtension == "jpeg")
//            this->response.append("image/jpeg");
//        else
//            this->response.append("text/html");
//    }
//    else
//        this->response.append("text/html");

    this->response.append("Content-Type: ");

    if (this->request.path.find(".png", 0) != std::string::npos)
        this->response.append("image/png");
    if (this->request.path.find(".jpeg", 0) != std::string::npos)
        this->response.append("image/jpeg");
    else
        this->response.append("text/html");
    this->response.append("\r\n");
}

void Handler::addHeaderLastModified(void)
{
    this->response.append("Last-Modified: ");
    this->response.append(this->lastModTime);
    this->response.append("\r\n");
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
   while (*tmp != NULL)
   {
       free(*tmp);
       tmp++;
   }
   free(to_free);
   to_free = NULL;
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

std::string Handler::lltostr(long long number, int base)
{
	std::string res;
	
	if (number >= 10)
	{
		while (number > 0)
		{
			if (base == 16 && number % base >= 10)
				res.insert(res.begin(), (number % base + 87));
			else
				res.insert(res.begin(), (number % base + '0'));
			number = number / base;
		}
	}
	else
		res.insert(res.begin(), (number + '0'));
	return res;
}