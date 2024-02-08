#ifndef HUFFMANENCODING_ENCODER_H
#define HUFFMANENCODING_ENCODER_H

typedef struct encoder_entry {
	char* str;
	huffman_code* hfc;
} encoder_entry;

int ee_compare_by_wrapped_character(const void *a, const void *b, void *udata);
uint64_t ee_hash_by_wrapped_character(const void *item, uint64_t seed0, uint64_t seed1);
bool ee_iter(const void *item, void *udata);
encoder_entry* ee_init(huffman_code* hfc);

#endif //HUFFMANENCODING_ENCODER_H