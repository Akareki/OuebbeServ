/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:19:15 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/25 11:51:47 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"
#include "../includes/VirtualServer.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

const std::string Location::optNames[OPTNB]
	= {"root", "index", "autoindex", "return", "client_max_body_size",
		"error_page", "allow"};

void (Location::*Location::optSetters[OPTNB])(const std::string &) =
	{&Location::setRoot, &Location::setIndex, &Location::setAutoindex,
		&Location::setRedirect, &Location::setClientMaxBodySize,
		&Location::setErrorPage, &Location::setAllowedMethods};

Location::Location()
{
}

Location::Location(const VirtualServer &vserv):
	_root(vserv.getRoot()), _index(vserv.getIndex()), _redirect(),
	_autoindex(vserv.getAutoindex()), _client_max_body_size(vserv.getClientMaxBodySize()),
	_error_pages(vserv.getErrorPages())
{
	_allowed_methods["GET"] = true;
	_allowed_methods["POST"] = true;
	_allowed_methods["DELETE"] = true;
}

Location::Location(const VirtualServer &vserv, std::ifstream &config):
	_root(vserv.getRoot()), _index(vserv.getIndex()), _redirect(),
	_autoindex(vserv.getAutoindex()), _client_max_body_size(vserv.getClientMaxBodySize()),
	_error_pages(vserv.getErrorPages())
{
	_allowed_methods["GET"] = true;
	_allowed_methods["POST"] = true;
	_allowed_methods["DELETE"] = true;

	std::string line;
	std::string opt_name;
	std::string opt_value;
	
	while (1)
	{
		line_nb++;
		std::getline(config, line);
		if (!config)
			throw LocationException("unexpected end of file.");
		line.erase(0, line.find_first_not_of(WS));
		if (line == "}")
			break;
		if (line.find_first_not_of(WS) == std::string::npos)
			continue;
		if (line.find(" ") == std::string::npos || line.find(" ") == line.size() - 1)
			throw LocationException("format error.");
		opt_name = line.substr(0, line.find(" "));
		opt_value = line.substr(line.find(" ") + 1);
		setOpt(opt_name, opt_value);
	}
}

Location::~Location()
{
}

Location::Location(const Location &other)
{
	*this = other;
}

Location &Location::operator=(const Location &other)
{
	_root = other._root;
	_index = other._index;
	_redirect = other._redirect;
	_autoindex = other._autoindex;
	_allowed_methods = other._allowed_methods;
	_client_max_body_size = other._client_max_body_size;
	_error_pages = other._error_pages;
	_allowed_methods = other._allowed_methods;
	return(*this);
}

void Location::setRoot(const std::string &opt_value)
{
	_root = opt_value;
}

void Location::setIndex(const std::string &opt_value)
{
	_index = opt_value;
}

void Location::setAutoindex(const std::string &opt_value)
{
	if (opt_value == "on")
		_autoindex = true;
	else if (opt_value == "off")
		_autoindex = false;
	else
		throw LocationException("wrong option value.");
}

void Location::setRedirect(const std::string &opt_value)
{
	_redirect = opt_value;
}

void Location::setClientMaxBodySize(const std::string &opt_value)
{
	long value;
	char *endptr;

	errno = 0;
	value = strtol(opt_value.c_str(), &endptr, 0);
	if (value <= 0 || errno == ERANGE || *endptr != '\0' )
		throw LocationException("wrong value for client_max_body_size");
	_client_max_body_size = value;
}

void Location::setErrorPage(const std::string &opt_value)
{
	if (opt_value.find(" ") == std::string::npos)
		throw LocationException("wrong value for error_page");
	_error_pages[opt_value.substr(0, opt_value.find(" "))] = opt_value.substr(opt_value.find(" ") + 1, std::string::npos);
}

void Location::setAllowedMethods(const std::string &opt_value)
{
	_allowed_methods["GET"] = false;
	_allowed_methods["POST"] = false;
	_allowed_methods["DELETE"] = false;
	std::vector<std::string> methods = split(opt_value, ',');
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == "GET" || *it == "POST" || *it == "DELETE")
			_allowed_methods[*it] = true;
		else
	  		throw LocationException("wrong method in allow");
	}
	
}

void Location::setOpt(const std::string &opt_name, const std::string &opt_value)
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
	throw LocationException("invalid option");	
}

const std::string &Location::getRoot() const
{
	return (_root);
}

const std::string &Location::getIndex() const
{
	return (_index);
}

bool Location::getAutoindex() const
{
	return (_autoindex);
}

const std::string &Location::getRedirect() const
{
	return (_redirect);
}

Location::LocationException::LocationException(const std::string &msg)
{
	std::stringstream tmp;

	tmp << line_nb;
	_msg = ("Location error on line " + tmp.str() + " : " + msg);
}

Location::LocationException::LocationException(const std::string &msg, int line)
{
	std::stringstream tmp;

	tmp << line;
	_msg = ("Location error on line " + tmp.str() + " : " + msg);
}

const char *Location::LocationException::what() const throw()
{
	return (_msg.c_str());
}

Location::LocationException::~LocationException() throw()
{}

void Location::display() const
{
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index: " << _index << std::endl;
	std::cout << "Autoindex: " << _autoindex << std::endl;
	std::cout << "Redirect: " << _redirect << std::endl;
	std::cout << "Client max body size: " << _client_max_body_size << std::endl;
	std::cout << "Error pages:" << _error_pages.size() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); it++)
	{
		std::cout << "	" << it->first << " -> " << it->second << std::endl;
	}
	std::cout << "Allowed methods: " << _allowed_methods.size() << std::endl;
	for (std::map<std::string, bool>::const_iterator it = _allowed_methods.begin(); it != _allowed_methods.end(); it++)
	{
		std::cout << "	" << it->first << " : " << it->second << std::endl;
	}
	std::cout << std::endl;
}
