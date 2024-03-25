#include "../includes/VirtualServer.hpp"

unsigned int line_nb = 1;

const std::string VirtualServer::optNames[OPTNB]
		= {"host", "listen", "server_name", "root",
		   "index", "autoindex", "client_max_body_size"};

void (VirtualServer::*VirtualServer::optSetters[OPTNB])(const std::string &) =
		{&VirtualServer::setHost, &VirtualServer::setPort, &VirtualServer::setServerName,
		 &VirtualServer::setRoot, &VirtualServer::setIndex, &VirtualServer::setAutoindex,
		 &VirtualServer::setClientMaxBodySize};

VirtualServer::VirtualServer() :
		_host(""), _port("8080"), _server_name(), _root("public_html"),
		_index("index.html"), _autoindex(false), _client_max_body_size(10000)
{
}

VirtualServer::VirtualServer(std::ifstream &config):
		_host(""), _port("8080"), _server_name(), _root(),
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

/*std::string directory_listing(const std::string &directory)
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
}*/

void	VirtualServer::answer_request(HTTPMessage &http_request, int connfd)
{
	//Location location;

	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if (it->first == http_request.getPath()) //TODO : replace with a function that checks if the path matches.
		{
			_locations[it->first].answer_request(http_request, connfd);
			break ;
		}
	}
	_locations["/"].answer_request(http_request, connfd);
	//location.answer_request(http_request, connfd);
	/*bool isindexadded = false;
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
	close(connfd);*/
}

std::string VirtualServer::get_full_path(const HTTPMessage &http_request, bool &isindexadded)
{
	std::string full_path(_root + http_request.getPath());
	if (full_path[full_path.length() - 1] == '/')
	{
		full_path += _index;
		isindexadded = true;
	}
	return (full_path);
}