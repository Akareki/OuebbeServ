/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:28:02 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/19 13:57:30 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/VirtualServer.hpp"
#include "../includes/WebServ.hpp"
#include "../includes/HTTPMessage.hpp"
#include <fstream>
#include <iostream>

int	main(int argc, char **argv)
{
	int i = 1;
	HTTPMessage http("404 not found", "this is an http body");

	while (i < argc - 1)
	{
		http.addHeader(argv[i], argv[i + 1]);
		i += 2;
	}
	std::cout << http.getMessage() << std::endl;
}
