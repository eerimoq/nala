#include "foo.h"
#include "nala.h"

TEST(foo_no_mocks)
{
    ASSERT_EQ(myfunction(1, 2), 5);
}

TEST(foo_mock_once)
{
    square_mock_once(1, 0);
    square_mock_once(2, 1);

    ASSERT_EQ(myfunction(1, 2), 1);
}
