#include <sys/types.h>
#include "nala.h"
#include "subprocess.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#define B(...) ANSI_RESET ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define C(color, ...) ANSI_RESET ANSI_COLOR_##color __VA_ARGS__ ANSI_RESET
#define CR(...) C(RED, __VA_ARGS__)
#define CM(...) C(MAGENTA, __VA_ARGS__)

#define CB(color, ...)                                                  \
    ANSI_RESET ANSI_COLOR_##color ANSI_BOLD __VA_ARGS__ ANSI_RESET
#define CBR(...) CB(RED, __VA_ARGS__)

TEST(assert_eq)
{
    char string[] = "foo";
    const char const_string[] = "bar";

    ASSERT_EQ((char)'1', (char)'1');
    ASSERT_EQ((signed char)'1', (signed char)'1');
    ASSERT_EQ((unsigned char)'1', (unsigned char)'1');
    ASSERT_EQ((short)1, (short)1);
    ASSERT_EQ((unsigned short)1, (unsigned short)1);
    ASSERT_EQ((int)1, (int)1);
    ASSERT_EQ((unsigned int)1, (unsigned int)1);
    ASSERT_EQ((long)1, (long)1);
    ASSERT_EQ((unsigned long)1, (unsigned long)1);
    ASSERT_EQ((long long)1, (long long)1);
    ASSERT_EQ((unsigned long long)1, (unsigned long long)1);
    ASSERT_EQ((float)1, (float)1);
    ASSERT_EQ((double)1, (double)1);
    ASSERT_EQ((long double)1, (long double)1);
    ASSERT_EQ((bool)true, (bool)true);
    ASSERT_EQ((void *)NULL, (void *)NULL);
    ASSERT_EQ("", "");
    ASSERT_EQ("12", "12");
    ASSERT_EQ(string, "foo");
    ASSERT_EQ(const_string, "bar");
    ASSERT_EQ("foo", string);
    ASSERT_EQ("bar", const_string);
    ASSERT_EQ((size_t)1, (size_t)1);
    ASSERT_EQ((ssize_t)1, (ssize_t)1);
}

TEST(assert_ne)
{
    ASSERT_NE(1, 2);
    ASSERT_NE("1", "2");
    ASSERT_NE(NULL, "2");
    ASSERT_NE("1", NULL);
}

TEST(assert_lt)
{
    ASSERT_LT(0, 1);
}

TEST(assert_le)
{
    ASSERT_LE(0, 1);
}

TEST(assert_gt)
{
    ASSERT_GT(1, 0);
}

TEST(assert_ge)
{
    ASSERT_GE(1, 0);
}

TEST(assert_substring)
{
    ASSERT_SUBSTRING("123", "2");
}

TEST(assert_not_substring)
{
    ASSERT_NOT_SUBSTRING("123", "4");
}

TEST(assert_memory_eq)
{
    ASSERT_MEMORY_EQ("1", "1", 2);
}

static void foo()
{
}

static void bar()
{
}

TEST(assert_function_pointers)
{
    ASSERT_FUNCTION_POINTER_EQ(foo, foo);
    ASSERT_FUNCTION_POINTER_NE(foo, bar);
}

TEST(assert_true)
{
    ASSERT_TRUE(true);
}

TEST(assert_false)
{
    ASSERT_FALSE(false);
}

TEST(assert)
{
    ASSERT(1 == 1);
}

static void subprocess_entry(void *arg_p)
{
    (void)arg_p;

    printf("Hello!");
}

TEST(subprocess)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(subprocess_entry, NULL);
    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_EQ(result_p->stdout.buf_p, "Hello!");
    subprocess_result_free(result_p);
}

TEST(print)
{
    printf("Printing.\n");
}

static void expect_error_in_subprocess(void (*entry)(void),
                                       const char *expect_stdout_p)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output((subprocess_entry_t)entry, NULL);
    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, expect_stdout_p);
    subprocess_result_free(result_p);
}

static void assert_eq_error_entry()
{
    ASSERT_EQ(1, 0);
}

TEST(assert_eq_error)
{
    expect_error_in_subprocess(assert_eq_error_entry,
                               "1 is not equal to 0 (0x1, 0x0)");
}

