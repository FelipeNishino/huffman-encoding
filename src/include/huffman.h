#ifndef HUFFMANENCODING_HUFFMAN_H
#define HUFFMANENCODING_HUFFMAN_H

#include "hashmap.h"

#define ASCII_MAX 96
#define ASCII_OFFSET 32

typedef struct hashmap coder;

typedef enum {
	by_freq,by_char
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

typedef struct hfc_node {
    huffman_code* hfc;
    struct hfc_node *l;
  	struct hfc_node *r;
} hfc_node;

typedef struct encoder_entry {
	char* str;
	huffman_code* hfc;
} encoder_entry;

typedef struct node {
	struct node* next;
	hfc_node* hfcn;
} node;

void hfc_print(huffman_code *hfc);
void hfc_display(huffman_code** arr, int n);
void hfc_quicksort_swap(huffman_code* a, huffman_code* b);
int hfc_quicksort_partition(huffman_code** array, int left, int right, int pivot, hfc_sort_option option);
void hfc_quicksort(huffman_code** array, int left, int right, hfc_sort_option option);
void hfc_sort_subsection_by_char(huffman_code** array, int n);
huffman_code* hfc_init(char c);
huffman_code* hfc_init_with_params(int freq, char c, const char* code);
huffman_code* hfc_copy(huffman_code* hfc);
huffman_code** hfc_array_init(int n);
huffman_code** hfc_generate_frequency_chart(char* src);
int huffman_count_hfc_array(huffman_code** array);
hfc_node* hfc_init_blank_node();
hfc_node* hfc_init_node(huffman_code* hfc);
node* node_init(hfc_node* hfc_node); 
void node_display(node* current);
void huffman_print_tree(hfc_node* hfcn, int tab, int spacing);
void node_insert_sorted(node* current, hfc_node* hfcn);
hfc_node* huffman_make_tree(huffman_code** hfc_array, int *n, int offset);
int ee_compare_by_wrapped_character(const void *a, const void *b, void *udata);
uint64_t ee_hash_by_wrapped_character(const void *item, uint64_t seed0, uint64_t seed1);
bool ee_iter(const void *item, void *udata);
encoder_entry* ee_init(huffman_code* hfc);
void huffman_make_coder(struct hashmap* map, hfc_node* hfcn, const char* code, int depth);
coder* huffman_make_encoder(char* src);
int huffman_compress(const char* filename);
int* huffman_read_frequency_chart(huffman_code** hfc_array, char* src, int* text_start_i);
hfc_node* huffman_make_decoder(char* src, int* text_start_i);
char huffman_tree_find_char(hfc_node* root, text_tape* tape);
void huffman_assert_file_extension(const char* filename, const char* ext);
int huffman_decompress(const char* filename);
int huffman_run(huffman_option option, const char* filename);

#endif //HUFFMANENCODING_HUFFMAN_H