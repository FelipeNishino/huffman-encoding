#include <stdio.h>
#include <stdlib.h>
#include "verbose.h"
#include "huffman.h"
#include "huffman_array_utils.h"
#include "macros.h"

hfc_node* 
hfc_init_node(huffman_code* hfc) {
    hfc_node *temp = (hfc_node*)malloc(sizeof(hfc_node));
    temp->hfc = (hfc ? hfc : hfc_init('\0'));
    temp->l = NULL;
    temp->r = NULL;
    return temp;
}

node* 
node_init(hfc_node* hfc_node) {
    node* temp = calloc(1, sizeof(node));
    temp->next = NULL;
    temp->hfcn = hfc_node;
    return temp;
}

void 
node_display(node* current) {
    if (!current) return;
    do {
        hfc_print(current->hfcn->hfc);
        current = current->next;
    } while(current);
}

void 
huffman_print_tree(hfc_node* hfcn, int tab, int spacing) { 
    for(int i = 0; i < tab; i++){
        if (i < tab - (spacing - 1)) printf("%c", ((i % spacing) ? ' ' : '|'));
        else printf("-");
    }
    if (hfcn->hfc->character == 10) printf("char {%d} - f {%d}\n", hfcn->hfc->character, hfcn->hfc->frequency);
    else printf("char {%c} - f {%d}\n", hfcn->hfc->character, hfcn->hfc->frequency);
  
    if (hfcn->l) huffman_print_tree(hfcn->l, tab+spacing, spacing);
    if (hfcn->r) huffman_print_tree(hfcn->r, tab+spacing, spacing);
}

void 
node_insert_sorted(node* current, hfc_node* hfcn) {
    node* newNode = node_init(hfcn);
    node* prev;
    while (current && current->hfcn->hfc->frequency < hfcn->hfc->frequency) {
        prev = current;
        current = current->next;
    }

    prev->next = newNode;
    newNode->next = current;
}

hfc_node*
huffman_make_tree(huffman_code** hfc_array, int *n, int offset) {
    int i;
    node *head, *tail;
    head = calloc(1, sizeof(node));
        
    if (!n) {
        n = calloc(1, sizeof(int));
        *n = huffman_count_hfc_array(hfc_array);
    }
    if (offset < 0) offset = ASCII_MAX - *n;

    for (i = 0; i < *n; ++i) {
        if (!head->hfcn) {
            head = node_init(hfc_init_node(hfc_array[i + offset]));
            tail = head;  
        } 
        else {
            tail->next = node_init(hfc_init_node(hfc_array[i + offset]));
            tail = tail->next;
        }
    }

    while(head->next) {
        hfc_node* blank_hfcn = hfc_init_node(NULL);
        blank_hfcn->l = head->hfcn;
        blank_hfcn->r = head->next->hfcn;

        blank_hfcn->hfc->frequency = blank_hfcn->l->hfc->frequency + blank_hfcn->r->hfc->frequency;

        node_insert_sorted(head, blank_hfcn);

        head = head->next->next;

        if (is_verbose()) {
            printf("\n");
            node_display(head);
            printf("\n");
        }
    }

    if (is_verbose()) huffman_print_tree(head->hfcn, 0, 4);
    return head->hfcn;
}

char
huffman_tree_find_char(hfc_node* root, text_tape* tape) {
    while(*tape->head) {
        root = (*tape->head++ == '1' ? root->r : root->l);
        if (root->hfc->character) {
            return root->hfc->character;
        }
    }

    panic("Didn't find corresponding character in huffman tree.");
}
