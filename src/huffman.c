#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/huffman.h"
#include "include/iomanager.h"

int cfr_compare(const void *a, const void *b, void *udata) {
    const char_frequency_row *cfra = a;
    const char_frequency_row *cfrb = b;
    return strcmp(cfra->code, cfrb->code);
}

bool cfr_iter(const void *item, void *udata) {
    const char_frequency_row *cfr = item;
    printf("char{%d}: (f=%d;code={%s})\n", cfr->character, cfr->frequency, cfr->code);
    return true;
}

uint64_t cfr_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const char_frequency_row *cfr = item;
    return hashmap_sip(cfr->code, strlen(cfr->code), seed0, seed1);
}

int cfr_generate_frequency_chart(struct hashmap *map, char* src) {
    int i = 0;
    int *cf_array = calloc(95, sizeof(int));
    // total 95
    // comeco 32
    while(src[i]) {
        if (src[i] == 10) continue;
        // printf("Vai tentar incrementar a freq em %d para o char %d\n", src[i]-32, src[i]);
        cf_array[src[i]-32]++;
        i++;
    }

    for (i = 0; i < 95; ++i)
    {
        printf("char (%d,%c), f %d\n", i+32, i+32, cf_array[i]);
    }

    

    // hashmap_set(map, &(struct user){ .name="Dale", .age=44 });
    //     // user = hashmap_get(map, &(struct user){ .name="Jane" });

    // printf("\n-- iterate over all users (hashmap_scan) --\n");
    // hashmap_scan(map, cfr_iter, NULL);

    // printf("\n-- iterate over all users (hashmap_iter) --\n");
    // size_t iter = 0;
    // void *item;
    // while (hashmap_iter(map, &iter, &item)) {
    //     const struct user *user = item;
    //     printf("%s (age=%d)\n", user->name, user->age);
    // }
    // hashmap_free(map);
    return 0;
}

int huffman_encode_text(char* src) {
    struct hashmap *map = hashmap_new(
        sizeof(struct char_frequency_row),
        0,
        0,
        0,
        cfr_hash,
        cfr_compare,
        NULL,
        NULL);

    cfr_generate_frequency_chart(map, src);
    return 0;
}

int huffman_encode_file(const char* filename) {
    return huffman_encode_text(iomanager_getInputFromFile(filename));
}