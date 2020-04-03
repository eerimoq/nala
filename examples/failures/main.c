#include <stdlib.h>
#include <time.h>
#include <sys/mount.h>
#include <fcntl.h>
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

TEST(time_wrong_pointer_once)
{
    time_t now;

    time_mock_once(42);
    time_mock_set_tloc_in_pointer(&now);

    ASSERT_EQ(time(NULL), 42);
    time(NULL);
}

TEST(time_wrong_in_once)
{
    time_t now;

    time_mock_once(42);
    now = 5;
    time_mock_set_tloc_in(&now, sizeof(now));

    now = 4;
    ASSERT_EQ(time(&now), 42);
    time(NULL);
}

TEST(time_wrong_pointer)
{
    time_t now;

    time_mock(42);
    time_mock_set_tloc_in_pointer(&now);

    ASSERT_EQ(time(NULL), 42);
    time(NULL);
}

TEST(time_wrong_in)
{
    time_t now;

    time_mock(42);
    now = 5;
    time_mock_set_tloc_in(&now, sizeof(now));

    now = 4;
    ASSERT_EQ(time(&now), 42);
    time(NULL);
}

TEST(time_none)
{
    time_t now;

    time_mock_none();
    time(&now);
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

TEST(out_of_bounds)
{
    int a[2];

    a[2] = 1;
}

TEST(memory_leak)
{
    malloc(1);
}

TEST(double_free)
{
    void *buf_p;

    buf_p = malloc(1);
    ASSERT_NE(buf_p, NULL);
    free(buf_p);
    free(buf_p);
}

TEST(eq)
{
    ASSERT_EQ(1, 2);
}

TEST(fail)
{
    FAIL("The failure message.");
}

TEST(strings_eq)
{
    ASSERT_EQ("The\n"
              "first\n"
              "string.\n",
              "The\n"
              "second\n"
              "string.\n");
}

TEST(substring)
{
    ASSERT_SUBSTRING("The\n"
                     "first\n"
                     "string.\n",
                     "second\n"
                     "string.\n");
}

TEST(mock_string_param)
{
    mount_mock_once("fie", "foo", "bar", 0, 0);

    mount("foo", "fie", "bar", 0, NULL);
}

TEST(mock_integer_param)
{
    mount_mock_once("foo", "fie", "bar", 1, 0);

    mount("foo", "fie", "bar", 0, NULL);
}

TEST(int_array)
{
    int a[] = { 1, 4, 3 };
    int b[] = { 1, 2, 3 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(int_array_long)
{
    int a[] = { 1, 2, 3, 4, 16, 16, 17, 8, 9 };
    int b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(float_array)
{
    float a[] = { 1.1, 4.4, 3.3 };
    float b[] = { 1.1, 2.2, 3.3 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

struct struct_array_t {
    int a;
};

TEST(struct_array)
{
    struct struct_array_t a[3] = { { 1 }, { 4 }, { 3 } };
    struct struct_array_t b[3] = { { 1 }, { 2 }, { 3 } };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(pointer_array)
{
    int a;
    int b;
    void *c[3] = { &a, &b, &a };
    void *d[3] = { &a, &b, &b };

    ASSERT_ARRAY(c, d, sizeof(c));
}

TEST(char_array)
{
    char a[] = "134";
    char b[] = "123";

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(variadic_function_open)
{
    open_mock("hello", 0, 0, "%d", 1);
    open("hello", 0, 2);
}

TEST(variadic_function_open_null_format)
{
    open_mock("hello", 0, 0, NULL);
}
