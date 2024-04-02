/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 09:19:26 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 13:47:09 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include <algorithm>
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

const HTTPMessage &Client::getRequest() const
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

int Client::readRequest()
{
	char buffer[10000];
	ssize_t len_read = recv(_connfd, buffer, sizeof(buffer) - 1, O_NONBLOCK);
	if (len_read <= 0)
	{
		close(_connfd);
		return (-1);
	}
	buffer[len_read] = '\0';
	_request = HTTPMessage(buffer);
	_ready = true;
	return (0);
}
