#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

typedef enum {
    SUCCESS,
    FAILURE
} Status;

typedef enum {
    VIEW_MODE,
    EDIT_MODE,
    e_unsupported
} OperationType;

#endif /* TYPES_H */

