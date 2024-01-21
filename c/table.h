#ifndef _FREQ_TABLE_H
#define _FREQ_TABLE_H

#include <stdbool.h>

typedef struct table_s table;

typedef struct keys_s {
    char **keys;
    int count;
} t_keys;

table* table_new(void);
int table_size(table *t);
t_keys* table_keys(table *t);
void table_t_keys_del(t_keys *k);
long table_get(table *t, char *key);
void table_del(table *t);
void table_insert(table *t, char *key, long value);
bool table_contains(table *t, char *key);
void table_remove(table *t, char *key);
void table_print(table *t);

#endif
