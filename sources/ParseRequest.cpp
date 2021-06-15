#include "ParseRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseRequest::ParseRequest()
{
    _data.nmb      = 0;
    _data.headers  = new std::multimap<std::string, std::string>;
    _data.formData = new std::multimap<std::string, std::string>;
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
    delete _data.headers;
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
	    if (_data.status == REQUEST_READY)
	       clearData();

        _buff += str;
//		std::cout << GREEN << _buff << BW << std::endl << std::endl;

        this->parseHTTPRequest();

		if (_data.status == REQUEST_READY)
        {
		    if (_data.method.length() == 0)
            {
                _data.status = REQUEST_PARSE;
                if (_buff.length() != 0)
                    return true; // запустить парсинг снова
                return false; // буфер пустой
            }
//		    std::cout << YELLOW << "|" << _data.body << "|" << std::endl << std::endl;
            std::cout << GREEN << "REQUEST_READY" << BW << std::endl << std::endl;
        }


		if (_buff.length() != 0)
            return true; // запустить парсинг снова
        return false; // буфер пустой
	}



	void					ParseRequest::parseHTTPRequest() // <CRLF><CRLF> == "\r\n\r\n"
	{
		size_t pos;
		std::string tmp; // убрать tmp из body
		
		while (_parsPart != BODY_PART && (pos = _buff.find("\r\n", 0)) != std::string::npos)
		{
		    if (pos == 0) // закончились заголовки
            {
                _buff.erase(0, 2);
                checkIfBody();
                if (_data.status == REQUEST_READY)
                    return;
                break;
            }

			tmp.clear();
			tmp.insert(0, _buff, 0, pos);
			_buff.erase(0, pos + 2);

			if (_parsPart == START_LINE_PART || _parsPart == HEADERS_PART)
				parseHeaders(tmp);
            else if (_parsPart == PRE_PART)
                parseStartingLine(tmp.c_str());
		}

		int status = 0;

        while  (_parsPart == BODY_PART && _data.status != REQUEST_READY && !status)
        {
            switch (_data.bodyEncryption) {

                case TRANSFER_ENCODING_CHANG:
                    while (_data.status != REQUEST_READY && !status)
                        status = parseBodyChang();
                    break;
                case TRANSFER_ENCODING_COMPRESS:
                    break;
                case TRANSFER_ENCODING_DEFLATE:
                    break;
                case TRANSFER_ENCODING_GZIP:
                    break;
                case TRANSFER_ENCODING_IDENTYTY:
                    break;
                case CONTENT_LENGTH:
                    while (_data.status != REQUEST_READY && !status)
                        status = parseContentLength();
                    break;
            }
        }
	}

	void					ParseRequest::parseStartingLine(std::string head)
	{
        std::cout << YELLOW << "REQUEST #" << _data.nmb++ << std::endl << BW;

        _parsPart = START_LINE_PART;

		std::cout << RED << "Starting Line " << BW;

		size_t pos = 0;

		if ((pos = head.find(" ", 0)) != std::string::npos) // error
		{
			_data.method.insert(0, head, 0, pos);
			head.erase(0, pos + 1);
		}
		if ((pos = head.find(" ", 0)) != std::string::npos) // error
		{
			_data.path.insert(0, head, 0, pos);
			head.erase(0, pos + 1);

//            if (_data.path.find("/close/space.html", 0) != std::string::npos)
//                _data.path = "/close/space.html";
		}

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
	
		_data.headers->insert(std::make_pair(key, value));
		std::cout << BLUE << key << ": " << value << BW << std::endl;

//        std::multimap <std::string, std::string>::iterator printB = _data.headers.begin();
//        std::multimap <std::string, std::string>::iterator printEND = _data.headers.end();
//
//        while (printB != printEND)
//        {
//            std::cout << "FIRST -> " << printB->first << " SECOND -> " << printB->second << std::endl;
//
//            printB++;
//        }
	}

    void					ParseRequest::checkIfBody()
    {
        std::multimap <std::string, std::string>::iterator itCL = _data.headers->find("Content-Length");

        std::multimap <std::string, std::string>::iterator itTE = _data.headers->find("Transfer-Encoding");

        if (itCL != _data.headers->end())
        {
            _data.bodyEncryption = CONTENT_LENGTH;
            _data.bodyLen = stoi(itCL->second, 0, 10);
            _parsPart = BODY_PART;
        }
        else if (itTE != _data.headers->end()) //
        {
            if (itTE->second == "chunked")
                _data.bodyEncryption = TRANSFER_ENCODING_CHANG;
            else if (itTE->second == "compress")
                _data.bodyEncryption = TRANSFER_ENCODING_COMPRESS;
            else if (itTE->second == "deflate")
                _data.bodyEncryption = TRANSFER_ENCODING_DEFLATE;
            else if (itTE->second == "gzip")
                _data.bodyEncryption = TRANSFER_ENCODING_GZIP;
            else if (itTE->second == "identity")
                _data.bodyEncryption = TRANSFER_ENCODING_IDENTYTY;
            _parsPart = BODY_PART;
        }
        else
            _data.status = REQUEST_READY;
    }

	int				    	ParseRequest::parseBodyChang()     // <длина блока в HEX><CRLF><содержание блока><CRLF>
	{
	    /*
	     * <длина блока в HEX><CRLF>
	     */
	    if (_packetPart == HEX_CRLF_PART)
        {
            if (_buff.find("\r\n", 0) == std::string::npos)
                return 1; // need more data
            int pos = _buff.find("\r\n", 0);
            _data.bodyLen = stoi (_buff, 0, 16);
            _buff.erase(0, pos + 2);
            _packetPart = BODY_CRLF_PART;
        }
        /*
	     * <содержание блока>
	     */
        if (_packetPart == BODY_CRLF_PART)
        {
            if ((int)_buff.length() >= _data.bodyLen)
            {
                _data.body.append(_buff, 0, _data.bodyLen); // а я точно могу считать это все???
                _buff.erase(0, _data.bodyLen);
                _packetPart = CRLF_PART;
            }
            else
                return 1;
        }
        if (_packetPart == CRLF_PART)
        {
            if (_buff.find("\r\n", 0) != std::string::npos)
            {
                _buff.erase(0, 2);
                _packetPart = HEX_CRLF_PART;
                if (_data.bodyLen == 0)
                    _data.status = REQUEST_READY;
            } else
                return 1;
        }
        return 0;
	}

	int                     ParseRequest::parseContentLength()
    {
        if ((int)_buff.length() >= _data.bodyLen)
        {
            _data.body.append(_buff, 0, _data.bodyLen); // _data.bodyLen
            _buff.erase(0, _data.bodyLen);
            _data.status = REQUEST_READY;

            std::multimap <std::string, std::string>::iterator itCT = _data.headers->find("Content-Type");
            if (itCT != _data.headers->end() && itCT->second == "application/x-www-form-urlencoded")
            {
                // std::cout << YELLOW << _data.body << BW << std::endl;

                size_t pos = 0;
                std::string tmp = _data.body;
                std::string key;
                std::string value;

                while (tmp.length() > 0)
                {
                    key.clear();
                    value.clear();

                    pos = tmp.find("=", 0);
                    key.insert(0, tmp, 0, pos);
                    tmp = tmp.erase(0, pos + 1);

                    if ((pos = tmp.find("&", 0)) != std::string::npos)
                    {
                        value.insert(0, tmp, 0, pos);
                        tmp =tmp.erase(0, pos + 1);
                    }
                    else
                    {
                        value.insert(0, tmp, 0, tmp.length());
                        tmp.clear();
                    }
//                    std::cout << BLUE << key << ": " << value << BW << std::endl;
                    _data.formData->insert(std::make_pair(key, value));
                }
            }
        }
        else
            return 1;
        return 0;
    }

	void					ParseRequest::clearData()
	{
        _parsPart = PRE_PART;
        _packetPart = HEX_CRLF_PART;

		_data.method.clear();
		_data.path.clear();
		_data.version.clear();
		_data.headers->clear();
		_data.body.clear();
		_data.bodyLen = -1;
		_data.status = REQUEST_PARSE;

		/*
		 * headers
		 */
        _data.bodyEncryption = -1;
        _data.formData->clear();
	}

/*
** --------------------------------- GET ----------------------------------
*/

    data                        ParseRequest::getData() const { return _data; }
	bool						ParseRequest::isRequestReady(void) const
	{
		return _data.status == REQUEST_READY;
	}
    std::string             ParseRequest::getHost(void) const
    {
        std::map<std::string, std::string>::iterator it = _data.headers->find("Host");
        return it->second;
    }
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
