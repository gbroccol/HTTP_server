/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvivian <pvivian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 17:43:31 by pvivian           #+#    #+#             */
/*   Updated: 2021/05/07 16:28:43 by pvivian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp" 

Session::Session(void) 
{
	return; 
}

Session::~Session(void) { return; }

int Session::send_message(void)
{
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
	if ((write(this->fd, wr_buf.c_str(), wr_buf.length())) < 0)
	{
		this->state = fsm_error;
		return 0;
	}
	// this->state = fsm_finish;
	// return 0;
	return 1;
}

int Session::do_read(void)
{
	int read_res, bufp = this->buf_used;
	ParseRequest parseRequest;
	read_res = read(this->fd, this->buf + bufp, INBUFSIZE - bufp);
	if(read_res <= 0) {
		this->state = fsm_error;
		return 0;
	}

	//parse
	
	try {
	parseRequest.addToBuffer((std::string)this->buf);
	}
	catch (int const & e)
	{
		return 2;
	}
		
	this->buf_used += read_res;
	// if(this->buf_used >= INBUFSIZE) {
	// 	send_message("Line too long! Good bye...\n");
	// 	this->state = fsm_error;
	// 	return 0;
	// }
	if(this->state == fsm_finish)
		return 0;
	return 1;
}

void Session::do_write(const char *str, fd_set * writefds)
{
	this->wr_buf = (std::string)str;
	FD_SET(this->fd, writefds);
}

void Session::commit(FILE *f)
{
	// неразрешенные функции
	unsigned int ip = this->from_ip;
	fprintf(f, "From %d.%d.%d.%d:%d\n",
			(ip>>24 & 0xff), (ip>>16 & 0xff), (ip>>8 & 0xff), (ip & 0xff),
			this->from_port);
	fflush(f);
	
}

std::string Session::getPresentTime()
{
    char buffer[80];
    time_t seconds = time(NULL);
    std::string format = "%a, %d %b %Y %I:%M:%S";
    
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, format.c_str(), timeinfo);
    std::string resultTime = std::string(buffer);
    resultTime = resultTime + " GMT";
    return (resultTime);
}

std::string Session::getLastModificationFile(std::string fileName)
{
	struct stat buff;
	std::ifstream file(fileName);
		if (!file.is_open())
			std::cout << "FileNotOpenException"<<std::endl; /*Здесь должен быть Эксепшн*/
	stat(fileName.c_str(), &buff);
	tm *ltm = std::localtime(&buff.st_mtime); 
	char mbstr[100];
	std::strftime(mbstr, 100, "%a, %d %b %Y %I:%M:%S", ltm);
	std::string lastModifTime (mbstr);
	lastModifTime += " GMT";
	std::cout << lastModifTime<<std::endl;
	file.close();
	return (lastModifTime);
}
