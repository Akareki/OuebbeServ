/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:41:36 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/21 09:04:49 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
#include "VirtualServer.hpp"
#include "defines.hpp"
#include <exception>
#include <string>
#include <sys/epoll.h>
#include <vector>

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
