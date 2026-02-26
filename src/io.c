//
// Created by Leonard on 2026-02-24.
//

#include "io.h"

#include <stdlib.h>

int file_reader_init(FileReader *fr, FILE *file) {
    fseek(file, 0, SEEK_END);
    fr->len = ftell(file);
    rewind(file);
    fr->buf = malloc(fr->len);
    if (fr->buf == NULL) {
        return 1;
    }
    fread(fr->buf, 1, fr->len, file);
    return 0;
}

void file_reader_deinit(FileReader *fr) {
    free(fr->buf);
    fr->buf = NULL;
}

int file_reader_read(FileReader *fr, uint8_t *out) {
    // skip partially started bit
    size_t byte_index = (fr->bit_index + 7) / 8;
    if (fr->len <= byte_index)
        return 1;
    *out = fr->buf[byte_index];
    fr->bit_index = 8 * (byte_index + 1);
    return 0;
}

int file_reader_read_long(FileReader *fr, uint64_t *out) {
    uint64_t result = 0;
    // read little-endian
    for (int i = 0; i < 64; i += 8) {
        uint8_t byte = 0;
        int err = file_reader_read(fr, &byte);
        if (err > 0)
            return err;
        result |= byte << i;
    }
    *out = result;
    return 0;
}

int file_reader_read_bit(FileReader *fr, bool *out) {
    size_t byte_index = fr->bit_index / 8;
    if (fr->len <= byte_index)
        return 1;
    uint8_t byte = fr->buf[byte_index];
    *out = (byte >> (fr->bit_index % 8)) & 0x1;
    fr->bit_index++;
    return 0;
}

int file_reader_cmp_string(FileReader *fr, uint8_t const *string) {
    // skip partially started bit
    size_t byte_index = (fr->bit_index + 7) / 8;
    size_t i = 0;
    for (; string[i] != 0x00; i++) {
        if (fr->len <= byte_index + i)
            return 1;
    }
    fr->bit_index = 8 * (byte_index + i);
    return 0;
}

int file_reader_test_end(FileReader *fr) {
    size_t byte_index = (fr->bit_index + 7) / 8;

    // test that reader is at end of file
    if (byte_index != fr->len)
        return 1;

    uint8_t final_byte = fr->buf[fr->len - 1];

    // test remaining bits are 0
    if ((final_byte >> fr->bit_index % 8) != 0)
        return 1;

    return 0;
}

int file_writer_init(FileWriter *fw) {
    fw->buf = malloc(16);
    if (fw->buf == NULL)
        return 1;
    fw->bit_index = 0;
    fw->capacity = 16;
    return 0;
}

void file_writer_deinit(FileWriter *fw) {
    free(fw->buf);
    fw->buf = NULL;
}

int file_write_buffer_grow(FileWriter *fwb) {
    size_t new_capacity = 2 * fwb->capacity;
    uint8_t *new_buf = realloc(fwb->buf, new_capacity);
    if (new_buf == NULL)
        return 1;

    fwb->buf = new_buf;
    fwb->capacity = new_capacity;
    return 0;
}

int file_write_buffer_ensure_capacity(FileWriter *fwb, size_t capacity) {
    while (fwb->capacity < capacity) {
        int err = file_write_buffer_grow(fwb);
        if (err > 0) {
            return err;
        }
    }
    return 0;
}

int file_writer_put(FileWriter *fw, uint8_t byte) {
    // if in the middle of writing bits, truncate the last byte
    size_t index = fw->bit_index / 8;
    int err = file_write_buffer_ensure_capacity(fw, index + 1);
    if (err > 0)
        return err;

    fw->buf[index] = byte;
    fw->bit_index = (index + 1) * 8;
    return 0;
}

int file_writer_write(FileWriter *fw, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        int err = file_writer_put(fw, bytes[i]);
        if (err > 0)
            return err;
    }
    return 0;
}

int file_writer_write_string(FileWriter *fw, uint8_t *bytes) {
    for (size_t i = 0; bytes[i] != 0x00; i++) {
        int err = file_writer_put(fw, bytes[i]);
        if (err > 0)
            return err;
    }
    return 0;
}

int file_writer_write_long(FileWriter *fw, uint64_t bytes) {
    // write little-endian
    for (int i = 0; i < 64; i += 8) {
        int err = file_writer_put(fw, bytes >> i);
        if (err > 0)
            return err;
    }
    return 0;
}

int file_writer_write_bits(FileWriter *fw, Bitmap const *bits) {
    size_t new_len = (fw->bit_index + bits->len + 7) / 8;
    int err = file_write_buffer_ensure_capacity(fw, new_len);
    if (err > 0)
        return err;

    bitmap_copy_buffer(bits, fw->buf, fw->bit_index);
    fw->bit_index += bits->len;
    return 0;
}

int file_writer_flush(FileWriter const *fw, FILE *file) {
    size_t len = (fw->bit_index + 7) / 8;
    size_t written_bytes = fwrite(fw->buf, 1, len, file);
    if (written_bytes != len)
        return 1;

    return 0;
}
