//
// Created by Leonard on 2026-02-23.
//

#pragma once

#include "bitmap.h"
#include "io.h"

typedef enum {
    LEAF,
    BRANCH
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

typedef union {
    HuffmanTreeNode branch;
    HuffmanTreeNode *entry;
} HuffmanFreqQueueEntry;

typedef struct {
    // design idea: the queue "shrinks" with len, so that the space left can be
    // used as additional arena space for new branch nodes. the maximum amount
    // of nodes in a huffman tree is 2n-1, where n = 256 for single byte coding.
    // so in the max case, we will have 511 nodes + 1 reference to the top node.
    HuffmanFreqQueueEntry queue[256];
    HuffmanTreeNode leaf_arena[256];
    Bitmap *bit_sequences[256];
    size_t len;
} HuffmanTreeBuilder;

HuffmanTreeBuilder *huffman_tree_builder_create(FileBuffer const *file_buffer);

void huffman_tree_builder_destroy(HuffmanTreeBuilder *htb);

int huffman_tree_builder_write_tree(
    HuffmanTreeBuilder const *htb, FileWriteBuffer *buf);
