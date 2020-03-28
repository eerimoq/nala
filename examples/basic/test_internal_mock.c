#include "nala.h"
#include "nala_mocks.h"

int foo(void)
{
    return (0);
}

TEST(internal_mock)
{
    foo_mock_once(1);
    ASSERT_EQ(foo(), 1);
}
