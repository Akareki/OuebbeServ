//
// Created by wlalaoui on 3/15/24.
//

#include "TcpListener.hpp"

int	initialize()
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
	serv_addr.sin_port = htons(SERV_PORT);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

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
	char buffer[10000];
	ssize_t len_read = read(connfd, buffer, 10000);
	if (len_read == -1)
	{
		std::cerr << "read failed" << std::endl;
		close(connfd);
		throw std::exception();
	}
	buffer[len_read] = '\0';
	std::string request(buffer);

	return (request);
}

void	answer_request(const std::string &request, int connfd)
{
	VirtualServer virtual_serv;
	std::string full_request = virtual_serv.answer_request(request);
	ssize_t size_send = send(connfd, full_request.c_str(), full_request.length(), MSG_CONFIRM);
	if (size_send == -1) throw std::exception();
	std::cout << "size_send : " << size_send << std::endl;
	close(connfd);
}


void TcpListener::http_listen()
{
	int sockfd = initialize();
	int connfd;

	if (sockfd == -1)
		throw std::exception();
	struct epoll_event event;
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		throw std::exception();

	event.events = EPOLLIN;
	event.data.fd = sockfd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
	{
		close(epoll_fd);
		throw std::exception();
	}
	if (close(epoll_fd))
		throw std::exception();

	while (true)
	{
		connfd = accept_connection(sockfd);
		if (connfd == -1)
			continue ;
		std::string request = read_request(connfd);
		answer_request(request, connfd);
	}
}