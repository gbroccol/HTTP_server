/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 12:51:05 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/12 15:47:36 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"

Handler::Handler(void)
{
	return;
}
Handler::~Handler(void)
{
	return;
}

// std::string const & Handler::handle(data request)
std::string const & Handler::handle(configServer const & config)
{
	// for debug:
	request.method.append("GET");
	request.path.append("/index.html");
	request.version.append("HTTP/1.1");
	//

	this->response.append("HTTP/1.1 ");
	this->config = config;
	if (!isRequestCorrect())
		return this->response;
	
	makePath();
	if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		;
	else if (request.method == "PUT")
		;
	this->response.append("\r\n");
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
	// else if (есть ли такой локейшн. Если -1, то ошибка 404)
	else if (methods.find(request.method) == std::string::npos)
		status_code = 501;
	// else if нужно проверить, что локейшн отвечает на метод. Если нет -  ошибка 405
	
	if (status_code != 0)
	{
		error_message(status_code);
		this->response.append("\r\n");
		return 1;
	}
	return 0;
}

void Handler::makePath(void)
{
	//for debug
	this->index_location = 0;
	//

	DIR	*dir;
	
	this->path = ".";
	this->path.append(config.locations[index_location]->root);
	this->path.append(request.path);
	
	dir = opendir(path.c_str());
	if (dir)
		this->path.append(config.locations[index_location]->index);
	closedir(dir);
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
	this->response.append(this->path);
	this->response.append("\r\n");
		
	// как определяем тип файла ??
		
	this->response.append("Content-Length: ");
	this->response.append(lltostr(file_stat.st_size));
	this->response.append("\r\n");

	this->response.append("Last-Modified: ");
	this->response.append(getLastModificationTime(file_stat.st_mtime));
	this->response.append("\r\n");

	//Transfer-Encoding ?
	
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
	this->response.append("\r\n");
	ifs.close();
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
	if (status_code >= 100 && status_code < 200)
		error_message_100(status_code);
	else if (status_code >= 200 && status_code < 300)
		error_message_200(status_code);
	else if (status_code >= 300 && status_code < 400)
		error_message_300(status_code);
	else if (status_code >= 400 && status_code < 500)
		error_message_400(status_code);
	else if (status_code >= 500 && status_code < 600)
		error_message_500(status_code);
}

void Handler::error_message_100(int const & status_code)
{
	switch(status_code)
	{
		
	}
	return;
}

void Handler::error_message_200(int const & status_code)
{
	switch(status_code)
	{
		
	}
	return;
}

void Handler::error_message_300(int const & status_code)
{
	switch(status_code)
	{
		
	}
	return;
}

void Handler::error_message_400(int const & status_code)
{
	location * loc = config.locations[index_location];
	size_t size = loc->method.size();
	
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
			this->response.append("Allow: ");
			for (size_t i = 0; i < size; i++)
			{
				this->response.append(loc->method[i].c_str());
				if (i != size - 1)
					this->response.append(", ");
			}
			this->response.append("\r\n");
			break;
	}
	return;
}

void Handler::error_message_500(int const & status_code)
{
	location * loc = config.locations[index_location];
	size_t size = loc->method.size();
	
	switch(status_code)
	{
		case 500:
			this->response.append("500 Internal Server Error\r\n");
			break;
		case 501:
			this->response.append("501 Not Implemented\r\n");;
			this->response.append("Allow: ");
			for (size_t i = 0; i < size; i++)
			{
				this->response.append(loc->method[i].c_str());
				if (i != size - 1)
					this->response.append(", ");
			}
			this->response.append("\r\n");
			break;
		case 505:
			this->response.append("505 HTTP Version Not Supported\r\n");
			break;
	}
	return;
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