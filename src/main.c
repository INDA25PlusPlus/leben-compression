#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void err_invalid_usage() {
    fprintf(stderr, "Usage:\nEXE <encode|decode> <input_file> <output_file>\n");
    exit(1);
}

void err_failed_to_open(char const *path) {
    fprintf(stderr, "Failed to open '%s'\n", path);
    exit(1);
}

void err_failed_to_stat(char const *path) {
    fprintf(stderr, "Failed to stat '%s'\n", path);
    exit(1);
}

void err_same_input_output() {
    fprintf(stderr, "'input' and 'output' may not be the same file\n");
    exit(1);
}

void err_failed_to_close(char const *path) {
    fprintf(stderr, "Failed to close '%s'\n", path);
    exit(1);
}

int assert_different_files(FILE *a, FILE *b) {
    struct stat a_stat;
    struct stat b_stat;
    if (fstat(fileno(a), &a_stat)) {
        return 1;
    }
    if (fstat(fileno(b), &b_stat)) {
        return 2;
    }

    if (a_stat.st_dev == b_stat.st_dev && a_stat.st_ino == b_stat.st_ino) {
        err_same_input_output();
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 4 || (
            strcmp(argv[1], "encode") != 0 &&
            strcmp(argv[1], "decode") != 0))
    {
        err_invalid_usage();
    }

    bool is_decode_mode = strcmp(argv[1], "decode") != 0;

    FILE *inp = fopen(argv[2], "r");
    if (inp == NULL) {
        err_failed_to_open(argv[2]);
    }

    FILE *outp = fopen(argv[3], "w");
    if (outp == NULL) {
        err_failed_to_open(argv[3]);
    }

    int different_files = assert_different_files(inp, outp);
    if (different_files == 1) {
        err_failed_to_stat(argv[2]);
    } else if (different_files == 2) {
        err_failed_to_stat(argv[3]);
    }

    if (is_decode_mode) {
        // todo
        exit(1);
    } else {
        // todo
        exit(1);
    }

    if (fclose(inp)) {
        err_failed_to_close(argv[2]);
    }

    if (fclose(outp)) {
        err_failed_to_close(argv[3]);
    }

    return 0;
}
