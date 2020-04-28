#include "nala.h"

int foo(int __a, int __)
{
    return (__a + __);
}

TEST(mock_foo)
{
    foo_mock(0, 0, 0);
    foo_mock_ignore_a_in();
    foo_mock_ignore____in();
    foo();
}
