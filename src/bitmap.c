//
// Created by Leonard on 2026-02-23.
//

#include "bitmap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Bitmap *bitmap_create(uint16_t size) {
    Bitmap *bm = malloc(sizeof(Bitmap));
    bm->len = size;
    if (bm == NULL) {
        return NULL;
    }
    for (int i = 0; i < BITMAP_BYTES; ++i) {
        bm->buf[i] = 0;
    }
    return bm;
}

void bitmap_destroy(Bitmap *bm) {
    free(bm);
}

bool bitmap_get(Bitmap const *bm, uint8_t index) {
    char segment = bm->buf[index / 8];
    return (segment >> (index % 8)) & 0x1;
}

void set_bit_at(uint8_t *buf, size_t index, bool value) {
    uint8_t *byte = &buf[index / 8];
    uint8_t mask = 0x1 << (index % 8);
    uint8_t value_mask = value << (index % 8);
    *byte = (*byte & ~mask) | value_mask;
}

void bitmap_set(Bitmap *bm, uint8_t index, bool value) {
    set_bit_at(bm->buf, index, value);
}

void bitmap_resize(Bitmap *bm, uint16_t new_size) {
    bm->len = new_size;
}

Bitmap *bitmap_clone(Bitmap *bm) {
    Bitmap *new = bitmap_create(bm->len);
    if (new == NULL) {
        return NULL;
    }
    memcpy(new->buf, bm->buf, BITMAP_BYTES);
    return new;
}

void bitmap_copy_buffer(Bitmap const *bm, uint8_t *buf, size_t index) {
    for (int i = 0; i < bm->len; i++) {
        set_bit_at(buf, index + i, bitmap_get(bm, i));
    }
}
