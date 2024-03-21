
#ifndef WEBSERV_TCPLISTENER_HPP
#define WEBSERV_TCPLISTENER_HPP

#include "webserv.hpp"

class TcpListener {
public:
	TcpListener() {};
	//TcpListener(const std::vector<std::string> &server_names, int server_amount) {(void)server_names; (void)server_names;};
	void	http_listen();
	void	parse_request(const std::string &request);
	void	parse_config();
private:

};


#endif //WEBSERV_TCPLISTENER_HPP
