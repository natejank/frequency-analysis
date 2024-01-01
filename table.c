#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

#define DEFAULT_TABLE_SIZE 20
#define MAX_UTILIZATION(size) size/2

struct key_s {
    char *key;
    int hash;
};

struct value_s {
    long value;
    int hash;
    struct value_s *next;
};

struct table_s {
    struct key_s **keys;
    struct value_s **values;
    int size;
    int util;
};

int _hash_key(char *key) {
    // http://www.cse.yorku.ca/~oz/hash.html - djb2
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

struct key_s* _key_new(char *k) {
    int size = strlen(k) + 1;
    struct key_s *key = malloc(sizeof(struct key_s));
    key->key = malloc(sizeof(char) * size);
    strncpy(key->key, k, size);
    key->hash = _hash_key(k);

    return key;
}


void _key_del(struct key_s *k) {
    free(k->key);
    free(k);
}

struct value_s* _value_new(long v, int hash) {
    struct value_s *value = malloc(sizeof(struct value_s));
    value->value = v;
    value->hash = hash;
    value->next = NULL;

    return value;
}

void _value_del(struct value_s *v) {
    free(v);
}

struct table_s* _table_new(int size) {
    struct table_s *t = malloc(sizeof(struct table_s));
    t->size = size;
    t->util = 0;
    t->keys = malloc(sizeof(struct key_s*) * size);
    t->values = calloc(sizeof(struct value_s*), size);
    return t;
}

int _table_index(int size, int hash) {
    return hash % size;
}

struct value_s* _table_get_v(struct table_s *t, int hash) {
    int index = _table_index(t->size, hash);
    struct value_s *ev = t->values[index];
    while (ev != NULL && ev->hash != hash) {
        ev = ev->next;
    }
    return ev;
}


void _table_resize(struct table_s *t) {
    int new_size = t->size * 2;

    struct table_s *tmp = _table_new(new_size);

    for (int i = 0; i < t->util; i++) {
        char *key = t->keys[i]->key;
        long value = table_get(t, key);
        table_insert(tmp, key, value);
    }

    // a little switcharoo to make freeing easier
    struct key_s **old_keys = t->keys;
    struct value_s **old_vals = t->values;
    t->keys = tmp->keys;
    t->values = tmp->values;
    tmp->size = t->size;
    t->size = new_size;
    tmp->keys = old_keys;
    tmp->values = old_vals;
    table_del(tmp);
}

struct table_s* table_new(void) {
    return _table_new(DEFAULT_TABLE_SIZE);
}

int table_size(struct table_s *t) {
    return t->util;
}

long table_get(struct table_s *t, char *key) {
    int hash = _hash_key(key);
    struct value_s *v = _table_get_v(t, hash);
    if (v == NULL) {
        return -1;
    }
    return v->value;
}

void table_del(struct table_s *t) {
    // Keys are nodes of a singly-linked list, to handle collisions. By freeing
    // in reverse-order from insertion we can implicitly free the back of the
    // list before the front, to avoid a use-after-free.  This is kind of a hack
    // so it may or may not work. (but it seems to work in testing)
    for (int i = t->util-1; i >= 0; i--) {
        struct key_s *k = t->keys[i];
        struct value_s *v = _table_get_v(t, k->hash);
        _key_del(k);
        _value_del(v);
    }
    free(t->keys);
    free(t->values);
    free(t);
}

void table_insert(table *t, char *key, long value) {
    struct key_s *k = _key_new(key);
    struct value_s *v = _value_new(value, k->hash);
    int index = _table_index(t->size, k->hash);

    struct value_s *ev = t->values[index];
    if (ev == NULL) {
        t->keys[t->util++] = k;
        t->values[index] = v;
    } else if (ev->hash == k->hash) {
        ev->value = value;
        _key_del(k);
        _value_del(v);
        return;
    } else {
        while (ev->next != NULL) {
            ev = ev->next;
        }
        ev->next = v;
        t->keys[t->util++] = k;
    }

    if (t->util > MAX_UTILIZATION(t->size)) {
        _table_resize(t);
    }
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
        printf("\t%s: %ld\n", t->keys[i]->key, table_get(t, t->keys[i]->key));
    }
    printf("}\n");
}

