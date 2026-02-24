//
// Created by Leonard on 2026-02-23.
//

#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>

HuffmanFreqQueue *huffman_freq_queue_create() {
    HuffmanFreqQueue *hfq = malloc(sizeof(HuffmanFreqQueue));
    if (hfq == NULL) {
        return NULL;
    }
    for (int i = 0; i < 256; ++i) {
        hfq->queue[i] = &hfq->node_arena[i];
        hfq->node_arena[i].freq = 0;
        hfq->node_arena[i].node_type = LEAF;
        hfq->node_arena[i].character = i;
    }
    hfq->len = 256;
    return hfq;
}

void huffman_freq_queue_destroy(HuffmanFreqQueue *hfq) {
    for (int i = 0; i < 256; ++i) {
        hfq->queue[i] = NULL;
    }
    free(hfq);
}

int huffman_freq_queue_sort_fn(void const *a, void const *b) {
    return (int) (*(HuffmanTreeNode *const *) b)->freq -
           (int) (*(HuffmanTreeNode *const *) a)->freq;
}

void huffman_freq_queue_sort(HuffmanFreqQueue *hfq) {
    qsort(
        hfq->queue, 256, sizeof(HuffmanTreeNode *), huffman_freq_queue_sort_fn);
}

void huffman_freq_queue_init(
    HuffmanFreqQueue *hfq, FileBuffer const *file_buffer) {
    for (int i = 0; i < file_buffer->len; ++i) {
        char ch = file_buffer->buf[i];
        hfq->node_arena[ch].freq++;
    }
    huffman_freq_queue_sort(hfq);

    // debug
    for (int i = 0; i < 256; ++i) {
        HuffmanTreeNode *entry = hfq->queue[i];
        if (entry->freq > 0) {
            printf("%lu counts of '%c'\n", entry->freq, entry->character);
        }
    }

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
