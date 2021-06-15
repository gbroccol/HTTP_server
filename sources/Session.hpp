#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"
# include "Handler.hpp"
# include "Authentication.hpp"


class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    unsigned long ip;
    unsigned short port;

    std::string buf;
	std::string wr_buf;

private:
    std::vector<configServer*> confServer;
	ParseRequest *      parseRequest;
	Handler *           handler;
    Authentication *    authentication;
	bool                request_left;
	

public:

   Session(std::vector<configServer*> config, Authentication * authentication, int fd);
    ~Session(void);
	int send_message(void);
	int do_read(void);
	void commit(FILE *f);
	void handle_request(fd_set * writefds);
	void handle_cgi(fd_set * writefds);
	bool isRequestLeft(void);
	int  getCgiFd(void) const;
    void checkAuthentication(void);
    configServer *getConfig(void);

private:

	Session(void);
    user    _user;
};

#endif
