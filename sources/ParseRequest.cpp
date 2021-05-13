#include "ParseRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseRequest::ParseRequest()
{
	clearData();
}

// ParseRequest::ParseRequest( const ParseRequest & src )
// {
// }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ParseRequest::~ParseRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// ParseRequest &				ParseRequest::operator=( ParseRequest const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, ParseRequest const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

	void					ParseRequest::addToBuffer(std::string str)
	{

		std::cout << GREEN << "--- BUFFER ---" << std::endl << str << BW << std::endl << std::endl;
		_buff += str;

		if (_buff.find("\r\n", 0) != std::string::npos)
			this->parseHTTPRequest();
	}

	void					ParseRequest::checkIfBody()
    {
        std::multimap <std::string, std::string>::iterator itCL = _data.headers.find("Content-Length");
        std::multimap <std::string, std::string>::iterator itTE = _data.headers.find("Transfer-Encoding");

        if (itCL != _data.headers.end()) {
            std::cout << "I find Content-Length -> there is body part" << std::endl;
            _parsPart = BODY_PART;
        }
        else if (itTE != _data.headers.end()) {
            std::cout << "I find Transfer-Encoding -> there is body part" << std::endl;
            _parsPart = BODY_PART;
        }
        else
        {
//            std::cout << "NO BODY" << std::endl; // delete
            _data.status = REQUEST_READY;
        }
    }

	void					ParseRequest::parseHTTPRequest()
	{
		size_t pos;
		std::string tmp;
		
		while ((pos = _buff.find("\r\n", 0)) != std::string::npos)
		{
		    if (pos == 0) // && _parsPart == HEADERS_PART) // закончились заголовки
			{
		        _buff = _buff.erase(0, 2);
                checkIfBody();
                if (_data.status == REQUEST_READY)
                {
                    // launch
                    clearData();
                    std::cout << "NO BODY" << std::endl; // добавить дату в лист и продолжить считывание
                }
				continue ;
			}
		
			tmp.clear();
			tmp.insert(0, _buff, 0, pos);
			_buff.erase(0, pos + 2);

			if (_parsPart == PRE_PART)
				parseStartingLine(tmp.c_str());
			else if (_parsPart == START_LINE_PART || _parsPart == HEADERS_PART)
				parseHeaders(tmp);
//			else if (_parsPart == BODY_PART)
//				parseBody(tmp);
		}
	}

	void					ParseRequest::parseStartingLine(std::string head)
	{
        _parsPart = START_LINE_PART;

		std::cout << RED << "Starting Line " << BW;

		size_t pos = 0;
		
		// Find request type
		if ((pos = head.find(" ", 0)) != std::string::npos) // error
		{
			_data.method.insert(0, head, 0, pos);
			head.erase(0, pos + 1);
		}
		
		// Find path
		if ((pos = head.find(" ", 0)) != std::string::npos) // error
		{
			_data.path.insert(0, head, 0, pos);
			head.erase(0, pos + 1);
		}
		
		// Find HTTP version
		pos = head.size(); // error
		_data.version.insert(0, head, 0, pos);

		std::cout << BLUE << _data.method << " " << _data.path << " " << _data.version << BW << std::endl << std::endl;
	}

	void					ParseRequest::parseHeaders(std::string header)
    {
	    _parsPart = HEADERS_PART;

		std::cout << RED << "Header " << BW;

		size_t pos;

		pos = header.find(":", 0);
		std::string key;
		key.clear();
		key.insert(0, header, 0, pos);
		header = header.erase(0, pos + 2);

		pos = header.length();
		std::string value;
		value.clear();
		value.insert(0, header, 0, pos);
//		header = header.erase(0, pos + 2);
	
		_data.headers.insert(std::make_pair(key, value));
		std::cout << BLUE << key << ": " << value << BW << std::endl;
	}

//	void					ParseRequest::parseBody(std::string body)
//	{
//		if (checkEndBody(body))
//		{
//			clearData();
//			_parsPart = PRE_PART;
//			parseStartingLine(body.c_str());
//			parseHTTPRequest();
//			return;
//		}
//		_data.body += body;
//		_data.body += "\r\n";
//
//		std::cout  << std::endl << RED << "Body " << BW;
//		std::cout << BLUE << _data.body << BW << std::endl;
//	}

	void					ParseRequest::clearData()
	{
        _parsPart = PRE_PART;

		_data.method.clear();
		_data.path.clear();
		_data.version.clear();
		_data.headers.clear();
		_data.body.clear();
		_data.status = REQUEST_PARSE;
	}

/*
** --------------------------------- GET ----------------------------------
*/

	std::string					ParseRequest::getBuff() { return _buff; }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
