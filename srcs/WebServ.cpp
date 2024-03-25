/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:21:22 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/25 11:33:23 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServ.hpp"

unsigned int line_nb = 1;

WebServ::WebServ()
{

}

static std::vector<Socket>::iterator findSocket(std::vector<Socket>::iterator begin, std::vector<Socket>::iterator end, VirtualServer &vserv)
{
	while (begin != end)
	{
		if (begin->getHost() == vserv.getHost() && begin->getPort() == vserv.getPort())
			break;
		begin++;
	}
	return (begin);
}

WebServ::WebServ(const std::string &config_file)
{
	std::ifstream config;
	std::string line;

	config.open(config_file.c_str());
	if (!config)
		throw WebServException("Can't open config file");
	while (1)
	{
		std::getline(config, line);
		if (!config)
			break;
		if (line.find_first_not_of(WS) == std::string::npos)
			continue;
		if (line == "server {")
		{
			VirtualServer vserv(config);
			if (findSocket(_sockets.begin(), _sockets.end(), vserv) == _sockets.end())
			{
				Socket socket(vserv.getHost(), vserv.getPort());
				socket.addServer(vserv);
				_sockets.push_back(socket);
			}
			else
			{
				findSocket(_sockets.begin(), _sockets.end(), vserv)->addServer(vserv);
			}
		}
		else
			throw WebServException(std::string("Unexpected token in config file"));
	}
}

WebServ::~WebServ()
{
}

WebServ::WebServ(const WebServ &other)
{
	(void)other;
}

WebServ &WebServ::operator=(const WebServ &other)
{
	(void)other;
	return(*this);
}

void WebServ::display()
{
	for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
	{
		it->display();
	}
}

WebServ::WebServException::WebServException(const std::string &msg): _msg("Web server error: " + msg)
{
}

const char *WebServ::WebServException::what() const throw()
{
	return (_msg.c_str());
}

WebServ::WebServException::~WebServException() throw()
{
}


void WebServ::start()
{
	/*Socket socket("127.0.0.1", "8082");
	Socket socket2("127.0.0.1", "8083");
	TcpListener tcpListener3(8084);
	TcpListener tcpListener4(8085);
	TcpListener tcpListener5(8086);*/



	while (true)
	{
		for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
		{
			it->http_listen();
		}
		/*tcpListener3.http_listen();
		tcpListener4.http_listen();
		tcpListener5.http_listen();*/
	}
}
