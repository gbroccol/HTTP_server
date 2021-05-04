
#include "ParseRequest.hpp"
#include <iostream>

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

int main()
{
	ParseRequest parse;
	
	parse.addToBuffer("GET / HT");
	parse.addToBuffer("TP/1.1\r\n");

	parse.addToBuffer("Host: site.ru\r\n");
	parse.addToBuffer("User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:18.0) Gecko/20100101 Firefox/18.0\r\n");
	parse.addToBuffer("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
	parse.addToBuffer("Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n");
	parse.addToBuffer("Accept-Encoding: gzip, deflate\r\n");
	parse.addToBuffer("Cookie: wp-settings\r\n");
	parse.addToBuffer("Connection: keep-alive\r\n");
}
