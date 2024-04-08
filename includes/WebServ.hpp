//
// Created by wlalaoui on 3/15/24.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include "webh.hpp"
#include "Socket.hpp"

class Socket;


class WebServ {
public:
	WebServ(const std::string &);
	~WebServ();
	void	start();
	void display();
	void	http_listen();
	class WebServException: public std::exception
	{
	public:
		virtual const char *what() const throw();
		WebServException(const std::string &);
		~WebServException() throw();
	private:
		std::string _msg;
	};
private:
	WebServ &operator=(const WebServ&);
	WebServ();
	WebServ(const WebServ&);
	std::vector<Socket> _sockets;
	bool _running;
	std::map<int, Client> _clients;
	int _epollfd;
	struct epoll_event _event;
	struct epoll_event _events[10];
	std::map<int, Socket*> _socket_map;
	std::map<int, Socket*> _socket_connfd_map;
};


#endif //WEBSERV_WEBSERV_HPP
