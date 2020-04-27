#include <time.h>
#include "nala.h"

TEST(mock_time)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}
