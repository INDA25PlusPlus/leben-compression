#include "huffman.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define panic(...)                                                             \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);

void err_invalid_usage() {
    panic("Usage:\nEXE <encode|decode> <input_file> <output_file>\n");
}

void err_failed_to_open(char const *path) {
    panic("Failed to open '%s'\n", path);
}

void err_failed_to_stat(char const *path) {
    panic("Failed to stat '%s'\n", path);
}

void err_same_input_output() {
    panic("'input' and 'output' may not be the same file\n");
}

void err_failed_to_close(char const *path) {
    panic("Failed to close '%s'\n", path);
}

void err_failed_to_encode(int err) {
    panic("Failed to encode file with error code %d\n", err);
}

void err_failed_to_init_buffer(int err) {
    panic("Failed to init buffer with error code %d\n", err);
}

void err_failed_to_write_buffer(int err) {
    panic("Failed to write to buffer with error code %d\n", err);
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
    int err;

    if (argc != 4 ||
        (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0)) {
        err_invalid_usage();
    }

    FILE *inp_file = fopen(argv[2], "r");
    if (inp_file == NULL)
        err_failed_to_open(argv[2]);

    FILE *outp_file = fopen(argv[3], "w");
    if (outp_file == NULL)
        err_failed_to_open(argv[3]);

    int different_files = assert_different_files(inp_file, outp_file);
    if (different_files == 1) {
        err_failed_to_stat(argv[2]);
    } else if (different_files == 2) {
        err_failed_to_stat(argv[3]);
    }

    FileReader inp_buffer;
    FileWriter outp_buffer;
    err = file_reader_init(&inp_buffer, inp_file);
    if (err > 0)
        err_failed_to_init_buffer(err);
    err = file_writer_init(&outp_buffer);
    if (err > 0)
        err_failed_to_init_buffer(err);

    if (strcmp(argv[1], "encode") == 0) {
        int err = huffman_encode(&inp_buffer, &outp_buffer);
        if (err > 0)
            err_failed_to_encode(err);
    } else {
        // todo decode
        exit(1);
    }

    file_reader_deinit(&inp_buffer);
    err = file_writer_flush(&outp_buffer, outp_file);
    if (err > 0)
        err_failed_to_write_buffer(err);
    file_writer_deinit(&outp_buffer);

    if (fclose(inp_file))
        err_failed_to_close(argv[2]);

    if (fclose(outp_file))
        err_failed_to_close(argv[3]);

    return 0;
}
