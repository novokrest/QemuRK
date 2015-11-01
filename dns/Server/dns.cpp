#include "dns.hpp"
#include "dns_util.h"

static std::string ConvertToDnsName(std::string const &hostname)
{
    std::string dnsName;

    for (size_t pos = 0; pos < hostname.length();) {
        size_t nextpos = hostname.find('.', pos);
        if (nextpos == std::string::npos) {
            nextpos = hostname.length();
        }
        size_t len = nextpos - pos;
        dnsName += std::to_string(len);
        dnsName += hostname.substr(pos, len);
        pos = nextpos + 1;
    }

    return dnsName;
}

static std::string ConvertToHostname(std::string const &dnsName)
{
    std::string hostname;
    std::stringstream ss(dnsName);

    int count;
    while (ss >> count) {
        for (int i = 0; i < count; ++i) {
            hostname += ss.get();
        }
        hostname += ".";
    }

    return hostname.substr(0, hostname.length() - 1);
}

DnsQuery::DnsQuery(const std::string &hostname)
    : hostname_(hostname)
{
}

std::string DnsQuery::HostName() const
{
    return hostname_;
}

int DnsQuery::Serialize(char *buf, int length) const
{
    int count = 0;

    struct DNS_HEADER dnsHeader;
    init_default_dns_header(&dnsHeader);
    count += write_dns_header(&dnsHeader, buf);

    unsigned char *qname = (unsigned char *)&buf[count];
    convert_hostname_to_dnsname(qname, (unsigned char*) hostname_.c_str());
    count += strlen((const char *)qname) + 1;

    struct QUESTION question;
    init_default_dns_question(&question);
    count += write_dns_question(&question, buf + count);

    return count;
}

DnsQuery DnsQuery::Deserialize(char *buf, int length)
{
    int bufpos = 0;

    struct DNS_HEADER dnsHeader;
    bufpos += read_dns_header(buf, &dnsHeader);

    char host[256];
    convert_dnsname_to_hostname((unsigned char *)host, (unsigned char *)&buf[bufpos]);

    return DnsQuery(host);
}

/****************************/
/*DnsQuerySender*/
/****************************/

DnsQuerySender::DnsQuerySender(const std::string &remote, int port)
    : sender_(new UdpSender(remote, port))
{

}

DnsQuerySender::~DnsQuerySender()
{
    delete sender_;
}

void DnsQuerySender::SendQuery(const DnsQuery &query)
{
    char buffer[DNS_QUERY_MAX_LENGTH];
    int count = query.Serialize(buffer, sizeof(buffer));
    sender_->Send(buffer, count);
}

/****************************/
/*DnsQueryReceiver*/
/****************************/


DnsQueryReceiver::DnsQueryReceiver(std::string const &ip, int port)
    : receiver_(new UdpReceiver(ip, port))
{
}

DnsQueryReceiver::~DnsQueryReceiver()
{
    delete receiver_;
}

DnsQuery DnsQueryReceiver::ReceiveQuery()
{
    char buffer[DNS_QUERY_MAX_LENGTH];

    int count = receiver_->Receive(buffer, sizeof(buffer));
    DnsQuery dnsQuery = DnsQuery::Deserialize(buffer, count);

    return dnsQuery;
}

/****************************/
/*DnsNameResolver*/
/****************************/

DnsNameResolver::DnsNameResolver(std::string const &server)
    : DnsNameResolver(server, DEFAULT_DNS_PORT)
{

}

DnsNameResolver::DnsNameResolver(std::string const &server, int port)
    : server_(server), port_(port)
{

}

