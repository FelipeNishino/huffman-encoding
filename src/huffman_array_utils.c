#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "verbose.h"

huffman_code** hfc_array_init(int n) {
    huffman_code **arr = (huffman_code**)malloc(n* sizeof(huffman_code*));
    for (int i = 0; i < n; ++i) arr[i] = calloc(1, sizeof(huffman_code));
    return arr;
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

int huffman_count_hfc_array(huffman_code** array) {
    int i = 0;
    while(!array[i]->character) i++;
    return ASCII_MAX - i;
}