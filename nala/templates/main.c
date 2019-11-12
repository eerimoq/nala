#include <time.h>
#include "nala.h"
#include "__mocks__.h"

TEST(assertions)
{
    ASSERT_EQ(NULL, NULL);
    ASSERT_NE(1, 2);
    ASSERT_LT(1.0, 2.0);
    ASSERT_LE(1, 1);
    ASSERT_GT(2L, 1L);
    ASSERT_GE(1, 1);
    ASSERT_SUBSTRING("12345", "34");
    ASSERT_NOT_SUBSTRING("12345", "4567");
    ASSERT_MEMORY("abcd", "abcd", 5);
    ASSERT(1 == 1);

    CAPTURE_OUTPUT(stdoutput, stderrput) {
        printf("std!\n");
        fprintf(stderr, "err!\n");
    }

    ASSERT_EQ(stdoutput, "std!\n");
    ASSERT_EQ(stderrput, "err!\n");
}

TEST(mock_time)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}
