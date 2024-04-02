/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:42:16 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 13:40:25 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "HTTPMessage.hpp"
#include <string>
class Client
{
	public:
		Client();
		Client(int);
		Client(const Client&);
		~Client();
		Client &operator=(const Client&);
		int getFd() const;
		const HTTPMessage &getRequest() const;
		bool isReady();
		int readRequest();
	private:
		bool _ready;
		int _connfd;
		HTTPMessage _request;
		
};

#endif
