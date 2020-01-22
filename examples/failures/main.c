#include <time.h>
#include "nala.h"
#include "nala_mocks.h"
#include "foo.h"

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

TEST(time_wrong_in)
{
    time_t now;

    time_mock_once(42);
    now = 5;
    time_mock_set_tloc_in(&now, sizeof(now));

    now = 4;
    ASSERT_EQ(time(&now), 42);
    time(NULL);
}

TEST(foo_wrong_in)
{
    struct foo_t value;

    value.a = 1;
    value.b = -1;
    foo_mock_once(0);
    foo_mock_set_foo_p_in(&value, sizeof(value));

    value.a = 1;
    value.b = -2;
    ASSERT_EQ(foo(&value), 0);
}

TEST(segfault)
{
    (*((int *)0)) = 1;
}

TEST(eq)
{
    ASSERT_EQ(1, 2);
}
