#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#define MAX_UTILIZATION(size) size/2

struct key_s {
    char *key;
    int hash;
};

struct value_s {
    long value;
    struct value_s *next;
};


struct table_s {
    struct key_s **keys;
    struct value_s **values;
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
        char *key = tmp->keys[i]->key;
        long value = table_get(t, key);
        table_insert(tmp, key, value);
    }

    t->size = tmp->size;
    memcpy(t->values, tmp->values, new_size);

    table_del(tmp);
}

struct key_s* _new_key(char *k) {
    int size = strlen(k);
    struct key_s *key = malloc(sizeof(struct key_s));
    key->key = malloc(sizeof(char) * size);
    key->hash = _hash_key(k);

    return key;
}

struct value_s* _new_value(long v) {
    struct value_s *value = malloc(sizeof(struct value_s));
    value->value = v;
    value->next = NULL;

    return value;
}

void _del_key(struct key_s *k) {
    free(k->key);
    free(k);
}

void _del_value(struct value_s *v) {
    free(v);
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
        _del_key(t->keys[i]);
    }
    free(t->keys);
    free(t->values);
    free(t);
}

bool table_contains(table *t, char *key) {
    int hash = _hash_key(key);
    for (int i = 0; i < t->util; i++) {
        if (t->keys[i]->hash == hash) {
            return true;
        }
    }
    return false;
}

long table_get(struct table_s *t, char *key) {
    int index = _table_index(t->size, key);
    return t->values[index]->value;
}

void table_remove(table *t, char *key) {
    int hash = _hash_key(key);
    for (int i = 0; i < t->util; i++) {
        if (t->keys[i]->hash == hash) {
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
        printf("%s: %ld\n", t->keys[i]->key, table_get(t, t->keys[i]->key));
    }
    printf("}\n");
}

