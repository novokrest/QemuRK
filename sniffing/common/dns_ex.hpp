#ifndef DNS_HPP
#define DNS_HPP

#include "dns.h"
#include "udp.hpp"

#define DEFAULT_DNS_PORT 53
#define DNS_QUERY_MAX_LENGTH 65536

class DnsQuery
{
    std::string hostname_;

    DnsQuery();

    static void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host);

public:
    static DnsQuery Create(std::string const &hostname);
    DnsQuery(std::string const &hostname);

    std::string HostName() const;

    int Serialize(char *buf, int length) const;
    static DnsQuery Deserialize(char *buf, int length);
};

class DnsQueryReceiver
{
    UdpReceiver *receiver_;

public:
    DnsQueryReceiver(int port);
    DnsQueryReceiver(std::string const &ip, int port);
    ~DnsQueryReceiver();

    DnsQuery ReceiveQuery();
};

struct DnsAnswer
{
    Strings IpAddresses;
    Strings AuthoritativeServers;
    Strings AdditionalRRs;

    std::string ToString() const;

    static DnsAnswer Deserialize(char *buffer, int count);
};

class DnsNameResolver
{
    std::string server_;
    int port_;

public:
    DnsNameResolver(std::string const &dnsServer);
    DnsNameResolver(std::string const &dnsServer, int port);

    DnsAnswer Resolve(std::string const &hostname);
};

#endif // DNS_HPP
