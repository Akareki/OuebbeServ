/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:00:50 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/15 15:01:11 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP
#include "Location.hpp"
#include <exception>
#include <fstream>
#include <map>
#include <string>


class VirtualServer
{
	public:
		VirtualServer();
		VirtualServer(std::ifstream &);
		VirtualServer(const VirtualServer&);
		VirtualServer &operator=(const VirtualServer&);
		~VirtualServer();
		const std::string &getHost() const;
		const std::string &getPort() const;
		class VirtualServerException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		std::string _host;
		std::string _port;
		std::string _server_name;
		std::string _root;
		std::string _index;
		bool _autoindex;
		unsigned int _client_max_body_size;
		std::map<int, std::string> _error_pages;
		std::map<std::string, Location> _locations;
};

#endif
