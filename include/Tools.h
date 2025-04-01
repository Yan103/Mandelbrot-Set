#ifndef TOOLS_H
#define TOOLS_H

#include <assert.h>
#include <stdlib.h>

#define END         "\033[0;0m"                             //* Add colour outputs in my program :)
#define RED_CLR     "\033[1;31m"
#define GREEN_CLR   "\033[1;32m"
#define RED(str)     RED_CLR     str END
#define GREEN(str)   GREEN_CLR   str END

#define FREE(object) { free(object); (object) = NULL; }     //* Proper release of dynamic memory

typedef enum ReturnCodes {
    SUCCESS =  0,
    ERROR   = -1,
} ReturnCodes;

#define NULL_CHECK(object) {                                      \
    if (object == NULL) {                                         \
        fprintf(stderr, RED("NULL POINTER WAS RECEIVED!\n"));     \
        return NULL_PTR_ERROR;                                    \
    }                                                             \
}

#endif // TOOLS_H