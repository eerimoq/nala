#include "nala.h"

TEST(failing_test)
{
    FAIL("Just fail!\n");
}

TEST(segfault)
{
    *(int *)NULL = 0;
}

TEST(foo)
{
}
