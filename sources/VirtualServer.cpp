/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:33:51 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/18 14:35:51 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/VirtualServer.hpp"
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

unsigned int line_nb = 1;

const std::string VirtualServer::optNames[OPTNB]
	= {"host", "listen", "server_name", "root",
		"index", "autoindex", "client_max_body_size"};

void (VirtualServer::*VirtualServer::optSetters[OPTNB])(const std::string &) =
	{&VirtualServer::setHost, &VirtualServer::setPort, &VirtualServer::setServerName,
		&VirtualServer::setRoot, &VirtualServer::setIndex, &VirtualServer::setAutoindex,
		&VirtualServer::setClientMaxBodySize};

VirtualServer::VirtualServer():
	_host("127.0.0.1"), _port("8080"), _server_name(), _root(),
	_index("index.html"), _autoindex(false), _client_max_body_size(10000)
{
}

VirtualServer::VirtualServer(std::ifstream &config):
	_host("127.0.0.1"), _port("8080"), _server_name(), _root(),
	_index("index.html"), _autoindex(false), _client_max_body_size(10000)
{
	std::string line;
	std::string opt_name;
	std::string opt_value;

	while (1)
	{
		std::getline(config, line);
		if (line == "}")
			break;
		if (!config)
			throw VirtualServerException("unexpected end of file.", line_nb);
		if (line.find_first_not_of(WS) == std::string::npos)
			continue;
		line.erase(0, line.find_first_not_of(WS));
		if (line.find(" ") == std::string::npos || line.find(" ") == line.size() - 1)
			throw VirtualServerException("format error.");
		opt_name = line.substr(0, line.find(" "));
		opt_value = line.substr(line.find(" ") + 1);
		setOpt(opt_name, opt_value);
		line_nb++;
	}
}

const std::string &VirtualServer::getHost() const
{
	return (_host);
}

const std::string &VirtualServer::getPort() const
{
	return (_port);
}

const std::string &VirtualServer::getServerName() const
{
	return (_server_name);
}

const std::string &VirtualServer::getRoot() const
{
	return (_root);
}

const std::string &VirtualServer::getIndex() const
{
	return (_index);
}

bool VirtualServer::getAutoindex() const
{
	return (_autoindex);
}

unsigned int VirtualServer::getClientMaxBodySize() const
{
	return (_client_max_body_size);
}

void VirtualServer::setHost(const std::string &opt_value)
{
	_host = opt_value;
}

void VirtualServer::setPort(const std::string &opt_value)
{
	_port = opt_value;
}
void VirtualServer::setServerName(const std::string &opt_value)
{
	_server_name = opt_value;
}

void VirtualServer::setRoot(const std::string &opt_value)
{
	_root = opt_value;
}

void VirtualServer::setIndex(const std::string &opt_value)
{
	_index = opt_value;
}

void VirtualServer::setAutoindex(const std::string &opt_value)
{
	if (opt_value == "on")
		_autoindex = true;
	else if (opt_value == "off")
		_autoindex = false;
	else
		throw VirtualServerException("wrong option value.");
}

void VirtualServer::setClientMaxBodySize(const std::string &opt_value)
{
	long value;
	char *endptr;

	errno = 0;
	value = strtol(opt_value.c_str(), &endptr, 0);
	if (value <= 0 || errno == ERANGE || *endptr != '\0' )
		throw VirtualServerException("wrong value for client_max_body_size");
	_client_max_body_size = value;
}

void VirtualServer::setOpt(const std::string &opt_name, const std::string &opt_value)
{
	unsigned int idx = 0;
	while (idx < OPTNB)
	{
		if (opt_name == optNames[idx])
		{
			(this->*optSetters[idx])(opt_value);
			return ;
		}
		idx++;
	}
	throw VirtualServerException("invalid option");	
}

VirtualServer::~VirtualServer()
{
}

VirtualServer::VirtualServer(const VirtualServer &other)
{
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other)
{
	_host = other._host;
	_port = other._port;
	_server_name = other._server_name;
	_root = other._root;
	_index = other._index;
	_autoindex = other._autoindex;
	_client_max_body_size = other._client_max_body_size;
	_error_pages = other._error_pages;
	_locations = other._locations;
	return(*this);
}

VirtualServer::VirtualServerException::VirtualServerException(const std::string &msg)
{
	std::stringstream tmp;

	tmp << line_nb;
	_msg = ("Virtual server error on line " + tmp.str() + " : " + msg);
}

VirtualServer::VirtualServerException::VirtualServerException(const std::string &msg, int line)
{
	std::stringstream tmp;

	tmp << line;
	_msg = ("Virtual server error on line " + tmp.str() + " : " + msg);
}

const char *VirtualServer::VirtualServerException::what() const throw()
{
	return (_msg.c_str());
}

VirtualServer::VirtualServerException::~VirtualServerException() throw()
{}

void VirtualServer::display() const
{
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Server name: " << _server_name << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index: " << _index << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Client max body size: " << _client_max_body_size << std::endl;
}
