#ifndef HUFFMANENCODING_ARRAY_UTILS_H
#define HUFFMANENCODING_ARRAY_UTILS_H

huffman_code** hfc_array_init(int n);
void hfc_display(huffman_code** arr, int n);
void hfc_quicksort_swap(huffman_code* a, huffman_code* b);
int hfc_quicksort_partition(huffman_code** array, int left, int right, int pivot, hfc_sort_option option);
void hfc_quicksort(huffman_code** array, int left, int right, hfc_sort_option option);
void hfc_sort_subsection_by_char(huffman_code** array, int n);
int huffman_count_hfc_array(huffman_code** array);

#endif //HUFFMANENCODING_ARRAY_UTILS_H