#include "foo.h"
#include "nala.h"

TEST(foo_no_mocks)
{
    ASSERT_EQ(myfunction(1, 2), 5);
}
