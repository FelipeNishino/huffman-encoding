#ifndef HUFFMANENCODING_HUFFMAN_H
#define HUFFMANENCODING_HUFFMAN_H

#include "hashmap.h"

typedef struct char_frequency_row {
	int frequency;
	char character;
	char* code;
} char_frequency_row;

int cfr_compare(const void *a, const void *b, void *udata);
bool cfr_iter(const void *item, void *udata);
uint64_t cfr_hash(const void *item, uint64_t seed0, uint64_t seed1);
int cfr_generate_frequency_chart(struct hashmap *map, char* src);
int huffman_encode_text(char* src);
int huffman_encode_file(const char* filename);

#endif //HUFFMANENCODING_HUFFMAN_H