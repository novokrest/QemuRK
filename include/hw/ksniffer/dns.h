#ifndef DNS_H
#define DNS_H

#define DNS_QUERY_MAX_LENGTH 65535

#define T_A      1  // Ipv4 address
#define T_NS     2  // Nameserver
#define T_CNAME  5  // canonical name
#define T_SOA    6  // start of authority zone
#define T_PTR   12  // domain name pointer
#define T_MX    15  // Mail server

struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};

struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};


#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};

typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;


void init_default_dns_header(struct DNS_HEADER *dns);
int read_dns_header(char *buf, struct DNS_HEADER *dns_header);
int write_dns_header(struct DNS_HEADER *dns_header, char *buf);

void convert_hostname_to_dnsname (unsigned char *dns, unsigned char *host);
void convert_dnsname_to_hostname (unsigned char *host, unsigned char *dns);
u_char *read_hostname_from_dnsname(unsigned char *reader, unsigned char *buffer, int *count);

void init_default_dns_question(struct QUESTION *question);
int read_dns_question(char *buf, struct QUESTION *question);
int write_dns_question(struct QUESTION *question, char *buf);

#endif // DNS_H
