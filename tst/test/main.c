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

TEST(assert_memory)
{
    ASSERT_MEMORY("1", "1", 2);
}

static void foo()
{
}

static void bar()
{
}

TEST(assert_function_pointers)
{
    ASSERT_FUNCTION_POINTERS_EQ(foo, foo);
    ASSERT_FUNCTION_POINTERS_NE(foo, bar);
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
    expect_error_in_subprocess(assert_eq_error_entry, "1 != 0 (0x1 != 0x0)");
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
        CM("       1")" |  1\n"
        "     "CR("+ ")CBR("2")CR(" |  ")CBR("2\n")
        CM("       3")" |  3\n");
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
        CM("       1")" |  1\n"
        "     "CR("- ")CBR("2")CR(" |  ")CBR("2\n")
        CM("       2")" |  3\n");
}

static void assert_ne_error_entry()
{
    ASSERT_NE(1, 1);
}

TEST(assert_ne_error)
{
    expect_error_in_subprocess(assert_ne_error_entry, "1 == 1 (0x1 == 0x1)");
}

static void assert_lt_error_entry()
{
    ASSERT_LT(1, 1);
}

TEST(assert_lt_error)
{
    expect_error_in_subprocess(assert_lt_error_entry, "1 >= 1");
}

static void assert_le_error_entry()
{
    ASSERT_LE(2, 1);
}

TEST(assert_le_error)
{
    expect_error_in_subprocess(assert_le_error_entry, "2 > 1");
}

static void assert_gt_error_entry()
{
    ASSERT_GT(1, 1);
}

TEST(assert_gt_error)
{
    expect_error_in_subprocess(assert_gt_error_entry, "1 <= 1");
}

static void assert_ge_error_entry()
{
    ASSERT_GE(1, 2);
}

TEST(assert_ge_error)
{
    expect_error_in_subprocess(assert_ge_error_entry, "1 < 2");
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
                               "  Needle:\n"
                               "\n"
                               "    4\n");
}

static void assert_not_substring_error_entry()
{
    ASSERT_NOT_SUBSTRING("123", "3");
}

TEST(assert_not_substring_error)
{
    expect_error_in_subprocess(assert_not_substring_error_entry,
                               "\"123\" contains \"3\"");
}

static void assert_memory_error_entry()
{
    ASSERT_MEMORY("12345678901234567890123456789012345678901234567890"
                  "12345678901234567890123456789012345678901234567890"
                  "12345678901234567890123456789012345678901234567890",
                  "X2345678901234567890123456789012345678901234567890"
                  "12345678901234567890123456789012345678901234XX7890"
                  "12345678901234567890123456789012345678901234567890",
                  150);
}

