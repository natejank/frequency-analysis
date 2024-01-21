#include <stdio.h>

int main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");

    while (!feof(fp)) {
        char c = fgetc(fp);
        if (0x0 > c || c > 0x7f) {
            if (c == EOF) return 0;
            printf("not ascii! %x\n", c);
            return 1;
        }
    }
    return 0;
}
