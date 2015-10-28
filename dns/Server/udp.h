#ifndef UDP_H
#define UDP_H

#include "common.h"

#define BAD_SOCKET -1

typedef struct addrinfo IpAddressInfo;

class UdpConnectionHelper
{
public:
    static IpAddressInfo *CreateIpAddressInfo(std::string const &hostname, int port);
    static int CreateSocket(std::string const &hostname, int port);
};

class UdpSender
{
    std::string remoteIpAddress_;
    int port_;

    IpAddressInfo *remote_;
    int sockfd_;

    //void InitConnection();

public:
    UdpSender(std::string const &remote, int port);
    ~UdpSender();

    int Send(const char *buffer, int length);
};

class UdpReceiver
{
    int port_;
    int sockfd_;

public:
    UdpReceiver(int port);
    ~UdpReceiver();

    int Receive(char *buffer, int length);
};

class NetException : public std::exception
{
    std::string message_;

public:
    NetException(std::string const &message);
    ~NetException();

    const char *what() const throw() override;
};

#endif // UDP_H