DnsAnswer DnsNameResolver::Resolve(std::string const &hostname)
{
    int sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(53);
    serverAddr.sin_addr.s_addr = inet_addr(server_.c_str());

    char buf[DNS_QUERY_MAX_LENGTH];
    int bufpos = 0;

    struct DNS_HEADER dnsHeader;
    init_default_dns_header(&dnsHeader);
    bufpos += write_dns_header(&dnsHeader, buf);

    unsigned char *qname = (unsigned char *)&buf[bufpos];
    convert_hostname_to_dnsname(qname, (unsigned char *)hostname.c_str());
    bufpos += strlen((const char *)qname) + 1;

    struct QUESTION question;
    init_default_dns_question(&question);
    bufpos += write_dns_question(&question, buf + bufpos);

    int sendCount = sendto(sockfd,
                           (char*)buf, bufpos,
                           0,
                           (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    struct sockaddr_in dest;
    socklen_t destlen = sizeof(dest);

    int receiveCount = recvfrom(sockfd,
                                (char*)buf, sizeof(buf),
                                0,
                                (struct sockaddr*)&dest, &destlen);

    DnsAnswer dnsAnswer = DnsAnswer::Deserialize(buf, receiveCount);

    return dnsAnswer;
}

/****************************/
/*DnsAnswer*/
/****************************/

std::string DnsAnswer::ToString() const
{
    std::stringstream ss;

    for (std::string const &ipAddress : IpAddresses) {
        ss << "IpAddress: " << ipAddress << std::endl;
    }

    return ss.str();
}

DnsAnswer DnsAnswer::Deserialize(char *buf, int length)
{
    DnsAnswer answer;
    unsigned char *reader = (unsigned char *)buf;

    struct DNS_HEADER dns;
    struct QUESTION dnsQuestion;
    char *qname;

    reader += read_dns_header((char*)reader, &dns);
    qname = (char*)reader;
    reader += strlen((const char *)qname) + 1;
    reader += read_dns_question((char*)reader, &dnsQuestion);

    int stop = 0;

    struct RES_RECORD answers[20],auth[20],addit[20];

    for(int i=0;i<ntohs(dns.ans_count);i++)
    {
        answers[i].name = read_hostname_from_dnsname(reader, (unsigned char*)buf, &stop);
        reader = reader + stop;

        answers[i].resource = (struct R_DATA*)(reader);
        reader = reader + sizeof(struct R_DATA);

        if(ntohs(answers[i].resource->type) == 1) //if its an ipv4 address
        {
            answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->data_len));

            for(int j=0 ; j<ntohs(answers[i].resource->data_len) ; j++)
            {
                answers[i].rdata[j]=reader[j];
            }

            answers[i].rdata[ntohs(answers[i].resource->data_len)] = '\0';

            reader = reader + ntohs(answers[i].resource->data_len);
        }
        else
        {
            answers[i].rdata = read_hostname_from_dnsname(reader,(unsigned char*)buf,&stop);
            reader = reader + stop;
        }

        long *p = (long*)answers[i].rdata;
        struct sockaddr_in a;
        a.sin_addr.s_addr = *p;

        answer.IpAddresses.push_back(std::string(inet_ntoa(a.sin_addr)));
    }

    //read authorities
    for(int i=0;i<ntohs(dns.auth_count);i++)
    {
        auth[i].name=read_hostname_from_dnsname(reader,(unsigned char*)buf,&stop);
        reader+=stop;

        auth[i].resource=(struct R_DATA*)(reader);
        reader+=sizeof(struct R_DATA);

        auth[i].rdata=read_hostname_from_dnsname(reader,(unsigned char*)buf,&stop);
        reader+=stop;
    }

    //read additional
    for(int i=0;i<ntohs(dns.add_count);i++)
    {
        addit[i].name=read_hostname_from_dnsname(reader,(unsigned char*)buf,&stop);
        reader+=stop;

        addit[i].resource=(struct R_DATA*)(reader);
        reader+=sizeof(struct R_DATA);

        if(ntohs(addit[i].resource->type)==1)
        {
            addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->data_len));
            for(int j=0;j<ntohs(addit[i].resource->data_len);j++)
            addit[i].rdata[j]=reader[j];

            addit[i].rdata[ntohs(addit[i].resource->data_len)]='\0';
            reader+=ntohs(addit[i].resource->data_len);
        }
        else
        {
            addit[i].rdata=read_hostname_from_dnsname(reader,(unsigned char*)buf,&stop);
            reader+=stop;
        }
    }

    return answer;
}
