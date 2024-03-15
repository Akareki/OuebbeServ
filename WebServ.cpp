
#include "WebServ.hpp"

WebServ::WebServ()
{

}

void WebServ::start()
{
	TcpListener tcpListener;

	while (true)
	{
		tcpListener.http_listen();
	}
}