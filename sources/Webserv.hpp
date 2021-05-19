
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <cstdlib> //malloc, free, exit
# include <unistd.h> //write, read, close, mkdir, rmdir, unlink, fork, getcwd, chdir, stat, lstat, fstat, lseek, execve, dup, dup2, pipe, usleep, select
# include <sys/stat.h> //stat, lstat, fstat
# include <sys/wait.h>  //wait, waitpid, wait3, wait4,
# include <fcntl.h>  //open, fcntl
# include <signal.h> // signal, kill
# include <netinet/in.h> //htons, htonl, ntohs, ntohl
# include <arpa/inet.h> // inet_addr
# include <sys/types.h> //opendir, readdir, closedir
# include <cstring>  // strerror
# include <cerrno> // errno
# include <sys/time.h> //gettimeofday
# include <time.h> //strptime, strftime
# include <sys/socket.h> // socket, accept, listen, send, recv, bind, connect, inet_addr, setsockopt, getsockname
# include <dirent.h> // opendir, readdir, closedir

# include <iostream>
# include <fstream> // нет в сабджекте
# include <sstream> // нет в сабджекте
# include <string>
# include <vector>
# include <list>
# include <queue>
# include <stack>
# include <map>
# include <algorithm>
# include <exception>
# include <ctime>

# define INBUFSIZE 1024
# define LISTEN_QLEN 32
# define INIT_SESS_ARR_SIZE 32

# define REQUEST_PARSE 0
# define REQUEST_READY 1


struct data
{
    std::string							    	method;
    std::string							    	path;
    std::string							    	version;

    std::multimap <std::string, std::string>    headers;

    std::string							    	body;
    std::string                                 bodyLen;

    size_t                                      status;
    int                                         nmb;


    /*
     * headers
    */
    bool                                        transferEncoding;
};



#endif
