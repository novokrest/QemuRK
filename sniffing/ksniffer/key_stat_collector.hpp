#ifndef KEY_STAT_COLLECTOR_H
#define KEY_STAT_COLLECTOR_H

#include "common/common.hpp"

class KeyStatCollector
{
    std::map<int, int> keyCounts_;

public:
    void AddScancode(int scancode);
    int GetScancodeCount(int scancode) const;
};

#endif // KEY_STAT_COLLECTOR_H
