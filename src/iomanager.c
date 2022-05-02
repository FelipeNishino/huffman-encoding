#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/iomanager.h"

char *iomanager_getInputFromFile(const char *filename){ 
    FILE *fp;
    char basename[20] = "./";
    char lineBuffer[256];
    int currentBufferSize = 1;
    char *input = calloc(1, sizeof(char));

    // Abre o arquivo com o caminho ".\filename"
    strcat(basename, filename);
    fp = fopen(basename, "r");
    // Checa se o fopen foi bem sucedido
    if (!fp)  {
        printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
        exit(1);
    }
    
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

char *iomanager_getInputFromStdin() {
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
