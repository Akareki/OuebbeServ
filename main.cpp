#include "webserv.hpp"
int main()
{
	TcpListener webserv;

	//webserv.parse();
	webserv.http_listen();

}