/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:13:29 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/21 09:47:21 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HTTPMessage.hpp"
#include <map>
#include <string>
#include <vector>

HTTPMessage::HTTPMessage(): _http_version("HTTP/1.1")
{
}

HTTPMessage::HTTPMessage(const std::string &status, const std::string &body):
	_http_version("HTTP/1.1"), _status(status), _body(body)
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
	return(*this);
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

