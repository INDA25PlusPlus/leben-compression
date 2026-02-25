//
// Created by Leonard on 2026-02-24.
//

#pragma once

#include "bitmap.h"

#include <stdio.h>

typedef struct {
    uint8_t *buf;
    long len;
} FileBuffer;

int file_buffer_init(FileBuffer *file_buffer, FILE *file);

void file_buffer_deinit(FileBuffer *file_buffer);

typedef struct {
    uint8_t *buf;
    size_t bit_index;
    size_t capacity;
} FileWriteBuffer;

int file_write_buffer_init(FileWriteBuffer *fwb);

void file_write_buffer_deinit(FileWriteBuffer *fwb);

int file_write_buffer_put(FileWriteBuffer *fwb, uint8_t byte);

int file_write_buffer_write(FileWriteBuffer *fwb, uint8_t *bytes, size_t len);

int file_write_buffer_write_string(FileWriteBuffer *fwb, uint8_t *bytes);

int file_write_buffer_write_long(FileWriteBuffer *fwb, uint64_t bytes);

int file_write_buffer_write_bits(FileWriteBuffer *fwb, Bitmap const *bits);

int file_write_buffer_flush(FileWriteBuffer const *fwb, FILE *file);
