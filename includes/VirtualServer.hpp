#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "Location.hpp"
#include "HTTPMessage.hpp"
#include <exception>
#include <fstream>
#include <map>
#include <string>
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <sstream>

#define WS " \t\r\n"
#define OPTNB 10

class Location;

class VirtualServer {
public:
	VirtualServer();
	VirtualServer(std::ifstream &);
	VirtualServer(const VirtualServer&);
	VirtualServer &operator=(const VirtualServer&);
	~VirtualServer();
	const std::string &getHost() const;
	const std::string &getPort() const;
	const std::string &getServerName() const;
	const std::string &getRoot() const;
	const std::string &getIndex() const;
	bool getAutoindex() const;
	unsigned int getClientMaxBodySize() const;
	std::map<std::string, std::string> getErrorPages() const;
	void setOpt(const std::string &, const std::string &);
	void setHost(const std::string &);
	void setPort(const std::string &);
	void setServerName(const std::string &);
	void setRoot(const std::string &);
	void setIndex(const std::string &);
	void setAutoindex(const std::string &);
	void setErrorPage(const std::string &);
	void setClientMaxBodySize(const std::string &);
	void addLocation(std::string &, Location &);
	class VirtualServerException: public std::exception
	{
	public:
		virtual const char *what() const throw();
		VirtualServerException(const std::string &);
		VirtualServerException(const std::string &, int);
		~VirtualServerException() throw();
	private:
		std::string _msg;
	};
	const static std::string optNames[OPTNB];
	static void (VirtualServer::*optSetters[OPTNB])(const std::string &);
	void display() const;
	std::string			get_full_path(const HTTPMessage &http_request, bool &isindexadded);
	void				answer_request(HTTPMessage &http_request, int connfd);
private:
	std::string _host;
	std::string _port;
	std::string _server_name;
	std::string _root;
	std::string _index;
	bool _autoindex;
	unsigned int _client_max_body_size;
	std::map<std::string, std::string> _error_pages;
	std::map<std::string, Location> _locations;
};


#endif //WEBSERV_SERVER_HPP
