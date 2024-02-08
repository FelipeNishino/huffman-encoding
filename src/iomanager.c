#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "iomanager.h"

void
iomanager_raise_error(const char* err_str, ...) {
    va_list args;
    va_start(args, err_str);
    char* format = "[iomanager.c]: %s\n";

    char* str = malloc((strlen(format) + strlen(err_str) + 8) * sizeof(char));
    sprintf(str, format, err_str);

    vfprintf(stderr, str, args);

    va_end(args);
    free(str);
    exit(1);
}

void
iomanager_assert_file_extension(const char* filename, const char* ext) {
    int filename_size = strlen(filename);
    int etx_size = strlen(ext);
    const char* extension = &filename[filename_size - etx_size];
    if (strcmp(extension, ext) != 0)
        iomanager_raise_error("Invalid extension for given file, expected '%s'.\n", ext);
}

char *
iomanager_getInputFromFile(const char *filename){ 
    FILE *fp;
    char basename[20] = "./";
    char lineBuffer[256];
    int currentBufferSize = 1;
    char *input = calloc(1, sizeof(char));

    // Abre o arquivo com o caminho ".\filename"
    strcat(basename, filename);
    fp = fopen(basename, "r");
    // Checa se o fopen foi bem sucedido
    if (!fp)
        iomanager_raise_error("Couldn't find file named '%s'", filename);

    
    // printf("Opened input file successfully, reading contents...\n");

    while (fgets(lineBuffer, 256, fp)) {
        currentBufferSize += strlen(lineBuffer);

        input = realloc(input, currentBufferSize * sizeof(char));
        if (input == NULL) {
            printf("Error during memory reallocation.\n");
            return NULL;
        }

        strcat(input, lineBuffer);
    }

    fclose(fp);

    return input;
}

char *
iomanager_getInputFromStdin() {
    int nChars = 0, bufferLimit = 1024;
    char currentChar;
    char auxStr[2] = {0};
    char *buffer = calloc(STDIN_BUFFER_SIZE, sizeof(char));
    
    while (1) {
        currentChar = getchar();
        nChars++;

        if (nChars > 1 && currentChar == 10 && buffer[nChars-2] == 10) {
            break;
        }
        if (nChars == bufferLimit) {
            bufferLimit += 1024;
            if (realloc(buffer, bufferLimit * sizeof(char)) == NULL) {
                printf("Error during memory reallocation.\n");
                return NULL;
            }
        }
        
        auxStr[0] = currentChar;
        strcat(buffer, auxStr);
    }
    return buffer;
}
