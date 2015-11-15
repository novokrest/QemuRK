#include "key_stat_collector.hpp"

void KeyStatCollector::AddScancode(int scancode)
{
    ++keyCounts_[scancode];
}

int KeyStatCollector::GetScancodeCount(int scancode) const
{
    return keyCounts_.at(scancode);
}

