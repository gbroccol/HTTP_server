#include "ParseConfig.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseConfig::ParseConfig()
{
}

// ParseConfig::ParseConfig( const ParseConfig & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ParseConfig::~ParseConfig()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// ParseConfig &				ParseConfig::operator=( ParseConfig const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, ParseConfig const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/
void				ParseConfig::parseFile(std::string file)
{
	std::string file_in_str;
	std::string tmp;


	if(file.length() != 0 && file == "example.conf")
	{
		std::ifstream config(file);
		if (!config.is_open())
			throw ParseConfig::FileNotOpenException();
		while (getline(config, tmp))
		{
			file_in_str += tmp;
		}
		std::cout << file_in_str<<std::endl;
	}
	else
		throw ParseConfig::FileIsNotCorrectException();
	

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/*
** --------------------------------- EXCEPTIONS ----------------------------------
*/
const char   *ParseConfig::FileNotOpenException::what() const throw()
{
	return ("File not open");
}

const char   *ParseConfig::FileIsNotCorrectException::what() const throw()
{
	return ("File is not correct");
}
/* ************************************************************************** */