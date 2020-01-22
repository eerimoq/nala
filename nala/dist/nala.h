#ifndef NALA_H
#define NALA_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define NALA_VERSION "0.81.0"

#define TEST(name)                                      \
    static void name(void);                             \
    static void name ## _before_fork() {}               \
    static struct nala_test_t nala_test_ ## name = {    \
        .name_p = #name,                                \
        .file_p = __FILE__,                             \
        .line = __LINE__,                               \
        .func = name,                                   \
        .before_fork_func = name ## _before_fork,       \
        .next_p = NULL                                  \
    };                                                  \
    __attribute__ ((constructor))                       \
    static void nala_constructor_ ## name(void)         \
    {                                                   \
        nala_register_test(&nala_test_ ## name);        \
    }                                                   \
    static void name(void)

#define NALA_TEST_FAILURE(message_p)            \
    nala_test_failure(message_p)

#define NALA_CHECK_EQ  0
#define NALA_CHECK_NE  1
#define NALA_CHECK_LT  2
#define NALA_CHECK_LE  3
#define NALA_CHECK_GT  4
#define NALA_CHECK_GE  5

#define NALA_ASSERT_FUNC(value)                         \
    _Generic((value),                                   \
             char: nala_assert_char,                    \
             signed char: nala_assert_schar,            \
             unsigned char: nala_assert_uchar,          \
             short: nala_assert_short,                  \
             unsigned short: nala_assert_ushort,        \
             int: nala_assert_int,                      \
             unsigned int: nala_assert_uint,            \
             long: nala_assert_long,                    \
             unsigned long: nala_assert_ulong,          \
             long long: nala_assert_llong,              \
             unsigned long long: nala_assert_ullong,    \
             float: nala_assert_float,                  \
             double: nala_assert_double,                \
             long double: nala_assert_ldouble,          \
             char *: nala_assert_string,                \
             const char *: nala_assert_string,          \
             bool: nala_assert_bool,                    \
             default: nala_assert_ptr)

#define ASSERT_EQ(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_EQ)

#define ASSERT_NE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_NE)

#define ASSERT_LT(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_LT)

#define ASSERT_LE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_LE)

#define ASSERT_GT(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_GT)

#define ASSERT_GE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_GE)

#define ASSERT_SUBSTRING(haystack, needle)      \
    nala_assert_substring(haystack, needle)

#define ASSERT_NOT_SUBSTRING(haystack, needle)  \
    nala_assert_not_substring(haystack, needle)

#define ASSERT_MEMORY(actual, expected, size)   \
    nala_assert_memory(actual, expected, size)

#define ASSERT(cond) nala_assert(cond)

#define FAIL() nala_fail()

#define CAPTURE_OUTPUT(stdout_name, stderr_name)                        \
    int stdout_name ## i;                                               \
    static char *stdout_name = NULL;                                    \
    static char *stderr_name = NULL;                                    \
                                                                        \
    for (stdout_name ## i = 0, nala_capture_output_start(&stdout_name,  \
                                                         &stderr_name); \
         stdout_name ## i < 1;                                          \
         stdout_name ## i++, nala_capture_output_stop())

struct nala_test_t {
    const char *name_p;
    const char *file_p;
    int line;
    void (*func)(void);
    void (*before_fork_func)(void);
    int exit_code;
    int signal_number;
    float elapsed_time_ms;
    struct nala_test_t *next_p;
};

char *nala_char_p;
const char *nala_const_char_p;

bool nala_check_string_equal(const char *actual_p, const char *expected_p);

const char *nala_format(const char *format_p, ...);

const char *nala_format_string(const char *format_p, ...);

const char *nala_format_memory(const char *prefix_p,
                               const void *left_p,
                               const void *right_p,
                               size_t size);

bool nala_check_substring(const char *string_p, const char *substring_p);

bool nala_check_memory(const void *left_p, const void *right_p, size_t size);

void nala_capture_output_start(char **stdout_pp, char **stderr_pp);

void nala_capture_output_stop(void);

/**
 * message_p is freed by this function.
 */
__attribute__ ((noreturn)) void nala_test_failure(const char *message_p);

void nala_register_test(struct nala_test_t *test_p);

int nala_run_tests(void);

void nala_reset_all_mocks(void);

char *nala_mock_traceback_format(void **buffer_pp, int depth);

void nala_assert_char(char actual, char expected, int op);

void nala_assert_schar(signed char actual, signed char expected, int op);

void nala_assert_uchar(unsigned char actual, unsigned char expected, int op);

void nala_assert_short(short actual, short expected, int op);

void nala_assert_ushort(unsigned short actual, unsigned short expected, int op);

void nala_assert_int(int actual, int expected, int op);

void nala_assert_uint(unsigned int actual, unsigned int expected, int op);

void nala_assert_long(long actual, long expected, int op);

void nala_assert_ulong(unsigned long actual, unsigned long expected, int op);

void nala_assert_llong(long long actual, long long expected, int op);

void nala_assert_ullong(unsigned long long actual,
                        unsigned long long expected,
                        int op);

void nala_assert_float(float actual, float expected, int op);

void nala_assert_double(double actual, double expected, int op);

void nala_assert_ldouble(long double actual, long double expected, int op);

void nala_assert_bool(bool actual, bool expected, int op);

void nala_assert_ptr(const void *actual_p, const void *expected_p, int op);

void nala_assert_string(const char *actual_p, const char *expected_p, int op);

void nala_assert_substring(const char *haystack_p, const char *needle_p);

void nala_assert_not_substring(const char *haystack_p, const char *needle_p);

void nala_assert_memory(const void *actual_p, const void *expected_p, size_t size);

void nala_assert(bool cond);

void nala_fail(void);

#endif
