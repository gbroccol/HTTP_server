#ifndef SESSION_HPP
# define SESSION_HPP

# include "Webserv.hpp"
# include "ParseRequest.hpp"
# include "Handler.hpp"
# include "Authentication.hpp"

enum states {
    fsm_start, fsm_finish, fsm_error, fsm_request
};

class Session
{
public:
	int fd;
    unsigned long from_ip;
    unsigned short from_port;
    std::string buf;
	std::string wr_buf;
    enum states state;

private:
	ParseRequest *      parseRequest;
	Handler *           handler;
	bool                request_left;

public:

   Session(configServer config, int fd);
    ~Session(void);
	int send_message(void);
	int do_read(void);
//	void commit(FILE *f);
	void handle_request(fd_set * writefds);
	void handle_cgi(fd_set * writefds);
	bool isRequestLeft(void);
	int  getCgiFd(void) const;
    void setAuthenticationOff();

private:

	Session(void);
};

#endif
