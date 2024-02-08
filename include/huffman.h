#ifndef HUFFMANENCODING_HUFFMAN_H
#define HUFFMANENCODING_HUFFMAN_H

#include "hashmap.h"

#define ASCII_MAX 96
#define ASCII_OFFSET 32

typedef struct hashmap coder;

typedef enum {
	by_freq,  by_char
} hfc_sort_option;

typedef struct {
	char* text;
	char* head;
} text_tape;

typedef enum huffman_option {
	decompress, compress
} huffman_option;

typedef struct huffman_code {
	int frequency;
	char character;
	const char* code;
} huffman_code;

#include "huffman_tree.h"

void hfc_print(huffman_code *hfc);
huffman_code* hfc_init(char c);
huffman_code* hfc_init_with_params(int freq, char c, const char* code);
huffman_code* hfc_copy(huffman_code* hfc);
huffman_code** hfc_generate_frequency_chart(char* src);
void huffman_make_coder(struct hashmap* map, hfc_node* hfcn, const char* code, int depth);
coder* huffman_make_encoder(char* src);
int* huffman_read_frequency_chart(huffman_code** hfc_array, char* src, int* text_start_i);
hfc_node* huffman_make_decoder(char* src, int* text_start_i);
char huffman_tree_find_char(hfc_node* root, text_tape* tape);
int huffman_compress(const char* filename);
int huffman_decompress(const char* filename);
int huffman_run(huffman_option option, const char* filename);

#endif //HUFFMANENCODING_HUFFMAN_H