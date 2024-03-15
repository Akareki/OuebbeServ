
#ifndef WEBSERV_TCPLISTENER_HPP
#define WEBSERV_TCPLISTENER_HPP

#include "webh.hpp"
#include <iostream>
#include <sys/epoll.h>

class TcpListener {
public:
	TcpListener();
	//TcpListener(const std::vector<std::string> &server_names, int server_amount) {(void)server_names; (void)server_names;};
	void	http_listen();
	void	parse_request(const std::string &request);
	void	parse_config();
private:
	int 	_epoll_fd;
	int 	_sock_fd;
	struct epoll_event _event;
	struct epoll_event _events[10];
};


#endif //WEBSERV_TCPLISTENER_HPP
