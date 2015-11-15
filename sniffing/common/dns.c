#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dns.h"

int read_dns_header(char *buf, struct DNS_HEADER *dns_header)
{
    memcpy((char*)dns_header, buf, sizeof(struct DNS_HEADER));

    return sizeof(struct DNS_HEADER);
}

int write_dns_header(struct DNS_HEADER *dns_header, char *buf)
{
    memcpy(buf, (char*)dns_header, sizeof(struct DNS_HEADER));

    return sizeof(struct DNS_HEADER);
}

void convert_hostname_to_dnsname(unsigned char *dns, unsigned char *host)
{
    int lock = 0 , i;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
            *dns++ = i-lock;
            for(;lock<i;lock++)
            {
                *dns++=host[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}

void convert_dnsname_to_hostname (unsigned char *host, unsigned char *dns)
{
    int i, j, k;

    for (i = 0, k = 0; i < strlen((const char *)dns);) {
        int count = dns[i];
        ++i;
        for (j = i; j < i + count; ++j, ++k) {
            host[k] = dns[j];
        }
        host[k] = '.';
        ++k;
        i += count;
    }

    host[k - 1] = '\0';
}

u_char *read_hostname_from_dnsname(unsigned char *reader, unsigned char *buffer, int *count)
{
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;

    *count = 1;
    name = (unsigned char*)malloc(256);

    name[0]='\0';

    //read the names in 3www6google3com format
    while(*reader!=0)
    {
        if(*reader>=192)
        {
            offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
            reader = buffer + offset - 1;
            jumped = 1; //we have jumped to another location so counting wont go up!
        }
        else
        {
            name[p++]=*reader;
        }

        reader = reader+1;

        if(jumped==0)
        {
            *count = *count + 1; //if we havent jumped to another location then we can count up
        }
    }

    name[p]='\0'; //string complete
    if(jumped==1)
    {
        *count = *count + 1; //number of steps we actually moved forward in the packet
    }

    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='\0'; //remove the last dot
    return name;
}

void init_default_dns_header(struct DNS_HEADER *dns)
{
    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
}

void init_default_dns_question(struct QUESTION *question)
{
    question->qtype = htons(T_A); //type of the query , A , MX , CNAME , NS etc
    question->qclass = htons(1); //its internet
}

int read_dns_question(char *buf, struct QUESTION *question)
{
    memcpy((char *)question, buf, sizeof(struct QUESTION));

    return sizeof(struct QUESTION);
}

int write_dns_question(struct QUESTION *question, char *buf)
{
    memcpy(buf, (char *)question, sizeof(struct QUESTION));

    return sizeof(struct QUESTION);
}

