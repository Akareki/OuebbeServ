/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:42:16 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 10:30:50 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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
		const std::string &getRequest() const;
		bool isReady();
		void readRequest();
	private:
		bool _ready;
		int _connfd;
		std::string _request;
};

#endif
