//
// Created by Leonard on 2026-02-24.
//

#include "io.h"

#include <stdlib.h>

void err_failed_to_allocate() {
    fprintf(stderr, "Failed to allocate file buffer");
    exit(1);
}

void file_buffer_init(FileBuffer *file_buffer, FILE *file) {
    fseek(file, 0, SEEK_END);
    file_buffer->len = ftell(file);
    rewind(file);
    file_buffer->buf = malloc(file_buffer->len);
    if (file_buffer->buf == NULL) {
        err_failed_to_allocate();
    }
    fread(file_buffer->buf, 1, file_buffer->len, file);
}
void file_buffer_deinit(FileBuffer *file_buffer) {
    free(file_buffer->buf);
    file_buffer->buf = NULL;
}

