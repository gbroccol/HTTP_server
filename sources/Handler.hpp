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

class Handler
{
private:

	std::string 	response;
	data 			request;
	int 			index_location;
	std::string		path;
	std::string 	location_path;
	configServer	config;

public:
	Handler(void);
	~Handler(void);

	std::string const & handle(configServer const & config, data const & request);
	int isRequestCorrect(void);
    int doesLocationAnswersMethod(void);
	void makePath(void);
    std::string subpath(void);
	void handle_head(void);
	void append_body(void);
	void handle_put(void);

	std::string getPresentTime(void);
	std::string getLastModificationTime(time_t const & time);
	
	void error_message(int const & status_code);
	void allow_header(void);

	std::string lltostr(long long number);

	int isLocation(std::vector<location *> locations, std::string path);
	void searchPath(std::string &locTmp, std::string &reqTmp, std::vector<location *> locations,size_t i,
					size_t &j, int &theBestLocation, std::string path);
	int putVal(std::string &locTmp, std::string &reqTmp,
					std::vector<location *> locations,size_t i, size_t j, int theBestLocation);
	int isFiles(std::string path, std::string locPath);
};

#endif