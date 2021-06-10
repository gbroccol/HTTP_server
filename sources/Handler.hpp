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

# define ON true
# define OFF false

class Handler
{
private:

    Handler(void);

	std::string 	response;
	data 			request;

	bool			isCgiReading;
	int             read_res;
	char *			tmp;

	int 			index_location;
	std::string		path;
	std::string 	location_path;

	configServer	config;
	user            _userData;
  
	std::vector<std::string> arrDir;
	bool			isDir;

	std::string		lastModTime;
	std::string		contentLength;

	bool            _error401;

public:


    Handler(configServer const & config);
	~Handler(void);

	std::string const & handle(data const & request, user & userData);
	std::string const & handle(void);
	int isRequestCorrect(void);
    int doesLocationAnswersMethod(void);
	void makePath(void);
    std::string subpath(void);

    /* 401 */
    void handle_401(void);

    /* HEAD */
    void handle_head(void);

    /* PUT */
	void handle_put(void);

	/* POST */
	void handle_post(void);
	char ** create_env(void);
  	void add_env(std::vector<std::string> * envs);
	int launchCgi(char **args, char ** env, std::string * body);
	int readCgi(std::string * body);
	int updateFile(std::string & boundary);
    int createNewFile(std::string fileName, std::string content, std::string fileExtension);

	/* DELETE */
    void handle_delete(void);

	std::string getPresentTime(void);
	std::string getLastModificationTime(time_t const & time);
	
	void error_message(int const & status_code);

int isFiles(std::string path, std::string locPath);
int putVal(std::string locPath,size_t j,size_t i, int theBestLocation, std::vector<location *> locations);
int searchlocPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i, int &theBestLocation,  std::vector<location *> locations,
						std::string &reqPath, int flag, std::string &locPath);
int searchreqPath(std::string &locTmp, std::string &reqTmp, size_t &j,size_t i,
						int &theBestLocation, std::vector<location *> locations, std::string &reqPath, std::string &locPath);
void searchPath(std::string &locTmp, std::string &reqTmp, std::string &locPath,size_t &j,size_t i,
						int &theBestLocation, std::string &reqPath, std::vector<location *> locations);
int isLocation(std::vector<location *> locations, std::string path);

    /*
     * ADD HEADERS
     */

    void addHeaderStatus(int status);
    void addHeaderServer(void);
    void addHeaderDate(void);
    void addHeaderContentLanguage(void);
    void addHeaderContentLocation(void);
    void addHeaderContentLength(std::string size);
    void addHeaderLocation(void);
    void addHeaderContentType(void);
    void addHeaderLastModified(void);
	void addHeaderAllow(void);

  	/*
  	 * libft
  	 */

    int             ft_strlen(const char *str);
    void		    ft_free_array(char **to_free);
    char *          ft_strdup(const char *s);
	std::string lltostr(long long number, int base);

    /*
     * extra
     */

    void	getFilesOrDirFromRoot(std::string LocPath);
    std::string getLink(std::string path);
    void makeAutoindexPage(std::string * body);
    int checkFile(void);
    void loadBodyFromFile(std::string * body);
	bool	isReadingCgi(void) const;

};

#endif