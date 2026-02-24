//
// Created by Leonard on 2026-02-24.
//

#pragma once
#include <stdio.h>

typedef struct {
    char *buf;
    long len;
} FileBuffer;

void file_buffer_init(FileBuffer *file_buffer, FILE *file);

void file_buffer_deinit(FileBuffer *file_buffer);
