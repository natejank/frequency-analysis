#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#define MAX_UTILIZATION(size) size/2

struct table_s {
    char **keys;
    long *values;
    int size;
    int util;
};

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

int _table_index(int size, char *key) {
    return _hash_key(key) % size;
}


void _resize_table(struct table_s *t) {
    int new_size = t->size * 2;

    struct table_s *tmp = table_new(new_size);
    t->keys = realloc(t, sizeof(char*) * new_size);
    t->keys = realloc(t, sizeof(long) * new_size);
    t->size = new_size;

    for (int i = 0; i < tmp->util; i++) {
        char *key = tmp->keys[i];
        long value = table_get(t, key);
        table_insert(tmp, key, value);
    }

    t->size = tmp->size;
    memcpy(t->values, tmp->values, new_size);

    table_del(tmp);
}

struct table_s* table_new(int size) {
    struct table_s *t = malloc(sizeof(struct table_s));
    t->size = size;
    t->util = 0;
    t->keys = malloc(sizeof(char*) * size);
    t->values = malloc(sizeof(long) * size);
    return t;
}

void table_del(struct table_s *t) {
    for (int i = 0; i < t->util; i++) {
        free(t->keys[i]);
    }
    free(t->keys);
    free(t->values);
    free(t);
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

long table_get(struct table_s *t, char *key) {
    int index = _table_index(t->size, key);
    return t->values[index];
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

