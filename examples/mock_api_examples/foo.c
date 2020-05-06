#include "foo.h"

int foo(int value)
{
    return (value + 1);
}

void bar(callback_t callback)
{
    callback();
}

void fum(int *value_p)
{
    *value_p = 10;
}
