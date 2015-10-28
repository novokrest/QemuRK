#ifndef DNSUTIL_H
#define DNSUTIL_H

#include "dns.h"

void init_default_dns_header(struct DNS_HEADER *dns);
int read_dns_header(char *buf, struct DNS_HEADER *dns_header);
int write_dns_header(struct DNS_HEADER *dns_header, char *buf);

void convert_hostname_to_dnsname (unsigned char *dns, unsigned char *host);
void convert_dnsname_to_hostname (unsigned char *host, unsigned char *dns);
u_char *read_hostname_from_dnsname(unsigned char *reader, unsigned char *buffer, int *count);

void init_default_dns_question(struct QUESTION *question);
int read_dns_question(char *buf, struct QUESTION *question);
int write_dns_question(struct QUESTION *question, char *buf);
void init_default_dns_header(struct DNS_HEADER *dns);
int read_dns_header(char *buf, struct DNS_HEADER *dns_header);
int write_dns_header(struct DNS_HEADER *dns_header, char *buf);

void convert_hostname_to_dnsname (unsigned char *dns, unsigned char *host);
void convert_dnsname_to_hostname (unsigned char *host, unsigned char *dns);
u_char *read_hostname_from_dnsname(unsigned char *reader, unsigned char *buffer, int *count);

void init_default_dns_question(struct QUESTION *question);
int read_dns_question(char *buf, struct QUESTION *question);
int write_dns_question(struct QUESTION *question, char *buf);

#endif // DNSUTIL_H
