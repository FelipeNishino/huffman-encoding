#ifndef COMPILER_IOMANAGER_H
#define COMPILER_IOMANAGER_H

#define BUFFER_SIZE 256
#define STDIN_BUFFER_SIZE 1024

char *iomanager_getInputFromFile(const char *filename);
char *iomanager_getInputFromStdin();

#endif //COMPILER_IOMANAGER_H
