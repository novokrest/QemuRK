#include "udp.h"

IpAddressInfo *UdpConnectionHelper::CreateIpAddressInfo(const std::string &hostname, int port)
{
    struct addrinfo *res = 0;
    struct addrinfo hints;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

    std::string portname = std::to_string(port);
    int err = getaddrinfo(hostname.c_str(), portname.c_str(), &hints, &res);
    if (err != 0) {
        std::cout << "Failed to resolve local socket address" << std::endl;
        return NULL;
    }

    return res;
}

int UdpConnectionHelper::CreateSocket(const std::string &hostname, int port)
{
    struct addrinfo *saddr = CreateIpAddressInfo(hostname, port);
    if (!saddr) {
        return BAD_SOCKET;
    }

    int sockfd = socket(saddr->ai_family, saddr->ai_socktype, saddr->ai_protocol);
    if (sockfd == -1) {
        std::cout << "Failed to create socket: " << strerror(errno) << std::endl;
        return BAD_SOCKET;
    }

    return sockfd;
}

UdpSender::UdpSender(const std::string &remote, int port)
{
    remote_ = UdpConnectionHelper::CreateIpAddressInfo(remote, port);
    sockfd_ = socket(remote_->ai_family, remote_->ai_socktype, remote_->ai_protocol);
    if (sockfd_ == -1) {
        throw NetException(strerror(errno));
    }
}

UdpSender::~UdpSender()
{
    if (sockfd_ != BAD_SOCKET) close(sockfd_);
    if (remote_) freeaddrinfo(remote_);
}

int UdpSender::Send(std::string const &msg)
{
    return Send(msg.c_str(), msg.length());
}

int UdpSender::Send(const char *buffer, int length)
{
    ssize_t count = sendto (sockfd_, buffer, length, 0, remote_->ai_addr, remote_->ai_addrlen);
    if (count == -1) {
        throw NetException(strerror(errno));
    }
    return count;
}

UdpReceiver::UdpReceiver(std::string const &ip, int port)
{
    IpAddressInfo *saddr = UdpConnectionHelper::CreateIpAddressInfo(ip, port);

    sockfd_ = socket(saddr->ai_family, saddr->ai_socktype, saddr->ai_protocol);
    if (sockfd_ == -1) {
        throw NetException(strerror(errno));
    }

    if (bind(sockfd_, saddr->ai_addr, saddr->ai_addrlen) == -1) {
        throw NetException(strerror(errno));
    }

    freeaddrinfo(saddr);
}

UdpReceiver::~UdpReceiver()
{
    if (sockfd_ != BAD_SOCKET) close(sockfd_);
}

int UdpReceiver::Receive(char *buffer, int length)
{
    struct sockaddr_storage srcaddr;
    socklen_t srcaddr_len = sizeof(srcaddr);

    ssize_t count = recvfrom(sockfd_, buffer, length, 0, (struct sockaddr*)&srcaddr, &srcaddr_len);
    return count;
}

NetException::NetException(const std::string &message)
    : message_(message)
{

}

NetException::~NetException()
{

}

const char *NetException::what() const throw()
{
    return message_.c_str();
}

//LocalServer::LocalServer(int port)
//    : port_(port)
//{
//}

//int LocalServer::port() const
//{
//    return port_;
//}

//LinuxServer::LinuxServer(int port)
//    : LocalServer(port), isListened_(false)
//{

//}

//void LinuxServer::startListen()
//{
//    const char *hostname = 0;
//    const char *portname = "13131";
//    struct addrinfo *saddr = 0;
//    int sockfd;
//    int err;

//    struct addrinfo hints;
//    memset(&hints,0,sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_DGRAM;
//    hints.ai_protocol = 0;
//    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

//    err = getaddrinfo(hostname, portname, &hints, &saddr);
//    if (err != 0) {
//        std::cout << "Failed to resolve local socket address" << std::endl;
//        return;
//    }

//    sockfd = socket(saddr->ai_family, saddr->ai_socktype, saddr->ai_protocol);
//    if (sockfd == -1) {
//        std::cout << "Failed to create socket: " << strerror(errno) << std::endl;
//        return;
//    }

//    err = bind(sockfd, saddr->ai_addr, saddr->ai_addrlen);
//    if (err == -1) {
//        std::cout << "Failed to bind socket: " << strerror(errno) << std::endl;
//        return;
//    }

//    freeaddrinfo(saddr);

//    char buffer[549];
//    struct sockaddr_storage srcaddr;
//    socklen_t srcaddr_len = sizeof(srcaddr);
//    isListened_ = true;

//    std::cout << "Start listen..." << std::endl;
//    while (isListened_) {
//        ssize_t count = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&srcaddr, &srcaddr_len);
//        if (count == -1) {
//            std::cout << "Failed to receive datagram" << std::endl;
//            continue;
//        }
//        if (count == sizeof(buffer)){
//            std::cout  << "Received datagram is to large. Truncated " << std::endl;
//            count -= 1;
//            buffer[count] = '\0';
//        }
//        handle(buffer, count);
//    }

//    close(sockfd);
//}

//void LinuxServer::handle(char *buffer, int count)
//{
//    std::cout << "Received buffer content [" << count << " byte(s)]:" << std::endl
//               << std::string(buffer)
//               << std::endl;
//}

//void LinuxServer::stopListen()
//{
//    isListened_ = false;
//}

//void Client::send(const std::string &message)
//{
//    const char* hostname="localhost";
//    const char* portname="13131";
//    struct addrinfo hints;
//    memset(&hints,0,sizeof(hints));
//    hints.ai_family=AF_UNSPEC;
//    hints.ai_socktype=SOCK_DGRAM;
//    hints.ai_protocol=0;
//    hints.ai_flags=AI_ADDRCONFIG;
//    struct addrinfo* res=0;
//    int err=getaddrinfo(hostname,portname,&hints,&res);
//    if (err!=0) {
//        std::cout << "Failed to resolve remote socket address" << std::endl;
//        return;
//    }

//    int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
//    if (fd==-1) {
//        std::cout << strerror(errno) << std::endl;
//        return;
//    }

//    if (sendto(fd, message.c_str(), message.length(), 0, res->ai_addr, res->ai_addrlen)==-1) {
//        std::cout << strerror(errno) << std::endl;
//        return;
//    }

//    std::cout << "Sended" << std::endl;
//}
