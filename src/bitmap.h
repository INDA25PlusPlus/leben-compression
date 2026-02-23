//
// Created by Leonard on 2026-02-23.
//

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Bitmap Bitmap;
struct Bitmap {
    size_t size;
    uint64_t *buf;
};

void bitmap_init(Bitmap *bitmap, size_t size);

void bitmap_deinit(Bitmap *bitmap);

bool bitmap_get(Bitmap const *bitmap, size_t index);

void bitmap_set(Bitmap *bitmap, size_t index, bool val);
