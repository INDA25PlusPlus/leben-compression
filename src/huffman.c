//
// Created by Leonard on 2026-02-23.
//

#include "huffman.h"

#include <stdint.h>
#include <stdlib.h>

HuffmanTreeNode *huffman_tree_create(char *buf, size_t len) {
    // todo
    exit(1);
}

void huffman_tree_destroy(HuffmanTreeNode *tree) {
    switch (tree->node_type) {
    case LEAF:
        break;
    case TREE:
        huffman_tree_destroy(tree->left_child);
        huffman_tree_destroy(tree->right_child);
        break;
    }
    free(tree);
}
