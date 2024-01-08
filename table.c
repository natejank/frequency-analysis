#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

#define DEFAULT_TABLE_SIZE 20
#define MAX_UTILIZATION(size) size/2
#define RESIZE(size) size*2

typedef unsigned int hash_t;

struct key_s {
    char *key;
    hash_t hash;
};

struct value_s {
    long value;
    hash_t hash;
    struct value_s *next;
};

struct table_s {
    struct key_s **keys;
    struct value_s **values;
    int size;
    int util;
};

hash_t _hash_key(char *key) {
    // http://www.cse.yorku.ca/~oz/hash.html - djb2
    hash_t hash = 5381;
    char c;

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

struct value_s* _value_new(long v, hash_t hash) {
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
    t->keys = calloc(sizeof(struct key_s*),  size);
    t->values = calloc(sizeof(struct value_s*), size);
    return t;
}

int _table_index(int size, hash_t hash) {
    return hash % size;
}

struct value_s* _table_get_v(struct table_s *t, hash_t hash) {
    int index = _table_index(t->size, hash);
    struct value_s *ev = t->values[index];
    while (ev != NULL && ev->hash != hash) {
        ev = ev->next;
    }
    return ev;
}


void _table_resize(struct table_s *t) {
    int new_size = RESIZE(t->size);

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

t_keys* table_keys(struct table_s *t) {
    t_keys *k = malloc(sizeof(t_keys));
    k->count = table_size(t);
    k->keys = malloc(sizeof(char*) * k->count);

    for (int i = 0; i < k->count; i++) {
        char *orig = t->keys[i]->key;
        int len = strlen(orig) + 1;
        char *new = malloc(sizeof(char) * len);
        strncpy(new, orig, len);
        k->keys[i] = new;
    }
    return k;
}

void table_t_keys_del(t_keys *k) {
    for (int i = 0; i < k->count; i++) {
        free(k->keys[i]);
    }
    free(k);
}

long table_get(struct table_s *t, char *key) {
    hash_t hash = _hash_key(key);
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
    } else {
        while (ev->hash != k->hash && ev->next != NULL) {
            ev = ev->next;
        }
        if (ev->hash == k->hash) {
            ev->value = value;
            _key_del(k);
            _value_del(v);
            return;
        } else {
            ev->next = v;
            t->keys[t->util++] = k;
        }
    }

    if (t->util > MAX_UTILIZATION(t->size)) {
        _table_resize(t);
    }
}

bool table_contains(table *t, char *key) {
    hash_t hash = _hash_key(key);
    for (int i = 0; i < t->util; i++) {
        if (t->keys[i]->hash == hash) {
            return true;
        }
    }
    return false;
}

void table_remove(table *t, char *key) {
    hash_t hash = _hash_key(key);
    int index = _table_index(t->size, hash);
    struct value_s *pv = NULL;
    struct value_s *v = t->values[index];

    if (v == NULL) {
        return;
    }

    while (v->hash != hash) {
        pv = v;
        v = pv->next;
        if (v == NULL) {
            return;
        }
    }

    if (v->next != NULL) {
        if (pv != NULL) {
            pv->next = v->next;
        } else {
            t->values[index] = v->next;
        }
    }
    _value_del(v);

    for (int i = 0; i < t->util; i++) {
        struct key_s *k = t->keys[i];
        if (k->hash == hash) {
            _key_del(k);
            memmove(t->keys+i, t->keys+i+1, (t->util-1-i) * sizeof(struct key_s*));
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

