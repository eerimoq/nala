#include "nala.h"
#include "subprocess.h"

#define BLD "\x1b[1m"
#define RST "\x1b[0m"
#define RD  "\x1b[31m"
#define RED  RST RD
#define GN  "\x1b[32m"
#define MA "\x1b[35m"
#define GRN  RST GN
#define BRED RED BLD
#define BGRN GRN BLD

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
        "  Error: "BRED"The strings are not equal. See diff for details.\n"
        RST
        "  Diff:\n"
        "\n"
        "     - "RST BLD"1"RST" |  23\n"
        "     "RED"+ "RST BRED"1"RST RED" |  "RST BRED"1"RST"23\n");
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
        "  Error: "BRED"Memory mismatch. See diff for details.\n"
        RST
        "  Diff:\n"
        "\n"
        "     - "RST BLD"1"RST" |  000000  "RST BLD"58"RST" 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36  "RST BLD"X"RST"234567890123456\n"
        "     "RST RD"+ "RST BRED"1"RST RST RD" |  "RST"000000  "BRED"31"RST" 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36  "BRED"1"RST"234567890123456\n"
        RST MA"       2"RST" |  000010  37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32  7890123456789012\n"
        RST MA"       3"RST" |  000020  33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38  3456789012345678\n"
        RST MA"       4"RST" |  000030  39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34  9012345678901234\n"
        RST MA"       5"RST" |  000040  35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30  5678901234567890\n"
        "     - "RST BLD"6"RST" |  000050  31 32 33 34 35 36 37 38 39 30 31 32 33 34 "RST BLD"58"RST" "RST BLD"58"RST"  12345678901234"RST BLD"XX"RST"\n"
        "     "RST RD"+ "RST BRED"6"RST RST RD" |  "RST"000050  31 32 33 34 35 36 37 38 39 30 31 32 33 34 "BRED"35"RST" "BRED"36"RST"  12345678901234"BRED"56"RST"\n"
        RST MA"       7"RST" |  000060  37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32  7890123456789012\n"
        RST MA"       8"RST" |  000070  33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38  3456789012345678\n"
        RST MA"       9"RST" |  000080  39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34  9012345678901234\n"
        RST MA"      10"RST" |  000090  35 36 37 38 39 30 -- -- -- -- -- -- -- -- -- --  567890          \n");
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
        "The arrays differ at index 1.");
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
