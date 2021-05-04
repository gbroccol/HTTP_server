#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

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

#define BW "\033[0;0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define PURPLE "\033[0;35m"

class ParseRequest
{

	struct data
	{
		std::string								method;
		std::string								path;
		std::string								version;

		std::map <std::string, std::string>		headers;
	};
	

	public:

		ParseRequest();
		ParseRequest( ParseRequest const & src );
		~ParseRequest();

		ParseRequest &			operator=( ParseRequest const & rhs );

		void					addToBuffer(std::string str);

		void					addRequest();

		std::string				getBuff();
		

	private:

		std::string				_buff;
		struct data				_data;
		// std::string				_method;



};

// std::ostream &			operator<<( std::ostream & o, ParseRequest const & i );

#endif /* **************************************************** PARSEREQUEST_H */