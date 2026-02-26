//
// Created by Leonard on 2026-02-24.
//

#pragma once

#include "bitmap.h"

#include <stdio.h>

typedef struct {
    uint8_t *buf;
    size_t bit_index;
    long len;
} FileReader;

int file_reader_init(FileReader *fr, FILE *file);

void file_reader_deinit(FileReader *fr);

typedef struct {
    uint8_t *buf;
    size_t bit_index;
    size_t capacity;
} FileWriter;

int file_writer_init(FileWriter *fw);

void file_writer_deinit(FileWriter *fw);

int file_writer_put(FileWriter *fw, uint8_t byte);

int file_writer_write(FileWriter *fw, uint8_t *bytes, size_t len);

int file_writer_write_string(FileWriter *fw, uint8_t *bytes);

int file_writer_write_long(FileWriter *fw, uint64_t bytes);

int file_writer_write_bits(FileWriter *fw, Bitmap const *bits);

int file_writer_flush(FileWriter const *fw, FILE *file);
