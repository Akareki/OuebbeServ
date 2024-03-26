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

bool	is_directory(const std::string &path)
{
	struct stat statbuf;

	stat(path.c_str(), &statbuf);
	return S_ISDIR(statbuf.st_mode);
}

int	setResponseErrorBody(HTTPMessage &http_response, const std::string &full_error, const std::string &error_code, const std::map<std::string, std::string> &error_pages)
{
	http_response.setStatus(full_error);
	std::ifstream error_file(error_pages.at(error_code).c_str());
	if (error_file && error_file.is_open())
	{
		std::stringstream body_buffer;
		body_buffer << error_file.rdbuf();
		http_response.setBody(body_buffer.str());
		return 0;
	}
	return -1;
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
		if (setResponseErrorBody(http_response, "413 Request Entity Too Large", "413", _error_pages) == -1)
			http_response.setBody("<h1>413 Request Entity Too Large</h1");
	}
	else if (_allowed_methods.at(http_request.getMethod()) == false)
	{
		if (setResponseErrorBody(http_response, "405 Method Not Allowed", "405", _error_pages) == -1)
			http_response.setBody("<h1>405 Method Not Allowed.</h1>");
	}
	else if (!_redirect.empty())
	{
		http_response.setStatus("301 Moved Permanently");
		http_response.addHeader("Location", _redirect);
	}
	else if (http_request.getMethod() == "GET")
	{
		std::ifstream file(full_path.c_str());
		if (file && file.is_open() && !is_directory(full_path))
		{
			if (full_path.find("index.php") != std::string::npos)
			{
				char buffer[2000];
				char *list_buffer[100] = {const_cast<char*>("/usr/bin/php-cgi"), const_cast<char*>(full_path.c_str()), NULL};
				int pipefd[2];
				if (pipe(pipefd) == -1)
					throw std::exception();
				pid_t pid = fork();
				if (pid == -1)
					throw std::exception();
				if (pid == 0)
				{
					close(pipefd[0]);
					dup2(pipefd[1], 1);
					close(pipefd[1]);
					execve("/usr/bin/php-cgi", list_buffer, NULL);
				}
				else
				{
					close(pipefd[1]);
					ssize_t len_read;
					len_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
					if (len_read == -1)
						throw std::exception();
					buffer[len_read] = '\0';
					close(pipefd[0]);
					waitpid(pid, NULL, 0);
					//send the data
					std::string body = split(buffer, "\r\n")[2];
					http_response.addHeader("Content-Type", "text/html");
					http_response.addHeader("Content-Type", "charset=UTF-8");
					http_response.setBody(body);
				}
			}
			else
			{
				std::stringstream body_buffer;
				body_buffer << file.rdbuf();
				body = body_buffer.str();
				http_response.setBody(body);
			}
		}
		else if (_autoindex && (full_path[full_path.length() - 1] == '/' || (isindexadded && http_request.getPath() == "/")))
		{
			body = directory_listing(_root);
			http_response.setBody(body);
		}
		else
		{
			if (setResponseErrorBody(http_response, "404 Not Found", "404", _error_pages) == -1)
				http_response.setBody("<h1>404 Error</h1>");
		}
	}
	else if (http_request.getMethod() == "POST")
	{
		int number = 0;
		std::string filename = http_request.getFileName();
		if (filename.empty())
		{
			while (access(("database/file" + cpp_itoa(number)).c_str(), F_OK) == 0)
				number++;
			std::ofstream file(("database/file" + cpp_itoa(number)).c_str());
			if (file && file.is_open())
			{
				std::stringstream body_buffer;
				body_buffer << http_request.getBody();
				file << body_buffer.str();
				file.close();
			}
		}
		else
		{
			std::ofstream file(("database/" + filename).c_str(), std::ios::binary);
			if (file && file.is_open())
			{
				std::stringstream body_buffer;
				body_buffer << http_request.getBody();
				file << body_buffer.str();
				file.close();
			}
		}
	}
	else if (http_request.getMethod() == "DELETE")
	{
		if (remove(("database" + http_request.getPath()).c_str()) != 0)
		{
			if (setResponseErrorBody(http_response, "404 Not Found", "404", _error_pages) == -1)
				http_response.setBody("<h1>404 Not Found</h1>");
		}
		else
			http_response.setStatus("204 No Content");
	}

	//std::cout << "REQUEST : " << http_request.getMessage() << std::endl;
	//std::cout << "RESPONSE : " << http_response.getMessage() << std::endl;

	ssize_t size_send = send(connfd, http_response.getMessage().c_str(), http_response.getMessage().length(), MSG_CONFIRM);
	if (size_send == -1)
		throw std::exception();
	close(connfd);
}

std::string Location::get_full_path(const HTTPMessage &http_request, bool &isindexadded)
{
	std::string full_path(_root.substr(0, _root.length() - 1) + http_request.getPath());
	if (full_path[full_path.length() - 1] == '/')
	{
		full_path += _index;
		isindexadded = true;
	}
	return (full_path);
}