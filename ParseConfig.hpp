#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

# include <iostream>
# include <string>

class ParseConfig
{

	public:

		ParseConfig();
		ParseConfig( ParseConfig const & src );
		~ParseConfig();

		ParseConfig &		operator=( ParseConfig const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, ParseConfig const & i );

#endif /* ***************************************************** PARSECONFIG_H */