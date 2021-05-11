/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 12:51:05 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/10 18:22:42 by pvivian          ###   ########.fr       */
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
std::string const & Handler::handle(void)
{
	// for debug:
	request.method.append("GET");
	request.path.append("/index.html");
	//

	this->response.append("HTTP/1.1 ");
	if (request.headers.count("Host") > 1)
		error_message(400);
	// else if нужно проверить, что локейшн отвечает на метод. Если нет -  ошибка 405
	else if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		;
	else if (request.method == "PUT")
		;

	this->response.append("\r\n");
	return this->response;
}

void Handler::handle_head(void)
{
	//нужно найти файл в системе, а затем попытаться открыть его
	// если файл не найден - ошибка 404
	
	int fd;
	struct stat file_stat;

	if ( (fd = open(request.path.c_str(), O_RDONLY)) == -1)
	{
		close(fd);
		error_message(500);
		return;
	}
	else
	{
		fstat(fd, &file_stat);
		this->response.append("200 OK\r\n");
		this->response.append("Server: Webserv/1.1\r\n");
		
		this->response.append("Date: ");
		this->response.append(getPresentTime());
		this->response.append("\r\n");
		
		//определяем language ??
		//отправляем location
		//определяем тип файла ??
		
		this->response.append("Content-Length: ");
		this->response.append(lltostr(file_stat.st_size));
		this->response.append("\r\n");

		this->response.append("Last-Modified: ");
		this->response.append(getLastModificationTime(file_stat.st_mtime));
		this->response.append("\r\n");

		//Transfer-Encoding ?

		if (request.method == "GET")
			append_body(fd);
	}
	close(fd);
}

void Handler::append_body(int fd)
{
	//добавляем тело к ответу
	if(fd)
		;
	// std::ostringstream os();
	// os << fd;
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
			this->response.append("Allow: GET, HEAD, POST, PUT\r\n"); // нужно подтянуть методы из конфига по локейшену
			break;
	}
	return;
}

void Handler::error_message_500(int const & status_code)
{
	switch(status_code)
	{
		case 500:
			this->response.append("500 Internal Server Error\r\n");
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