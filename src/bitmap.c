//
// Created by Leonard on 2026-02-23.
//

#include "bitmap.h"

#include <stdlib.h>
#include <string.h>

Bitmap *bitmap_create(short size) {
    Bitmap *bm = malloc(sizeof(Bitmap));
    bm->size = size;
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

bool bitmap_get(Bitmap const *bm, char index) {
    char segment = bm->buf[index / 8];
    return (segment >> (index % 8)) & 0x1;
}

void set_bit_at(char *buf, size_t index, bool value) {
    char *byte = &buf[index / 8];
    char mask = 0x1 << (index % 8);
    char value_mask = value << (index % 8);
    *byte = (*byte & ~mask) | value_mask;
}

void bitmap_set(Bitmap *bm, char index, bool value) {
    set_bit_at(bm->buf, index, value);
}

void bitmap_resize(Bitmap *bm, short new_size) {
    bm->size = new_size;
}

Bitmap *bitmap_clone(Bitmap *bm) {
    Bitmap *new = bitmap_create(bm->size);
    if (new == NULL) {
        return NULL;
    }
    memcpy(new->buf, bm->buf, BITMAP_BYTES);
    return new;
}

void bitmap_copy_buffer(Bitmap const *bm, char *buf, size_t index) {
    for (int i = 0; i < bm->size; i++) {
        set_bit_at(buf, index + i, bitmap_get(bm, i));
    }
}
