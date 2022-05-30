#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/huffman.h"
#include "include/macros.h"
#include "include/verbose.h"

int main(int argc, const char** argv) {
	huffman_option option;

	if (strncmp(argv[1], OPTION_DECOMPRESS, OPTION_DECOMPRESS_SIZE) == 0) option = decompress;
	else if (strncmp(argv[1], OPTION_COMPRESS, OPTION_COMPRESS_SIZE) == 0) option = compress;
	else error("Choose a valid option[compress/decompress].");
	
	if (argc < 3) error("Missing file path.");

	for(int i = 3; i < argc; ++i) {
		if (strcmp(argv[i], "-v") == 0) set_verbose(1);
		else {
			fprintf(stderr, "[huffman:main.c]: Unrecognized argument '%s'.\n", argv[i]);
    		exit(1);
		}
	}

	return huffman_run(option, argv[2]);
}

