#include "foo.h"

int foo(struct foo_t *foo_p)
{
    return (foo_p->a + foo_p->b);
}

void bar(struct bar_t *bar_p)
{
    (void)bar_p;
}
