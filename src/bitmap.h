//
// Created by Leonard on 2026-02-23.
//

#pragma once
#include <stdbool.h>
#include <stddef.h>

#define BITMAP_BYTES 32

typedef struct Bitmap Bitmap;
struct Bitmap {
    short size;
    char buf[BITMAP_BYTES];
};

Bitmap *bitmap_create(short size);

void bitmap_destroy(Bitmap *bm);

bool bitmap_get(Bitmap const *bm, char index);

void bitmap_set(Bitmap *bm, char index, bool value);

void bitmap_resize(Bitmap *bm, short new_size);

Bitmap *bitmap_clone(Bitmap *bm);

void bitmap_copy_buffer(Bitmap const *bm, char *buf, size_t start_index);
