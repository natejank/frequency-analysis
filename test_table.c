#include "table.h"
#include <assert.h>

int main(void) {
    table *t = table_new();
    table_insert(t, "hi", 32);
    assert(table_size(t) == 1);

    table_insert(t, "hi", 34);
    table_insert(t, "hi", 34);
    assert(table_size(t) == 1);
    table_insert(t, "goodbye", 0xDEADBEEF);
    assert(table_size(t) == 2);

    table_insert(t, "a", 1);
    table_insert(t, "b", 1);
    table_insert(t, "c", 1);
    table_insert(t, "d", 1);
    table_insert(t, "e", 1);
    table_insert(t, "f", 1);
    table_insert(t, "g", 1);
    table_insert(t, "h", 1);
    table_insert(t, "i", 1);
    table_insert(t, "j", 1);
    table_print(t);
    table_insert(t, "k", 1);
    table_print(t);
    table_insert(t, "l", 1);
    table_insert(t, "m", 1);
    table_insert(t, "n", 1);
    table_insert(t, "o", 1);
    table_insert(t, "p", 1);
    table_insert(t, "q", 1);
    table_insert(t, "r", 1);
    table_insert(t, "s", 1);
    table_insert(t, "t", 1);
    table_insert(t, "u", 1);
    table_insert(t, "v", 1);
    table_insert(t, "w", 1);
    table_insert(t, "x", 1);
    table_insert(t, "y", 1);
    table_insert(t, "z", 1);
    assert(table_size(t) == 28);

    table_print(t);
}

