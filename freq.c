#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "table.h"

void usage(char *exe) {
    fprintf(stderr, "%s [file1] [file2] ...\n", exe);
}

bool is_alphanumeric(char c) {
    return (
               ('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9')
    );
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }


    table *t = table_new();

    FILE *fp;
    if (argv[1][0] == '-') {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
    }
    int ws = 0;
    int wl = 20;
    char *word = malloc(sizeof(char) * wl);
    while (!feof(fp)) {
        char c = fgetc(fp);
        if (!isspace(c)) {
            word[ws++] = c;
            if (ws == wl) {
                ws = ws * 2;
                word = realloc(word, sizeof(char) * ws);
            }
        } else {
            if (ws == 0) {
                continue;
            }
            word[ws++] = '\0';
            int cur = table_get(t, word);
            if (cur == -1) {
                table_insert(t, word, 1);
            } else {
                table_insert(t, word, cur + 1);
            }
            ws = 0;
        }
    }
    free(word);
    wl = 20;
    table_print(t);
    table_del(t);
    fclose(fp);
}
