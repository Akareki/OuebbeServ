/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 14:13:59 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/18 14:41:42 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"
#include <iostream>
#include <string>
#include <vector>

Socket::Socket()
{
}

Socket::Socket(const std::string &host, const std::string &port): _host(host), _port(port)
{
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
	return(*this);
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
