#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "include/verbose.h"
#include "include/huffman.h"
#include "include/iomanager.h"
#include "include/string_to_int.h"
#include "include/hashmap.h"
#include "include/macros.h"

void hfc_print(huffman_code *hfc) {
    printf("<char={%c}, int_char={%d}, f={%d}, code={%s})>\n", hfc->character, hfc->character, hfc->frequency, hfc->code);
}

void hfc_display(huffman_code** arr, int n) {
    int i;
    printf("[\n");
    
    for(i = 0; i < n; i++) hfc_print(arr[i]);
    
    printf("]\n");
}

void hfc_quicksort_swap(huffman_code* a, huffman_code* b) {
    huffman_code* t = &(huffman_code){.frequency = a->frequency, .character = a->character, .code=a->code};
    *a = *b;
    *b = *t;
}

int hfc_quicksort_partition(huffman_code** array, int left, int right, int pivot, hfc_sort_option option) {
    int left_i = left -1;
    int right_i = right;

    while(1) {
        while((option == by_freq ? array[++left_i]->frequency : array[++left_i]->character) < pivot) continue;
        while(right_i > 0 && (option == by_freq ? array[--right_i]->frequency : array[--right_i]->character) > pivot) continue;

        if(left_i >= right_i) break;
        else hfc_quicksort_swap(array[left_i], array[right_i]);
    }
    
    hfc_quicksort_swap(array[left_i],array[right]);
    return left_i;
}

void hfc_quicksort(huffman_code** array, int left, int right, hfc_sort_option option) {
    if (right-left <= 0) return;
    else {
        int pivot = (option == by_freq ? array[right]->frequency : array[right]->character);
        int partitionPoint = hfc_quicksort_partition(array, left, right, pivot, option);
        hfc_quicksort(array, left,partitionPoint-1, option);
        hfc_quicksort(array, partitionPoint+1,right, option);
    }        
}

void hfc_sort_subsection_by_char(huffman_code** array, int n) {
    int i = 0, j = 0, current = 1;
    while(i < n && j < n) {
        if (array[i]->frequency < current) i++;
        else {
            if (j < i) j = i;
            if (array[j]->frequency <= current) j++;
            else {
                verbose("Vai fazer o sort de %d a %d, chars %c a %c, \n", i, j-1, array[i]->character, array[j-1]->character);
                hfc_quicksort(array, i, j - 1, by_char);
                i = j;
                current++;
            }
        }
    }
}

huffman_code* hfc_init(char c) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = 0;
    temp->character = c;
    temp->code = NULL;
    return temp;
}

huffman_code* hfc_init_with_params(int freq, char c, const char* code) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = freq;
    temp->character = c;
    temp->code = code;
    return temp;
}

huffman_code* hfc_copy(huffman_code* hfc) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = hfc->frequency;
    temp->character = hfc->character;
    temp->code = hfc->code;
    return temp;
}

huffman_code** hfc_array_init(int n) {
    huffman_code **arr = calloc(n, sizeof(huffman_code*));
    for (int i = 0; i < n; ++i) arr[i] = calloc(1, sizeof(huffman_code));
    return arr;
}

huffman_code** hfc_generate_frequency_chart(char* src) {
    int i = 0; int j = 0;

    huffman_code **hfc_array = hfc_array_init(ASCII_MAX);

    while(src[j]) {
        i = (src[j] == 10 ? 95 : src[j] - ASCII_OFFSET);
        if (!hfc_array[i]->character) {
            hfc_array[i] = hfc_init(src[j]);
        } else {
        }
        hfc_array[i]->frequency++;
        j++;
    }

    hfc_quicksort(hfc_array, 0, ASCII_MAX - 1, by_freq);
    hfc_sort_subsection_by_char(hfc_array, ASCII_MAX);

    return hfc_array;
}

int huffman_count_hfc_array(huffman_code** array) {
    int i = 0;
    while(!array[i]->character) i++;
    return ASCII_MAX - i;
}

hfc_node* hfc_init_blank_node() {
    hfc_node *temp = calloc(1, sizeof(hfc_node));
    temp->hfc = hfc_init('\0');
    temp->l = NULL;
    temp->r = NULL;
    return temp;
}

hfc_node* hfc_init_node(huffman_code* hfc) {
    hfc_node *temp = calloc(1, sizeof(hfc_node));
    temp->hfc = hfc;
    temp->l = NULL;
    temp->r = NULL;
    return temp;
}

node* node_init(hfc_node* hfc_node) {
    node* temp = calloc(1, sizeof(node));
    temp->next = NULL;
    temp->hfcn = hfc_node;
    return temp;
}

void node_display(node* current) {
    if (!current) return;
    do {
        hfc_print(current->hfcn->hfc);
        current = current->next;
    } while(current);
}

void huffman_print_tree(hfc_node* hfcn, int tab, int spacing) { 
    for(int i = 0; i < tab; i++){
        if (i < tab - (spacing - 1)) printf("%c", ((i % spacing) ? ' ' : '|'));
        else printf("-");
    }
    if (hfcn->hfc->character == 10) printf("char {%d} - f {%d}\n", hfcn->hfc->character, hfcn->hfc->frequency);
    else printf("char {%c} - f {%d}\n", hfcn->hfc->character, hfcn->hfc->frequency);
  
    if (hfcn->l) huffman_print_tree(hfcn->l, tab+spacing, spacing);
    if (hfcn->r) huffman_print_tree(hfcn->r, tab+spacing, spacing);
}

void node_insert_sorted(node* current, hfc_node* hfcn) {
    node* newNode = node_init(hfcn);
    node* prev;
    while (current && current->hfcn->hfc->frequency < hfcn->hfc->frequency) {
        prev = current;
        current = current->next;
    }

    prev->next = newNode;
    newNode->next = current;
}

hfc_node* huffman_make_tree(huffman_code** hfc_array, int *n, int offset) {
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
        hfc_node* blank_hfcn = hfc_init_blank_node();
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

int ee_compare_by_wrapped_character(const void *a, const void *b, void *udata) {
    const encoder_entry *eea = a;
    const encoder_entry *eeb = b;
    return strcmp(eea->str, eeb->str);
}

uint64_t ee_hash_by_wrapped_character(const void *item, uint64_t seed0, uint64_t seed1) {
    const encoder_entry *ee = item;
    return hashmap_sip(ee->str, strlen(ee->str), seed0, seed1);
}

bool ee_iter(const void *item, void *udata) {
    const encoder_entry *ee_item = item;
    printf("str{%s}, char{%d}: (code={%s})\n", ee_item->str, ee_item->hfc->character, ee_item->hfc->code);
    return 1;
}

encoder_entry* ee_init(huffman_code* hfc) {
    encoder_entry* ee = (encoder_entry*) malloc(sizeof(encoder_entry));
    ee->hfc = hfc;
    ee->str = calloc(2, sizeof(char));
    ee->str[0] = ee->hfc->character;
    return ee;        
}

void huffman_make_coder(struct hashmap* map, hfc_node* hfcn, const char* code, int depth){
    if (!hfcn->r && !hfcn->l) {
        encoder_entry* ee = ee_init(hfc_init_with_params(hfcn->hfc->frequency, hfcn->hfc->character, code));
        hashmap_set(map, ee);
    }

    char* lcode = malloc((depth + 1) * sizeof(char));
    char* rcode = malloc((depth + 1) * sizeof(char));
    if (hfcn->l) {
        if (depth == 1) lcode = "0";
        else {
            strcpy(lcode, code);
            strcat(lcode, "0");
        }
        huffman_make_coder(map, hfcn->l, lcode, depth+1);
    }
    if (hfcn->r) {        
        if (depth == 1) rcode = "1";
        else {
            strcpy(rcode, code);
            strcat(rcode, "1");
        }
        huffman_make_coder(map, hfcn->r, rcode, depth+1);
    }
}

coder* huffman_make_encoder(char* src) {
    coder *encoder = hashmap_new(
        sizeof(encoder_entry),
        0,
        0,
        0,
        ee_hash_by_wrapped_character,
        ee_compare_by_wrapped_character,
        NULL,
        NULL);

    huffman_code** hfc_array = hfc_generate_frequency_chart(src);
    hfc_node* tree = huffman_make_tree(hfc_array, NULL, -1);
    huffman_make_coder(encoder, tree, NULL, 1);

    return encoder;
}

int huffman_compress(const char* filename) {
    huffman_assert_file_extension(filename, ".txt");
    char* input = iomanager_getInputFromFile(filename);
    char* intstr;
    char compressed_name[130] = "\0";
    char str[2] = "\0";
    char prevchar;
    int length;
    int i = 0;
    void *item;
    size_t iter = 0;
    coder* encoder = huffman_make_encoder(input);
    encoder_entry* ee;
    FILE *compressed;

    printf("Compressing file '%s'.\n", filename);

    // Abre o arquivo com o caminho ".\filename"
    strncpy(compressed_name, filename, 127);
    strcat(compressed_name, ".he");
    compressed = fopen(compressed_name, "w");

    // Checa se o fopen foi bem sucedido
    if (!compressed)  {
        printf("Error opening file %s.\n", compressed_name);
        exit(1);
    }

    while (hashmap_iter(encoder, &iter, &item)) {
        ee = (encoder_entry*) item;
        fputc(ee->hfc->character, compressed);
        length = snprintf(NULL, 0,"%d",42);
        intstr = calloc(length, sizeof(char));
        sprintf(intstr, "%d", ee->hfc->frequency);
        fputs(intstr, compressed);
        fputc('|', compressed);
        prevchar = ee->hfc->character;
        verbose("char %c, code {%s}\n", ee->hfc->character, ee->hfc->code);
    }

    fputc(prevchar, compressed);

    while(input[i]) {
        str[0] = input[i];
        ee = hashmap_get(encoder, &(encoder_entry){ .str=str });
        if (!ee) printf("Codigo nao encontrado\n");
        fputs(ee->hfc->code, compressed);
        i++;
    }

    printf("Compressed file written to '%s'.\n", compressed_name);

    fclose(compressed);
    hashmap_free(encoder);
    return 0;
}

int* huffman_read_frequency_chart(huffman_code** hfc_array, char* src, int* text_start_i) {
    char* intstr;
    char *numboundl, *numboundr;
    char* reader = &src[0];
    char prev;
    int* freq = (int*) malloc(sizeof(int));
    int step = 0;
    int i = 0;
    huffman_code* hfc = hfc_init('\0');

    while(1) {
        switch(step) {
            case 0:
                hfc->character = *(reader++);
                step++;
                break;
            case 1:
                numboundl = reader;
                while(isdigit(*++reader)) continue;
                numboundr = reader - sizeof(char);
                intstr = calloc(numboundr - numboundl + 2, sizeof(char));
                strncpy(intstr, numboundl, numboundr - numboundl + 1);
                switch(str2int(freq, intstr, 10)) {
                    case STR2INT_SUCCESS:
                        hfc->frequency = *freq;
                        step++;
                        break;
                    case STR2INT_OVERFLOW:
                    case STR2INT_UNDERFLOW:
                    case STR2INT_INCONVERTIBLE:
                    panic("Bad read of header. File might be corrupted");
                }
                break;
            case 2:
                prev = hfc->character;
                hfc_array[i++] = hfc_copy(hfc);
                step = (*(reader++ + sizeof(char)) != prev ? 0 : 3);
                break;
            case 3:
                *text_start_i = reader - src + 1;
                int *r = (int*) malloc(sizeof(int));
                *r = i;
                return r;
        }
    }
}

hfc_node* huffman_make_decoder(char* src, int* text_start_i) {
    huffman_code** hfc_array = hfc_array_init(ASCII_MAX);
    int* hfc_array_size = huffman_read_frequency_chart(hfc_array, src, text_start_i);
    
    hfc_quicksort(hfc_array, 0, ASCII_MAX - 1, by_freq);
    hfc_sort_subsection_by_char(hfc_array, ASCII_MAX);

    int arr_start = ASCII_MAX - *hfc_array_size;
    return huffman_make_tree(&hfc_array[arr_start], hfc_array_size, 0);
}

char huffman_tree_find_char(hfc_node* root, text_tape* tape) {
    while(*tape->head) {
        root = (*tape->head++ == '1' ? root->r : root->l);
        if (root->hfc->character) {
            return root->hfc->character;
        }
    }

    panic("Didn't find corresponding character in huffman tree.");
}

void huffman_assert_file_extension(const char* filename, const char* ext) {
    int filename_size = strlen(filename);
    int etx_size = strlen(ext);
    const char* extension = &filename[filename_size - etx_size];
    if (strcmp(extension, ext) != 0) error("Invalid extension for given file, expected '.he'.\n");
}

int huffman_decompress(const char* filename) {
    huffman_assert_file_extension(filename, ".he");
    int* text_start_i = calloc(1, sizeof(int));
    char* compressed_text = iomanager_getInputFromFile(filename);
    hfc_node* tree = huffman_make_decoder(compressed_text, text_start_i);

    printf("Decompressing file '%s'.\n", filename);

    text_tape* tape = calloc(1, sizeof(text_tape));
    tape->text = &compressed_text[*text_start_i];
    tape->head = &tape->text[0];
    
    FILE *decompressed;
    char decompressed_name[130] = "\0";

    // Abre o arquivo com o caminho ".\filename"
    strncpy(decompressed_name, filename, strlen(filename) - 3);
    decompressed = fopen(decompressed_name, "w");

    // Checa se o fopen foi bem sucedido
    if (!decompressed)  {
        printf("Error opening file %s.\n", decompressed_name);
        exit(1);
    }
    while (*tape->head) fputc(huffman_tree_find_char(tree, tape), decompressed);

    fclose(decompressed);

    printf("Decompressed file written to '%s'.\n", decompressed_name);
    
    free(compressed_text);
    return 0;
}

int huffman_run(huffman_option option, const char* filename) {
    switch(option) {
        case compress: return huffman_compress(filename);
        case decompress: return huffman_decompress(filename);
    }
    return -1;
}