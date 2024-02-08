#ifndef COMPILER_IOMANAGER_H
#define COMPILER_IOMANAGER_H

#define BUFFER_SIZE 256
#define STDIN_BUFFER_SIZE 1024

void iomanager_raise_error(const char* err_str, ...);
void iomanager_assert_file_extension(const char* filename, const char* ext);
char *iomanager_getInputFromFile(const char *filename);
char *iomanager_getInputFromStdin();

#endif //COMPILER_IOMANAGER_H
