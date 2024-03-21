
#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "Server.hpp"
#include "TcpListener.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>

#define SERV_PORT 8082
#define DEBUG_MODE 1

std::vector<std::string> split(const std::string &str, char delimiter);

#endif //WEBSERV_WEBSERV_HPP
