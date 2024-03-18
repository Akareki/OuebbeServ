
#include "WebServ.hpp"

WebServ::WebServ()
{

}

void WebServ::start()
{
	TcpListener tcpListener(8082);
	TcpListener tcpListener2(8083);
	/*TcpListener tcpListener3(8084);
	TcpListener tcpListener4(8085);
	TcpListener tcpListener5(8086);*/



	while (true)
	{
		tcpListener.http_listen();
		tcpListener2.http_listen();
		/*tcpListener3.http_listen();
		tcpListener4.http_listen();
		tcpListener5.http_listen();*/
	}
}