#include <stdio.h>
#include <stdarg.h>

void debug(const char * format, ...) {
    
    #ifdef TEST
        va_list argptr;
        va_start(argptr, format);

        vprintf(format, argptr);

        va_end(argptr);
    #endif

}
