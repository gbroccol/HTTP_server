/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:47:36 by pvivian           #+#    #+#             */

/*   Updated: 2021/05/10 15:41:58 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

# include "Webserv.hpp"
# include "Config.hpp"

struct data
	{
		std::string								method;
		std::string								path;
		std::string								version;

		std::multimap <std::string, std::string>		headers;

		std::string								body;
	};

class Handler
{
private:

	std::string response;
	data 		request; // for debug
	// int index_location;

public:
	Handler(void);
	~Handler(void);

	// std::string const & handle(заполненная структура с запросом);
	std::string const & handle(configServer const & config);
	int isRequestCorrect(configServer const & config);
	void handle_head(void);
	void append_body(int fd);

	std::string getPresentTime(void);
	std::string getLastModificationTime(time_t const & time);
	

	//можно вынести в отдельный класс
	void error_message(int const & status_code);
	void error_message_100(int const & status_code);
	void error_message_200(int const & status_code);
	void error_message_300(int const & status_code);
	void error_message_400(int const & status_code);
	void error_message_500(int const & status_code);

	std::string lltostr(long long number);
	
};

#endif