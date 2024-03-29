#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

bool is_quote(char c) {
    return (c == '"' || c == '\'');
}

void to_lower_case(char *w) {
    for (unsigned int i = 0; i < strlen(w); i++) {
        char c = *(w + i);
        if ('A' <= c && c <= 'Z') {
            *(w + i) = c + 32;
        }
    }
}

int trim_word(char *w, int size) {
    char first = w[0];
    char last = w[size - 1];
    if (!is_alphanumeric(first) && is_alphanumeric(last)) {
        memmove(w, w + 1, size - 1);
        size--;
        w[size] = '\0';
    } else if (is_alphanumeric(first) && !is_alphanumeric(last)) {
        size--;
        w[size] = '\0';
    } else if (!is_alphanumeric(first) && !is_alphanumeric(last)) {
        memmove(w, w + 1, size - 1);
        size = size - 2;
        w[size] = '\0';
    }

    return size;
}

void print_csv(table *t) {
    t_keys *keys = table_keys(t);
    for (int i = 0; i < keys->count; i++) {
        char *key = keys->keys[i];
        int value = table_get(t, key);

        printf("%s,%d\n", key, value);
    }
    table_t_keys_del(keys);
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
            int new_size = trim_word(word, ws-1);
            if (new_size == 0) {
                continue;
            }
            to_lower_case(word);
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
    print_csv(t);
    table_del(t);
    fclose(fp);
}