static void assert_eq_error_string_entry()
{
    ASSERT_EQ("123", "23");
}

TEST(assert_eq_error_string)
{
    expect_error_in_subprocess(
        assert_eq_error_string_entry,
        "  Error: "CBR("The strings are not equal. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  23\n"
        "     "CR("+ ")CBR("1") CR(" |  ")CBR("1")"23\n");
}

static void assert_eq_error_string_added_line_entry()
{
    ASSERT_EQ("1\n"
              "2\n"
              "3\n",
              "1\n"
              "3\n");
}

TEST(assert_eq_error_string_added_line)
{
    expect_error_in_subprocess(
        assert_eq_error_string_added_line_entry,
        "  Error: "CBR("The strings are not equal. See diff for details.\n")
        "  Diff:\n"
        "\n"
        CM("       1")" |  1\\n\n"
        "     "CR("+ ")CBR("2")CR(" |  ")CBR("2\\n\n")
        CM("       3")" |  3\\n\n");
}

static void assert_eq_error_string_deleted_line_entry()
{
    ASSERT_EQ("1\n"
              "3\n",
              "1\n"
              "2\n"
              "3\n");
}

TEST(assert_eq_error_string_deleted_line)
{
    expect_error_in_subprocess(
        assert_eq_error_string_deleted_line_entry,
        "  Error: "CBR("The strings are not equal. See diff for details.\n")
        "  Diff:\n"
        "\n"
        CM("       1")" |  1\\n\n"
        "     "CR("- ")CBR("2")CR(" |  ")CBR("2\\n\n")
        CM("       2")" |  3\\n\n");
}

static void assert_eq_error_string_control_characters_entry()
{
    ASSERT_EQ("\x011\n"
              "3\x04\n",
              "1\n"
              "2\n"
              "3\x07\n");
}

TEST(assert_eq_error_string_control_characters)
{
    expect_error_in_subprocess(
        assert_eq_error_string_control_characters_entry,
        "  Error: "CBR("The strings are not equal. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  "B("1")"\\"B("n")"\n"
        "     - "B("1")" |  "B("2")"\\n\n"
        "     - "B("1")" |  3\\x0"B("7")"\\n\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"\\"CBR("x11")"\\n\n"
        "     "CR("+ ")CBR("2")CR(" |  ")"3\\x0"CBR("4")"\\n\n");
}

static void assert_eq_error_string_actual_null_entry()
{
    ASSERT_EQ((char *)NULL, "23");
}

TEST(assert_eq_error_string_actual_null)
{
    expect_error_in_subprocess(
        assert_eq_error_string_actual_null_entry,
        "The strings are not equal. Actual string is NULL.");
}

static void assert_eq_error_string_expected_null_entry()
{
    ASSERT_EQ("23", NULL);
}

TEST(assert_eq_error_string_expected_null)
{
    expect_error_in_subprocess(
        assert_eq_error_string_expected_null_entry,
        "The strings are not equal. Expected string is NULL.");
}

static void assert_ne_error_entry()
{
    ASSERT_NE(1, 1);
}

TEST(assert_ne_error)
{
    expect_error_in_subprocess(assert_ne_error_entry,
                               "1 is equal to 1 (0x1, 0x1)");
}

static void assert_lt_error_entry()
{
    ASSERT_LT(1, 1);
}

TEST(assert_lt_error)
{
    expect_error_in_subprocess(assert_lt_error_entry,
                               "1 is not less than 1");
}

static void assert_le_error_entry()
{
    ASSERT_LE(2, 1);
}

TEST(assert_le_error)
{
    expect_error_in_subprocess(assert_le_error_entry,
                               "2 is not less than or equal to 1");
}

static void assert_gt_error_entry()
{
    ASSERT_GT(1, 1);
}

TEST(assert_gt_error)
{
    expect_error_in_subprocess(assert_gt_error_entry,
                               "1 is not greater than 1");
}

static void assert_ge_error_entry()
{
    ASSERT_GE(1, 2);
}

TEST(assert_ge_error)
{
    expect_error_in_subprocess(assert_ge_error_entry,
                               "1 is not greater than or equal to 2");
}

static void assert_true_error_entry()
{
    ASSERT_TRUE(false);
}

TEST(assert_true_error)
{
    expect_error_in_subprocess(assert_true_error_entry,
                               "False is not true.");
}

static void assert_false_error_entry()
{
    ASSERT_FALSE(true);
}

TEST(assert_false_error)
{
    expect_error_in_subprocess(assert_false_error_entry,
                               "True is not false.");
}

static void assert_substring_error_entry()
{
    ASSERT_SUBSTRING("123", "4");
}

TEST(assert_substring_error)
{
    expect_error_in_subprocess(assert_substring_error_entry,
                               "  Haystack:\n"
                               "\n"
                               "    123\n"
                               "\n"
                               "  Needle:\n"
                               "\n"
                               "    4\n"
                               "\n");
}

static void assert_substring_error_haystack_null_entry()
{
    ASSERT_SUBSTRING(NULL, "4");
}

TEST(assert_substring_error_haystack_null)
{
    expect_error_in_subprocess(assert_substring_error_haystack_null_entry,
                               CBR("The haystack is NULL.\n"));
}

static void assert_substring_error_needle_null_entry()
{
    ASSERT_SUBSTRING("4", NULL);
}

TEST(assert_substring_error_needle_null)
{
    expect_error_in_subprocess(assert_substring_error_needle_null_entry,
                               CBR("The needle is NULL.\n"));
}

static void assert_not_substring_error_entry()
{
    ASSERT_NOT_SUBSTRING("123\t\r\x01", "3\t\r\x01");
}

TEST(assert_not_substring_error)
{
    expect_error_in_subprocess(
        assert_not_substring_error_entry,
        "The haystack contains the needle. See below for details.\n"
        "\x1b[0m  Haystack:\n"
        "\n"
        "    123\\t\\r\\x01\n"
        "\n"
        "  Needle:\n"
        "\n"
        "    3\\t\\r\\x01\n"
        "\n");
}

static void assert_memory_eq_error_entry()
{
    ASSERT_MEMORY_EQ("12345678901234567890123456789012345678901234567890"
                     "12345678901234567890123456789012345678901234567890"
                     "12345678901234567890123456789012345678901234567890",
                     "X2345678901234567890123456789012345678901234567890"
                     "12345678901234567890123456789012345678901234XX7890"
                     "12345678901234567890123456789012345678901234567890",
                     150);
}

TEST(assert_memory_eq_error)
{
    expect_error_in_subprocess(
        assert_memory_eq_error_entry,
        "  Error: "CBR("Memory mismatch. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  000000  "B("58")" 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36  "B("X")"234567890123456\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"000000  "CBR("31")" 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36  "CBR("1")"234567890123456\n"
        CM("       2")" |  000010  37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32  7890123456789012\n"
        CM("       3")" |  000020  33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38  3456789012345678\n"
        CM("       4")" |  000030  39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34  9012345678901234\n"
        CM("       5")" |  000040  35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30  5678901234567890\n"
        "     - "B("6")" |  000050  31 32 33 34 35 36 37 38 39 30 31 32 33 34 "B("58")" "B("58")"  12345678901234"B("XX")"\n"
        "     "CR("+ ") CBR("6") CR(" |  ")"000050  31 32 33 34 35 36 37 38 39 30 31 32 33 34 "CBR("35")" "CBR("36")"  12345678901234"CBR("56")"\n"
        CM("       7")" |  000060  37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32  7890123456789012\n"
        CM("       8")" |  000070  33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38  3456789012345678\n"
        CM("       9")" |  000080  39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34  9012345678901234\n"
        CM("      10")" |  000090  35 36 37 38 39 30 -- -- -- -- -- -- -- -- -- --  567890          \n");
}

static void assert_memory_eq_error_actual_null_entry()
{
    ASSERT_MEMORY_EQ(NULL, "1", 1);
}

TEST(assert_memory_eq_error_actual_null)
{
    expect_error_in_subprocess(
        assert_memory_eq_error_actual_null_entry,
        "Actual memory pointer is NULL.");
}

static void assert_memory_eq_error_expected_null_entry()
{
    ASSERT_MEMORY_EQ("1", NULL, 1);
}

TEST(assert_memory_eq_error_expected_null)
{
    expect_error_in_subprocess(
        assert_memory_eq_error_expected_null_entry,
        "Expected memory pointer is NULL.");
}

static void assert_memory_eq_error_actual_and_expected_null_entry()
{
    ASSERT_MEMORY_EQ(NULL, NULL, 1);
}

TEST(assert_memory_eq_error_actual_and_expected_null)
{
    expect_error_in_subprocess(
        assert_memory_eq_error_actual_and_expected_null_entry,
        "Actual memory pointer is NULL.");
}

static void assert_function_pointers_eq_entry()
{
    ASSERT_FUNCTION_POINTER_EQ(foo, bar);
}

TEST(assert_function_pointers_eq_error)
{
    expect_error_in_subprocess(
        assert_function_pointers_eq_entry,
        " is not equal to 0x");
}

static void assert_function_pointers_ne_entry()
{
    ASSERT_FUNCTION_POINTER_NE(foo, foo);
}

TEST(assert_function_pointers_ne_error)
{
    expect_error_in_subprocess(
        assert_function_pointers_ne_entry,
        " is equal to 0x");
}

static void assert_error_entry()
{
    ASSERT(1 == 0);
}

TEST(assert_error)
{
    expect_error_in_subprocess(assert_error_entry, "The condition is not true.");
}

static void fail_error_entry()
{
    FAIL("Intentional failure.");
}

TEST(fail_error)
{
    expect_error_in_subprocess(fail_error_entry,
                               "Intentional failure.");
}

TEST(capture_output)
{
    CAPTURE_OUTPUT(output, errput) {
        printf("Foo!\n");
        fprintf(stderr, "Bar!\n");
    }

    ASSERT_EQ(output, "Foo!\n");
    ASSERT_EQ(errput, "Bar!\n");
}

static void set_to_zero(int *value_p)
{
    *value_p = 0;
}

static void fail_segfault_entry()
{
    set_to_zero(NULL);
}

TEST(fail_segfault)
{
    expect_error_in_subprocess(fail_segfault_entry, "");
}

static void assert_int_array_eq_entry()
{
    int a[3] = { 1, 2, 3 };
    int b[3] = { 1, 4, 3 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_int_array)
{
    expect_error_in_subprocess(
        assert_int_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 1. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { 1, "B("4")", 3 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ 1, "CBR("2")", 3 }\n");
}

static void assert_int_array_eq_long_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 8, 10, 11, 12, 13, 14, 15 };

    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_int_array_eq_long)
{
    expect_error_in_subprocess(
        assert_int_array_eq_long_entry,
        "  Error: "CBR("The arrays differ at index 8. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { ..., 6, 7, 8, "B("8")", 10, 11, 12, ... }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ ..., 6, 7, 8, "CBR("9")", 10, 11, 12, ... }\n");
}

static void assert_int_array_eq_index_0_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int b[15] = { 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_int_array_eq_index_0)
{
    expect_error_in_subprocess(
        assert_int_array_eq_index_0_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("2")", 2, 3, 4, ... }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("1")", 2, 3, 4, ... }\n");
}

static void assert_int_array_eq_index_13_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 15 };
    int b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 15 };

    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_int_array_eq_index_13)
{
    expect_error_in_subprocess(
        assert_int_array_eq_index_13_entry,
        "  Error: "CBR("The arrays differ at index 13. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { ..., 11, 12, 13, "B("15")", 15 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ ..., 11, 12, 13, "CBR("24")", 15 }\n");
}

static void assert_char_array_eq_entry()
{
    char a[1] = { 'a' };
    char b[1] = { 'A' };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_char_array)
{
    expect_error_in_subprocess(
        assert_char_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("65")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("97")" }\n");
}

static void assert_schar_array_eq_entry()
{
    signed char a[1] = { 'a' };
    signed char b[1] = { 'A' };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_schar_array)
{
    expect_error_in_subprocess(
        assert_schar_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("65")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("97")" }\n");
}

static void assert_uchar_array_eq_entry()
{
    unsigned char a[1] = { 'a' };
    unsigned char b[1] = { 'A' };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_uchar_array)
{
    expect_error_in_subprocess(
        assert_uchar_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("65")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("97")" }\n");
}

static void assert_short_array_eq_entry()
{
    short a[1] = { -5 };
    short b[1] = { 1 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_short_array)
{
    expect_error_in_subprocess(
        assert_short_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("1")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("-5")" }\n");
}

static void assert_ushort_array_eq_entry()
{
    unsigned short a[1] = { 5 };
    unsigned short b[1] = { 1 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_ushort_array)
{
    expect_error_in_subprocess(
        assert_ushort_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("1")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("5")" }\n");
}

static void assert_uint_array_eq_entry()
{
    unsigned int a[1] = { 5 };
    unsigned int b[1] = { 1 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_uint_array)
{
    expect_error_in_subprocess(
        assert_uint_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("1")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("5")" }\n");
}

static void assert_long_array_eq_entry()
{
    long a[1] = { 3 };
    long b[1] = { 4 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_long_array)
{
    expect_error_in_subprocess(
        assert_long_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")" }\n");
}

static void assert_ulong_array_eq_entry()
{
    unsigned long a[1] = { 3 };
    unsigned long b[1] = { 4 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_ulong_array)
{
    expect_error_in_subprocess(
        assert_ulong_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")" }\n");
}

static void assert_llong_array_eq_entry()
{
    long long a[1] = { 3 };
    long long b[1] = { 4 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_llong_array)
{
    expect_error_in_subprocess(
        assert_llong_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")" }\n");
}

static void assert_ullong_array_eq_entry()
{
    unsigned long long a[1] = { 3 };
    unsigned long long b[1] = { 4 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_ullong_array)
{
    expect_error_in_subprocess(
        assert_ullong_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")" }\n");
}

static void assert_float_array_eq_entry()
{
    float a[1] = { 3.0 };
    float b[1] = { 4.0 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_float_array)
{
    expect_error_in_subprocess(
        assert_float_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")".000000 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")".000000 }\n");
}

static void assert_double_array_eq_entry()
{
    double a[1] = { 3.0 };
    double b[1] = { 4.0 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_double_array)
{
    expect_error_in_subprocess(
        assert_double_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")".000000 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")".000000 }\n");
}

static void assert_ldouble_array_eq_entry()
{
    long double a[1] = { 3.0 };
    long double b[1] = { 4.0 };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_ldouble_array)
{
    expect_error_in_subprocess(
        assert_ldouble_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")".000000 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")".000000 }\n");
}

static void assert_bool_array_eq_entry()
{
    bool a[1] = { true };
    bool b[1] = { false };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_bool_array)
{
    expect_error_in_subprocess(
        assert_bool_array_eq_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("0")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("1")" }\n");
}

struct struct_array_t {
    int a;
};

static void assert_struct_array_eq_entry()
{
    struct struct_array_t a[3] = { { 1 }, { 2 }, { 3 } };
    struct struct_array_t b[3] = { { 1 }, { 4 }, { 3 } };

    ASSERT_ARRAY_EQ(a, a, sizeof(a));
    ASSERT_ARRAY_EQ(a, b, sizeof(a));
}

TEST(assert_struct_array)
{
    expect_error_in_subprocess(
        assert_struct_array_eq_entry,
        "The arrays differ at index 1. Memory mismatch. See diff for details.");
}

TEST(argument_help)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("build/app --help");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_EQ(
        result_p->stdout.buf_p,
        "usage: build/app [-h] [-v] [-c] [-a] [-r] [-f] [-j] [<test-pattern>]\n"
        "\n"
        "Run tests.\n"
        "\n"
        "positional arguments:\n"
        "  test-pattern                  Only run tests matching given pattern. "
        "'^' matches\n"
        "                                the beginning and '$' matches the end "
        "of the test\n"
        "                                name.\n"
        "\n"
        "optional arguments:\n"
        "  -h, --help                    Show this help message and exit.\n"
        "  -v, --version                 Print version information.\n"
        "  -c, --continue-on-failure     Continue on test failure.\n"
        "  -a, --print-all-calls         Print all calls to ease debugging.\n"
        "  -r, --report-json-file        JSON test report file (default: "
        "report.json).\n"
        "  -f, --print-test-file-func    Print file:function for exactly one "
        "test.\n"
        "  -j, --jobs                    Run given number of tests in "
        "parallel\n"
        "                                (default: 1).\n");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(argument_version)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("build/app --version");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_EQ(result_p->stdout.buf_p, NALA_VERSION "\n");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(argument_print_full_test_name)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output(
        "build/app --print-test-file-func argument_version");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_EQ(result_p->stdout.buf_p, "main.c:argument_version\n");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(argument_print_full_test_name_error_no_match)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output(
        "build/app --print-test-file-func not_a_test");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_EQ(result_p->stdout.buf_p, "");
    ASSERT_EQ(result_p->stderr.buf_p,
              "error: 'not_a_test' does not match any test.\n");

    subprocess_result_free(result_p);
}

TEST(argument_print_full_test_name_error_many_matches)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output(
        "build/app --print-test-file-func argument_print_full_test_name");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_EQ(result_p->stdout.buf_p, "");
    ASSERT_EQ(
        result_p->stderr.buf_p,
        "error: 'argument_print_full_test_name' matches more than one test.\n");

    subprocess_result_free(result_p);
}

TEST(pattern_match_test_end)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app foo$");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_NOT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo_extra");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_fail::foo");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo");

    subprocess_result_free(result_p);
}

TEST(pattern_match_test_only_end_match_all)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app --continue-on-failure $");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo_extra");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_fail::foo");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo");

    subprocess_result_free(result_p);
}

TEST(pattern_match_test_empty_string)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output(
        "subtest/build/app --continue-on-failure \"\"");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo_extra");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_fail::foo");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_ok::foo");

    subprocess_result_free(result_p);
}

/* Tests used to test beginning of test match. */
TEST(beginning_1)
{
}

TEST(another_beginning_2)
{
}

TEST(pattern_match_test_only_beginning)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("build/app ^beginning");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "beginning_1");
    ASSERT_NOT_SUBSTRING(result_p->stdout.buf_p, "beginning_2");

    subprocess_result_free(result_p);
}

