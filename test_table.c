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

    /* for (char *i = "a"; i[0] <= 'z'; i++) { */
    /*     table_insert(t, i, (int) i[0]); */
    /* } */
    table_print(t);
}

