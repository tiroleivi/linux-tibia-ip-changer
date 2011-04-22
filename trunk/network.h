#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define GOOGLECODE "code.google.com"
#define ACTUALVERSION "1.6"
#define CODENAME "eta"

class Network
{
 public:
    std::string getIpCangerFromWebSite();
};

#endif // NETWORK_H
