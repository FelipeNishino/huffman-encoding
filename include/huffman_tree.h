#ifndef HUFFMANENCODING_TREE_H
#define HUFFMANENCODING_TREE_H

typedef struct hfc_node {
    huffman_code* hfc;
    struct hfc_node *l;
  	struct hfc_node *r;
} hfc_node;

typedef struct node {
	struct node* next;
	hfc_node* hfcn;
} node;

hfc_node* hfc_init_node(huffman_code* hfc);
node* node_init(hfc_node* hfc_node);
void node_display(node* current);
void huffman_print_tree(hfc_node* hfcn, int tab, int spacing);
void node_insert_sorted(node* current, hfc_node* hfcn);
hfc_node* huffman_make_tree(huffman_code** hfc_array, int *n, int offset);
char huffman_tree_find_char(hfc_node* root, text_tape* tape);

#endif //HUFFMANENCODING_TREE_H