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
#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "webh.hpp"

class HTTPMessage
{
	public:
		HTTPMessage();
		HTTPMessage(const std::string &request);
		HTTPMessage(const std::string &status, const std::string &response_body);
		HTTPMessage(const HTTPMessage&);
		~HTTPMessage();
		HTTPMessage &operator=(const HTTPMessage&);
		void addHeader(const std::string &, const std::string &);
		std::string getMessage() const;
		const std::string &getMethod() const;
		const std::string &getPath() const;
		const std::string &getBody() const;
		const std::map<std::string, std::vector<std::string> > &getHeaders() const;
		void setStatus(const std::string &status);
		void setBody(const std::string &body);
		class HTTPMessageException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		std::string _http_version;
		std::string _method;
		std::string _path;
		std::string _status;
		std::map<std::string, std::vector<std::string> > _headers;
		std::string _body;
};

#endif
