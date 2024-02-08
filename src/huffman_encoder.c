#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "huffman_encoder.h"

int
ee_compare_by_wrapped_character(const void *a, const void *b, void *udata) {
    const encoder_entry *eea = a;
    const encoder_entry *eeb = b;
    return strcmp(eea->str, eeb->str);
}

uint64_t
ee_hash_by_wrapped_character(const void *item, uint64_t seed0, uint64_t seed1) {
    const encoder_entry *ee = item;
    return hashmap_sip(ee->str, strlen(ee->str), seed0, seed1);
}

bool
ee_iter(const void *item, void *udata) {
    const encoder_entry *ee_item = item;
    printf("str{%s}, char{%d}: (code={%s})\n", ee_item->str, ee_item->hfc->character, ee_item->hfc->code);
    return 1;
}

encoder_entry*
ee_init(huffman_code* hfc) {
    encoder_entry* ee = (encoder_entry*) malloc(sizeof(encoder_entry));
    ee->hfc = hfc;
    ee->str = calloc(2, sizeof(char));
    ee->str[0] = ee->hfc->character;
    return ee;        
}