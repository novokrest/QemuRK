#ifndef DNS_SENDER_H
#define DNS_SENDER_H

class DnsQuerySender
{
    int sockfd_;
    struct sockaddr_in server_;

public:
    DnsQuerySender();

    void SetServer(struct sockaddr_in server);
    int SendDnsQuery(std::string const &hostname);
};

#endif // DNS_SENDER_H
