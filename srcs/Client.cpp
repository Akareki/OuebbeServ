/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 09:19:26 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 11:10:05 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Client::Client()
{
}

Client::Client(int connfd): _connfd(connfd)
{
}

Client::~Client()
{
}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this == &other)
		return (*this);
	_connfd = other._connfd;
	return(*this);
}

int Client::getFd() const
{
	return (_connfd);
}

const std::string &Client::getRequest() const
{
	return (_request);
}

bool Client::isReady()
{
	if (_ready)
	{
		_ready = false;
		return (true);
	}
	return (_ready);
}

void Client::readRequest()
{
	char buffer[10000];
	ssize_t len_read = recv(_connfd, buffer, sizeof(buffer) - 1, O_NONBLOCK);
	if (len_read == -1)
	{
		close(_connfd);
		throw std::runtime_error("Recv error");
	}
	buffer[len_read] = '\0';
	_request = std::string(buffer);
	_ready = true;
}
