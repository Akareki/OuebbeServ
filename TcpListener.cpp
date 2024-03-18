//
// Created by wlalaoui on 3/15/24.
//

#include "TcpListener.hpp"

int	initialize(int port)
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
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	/*
	char *buff = "ouais";
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));
	*/

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

//init listen
TcpListener::TcpListener(int port)
{
	_sock_fd = initialize(port);
	if (_sock_fd == -1)
		throw std::exception();
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
		throw std::exception();

	_event.events = EPOLLIN;
	_event.data.fd = _sock_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _sock_fd, &_event))
	{
		close(_epoll_fd);
		throw std::exception();
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
	char buffer[10000] = {0};
	ssize_t len_read = recv(connfd, buffer, sizeof(buffer) - 1, O_NONBLOCK);
	if (len_read == -1)
	{
		std::cerr << "read failed" << std::endl;
		close(connfd);
		return ("");
		//throw std::exception();
	}
	buffer[len_read] = '\0';
	return (buffer);
}

void	answer_request(const std::string &request, int connfd)
{
	VirtualServer virtual_serv;
	std::string full_request = virtual_serv.answer_request(request);
	ssize_t size_send = send(connfd, full_request.c_str(), full_request.length(), MSG_CONFIRM);
	if (size_send == -1)
		throw std::exception();
	std::cout << "size_send : " << size_send << std::endl;
	close(connfd);
}

void TcpListener::http_listen()
{
		int connfd;
		int fd_amount = epoll_wait(_epoll_fd, _events, 10, 0);
		if (fd_amount == -1)
		{
			std::cerr << "epoll_wait failed" << std::endl;
			throw std::exception();
		}
		for (int n = 0; n < fd_amount; n++)
		{
			if (_events[n].data.fd == _sock_fd)
			{
				connfd = accept_connection(_sock_fd);
				if (connfd == -1)
				{
					std::cerr << "accept connection failed" << std::endl;
					throw std::exception();
				}
				//todo : set non blocking
				//fcntl(connfd, F_SETFL, O_NONBLOCK);
				_event.events = EPOLLIN | EPOLLET;
				_event.data.fd = connfd;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, connfd, &_event) == -1)
				{
					std::cerr << "epoll ctl failed" << std::endl;
					throw std::exception();
				}
			}
			else
			{
				std::string request = read_request(_events[n].data.fd);
				if (!request.empty())
					answer_request(request, _events[n].data.fd);
			}
		}
}