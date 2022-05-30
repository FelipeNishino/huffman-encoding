#define panic(_msg_) { \
    fprintf(stderr, "panic: %s (%s:%d)\n", (_msg_), __FILE__, __LINE__); \
    exit(1); \
}

#define error(_msg_) { \
    fprintf(stderr, "[huffman]: %s \n", (_msg_)); \
    exit(1); \
}

#define OPTION_DECOMPRESS "decompress"
#define OPTION_DECOMPRESS_SIZE 11
#define OPTION_COMPRESS "compress"
#define OPTION_COMPRESS_SIZE 9
