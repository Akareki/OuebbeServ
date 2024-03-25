/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:16:44 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/21 09:00:58 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
#define WS " \t\r\n"
#define CRLF "\r\n"
#define OPTNB 10
extern unsigned int line_nb;
#include <fstream>
#include <string>
#include <vector>
#include "HTTPMessage.hpp"

class HTTPMessage;

class Location
{
public:
	Location();
	Location(std::ifstream &);
	Location(const Location &);
	Location &operator=(const Location&);
	~Location();
	const std::string &getRoot() const;
	const std::string &getIndex() const;
	const std::string &getRedirect() const;
	bool getAutoindex() const;
	void setOpt(const std::string &, const std::string &);
	void setRoot(const std::string &);
	void setIndex(const std::string &);
	void setRedirect(const std::string &);
	void setAutoindex(const std::string &);
	void setAllowedMethods(const std::string &);
	class LocationException: public std::exception
	{
	public:
		virtual const char *what() const throw();
		LocationException(const std::string &);
		LocationException(const std::string &, int);
		~LocationException() throw();
	private:
		std::string _msg;
	};
	const static std::string optNames[OPTNB];
	static void (Location::*optSetters[OPTNB])(const std::string &);
	void answer_request(HTTPMessage &http_request, int connfd);
	std::string			get_full_path(const HTTPMessage &http_request, bool &isindexadded);
	void display() const;
private:
	std::string _root;
	std::string _index;
	std::string _redirect;
	bool _autoindex;
	unsigned int  _client_max_body_size;
	std::vector<std::string> _allowed_methods;
};

#endif
