/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:53:37 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/15 15:13:21 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServ.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

WebServ::WebServ()
{
}

WebServ::WebServ(const std::string &config_file_path)
{
	std::ifstream config_file;
	std::string line;
	unsigned int line_nb = 0;
	
	config_file.open(config_file_path);
	if (!config_file)
		throw WebServException("Can't open config file");
	std::getline(config_file, line);
	while (1)
	{
		if (line == "")
			continue;
		if (!config_file)
			break;
		if (line == "server {")
		{
			VirtualServer vserv(config_file);
			if (findSocket(_sockets.begin(), _sockets.end(), vserv) == _sockets.end())
			{
				Socket socket(vserv.getHost(), vserv.getPort());
				socket.addServer(vserv);
				_sockets.push_back(socket);
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
}

WebServ &WebServ::operator=(const WebServ &other)
{
	return(*this);
}


WebServ::WebServException::WebServException(const std::string &msg): _msg("Web server error: " + msg)
{
}

const char *WebServ::WebServException::what() const throw()
{
	return (_msg.c_str());
}




