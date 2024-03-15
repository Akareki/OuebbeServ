/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:16:44 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/15 11:42:22 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
#include <string>
#include <vector>

class Location
{
	public:
		Location(const std::string &);
		~Location();
	private:
		Location(const Location&);
		Location &operator=(const Location&);
		Location();
		std::string _root;
		std::string _index;
		std::string _redirect;
		bool _autoindex;
		std::vector<std::string> _allowed_methods;
};

#endif
