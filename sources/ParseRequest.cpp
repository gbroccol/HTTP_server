#include "ParseRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseRequest::ParseRequest()
{
    _data.nmb = 0;
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

	bool					ParseRequest::addToBuffer(std::string str)
	{
        if (_data.nmb == 14)
            std::cout << BW;

	    if (_data.status == REQUEST_READY)
            clearData();

        _buff += str;
//		std::cout << GREEN << _buff << BW << std::endl << std::endl;


		if (_buff.find("\r\n", 0) != std::string::npos)
            this->parseHTTPRequest();

		if (_data.status == REQUEST_READY)
        {
		    std::cout << GREEN << "REQUEST_READY" << BW << std::endl << std::endl;
        }

		if (_buff.length() != 0)
            return true; // запустить парсинг снова
        return false; // буфер пустой
	}

	void					ParseRequest::checkIfBody()
    {
        std::multimap <std::string, std::string>::iterator itCL = _data.headers.find("Content-Length");
        std::multimap <std::string, std::string>::iterator itTE = _data.headers.find("Transfer-Encoding");

        if (itCL != _data.headers.end()) {// I find Content-Length -> there is body part
            _parsPart = BODY_PART;
        }
        else if (itTE != _data.headers.end()) {
            _parsPart = BODY_PART;
        }
        else
            _data.status = REQUEST_READY;
    }

	void					ParseRequest::parseHTTPRequest()
	{
		size_t pos;
		std::string tmp;
		
		while ((pos = _buff.find("\r\n", 0)) != std::string::npos)
		{
		    if (_parsPart != BODY_PART && pos == 0) // && _parsPart == HEADERS_PART) // закончились заголовки
			{
		        _buff = _buff.erase(0, 2);
                checkIfBody();
                if (_data.status == REQUEST_READY)
                    return;
                if (_buff.find("\r\n\r\n", 0) == std::string::npos)
                    return;
				continue ;
			}

            if (_parsPart == BODY_PART)
            {
                if (_buff.find("0\r\n\r\n", 0) == std::string::npos) // не обрабатывать body пока не найдем символы "0\r\n\r\n" - написать поиск с конца
                    return;
                tmp.clear();
                tmp.insert(0, _buff, 0, pos);
                _buff.erase(0, pos + 2);

                parseBody(tmp);
            }

			tmp.clear();
			tmp.insert(0, _buff, 0, pos);
			_buff.erase(0, pos + 2);

			if (_parsPart == PRE_PART)
                parseStartingLine(tmp.c_str());
			else if (_parsPart == START_LINE_PART || _parsPart == HEADERS_PART)
				parseHeaders(tmp);
		}
	}

	void					ParseRequest::parseStartingLine(std::string head)
	{
        std::cout << YELLOW << "REQUEST #" << _data.nmb++ << std::endl << BW;

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

	void					ParseRequest::parseBody(std::string size)
	{
	    _data.status = REQUEST_READY;
	    if (_data.bodyLen == -1) // count bodyLen
        {
	        if (size.length() == 1 && size == "0")
            {
                _data.bodyLen = 0;

                std::cout  << std::endl << RED << "Body " << BW;
                std::cout << BLUE << _data.body << BW << std::endl;

                _buff.erase(0, 2);

                return;
            }
            _data.bodyLen = 1000; // hex to decimal make func
        }
        _data.status = REQUEST_READY;

	    int pos = _buff.find("0\r\n\r\n", 0);
        _data.body.insert(0, _buff, 0, pos);


//        _data.body.clear();
//		_data.body.append(body);
//		_data.body += "\r\n";

		std::cout  << std::endl << RED << "Body " << BW;
		std::cout << BLUE << _data.body << BW << std::endl;

		_buff.erase(0, pos + 5);
	}

	void					ParseRequest::clearData()
	{
        _parsPart = PRE_PART;

		_data.method.clear();
		_data.path.clear();
		_data.version.clear();
		_data.headers.clear();
		_data.body.clear();
		_data.bodyLen = -1;
		_data.status = REQUEST_PARSE;
	}

/*
** --------------------------------- GET ----------------------------------
*/

//	std::string					ParseRequest::getBuff() { return _buff; }
    data                        ParseRequest::getData() const { return _data; }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
