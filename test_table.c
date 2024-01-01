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

    int m = 73;
    table_insert(t, "a", m++);
    table_insert(t, "b", m++);
    table_insert(t, "c", m++);
    table_insert(t, "d", m++);
    table_insert(t, "e", m++);
    table_insert(t, "f", m++);
    table_insert(t, "g", m++);
    table_insert(t, "h", m++);
    table_insert(t, "i", m++);
    table_insert(t, "j", m++);
    table_insert(t, "k", m++);
    table_insert(t, "l", m++);
    table_insert(t, "m", m++);
    table_insert(t, "n", m++);
    table_insert(t, "o", m++);
    table_insert(t, "p", m++);
    table_insert(t, "q", m++);
    table_insert(t, "r", m++);
    table_insert(t, "s", m++);
    table_insert(t, "t", m++);
    table_insert(t, "u", m++);
    table_insert(t, "v", m++);
    table_insert(t, "w", m++);
    table_insert(t, "x", m++);
    table_insert(t, "y", m++);
    table_insert(t, "z", m++);
    table_print(t);
    assert(table_size(t) == 28);

    table_remove(t, "a");
    table_remove(t, "b");
    table_remove(t, "c");
    table_remove(t, "d");
    table_remove(t, "e");
    table_remove(t, "f");
    table_remove(t, "g");
    table_remove(t, "h");
    table_remove(t, "i");
    table_remove(t, "j");
    table_remove(t, "k");
    table_remove(t, "l");
    table_remove(t, "m");
    table_remove(t, "n");
    table_remove(t, "o");
    table_remove(t, "p");
    table_remove(t, "q");
    table_remove(t, "r");
    table_remove(t, "s");
    table_remove(t, "t");
    table_remove(t, "u");
    table_remove(t, "v");
    table_remove(t, "w");
    table_remove(t, "x");
    table_remove(t, "y");
    table_remove(t, "z");
    table_print(t);
    assert(table_size(t) == 2);

    table_del(t);
}

