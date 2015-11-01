#ifndef KSNIFFER_H
#define KSNIFFER_H

#include <stdint.h>

typedef struct KeySniffer KeySniffer, *KeySnifferPtr;

KeySnifferPtr key_sniffer_get_instance(void);

void key_sniffer_turn_on(KeySnifferPtr ksniffer);
void key_sniffer_turn_off(KeySnifferPtr ksniffer);

void key_sniffer_sniff(KeySnifferPtr sniffer, int scancode);
void key_sniffer_analyze_packet(KeySnifferPtr ksniffer, const uint8_t *buf, size_t size);




#endif //KSNIFFER_H
