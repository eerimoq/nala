#include "nala.h"

int foo(int __a)
{
    return (__a);
}

TEST(mock_foo)
{
    foo_mock(0, 0);
    foo_mock_ignore___a_in();
    foo();
}
