#include "foo.h"
#include "nala.h"

static int my_square(int x)
{
    return x + 1;
}

/* Replace implemenation of square(). */
TEST(implementation)
{
    square_mock_implementation(my_square);

    ASSERT_EQ(myfunction(0, 0), 2);
}

/* Expect square(x=0) and return 1. */
TEST(mock)
{
    square_mock(0, 1);

    ASSERT_EQ(myfunction(0, 0), 2);
}

/* Expect square(x=0) and return 1 exactly twice. */
TEST(mock_once)
{
    square_mock_once(0, 1);
    square_mock_once(0, 1);

    ASSERT_EQ(myfunction(0, 0), 2);
}
