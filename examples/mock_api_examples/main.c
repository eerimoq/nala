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

static void assert_foo_string(struct foo_t *actual_p,
                              struct foo_t *expected_p,
                              size_t size)
{
    ASSERT_EQ(size, sizeof(*expected_p));
    ASSERT_EQ(actual_p->string_p, expected_p->string_p);
}

TEST(fum_in_out)
{
    int value;
    struct foo_t foo;

    fum_mock_once();

    /* Expect *value_p to be 1 when fum() is called, and assign 2 to
       it before returning. */
    value = 1;
    fum_mock_set_value_p_in(&value, sizeof(value));
    value = 2;
    fum_mock_set_value_p_out(&value, sizeof(value));

    /* Use a custom parameter assert function to check that
       foo_p->string_p is "Hello!"  when fum() is called. */
    foo.string_p = "Hello!";
    fum_mock_set_foo_p_in(&foo, sizeof(foo));
    fum_mock_set_foo_p_in_assert(assert_foo_string);

    value = 1;
    foo.string_p = "Hello!";
    fum(&value, &foo);
    ASSERT_EQ(value, 2);
}

TEST(foo_ignore_value)
{
    foo_mock_once(1, 2);
    foo_mock_ignore_value_in();

    ASSERT_EQ(foo(9), 2);
}