TEST(pattern_match_test_no_name)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("build/app ^$");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "0 total");

    subprocess_result_free(result_p);
}

TEST(subtest_run_all_tests_continue_on_failure)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app --continue-on-failure");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Terminated by signal 11.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "2 failed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "4 passed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "6 total");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(subtest_parallel_continue_on_with_failure)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output(
        "subtest/build/app --jobs 2 --continue-on-failure");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Terminated by signal 11.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "2 failed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "4 passed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "6 total");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(subtest_parallel_stop_on_failure)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app --jobs 2");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Just fail!");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "failed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "6 total");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(subtest_zero_jobs_failure)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app --jobs 0");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "error: At least one job is required, 0 given.\n");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(subtest_run_all_tests_skipped)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app");

    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 failed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "5 skipped");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "6 total");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

TEST(subprocess_run_passed)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_exec_output("subtest/build/app test_ok::empty");

    ASSERT_EQ(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 passed");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 total");
    ASSERT_EQ(result_p->stderr.buf_p, "");

    subprocess_result_free(result_p);
}

static void with_two_messages_entry()
{
    WITH_MESSAGE("Additional message (i = %d).", 5) {
        WITH_MESSAGE("Another additional message (j = %d).", 1) {
            ASSERT(1 == 2);
        }
    }
}

TEST(with_two_messages)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(with_two_messages_entry, NULL);
    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(
        result_p->stdout.buf_p,
        "  Messages: \x1b[0m\x1b[31m\x1b[1mAdditional message (i = 5).\n");
    ASSERT_SUBSTRING(
        result_p->stdout.buf_p,
        "            \x1b[0m\x1b[31m\x1b[1mAnother additional message (j = 1).\n");
    subprocess_result_free(result_p);
}

static void old_with_message_not_part_of_messages_entry()
{
    WITH_MESSAGE("Should not be shown.", NULL) {
        ASSERT(1 == 1);
    }

    WITH_MESSAGE("This should be shown.", NULL) {
        ASSERT(1 == 2);
    }
}

TEST(old_with_message_not_part_of_messages)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(old_with_message_not_part_of_messages_entry,
                                      NULL);
    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_NOT_SUBSTRING(result_p->stdout.buf_p, "Should not be shown.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "This should be shown.");
    subprocess_result_free(result_p);
}
