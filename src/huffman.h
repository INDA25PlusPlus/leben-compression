//
// Created by Leonard on 2026-02-23.
//

#pragma once
#include <stddef.h>

typedef enum {
    LEAF,
    TREE
} HuffmanNodeType;

typedef struct HuffmanTreeNode HuffmanTreeNode;
struct HuffmanTreeNode {
    unsigned priority;
    HuffmanNodeType node_type;
    union {
        char character;
        struct {
            HuffmanTreeNode *left_child;
            HuffmanTreeNode *right_child;
        };
    };
};

HuffmanTreeNode *huffman_tree_create(char *buf, size_t len);

void huffman_tree_destroy(HuffmanTreeNode *tree);
