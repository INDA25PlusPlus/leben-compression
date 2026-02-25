//
// Created by Leonard on 2026-02-23.
//

#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>

int huffman_freq_queue_sort_fn(void const *a, void const *b) {
    HuffmanFreqQueueEntry const *a_node = a;
    HuffmanFreqQueueEntry const *b_node = b;
    if (a_node->entry->freq == b_node->entry->freq) {
        // put branch nodes further back in the queue to encourage connecting
        // leaf nodes rather than branch nodes
        return (b_node->entry->node_type == BRANCH) -
               (a_node->entry->node_type == BRANCH);
    }
    return (int) b_node->entry->freq - (int) a_node->entry->freq;
}

void huffman_freq_queue_sort(HuffmanFreqQueue *hfq) {
    qsort(
        hfq->queue,
        hfq->len,
        sizeof(HuffmanFreqQueueEntry),
        huffman_freq_queue_sort_fn);
}

void debug_print(HuffmanTreeNode *node) {
    printf("(");
    if (node->node_type == LEAF) {
        printf("'%c' 0x%x", node->character, node->character);
    } else {
        debug_print(node->left_child);
        printf("|");
        debug_print(node->right_child);
    }

    printf(")");
}

HuffmanFreqQueue *huffman_freq_queue_create(FileBuffer const *file_buffer) {
    HuffmanFreqQueue *hfq = malloc(sizeof(HuffmanFreqQueue));
    if (hfq == NULL) {
        return NULL;
    }
    for (int i = 0; i < 256; ++i) {
        hfq->queue[i].entry = &hfq->leaf_arena[i];
        hfq->leaf_arena[i] =
            (HuffmanTreeNode) {.freq = 0, .node_type = LEAF, .character = i};
    }
    hfq->len = 256;
    for (int i = 0; i < file_buffer->len; ++i) {
        char ch = file_buffer->buf[i];
        hfq->leaf_arena[ch].freq++;
    }
    huffman_freq_queue_sort(hfq);

    // set len to no. of distinct present code points
    for (int i = 0; i < 256; ++i) {
        if (hfq->queue[i].entry->freq == 0) {
            hfq->len = i;
            break;
        }
    }

    // connect all nodes to one root node
    while (hfq->len > 1) {
        // connect the two nodes with the lowest frequencies
        HuffmanFreqQueueEntry *last = &hfq->queue[hfq->len - 1];
        HuffmanFreqQueueEntry *second_last = &hfq->queue[hfq->len - 2];

        size_t total_freq = last->entry->freq + second_last->entry->freq;

        // replace the last queue entry with a branch node connecting the two
        // nodes
        last->branch = (HuffmanTreeNode) {.freq = total_freq,
                                          .node_type = BRANCH,
                                          .left_child = last->entry,
                                          .right_child = second_last->entry};

        // point the second last queue entry to the new branch node
        second_last->entry = &hfq->queue[hfq->len - 1].branch;

        hfq->len--;
        huffman_freq_queue_sort(hfq);
    }

    // This is the Huffman tree for 'This-is-a-test-file!'
    //
    //        _______|_______
    //    ____|____      ___|___
    //  __|__   __|__  __|__ __|__
    // _|_ _|_  t   e _|_  s i   -
    // T ! f a        l h
    //
    debug_print(hfq->queue[0].entry);

    // todo
    exit(1);
}

void huffman_freq_queue_destroy(HuffmanFreqQueue *hfq) {
    free(hfq);
}
