#include "foo.h"

int foo(int value)
{
    return (value + 1);
}

void bar(callback_t callback)
{
    callback();
}

void fum(int *value_p, struct foo_t *foo_p)
{
    (void)foo_p;

    *value_p = 10;
}
