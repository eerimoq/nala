#include <errno.h>
#include <stdio.h>
#include "foo.h"
#include "nala.h"

TEST(foo_every_call)
{
    foo_mock(1, 2);

    /* All calls to foo() expects its parameter to be 1 and returns 2. */
    ASSERT_EQ(foo(1), 2);
    ASSERT_EQ(foo(1), 2);
}

TEST(foo_per_call)
{
    foo_mock_once(1, 2);
    foo_mock_once(4, 5);

    /* First call to foo() expects its parameter to be 1 and returns 2. */
    ASSERT_EQ(foo(1), 2);

    /* Second call to foo() expects its parameter to be 4 and returns 5. */
    ASSERT_EQ(foo(4), 5);

    /* Third call will fail and the test will end. */
    foo(10);
}

TEST(foo_set_errno)
{
    foo_mock_once(1, 2);
    foo_mock_set_errno(EINVAL);

    ASSERT_EQ(foo(1), 2);
    ASSERT_EQ(errno, EINVAL);
}

static void fie(void)
{
    printf("fie() called!\n");
}

TEST(bar_get_params_call_callback)
{
    int handle;

    handle = bar_mock_once();

    bar(fie);

    /* Call the callback (calls fie()). */
    bar_mock_get_params_in(handle)->callback();
}

TEST(fum_in_out)
{
    int value;

    fum_mock_once();
    value = 1;
    fum_mock_set_value_p_in(&value, sizeof(value));
    value = 2;
    fum_mock_set_value_p_out(&value, sizeof(value));

    value = 1;
    fum(&value);
    ASSERT_EQ(value, 2);
}

TEST(foo_ignore_value)
{
    foo_mock_once(1, 2);
    foo_mock_ignore_value_in();

    ASSERT_EQ(foo(9), 2);
}
