//
// Created by Leonard on 2026-02-23.
//

#include "bitmap.h"

#include <stdlib.h>

void bitmap_init(Bitmap *bitmap, size_t size) {
    bitmap->size = size;
    // ceil(size / 64) * 8
    size_t buf_size = (size + 63) / 64 * sizeof(uint64_t);
    bitmap->buf = malloc(buf_size);
}

void bitmap_deinit(Bitmap *bitmap) {
    free(bitmap->buf);
    bitmap->buf = NULL;
}

bool bitmap_get(Bitmap const *bitmap, size_t index) {
    uint64_t segment = bitmap->buf[index / 64];
    return (segment >> (index % 64)) & 0x1;
}

void bitmap_set(Bitmap *bitmap, size_t index, bool val) {
    uint64_t *segment = &bitmap->buf[index / 64];
    uint64_t mask = val << (index % 64);
    *segment = *segment ^ (*segment ^ mask);
}
