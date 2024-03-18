#include "VirtualServer.hpp"

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

std::string	VirtualServer::answer_request(const std::string &request)
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
	//body = directory_listing(_root_path + _index_file);
	std::string full_request = "HTTP/1.1 200 OK\r\nServer: webserv\r\n\r\n" + body;
	return full_request;
}

void VirtualServer::parse_request(const std::string &request)
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