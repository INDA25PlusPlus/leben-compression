//
// Created by Leonard on 2026-02-24.
//

#include "io.h"

#include <stdlib.h>

int file_buffer_init(FileBuffer *file_buffer, FILE *file) {
    fseek(file, 0, SEEK_END);
    file_buffer->len = ftell(file);
    rewind(file);
    file_buffer->buf = malloc(file_buffer->len);
    if (file_buffer->buf == NULL) {
        return 1;
    }
    fread(file_buffer->buf, 1, file_buffer->len, file);
    return 0;
}

void file_buffer_deinit(FileBuffer *file_buffer) {
    free(file_buffer->buf);
    file_buffer->buf = NULL;
}

int file_write_buffer_init(FileWriteBuffer *fwb) {
    fwb->buf = malloc(16);
    if (fwb->buf == NULL) {
        return 1;
    }
    fwb->bit_index = 0;
    fwb->capacity = 16;
    return 0;
}

void file_write_buffer_deinit(FileWriteBuffer *fwb) {
    free(fwb->buf);
    fwb->buf = NULL;
}

int file_write_buffer_grow(FileWriteBuffer *fwb) {
    size_t new_capacity = 2 * fwb->capacity;
    char *new_buf = realloc(fwb->buf, new_capacity);
    if (new_buf == NULL) {
        return 1;
    }
    fwb->buf = new_buf;
    fwb->capacity = new_capacity;
    return 0;
}

int file_write_buffer_ensure_capacity(FileWriteBuffer *fwb, size_t capacity) {
    while (fwb->capacity < capacity) {
        int result = file_write_buffer_grow(fwb);
        if (result > 0) {
            return result;
        }
    }
    return 0;
}

int file_write_buffer_put(FileWriteBuffer *fwb, char byte) {
    // if in the middle of writing bits, truncate the last byte
    size_t index = fwb->bit_index / 8;
    int result = file_write_buffer_ensure_capacity(fwb, index + 1);
    if (result > 0) {
        return result;
    }
    fwb->buf[index] = byte;
    fwb->bit_index = (index + 1) * 8;
    return 0;
}

int file_write_buffer_write(FileWriteBuffer *fwb, char *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        int err = file_write_buffer_put(fwb, bytes[i]);
        if (err > 0)
            return err;
    }
    return 0;
}

int file_write_buffer_write_string(FileWriteBuffer *fwb, char *bytes) {
    for (size_t i = 0; bytes[i] != 0x00; i++) {
        int err = file_write_buffer_put(fwb, bytes[i]);
        if (err > 0)
            return err;
    }
    return 0;
}

int file_write_buffer_write_bits(FileWriteBuffer *fwb, Bitmap const *bits) {
    size_t new_len = (fwb->bit_index + bits->len + 7) / 8;
    int result = file_write_buffer_ensure_capacity(fwb, new_len);
    if (result > 0) {
        return result;
    }
    bitmap_copy_buffer(bits, fwb->buf, fwb->bit_index);
    fwb->bit_index += bits->len;
    return 0;
}

int file_write_buffer_flush(FileWriteBuffer const *fwb, FILE *file) {
    size_t len = (fwb->bit_index + 7) / 8;
    size_t written_bytes = fwrite(fwb->buf, 1, len, file);
    if (written_bytes != len) {
        return 1;
    }
    return 0;
}
