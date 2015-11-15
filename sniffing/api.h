#ifndef SNIFFING_API_H
#define SNIFFING_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

void sniffer_sniff(int scancode);
void sniffer_analyze_packet(const uint8_t *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif//SNIFFING_API_H
