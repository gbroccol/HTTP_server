/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 12:51:05 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/10 17:07:55 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"
#include "Webserv.hpp"

Handler::Handler(void)
{
	this->response.append("HTTP/1.1");
	return;
}
Handler::~Handler(void)
{
	return;
}

// std::string const & Handler::handle(data request)
std::string const & Handler::handle(void)
{
	this->response.append("HTTP/1.1 ");

	// for debug:
	request.method.append("GET");
	request.path.append("/index.html");
	//нужно проверить, что локейшн отвечает на метод. Если нет -  ошибка 405

	// нужно вызвать функцию, которая соответствует методу запроса
	if (request.method == "HEAD" || request.method == "GET")
		handle_head();
	else if (request.method == "POST")
		;
	else if (request.method == "PUT")
		;
	else
		error_message(405);
	return this->response;
	
}

std::string const & lltostr(long long number)
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

void Handler::handle_head(void)
{
	//нужно найти файл в системе, а затем попытаться открыть его
	
	int fd;
	struct stat file_stat;

	if ( (fd = open(request.path.c_str(), O_RDONLY)) == -1)
		// error_message(404) ?
		;
	else
	{
		fstat(fd, &file_stat);
		this->response.append("200 OK\r\n");
		this->response.append("Server: Webserv/1.1\r\n");
		
		//вставляем дату
		//определяем language ??
		//отправляем location
		//определяем тип файла ??
		
		this->response.append("Content-Length: ");
		this->response.append(lltostr(file_stat.st_size));
		this->response.append("\r\n");

		this->response.append("Last-Modified: ");
		//переводим дату в нужный формат и добавляем к строке ответа
		this->response.append("\r\n");

		//Transfer-Encoding ?
		
		this->response.append("\r\n");

		if (request.method == "GET")
			handle_get(fd);
	}
	close(fd);
}

void Handler::handle_get(int fd)
{
	std::ostringstream os();
	//добавляем тело к ответу
	// os << fd;
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
	if (status_code)
	return;
}

void Handler::error_message_200(int const & status_code)
{
	if (status_code)
	return;
}

void Handler::error_message_300(int const & status_code)
{
	if (status_code)
	return;
}

void Handler::error_message_400(int const & status_code)
{
	if (status_code == 405)
	{
		this->response.append("405 Method Not Allowed\r\n");
		this->response.append("Allow: GET, HEAD, POST, PUT\r\n\r\n"); // нужно подтянуть методы из конфига по локейшену
	}
	return;
}

void Handler::error_message_500(int const & status_code)
{
	if (status_code)
	return;
}
