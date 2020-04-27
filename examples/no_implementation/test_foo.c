#include "foo.h"
#include "nala.h"

TEST(mock_function_without_implementation)
{
    foo_add_mock_once(0, 1, 1);

    ASSERT_EQ(foo_add(0, 1), 1);
}
