#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

# include <iostream>
# include <string>
# include <vector>

#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <fstream>

#define BW "\033[0;0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define PURPLE "\033[0;35m"

class ParseConfig
{
	struct server{

	};
	public:

		ParseConfig();
		ParseConfig( ParseConfig const & src );
		~ParseConfig();

		ParseConfig &		operator=( ParseConfig const & rhs );

		void				parseFile(std::string file);




		class FileNotOpenException: public std::exception
		{
			const char    *what() const throw();
		};
		class FileIsNotCorrectException: public std::exception
		{
			const char    *what() const throw();
		};
		
	private:


};

std::ostream &			operator<<( std::ostream & o, ParseConfig const & i );

#endif /* ***************************************************** PARSECONFIG_H */