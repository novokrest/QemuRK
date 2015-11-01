#include <stdlib.h>

#include "hw/ksniffer/key_stat_collector.h"

struct KeyStatCollector
{
    int *scancode_counts;
};

KeyStatCollectorPtr key_stat_collector_create(void)
{
    KeyStatCollectorPtr key_stat_collector;
    int *scancode_counts;

    key_stat_collector = (KeyStatCollectorPtr)malloc(sizeof(KeyStatCollector));
    scancode_counts = (int *)calloc(255, sizeof(int));
    key_stat_collector->scancode_counts = scancode_counts;

    return key_stat_collector;
}

void key_stat_collector_delete(KeyStatCollectorPtr key_stat_collector)
{
    free(key_stat_collector->scancode_counts);
    free(key_stat_collector);
}

void key_stat_collector_add_scancode(KeyStatCollectorPtr key_stat_collector, int scancode)
{
    ++key_stat_collector->scancode_counts[scancode];
}

int key_stat_collector_get_scancode_count(KeyStatCollectorPtr key_stat_collector, int scancode)
{
    return key_stat_collector->scancode_counts[scancode];
}
