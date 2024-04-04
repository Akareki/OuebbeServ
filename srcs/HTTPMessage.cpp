/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:13:29 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/19 14:00:40 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HTTPMessage.hpp"
#define CRLF "\r\n"
#include <map>
#include <algorithm>
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
		std::vector<std::string> splited_second_half = split(second_half, ';');
		for (std::vector<std::string>::iterator it = splited_second_half.begin(); it != splited_second_half.end(); it++)
		{
			headers[first_half].push_back(*it);
		}
	}
}

HTTPMessage::HTTPMessage() : _http_version("HTTP/1.1"), _status("200 OK")
{
	this->addHeader("server", "webserv");
}

int check_request(const std::string request)
{
	std::string line;
	std::istringstream request_stream(request);
	std::getline(request_stream, line, '\n');

	if (split(line, ' ').size() != 3 || split(line,' ')[2] != "HTTP/1.1\r")
		return -1;
	if (split(request, "\r\n\r\n").size() != 2 && split(request, "\r\n\r\n").size() != 3)
		return -1;
	return 0;
}

HTTPMessage::HTTPMessage(const std::string &request) : _is_bad_request(false) //request constructor
{
	if (check_request(request) == -1)
	{
		_is_bad_request = true;
		return ;
	}
	//parse first line with path and method
	std::string line;
	std::istringstream request_stream(request);
	std::getline(request_stream, line, '\n');

	std::vector<std::string> first_line = split(line, ' ');

	_method = first_line[0];

	size_t index_interr = first_line[1].find('?');
	if (index_interr == std::string::npos)
		_path = first_line[1];
	else
		_path = first_line[1].substr(0, index_interr);
	std::cout << "path " << _path << std::endl;
	if (index_interr != std::string::npos)
		_url_params = first_line[1].substr(index_interr + 1);
	//parse headers
	fill_map(request, _headers);

	//parse body (POST requests usually have a body)
	const std::map<std::string, std::vector<std::string> > &requestMap = _headers;
	try {
		const std::vector<std::string> &contentType = requestMap.at("Content-Type");

		if (std::find(contentType.begin(),
					  contentType.end(), "multipart/form-data") != contentType.end())
		{
			std::vector<std::string> splited_request = split(request, "\r\n\r\n");
			_file_header = splited_request[1];
			_body = splited_request[2];
		}
		else
		{
			std::vector<std::string> splited_request = split(request, "\r\n\r\n");
			_body = splited_request[1];
		}

	} catch (std::exception &e) {
	}
}

bool HTTPMessage::isBadRequest() const
{
	return _is_bad_request;
}

std::string	HTTPMessage::getFileName() const
{
	size_t index_begin = _file_header.find("filename=\"", 0);
	if (index_begin == std::string::npos)
		return ("");
	size_t index_end = _file_header.find("\"", index_begin + 10);
	if (index_end == std::string::npos)
		return ("");
	std::string filename = _file_header.substr(index_begin + 10, index_end - (index_begin + 10));
	return (filename);
}

const std::map<std::string, std::vector<std::string> > &HTTPMessage::getHeaders() const
{
	return _headers;
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
		msg += it->first + ": ";
		for (std::vector<std::string>::const_iterator vit = it->second.begin(); vit != it->second.end(); vit++)
		{
			msg += *vit;
			if (vit != it->second.end() - 1)
				msg += "; ";
		}
		msg += CRLF;
	}
	msg += CRLF;
	msg += _body;
	return (msg);
}

const std::string &HTTPMessage::getBody() const
{
	return _body;
}

const std::string &HTTPMessage::getMethod() const
{
	return _method;
}

const std::string &HTTPMessage::getPath() const
{
	return _path;
}

const std::string &HTTPMessage::getUrlParams() const
{
	return _url_params;
}

void HTTPMessage::setStatus(const std::string &status)
{
	_status = status;
}

void HTTPMessage::setBody(const std::string &body)
{
	_body = body;
}