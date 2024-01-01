#include "table.h"

int main(void) {
    table *t = table_new();
    table_insert(t, "hi", 32);
    table_print(t);
    table_insert(t, "hi", 34);
    table_insert(t, "goodbye", 0xDEADBEEF);
    table_print(t);
}

