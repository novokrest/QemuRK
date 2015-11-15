#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <thread>
#include <exception>

#include <cstdint>

#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#include <tomcrypt.h>

typedef std::vector<std::string> Strings;

#endif // COMMON_H
