#ifndef TEST_DUMMY_FUNCTIONS_H
#define TEST_DUMMY_FUNCTIONS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct DummyStruct
{
    int number;
} DummyStruct;

DummyStruct *edit_number(DummyStruct *dummy_struct, int number);

#endif
