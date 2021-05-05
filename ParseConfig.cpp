#include "ParseConfig.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParseConfig::ParseConfig()
{
}

ParseConfig::ParseConfig( const ParseConfig & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ParseConfig::~ParseConfig()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ParseConfig &				ParseConfig::operator=( ParseConfig const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ParseConfig const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */