#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "hw/ksniffer/dns_sender.h"
#include "hw/ksniffer/dns.h"
#include "hw/ksniffer/util.h"

#define INVALID_SOCKET -1

struct DnsQuerySender
{
    int sockfd;
    struct sockaddr_in saddr;
};

static int generate_dns_query(const char *hostname, char *buf);

DnsQuerySenderPtr dns_query_sender_create(void)
{
    DnsQuerySenderPtr sender = (DnsQuerySenderPtr)malloc(sizeof(DnsQuerySender));
    sender->sockfd = INVALID_SOCKET;
    sender->saddr = (struct sockaddr_in){0};

    return sender;
}

void dns_query_sender_delete(DnsQuerySenderPtr sender)
{
    free(sender);
}

int dns_query_sender_connect_to_server(DnsQuerySenderPtr sender, const char *server, int port)
{
    int sockfd;
    struct sockaddr_in saddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        print_error_msg("Fail to create udp socket");
        return -1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(server);

    sender->sockfd = sockfd;
    sender->saddr = saddr;

    return 0;
}

int dns_query_sender_send_dns_query(DnsQuerySenderPtr sender, const char *hostname)
{
    char query[DNS_QUERY_MAX_LENGTH];
    int length, sendcount;

    length = generate_dns_query(hostname, query);

    sendcount = sendto(sender->sockfd,
                       (char*)query, length,
                       0,
                       (struct sockaddr*)&sender->saddr, sizeof(sender->saddr));

    if (sendcount != length) {
        print_error_msg("Fail to send full dns query");
        return -1;
    }

    return 0;
}

int dns_query_sender_close_connection(DnsQuerySenderPtr sender)
{
    if (sender->sockfd == INVALID_SOCKET) {
        close(sender->sockfd);
        sender->sockfd = INVALID_SOCKET;
        sender->saddr = (struct sockaddr_in){0};
    }

    return 0;
}

static int generate_dns_query(const char *hostname, char *buf)
{
    int bufpos = 0;

    struct DNS_HEADER dnsHeader;
    init_default_dns_header(&dnsHeader);
    bufpos += write_dns_header(&dnsHeader, buf);

    unsigned char *qname = (unsigned char *)&buf[bufpos];
    convert_hostname_to_dnsname(qname, (unsigned char *)hostname);
    bufpos += strlen((const char *)qname) + 1;

    struct QUESTION question;
    init_default_dns_question(&question);
    bufpos += write_dns_question(&question, buf + bufpos);

    return bufpos;
}

