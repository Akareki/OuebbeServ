
#include "Server.hpp"

std::string	Server::answer_request(const std::string &request)
{
	this->parse_request(request);

	std::string full_path = _root_path + _index_file;
	std::ifstream file1(full_path.c_str());
	std::string body;
	if (file1)
	{
		std::stringstream body_buffer;
		body_buffer << file1.rdbuf();
		body = body_buffer.str();
	}
	std::string full_request = "HTTP/1.1 200 OK\r\nServer: webserv\r\n\r\n" + body;
	return full_request;
}

void Server::parse_request(const std::string &request)
{
	if (DEBUG_MODE == 1)
		std::cout << request << std::endl;
	std::vector<std::string> request_vector = split(request, ' ');

	_index_file = request_vector[1];

	//set default rootpath
	if (_root_path.empty())
		_root_path = "public_html";
	if (_index_file.empty() || _index_file == "/")
		_index_file = "/index.html";

	std::cout << request_vector[1] << std::endl;
}