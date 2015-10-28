#ifndef KSNIFFER_H
#define KSNIFFER_H

typedef struct KeySniffer KeySniffer, *KeySnifferPtr;

KeySnifferPtr get_key_sniffer(void);

void turn_on(KeySnifferPtr sniffer);
void turn_off(KeySnifferPtr sniffer);
void sniff(KeySnifferPtr sniffer, int scancode);




#endif //KSNIFFER_H
