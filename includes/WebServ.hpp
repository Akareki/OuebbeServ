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
};


#endif //WEBSERV_WEBSERV_HPP
