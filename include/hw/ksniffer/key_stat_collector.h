#ifndef KEY_STAT_COLLECTOR_H
#define KEY_STAT_COLLECTOR_H

typedef struct KeyStatCollector KeyStatCollector, *KeyStatCollectorPtr;

KeyStatCollectorPtr key_stat_collector_create(void);
void key_stat_collector_delete(KeyStatCollectorPtr key_stat_collector);

void key_stat_collector_add_scancode(KeyStatCollectorPtr key_stat_collector, int scancode);
int key_stat_collector_get_scancode_count(KeyStatCollectorPtr key_stat_collector, int scancode);


#endif // KEY_STAT_COLLECTOR_H
