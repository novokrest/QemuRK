#include "api.h"
#include "ksniffer/key_sniffer.hpp"

void sniffer_sniff(int scancode)
{
    KeySniffer *ksniffer = KeySniffer::Instance();
    ksniffer->Sniff(scancode);
}

void sniffer_analyze_packet(const uint8_t *buf, size_t size)
{
    KeySniffer *ksniffer = KeySniffer::Instance();
    ksniffer->AnalyzePacket((const char *)buf, size);
}
