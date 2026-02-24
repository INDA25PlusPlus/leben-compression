//
// Created by Leonard on 2026-02-23.
//

#pragma once

#include "io.h"

typedef enum {
    LEAF,
    TREE
} HuffmanNodeType;

typedef struct HuffmanTreeNode HuffmanTreeNode;
struct HuffmanTreeNode {
    size_t freq;
    HuffmanNodeType node_type;
    union {
        char character;
        struct {
            HuffmanTreeNode *left_child;
            HuffmanTreeNode *right_child;
        };
    };
};

typedef struct {
    HuffmanTreeNode node_arena[256];
    HuffmanTreeNode *queue[256];
    size_t len;
} HuffmanFreqQueue;

HuffmanFreqQueue *huffman_freq_queue_create();

void huffman_freq_queue_destroy(HuffmanFreqQueue *hfq);

void huffman_freq_queue_init(
    HuffmanFreqQueue *hfq, FileBuffer const *file_buffer);
