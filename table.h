#ifndef _FREQ_TABLE_H
#define _FREQ_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_TABLE_SIZE 20
#define MAX_UTILIZATION(size) size/2

struct table_s {
    char **keys;
    long *values;
    int size;
    int util;
};

// TODO wrap this such that we can keep pointers between table resizes
typedef struct table_s table;

int _hash_key(char *key) {
    // http://www.cse.yorku.ca/~oz/hash.html
    // djb2

    int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

table* table_new(int size) {
    table *t = malloc(sizeof(struct table_s));
    t->size = size;
    t->util = 0;
    t->keys = malloc(sizeof(char*) * size);
    t->values = malloc(sizeof(long) * size);
    return t;
}

void table_del(table *t) {
    for (int i = 0; i < t->util; i++) {
        free(t->keys[i]);
    }
    free(t->keys);
    free(t->values);
    free(t);
}

table* table_insert(table *t, char *key, long value);

int _table_index(int size, char *key) {
    return _hash_key(key) % size;
}


long table_get(table *t, char *key) {
    int index = _table_index(t->size, key);
    return t->values[index];
}


table* _resize_table(table *t) {
    int new_size = t->size * 2;
    table *new = table_new(new_size);
    new->size = new_size;
    new->util = t->util;

    memcpy(new->keys, t->keys, t->util * sizeof(char*));
    for (int i = 0; i < new->util; i++) {
        char *key = new->keys[i];
        long value = table_get(t, key);
        new = table_insert(new, key, value);
    }

    table_del(t);
    return new;
}


bool table_contains(table *t, char *key) {
    // TODO we should probably hash keys when we insert them for easier comparison
    // key: {string, hash}
    for (int i = 0; i < t->util; i++) {
        if (strcmp(t->keys[i], key) == 0) {
            return true;
        }
    }
    return false;
}

void table_remove(table *t, char *key) {
    for (int i = 0; i < t->util; i++) {
        if (strcmp(t->keys[i], key) == 0) {
            free(t->keys[i]);
            // TODO if there is a bug, it's probably here.
            memmove(t->keys + i, t->keys + i + 1, i - t->util);
            t->util = t->util - 1;
            return;
        }
    }
}

void table_print(table *t) {
    printf("{\n");
    for (int i = 0; i < t->util; i++) {
        printf("%s: %ld\n", t->keys[i], table_get(t, t->keys[i]));
    }
    printf("}\n");
}
#endif
