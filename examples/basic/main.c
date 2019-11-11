#include <time.h>
#include "nala.h"
#include "__mocks__.h"

TEST(example)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}
