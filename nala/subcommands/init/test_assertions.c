#include "nala.h"

TEST(assertions)
{
    int array[] = { 1, 5, 2 };

    ASSERT_EQ(NULL, NULL);
    ASSERT_NE(1, 2);
    ASSERT_LT(1.0, 2.0);
    ASSERT_LE(1, 1);
    ASSERT_GT(2L, 1L);
    ASSERT_GE(1, 1);
    ASSERT_TRUE(1 == 1);
    ASSERT_FALSE(2 == 1);
    ASSERT_SUBSTRING("12345", "34");
    ASSERT_NOT_SUBSTRING("12345", "4567");
    ASSERT_MEMORY_EQ("abcd", "abcd", 5);
    ASSERT_ARRAY_EQ(array, array, sizeof(array));
    ASSERT_FUNCTION_POINTER_EQ(assertions, assertions);
    ASSERT_FUNCTION_POINTER_NE(assertions, NULL);

    CAPTURE_OUTPUT(output, errput) {
        printf("output!\n");
        fprintf(stderr, "errput!\n");
    }

    ASSERT_EQ(output, "output!\n");
    ASSERT_EQ(errput, "errput!\n");
}
