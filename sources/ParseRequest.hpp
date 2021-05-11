#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "Webserv.hpp"

#define BW "\033[0;0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define PURPLE "\033[0;35m"

#define	PRE_PART 0
#define	START_LINE_PART 1
#define HEADERS_PART 2
#define BODY_PART 3

class ParseRequest
{

	struct data
	{
		std::string								method;
		std::string								path;
		std::string								version;

		std::multimap <std::string, std::string>		headers;

		std::string								body;
	};
	

	public:

		ParseRequest();
		ParseRequest( ParseRequest const & src );
		~ParseRequest();

		ParseRequest &			operator=( ParseRequest const & rhs );

		void					addToBuffer(std::string str); // change to void *



		void					parseHTTPRequest();
		void					parseStartingLine(std::string startLine);
		void					parseHeaders(std::string header);
		void					parseMessageBody(std::string body);
		size_t					checkEndBody(std::string body);
		void					clearData();




		std::string				getBuff();
		

	private:

		std::string				_buff;
		struct data				_data;
		
		size_t					_parsPart;



};

// std::ostream &			operator<<( std::ostream & o, ParseRequest const & i );

#endif /* **************************************************** PARSEREQUEST_H */