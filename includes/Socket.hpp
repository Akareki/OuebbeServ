/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:41:36 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/15 11:43:20 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
#include "VirtualServer.hpp"
#include <exception>
#include <string>
#include <vector>

class Socket
{
	public:
		Socket(const std::string &, const std::string &);
		~Socket();
		class SocketException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		Socket &operator=(const Socket&);
		Socket(const Socket&);
		Socket();
		int _sockfd;
		std::string _host;
		std::string _port;
		std::vector<VirtualServer> _servers;
};

#endif
