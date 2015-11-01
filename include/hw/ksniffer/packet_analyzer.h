#ifndef PACKET_ANALYZER_H
#define PACKET_ANALYZER_H

#include <stdlib.h>
#include "hw/ksniffer/common.h"

typedef struct PacketInfo
{
    int is_secret;
    char *server_ip;
    int port;
} PacketInfo, *PacketInfoPtr;

PacketInfoPtr packet_info_create(void);
void packet_info_delete(PacketInfoPtr packet_info);

struct PacketAnalyzer;
typedef struct PacketAnalyzer PacketAnalyzer, *PacketAnalyzerPtr;

PacketAnalyzerPtr packet_analyzer_create(void);
void packet_analyzer_delete(PacketAnalyzerPtr packet_analyzer);
PacketInfoPtr packet_analyzer_analyze_packet(PacketAnalyzerPtr packet_analyzer, const char *buf, size_t size);




#endif // PACKET_ANALYZER_H
