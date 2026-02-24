//
// Created by Leonard on 2026-02-23.
//

#include "huffman.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t ch;
    size_t freq;
} HuffmanFreqTableEntry;

typedef struct {
    HuffmanFreqTableEntry *entries;
} HuffmanFreqTable;

typedef struct {
    HuffmanFreqTableEntry *entries;
} HuffmanSortedFreqTable;

void huffman_freq_table_init(HuffmanFreqTable *hft) {
    hft->entries = malloc(256 * sizeof(HuffmanFreqTableEntry));
    for (int i = 0; i < 256; i++) {
        hft->entries[i].ch = i;
        hft->entries[i].freq = 0;
    }
}

void huffman_freq_table_deinit(HuffmanFreqTable *hft) {
    free(hft->entries);
    hft->entries = NULL;
}

void huffman_freq_table_inc(HuffmanFreqTable *hft, uint8_t ch) {
    hft->entries[ch].freq++;
}

size_t huffman_freq_table_get(HuffmanFreqTable const *hft, uint8_t ch) {
    return hft->entries[ch].freq;
}

int huffman_freq_table_sort_fn(void const *a, void const *b) {
    return (int) ((HuffmanFreqTableEntry const *) b)->freq -
           (int) ((HuffmanFreqTableEntry const *) a)->freq;
}

HuffmanSortedFreqTable huffman_freq_table_sort(HuffmanFreqTable table) {
    qsort(
        table.entries,
        256,
        sizeof(HuffmanFreqTableEntry),
        huffman_freq_table_sort_fn);
    return *(HuffmanSortedFreqTable *) &table;
}

HuffmanTreeNode *huffman_tree_create(FileBuffer const *file_buffer) {
    HuffmanSortedFreqTable sorted_freq_table;
    {
        HuffmanFreqTable freq_table;
        huffman_freq_table_init(&freq_table);
        for (size_t i = 0; i < file_buffer->len; i++) {
            huffman_freq_table_inc(&freq_table, file_buffer->buf[i]);
        }
        sorted_freq_table = huffman_freq_table_sort(freq_table);
    }

    // debug
    for (size_t i = 0; i < 256; i++) {
        size_t freq = sorted_freq_table.entries[i].freq;
        if (freq > 0) {
            printf(
                "%lu counts of '%c'\n", freq, sorted_freq_table.entries[i].ch);
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
