#include <stdio.h>

void foo()
{
    printf("foo()\n");
}

void bar()
{
    printf("bar()\n");
    foo();
}

void fie()
{
    printf("fie()\n");
    foo();
    bar();
}
