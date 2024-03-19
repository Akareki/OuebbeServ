/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:39:11 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/19 13:41:16 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP
#include "defines.hpp"
#include <exception>
#include <map>
#include <string>
#include <vector>
class HTTPMessage
{
	public:
		HTTPMessage(const std::string &);
		HTTPMessage(const std::string &, const std::string &);
		HTTPMessage(const HTTPMessage&);
		~HTTPMessage();
		HTTPMessage &operator=(const HTTPMessage&);
		void addHeader(const std::string &, const std::string &);
		std::string getMessage();
		class HTTPMessageException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		HTTPMessage();
		std::string _http_version;
		std::string _method;
		std::string _target;
		std::string _status;
		std::map<std::string, std::vector<std::string> > _headers;
		std::string _body;
};

#endif
