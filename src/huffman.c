//
// Created by Leonard on 2026-02-23.
//

#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>

int sort_freq_queue_fn(void const *a, void const *b) {
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

void sort_freq_queue(HuffmanTreeBuilder *htb) {
    qsort(
        htb->queue,
        htb->len,
        sizeof(HuffmanFreqQueueEntry),
        sort_freq_queue_fn);
}

void debug_print_tree(HuffmanTreeNode *node) {
    printf("(");
    if (node->node_type == LEAF) {
        printf("'%c' 0x%x", node->character, node->character);
    } else {
        debug_print_tree(node->left_child);
        printf("|");
        debug_print_tree(node->right_child);
    }

    printf(")");
}

void register_bit_sequences(
    HuffmanTreeBuilder *htb, HuffmanTreeNode *node, Bitmap *bit_stack) {
    switch (node->node_type) {
    case LEAF:
        char ch = node->character;
        if (bit_stack->size == 0) {
            // special case where only one type of character is present
            htb->bit_sequences[ch] = bitmap_create(1);
        } else {
            htb->bit_sequences[ch] = bitmap_clone(bit_stack);
        }
        break;
    case BRANCH:
        bitmap_resize(bit_stack, bit_stack->size + 1);

        // push a 0
        bitmap_set(bit_stack, bit_stack->size - 1, false);
        register_bit_sequences(htb, node->left_child, bit_stack);

        // push a 1
        bitmap_set(bit_stack, bit_stack->size - 1, false);
        register_bit_sequences(htb, node->right_child, bit_stack);

        // pop
        bitmap_resize(bit_stack, bit_stack->size - 1);
        break;
    }
}

HuffmanTreeBuilder *huffman_tree_builder_create(FileBuffer const *file_buffer) {
    HuffmanTreeBuilder *htb = malloc(sizeof(HuffmanTreeBuilder));
    if (htb == NULL) {
        return NULL;
    }
    for (int i = 0; i < 256; ++i) {
        htb->queue[i].entry = &htb->leaf_arena[i];
        htb->leaf_arena[i] =
            (HuffmanTreeNode) {.freq = 0, .node_type = LEAF, .character = i};
        htb->bit_sequences[i] = NULL;
    }
    htb->len = 256;
    for (int i = 0; i < file_buffer->len; ++i) {
        char ch = file_buffer->buf[i];
        htb->leaf_arena[ch].freq++;
    }
    sort_freq_queue(htb);

    // set len to no. of distinct present code points
    for (int i = 0; i < 256; ++i) {
        if (htb->queue[i].entry->freq == 0) {
            htb->len = i;
            break;
        }
    }

    // connect all nodes to one root node
    while (htb->len > 1) {
        // connect the two nodes with the lowest frequencies
        HuffmanFreqQueueEntry *last = &htb->queue[htb->len - 1];
        HuffmanFreqQueueEntry *second_last = &htb->queue[htb->len - 2];

        size_t total_freq = last->entry->freq + second_last->entry->freq;

        // replace the last queue entry with a branch node connecting the two
        // nodes
        last->branch = (HuffmanTreeNode) {.freq = total_freq,
                                          .node_type = BRANCH,
                                          .left_child = last->entry,
                                          .right_child = second_last->entry};

        // point the second last queue entry to the new branch node
        second_last->entry = &htb->queue[htb->len - 1].branch;

        htb->len--;
        sort_freq_queue(htb);
    }

    Bitmap *bit_stack = bitmap_create(0);
    register_bit_sequences(htb, htb->queue[0].entry, bit_stack);
    bitmap_destroy(bit_stack);

    return htb;
}

void huffman_tree_builder_destroy(HuffmanTreeBuilder *htb) {
    for (int i = 0; i < 256; i++) {
        // free(null) is fine
        free(htb->bit_sequences[i]);
    }
    free(htb);
}
