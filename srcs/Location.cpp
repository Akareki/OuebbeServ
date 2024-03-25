/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:19:15 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/19 11:27:58 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"
#include <iostream>
#include <sstream>
#include <string>

const std::string Location::optNames[OPTNB]
		= {"root", "index", "autoindex", "return"};

void (Location::*Location::optSetters[OPTNB])(const std::string &) =
		{&Location::setRoot, &Location::setIndex, &Location::setAutoindex,
		 &Location::setRedirect};

Location::Location(): _root(), _index(), _redirect(), _autoindex(false)
{
	_allowed_methods.push_back("GET");
	_allowed_methods.push_back("POST");
	_allowed_methods.push_back("DELETE");
}

Location::Location(std::ifstream &config):
		_root(), _index("index.html"), _redirect(), _autoindex(false)
{
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
}

std::string directory_listing(const std::string &directory)
{
	std::string html_body = "<html><body><ul>";
	DIR *dir = opendir(directory.c_str());
	struct dirent *s_read;

	if (!dir)
	{
		std::cerr << "opendir failed" << std::endl;
		return ("");
	}
	s_read = readdir(dir);
	while (s_read != NULL)
	{
		std::cout << "sread name : " << s_read->d_name << std::endl;
		html_body += "<a href=\"/";
		html_body += s_read->d_name;
		html_body += "\">";
		html_body += "<li>";
		html_body += s_read->d_name;
		html_body += "</li>";
		html_body += "</a>";
		html_body += "\n";
		s_read = readdir(dir);
	}
	html_body += "</ul></body><html>";
	closedir(dir);
	return html_body;
}

void Location::answer_request(HTTPMessage &http_request, int connfd)
{
	bool isindexadded = false;
	std::string full_path = this->get_full_path(http_request, isindexadded);
	std::cout << "full_path : " << full_path << std::endl;
	std::string body;
	HTTPMessage http_response;
	if (http_request.getBody().length() > _client_max_body_size)
	{
		http_response.setStatus("413 Request Entity Too Large");
		std::ifstream error_file("public_html/error_pages/413.html"); // TODO : replace with directory in config file
		if (error_file)
		{
			std::stringstream body_buffer;
			body_buffer << error_file.rdbuf();
			body = body_buffer.str();
			http_response.setBody(body);
		}
		ssize_t size_send = send(connfd, http_response.getMessage().c_str(), http_response.getMessage().length(), MSG_CONFIRM);
		if (size_send == -1)
			throw std::exception();
		close(connfd);
		return ;
	}
	if (http_request.getMethod() == "GET")
	{
		std::ifstream file(full_path.c_str());
		if (file)
		{
			std::stringstream body_buffer;
			body_buffer << file.rdbuf();
			body = body_buffer.str();
			http_response.setBody(body);
		}
		else if (_autoindex && (full_path[full_path.length() - 1] == '/' || (isindexadded && http_request.getPath() == "/")))
		{
			body = directory_listing(_root);
			http_response.setBody(body);
		}
		else
		{
			http_response.setStatus("404 Not Found");
			std::ifstream error_file("public_html/error_pages/404.html"); // TODO : replace with directory in config file
			if (error_file)
			{
				std::stringstream body_buffer;
				body_buffer << error_file.rdbuf();
				body = body_buffer.str();
				http_response.setBody(body);
			}
		}
	}
	else if (http_request.getMethod() == "POST")
	{
		int number = 0;
		while (access(("database/file" + cpp_itoa(number)).c_str(), F_OK) == 0)
			number++;
		std::ofstream file(("database/file" + cpp_itoa(number)).c_str());
		if (file)
		{
			std::stringstream body_buffer;
			body_buffer << http_request.getBody();
			file << body_buffer.str();
		}
		std::cout << "GET BODY : " << http_request.getBody() << std::endl;
	}
	else if (http_request.getMethod() == "DELETE")
	{
		if (remove(("database" + http_request.getPath()).c_str()) != 0)
			std::cerr << "could not remove file" << std::endl; //error to define (probably send error code)
	}

	std::cout << "REQUEST : " << http_request.getMessage() << std::endl;
	std::cout << "RESPONSE : " << http_response.getMessage() << std::endl;

	ssize_t size_send = send(connfd, http_response.getMessage().c_str(), http_response.getMessage().length(), MSG_CONFIRM);
	if (size_send == -1)
		throw std::exception();
	close(connfd);
}

std::string Location::get_full_path(const HTTPMessage &http_request, bool &isindexadded)
{
	std::string full_path(_root + http_request.getPath());
	if (full_path[full_path.length() - 1] == '/')
	{
		full_path += _index;
		isindexadded = true;
	}
	return (full_path);
}