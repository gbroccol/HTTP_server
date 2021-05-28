#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "Webserv.hpp"

#define BW "\033[0;0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define PURPLE "\033[0;35m"

// _parsPart
#define	PRE_PART 0
#define	START_LINE_PART 1
#define HEADERS_PART 2
#define BODY_PART 3

// _packetPart
#define	HEX_CRLF_PART 0
#define	BODY_CRLF_PART 1
#define CRLF_PART 2

class ParseRequest
{

	public:

		ParseRequest();
		ParseRequest( ParseRequest const & src );
		~ParseRequest();

		ParseRequest &			operator=( ParseRequest const & rhs );

		bool					addToBuffer(std::string str); // change to void *

		void					parseHTTPRequest();
		void					parseStartingLine(std::string startLine);       // part 1
		void					parseHeaders(std::string header);               // part 2
		int 					parseBodyTE();                                  // part 3 transferEncoding BODY


        void					checkIfBody();

		void					clearData();




//		std::string				getBuff();

        data                    getData() const;



	private:

		std::string				_buff;
        size_t					_parsPart;
		struct data				_data; // tmp_data
        size_t                  _packetPart;
};

// std::ostream &			operator<<( std::ostream & o, ParseRequest const & i );

#endif /* **************************************************** PARSEREQUEST_H */
