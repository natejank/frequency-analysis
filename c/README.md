# C frequency analysis

This is the original frequency analysis written in C.  It uses a custom
string:long hash table (`table.c` and `table.h`) to store words and their
frequencies, while trying to be fast and elegant.

The program is implemented in `freq.c`.  However, it only properly supports
ascii inputs.
