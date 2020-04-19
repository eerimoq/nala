/*
Mocks source file


Do not edit manually
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "nala.h"
#include <stddef.h>
#include <errno.h>
#include "nala_mocks.h"
#include <execinfo.h>

#define MODE_REAL            0
#define MODE_MOCK_ONCE       1
#define MODE_IMPLEMENTATION  2
#define MODE_MOCK            3
#define MODE_NONE            4

#define INSTANCE_MODE_NORMAL  0
#define INSTANCE_MODE_REAL    1

const char *nala_mock_func_p = NULL;
const char *nala_mock_param_p = NULL;
struct nala_traceback_t *nala_mock_traceback_p = NULL;

#define NALA_INSTANCE_NEW(instance_p, mode_in)          \
    do {                                                \
        instance_p = nala_xmalloc(sizeof(*instance_p)); \
        instance_p->mode = mode_in;                     \
        instance_p->handle = nala_next_handle++;        \
        instance_p->next_p = NULL;                      \
        nala_traceback(&instance_p->data.traceback);    \
    } while (0);

#define NALA_INSTANCES_APPEND(list, item_p)     \
    do {                                        \
        if ((list).head_p == NULL) {            \
            (list).head_p = item_p;             \
        } else {                                \
            (list).tail_p->next_p = item_p;     \
        }                                       \
                                                \
        if ((list).next_p == NULL) {            \
            (list).next_p = item_p;             \
        }                                       \
                                                \
        (list).tail_p = item_p;                 \
        (list).length++;                        \
    } while (0);

#define NALA_INSTANCES_POP(list, instance_pp)           \
    do {                                                \
        *(instance_pp) = (list).next_p;                 \
                                                        \
        if (*(instance_pp) != NULL) {                   \
            (list).next_p = (*(instance_pp))->next_p;   \
                                                        \
            if ((*(instance_pp))->next_p == NULL) {     \
                (list).next_p = NULL;                   \
            }                                           \
                                                        \
            (list).length--;                            \
        }                                               \
    } while (0);

#define NALA_INSTANCES_DESTROY(list, current_p, tmp_p)  \
    do {                                                \
        current_p = (list).head_p;                      \
                                                        \
        while (current_p != NULL) {                     \
            tmp_p = current_p;                          \
            current_p = current_p->next_p;              \
            nala_free(tmp_p);                           \
        }                                               \
                                                        \
        (list).head_p = NULL;                           \
        (list).next_p = NULL;                           \
        (list).tail_p = NULL;                           \
        (list).length = 0;                              \
    } while (0);

#define NALA_INSTANCES_FIND_USED(list, instance_pp, handle_in)  \
    do {                                                        \
        *(instance_pp) = (list).head_p;                         \
                                                                \
        while (*(instance_pp) != NULL) {                        \
            if (*(instance_pp) == (list).next_p) {              \
                *(instance_pp) = NULL;                          \
                break;                                          \
            }                                                   \
                                                                \
            if ((*(instance_pp))->handle == handle_in) {        \
                break;                                          \
            }                                                   \
                                                                \
            *(instance_pp) = (*(instance_pp))->next_p;          \
        }                                                       \
    } while (0);

#define CHECK_NO_INSTANCES(state)                                       \
    if ((state).instances.next_p != NULL) {                             \
        nala_test_failure(                                              \
            nala_format(                                                \
                "Cannot change mock mode with mock instances enqueued.\n")); \
    }

#define NALA_STATE_RESET(_state, current_p, tmp_p)                      \
    (_state).state.mode = MODE_REAL;                                    \
    (_state).state.suspended.count = 0;                                 \
    NALA_INSTANCES_DESTROY((_state).instances, current_p, tmp_p)

struct nala_set_param {
    void *buf_p;
    size_t size;
};

static void *nala_xmalloc(size_t size)
{
    void *buf_p;

    nala_suspend_all_mocks();
    buf_p = malloc(size);

    if (buf_p == NULL) {
        perror("malloc");
        exit(1);
    }

    nala_resume_all_mocks();

    return (buf_p);
}

static void nala_free(void *buf_p)
{
    nala_suspend_all_mocks();
    free(buf_p);
    nala_resume_all_mocks();
}

char *nala_format_mock_traceback(const char *message_p,
                                 struct nala_traceback_t *traceback_p)
{
    FILE *file_p;
    char *buf_p;
    size_t file_size;
    char *formatted_traceback_p;

    nala_suspend_all_mocks();

    if (traceback_p != NULL) {
        formatted_traceback_p = nala_mock_traceback_format(
            &traceback_p->addresses[0],
            traceback_p->depth);
    } else {
        formatted_traceback_p = strdup("  Traceback missing.\n");
    }

    file_p = open_memstream(&buf_p, &file_size);
    fprintf(file_p, "%s%s", message_p, formatted_traceback_p);
    fputc('\0', file_p);
    fclose(file_p);
    free((void *)message_p);
    free(formatted_traceback_p);

    nala_resume_all_mocks();

    return (buf_p);
}

#define NALA_MOCK_ASSERT_IN_EQ_FUNC(value)                      \
    _Generic((value),                                           \
             char: nala_mock_assert_in_eq_char,                 \
             signed char: nala_mock_assert_in_eq_schar,         \
             unsigned char: nala_mock_assert_in_eq_uchar,       \
             short: nala_mock_assert_in_eq_short,               \
             unsigned short: nala_mock_assert_in_eq_ushort,     \
             int: nala_mock_assert_in_eq_int,                   \
             unsigned int: nala_mock_assert_in_eq_uint,         \
             long: nala_mock_assert_in_eq_long,                 \
             unsigned long: nala_mock_assert_in_eq_ulong,       \
             long long: nala_mock_assert_in_eq_llong,           \
             unsigned long long: nala_mock_assert_in_eq_ullong, \
             float: nala_mock_assert_in_eq_float,               \
             double: nala_mock_assert_in_eq_double,             \
             long double: nala_mock_assert_in_eq_ldouble,       \
             bool: nala_mock_assert_in_eq_bool,                 \
             default: nala_mock_assert_in_eq_ptr)

#define PRINT_FORMAT(value)                             \
    _Generic((value),                                   \
             char: "%c",                                \
             signed char: "%hhd",                       \
             unsigned char: "%hhu",                     \
             signed short: "%hd",                       \
             unsigned short: "%hu",                     \
             signed int: "%d",                          \
             unsigned int: "%u",                        \
             long int: "%ld",                           \
             unsigned long int: "%lu",                  \
             long long int: "%lld",                     \
             unsigned long long int: "%llu",            \
             float: "%f",                               \
             double: "%f",                              \
             long double: "%Lf",                        \
             bool: "%d",                                \
             default: "%p")

#define PRINT_FORMAT_HEX(value)                 \
    _Generic((value),                           \
             signed char: "%hhx",               \
             unsigned char: "%hhx",             \
             signed short: "%hx",               \
             unsigned short: "%hx",             \
             signed int: "%x",                  \
             unsigned int: "%x",                \
             long int: "%lx",                   \
             unsigned long int: "%lx",          \
             long long int: "%llx",             \
             unsigned long long int: "%llx")

#define MOCK_BINARY_ASSERTION(traceback_p,              \
                              func_p,                   \
                              param_p,                  \
                              ignore_in,                \
                              actual,                   \
                              expecetd)                 \
    if (!(ignore_in)) {                                 \
        if ((actual) != (expecetd)) {                   \
            char assert_format[512];                    \
            nala_suspend_all_mocks();                   \
            snprintf(&assert_format[0],                 \
                     sizeof(assert_format),             \
                     "Mocked %s(%s): %s != %s\n\n",     \
                     func_p,                            \
                     param_p,                           \
                     PRINT_FORMAT(actual),              \
                     PRINT_FORMAT(expecetd));           \
            nala_test_failure(                          \
                nala_format_mock_traceback(             \
                    nala_format(&assert_format[0],      \
                                (actual),               \
                                (expecetd)),            \
                    traceback_p));                      \
        }                                               \
    }

#define MOCK_BINARY_ASSERTION_HEX(traceback_p,                          \
                                  func_p,                               \
                                  param_p,                              \
                                  ignore_in,                            \
                                  actual,                               \
                                  expecetd)                             \
    if (!(ignore_in)) {                                                 \
        if ((actual) != (expecetd)) {                                   \
            char assert_format[512];                                    \
            nala_suspend_all_mocks();                                   \
            snprintf(&assert_format[0],                                 \
                     sizeof(assert_format),                             \
                     "Mocked %s(%s): %s != %s (0x%s != 0x%s)\n\n",      \
                     func_p,                                            \
                     param_p,                                           \
                     PRINT_FORMAT(actual),                              \
                     PRINT_FORMAT(expecetd),                            \
                     PRINT_FORMAT_HEX(actual),                          \
                     PRINT_FORMAT_HEX(expecetd));                       \
            nala_test_failure(                                          \
                nala_format_mock_traceback(                             \
                    nala_format(&assert_format[0],                      \
                                (actual),                               \
                                (expecetd),                             \
                                (actual),                               \
                                (expecetd)),                            \
                    traceback_p));                                      \
        }                                                               \
    }

#define MOCK_ASSERT_IN_EQ(data_p, func, param)          \
    NALA_MOCK_ASSERT_IN_EQ_FUNC(param)(                 \
        &(data_p)->traceback,                           \
        #func,                                          \
        #param,                                         \
        (data_p)->params.ignore_ ## param ## _in,       \
        param,                                          \
        (data_p)->params.param)

#define MOCK_ASSERT_POINTERS_IN_EQ(data_p, func, param)         \
    nala_mock_assert_in_eq_ptr(                                 \
        &(data_p)->traceback,                                   \
        #func,                                                  \
        #param,                                                 \
        (data_p)->params.ignore_ ## param ## _in,               \
        (const void *)(uintptr_t)param,                         \
        (const void *)(uintptr_t)(data_p)->params.param)

void nala_mock_assert_memory(struct nala_traceback_t *traceback_p,
                             const char *func_p,
                             const char *param_p,
                             const void *left_p,
                             const void *right_p,
                             size_t size)
{
    char assert_format[512];

    if (!nala_check_memory(left_p, right_p, size)) {
        nala_suspend_all_mocks();
        snprintf(&assert_format[0],
                 sizeof(assert_format),
                 "Mocked %s(%s): ",
                 func_p,
                 param_p);
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format_memory(
                    &assert_format[0],
                    left_p,
                    right_p,
                    size),
                traceback_p));
    }
}

void nala_mock_assert_string(struct nala_traceback_t *traceback_p,
                             const char *func_p,
                             const char *param_p,
                             const char *acutal_p,
                             const char *expected_p,
                             size_t size)
{
    (void)size;

    char assert_format[512];

    if (!nala_check_string_equal(acutal_p, expected_p)) {
        nala_suspend_all_mocks();
        snprintf(&assert_format[0],
                 sizeof(assert_format),
                 "Mocked %s(%s):",
                 func_p,
                 param_p);
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format_string(&assert_format[0],
                                   acutal_p,
                                   expected_p),
                traceback_p));
    }
}

void nala_mock_assert_in_string(void *acutal_p, void *expected_p, size_t size)
{
    (void)size;

    nala_assert_string(acutal_p, expected_p, NALA_CHECK_EQ);
}

#define PRIMITIVE_TYPE_ASSERT(type, name)               \
    void nala_mock_assert_ ## name(type *acutal_p,      \
                                   type *expected_p,    \
                                   size_t size)         \
    {                                                   \
        ASSERT_EQ(size, sizeof(*acutal_p));             \
        ASSERT_NE(acutal_p, NULL);                      \
        ASSERT_NE(expected_p, NULL);                    \
        ASSERT_EQ(*acutal_p, *expected_p);              \
    }

PRIMITIVE_TYPE_ASSERT(char, char)
PRIMITIVE_TYPE_ASSERT(signed char, signed_char)
PRIMITIVE_TYPE_ASSERT(unsigned char, unsigned_char)
PRIMITIVE_TYPE_ASSERT(short, short)
PRIMITIVE_TYPE_ASSERT(short int, short_int)
PRIMITIVE_TYPE_ASSERT(signed short, signed_short)
PRIMITIVE_TYPE_ASSERT(signed short int, signed_short_int)
PRIMITIVE_TYPE_ASSERT(unsigned short, unsigned_short)
PRIMITIVE_TYPE_ASSERT(unsigned short int, unsigned_short_int)
PRIMITIVE_TYPE_ASSERT(int, int)
PRIMITIVE_TYPE_ASSERT(signed, signed)
PRIMITIVE_TYPE_ASSERT(signed int, signed_int)
PRIMITIVE_TYPE_ASSERT(unsigned, unsigned)
PRIMITIVE_TYPE_ASSERT(unsigned int, unsigned_int)
PRIMITIVE_TYPE_ASSERT(long, long)
PRIMITIVE_TYPE_ASSERT(long int, long_int)
PRIMITIVE_TYPE_ASSERT(signed long, signed_long)
PRIMITIVE_TYPE_ASSERT(signed long int, signed_long_int)
PRIMITIVE_TYPE_ASSERT(unsigned long, unsigned_long)
PRIMITIVE_TYPE_ASSERT(unsigned long int, unsigned_long_int)
PRIMITIVE_TYPE_ASSERT(long long, long_long)
PRIMITIVE_TYPE_ASSERT(long long int, long_long_int)
PRIMITIVE_TYPE_ASSERT(signed long long, signed_long_long)
PRIMITIVE_TYPE_ASSERT(signed long long int, signed_long_long_int)
PRIMITIVE_TYPE_ASSERT(unsigned long long, unsigned_long_long)
PRIMITIVE_TYPE_ASSERT(unsigned long long int, unsigned_long_long_int)
PRIMITIVE_TYPE_ASSERT(float, float)
PRIMITIVE_TYPE_ASSERT(double, double)
PRIMITIVE_TYPE_ASSERT(long double, long_double)

void nala_mock_assert_in_eq_char(struct nala_traceback_t *traceback_p,
                                 const char *func_p,
                                 const char *param_p,
                                 bool ignore_in,
                                 char actual,
                                 char expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_schar(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  signed char actual,
                                  signed char expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_uchar(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  unsigned char actual,
                                  unsigned char expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_short(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  short actual,
                                  short expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_ushort(struct nala_traceback_t *traceback_p,
                                   const char *func_p,
                                   const char *param_p,
                                   bool ignore_in,
                                   unsigned short actual,
                                   unsigned short expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_int(struct nala_traceback_t *traceback_p,
                                const char *func_p,
                                const char *param_p,
                                bool ignore_in,
                                int actual,
                                int expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_uint(struct nala_traceback_t *traceback_p,
                                 const char *func_p,
                                 const char *param_p,
                                 bool ignore_in,
                                 unsigned int actual,
                                 unsigned int expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_long(struct nala_traceback_t *traceback_p,
                                 const char *func_p,
                                 const char *param_p,
                                 bool ignore_in,
                                 long actual,
                                 long expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_ulong(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  unsigned long actual,
                                  unsigned long expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_llong(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  long long actual,
                                  long long expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_ullong(struct nala_traceback_t *traceback_p,
                                   const char *func_p,
                                   const char *param_p,
                                   bool ignore_in,
                                   unsigned long long actual,
                                   unsigned long long expected)
{
    MOCK_BINARY_ASSERTION_HEX(traceback_p,
                              func_p,
                              param_p,
                              ignore_in,
                              actual,
                              expected);
}

void nala_mock_assert_in_eq_float(struct nala_traceback_t *traceback_p,
                                  const char *func_p,
                                  const char *param_p,
                                  bool ignore_in,
                                  float actual,
                                  float expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_double(struct nala_traceback_t *traceback_p,
                                   const char *func_p,
                                   const char *param_p,
                                   bool ignore_in,
                                   double actual,
                                   double expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_ldouble(struct nala_traceback_t *traceback_p,
                                    const char *func_p,
                                    const char *param_p,
                                    bool ignore_in,
                                    long double actual,
                                    long double expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_bool(struct nala_traceback_t *traceback_p,
                                 const char *func_p,
                                 const char *param_p,
                                 bool ignore_in,
                                 bool actual,
                                 bool expected)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual,
                          expected);
}

void nala_mock_assert_in_eq_ptr(struct nala_traceback_t *traceback_p,
                                const char *func_p,
                                const char *param_p,
                                bool ignore_in,
                                const void *actual_p,
                                const void *expected_p)
{
    MOCK_BINARY_ASSERTION(traceback_p,
                          func_p,
                          param_p,
                          ignore_in,
                          actual_p,
                          expected_p);
}

void nala_mock_assert_in_eq_string(struct nala_traceback_t *traceback_p,
                                   const char *func_p,
                                   const char *param_p,
                                   bool ignore_in,
                                   const char *acutal_p,
                                   const char *expected_p)
{
    if (!ignore_in) {
        nala_mock_assert_string(traceback_p,
                                func_p,
                                param_p,
                                acutal_p,
                                expected_p,
                                0);
    }
}

void nala_mock_current_set(const char *func_p,
                           const char *param_p,
                           struct nala_traceback_t *traceback_p)
{
    nala_mock_func_p = func_p;
    nala_mock_param_p = param_p;
    nala_mock_traceback_p = traceback_p;
}

void nala_mock_current_clear(void)
{
    nala_mock_func_p = NULL;
}

bool nala_mock_current_is_set(void)
{
    return (nala_mock_func_p != NULL);
}

enum nala_va_arg_item_type_t {
    nala_va_arg_item_type_d_t = 0,
    nala_va_arg_item_type_u_t,
    nala_va_arg_item_type_ld_t,
    nala_va_arg_item_type_lu_t,
    nala_va_arg_item_type_p_t,
    nala_va_arg_item_type_s_t
};

struct nala_va_arg_item_t {
    enum nala_va_arg_item_type_t type;
    bool ignore_in;
    union {
        int d;
        unsigned int u;
        long ld;
        unsigned long lu;
        const void *p_p;
        const char *s_p;
    };
    struct nala_set_param in;
    struct nala_set_param out;
    nala_mock_in_assert_t in_assert;
    nala_mock_out_copy_t out_copy;
    struct nala_va_arg_item_t *next_p;
};

struct nala_va_arg_list_t {
    struct nala_va_arg_item_t *head_p;
    struct nala_va_arg_item_t *tail_p;
    unsigned int length;
    struct nala_traceback_t *traceback_p;
    const char *func_p;
};

struct nala_suspended_t {
    int count;
    int mode;
};

struct nala_state_t {
    int mode;
    struct nala_suspended_t suspended;
};

void nala_set_param_init(struct nala_set_param *self_p)
{
    self_p->buf_p = NULL;
    self_p->size = 0;
}

void nala_set_param_buf(struct nala_set_param *self_p,
                        const void *buf_p,
                        size_t size)
{
    self_p->buf_p = nala_xmalloc(size);
    self_p->size = size;
    memcpy(self_p->buf_p, buf_p, size);
}

void nala_set_param_string(struct nala_set_param *self_p, const char *string_p)
{
    nala_set_param_buf(self_p, string_p, strlen(string_p) + 1);
}

const char *va_arg_type_specifier_string(enum nala_va_arg_item_type_t type)
{
    const char *res_p;

    switch (type) {

    case nala_va_arg_item_type_d_t:
        res_p = "d";
        break;

    case nala_va_arg_item_type_u_t:
        res_p = "u";
        break;

    case nala_va_arg_item_type_ld_t:
        res_p = "ld";
        break;

    case nala_va_arg_item_type_lu_t:
        res_p = "lu";
        break;

    case nala_va_arg_item_type_p_t:
        res_p = "p";
        break;

    case nala_va_arg_item_type_s_t:
        res_p = "s";
        break;

    default:
        nala_test_failure(nala_format("Nala internal failure.\n"));
        exit(1);
        break;
    }

    return (res_p);
}

void nala_va_arg_list_init(struct nala_va_arg_list_t *self_p,
                           struct nala_traceback_t *traceback_p,
                           const char *func_p)
{
    self_p->head_p = NULL;
    self_p->tail_p = NULL;
    self_p->length = 0;
    self_p->traceback_p = traceback_p;
    self_p->func_p = func_p;
}

void nala_va_arg_list_destroy(struct nala_va_arg_list_t *self_p)
{
    struct nala_va_arg_item_t *item_p;
    struct nala_va_arg_item_t *tmp_p;

    item_p = self_p->head_p;

    while (item_p != NULL) {
        if (item_p->in.buf_p != NULL) {
            nala_free(item_p->in.buf_p);
        }

        if (item_p->out.buf_p) {
            nala_free(item_p->out.buf_p);
        }

        tmp_p = item_p;
        item_p = tmp_p->next_p;
        nala_free(tmp_p);
    }
}

void nala_va_arg_list_append(struct nala_va_arg_list_t *self_p,
                             struct nala_va_arg_item_t *item_p)
{
    self_p->length++;

    if (self_p->head_p == NULL) {
        self_p->head_p = item_p;
    } else {
        self_p->tail_p->next_p = item_p;
    }

    item_p->next_p = NULL;
    self_p->tail_p = item_p;
}

struct nala_va_arg_item_t *nala_va_arg_list_get(
    struct nala_va_arg_list_t *self_p,
    unsigned int index)
{
    unsigned int i;
    struct nala_va_arg_item_t *item_p;

    if (index >= self_p->length) {
        nala_test_failure(
            nala_format(
                "Trying to access variable argument at index %u when only %u "
                "exists.\n",
                index,
                self_p->length));
    }

    item_p = self_p->head_p;

    for (i = 0; (i < index) && (item_p != NULL); i++) {
        item_p = item_p->next_p;
    }

    return (item_p);
}

void nala_parse_va_arg_long(const char **format_pp,
                            va_list *vl_p,
                            struct nala_va_arg_item_t *item_p)
{
    switch (**format_pp) {

    case 'd':
        item_p->type = nala_va_arg_item_type_ld_t;

        if (vl_p != NULL) {
            item_p->ignore_in = false;
            item_p->ld = va_arg(*vl_p, long);
        } else {
            item_p->ignore_in = true;
            item_p->ld = 0;
        }

        break;

    case 'u':
        item_p->type = nala_va_arg_item_type_lu_t;

        if (vl_p != NULL) {
            item_p->ignore_in = false;
            item_p->lu = va_arg(*vl_p, unsigned long);
        } else {
            item_p->ignore_in = true;
            item_p->lu = 0;
        }

        break;

    default:
        nala_free(item_p);
        nala_test_failure(
            nala_format("Unsupported type specifier %%l%c.\n", **format_pp));
        exit(1);
        break;
    }

    (*format_pp)++;
}

void nala_parse_va_arg_non_long(const char **format_pp,
                                va_list *vl_p,
                                struct nala_va_arg_item_t *item_p)
{
    const char *string_p;

    switch (**format_pp) {

    case 'd':
        item_p->type = nala_va_arg_item_type_d_t;

        if (vl_p != NULL) {
            item_p->ignore_in = false;
            item_p->d = va_arg(*vl_p, int);
        } else {
            item_p->ignore_in = true;
            item_p->d = 0;
        }

        break;

    case 'u':
        item_p->type = nala_va_arg_item_type_u_t;

        if (vl_p != NULL) {
            item_p->ignore_in = false;
            item_p->u = va_arg(*vl_p, unsigned int);
        } else {
            item_p->ignore_in = true;
            item_p->u = 0;
        }

        break;

    case 'p':
        item_p->type = nala_va_arg_item_type_p_t;
        item_p->ignore_in = true;
        item_p->p_p = NULL;
        break;

    case 's':
        item_p->type = nala_va_arg_item_type_s_t;
        item_p->s_p = NULL;

        if (vl_p != NULL) {
            string_p = va_arg(*vl_p, char *);

            if (string_p != NULL) {
                item_p->ignore_in = true;
                nala_set_param_string(&item_p->in, string_p);
            } else {
                item_p->ignore_in = false;
            }
        } else {
            item_p->ignore_in = true;
        }

        break;

    default:
        nala_free(item_p);
        nala_test_failure(
            nala_format("Unsupported type specifier %%%c.\n", **format_pp));
        exit(1);
        break;
    }

    (*format_pp)++;
}

struct nala_va_arg_item_t *nala_parse_va_arg(const char **format_pp,
                                             va_list *vl_p)
{
    struct nala_va_arg_item_t *item_p;

    item_p = nala_xmalloc(sizeof(*item_p));
    item_p->in.buf_p = NULL;
    item_p->out.buf_p = NULL;
    item_p->in_assert = NULL;
    item_p->out_copy = NULL;

    if (**format_pp == 'l') {
        (*format_pp)++;
        nala_parse_va_arg_long(format_pp, vl_p, item_p);
    } else {
        nala_parse_va_arg_non_long(format_pp, vl_p, item_p);
    }

    return (item_p);
}

void nala_va_arg_copy_out(void *dst_p, struct nala_va_arg_item_t *self_p)
{
    if (self_p->out.buf_p != NULL) {
        if (self_p->out_copy != NULL) {
            self_p->out_copy(dst_p, self_p->out.buf_p, self_p->out.size);
        } else {
            memcpy(dst_p, self_p->out.buf_p, self_p->out.size);
        }
    }
}

void nala_parse_va_list(struct nala_va_arg_list_t *list_p,
                        const char *format_p,
                        va_list *vl_p)
{
    struct nala_va_arg_item_t *item_p;

    if (format_p == NULL) {
        nala_test_failure(
            nala_format(
                "Mocked variadic function format must be a string, not NULL.\n"));
    }

    while (true) {
        if (*format_p == '\0') {
            break;
        } else if (*format_p == '%') {
            format_p++;
            item_p = nala_parse_va_arg(&format_p, vl_p);
            nala_va_arg_list_append(list_p, item_p);
        } else {
            nala_test_failure(
                nala_format("Bad format string '%s'.\n", format_p));
        }
    }
}

void nala_va_arg_list_assert_d(struct nala_va_arg_list_t *self_p,
                               struct nala_va_arg_item_t *item_p,
                               int value)
{
    nala_mock_assert_in_eq_int(self_p->traceback_p,
                               self_p->func_p,
                               "...",
                               item_p->ignore_in,
                               value,
                               item_p->d);
}

void nala_va_arg_list_assert_u(struct nala_va_arg_list_t *self_p,
                               struct nala_va_arg_item_t *item_p,
                               unsigned int value)
{
    nala_mock_assert_in_eq_ulong(self_p->traceback_p,
                                 self_p->func_p,
                                 "...",
                                 item_p->ignore_in,
                                 value,
                                 item_p->u);
}

void nala_va_arg_list_assert_ld(struct nala_va_arg_list_t *self_p,
                                struct nala_va_arg_item_t *item_p,
                                long value)
{
    nala_mock_assert_in_eq_long(self_p->traceback_p,
                                self_p->func_p,
                                "...",
                                item_p->ignore_in,
                                value,
                                item_p->ld);
}

void nala_va_arg_list_assert_lu(struct nala_va_arg_list_t *self_p,
                                struct nala_va_arg_item_t *item_p,
                                unsigned long value)
{
    nala_mock_assert_in_eq_ulong(self_p->traceback_p,
                                 self_p->func_p,
                                 "...",
                                 item_p->ignore_in,
                                 value,
                                 item_p->lu);
}

void nala_va_arg_list_assert_p(struct nala_va_arg_list_t *self_p,
                               struct nala_va_arg_item_t *item_p,
                               void *value_p)
{
    nala_mock_assert_in_eq_ptr(self_p->traceback_p,
                               self_p->func_p,
                               "...",
                               item_p->ignore_in,
                               value_p,
                               item_p->p_p);

    if (item_p->in.buf_p != NULL) {
        if (item_p->in_assert != NULL) {
            nala_mock_current_set(self_p->func_p, "...", self_p->traceback_p);
            item_p->in_assert(value_p, item_p->in.buf_p, item_p->in.size);
            nala_mock_current_clear();
        } else {
            nala_mock_assert_memory(self_p->traceback_p,
                                    self_p->func_p,
                                    "...",
                                    value_p,
                                    item_p->in.buf_p,
                                    item_p->in.size);
        }
    }

    nala_va_arg_copy_out(value_p, item_p);
}

void nala_va_arg_list_assert_s(struct nala_va_arg_list_t *self_p,
                               struct nala_va_arg_item_t *item_p,
                               char *value_p)
{
    nala_mock_assert_in_eq_ptr(self_p->traceback_p,
                               self_p->func_p,
                               "...",
                               item_p->ignore_in,
                               value_p,
                               item_p->s_p);

    if (item_p->in.buf_p != NULL) {
        if (item_p->in_assert != NULL) {
            nala_mock_current_set(self_p->func_p, "...", self_p->traceback_p);
            item_p->in_assert(value_p, item_p->in.buf_p, item_p->in.size);
            nala_mock_current_clear();
        } else {
            nala_mock_assert_string(self_p->traceback_p,
                                    self_p->func_p,
                                    "...",
                                    value_p,
                                    item_p->in.buf_p,
                                    0);
        }
    }

    nala_va_arg_copy_out(value_p, item_p);
}

void nala_va_arg_list_set_param_buf_in_at(struct nala_va_arg_list_t *self_p,
                                          unsigned int index,
                                          const void *buf_p,
                                          size_t size)
{
    struct nala_va_arg_item_t *item_p;

    item_p = nala_va_arg_list_get(self_p, index);

    switch (item_p->type) {

    case nala_va_arg_item_type_p_t:
        nala_set_param_buf(&item_p->in, buf_p, size);
        break;

    default:
        nala_test_failure(
            nala_format(
                "Cannot set input for '%%%s' at index %u. Only '%%p' can be set.\n",
                va_arg_type_specifier_string(item_p->type),
                index));
        exit(1);
        break;
    }
}

void nala_va_arg_list_set_param_buf_out_at(struct nala_va_arg_list_t *self_p,
                                           unsigned int index,
                                           const void *buf_p,
                                           size_t size)
{
    struct nala_va_arg_item_t *item_p;

    item_p = nala_va_arg_list_get(self_p, index);

    switch (item_p->type) {

    case nala_va_arg_item_type_p_t:
    case nala_va_arg_item_type_s_t:
        nala_set_param_buf(&item_p->out, buf_p, size);
        break;

    default:
        nala_test_failure(
            nala_format(
                "Cannot set output for '%%%s' at index %u. Only '%%p' and '%%s' "
                "can be set.\n",
                va_arg_type_specifier_string(item_p->type),
                index));
        exit(1);
        break;
    }
}

void nala_va_arg_list_assert(struct nala_va_arg_list_t *self_p,
                             va_list vl)
{
    unsigned int i;
    struct nala_va_arg_item_t *item_p;

    item_p = self_p->head_p;

    for (i = 0; i < self_p->length; i++) {
        switch (item_p->type) {

        case nala_va_arg_item_type_d_t:
            nala_va_arg_list_assert_d(self_p, item_p, va_arg(vl, int));
            break;

        case nala_va_arg_item_type_u_t:
            nala_va_arg_list_assert_u(self_p, item_p, va_arg(vl, unsigned int));
            break;

        case nala_va_arg_item_type_ld_t:
            nala_va_arg_list_assert_ld(self_p, item_p, va_arg(vl, long));
            break;

        case nala_va_arg_item_type_lu_t:
            nala_va_arg_list_assert_lu(self_p, item_p, va_arg(vl, unsigned long));
            break;

        case nala_va_arg_item_type_p_t:
            nala_va_arg_list_assert_p(self_p, item_p, va_arg(vl, void *));
            break;

        case nala_va_arg_item_type_s_t:
            nala_va_arg_list_assert_s(self_p, item_p, va_arg(vl, char *));
            break;

        default:
            nala_test_failure(nala_format("Nala internal failure.\n"));
            exit(1);
            break;
        }

        item_p = item_p->next_p;
    }
}

void nala_traceback(struct nala_traceback_t *traceback_p)
{
    traceback_p->depth = backtrace(&traceback_p->addresses[0], 32);
}

#define MOCK_ASSERT_PARAM_IN_EQ(traceback_p,            \
                                format_p,               \
                                member_p,               \
                                left,                   \
                                right)                  \
    if ((left) != (right)) {                            \
        char _nala_assert_format[512];                  \
        nala_suspend_all_mocks();                       \
        snprintf(&_nala_assert_format[0],               \
                 sizeof(_nala_assert_format),           \
                 format_p,                              \
                 member_p,                              \
                 PRINT_FORMAT(left),                    \
                 PRINT_FORMAT(right));                  \
        nala_test_failure(                              \
            nala_format_mock_traceback(                 \
                nala_format(&_nala_assert_format[0],    \
                            left,                       \
                            right),                     \
                traceback_p));                          \
    }

#define MOCK_ASSERT_PARAM_IN(data_p, func, name)                \
    nala_mock_current_set(#func, #name, &(data_p)->traceback);  \
    (data_p)->params.name ## _in_assert(                        \
        name,                                                   \
        (__typeof__((data_p)->params.name))(uintptr_t)(data_p)  \
        ->params.name ## _in.buf_p,                             \
        (data_p)->params.name ## _in.size);                     \
    nala_mock_current_clear();

#define MOCK_COPY_PARAM_OUT(params_p, name)             \
    if ((params_p)->name ## _out_copy == NULL) {        \
        memcpy((void *)(uintptr_t)name,                 \
               (params_p)->name ## _out.buf_p,          \
               (params_p)->name ## _out.size);          \
    } else {                                            \
        (params_p)->name ## _out_copy(                  \
            name,                                       \
            (__typeof__((params_p)->name))(uintptr_t)   \
            (params_p)->name ## _out.buf_p,             \
            (params_p)->name ## _out.size);             \
    }

#define MOCK_ASSERT_COPY_SET_PARAM(instance_p, data_p, func, name)      \
    if ((data_p)->params.name ## _in.buf_p != NULL) {                   \
        MOCK_ASSERT_PARAM_IN(data_p, func, name);                       \
        if (instance_p != NULL) {                                       \
            nala_free((data_p)->params.name ## _in.buf_p);              \
        }                                                               \
    }                                                                   \
                                                                        \
    if ((data_p)->params.name ## _out.buf_p != NULL) {                  \
        MOCK_COPY_PARAM_OUT(&(data_p)->params, name);                   \
        if (instance_p != NULL) {                                       \
            nala_free((data_p)->params.name ## _out.buf_p);             \
        }                                                               \
    }

void nala_state_suspend(struct nala_state_t *state_p)
{
    if (state_p->suspended.count == 0) {
        state_p->suspended.mode = state_p->mode;
        state_p->mode = MODE_REAL;
    }

    state_p->suspended.count++;
}

void nala_state_resume(struct nala_state_t *state_p)
{
    state_p->suspended.count--;

    if (state_p->suspended.count == 0) {
        state_p->mode = state_p->suspended.mode;
    }
}

void nala_mock_none_fail(struct nala_traceback_t *traceback_p,
                         const char *func_p)
{
    nala_test_failure(
        nala_format_mock_traceback(
            nala_format("Mocked %s() called unexpectedly.\n\n",
                        func_p),
            traceback_p));
}

int nala_print_call_mask = 0;

void nala_print_call(const char *function_name_p, struct nala_state_t *state_p)
{
    const char *mode_p;

    if (state_p->suspended.count != 0) {
        return;
    }

    if (((1 << state_p->mode) & nala_print_call_mask) == 0) {
        return;
    }

    switch (state_p->mode) {

    case MODE_REAL:
        mode_p = "real";
        break;

    case MODE_MOCK_ONCE:
        mode_p = "once";
        break;

    case MODE_IMPLEMENTATION:
        mode_p = "impl";
        break;

    case MODE_MOCK:
        mode_p = "mock";
        break;

    case MODE_NONE:
        mode_p = "none";
        break;

    default:
        mode_p = "unknown";
        break;
    }

    fprintf(nala_get_stdout(), "%s: %s()\n", mode_p, function_name_p);
}

int nala_next_handle = 1;

void nala_suspend_all_mocks(void)
{
    bar_mock_suspend();
    fie_mock_suspend();
    foo_mock_suspend();
    fum_mock_suspend();
    gam_mock_suspend();
    hit_mock_suspend();
}

void nala_resume_all_mocks(void)
{
    bar_mock_resume();
    fie_mock_resume();
    foo_mock_resume();
    fum_mock_resume();
    gam_mock_resume();
    hit_mock_resume();
}

void nala_reset_all_mocks(void)
{
    bar_mock_reset();
    fie_mock_reset();
    foo_mock_reset();
    fum_mock_reset();
    gam_mock_reset();
    hit_mock_reset();
}

void nala_assert_all_mocks_completed(void)
{
    bar_mock_assert_completed();
    fie_mock_assert_completed();
    foo_mock_assert_completed();
    fum_mock_assert_completed();
    gam_mock_assert_completed();
    hit_mock_assert_completed();
}

// NALA_IMPLEMENTATION bar

int __real_bar();

struct nala_params_bar_t {
    int dummy;
};

struct nala_data_bar_t {
    struct nala_params_bar_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_bar_params_t params_in;
};

struct nala_instance_bar_t {
    int mode;
    int handle;
    struct nala_data_bar_t data;
    struct nala_instance_bar_t *next_p;
};

struct nala_instances_bar_t {
    struct nala_instance_bar_t *head_p;
    struct nala_instance_bar_t *next_p;
    struct nala_instance_bar_t *tail_p;
    int length;
};

struct nala_mock_bar_t {
    struct nala_state_t state;
    struct nala_data_bar_t data;
    struct nala_instances_bar_t instances;
};

static struct nala_mock_bar_t nala_mock_bar = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_bar_t *nala_get_data_bar()
{
    if (nala_mock_bar.instances.tail_p != NULL) {
        return (&nala_mock_bar.instances.tail_p->data);
    } else {
        return (&nala_mock_bar.data);
    }
}

struct nala_params_bar_t *nala_get_params_bar()
{
    return (&nala_get_data_bar()->params);
}

int __wrap_bar()
{
    struct nala_instance_bar_t *nala_instance_p;
    struct nala_data_bar_t *nala_data_p;
    int return_value;

    nala_print_call("bar", &nala_mock_bar.state);

    switch (nala_mock_bar.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_bar.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_bar.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked bar() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_bar.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_bar();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_bar.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_bar.data.traceback,
                            "bar");
        exit(1);
        break;

    default:
        return_value =
        __real_bar();
        break;
    }

    return return_value;
}

void bar_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_bar);
    nala_mock_bar.state.mode = MODE_MOCK;
    nala_mock_bar.data.return_value = return_value;
    nala_mock_bar.data.errno_value = 0;
    nala_mock_bar.data.callback = NULL;
    nala_traceback(&nala_mock_bar.data.traceback);
}

int bar_mock_once(int return_value)
{
    struct nala_instance_bar_t *nala_instance_p;

    nala_mock_bar.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_bar.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void bar_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_bar);
    nala_mock_bar.state.mode = MODE_MOCK;
    nala_mock_bar.data.return_value = return_value;
    nala_mock_bar.data.errno_value = 0;
    nala_mock_bar.data.callback = NULL;
}

int bar_mock_ignore_in_once(int return_value)
{
    struct nala_instance_bar_t *instance_p;

    nala_mock_bar.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_bar.instances,
                          instance_p);

    return (instance_p->handle);
}

void bar_mock_set_errno(int errno_value)
{
    nala_get_data_bar()->errno_value = errno_value;
}

void bar_mock_set_callback(void (*callback)())
{
    nala_get_data_bar()->callback = callback;
}

struct nala_bar_params_t *bar_mock_get_params_in(int handle)
{
    struct nala_instance_bar_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_bar.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "bar() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void bar_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_bar);
    nala_mock_bar.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_bar.data.traceback);
}

void bar_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_bar);
    nala_mock_bar.state.mode = MODE_IMPLEMENTATION;
    nala_mock_bar.data.implementation = implementation;
    nala_traceback(&nala_mock_bar.data.traceback);
}

void bar_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_bar);
    nala_mock_bar.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_bar.data.traceback);
}

void bar_mock_real_once(void)
{
    struct nala_instance_bar_t *instance_p;

    nala_mock_bar.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_bar.instances,
                          instance_p);
}

void bar_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_bar.state);
}

void bar_mock_resume(void)
{
    nala_state_resume(&nala_mock_bar.state);
}

void bar_mock_reset(void)
{
    struct nala_instance_bar_t *current_p;
    struct nala_instance_bar_t *tmp_p;

    NALA_STATE_RESET(nala_mock_bar, current_p, tmp_p);
}

void bar_mock_assert_completed(void)
{
    if (nala_mock_bar.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked bar() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_bar.instances.length),
                &nala_mock_bar.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fie

int __real_fie();

struct nala_params_fie_t {
    int dummy;
};

struct nala_data_fie_t {
    struct nala_params_fie_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_fie_params_t params_in;
};

struct nala_instance_fie_t {
    int mode;
    int handle;
    struct nala_data_fie_t data;
    struct nala_instance_fie_t *next_p;
};

struct nala_instances_fie_t {
    struct nala_instance_fie_t *head_p;
    struct nala_instance_fie_t *next_p;
    struct nala_instance_fie_t *tail_p;
    int length;
};

struct nala_mock_fie_t {
    struct nala_state_t state;
    struct nala_data_fie_t data;
    struct nala_instances_fie_t instances;
};

static struct nala_mock_fie_t nala_mock_fie = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_fie_t *nala_get_data_fie()
{
    if (nala_mock_fie.instances.tail_p != NULL) {
        return (&nala_mock_fie.instances.tail_p->data);
    } else {
        return (&nala_mock_fie.data);
    }
}

struct nala_params_fie_t *nala_get_params_fie()
{
    return (&nala_get_data_fie()->params);
}

int __wrap_fie()
{
    struct nala_instance_fie_t *nala_instance_p;
    struct nala_data_fie_t *nala_data_p;
    int return_value;

    nala_print_call("fie", &nala_mock_fie.state);

    switch (nala_mock_fie.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fie.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fie.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fie() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fie.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fie();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fie.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fie.data.traceback,
                            "fie");
        exit(1);
        break;

    default:
        return_value =
        __real_fie();
        break;
    }

    return return_value;
}

void fie_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fie);
    nala_mock_fie.state.mode = MODE_MOCK;
    nala_mock_fie.data.return_value = return_value;
    nala_mock_fie.data.errno_value = 0;
    nala_mock_fie.data.callback = NULL;
    nala_traceback(&nala_mock_fie.data.traceback);
}

int fie_mock_once(int return_value)
{
    struct nala_instance_fie_t *nala_instance_p;

    nala_mock_fie.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fie.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fie_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fie);
    nala_mock_fie.state.mode = MODE_MOCK;
    nala_mock_fie.data.return_value = return_value;
    nala_mock_fie.data.errno_value = 0;
    nala_mock_fie.data.callback = NULL;
}

int fie_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fie_t *instance_p;

    nala_mock_fie.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fie.instances,
                          instance_p);

    return (instance_p->handle);
}

void fie_mock_set_errno(int errno_value)
{
    nala_get_data_fie()->errno_value = errno_value;
}

void fie_mock_set_callback(void (*callback)())
{
    nala_get_data_fie()->callback = callback;
}

struct nala_fie_params_t *fie_mock_get_params_in(int handle)
{
    struct nala_instance_fie_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fie.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fie() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fie_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fie);
    nala_mock_fie.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fie.data.traceback);
}

void fie_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_fie);
    nala_mock_fie.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fie.data.implementation = implementation;
    nala_traceback(&nala_mock_fie.data.traceback);
}

void fie_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fie);
    nala_mock_fie.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fie.data.traceback);
}

void fie_mock_real_once(void)
{
    struct nala_instance_fie_t *instance_p;

    nala_mock_fie.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fie.instances,
                          instance_p);
}

void fie_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fie.state);
}

void fie_mock_resume(void)
{
    nala_state_resume(&nala_mock_fie.state);
}

void fie_mock_reset(void)
{
    struct nala_instance_fie_t *current_p;
    struct nala_instance_fie_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fie, current_p, tmp_p);
}

void fie_mock_assert_completed(void)
{
    if (nala_mock_fie.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked fie() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fie.instances.length),
                &nala_mock_fie.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION foo

int __real_foo();

struct nala_params_foo_t {
    int dummy;
};

struct nala_data_foo_t {
    struct nala_params_foo_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_foo_params_t params_in;
};

struct nala_instance_foo_t {
    int mode;
    int handle;
    struct nala_data_foo_t data;
    struct nala_instance_foo_t *next_p;
};

struct nala_instances_foo_t {
    struct nala_instance_foo_t *head_p;
    struct nala_instance_foo_t *next_p;
    struct nala_instance_foo_t *tail_p;
    int length;
};

struct nala_mock_foo_t {
    struct nala_state_t state;
    struct nala_data_foo_t data;
    struct nala_instances_foo_t instances;
};

static struct nala_mock_foo_t nala_mock_foo = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_foo_t *nala_get_data_foo()
{
    if (nala_mock_foo.instances.tail_p != NULL) {
        return (&nala_mock_foo.instances.tail_p->data);
    } else {
        return (&nala_mock_foo.data);
    }
}

struct nala_params_foo_t *nala_get_params_foo()
{
    return (&nala_get_data_foo()->params);
}

int __wrap_foo()
{
    struct nala_instance_foo_t *nala_instance_p;
    struct nala_data_foo_t *nala_data_p;
    int return_value;

    nala_print_call("foo", &nala_mock_foo.state);

    switch (nala_mock_foo.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_foo.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_foo.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked foo() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_foo.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_foo();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_foo.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_foo.data.traceback,
                            "foo");
        exit(1);
        break;

    default:
        return_value =
        __real_foo();
        break;
    }

    return return_value;
}

void foo_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_foo);
    nala_mock_foo.state.mode = MODE_MOCK;
    nala_mock_foo.data.return_value = return_value;
    nala_mock_foo.data.errno_value = 0;
    nala_mock_foo.data.callback = NULL;
    nala_traceback(&nala_mock_foo.data.traceback);
}

int foo_mock_once(int return_value)
{
    struct nala_instance_foo_t *nala_instance_p;

    nala_mock_foo.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_foo.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void foo_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_foo);
    nala_mock_foo.state.mode = MODE_MOCK;
    nala_mock_foo.data.return_value = return_value;
    nala_mock_foo.data.errno_value = 0;
    nala_mock_foo.data.callback = NULL;
}

int foo_mock_ignore_in_once(int return_value)
{
    struct nala_instance_foo_t *instance_p;

    nala_mock_foo.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_foo.instances,
                          instance_p);

    return (instance_p->handle);
}

void foo_mock_set_errno(int errno_value)
{
    nala_get_data_foo()->errno_value = errno_value;
}

void foo_mock_set_callback(void (*callback)())
{
    nala_get_data_foo()->callback = callback;
}

struct nala_foo_params_t *foo_mock_get_params_in(int handle)
{
    struct nala_instance_foo_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_foo.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "foo() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void foo_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_foo);
    nala_mock_foo.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_foo.data.traceback);
}

void foo_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_foo);
    nala_mock_foo.state.mode = MODE_IMPLEMENTATION;
    nala_mock_foo.data.implementation = implementation;
    nala_traceback(&nala_mock_foo.data.traceback);
}

void foo_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_foo);
    nala_mock_foo.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_foo.data.traceback);
}

void foo_mock_real_once(void)
{
    struct nala_instance_foo_t *instance_p;

    nala_mock_foo.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_foo.instances,
                          instance_p);
}

void foo_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_foo.state);
}

void foo_mock_resume(void)
{
    nala_state_resume(&nala_mock_foo.state);
}

void foo_mock_reset(void)
{
    struct nala_instance_foo_t *current_p;
    struct nala_instance_foo_t *tmp_p;

    NALA_STATE_RESET(nala_mock_foo, current_p, tmp_p);
}

void foo_mock_assert_completed(void)
{
    if (nala_mock_foo.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked foo() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_foo.instances.length),
                &nala_mock_foo.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fum

int __real_fum();

struct nala_params_fum_t {
    int dummy;
};

struct nala_data_fum_t {
    struct nala_params_fum_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_fum_params_t params_in;
};

struct nala_instance_fum_t {
    int mode;
    int handle;
    struct nala_data_fum_t data;
    struct nala_instance_fum_t *next_p;
};

struct nala_instances_fum_t {
    struct nala_instance_fum_t *head_p;
    struct nala_instance_fum_t *next_p;
    struct nala_instance_fum_t *tail_p;
    int length;
};

struct nala_mock_fum_t {
    struct nala_state_t state;
    struct nala_data_fum_t data;
    struct nala_instances_fum_t instances;
};

static struct nala_mock_fum_t nala_mock_fum = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_fum_t *nala_get_data_fum()
{
    if (nala_mock_fum.instances.tail_p != NULL) {
        return (&nala_mock_fum.instances.tail_p->data);
    } else {
        return (&nala_mock_fum.data);
    }
}

struct nala_params_fum_t *nala_get_params_fum()
{
    return (&nala_get_data_fum()->params);
}

int __wrap_fum()
{
    struct nala_instance_fum_t *nala_instance_p;
    struct nala_data_fum_t *nala_data_p;
    int return_value;

    nala_print_call("fum", &nala_mock_fum.state);

    switch (nala_mock_fum.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fum.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fum.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fum() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fum.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fum();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fum.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fum.data.traceback,
                            "fum");
        exit(1);
        break;

    default:
        return_value =
        __real_fum();
        break;
    }

    return return_value;
}

void fum_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fum);
    nala_mock_fum.state.mode = MODE_MOCK;
    nala_mock_fum.data.return_value = return_value;
    nala_mock_fum.data.errno_value = 0;
    nala_mock_fum.data.callback = NULL;
    nala_traceback(&nala_mock_fum.data.traceback);
}

int fum_mock_once(int return_value)
{
    struct nala_instance_fum_t *nala_instance_p;

    nala_mock_fum.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fum.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fum_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fum);
    nala_mock_fum.state.mode = MODE_MOCK;
    nala_mock_fum.data.return_value = return_value;
    nala_mock_fum.data.errno_value = 0;
    nala_mock_fum.data.callback = NULL;
}

int fum_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fum_t *instance_p;

    nala_mock_fum.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fum.instances,
                          instance_p);

    return (instance_p->handle);
}

void fum_mock_set_errno(int errno_value)
{
    nala_get_data_fum()->errno_value = errno_value;
}

void fum_mock_set_callback(void (*callback)())
{
    nala_get_data_fum()->callback = callback;
}

struct nala_fum_params_t *fum_mock_get_params_in(int handle)
{
    struct nala_instance_fum_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fum.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fum() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fum_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fum);
    nala_mock_fum.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fum.data.traceback);
}

void fum_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_fum);
    nala_mock_fum.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fum.data.implementation = implementation;
    nala_traceback(&nala_mock_fum.data.traceback);
}

void fum_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fum);
    nala_mock_fum.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fum.data.traceback);
}

void fum_mock_real_once(void)
{
    struct nala_instance_fum_t *instance_p;

    nala_mock_fum.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fum.instances,
                          instance_p);
}

void fum_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fum.state);
}

void fum_mock_resume(void)
{
    nala_state_resume(&nala_mock_fum.state);
}

void fum_mock_reset(void)
{
    struct nala_instance_fum_t *current_p;
    struct nala_instance_fum_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fum, current_p, tmp_p);
}

void fum_mock_assert_completed(void)
{
    if (nala_mock_fum.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked fum() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fum.instances.length),
                &nala_mock_fum.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION gam

int __real_gam();

struct nala_params_gam_t {
    int dummy;
};

struct nala_data_gam_t {
    struct nala_params_gam_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_gam_params_t params_in;
};

struct nala_instance_gam_t {
    int mode;
    int handle;
    struct nala_data_gam_t data;
    struct nala_instance_gam_t *next_p;
};

struct nala_instances_gam_t {
    struct nala_instance_gam_t *head_p;
    struct nala_instance_gam_t *next_p;
    struct nala_instance_gam_t *tail_p;
    int length;
};

struct nala_mock_gam_t {
    struct nala_state_t state;
    struct nala_data_gam_t data;
    struct nala_instances_gam_t instances;
};

static struct nala_mock_gam_t nala_mock_gam = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_gam_t *nala_get_data_gam()
{
    if (nala_mock_gam.instances.tail_p != NULL) {
        return (&nala_mock_gam.instances.tail_p->data);
    } else {
        return (&nala_mock_gam.data);
    }
}

struct nala_params_gam_t *nala_get_params_gam()
{
    return (&nala_get_data_gam()->params);
}

int __wrap_gam()
{
    struct nala_instance_gam_t *nala_instance_p;
    struct nala_data_gam_t *nala_data_p;
    int return_value;

    nala_print_call("gam", &nala_mock_gam.state);

    switch (nala_mock_gam.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_gam.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_gam.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked gam() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_gam.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_gam();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_gam.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_gam.data.traceback,
                            "gam");
        exit(1);
        break;

    default:
        return_value =
        __real_gam();
        break;
    }

    return return_value;
}

void gam_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_gam);
    nala_mock_gam.state.mode = MODE_MOCK;
    nala_mock_gam.data.return_value = return_value;
    nala_mock_gam.data.errno_value = 0;
    nala_mock_gam.data.callback = NULL;
    nala_traceback(&nala_mock_gam.data.traceback);
}

int gam_mock_once(int return_value)
{
    struct nala_instance_gam_t *nala_instance_p;

    nala_mock_gam.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_gam.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void gam_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_gam);
    nala_mock_gam.state.mode = MODE_MOCK;
    nala_mock_gam.data.return_value = return_value;
    nala_mock_gam.data.errno_value = 0;
    nala_mock_gam.data.callback = NULL;
}

int gam_mock_ignore_in_once(int return_value)
{
    struct nala_instance_gam_t *instance_p;

    nala_mock_gam.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_gam.instances,
                          instance_p);

    return (instance_p->handle);
}

void gam_mock_set_errno(int errno_value)
{
    nala_get_data_gam()->errno_value = errno_value;
}

void gam_mock_set_callback(void (*callback)())
{
    nala_get_data_gam()->callback = callback;
}

struct nala_gam_params_t *gam_mock_get_params_in(int handle)
{
    struct nala_instance_gam_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_gam.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "gam() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void gam_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_gam);
    nala_mock_gam.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_gam.data.traceback);
}

void gam_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_gam);
    nala_mock_gam.state.mode = MODE_IMPLEMENTATION;
    nala_mock_gam.data.implementation = implementation;
    nala_traceback(&nala_mock_gam.data.traceback);
}

void gam_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_gam);
    nala_mock_gam.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_gam.data.traceback);
}

void gam_mock_real_once(void)
{
    struct nala_instance_gam_t *instance_p;

    nala_mock_gam.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_gam.instances,
                          instance_p);
}

void gam_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_gam.state);
}

void gam_mock_resume(void)
{
    nala_state_resume(&nala_mock_gam.state);
}

void gam_mock_reset(void)
{
    struct nala_instance_gam_t *current_p;
    struct nala_instance_gam_t *tmp_p;

    NALA_STATE_RESET(nala_mock_gam, current_p, tmp_p);
}

void gam_mock_assert_completed(void)
{
    if (nala_mock_gam.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked gam() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_gam.instances.length),
                &nala_mock_gam.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION hit

int __real_hit();

struct nala_params_hit_t {
    int dummy;
};

struct nala_data_hit_t {
    struct nala_params_hit_t params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_hit_params_t params_in;
};

struct nala_instance_hit_t {
    int mode;
    int handle;
    struct nala_data_hit_t data;
    struct nala_instance_hit_t *next_p;
};

struct nala_instances_hit_t {
    struct nala_instance_hit_t *head_p;
    struct nala_instance_hit_t *next_p;
    struct nala_instance_hit_t *tail_p;
    int length;
};

struct nala_mock_hit_t {
    struct nala_state_t state;
    struct nala_data_hit_t data;
    struct nala_instances_hit_t instances;
};

static struct nala_mock_hit_t nala_mock_hit = {
    .state = {
        .mode = MODE_REAL,
        .suspended = {
            .count = 0,
            .mode = MODE_REAL
        }
    },
    .instances = {
        .head_p = NULL,
        .next_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct nala_data_hit_t *nala_get_data_hit()
{
    if (nala_mock_hit.instances.tail_p != NULL) {
        return (&nala_mock_hit.instances.tail_p->data);
    } else {
        return (&nala_mock_hit.data);
    }
}

struct nala_params_hit_t *nala_get_params_hit()
{
    return (&nala_get_data_hit()->params);
}

int __wrap_hit()
{
    struct nala_instance_hit_t *nala_instance_p;
    struct nala_data_hit_t *nala_data_p;
    int return_value;

    nala_print_call("hit", &nala_mock_hit.state);

    switch (nala_mock_hit.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_hit.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_hit.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked hit() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_hit.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_hit();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_hit.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_hit.data.traceback,
                            "hit");
        exit(1);
        break;

    default:
        return_value =
        __real_hit();
        break;
    }

    return return_value;
}

void hit_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_hit);
    nala_mock_hit.state.mode = MODE_MOCK;
    nala_mock_hit.data.return_value = return_value;
    nala_mock_hit.data.errno_value = 0;
    nala_mock_hit.data.callback = NULL;
    nala_traceback(&nala_mock_hit.data.traceback);
}

int hit_mock_once(int return_value)
{
    struct nala_instance_hit_t *nala_instance_p;

    nala_mock_hit.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_hit.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void hit_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_hit);
    nala_mock_hit.state.mode = MODE_MOCK;
    nala_mock_hit.data.return_value = return_value;
    nala_mock_hit.data.errno_value = 0;
    nala_mock_hit.data.callback = NULL;
}

int hit_mock_ignore_in_once(int return_value)
{
    struct nala_instance_hit_t *instance_p;

    nala_mock_hit.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_hit.instances,
                          instance_p);

    return (instance_p->handle);
}

void hit_mock_set_errno(int errno_value)
{
    nala_get_data_hit()->errno_value = errno_value;
}

void hit_mock_set_callback(void (*callback)())
{
    nala_get_data_hit()->callback = callback;
}

struct nala_hit_params_t *hit_mock_get_params_in(int handle)
{
    struct nala_instance_hit_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_hit.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "hit() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void hit_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_hit);
    nala_mock_hit.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_hit.data.traceback);
}

void hit_mock_implementation(int (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_hit);
    nala_mock_hit.state.mode = MODE_IMPLEMENTATION;
    nala_mock_hit.data.implementation = implementation;
    nala_traceback(&nala_mock_hit.data.traceback);
}

void hit_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_hit);
    nala_mock_hit.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_hit.data.traceback);
}

void hit_mock_real_once(void)
{
    struct nala_instance_hit_t *instance_p;

    nala_mock_hit.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_hit.instances,
                          instance_p);
}

void hit_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_hit.state);
}

void hit_mock_resume(void)
{
    nala_state_resume(&nala_mock_hit.state);
}

void hit_mock_reset(void)
{
    struct nala_instance_hit_t *current_p;
    struct nala_instance_hit_t *tmp_p;

    NALA_STATE_RESET(nala_mock_hit, current_p, tmp_p);
}

void hit_mock_assert_completed(void)
{
    if (nala_mock_hit.instances.length != 0) {
        nala_test_failure(
            nala_format_mock_traceback(
                nala_format(
                    "Mocked hit() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_hit.instances.length),
                &nala_mock_hit.instances.head_p->data.traceback));
    }
}

