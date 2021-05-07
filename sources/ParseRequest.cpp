#include "ParseRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseRequest::ParseRequest()
{
	_parsPart = PRE_PART;
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
		// std::string *tmp = static_cast<std::string *>(str);
		// _buff += *tmp;

		_buff += str;


// 		Starting Line GET / HTTP/1.1

// Header Host: 127.0.0.1:8080
// Header User-Agent: Go-http-client/1.1
// Header Accept-Encoding: gzip
// Starting Line Host:  127.0.0.1:8080

// Header User-Agent: Go-http-client/1.1
// Header Accept-Encoding: gzip


// Body coding: gzip

		// std::cout << GREEN << "--- BUFFER ---" << std::endl << _buff << BW << std::endl << std::endl;

		if (_buff.find("\r\n", 0) != std::string::npos)
			this->parseHTTPRequest();
	}

	void					ParseRequest::parseHTTPRequest()
	{
		size_t pos;
		std::string tmp;
		
		while ((pos = _buff.find("\r\n", 0)) != std::string::npos)
		{
			if (pos == 0 && _parsPart == HEADERS_PART)
			{
				_parsPart = BODY_PART;
				_buff = _buff.erase(0, 2);
				continue ;
			}
		
			tmp.clear();
			tmp.insert(0, _buff, 0, pos);
			_buff = _buff.erase(0, pos + 2); // !!!

			// std::cout << "delete " << pos << std::endl;

			if (_parsPart == PRE_PART)
				parseStartingLine(tmp.c_str());
			else if (_parsPart == START_LINE_PART || _parsPart == HEADERS_PART)
				parseHeaders(tmp);
			else if (_parsPart == BODY_PART)
				parseMessageBody(tmp);
		}
	}

	void					ParseRequest::parseStartingLine(std::string head)
	{
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

		_parsPart = START_LINE_PART;
	}

	void					ParseRequest::parseHeaders(std::string header)
	{
		// std::cout << RED << "Header to parse - " << header << BW << std::endl;
		std::cout << RED << "Header " << BW;

		size_t pos;

		pos = header.find(":", 0);
		std::string key;
		key.clear();
		key.insert(0, header, 0, pos);
		header = header.erase(0, pos + 2);

		pos = header.find("\r\n", 0);
		std::string value;
		value.clear();
		value.insert(0, header, 0, pos);
		header = header.erase(0, pos + 2);
	
		_data.headers.insert(std::pair<std::string, std::string> (key, value));
		std::cout << BLUE << key << ": " << value << BW << std::endl;

		
		if (_buff.find("\r\n", 0) == 0)
		{
			_parsPart = BODY_PART;
			_buff = _buff.erase(0, 2);
			std::cout << std::endl;
		}
		else
			_parsPart = HEADERS_PART;
	}

	void					ParseRequest::parseMessageBody(std::string body)
	{
		if (checkEndBody(body))
		{
			//handler
			// std::cout << "finish1" << std::endl;
			// throw 0;
			// getchar();

			clearData();
			_parsPart = PRE_PART;
			parseStartingLine(body.c_str());
			parseHTTPRequest();
			return;
		}
		_data.body += body;
		_data.body += "\r\n";

		std::cout  << std::endl << RED << "Body " << BW;
		std::cout << BLUE << _data.body << BW << std::endl;

	}

	size_t					ParseRequest::checkEndBody(std::string body)
	{
		if (body[0] == 'G')
		{
			std::cout << "stop" << std::endl  << std::endl;
			return 1;
		}
		return 0;
	}
	void					ParseRequest::clearData()
	{
		_data.method.clear();
		_data.path.clear();
		_data.version.clear();
		_data.headers.clear();
		_data.body.clear();
	}

/*
** --------------------------------- GET ----------------------------------
*/

	std::string					ParseRequest::getBuff() { return _buff; }


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */