#include <stdio.h>
#include <string.h>
// #include <stdlib.h>
// #include <ctype.h>
#include "iomanager.h"
#include "huffman.h"
#include "huffman_array_utils.h"
#include "huffman_encoder.h"
#include "huffman_tree.h"
#include "macros.h"
#include "verbose.h"
#include "string_to_int.h"

void
hfc_print(huffman_code *hfc) {
    printf("<char={%c}, int_char={%d}, f={%d}, code={%s})>\n", hfc->character, hfc->character, hfc->frequency, hfc->code);
}

huffman_code*
hfc_init(char c) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = 0;
    temp->character = c;
    temp->code = NULL;
    return temp;
}

huffman_code*
hfc_init_with_params(int freq, char c, const char* code) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = freq;
    temp->character = c;
    temp->code = code;
    return temp;
}

huffman_code*
hfc_copy(huffman_code* hfc) {
    huffman_code* temp = calloc(1, sizeof(huffman_code));
    temp->frequency = hfc->frequency;
    temp->character = hfc->character;
    temp->code = hfc->code;
    return temp;
}

huffman_code**
hfc_generate_frequency_chart(char* src) {
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


void
huffman_make_coder(struct hashmap* map, hfc_node* hfcn, const char* code, int depth){
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

int*
huffman_read_frequency_chart(huffman_code** hfc_array, char* src, int* text_start_i) {
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

coder*
huffman_make_encoder(char* src) {
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

int
huffman_compress(const char* filename) {
    iomanager_assert_file_extension(filename, ".txt");
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

hfc_node*
huffman_make_decoder(char* src, int* text_start_i) {
    huffman_code** hfc_array = hfc_array_init(ASCII_MAX);
    int* hfc_array_size = huffman_read_frequency_chart(hfc_array, src, text_start_i);
    
    hfc_quicksort(hfc_array, 0, ASCII_MAX - 1, by_freq);
    hfc_sort_subsection_by_char(hfc_array, ASCII_MAX);

    int arr_start = ASCII_MAX - *hfc_array_size;
    return huffman_make_tree(&hfc_array[arr_start], hfc_array_size, 0);
}

int
huffman_decompress(const char* filename) {
    iomanager_assert_file_extension(filename, ".he");
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

int
huffman_run(huffman_option option, const char* filename) {
    switch(option) {
        case compress: return huffman_compress(filename);
        case decompress: return huffman_decompress(filename);
    }
    return -1;
}