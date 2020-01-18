#include <time.h>
#include "nala.h"
#include "nala_mocks.h"

TEST(time_called_more_times_than_expected)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
    time(NULL);
}

TEST(time_called_fewer_times_than_expected)
{
    time_mock_once(41);
    time_mock_once(42);
    time_mock_once(43);

    ASSERT_EQ(time(NULL), 41);
}

TEST(time_wrong_pointer)
{
    time_t now;

    time_mock_once(42);
    time_mock_set_tloc_in_pointer(&now);

    ASSERT_EQ(time(NULL), 42);
    time(NULL);
}

TEST(segfault)
{
    (*((int *)0)) = 1;
}
