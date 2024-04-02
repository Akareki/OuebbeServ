/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:39:11 by aoizel            #+#    #+#             */
/*   Updated: 2024/04/02 10:11:17 by aoizel           ###   ########.fr       */
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
		HTTPMessage();
		HTTPMessage(const std::string &);
		HTTPMessage(const std::string &, const std::string &);
		HTTPMessage(const HTTPMessage&);
		~HTTPMessage();
		HTTPMessage &operator=(const HTTPMessage&);
		const std::string &getMethod() const;
		const std::string &getBody() const;
		const std::string &getPath() const;
		const std::map<std::string, std::vector<std::string> > &getHeaders() const;
		void setStatus(const std::string &);
		void setBody(const std::string &);
		void addHeader(const std::string &, const std::string &);
		std::string getMessage() const;
		std::string getFileName() const; // relevant only for multipart/form-data contenttype
		class HTTPMessageException: public std::exception
		{
			virtual const char *what() const throw();
		};
	private:
		std::string _http_version;
		std::string _method;
		std::string _path;
		std::string _status;
		std::string _file_header; //relevant only for multipart/form-data contenttype
		std::map<std::string, std::vector<std::string> > _headers;
		std::string _body;
};

#endif
