#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common/dns_ex.hpp"
#include "dns_sender.hpp"

DnsQuerySender::DnsQuerySender()
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd_ == -1) {
        std::cout << "Fail to open socket for sending dns queries" << std::endl;
    }
}

void DnsQuerySender::SetServer(struct sockaddr_in server)
{
    server_ = server;
}

int DnsQuerySender::SendDnsQuery(const std::string &hostname)
{
    char buf[DNS_QUERY_MAX_LENGTH];
    DnsQuery query(hostname);

    int length = query.Serialize(buf, sizeof(buf));
    int count = sendto(sockfd_, buf, length, 0, (struct sockaddr *)&server_, sizeof(server_));
    if (count != length) {
        std::cout << "Fail to send dns query" << std::endl;
        return -1;
    }
    return 0;
}
