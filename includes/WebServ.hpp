/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:37:37 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/15 11:49:45 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP
#include "Socket.hpp"
#include <exception>
#include <string>
#include <vector>

class WebServ
{
	public:
		WebServ(const std::string &);
		~WebServ();
		class WebServException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		WebServ &operator=(const WebServ&);
		WebServ();
		WebServ(const WebServ&);
		std::vector<Socket> _sockets;
		bool _running;
};

#endif
