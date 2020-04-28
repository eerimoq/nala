#include "nala.h"

static void set_to_zero(int *value_p)
{
    *value_p = 0;
}

TEST(failing_test)
{
    FAIL("Just fail!\n");
}

TEST(segfault)
{
    set_to_zero(NULL);
}

TEST(foo)
{
}
