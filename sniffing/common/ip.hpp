#ifndef IP_H
#define IP_H

#include "common/common.hpp"

struct IpAddress
{
    char bytes[4];
};

class IpAddressManager
{
public:
    static IpAddress GetLocalIpAddress();
};

#endif // IP_H
