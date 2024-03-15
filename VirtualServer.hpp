#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "webh.hpp"

class VirtualServer {
public:
	VirtualServer() {};
	void			parse_request(const std::string &request);
	std::string	answer_request(const std::string &request);
private:
	std::map<int, std::string>			_error_code_list;
	//request attributes
	std::string 						_index_file;
	std::string 						_endpoint;
	std::map<std::string, std::string>	_request_headers;
	//config attributes
	std::string 						_root_path;
};


#endif //WEBSERV_SERVER_HPP
