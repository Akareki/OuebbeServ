/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:28:02 by aoizel            #+#    #+#             */
/*   Updated: 2024/03/18 14:26:50 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/VirtualServer.hpp"
#include "../includes/WebServ.hpp"
#include <fstream>
#include <iostream>

int	main(int argc, char **argv)
{
	std::ifstream config;

	if (argc != 2)
		return (1);
	config.open(argv[1]);
	try
	{
		WebServ ws(argv[1]);
		ws.display();
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
