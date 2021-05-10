/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 12:51:05 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/10 13:47:16 by pvivian          ###   ########.fr       */
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

// std::string const & Handler::handle(заполненная структура с запросом)
std::string const & Handler::handle(void)
{
	this->response.append("HTTP/1.1 ");
	// нужно вызвать функцию, которая соответствует методу запроса
	// if GET ...
	// else if POST ..
	// else if HEAD...
	// else if PUT
	// else
		// error_message(405);
	return this->response;
	
}

void Handler::error_message(int status_code)
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

void Handler::error_message_100(int status_code)
{
	if (status_code)
	return;
}

void Handler::error_message_200(int status_code)
{
	if (status_code)
	return;
}

void Handler::error_message_300(int status_code)
{
	if (status_code)
	return;
}

void Handler::error_message_400(int status_code)
{
	if (status_code == 405)
	{
		this->response.append("405 Method Not Allowed\r\n");
		this->response.append("Allow: GET, HEAD, POST, PUT\r\n\r\n"); // нужно подтянуть методы из конфига по локейшену
	}
	return;
}

void Handler::error_message_500(int status_code)
{
	if (status_code)
	return;
}
