#include "nala.h"

TEST(assertions)
{
    int i;
    int array[] = { 1, 5, 2 };

    ASSERT_EQ(NULL, NULL);
    ASSERT_NE(1, 2);
    ASSERT_LT(1.0, 2.0);
    ASSERT_LE(1, 1);
    ASSERT_GT(2L, 1L);
    ASSERT_GE(1, 1);
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_SUBSTRING("12345", "34");
    ASSERT_NOT_SUBSTRING("12345", "4567");
    ASSERT_MEMORY_EQ("abcd", "abcd", 5);
    ASSERT_FILE_EQ("test_assertions.c", "test_assertions.c");
    ASSERT_ARRAY_EQ(array, array, sizeof(array));
    ASSERT_FUNCTION_POINTER_EQ(assertions, assertions);
    ASSERT_FUNCTION_POINTER_NE(assertions, NULL);
    ASSERT(1 == 1);

    CAPTURE_OUTPUT(output, errput) {
        printf("output!\n");
        fprintf(stderr, "errput!\n");
    }

    ASSERT_EQ(output, "output!\n");
    ASSERT_EQ(errput, "errput!\n");

    for (i = 0; i < 3; i++) {
        WITH_MESSAGE("i: %d", i) {
            ASSERT_EQ(array[i], array[i]);
        }
    }
}
