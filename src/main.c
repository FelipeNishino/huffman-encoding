#include <stdio.h>
#include "include/huffman.h"

int main(int argc, const char** argv) {
	if (argc != 1) {
		printf("Passe o nome do .txt como argumento");
	}

	huffman_encode_file(argv[1]);

	return 0;
}

