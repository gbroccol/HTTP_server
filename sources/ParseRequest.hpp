#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include "Webserv.hpp"

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

		bool					addToBuffer(std::string str);
		void					parseHTTPRequest();
		/*
		 * parse STARTING LINE
		 */
		void					parseStartingLine(std::string startLine);
		/*
		 * parse HEADERS
		 */
		void					parseHeaders(std::string header);
		/*
		 * parse BODY
		 */
        void					checkIfBody();
		int 					parseBodyChang();
        int                     parseContentLength();
        /*
         * clear
         */
		void					clearData();
		/*
		 * get
		 */
        data                    getData() const;
		bool					isRequestReady(void) const;

	private:

		std::string				_buff;
        size_t					_parsPart;
		struct data				_data;
        size_t                  _packetPart;
};

// std::ostream &			operator<<( std::ostream & o, ParseRequest const & i );

#endif /* **************************************************** PARSEREQUEST_H */
