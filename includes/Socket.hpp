
#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include "webh.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstdlib>
#include "VirtualServer.hpp"

class VirtualServer;

class Socket {
public:
	Socket(const Socket&);
	Socket(const std::string &, const std::string &);
	~Socket();
	const std::string &getHost() const;
	const std::string &getPort() const;
	Socket &operator=(const Socket&);
	void addServer(VirtualServer);
	class SocketException: public std::exception
	{
		virtual const char *what() const throw();
	};
	void display();
	//Socket(const std::vector<std::string> &server_names, int server_amount) {(void)server_names; (void)server_names;};
	void	http_listen();
	void	parse_request(const std::string &request);
	void	answer_request(const std::string &request, int connfd);
private:
	Socket();
	int 	_epollfd;
	int 	_sockfd;
	std::string _host;
	std::string _port;
	std::vector<VirtualServer> _servers;
	struct epoll_event _event;
	struct epoll_event _events[10];
};


#endif //WEBSERV_TCPLISTENER_HPP
