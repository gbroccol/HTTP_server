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

    Handler(void);
	std::string 	response;
	data 			request;
	int 			index_location;
	std::string		path;
	std::string 	location_path;
	configServer	config;
	char            **env;


public:


    Handler(configServer const & config);
	~Handler(void);

	std::string const & handle(data const & request, char **env);
	int isRequestCorrect(void);
    int doesLocationAnswersMethod(void);
	void makePath(void);
    std::string subpath(void);
	void handle_head(void);
	void append_body(void);
	void handle_put(void);

	void handle_post(void);
	char ** create_env(void);


	int launch_cgi(char **args, char ** env, std::string * body);

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


	/* libft */
    int             ft_strlen(const char *str);
    void		    ft_free_array(char **to_free);
    char *          ft_strdup(const char *s);

};

#endif