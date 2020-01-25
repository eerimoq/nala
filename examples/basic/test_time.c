#include <time.h>
#include "nala.h"
#include "nala_mocks.h"

TEST(mock_time)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}
