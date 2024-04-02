/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:21:02 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 10:17:25 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"
#include "../includes/Client.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <unistd.h>

int	initialize(const std::string &host, int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	int opt = 1;

	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1)
		return 1;

	//initialize structure
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	if (host.empty())
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
	serv_addr.sin_port = htons(port);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//network funcs
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		return -1;
	}
	if (listen(sockfd, 10) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		return -1;
	}
	return sockfd;
}

Socket::Socket(const std::string &host, const std::string &port): _host(host), _port(port)
{
	_sockfd = initialize(host, atoi(port.c_str()));
	if (_sockfd == -1)
		throw std::runtime_error("sockfd issue");
	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		throw std::runtime_error("epoll create issue");

	_event.events = EPOLLIN;
	_event.data.fd = _sockfd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &_event))
	{
		close(_epollfd);
		throw std::runtime_error("epoll ctl issue");
	}
}

Socket::~Socket()
{
}

Socket::Socket(const Socket &other)
{
	*this = other;
}

Socket &Socket::operator=(const Socket &other)
{
	_host = other._host;
	_port = other._port;
	_sockfd = other._sockfd;
	_servers = other._servers;
	_epollfd = other._epollfd;
	_event = other._event;
	for (int i = 0; i < 10; i++)
		_events[i] = other._events[i];

	return (*this);
}

const std::string &Socket::getHost() const
{
	return (_host);
}

const std::string &Socket::getPort() const
{
	return (_port);
}

void Socket::addServer(VirtualServer vserv)
{
	_servers.push_back(vserv);
}

void Socket::display()
{
	std::cout << "----- SOCKET -----" << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << std::endl;
	for (std::vector<VirtualServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->display();
		std::cout << std::endl;
	}
}

int	accept_connection(int sockfd)
{
	int connfd;

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(sockaddr_in);

	connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (connfd < 0)
	{
		std::cout << "error happened " << std::endl;
		return -1;
	}
	return connfd;
}

std::string read_request(int connfd)
{
	char buffer[1000000];
	ssize_t len_read = recv(connfd, buffer, sizeof(buffer) - 1, O_NONBLOCK);
	std::cout << "len read : " << len_read << std::endl;
	if (len_read == -1)
	{
		std::cerr << "read failed" << std::endl;
		close(connfd);
		throw std::runtime_error("read problem");
	}
	buffer[len_read] = '\0';
	std::cout << buffer << std::endl;
	return (buffer);
}

void	Socket::answer_request(const std::string &request, int connfd)
{
	HTTPMessage http_request(request);

	bool answered = false;
	std::string server_name = http_request.getHeaders().at("Host")[0];
	for (std::vector<VirtualServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (server_name == it->getServerName())
		{
			it->answer_request(http_request, connfd);
			answered = true;
			break;
		}
	}
	if (!answered)
		_servers[0].answer_request(http_request, connfd);
}

void Socket::http_listen()
{
		int connfd;
		int fd_amount = epoll_wait(_epollfd, _events, 10, 0);
		if (fd_amount == -1)
		{
			std::cerr << "epoll_wait failed" << std::endl;
			throw std::runtime_error("epoll wait issue");
		}
		for (int n = 0; n < fd_amount; n++)
		{
			if (_events[n].data.fd == _sockfd)
			{
				connfd = accept_connection(_sockfd);
				if (connfd == -1)
				{
					std::cerr << "accept connection failed" << std::endl;
					throw std::runtime_error("accept issue");
				}
				_event.events = EPOLLIN | EPOLLOUT;
				_event.data.fd = connfd;
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, connfd, &_event) == -1)
				{
					std::cerr << "epoll ctl failed" << std::endl;
					throw std::runtime_error("epoll ctl issue");
				}
				_clients.at(connfd) = Client(connfd);
			}
			else
			{
				_clients.at(_events[n].data.fd).readRequest();
				std::cout << _clients.at(_events[n].data.fd).getRequest() << std::endl;
				std::string request = _clients.at(_events[n].data.fd).getRequest();
				if (!request.empty())
					this->answer_request(request, _events[n].data.fd);
				this->answer_request(request, _events[n].data.fd);
			}
		}
}
