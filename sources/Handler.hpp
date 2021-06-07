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

#define ON true
#define OFF false

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
	bool            _signIn;

	std::vector<std::string> arrDir;
	bool			isDir;

	std::string		lastModTime;
	std::string		contentLength;

	bool            _error401;

public:


    Handler(configServer const & config);
	~Handler(void);

	std::string const & handle(data const & request, char **env, bool _signIn);
	int isRequestCorrect(void);
    int doesLocationAnswersMethod(void);
	void makePath(void);
    std::string subpath(void);

    void handle_401(void);
    void handle_head(void);
	void handle_put(void);

	void handle_post(void);
	char ** create_env(void);
    char ** add_headers(int len, int headersNmb, char **result);

	int launch_cgi(char **args, char ** env, std::string * body);

	std::string getPresentTime(void);
	std::string getLastModificationTime(time_t const & time);
	
	void error_message(int const & status_code);
	void allow_header(void);

	std::string lltostr(long long number, int base);

int isFiles(std::string path, std::string locPath);
int putVal(std::string locPath,size_t j,size_t i, int theBestLocation, std::vector<location *> locations);
int searchlocPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i, int &theBestLocation,  std::vector<location *> locations,
						std::string &reqPath, int flag, std::string &locPath);
int searchreqPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i,
						int &theBestLocation, std::vector<location *> locations, std::string &reqPath, std::string &locPath);
void searchPath(std::string &locTmp, std::string &reqTmp, std::string &locPath,size_t &j,size_t i,
						int &theBestLocation, std::string &reqPath, std::vector<location *> locations);
int isLocation(std::vector<location *> locations, std::string path);
  
  	/* libft */
    int             ft_strlen(const char *str);
    void		    ft_free_array(char **to_free);
    char *          ft_strdup(const char *s);

    void            getFilesOrDirFromRoot(std::string LocPath);
    std::string getLink(std::string path);
	void makeAutoindexPage(std::string * body);
	int checkFile(void);
	void loadBodyFromFile(std::string * body);
};

#endif