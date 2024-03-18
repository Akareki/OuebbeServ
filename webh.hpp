
#ifndef WEBSERV_WEBH_HPP
#define WEBSERV_WEBH_HPP

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "VirtualServer.hpp"
#include "TcpListener.hpp"
#include "WebServ.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

#define DEBUG_MODE 1

std::vector<std::string> split(const std::string &str, char delimiter);

#endif //WEBSERV_WEBSERV_HPP
