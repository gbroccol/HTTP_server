/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:47:36 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/10 13:44:13 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

# include "Webserv.hpp"


class Handler
{
private:
	std::string response;

public:
	Handler(void);
	~Handler(void);

	// std::string const & handle(заполненная структура с запросом);
	std::string const & handle(void);
	void error_message(int status_code);
	void error_message_100(int status_code);
	void error_message_200(int status_code);
	void error_message_300(int status_code);
	void error_message_400(int status_code);
	void error_message_500(int status_code);
	
};

#endif