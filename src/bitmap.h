//
// Created by Leonard on 2026-02-23.
//

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BITMAP_BYTES 32

typedef struct Bitmap Bitmap;
struct Bitmap {
    uint16_t len;
    uint8_t buf[BITMAP_BYTES];
};

Bitmap *bitmap_create(uint16_t size);

void bitmap_destroy(Bitmap *bm);

bool bitmap_get(Bitmap const *bm, uint8_t index);

void bitmap_set(Bitmap *bm, uint8_t index, bool value);

void bitmap_resize(Bitmap *bm, uint16_t new_size);

Bitmap *bitmap_clone(Bitmap *bm);

void bitmap_copy_buffer(Bitmap const *bm, uint8_t *buf, size_t start_index);
