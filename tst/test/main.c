#include "nala.h"
#include "subprocess.h"

TEST(assert_eq)
{
    ASSERT_EQ(1, 1);
    ASSERT_EQ("", "");
    ASSERT_EQ("12", "12");
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
                               "1 is not equal to 0.");
}

static void assert_eq_error_string_entry()
{
    ASSERT_EQ("123", "23");
}

TEST(assert_eq_error_string)
{
    expect_error_in_subprocess(assert_eq_error_string_entry,
                               "\"123\" is not equal to \"23\".");
    expect_error_in_subprocess(assert_eq_error_string_entry,
                               "Diff:");
}

static void assert_ne_error_entry()
{
    ASSERT_NE(1, 1);
}

TEST(assert_ne_error)
{
    expect_error_in_subprocess(assert_ne_error_entry,
                               "1 is not different from 1.");
}

static void assert_lt_error_entry()
{
    ASSERT_LT(1, 1);
}

TEST(assert_lt_error)
{
    expect_error_in_subprocess(assert_lt_error_entry,
                               "1 is not less than 1.");
}

static void assert_le_error_entry()
{
    ASSERT_LE(2, 1);
}

TEST(assert_le_error)
{
    expect_error_in_subprocess(assert_le_error_entry,
                               "2 is not less than or equal to 1.");
}

static void assert_gt_error_entry()
{
    ASSERT_GT(1, 1);
}

TEST(assert_gt_error)
{
    expect_error_in_subprocess(assert_gt_error_entry,
                               "1 is not greater than 1.");
}

static void assert_ge_error_entry()
{
    ASSERT_GE(1, 2);
}

TEST(assert_ge_error)
{
    expect_error_in_subprocess(assert_ge_error_entry,
                               "1 is not greater or equal to 2.");
}

static void assert_substring_error_entry()
{
    ASSERT_SUBSTRING("123", "4");
}

TEST(assert_substring_error)
{
    expect_error_in_subprocess(assert_substring_error_entry,
                               "\"123\" doesn't contain \"4\".");
}

static void assert_not_substring_error_entry()
{
    ASSERT_NOT_SUBSTRING("123", "3");
}

TEST(assert_not_substring_error)
{
    expect_error_in_subprocess(assert_not_substring_error_entry,
                               "\"123\" contains \"3\".");
}

static void assert_memory_error_entry()
{
    ASSERT_MEMORY("1", "2", 2);
}

TEST(assert_memory_error)
{
    expect_error_in_subprocess(assert_memory_error_entry, "");
}

static void assert_error_entry()
{
    ASSERT(1 == 0);
}

TEST(assert_error)
{
    expect_error_in_subprocess(assert_error_entry, "Assert.");
}

static void fail_error_entry()
{
    FAIL();
}

TEST(fail_error)
{
    expect_error_in_subprocess(fail_error_entry, "Fail.");
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
