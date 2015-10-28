#ifndef DNS_SENDER_H
#define DNS_SENDER_H

struct DnsQuerySender;
typedef struct DnsQuerySender DnsQuerySender, *DnsQuerySenderPtr;

DnsQuerySenderPtr dns_query_sender_create(void);
void dns_query_sender_delete(DnsQuerySenderPtr sender);

int dns_query_sender_connect_to_server(DnsQuerySenderPtr sender, const char *server, int port);
int dns_query_sender_send_dns_query(DnsQuerySenderPtr sender, const char *hostname);
int dns_query_sender_close_connection(DnsQuerySenderPtr sender);


#endif // DNS_SENDER_H
