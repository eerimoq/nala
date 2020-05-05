#include "dummy_functions.h"

#include <stdio.h>

DummyStruct *edit_number(DummyStruct *dummy_struct, int number)
{
    dummy_struct->number = number;
    return dummy_struct;
}
