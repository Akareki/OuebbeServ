/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:41:36 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/19 10:13:18 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
#include "VirtualServer.hpp"
#include "defines.hpp"
#include <exception>
#include <string>
#include <vector>

class Socket
{
	public:
		Socket();
		Socket(const Socket&);
		Socket(const std::string &, const std::string &);
		~Socket();
		const std::string &getHost() const;
		const std::string &getPort() const;
		Socket &operator=(const Socket&);
		void addServer(VirtualServer);
		class SocketException: public std::exception
		{
			virtual const char *what() const throw();
		};
		void display();
	private:
		int _sockfd;
		std::string _host;
		std::string _port;
		std::vector<VirtualServer> _servers;
};

#endif