TEST(assert_memory_error)
{
    expect_error_in_subprocess(
        assert_memory_error_entry,
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

static void assert_function_pointers_eq_entry()
{
    ASSERT_FUNCTION_POINTERS_EQ(foo, bar);
}

TEST(assert_function_pointers_eq_error)
{
    expect_error_in_subprocess(
        assert_function_pointers_eq_entry,
        " != 0x");
}

static void assert_function_pointers_ne_entry()
{
    ASSERT_FUNCTION_POINTERS_NE(foo, foo);
}

TEST(assert_function_pointers_ne_error)
{
    expect_error_in_subprocess(
        assert_function_pointers_ne_entry,
        " == 0x");
}

static void assert_error_entry()
{
    ASSERT(1 == 0);
}

TEST(assert_error)
{
    expect_error_in_subprocess(assert_error_entry, "assert");
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

static void fail_segfault_entry()
{
    (*((int *)0)) = 1;
}

TEST(fail_segfault)
{
    expect_error_in_subprocess(fail_segfault_entry, "");
}

static void assert_int_array_entry()
{
    int a[3] = { 1, 2, 3 };
    int b[3] = { 1, 4, 3 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_int_array)
{
    expect_error_in_subprocess(
        assert_int_array_entry,
        "  Error: "CBR("The arrays differ at index 1. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { 1, "B("4")", 3 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ 1, "CBR("2")", 3 }\n");
}

static void assert_int_array_long_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 8, 10, 11, 12, 13, 14, 15 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_int_array_long)
{
    expect_error_in_subprocess(
        assert_int_array_long_entry,
        "  Error: "CBR("The arrays differ at index 8. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { ..., 6, 7, 8, "B("8")", 10, 11, 12, ... }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ ..., 6, 7, 8, "CBR("9")", 10, 11, 12, ... }\n");
}

static void assert_int_array_index_0_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int b[15] = { 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_int_array_index_0)
{
    expect_error_in_subprocess(
        assert_int_array_index_0_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("2")", 2, 3, 4, ... }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("1")", 2, 3, 4, ... }\n");
}

static void assert_int_array_index_13_entry()
{
    int a[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 15 };
    int b[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 15 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_int_array_index_13)
{
    expect_error_in_subprocess(
        assert_int_array_index_13_entry,
        "  Error: "CBR("The arrays differ at index 13. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { ..., 11, 12, 13, "B("15")", 15 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ ..., 11, 12, 13, "CBR("24")", 15 }\n");
}

static void assert_char_array_entry()
{
    char a[1] = { 'a' };
    char b[1] = { 'A' };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_char_array)
{
    expect_error_in_subprocess(
        assert_char_array_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("65")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("97")" }\n");
}

static void assert_long_array_entry()
{
    long a[1] = { 3 };
    long b[1] = { 4 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_long_array)
{
    expect_error_in_subprocess(
        assert_long_array_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")" }\n");
}

static void assert_float_array_entry()
{
    float a[1] = { 3.0 };
    float b[1] = { 4.0 };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_float_array)
{
    expect_error_in_subprocess(
        assert_float_array_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("4")".000000 }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("3")".000000 }\n");
}

static void assert_bool_array_entry()
{
    bool a[1] = { true };
    bool b[1] = { false };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_bool_array)
{
    expect_error_in_subprocess(
        assert_bool_array_entry,
        "  Error: "CBR("The arrays differ at index 0. See diff for details.\n")
        "  Diff:\n"
        "\n"
        "     - "B("1")" |  { "B("0")" }\n"
        "     "CR("+ ")CBR("1")CR(" |  ")"{ "CBR("1")" }\n");
}

struct struct_array_t {
    int a;
};

static void assert_struct_array_entry()
{
    struct struct_array_t a[3] = { { 1 }, { 2 }, { 3 } };
    struct struct_array_t b[3] = { { 1 }, { 4 }, { 3 } };

    ASSERT_ARRAY(a, b, sizeof(a));
}

TEST(assert_struct_array)
{
    expect_error_in_subprocess(
        assert_struct_array_entry,
        "The arrays differ at index 1. Memory mismatch. See diff for details.");
}

TEST(argument_help)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec("build/app --help");
    }

    ASSERT_EQ(result_p->exit_code, 0);
    subprocess_result_free(result_p);

    ASSERT_EQ(
        output,
        "usage: build/app [-h] [-v] [-c] [-a] [<test-pattern>]\n"
        "\n"
        "Run tests.\n"
        "\n"
        "positional arguments:\n"
        "  test-pattern                  Only run tests containing given "
        "pattern.\n"
        "\n"
        "optional arguments:\n"
        "  -h, --help                    Show this help message and exit.\n"
        "  -v, --version                 Print version information.\n"
        "  -c, --continue-on-failure     Always run all tests.\n"
        "  -a, --print-all-calls         Print all calls to ease debugging.\n"
        "  -r, --report-json-file        JSON test report file (default: "
        "report.json).\n"
        "  -f, --print-test-file-func    Print file:function for exactly one "
        "test.\n");
    ASSERT_EQ(errput, "");
}

TEST(argument_version)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec("build/app --version");
    }

    ASSERT_EQ(result_p->exit_code, 0);
    subprocess_result_free(result_p);

    ASSERT_EQ(output, NALA_VERSION "\n");
    ASSERT_EQ(errput, "");
}

TEST(argument_print_full_test_name)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec(
            "build/app --print-test-file-func argument_version");
    }

    ASSERT_EQ(result_p->exit_code, 0);
    subprocess_result_free(result_p);

    ASSERT_EQ(output, "main.c:argument_version\n");
    ASSERT_EQ(errput, "");
}

TEST(argument_print_full_test_name_error_no_match)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec(
            "build/app --print-test-file-func not_a_test");
    }

    ASSERT_EQ(result_p->exit_code, 1);
    subprocess_result_free(result_p);

    ASSERT_EQ(output, "");
    ASSERT_EQ(errput, "error: 'not_a_test' does not match any test.\n");
}

TEST(argument_print_full_test_name_error_many_matches)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec(
            "build/app --print-test-file-func argument");
    }

    ASSERT_EQ(result_p->exit_code, 1);
    subprocess_result_free(result_p);

    ASSERT_EQ(output, "");
    ASSERT_EQ(errput, "error: 'argument' matches more than one test.\n");
}

TEST(subtest_run_all_tests_continue_on_failure)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec("subtest/build/app --continue-on-failure");
    }

    ASSERT_EQ(result_p->exit_code, 1);
    subprocess_result_free(result_p);

    ASSERT_SUBSTRING(output, "Terminated by signal 11.");
    ASSERT_SUBSTRING(output, "2 failed");
    ASSERT_SUBSTRING(output, "1 passed");
    ASSERT_SUBSTRING(output, "3 total");
    ASSERT_EQ(errput, "");
}

TEST(subtest_run_all_tests_skipped)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec("subtest/build/app");
    }

    ASSERT_EQ(result_p->exit_code, 1);
    subprocess_result_free(result_p);

    ASSERT_SUBSTRING(output, "1 failed");
    ASSERT_SUBSTRING(output, "2 skipped");
    ASSERT_SUBSTRING(output, "3 total");
    ASSERT_EQ(errput, "");
}

TEST(subprocess_run_passed)
{
    struct subprocess_result_t *result_p;

    CAPTURE_OUTPUT(output, errput) {
        result_p = subprocess_exec("subtest/build/app test_ok::empty");
    }

    ASSERT_EQ(result_p->exit_code, 0);
    subprocess_result_free(result_p);

    ASSERT_SUBSTRING(output, "1 passed");
    ASSERT_SUBSTRING(output, "1 total");
    ASSERT_EQ(errput, "");
}
