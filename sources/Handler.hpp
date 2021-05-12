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

	std::string 	response;
	data 			request; // for debug
	int 			index_location;
	std::string		path;
	configServer	config;

public:
	Handler(void);
	~Handler(void);

	// std::string const & handle(заполненная структура с запросом);
	std::string const & handle(configServer const & config);
	int isRequestCorrect(void);
	void makePath(void);
	void handle_head(void);
	void append_body(void);
	void handle_put(void);

	std::string getPresentTime(void);
	std::string getLastModificationTime(time_t const & time);
	
	void error_message(int const & status_code);
	void allow_header(void);

	std::string lltostr(long long number);
	
};

#endif