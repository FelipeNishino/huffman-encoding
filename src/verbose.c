#include "include/verbose.h"
#include <stdarg.h>
#include <stdio.h>

int Verbose = 0;

void set_verbose(int setting) {
    Verbose = setting;
}

int is_verbose() {
	return Verbose;
}

void verbose(const char * restrict format, ...) {
    if (!Verbose) return;

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
