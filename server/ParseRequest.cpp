#include "ParseRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseRequest::ParseRequest()
{
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
		_buff += str;
		if (_buff.find("\r\n", 0) != std::string::npos)
			this->addRequest();
	}

	void					ParseRequest::addRequest()
	{
		size_t pos = _buff.find("\r\n", 0);

		// std::cout << RED << "pos = " << pos << BW << std::endl;

		// std::cout << RED << _buff << BW << std::endl;

		if (_data.method.length() == 0)
		{
			char tmp[pos];
			_buff.copy(tmp, pos, 0);

			_data.method = "GET";
			_data.path = "/";
			_data.version = "HTTP/1.1";

			std::cout << BLUE << _data.method << " " << _data.path << " " << _data.version << BW << std::endl;

			_buff = _buff.erase(0, pos + 2);
		}
		else
		{
			pos = _buff.find(":", 0);
			std::string key;
			key.clear();
			key.insert(0, _buff, 0, pos);
			_buff = _buff.erase(0, pos + 2);
			// std::cout << YELLOW << "pos = " << pos << BW << std::endl;
			
			pos = _buff.find("\r\n", 0);
			std::string value;
			value.clear();
			value.insert(0, _buff, 0, pos);
			_buff = _buff.erase(0, pos + 2);
			// std::cout << YELLOW << "pos = " << pos << BW << std::endl;

			_data.headers.insert(std::pair<std::string, std::string> (key, value));
			std::cout << BLUE << key << ": " << value << BW << std::endl;
		}
	}

/*
** --------------------------------- GET ----------------------------------
*/

	std::string					ParseRequest::getBuff() { return _buff; }


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */