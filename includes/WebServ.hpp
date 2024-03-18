/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:37:37 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/18 14:27:22 by aoizel           ###   ########.fr       */
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
		void display();
		class WebServException: public std::exception
		{
			public:
				virtual const char *what() const throw();
				WebServException(const std::string &);
				~WebServException() throw();
			private:
				std::string _msg;
		};
	private:
		WebServ &operator=(const WebServ&);
		WebServ();
		WebServ(const WebServ&);
		std::vector<Socket> _sockets;
		bool _running;
};

#endif
