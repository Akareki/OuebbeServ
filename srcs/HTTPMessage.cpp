/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:13:29 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/21 10:54:17 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HTTPMessage.hpp"
#include <sstream>
#define CRLF "\r\n"
#include <map>
#include <string>
#include <vector>

void	fill_map(const std::string &request, std::map<std::string, std::vector<std::string> > &headers)
{
	std::string line;
	std::istringstream request_stream(request);
	std::getline(request_stream, line, '\n');
	while (std::getline(request_stream, line, '\n'))
	{
		if (line.find(':') == std::string::npos)
			continue ;
		std::string first_half = line.substr(0, line.find(':'));
		if (line.find(' ') == std::string::npos || line.find('\r') == std::string::npos)
			continue ;
		std::string second_half = line.substr(line.find(' ') + 1, line.find('\r') - 6);
		headers[first_half].push_back(second_half);
	}
}

HTTPMessage::HTTPMessage()
{
}

HTTPMessage::HTTPMessage(const std::string &request)
{
	std::string line;
	std::istringstream request_stream(request);
	std::getline(request_stream, line, '\n');
	std::vector<std::string> first_line = split(line, ' ');

	_method = first_line[0];
	_path = first_line[1];
	fill_map(request, _headers);
}

const std::map<std::string, std::vector<std::string> > &HTTPMessage::getHeaders() const
{
	return _headers;
}

HTTPMessage::HTTPMessage(const std::string &status, const std::string &response_body):
	_http_version("HTTP/1.1"), _status(status), _body(response_body)
{
}

HTTPMessage::~HTTPMessage()
{
}

HTTPMessage::HTTPMessage(const HTTPMessage &other)
{
	*this = other;
}

HTTPMessage &HTTPMessage::operator=(const HTTPMessage &other)
{
	if (this == &other)
		return (*this);
	_http_version = other._http_version;
	_method = other._method;
	_path = other._path;
	_status = other._status;
	_headers = other._headers;
	_body = other._body;
	return (*this);
}

void HTTPMessage::addHeader(const std::string &key, const std::string &value)
{
	_headers[key].push_back(value);
}

std::string HTTPMessage::getMessage() const
{
	std::string msg;

	msg += _http_version + " " + _status + CRLF;
	for (std::map<std::string, std::vector<std::string> >::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		for (std::vector<std::string>::const_iterator vit = it->second.begin(); vit != it->second.end(); vit++)
		{
			msg += it->first + ": " + *vit + CRLF;
		}
	}
	msg += CRLF;
	msg += _body;
	return (msg);
}

const std::string &HTTPMessage::getMethod() const
{
	return _method;
}

const std::string &HTTPMessage::getPath() const
{
	return _path;
}

void HTTPMessage::setStatus(const std::string &status)
{
	_status = status;
}

void HTTPMessage::setBody(const std::string &body)
{
	_body = body;
}
