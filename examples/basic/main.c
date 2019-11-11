#include <time.h>

#include "__mocks__.h"
#include "narwhal.h"

TEST(example)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}
