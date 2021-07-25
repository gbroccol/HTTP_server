# webserv

An HTTP server that can respond to GET, HEAD, POST, PUT and DELETE methods.

To clone and compile code:

1. git clone https://github.com/gbroccol/HTTP_server.git webserv
2. cd webserv
3. make

To launch server with a website:

4. ./webserv
5. open a web browser and go to 127.0.0.1:8080

To launch server for testing:

4. ./webserv ./config/tests.conf
5. in a new tab of terminal: ./tester http://127.0.0.1:8080

Contributors:
* https://github.com/helenSsnow - config parsing, autoindex pages creating etc.
* https://github.com/kate-eversunny - connection establishment, reading from/writing to sockets, GET and PUT requests handling, POST with cgi-tester (Go script) handling.
* me - requests parsing, DELETE request handling, POST request with python CGI script handling, cookie, Session, html pages, request path parsing etc.
