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

char *format_mock_traceback(const char *message_p,
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

#define MOCK_BINARY_ASSERTION(traceback_p,                              \
                              func_p,                                   \
                              param_p,                                  \
                              ignore_in,                                \
                              actual,                                   \
                              expecetd)                                 \
    if (!(ignore_in)) {                                                 \
        if ((actual) != (expecetd)) {                                   \
            nala_suspend_all_mocks();                                   \
            char _nala_assert_format[512];                              \
            snprintf(&_nala_assert_format[0],                           \
                     sizeof(_nala_assert_format),                       \
                     "Mocked %s(%s): %s != %s\n\n",                     \
                     func_p,                                            \
                     param_p,                                           \
                     PRINT_FORMAT(actual),                              \
                     PRINT_FORMAT(expecetd));                           \
            nala_test_failure(                                          \
                format_mock_traceback(                                  \
                    nala_format(&_nala_assert_format[0],                \
                                (actual),                               \
                                (expecetd)),                            \
                    traceback_p));                                      \
        }                                                               \
    }

#define MOCK_BINARY_ASSERTION_HEX(traceback_p,                          \
                                  func_p,                               \
                                  param_p,                              \
                                  ignore_in,                            \
                                  actual,                               \
                                  expecetd)                             \
    if (!(ignore_in)) {                                                 \
        if ((actual) != (expecetd)) {                                   \
            nala_suspend_all_mocks();                                   \
            char _nala_assert_format[512];                              \
            snprintf(&_nala_assert_format[0],                           \
                     sizeof(_nala_assert_format),                       \
                     "Mocked %s(%s): %s != %s (0x%s != 0x%s)\n\n",      \
                     func_p,                                            \
                     param_p,                                           \
                     PRINT_FORMAT(actual),                              \
                     PRINT_FORMAT(expecetd),                            \
                     PRINT_FORMAT_HEX(actual),                          \
                     PRINT_FORMAT_HEX(expecetd));                       \
            nala_test_failure(                                          \
                format_mock_traceback(                                  \
                    nala_format(&_nala_assert_format[0],                \
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

void nala_mock_assert_memory(struct nala_traceback_t *traceback_p,
                             const char *func_p,
                             const char *param_p,
                             const void *left_p,
                             const void *right_p,
                             size_t size)
{
    char _nala_assert_format[512];

    if (!nala_check_memory(left_p, right_p, size)) {
        nala_suspend_all_mocks();
        snprintf(&_nala_assert_format[0],
                 sizeof(_nala_assert_format),
                 "Mocked %s(%s): ",
                 func_p,
                 param_p);
        nala_test_failure(
            format_mock_traceback(
                nala_format_memory(
                    &_nala_assert_format[0],
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

    char _nala_assert_format[512];

    if (!nala_check_string_equal(acutal_p, expected_p)) {
        nala_suspend_all_mocks();
        snprintf(&_nala_assert_format[0],
                 sizeof(_nala_assert_format),
                 "Mocked %s(%s):",
                 func_p,
                 param_p);
        nala_test_failure(
            format_mock_traceback(
                nala_format_string(&_nala_assert_format[0],
                                   acutal_p,
                                   expected_p),
                traceback_p));
    }
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
                            va_list vl,
                            struct nala_va_arg_item_t *item_p)
{
    switch (**format_pp) {

    case 'd':
        item_p->type = nala_va_arg_item_type_ld_t;
        item_p->ignore_in = false;
        item_p->ld = va_arg(vl, long);
        break;

    case 'u':
        item_p->type = nala_va_arg_item_type_lu_t;
        item_p->ignore_in = false;
        item_p->lu = va_arg(vl, unsigned long);
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
                                va_list vl,
                                struct nala_va_arg_item_t *item_p)
{
    switch (**format_pp) {

    case 'd':
        item_p->type = nala_va_arg_item_type_d_t;
        item_p->ignore_in = false;
        item_p->d = va_arg(vl, int);
        break;

    case 'u':
        item_p->type = nala_va_arg_item_type_u_t;
        item_p->ignore_in = false;
        item_p->u = va_arg(vl, unsigned int);
        break;

    case 'p':
        item_p->type = nala_va_arg_item_type_p_t;
        item_p->ignore_in = true;
        item_p->p_p = NULL;
        break;

    case 's':
        /* ToDo: Should save in buffer. */
        item_p->type = nala_va_arg_item_type_s_t;
        item_p->ignore_in = false;
        item_p->s_p = va_arg(vl, char *);
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
                                             va_list vl)
{
    struct nala_va_arg_item_t *item_p;

    item_p = nala_xmalloc(sizeof(*item_p));
    item_p->in.buf_p = NULL;
    item_p->out.buf_p = NULL;
    item_p->in_assert = NULL;
    item_p->out_copy = NULL;

    if (**format_pp == 'l') {
        (*format_pp)++;
        nala_parse_va_arg_long(format_pp, vl, item_p);
    } else {
        nala_parse_va_arg_non_long(format_pp, vl, item_p);
    }

    return (item_p);
}

void nala_parse_va_list(struct nala_va_arg_list_t *list_p,
                        const char *format_p,
                        va_list vl)
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
            item_p = nala_parse_va_arg(&format_p, vl);
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
            item_p->in_assert(value_p, item_p->in.buf_p, item_p->in.size);
        } else {
            nala_mock_assert_memory(self_p->traceback_p,
                                    self_p->func_p,
                                    "...",
                                    value_p,
                                    item_p->in.buf_p,
                                    item_p->in.size);
        }
    }

    if (item_p->out.buf_p != NULL) {
        if (item_p->out_copy != NULL) {
            item_p->out_copy(value_p, item_p->out.buf_p, item_p->out.size);
        } else {
            memcpy(value_p, item_p->out.buf_p, item_p->out.size);
        }
    }
}

void nala_va_arg_list_assert_s(struct nala_va_arg_list_t *self_p,
                               struct nala_va_arg_item_t *item_p,
                               char *value_p)
{
    if (!item_p->ignore_in) {
        nala_mock_assert_string(self_p->traceback_p,
                                self_p->func_p,
                                "...",
                                value_p,
                                item_p->s_p,
                                strlen(item_p->s_p) + 1);
    }
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
        nala_set_param_buf(&item_p->out, buf_p, size);
        break;

    default:
        nala_test_failure(
            nala_format(
                "Cannot set output for '%%%s' at index %u. Only '%%p' can be set.\n",
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
        nala_suspend_all_mocks();                       \
        char _nala_assert_format[512];                  \
        snprintf(&_nala_assert_format[0],               \
                 sizeof(_nala_assert_format),           \
                 format_p,                              \
                 member_p,                              \
                 PRINT_FORMAT(left),                    \
                 PRINT_FORMAT(right));                  \
        nala_test_failure(                              \
            format_mock_traceback(                      \
                nala_format(&_nala_assert_format[0],    \
                            left,                       \
                            right),                     \
                traceback_p));                          \
    }

#define MOCK_ASSERT_PARAM_IN(data_p, assert_in, func, name)     \
    if ((data_p)->params.name ## _in_assert == NULL) {          \
        assert_in(&(data_p)->traceback,                         \
                  #func,                                        \
                  #name,                                        \
                  (const void *)(uintptr_t)name,                \
                  (data_p)->params.name ## _in.buf_p,           \
                  (data_p)->params.name ## _in.size);           \
    } else {                                                    \
        (data_p)->params.name ## _in_assert(                    \
            name,                                               \
            (data_p)->params.name ## _in.buf_p,                 \
            (data_p)->params.name ## _in.size);                 \
    }

#define MOCK_COPY_PARAM_OUT(params_p, name)             \
    if ((params_p)->name ## _out_copy == NULL) {        \
        memcpy((void *)(uintptr_t)name,                 \
               (params_p)->name ## _out.buf_p,          \
               (params_p)->name ## _out.size);          \
    } else {                                            \
        (params_p)->name ## _out_copy(                  \
            name,                                       \
            (params_p)->name ## _out.buf_p,             \
            (params_p)->name ## _out.size);             \
    }

#define MOCK_ASSERT_COPY_SET_PARAM(instance_p,                  \
                                   data_p,                      \
                                   assert_in,                   \
                                   func,                        \
                                   name)                        \
    if ((data_p)->params.name ## _in.buf_p != NULL) {           \
        MOCK_ASSERT_PARAM_IN(data_p, assert_in, func, name);    \
        if (instance_p != NULL) {                               \
            nala_free((data_p)->params.name ## _in.buf_p);      \
        }                                                       \
    }                                                           \
                                                                \
    if ((data_p)->params.name ## _out.buf_p != NULL) {          \
        MOCK_COPY_PARAM_OUT(&(data_p)->params, name);           \
        if (instance_p != NULL) {                               \
            nala_free((data_p)->params.name ## _out.buf_p);     \
        }                                                       \
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
        format_mock_traceback(
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
    add_mock_suspend();
    call_mock_suspend();
    close_mock_suspend();
    compose_twice_mock_suspend();
    double_pointer_mock_suspend();
    dup_mock_suspend();
    dup2_mock_suspend();
    edit_number_mock_suspend();
    endmntent_mock_suspend();
    enum_param_mock_suspend();
    fclose_mock_suspend();
    fflush_mock_suspend();
    fileno_mock_suspend();
    fopen_mock_suspend();
    fread_mock_suspend();
    free_mock_suspend();
    fseek_mock_suspend();
    ftell_mock_suspend();
    fwrite_mock_suspend();
    getmntent_mock_suspend();
    in_out_mock_suspend();
    io_control_mock_suspend();
    io_vcontrol_mock_suspend();
    malloc_mock_suspend();
    mount_mock_suspend();
    output_message_mock_suspend();
    pipe_mock_suspend();
    poll_mock_suspend();
    print_hello_mock_suspend();
    read_mock_suspend();
    sendto_mock_suspend();
    setsockopt_mock_suspend();
    sleep_mock_suspend();
    statvfs_mock_suspend();
    struct_param_mock_suspend();
    struct_param_and_return_type_mock_suspend();
    time_mock_suspend();
    timerfd_settime_mock_suspend();
    tmpfile_mock_suspend();
    typedef_struct_param_and_return_type_mock_suspend();
    typedef_union_param_and_return_type_mock_suspend();
    union_param_and_return_type_mock_suspend();
    usleep_mock_suspend();
    write_mock_suspend();
}

void nala_resume_all_mocks(void)
{
    add_mock_resume();
    call_mock_resume();
    close_mock_resume();
    compose_twice_mock_resume();
    double_pointer_mock_resume();
    dup_mock_resume();
    dup2_mock_resume();
    edit_number_mock_resume();
    endmntent_mock_resume();
    enum_param_mock_resume();
    fclose_mock_resume();
    fflush_mock_resume();
    fileno_mock_resume();
    fopen_mock_resume();
    fread_mock_resume();
    free_mock_resume();
    fseek_mock_resume();
    ftell_mock_resume();
    fwrite_mock_resume();
    getmntent_mock_resume();
    in_out_mock_resume();
    io_control_mock_resume();
    io_vcontrol_mock_resume();
    malloc_mock_resume();
    mount_mock_resume();
    output_message_mock_resume();
    pipe_mock_resume();
    poll_mock_resume();
    print_hello_mock_resume();
    read_mock_resume();
    sendto_mock_resume();
    setsockopt_mock_resume();
    sleep_mock_resume();
    statvfs_mock_resume();
    struct_param_mock_resume();
    struct_param_and_return_type_mock_resume();
    time_mock_resume();
    timerfd_settime_mock_resume();
    tmpfile_mock_resume();
    typedef_struct_param_and_return_type_mock_resume();
    typedef_union_param_and_return_type_mock_resume();
    union_param_and_return_type_mock_resume();
    usleep_mock_resume();
    write_mock_resume();
}

void nala_reset_all_mocks(void)
{
    add_mock_reset();
    call_mock_reset();
    close_mock_reset();
    compose_twice_mock_reset();
    double_pointer_mock_reset();
    dup_mock_reset();
    dup2_mock_reset();
    edit_number_mock_reset();
    endmntent_mock_reset();
    enum_param_mock_reset();
    fclose_mock_reset();
    fflush_mock_reset();
    fileno_mock_reset();
    fopen_mock_reset();
    fread_mock_reset();
    free_mock_reset();
    fseek_mock_reset();
    ftell_mock_reset();
    fwrite_mock_reset();
    getmntent_mock_reset();
    in_out_mock_reset();
    io_control_mock_reset();
    io_vcontrol_mock_reset();
    malloc_mock_reset();
    mount_mock_reset();
    output_message_mock_reset();
    pipe_mock_reset();
    poll_mock_reset();
    print_hello_mock_reset();
    read_mock_reset();
    sendto_mock_reset();
    setsockopt_mock_reset();
    sleep_mock_reset();
    statvfs_mock_reset();
    struct_param_mock_reset();
    struct_param_and_return_type_mock_reset();
    time_mock_reset();
    timerfd_settime_mock_reset();
    tmpfile_mock_reset();
    typedef_struct_param_and_return_type_mock_reset();
    typedef_union_param_and_return_type_mock_reset();
    union_param_and_return_type_mock_reset();
    usleep_mock_reset();
    write_mock_reset();
}

void nala_assert_all_mocks_completed(void)
{
    add_mock_assert_completed();
    call_mock_assert_completed();
    close_mock_assert_completed();
    compose_twice_mock_assert_completed();
    double_pointer_mock_assert_completed();
    dup_mock_assert_completed();
    dup2_mock_assert_completed();
    edit_number_mock_assert_completed();
    endmntent_mock_assert_completed();
    enum_param_mock_assert_completed();
    fclose_mock_assert_completed();
    fflush_mock_assert_completed();
    fileno_mock_assert_completed();
    fopen_mock_assert_completed();
    fread_mock_assert_completed();
    free_mock_assert_completed();
    fseek_mock_assert_completed();
    ftell_mock_assert_completed();
    fwrite_mock_assert_completed();
    getmntent_mock_assert_completed();
    in_out_mock_assert_completed();
    io_control_mock_assert_completed();
    io_vcontrol_mock_assert_completed();
    malloc_mock_assert_completed();
    mount_mock_assert_completed();
    output_message_mock_assert_completed();
    pipe_mock_assert_completed();
    poll_mock_assert_completed();
    print_hello_mock_assert_completed();
    read_mock_assert_completed();
    sendto_mock_assert_completed();
    setsockopt_mock_assert_completed();
    sleep_mock_assert_completed();
    statvfs_mock_assert_completed();
    struct_param_mock_assert_completed();
    struct_param_and_return_type_mock_assert_completed();
    time_mock_assert_completed();
    timerfd_settime_mock_assert_completed();
    tmpfile_mock_assert_completed();
    typedef_struct_param_and_return_type_mock_assert_completed();
    typedef_union_param_and_return_type_mock_assert_completed();
    union_param_and_return_type_mock_assert_completed();
    usleep_mock_assert_completed();
    write_mock_assert_completed();
}

// NALA_IMPLEMENTATION add

int __real_add(int x, int y);

struct nala_params_add_t {
    int x;
    int y;
    bool ignore_x_in;
    bool ignore_y_in;
};

struct nala_data_add_t {
    struct nala_params_add_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int x, int y);
    void (*callback)(int x, int y);
    struct nala_traceback_t traceback;
    struct nala_add_params_t params_in;
};

struct nala_instance_add_t {
    int mode;
    int handle;
    struct nala_data_add_t data;
    struct nala_instance_add_t *next_p;
};

struct nala_instances_add_t {
    struct nala_instance_add_t *head_p;
    struct nala_instance_add_t *next_p;
    struct nala_instance_add_t *tail_p;
    int length;
};

struct nala_mock_add_t {
    struct nala_state_t state;
    struct nala_data_add_t data;
    struct nala_instances_add_t instances;
};

static struct nala_mock_add_t nala_mock_add = {
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

struct nala_data_add_t *nala_get_data_add()
{
    if (nala_mock_add.instances.tail_p != NULL) {
        return (&nala_mock_add.instances.tail_p->data);
    } else {
        return (&nala_mock_add.data);
    }
}

struct nala_params_add_t *nala_get_params_add()
{
    return (&nala_get_data_add()->params);
}

int __wrap_add(int x, int y)
{
    struct nala_instance_add_t *nala_instance_p;
    struct nala_data_add_t *nala_data_p;
    int return_value;

    nala_print_call("add", &nala_mock_add.state);

    switch (nala_mock_add.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_add.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_add.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked add() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_add.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, add, x);
            nala_data_p->params_in.x = x;
            MOCK_ASSERT_IN_EQ(nala_data_p, add, y);
            nala_data_p->params_in.y = y;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(x, y);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_add(x, y);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_add.data.implementation(x, y);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_add.data.traceback,
                            "add");
        exit(1);
        break;

    default:
        return_value =
        __real_add(x, y);
        break;
    }

    return return_value;
}

void add_mock(int x, int y, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_add);
    nala_mock_add.state.mode = MODE_MOCK;
    nala_mock_add.data.params.x = x;
    nala_mock_add.data.params.ignore_x_in = false;
    nala_mock_add.data.params.y = y;
    nala_mock_add.data.params.ignore_y_in = false;
    nala_mock_add.data.return_value = return_value;
    nala_mock_add.data.errno_value = 0;
    nala_mock_add.data.callback = NULL;
    nala_traceback(&nala_mock_add.data.traceback);
}

int add_mock_once(int x, int y, int return_value)
{
    struct nala_instance_add_t *nala_instance_p;

    nala_mock_add.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.x = x;
    nala_instance_p->data.params.ignore_x_in = false;
    nala_instance_p->data.params.y = y;
    nala_instance_p->data.params.ignore_y_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_add.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void add_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_add);
    nala_mock_add.state.mode = MODE_MOCK;
    nala_mock_add.data.params.ignore_x_in = true;
    nala_mock_add.data.params.ignore_y_in = true;
    nala_mock_add.data.return_value = return_value;
    nala_mock_add.data.errno_value = 0;
    nala_mock_add.data.callback = NULL;
}

int add_mock_ignore_in_once(int return_value)
{
    struct nala_instance_add_t *instance_p;

    nala_mock_add.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_x_in = true;
    instance_p->data.params.ignore_y_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_add.instances,
                          instance_p);

    return (instance_p->handle);
}

void add_mock_set_errno(int errno_value)
{
    nala_get_data_add()->errno_value = errno_value;
}

void add_mock_set_callback(void (*callback)(int x, int y))
{
    nala_get_data_add()->callback = callback;
}

struct nala_add_params_t *add_mock_get_params_in(int handle)
{
    struct nala_instance_add_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_add.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "add() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void add_mock_ignore_x_in(void)
{
    nala_get_params_add()->ignore_x_in = true;
}

void add_mock_ignore_y_in(void)
{
    nala_get_params_add()->ignore_y_in = true;
}

void add_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_add);
    nala_mock_add.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_add.data.traceback);
}

void add_mock_implementation(int (*implementation)(int x, int y))
{
    CHECK_NO_INSTANCES(nala_mock_add);
    nala_mock_add.state.mode = MODE_IMPLEMENTATION;
    nala_mock_add.data.implementation = implementation;
    nala_traceback(&nala_mock_add.data.traceback);
}

void add_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_add);
    nala_mock_add.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_add.data.traceback);
}

void add_mock_real_once(void)
{
    struct nala_instance_add_t *instance_p;

    nala_mock_add.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_add.instances,
                          instance_p);
}

void add_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_add.state);
}

void add_mock_resume(void)
{
    nala_state_resume(&nala_mock_add.state);
}

void add_mock_reset(void)
{
    struct nala_instance_add_t *current_p;
    struct nala_instance_add_t *tmp_p;

    NALA_STATE_RESET(nala_mock_add, current_p, tmp_p);
}

void add_mock_assert_completed(void)
{
    if (nala_mock_add.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked add() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_add.instances.length),
                &nala_mock_add.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION call

int __real_call(int (*callback)(int value));

struct nala_params_call_t {
    int (*callback)(int value);
    bool ignore_callback_in;
    struct nala_set_param callback_in;
    void (*callback_in_assert)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size);
    struct nala_set_param callback_out;
    void (*callback_out_copy)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size);
};

struct nala_data_call_t {
    struct nala_params_call_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int (*callback)(int value));
    void (*callback)(int (*callback)(int value));
    struct nala_traceback_t traceback;
    struct nala_call_params_t params_in;
};

struct nala_instance_call_t {
    int mode;
    int handle;
    struct nala_data_call_t data;
    struct nala_instance_call_t *next_p;
};

struct nala_instances_call_t {
    struct nala_instance_call_t *head_p;
    struct nala_instance_call_t *next_p;
    struct nala_instance_call_t *tail_p;
    int length;
};

struct nala_mock_call_t {
    struct nala_state_t state;
    struct nala_data_call_t data;
    struct nala_instances_call_t instances;
};

static struct nala_mock_call_t nala_mock_call = {
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

struct nala_data_call_t *nala_get_data_call()
{
    if (nala_mock_call.instances.tail_p != NULL) {
        return (&nala_mock_call.instances.tail_p->data);
    } else {
        return (&nala_mock_call.data);
    }
}

struct nala_params_call_t *nala_get_params_call()
{
    return (&nala_get_data_call()->params);
}

int __wrap_call(int (*callback)(int value))
{
    struct nala_instance_call_t *nala_instance_p;
    struct nala_data_call_t *nala_data_p;
    int return_value;

    nala_print_call("call", &nala_mock_call.state);

    switch (nala_mock_call.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_call.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_call.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked call() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_call.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, call, callback);
            nala_data_p->params_in.callback = callback;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       call,
                                       callback);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(callback);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_call(callback);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_call.data.implementation(callback);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_call.data.traceback,
                            "call");
        exit(1);
        break;

    default:
        return_value =
        __real_call(callback);
        break;
    }

    return return_value;
}

void call_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_call);
    nala_mock_call.state.mode = MODE_MOCK;
    nala_mock_call.data.params.callback = NULL;
    nala_mock_call.data.params.ignore_callback_in = true;
    nala_mock_call.data.return_value = return_value;
    nala_mock_call.data.errno_value = 0;
    nala_mock_call.data.callback = NULL;
    nala_traceback(&nala_mock_call.data.traceback);
}

int call_mock_once(int return_value)
{
    struct nala_instance_call_t *nala_instance_p;

    nala_mock_call.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.callback_out);
    nala_set_param_init(&nala_instance_p->data.params.callback_in);
    nala_instance_p->data.params.callback_in_assert = NULL;
    nala_instance_p->data.params.callback_out_copy = NULL;
    nala_instance_p->data.params.callback = NULL;
    nala_instance_p->data.params.ignore_callback_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_call.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void call_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_call);
    nala_mock_call.state.mode = MODE_MOCK;
    nala_mock_call.data.params.ignore_callback_in = true;
    nala_mock_call.data.return_value = return_value;
    nala_mock_call.data.errno_value = 0;
    nala_mock_call.data.callback = NULL;
}

int call_mock_ignore_in_once(int return_value)
{
    struct nala_instance_call_t *instance_p;

    nala_mock_call.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.callback_out);
    nala_set_param_init(&instance_p->data.params.callback_in);
    instance_p->data.params.callback_in_assert = NULL;
    instance_p->data.params.callback_out_copy = NULL;
    instance_p->data.params.callback = NULL;
    instance_p->data.params.ignore_callback_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_call.instances,
                          instance_p);

    return (instance_p->handle);
}

void call_mock_set_errno(int errno_value)
{
    nala_get_data_call()->errno_value = errno_value;
}

void call_mock_set_callback(void (*callback)(int (*callback)(int value)))
{
    nala_get_data_call()->callback = callback;
}

struct nala_call_params_t *call_mock_get_params_in(int handle)
{
    struct nala_instance_call_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_call.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "call() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void call_mock_set_callback_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_call()->callback_in,
                       buf_p,
                       size);
}

void call_mock_set_callback_in_assert(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_call_t *nala_params_p;

    nala_params_p = nala_get_params_call();

    if (nala_params_p->callback_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "call_mock_set_callback_in() must be called "
                "before call_mock_set_callback_in_assert().\n"));
    }

    nala_params_p->callback_in_assert = callback;
}

void call_mock_set_callback_in_pointer(int (*callback)(int value))
{
    struct nala_params_call_t *nala_params_p;

    nala_params_p = nala_get_params_call();
    nala_params_p->ignore_callback_in = false;
    nala_params_p->callback = callback;
}

void call_mock_set_callback_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_call()->callback_out,
                       buf_p,
                       size);
}

void call_mock_set_callback_out_copy(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_call_t *nala_params_p;

    nala_params_p = nala_get_params_call();

    if (nala_params_p->callback_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "call_mock_set_callback_out() must be called "
                "before call_mock_set_callback_out_copy().\n"));
    }

    nala_params_p->callback_out_copy = callback;
}

void call_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_call);
    nala_mock_call.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_call.data.traceback);
}

void call_mock_implementation(int (*implementation)(int (*callback)(int value)))
{
    CHECK_NO_INSTANCES(nala_mock_call);
    nala_mock_call.state.mode = MODE_IMPLEMENTATION;
    nala_mock_call.data.implementation = implementation;
    nala_traceback(&nala_mock_call.data.traceback);
}

void call_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_call);
    nala_mock_call.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_call.data.traceback);
}

void call_mock_real_once(void)
{
    struct nala_instance_call_t *instance_p;

    nala_mock_call.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_call.instances,
                          instance_p);
}

void call_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_call.state);
}

void call_mock_resume(void)
{
    nala_state_resume(&nala_mock_call.state);
}

void call_mock_reset(void)
{
    struct nala_instance_call_t *current_p;
    struct nala_instance_call_t *tmp_p;

    NALA_STATE_RESET(nala_mock_call, current_p, tmp_p);
}

void call_mock_assert_completed(void)
{
    if (nala_mock_call.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked call() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_call.instances.length),
                &nala_mock_call.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION close

int __real_close(int fd);

struct nala_params_close_t {
    int fd;
    bool ignore_fd_in;
};

struct nala_data_close_t {
    struct nala_params_close_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int fd);
    void (*callback)(int fd);
    struct nala_traceback_t traceback;
    struct nala_close_params_t params_in;
};

struct nala_instance_close_t {
    int mode;
    int handle;
    struct nala_data_close_t data;
    struct nala_instance_close_t *next_p;
};

struct nala_instances_close_t {
    struct nala_instance_close_t *head_p;
    struct nala_instance_close_t *next_p;
    struct nala_instance_close_t *tail_p;
    int length;
};

struct nala_mock_close_t {
    struct nala_state_t state;
    struct nala_data_close_t data;
    struct nala_instances_close_t instances;
};

static struct nala_mock_close_t nala_mock_close = {
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

struct nala_data_close_t *nala_get_data_close()
{
    if (nala_mock_close.instances.tail_p != NULL) {
        return (&nala_mock_close.instances.tail_p->data);
    } else {
        return (&nala_mock_close.data);
    }
}

struct nala_params_close_t *nala_get_params_close()
{
    return (&nala_get_data_close()->params);
}

int __wrap_close(int fd)
{
    struct nala_instance_close_t *nala_instance_p;
    struct nala_data_close_t *nala_data_p;
    int return_value;

    nala_print_call("close", &nala_mock_close.state);

    switch (nala_mock_close.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_close.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_close.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked close() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_close.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, close, fd);
            nala_data_p->params_in.fd = fd;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(fd);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_close(fd);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_close.data.implementation(fd);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_close.data.traceback,
                            "close");
        exit(1);
        break;

    default:
        return_value =
        __real_close(fd);
        break;
    }

    return return_value;
}

void close_mock(int fd, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_close);
    nala_mock_close.state.mode = MODE_MOCK;
    nala_mock_close.data.params.fd = fd;
    nala_mock_close.data.params.ignore_fd_in = false;
    nala_mock_close.data.return_value = return_value;
    nala_mock_close.data.errno_value = 0;
    nala_mock_close.data.callback = NULL;
    nala_traceback(&nala_mock_close.data.traceback);
}

int close_mock_once(int fd, int return_value)
{
    struct nala_instance_close_t *nala_instance_p;

    nala_mock_close.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.fd = fd;
    nala_instance_p->data.params.ignore_fd_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_close.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void close_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_close);
    nala_mock_close.state.mode = MODE_MOCK;
    nala_mock_close.data.params.ignore_fd_in = true;
    nala_mock_close.data.return_value = return_value;
    nala_mock_close.data.errno_value = 0;
    nala_mock_close.data.callback = NULL;
}

int close_mock_ignore_in_once(int return_value)
{
    struct nala_instance_close_t *instance_p;

    nala_mock_close.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_fd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_close.instances,
                          instance_p);

    return (instance_p->handle);
}

void close_mock_set_errno(int errno_value)
{
    nala_get_data_close()->errno_value = errno_value;
}

void close_mock_set_callback(void (*callback)(int fd))
{
    nala_get_data_close()->callback = callback;
}

struct nala_close_params_t *close_mock_get_params_in(int handle)
{
    struct nala_instance_close_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_close.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "close() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void close_mock_ignore_fd_in(void)
{
    nala_get_params_close()->ignore_fd_in = true;
}

void close_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_close);
    nala_mock_close.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_close.data.traceback);
}

void close_mock_implementation(int (*implementation)(int fd))
{
    CHECK_NO_INSTANCES(nala_mock_close);
    nala_mock_close.state.mode = MODE_IMPLEMENTATION;
    nala_mock_close.data.implementation = implementation;
    nala_traceback(&nala_mock_close.data.traceback);
}

void close_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_close);
    nala_mock_close.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_close.data.traceback);
}

void close_mock_real_once(void)
{
    struct nala_instance_close_t *instance_p;

    nala_mock_close.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_close.instances,
                          instance_p);
}

void close_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_close.state);
}

void close_mock_resume(void)
{
    nala_state_resume(&nala_mock_close.state);
}

void close_mock_reset(void)
{
    struct nala_instance_close_t *current_p;
    struct nala_instance_close_t *tmp_p;

    NALA_STATE_RESET(nala_mock_close, current_p, tmp_p);
}

void close_mock_assert_completed(void)
{
    if (nala_mock_close.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked close() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_close.instances.length),
                &nala_mock_close.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION compose_twice

DummyStruct *__real_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));

struct nala_params_compose_twice_t {
    DummyStruct *dummy_struct;
    DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct);
    bool ignore_dummy_struct_in;
    struct nala_set_param dummy_struct_in;
    void (*dummy_struct_in_assert)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param dummy_struct_out;
    void (*dummy_struct_out_copy)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    bool ignore_dummy_struct_modifier_in;
    struct nala_set_param dummy_struct_modifier_in;
    void (*dummy_struct_modifier_in_assert)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size);
    struct nala_set_param dummy_struct_modifier_out;
    void (*dummy_struct_modifier_out_copy)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size);
};

struct nala_data_compose_twice_t {
    struct nala_params_compose_twice_t params;
    DummyStruct *return_value;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
    void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
    struct nala_traceback_t traceback;
    struct nala_compose_twice_params_t params_in;
};

struct nala_instance_compose_twice_t {
    int mode;
    int handle;
    struct nala_data_compose_twice_t data;
    struct nala_instance_compose_twice_t *next_p;
};

struct nala_instances_compose_twice_t {
    struct nala_instance_compose_twice_t *head_p;
    struct nala_instance_compose_twice_t *next_p;
    struct nala_instance_compose_twice_t *tail_p;
    int length;
};

struct nala_mock_compose_twice_t {
    struct nala_state_t state;
    struct nala_data_compose_twice_t data;
    struct nala_instances_compose_twice_t instances;
};

static struct nala_mock_compose_twice_t nala_mock_compose_twice = {
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

struct nala_data_compose_twice_t *nala_get_data_compose_twice()
{
    if (nala_mock_compose_twice.instances.tail_p != NULL) {
        return (&nala_mock_compose_twice.instances.tail_p->data);
    } else {
        return (&nala_mock_compose_twice.data);
    }
}

struct nala_params_compose_twice_t *nala_get_params_compose_twice()
{
    return (&nala_get_data_compose_twice()->params);
}

DummyStruct *__wrap_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct nala_instance_compose_twice_t *nala_instance_p;
    struct nala_data_compose_twice_t *nala_data_p;
    DummyStruct *return_value;

    nala_print_call("compose_twice", &nala_mock_compose_twice.state);

    switch (nala_mock_compose_twice.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_compose_twice.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_compose_twice.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked compose_twice() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_compose_twice.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, compose_twice, dummy_struct);
            nala_data_p->params_in.dummy_struct = dummy_struct;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, compose_twice, dummy_struct_modifier);
            nala_data_p->params_in.dummy_struct_modifier = dummy_struct_modifier;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       compose_twice,
                                       dummy_struct);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       compose_twice,
                                       dummy_struct_modifier);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(dummy_struct, dummy_struct_modifier);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_compose_twice(dummy_struct, dummy_struct_modifier);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_compose_twice.data.implementation(dummy_struct, dummy_struct_modifier);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_compose_twice.data.traceback,
                            "compose_twice");
        exit(1);
        break;

    default:
        return_value =
        __real_compose_twice(dummy_struct, dummy_struct_modifier);
        break;
    }

    return return_value;
}

void compose_twice_mock(DummyStruct *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_compose_twice);
    nala_mock_compose_twice.state.mode = MODE_MOCK;
    nala_mock_compose_twice.data.params.dummy_struct = NULL;
    nala_mock_compose_twice.data.params.ignore_dummy_struct_in = true;
    nala_mock_compose_twice.data.params.dummy_struct_modifier = NULL;
    nala_mock_compose_twice.data.params.ignore_dummy_struct_modifier_in = true;
    nala_mock_compose_twice.data.return_value = return_value;
    nala_mock_compose_twice.data.errno_value = 0;
    nala_mock_compose_twice.data.callback = NULL;
    nala_traceback(&nala_mock_compose_twice.data.traceback);
}

int compose_twice_mock_once(DummyStruct *return_value)
{
    struct nala_instance_compose_twice_t *nala_instance_p;

    nala_mock_compose_twice.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_in);
    nala_instance_p->data.params.dummy_struct_in_assert = NULL;
    nala_instance_p->data.params.dummy_struct_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_modifier_out);
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_modifier_in);
    nala_instance_p->data.params.dummy_struct_modifier_in_assert = NULL;
    nala_instance_p->data.params.dummy_struct_modifier_out_copy = NULL;
    nala_instance_p->data.params.dummy_struct = NULL;
    nala_instance_p->data.params.ignore_dummy_struct_in = true;
    nala_instance_p->data.params.dummy_struct_modifier = NULL;
    nala_instance_p->data.params.ignore_dummy_struct_modifier_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_compose_twice.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void compose_twice_mock_ignore_in(DummyStruct *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_compose_twice);
    nala_mock_compose_twice.state.mode = MODE_MOCK;
    nala_mock_compose_twice.data.params.ignore_dummy_struct_in = true;
    nala_mock_compose_twice.data.params.ignore_dummy_struct_modifier_in = true;
    nala_mock_compose_twice.data.return_value = return_value;
    nala_mock_compose_twice.data.errno_value = 0;
    nala_mock_compose_twice.data.callback = NULL;
}

int compose_twice_mock_ignore_in_once(DummyStruct *return_value)
{
    struct nala_instance_compose_twice_t *instance_p;

    nala_mock_compose_twice.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&instance_p->data.params.dummy_struct_in);
    instance_p->data.params.dummy_struct_in_assert = NULL;
    instance_p->data.params.dummy_struct_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.dummy_struct_modifier_out);
    nala_set_param_init(&instance_p->data.params.dummy_struct_modifier_in);
    instance_p->data.params.dummy_struct_modifier_in_assert = NULL;
    instance_p->data.params.dummy_struct_modifier_out_copy = NULL;
    instance_p->data.params.dummy_struct = NULL;
    instance_p->data.params.ignore_dummy_struct_in = true;
    instance_p->data.params.dummy_struct_modifier = NULL;
    instance_p->data.params.ignore_dummy_struct_modifier_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_compose_twice.instances,
                          instance_p);

    return (instance_p->handle);
}

void compose_twice_mock_set_errno(int errno_value)
{
    nala_get_data_compose_twice()->errno_value = errno_value;
}

void compose_twice_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)))
{
    nala_get_data_compose_twice()->callback = callback;
}

struct nala_compose_twice_params_t *compose_twice_mock_get_params_in(int handle)
{
    struct nala_instance_compose_twice_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_compose_twice.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "compose_twice() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void compose_twice_mock_set_dummy_struct_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_in,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();

    if (nala_params_p->dummy_struct_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "compose_twice_mock_set_dummy_struct_in() must be called "
                "before compose_twice_mock_set_dummy_struct_in_assert().\n"));
    }

    nala_params_p->dummy_struct_in_assert = callback;
}

void compose_twice_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();
    nala_params_p->ignore_dummy_struct_in = false;
    nala_params_p->dummy_struct = dummy_struct;
}

void compose_twice_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_out,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();

    if (nala_params_p->dummy_struct_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "compose_twice_mock_set_dummy_struct_out() must be called "
                "before compose_twice_mock_set_dummy_struct_out_copy().\n"));
    }

    nala_params_p->dummy_struct_out_copy = callback;
}

void compose_twice_mock_set_dummy_struct_modifier_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_modifier_in,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_modifier_in_assert(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();

    if (nala_params_p->dummy_struct_modifier_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "compose_twice_mock_set_dummy_struct_modifier_in() must be called "
                "before compose_twice_mock_set_dummy_struct_modifier_in_assert().\n"));
    }

    nala_params_p->dummy_struct_modifier_in_assert = callback;
}

void compose_twice_mock_set_dummy_struct_modifier_in_pointer(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();
    nala_params_p->ignore_dummy_struct_modifier_in = false;
    nala_params_p->dummy_struct_modifier = dummy_struct_modifier;
}

void compose_twice_mock_set_dummy_struct_modifier_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_modifier_out,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_modifier_out_copy(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_compose_twice_t *nala_params_p;

    nala_params_p = nala_get_params_compose_twice();

    if (nala_params_p->dummy_struct_modifier_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "compose_twice_mock_set_dummy_struct_modifier_out() must be called "
                "before compose_twice_mock_set_dummy_struct_modifier_out_copy().\n"));
    }

    nala_params_p->dummy_struct_modifier_out_copy = callback;
}

void compose_twice_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_compose_twice);
    nala_mock_compose_twice.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_compose_twice.data.traceback);
}

void compose_twice_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)))
{
    CHECK_NO_INSTANCES(nala_mock_compose_twice);
    nala_mock_compose_twice.state.mode = MODE_IMPLEMENTATION;
    nala_mock_compose_twice.data.implementation = implementation;
    nala_traceback(&nala_mock_compose_twice.data.traceback);
}

void compose_twice_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_compose_twice);
    nala_mock_compose_twice.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_compose_twice.data.traceback);
}

void compose_twice_mock_real_once(void)
{
    struct nala_instance_compose_twice_t *instance_p;

    nala_mock_compose_twice.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_compose_twice.instances,
                          instance_p);
}

void compose_twice_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_compose_twice.state);
}

void compose_twice_mock_resume(void)
{
    nala_state_resume(&nala_mock_compose_twice.state);
}

void compose_twice_mock_reset(void)
{
    struct nala_instance_compose_twice_t *current_p;
    struct nala_instance_compose_twice_t *tmp_p;

    NALA_STATE_RESET(nala_mock_compose_twice, current_p, tmp_p);
}

void compose_twice_mock_assert_completed(void)
{
    if (nala_mock_compose_twice.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked compose_twice() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_compose_twice.instances.length),
                &nala_mock_compose_twice.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION double_pointer

int __real_double_pointer(int **value_pp);

struct nala_params_double_pointer_t {
    int **value_pp;
    bool ignore_value_pp_in;
    struct nala_set_param value_pp_in;
    void (*value_pp_in_assert)(int **value_pp, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param value_pp_out;
    void (*value_pp_out_copy)(int **value_pp, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_double_pointer_t {
    struct nala_params_double_pointer_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int **value_pp);
    void (*callback)(int **value_pp);
    struct nala_traceback_t traceback;
    struct nala_double_pointer_params_t params_in;
};

struct nala_instance_double_pointer_t {
    int mode;
    int handle;
    struct nala_data_double_pointer_t data;
    struct nala_instance_double_pointer_t *next_p;
};

struct nala_instances_double_pointer_t {
    struct nala_instance_double_pointer_t *head_p;
    struct nala_instance_double_pointer_t *next_p;
    struct nala_instance_double_pointer_t *tail_p;
    int length;
};

struct nala_mock_double_pointer_t {
    struct nala_state_t state;
    struct nala_data_double_pointer_t data;
    struct nala_instances_double_pointer_t instances;
};

static struct nala_mock_double_pointer_t nala_mock_double_pointer = {
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

struct nala_data_double_pointer_t *nala_get_data_double_pointer()
{
    if (nala_mock_double_pointer.instances.tail_p != NULL) {
        return (&nala_mock_double_pointer.instances.tail_p->data);
    } else {
        return (&nala_mock_double_pointer.data);
    }
}

struct nala_params_double_pointer_t *nala_get_params_double_pointer()
{
    return (&nala_get_data_double_pointer()->params);
}

int __wrap_double_pointer(int **value_pp)
{
    struct nala_instance_double_pointer_t *nala_instance_p;
    struct nala_data_double_pointer_t *nala_data_p;
    int return_value;

    nala_print_call("double_pointer", &nala_mock_double_pointer.state);

    switch (nala_mock_double_pointer.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_double_pointer.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_double_pointer.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked double_pointer() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_double_pointer.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, double_pointer, value_pp);
            nala_data_p->params_in.value_pp = value_pp;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       double_pointer,
                                       value_pp);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(value_pp);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_double_pointer(value_pp);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_double_pointer.data.implementation(value_pp);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_double_pointer.data.traceback,
                            "double_pointer");
        exit(1);
        break;

    default:
        return_value =
        __real_double_pointer(value_pp);
        break;
    }

    return return_value;
}

void double_pointer_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_double_pointer);
    nala_mock_double_pointer.state.mode = MODE_MOCK;
    nala_mock_double_pointer.data.params.value_pp = NULL;
    nala_mock_double_pointer.data.params.ignore_value_pp_in = true;
    nala_mock_double_pointer.data.return_value = return_value;
    nala_mock_double_pointer.data.errno_value = 0;
    nala_mock_double_pointer.data.callback = NULL;
    nala_traceback(&nala_mock_double_pointer.data.traceback);
}

int double_pointer_mock_once(int return_value)
{
    struct nala_instance_double_pointer_t *nala_instance_p;

    nala_mock_double_pointer.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.value_pp_out);
    nala_set_param_init(&nala_instance_p->data.params.value_pp_in);
    nala_instance_p->data.params.value_pp_in_assert = NULL;
    nala_instance_p->data.params.value_pp_out_copy = NULL;
    nala_instance_p->data.params.value_pp = NULL;
    nala_instance_p->data.params.ignore_value_pp_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_double_pointer.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void double_pointer_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_double_pointer);
    nala_mock_double_pointer.state.mode = MODE_MOCK;
    nala_mock_double_pointer.data.params.ignore_value_pp_in = true;
    nala_mock_double_pointer.data.return_value = return_value;
    nala_mock_double_pointer.data.errno_value = 0;
    nala_mock_double_pointer.data.callback = NULL;
}

int double_pointer_mock_ignore_in_once(int return_value)
{
    struct nala_instance_double_pointer_t *instance_p;

    nala_mock_double_pointer.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.value_pp_out);
    nala_set_param_init(&instance_p->data.params.value_pp_in);
    instance_p->data.params.value_pp_in_assert = NULL;
    instance_p->data.params.value_pp_out_copy = NULL;
    instance_p->data.params.value_pp = NULL;
    instance_p->data.params.ignore_value_pp_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_double_pointer.instances,
                          instance_p);

    return (instance_p->handle);
}

void double_pointer_mock_set_errno(int errno_value)
{
    nala_get_data_double_pointer()->errno_value = errno_value;
}

void double_pointer_mock_set_callback(void (*callback)(int **value_pp))
{
    nala_get_data_double_pointer()->callback = callback;
}

struct nala_double_pointer_params_t *double_pointer_mock_get_params_in(int handle)
{
    struct nala_instance_double_pointer_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_double_pointer.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "double_pointer() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void double_pointer_mock_set_value_pp_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_double_pointer()->value_pp_in,
                       buf_p,
                       size);
}

void double_pointer_mock_set_value_pp_in_assert(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_double_pointer_t *nala_params_p;

    nala_params_p = nala_get_params_double_pointer();

    if (nala_params_p->value_pp_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "double_pointer_mock_set_value_pp_in() must be called "
                "before double_pointer_mock_set_value_pp_in_assert().\n"));
    }

    nala_params_p->value_pp_in_assert = callback;
}

void double_pointer_mock_set_value_pp_in_pointer(int **value_pp)
{
    struct nala_params_double_pointer_t *nala_params_p;

    nala_params_p = nala_get_params_double_pointer();
    nala_params_p->ignore_value_pp_in = false;
    nala_params_p->value_pp = value_pp;
}

void double_pointer_mock_set_value_pp_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_double_pointer()->value_pp_out,
                       buf_p,
                       size);
}

void double_pointer_mock_set_value_pp_out_copy(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_double_pointer_t *nala_params_p;

    nala_params_p = nala_get_params_double_pointer();

    if (nala_params_p->value_pp_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "double_pointer_mock_set_value_pp_out() must be called "
                "before double_pointer_mock_set_value_pp_out_copy().\n"));
    }

    nala_params_p->value_pp_out_copy = callback;
}

void double_pointer_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_double_pointer);
    nala_mock_double_pointer.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_double_pointer.data.traceback);
}

void double_pointer_mock_implementation(int (*implementation)(int **value_pp))
{
    CHECK_NO_INSTANCES(nala_mock_double_pointer);
    nala_mock_double_pointer.state.mode = MODE_IMPLEMENTATION;
    nala_mock_double_pointer.data.implementation = implementation;
    nala_traceback(&nala_mock_double_pointer.data.traceback);
}

void double_pointer_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_double_pointer);
    nala_mock_double_pointer.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_double_pointer.data.traceback);
}

void double_pointer_mock_real_once(void)
{
    struct nala_instance_double_pointer_t *instance_p;

    nala_mock_double_pointer.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_double_pointer.instances,
                          instance_p);
}

void double_pointer_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_double_pointer.state);
}

void double_pointer_mock_resume(void)
{
    nala_state_resume(&nala_mock_double_pointer.state);
}

void double_pointer_mock_reset(void)
{
    struct nala_instance_double_pointer_t *current_p;
    struct nala_instance_double_pointer_t *tmp_p;

    NALA_STATE_RESET(nala_mock_double_pointer, current_p, tmp_p);
}

void double_pointer_mock_assert_completed(void)
{
    if (nala_mock_double_pointer.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked double_pointer() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_double_pointer.instances.length),
                &nala_mock_double_pointer.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION dup

int __real_dup(int oldfd);

struct nala_params_dup_t {
    int oldfd;
    bool ignore_oldfd_in;
};

struct nala_data_dup_t {
    struct nala_params_dup_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int oldfd);
    void (*callback)(int oldfd);
    struct nala_traceback_t traceback;
    struct nala_dup_params_t params_in;
};

struct nala_instance_dup_t {
    int mode;
    int handle;
    struct nala_data_dup_t data;
    struct nala_instance_dup_t *next_p;
};

struct nala_instances_dup_t {
    struct nala_instance_dup_t *head_p;
    struct nala_instance_dup_t *next_p;
    struct nala_instance_dup_t *tail_p;
    int length;
};

struct nala_mock_dup_t {
    struct nala_state_t state;
    struct nala_data_dup_t data;
    struct nala_instances_dup_t instances;
};

static struct nala_mock_dup_t nala_mock_dup = {
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

struct nala_data_dup_t *nala_get_data_dup()
{
    if (nala_mock_dup.instances.tail_p != NULL) {
        return (&nala_mock_dup.instances.tail_p->data);
    } else {
        return (&nala_mock_dup.data);
    }
}

struct nala_params_dup_t *nala_get_params_dup()
{
    return (&nala_get_data_dup()->params);
}

int __wrap_dup(int oldfd)
{
    struct nala_instance_dup_t *nala_instance_p;
    struct nala_data_dup_t *nala_data_p;
    int return_value;

    nala_print_call("dup", &nala_mock_dup.state);

    switch (nala_mock_dup.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_dup.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_dup.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked dup() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_dup.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, dup, oldfd);
            nala_data_p->params_in.oldfd = oldfd;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(oldfd);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_dup(oldfd);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_dup.data.implementation(oldfd);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_dup.data.traceback,
                            "dup");
        exit(1);
        break;

    default:
        return_value =
        __real_dup(oldfd);
        break;
    }

    return return_value;
}

void dup_mock(int oldfd, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_dup);
    nala_mock_dup.state.mode = MODE_MOCK;
    nala_mock_dup.data.params.oldfd = oldfd;
    nala_mock_dup.data.params.ignore_oldfd_in = false;
    nala_mock_dup.data.return_value = return_value;
    nala_mock_dup.data.errno_value = 0;
    nala_mock_dup.data.callback = NULL;
    nala_traceback(&nala_mock_dup.data.traceback);
}

int dup_mock_once(int oldfd, int return_value)
{
    struct nala_instance_dup_t *nala_instance_p;

    nala_mock_dup.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.oldfd = oldfd;
    nala_instance_p->data.params.ignore_oldfd_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_dup.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void dup_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_dup);
    nala_mock_dup.state.mode = MODE_MOCK;
    nala_mock_dup.data.params.ignore_oldfd_in = true;
    nala_mock_dup.data.return_value = return_value;
    nala_mock_dup.data.errno_value = 0;
    nala_mock_dup.data.callback = NULL;
}

int dup_mock_ignore_in_once(int return_value)
{
    struct nala_instance_dup_t *instance_p;

    nala_mock_dup.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_oldfd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_dup.instances,
                          instance_p);

    return (instance_p->handle);
}

void dup_mock_set_errno(int errno_value)
{
    nala_get_data_dup()->errno_value = errno_value;
}

void dup_mock_set_callback(void (*callback)(int oldfd))
{
    nala_get_data_dup()->callback = callback;
}

struct nala_dup_params_t *dup_mock_get_params_in(int handle)
{
    struct nala_instance_dup_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_dup.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "dup() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void dup_mock_ignore_oldfd_in(void)
{
    nala_get_params_dup()->ignore_oldfd_in = true;
}

void dup_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_dup);
    nala_mock_dup.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_dup.data.traceback);
}

void dup_mock_implementation(int (*implementation)(int oldfd))
{
    CHECK_NO_INSTANCES(nala_mock_dup);
    nala_mock_dup.state.mode = MODE_IMPLEMENTATION;
    nala_mock_dup.data.implementation = implementation;
    nala_traceback(&nala_mock_dup.data.traceback);
}

void dup_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_dup);
    nala_mock_dup.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_dup.data.traceback);
}

void dup_mock_real_once(void)
{
    struct nala_instance_dup_t *instance_p;

    nala_mock_dup.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_dup.instances,
                          instance_p);
}

void dup_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_dup.state);
}

void dup_mock_resume(void)
{
    nala_state_resume(&nala_mock_dup.state);
}

void dup_mock_reset(void)
{
    struct nala_instance_dup_t *current_p;
    struct nala_instance_dup_t *tmp_p;

    NALA_STATE_RESET(nala_mock_dup, current_p, tmp_p);
}

void dup_mock_assert_completed(void)
{
    if (nala_mock_dup.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked dup() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_dup.instances.length),
                &nala_mock_dup.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION dup2

int __real_dup2(int oldfd, int newfd);

struct nala_params_dup2_t {
    int oldfd;
    int newfd;
    bool ignore_oldfd_in;
    bool ignore_newfd_in;
};

struct nala_data_dup2_t {
    struct nala_params_dup2_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int oldfd, int newfd);
    void (*callback)(int oldfd, int newfd);
    struct nala_traceback_t traceback;
    struct nala_dup2_params_t params_in;
};

struct nala_instance_dup2_t {
    int mode;
    int handle;
    struct nala_data_dup2_t data;
    struct nala_instance_dup2_t *next_p;
};

struct nala_instances_dup2_t {
    struct nala_instance_dup2_t *head_p;
    struct nala_instance_dup2_t *next_p;
    struct nala_instance_dup2_t *tail_p;
    int length;
};

struct nala_mock_dup2_t {
    struct nala_state_t state;
    struct nala_data_dup2_t data;
    struct nala_instances_dup2_t instances;
};

static struct nala_mock_dup2_t nala_mock_dup2 = {
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

struct nala_data_dup2_t *nala_get_data_dup2()
{
    if (nala_mock_dup2.instances.tail_p != NULL) {
        return (&nala_mock_dup2.instances.tail_p->data);
    } else {
        return (&nala_mock_dup2.data);
    }
}

struct nala_params_dup2_t *nala_get_params_dup2()
{
    return (&nala_get_data_dup2()->params);
}

int __wrap_dup2(int oldfd, int newfd)
{
    struct nala_instance_dup2_t *nala_instance_p;
    struct nala_data_dup2_t *nala_data_p;
    int return_value;

    nala_print_call("dup2", &nala_mock_dup2.state);

    switch (nala_mock_dup2.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_dup2.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_dup2.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked dup2() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_dup2.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, dup2, oldfd);
            nala_data_p->params_in.oldfd = oldfd;
            MOCK_ASSERT_IN_EQ(nala_data_p, dup2, newfd);
            nala_data_p->params_in.newfd = newfd;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(oldfd, newfd);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_dup2(oldfd, newfd);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_dup2.data.implementation(oldfd, newfd);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_dup2.data.traceback,
                            "dup2");
        exit(1);
        break;

    default:
        return_value =
        __real_dup2(oldfd, newfd);
        break;
    }

    return return_value;
}

void dup2_mock(int oldfd, int newfd, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_dup2);
    nala_mock_dup2.state.mode = MODE_MOCK;
    nala_mock_dup2.data.params.oldfd = oldfd;
    nala_mock_dup2.data.params.ignore_oldfd_in = false;
    nala_mock_dup2.data.params.newfd = newfd;
    nala_mock_dup2.data.params.ignore_newfd_in = false;
    nala_mock_dup2.data.return_value = return_value;
    nala_mock_dup2.data.errno_value = 0;
    nala_mock_dup2.data.callback = NULL;
    nala_traceback(&nala_mock_dup2.data.traceback);
}

int dup2_mock_once(int oldfd, int newfd, int return_value)
{
    struct nala_instance_dup2_t *nala_instance_p;

    nala_mock_dup2.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.oldfd = oldfd;
    nala_instance_p->data.params.ignore_oldfd_in = false;
    nala_instance_p->data.params.newfd = newfd;
    nala_instance_p->data.params.ignore_newfd_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_dup2.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void dup2_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_dup2);
    nala_mock_dup2.state.mode = MODE_MOCK;
    nala_mock_dup2.data.params.ignore_oldfd_in = true;
    nala_mock_dup2.data.params.ignore_newfd_in = true;
    nala_mock_dup2.data.return_value = return_value;
    nala_mock_dup2.data.errno_value = 0;
    nala_mock_dup2.data.callback = NULL;
}

int dup2_mock_ignore_in_once(int return_value)
{
    struct nala_instance_dup2_t *instance_p;

    nala_mock_dup2.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_oldfd_in = true;
    instance_p->data.params.ignore_newfd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_dup2.instances,
                          instance_p);

    return (instance_p->handle);
}

void dup2_mock_set_errno(int errno_value)
{
    nala_get_data_dup2()->errno_value = errno_value;
}

void dup2_mock_set_callback(void (*callback)(int oldfd, int newfd))
{
    nala_get_data_dup2()->callback = callback;
}

struct nala_dup2_params_t *dup2_mock_get_params_in(int handle)
{
    struct nala_instance_dup2_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_dup2.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "dup2() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void dup2_mock_ignore_oldfd_in(void)
{
    nala_get_params_dup2()->ignore_oldfd_in = true;
}

void dup2_mock_ignore_newfd_in(void)
{
    nala_get_params_dup2()->ignore_newfd_in = true;
}

void dup2_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_dup2);
    nala_mock_dup2.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_dup2.data.traceback);
}

void dup2_mock_implementation(int (*implementation)(int oldfd, int newfd))
{
    CHECK_NO_INSTANCES(nala_mock_dup2);
    nala_mock_dup2.state.mode = MODE_IMPLEMENTATION;
    nala_mock_dup2.data.implementation = implementation;
    nala_traceback(&nala_mock_dup2.data.traceback);
}

void dup2_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_dup2);
    nala_mock_dup2.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_dup2.data.traceback);
}

void dup2_mock_real_once(void)
{
    struct nala_instance_dup2_t *instance_p;

    nala_mock_dup2.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_dup2.instances,
                          instance_p);
}

void dup2_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_dup2.state);
}

void dup2_mock_resume(void)
{
    nala_state_resume(&nala_mock_dup2.state);
}

void dup2_mock_reset(void)
{
    struct nala_instance_dup2_t *current_p;
    struct nala_instance_dup2_t *tmp_p;

    NALA_STATE_RESET(nala_mock_dup2, current_p, tmp_p);
}

void dup2_mock_assert_completed(void)
{
    if (nala_mock_dup2.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked dup2() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_dup2.instances.length),
                &nala_mock_dup2.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION edit_number

DummyStruct *__real_edit_number(DummyStruct *dummy_struct, int number);

struct nala_params_edit_number_t {
    DummyStruct *dummy_struct;
    int number;
    bool ignore_dummy_struct_in;
    struct nala_set_param dummy_struct_in;
    void (*dummy_struct_in_assert)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param dummy_struct_out;
    void (*dummy_struct_out_copy)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    bool ignore_number_in;
};

struct nala_data_edit_number_t {
    struct nala_params_edit_number_t params;
    DummyStruct *return_value;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number);
    void (*callback)(DummyStruct *dummy_struct, int number);
    struct nala_traceback_t traceback;
    struct nala_edit_number_params_t params_in;
};

struct nala_instance_edit_number_t {
    int mode;
    int handle;
    struct nala_data_edit_number_t data;
    struct nala_instance_edit_number_t *next_p;
};

struct nala_instances_edit_number_t {
    struct nala_instance_edit_number_t *head_p;
    struct nala_instance_edit_number_t *next_p;
    struct nala_instance_edit_number_t *tail_p;
    int length;
};

struct nala_mock_edit_number_t {
    struct nala_state_t state;
    struct nala_data_edit_number_t data;
    struct nala_instances_edit_number_t instances;
};

static struct nala_mock_edit_number_t nala_mock_edit_number = {
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

struct nala_data_edit_number_t *nala_get_data_edit_number()
{
    if (nala_mock_edit_number.instances.tail_p != NULL) {
        return (&nala_mock_edit_number.instances.tail_p->data);
    } else {
        return (&nala_mock_edit_number.data);
    }
}

struct nala_params_edit_number_t *nala_get_params_edit_number()
{
    return (&nala_get_data_edit_number()->params);
}

DummyStruct *__wrap_edit_number(DummyStruct *dummy_struct, int number)
{
    struct nala_instance_edit_number_t *nala_instance_p;
    struct nala_data_edit_number_t *nala_data_p;
    DummyStruct *return_value;

    nala_print_call("edit_number", &nala_mock_edit_number.state);

    switch (nala_mock_edit_number.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_edit_number.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_edit_number.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked edit_number() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_edit_number.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, edit_number, dummy_struct);
            nala_data_p->params_in.dummy_struct = dummy_struct;
            MOCK_ASSERT_IN_EQ(nala_data_p, edit_number, number);
            nala_data_p->params_in.number = number;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       edit_number,
                                       dummy_struct);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(dummy_struct, number);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_edit_number(dummy_struct, number);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_edit_number.data.implementation(dummy_struct, number);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_edit_number.data.traceback,
                            "edit_number");
        exit(1);
        break;

    default:
        return_value =
        __real_edit_number(dummy_struct, number);
        break;
    }

    return return_value;
}

void edit_number_mock(int number, DummyStruct *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_edit_number);
    nala_mock_edit_number.state.mode = MODE_MOCK;
    nala_mock_edit_number.data.params.dummy_struct = NULL;
    nala_mock_edit_number.data.params.ignore_dummy_struct_in = true;
    nala_mock_edit_number.data.params.number = number;
    nala_mock_edit_number.data.params.ignore_number_in = false;
    nala_mock_edit_number.data.return_value = return_value;
    nala_mock_edit_number.data.errno_value = 0;
    nala_mock_edit_number.data.callback = NULL;
    nala_traceback(&nala_mock_edit_number.data.traceback);
}

int edit_number_mock_once(int number, DummyStruct *return_value)
{
    struct nala_instance_edit_number_t *nala_instance_p;

    nala_mock_edit_number.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&nala_instance_p->data.params.dummy_struct_in);
    nala_instance_p->data.params.dummy_struct_in_assert = NULL;
    nala_instance_p->data.params.dummy_struct_out_copy = NULL;
    nala_instance_p->data.params.dummy_struct = NULL;
    nala_instance_p->data.params.ignore_dummy_struct_in = true;
    nala_instance_p->data.params.number = number;
    nala_instance_p->data.params.ignore_number_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_edit_number.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void edit_number_mock_ignore_in(DummyStruct *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_edit_number);
    nala_mock_edit_number.state.mode = MODE_MOCK;
    nala_mock_edit_number.data.params.ignore_dummy_struct_in = true;
    nala_mock_edit_number.data.params.ignore_number_in = true;
    nala_mock_edit_number.data.return_value = return_value;
    nala_mock_edit_number.data.errno_value = 0;
    nala_mock_edit_number.data.callback = NULL;
}

int edit_number_mock_ignore_in_once(DummyStruct *return_value)
{
    struct nala_instance_edit_number_t *instance_p;

    nala_mock_edit_number.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&instance_p->data.params.dummy_struct_in);
    instance_p->data.params.dummy_struct_in_assert = NULL;
    instance_p->data.params.dummy_struct_out_copy = NULL;
    instance_p->data.params.dummy_struct = NULL;
    instance_p->data.params.ignore_dummy_struct_in = true;
    instance_p->data.params.ignore_number_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_edit_number.instances,
                          instance_p);

    return (instance_p->handle);
}

void edit_number_mock_set_errno(int errno_value)
{
    nala_get_data_edit_number()->errno_value = errno_value;
}

void edit_number_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, int number))
{
    nala_get_data_edit_number()->callback = callback;
}

struct nala_edit_number_params_t *edit_number_mock_get_params_in(int handle)
{
    struct nala_instance_edit_number_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_edit_number.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "edit_number() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void edit_number_mock_ignore_number_in(void)
{
    nala_get_params_edit_number()->ignore_number_in = true;
}

void edit_number_mock_set_dummy_struct_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_edit_number()->dummy_struct_in,
                       buf_p,
                       size);
}

void edit_number_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_edit_number_t *nala_params_p;

    nala_params_p = nala_get_params_edit_number();

    if (nala_params_p->dummy_struct_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "edit_number_mock_set_dummy_struct_in() must be called "
                "before edit_number_mock_set_dummy_struct_in_assert().\n"));
    }

    nala_params_p->dummy_struct_in_assert = callback;
}

void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct nala_params_edit_number_t *nala_params_p;

    nala_params_p = nala_get_params_edit_number();
    nala_params_p->ignore_dummy_struct_in = false;
    nala_params_p->dummy_struct = dummy_struct;
}

void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_edit_number()->dummy_struct_out,
                       buf_p,
                       size);
}

void edit_number_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_edit_number_t *nala_params_p;

    nala_params_p = nala_get_params_edit_number();

    if (nala_params_p->dummy_struct_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "edit_number_mock_set_dummy_struct_out() must be called "
                "before edit_number_mock_set_dummy_struct_out_copy().\n"));
    }

    nala_params_p->dummy_struct_out_copy = callback;
}

void edit_number_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_edit_number);
    nala_mock_edit_number.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_edit_number.data.traceback);
}

void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number))
{
    CHECK_NO_INSTANCES(nala_mock_edit_number);
    nala_mock_edit_number.state.mode = MODE_IMPLEMENTATION;
    nala_mock_edit_number.data.implementation = implementation;
    nala_traceback(&nala_mock_edit_number.data.traceback);
}

void edit_number_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_edit_number);
    nala_mock_edit_number.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_edit_number.data.traceback);
}

void edit_number_mock_real_once(void)
{
    struct nala_instance_edit_number_t *instance_p;

    nala_mock_edit_number.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_edit_number.instances,
                          instance_p);
}

void edit_number_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_edit_number.state);
}

void edit_number_mock_resume(void)
{
    nala_state_resume(&nala_mock_edit_number.state);
}

void edit_number_mock_reset(void)
{
    struct nala_instance_edit_number_t *current_p;
    struct nala_instance_edit_number_t *tmp_p;

    NALA_STATE_RESET(nala_mock_edit_number, current_p, tmp_p);
}

void edit_number_mock_assert_completed(void)
{
    if (nala_mock_edit_number.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked edit_number() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_edit_number.instances.length),
                &nala_mock_edit_number.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION endmntent

int __real_endmntent(FILE *streamp);

struct nala_params_endmntent_t {
    FILE *streamp;
    bool ignore_streamp_in;
    struct nala_set_param streamp_in;
    void (*streamp_in_assert)(FILE *streamp, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param streamp_out;
    void (*streamp_out_copy)(FILE *streamp, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_endmntent_t {
    struct nala_params_endmntent_t params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *streamp);
    void (*callback)(FILE *streamp);
    struct nala_traceback_t traceback;
    struct nala_endmntent_params_t params_in;
};

struct nala_instance_endmntent_t {
    int mode;
    int handle;
    struct nala_data_endmntent_t data;
    struct nala_instance_endmntent_t *next_p;
};

struct nala_instances_endmntent_t {
    struct nala_instance_endmntent_t *head_p;
    struct nala_instance_endmntent_t *next_p;
    struct nala_instance_endmntent_t *tail_p;
    int length;
};

struct nala_mock_endmntent_t {
    struct nala_state_t state;
    struct nala_data_endmntent_t data;
    struct nala_instances_endmntent_t instances;
};

static struct nala_mock_endmntent_t nala_mock_endmntent = {
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

struct nala_data_endmntent_t *nala_get_data_endmntent()
{
    if (nala_mock_endmntent.instances.tail_p != NULL) {
        return (&nala_mock_endmntent.instances.tail_p->data);
    } else {
        return (&nala_mock_endmntent.data);
    }
}

struct nala_params_endmntent_t *nala_get_params_endmntent()
{
    return (&nala_get_data_endmntent()->params);
}

int __wrap_endmntent(FILE *streamp)
{
    struct nala_instance_endmntent_t *nala_instance_p;
    struct nala_data_endmntent_t *nala_data_p;
    int return_value;

    nala_print_call("endmntent", &nala_mock_endmntent.state);

    switch (nala_mock_endmntent.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_endmntent.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_endmntent.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked endmntent() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_endmntent.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, endmntent, streamp);
            nala_data_p->params_in.streamp = streamp;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       endmntent,
                                       streamp);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(streamp);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_endmntent(streamp);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_endmntent.data.implementation(streamp);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_endmntent.data.traceback,
                            "endmntent");
        exit(1);
        break;

    default:
        return_value =
        __real_endmntent(streamp);
        break;
    }

    return return_value;
}

void endmntent_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_endmntent);
    nala_mock_endmntent.state.mode = MODE_MOCK;
    nala_mock_endmntent.data.params.streamp = NULL;
    nala_mock_endmntent.data.params.ignore_streamp_in = true;
    nala_mock_endmntent.data.return_value = return_value;
    nala_mock_endmntent.data.errno_value = 0;
    nala_mock_endmntent.data.callback = NULL;
    nala_traceback(&nala_mock_endmntent.data.traceback);
}

int endmntent_mock_once(int return_value)
{
    struct nala_instance_endmntent_t *nala_instance_p;

    nala_mock_endmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.streamp_out);
    nala_set_param_init(&nala_instance_p->data.params.streamp_in);
    nala_instance_p->data.params.streamp_in_assert = NULL;
    nala_instance_p->data.params.streamp_out_copy = NULL;
    nala_instance_p->data.params.streamp = NULL;
    nala_instance_p->data.params.ignore_streamp_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_endmntent.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void endmntent_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_endmntent);
    nala_mock_endmntent.state.mode = MODE_MOCK;
    nala_mock_endmntent.data.params.ignore_streamp_in = true;
    nala_mock_endmntent.data.return_value = return_value;
    nala_mock_endmntent.data.errno_value = 0;
    nala_mock_endmntent.data.callback = NULL;
}

int endmntent_mock_ignore_in_once(int return_value)
{
    struct nala_instance_endmntent_t *instance_p;

    nala_mock_endmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.streamp_out);
    nala_set_param_init(&instance_p->data.params.streamp_in);
    instance_p->data.params.streamp_in_assert = NULL;
    instance_p->data.params.streamp_out_copy = NULL;
    instance_p->data.params.streamp = NULL;
    instance_p->data.params.ignore_streamp_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_endmntent.instances,
                          instance_p);

    return (instance_p->handle);
}

void endmntent_mock_set_errno(int errno_value)
{
    nala_get_data_endmntent()->errno_value = errno_value;
}

void endmntent_mock_set_callback(void (*callback)(FILE *streamp))
{
    nala_get_data_endmntent()->callback = callback;
}

struct nala_endmntent_params_t *endmntent_mock_get_params_in(int handle)
{
    struct nala_instance_endmntent_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_endmntent.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "endmntent() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void endmntent_mock_set_streamp_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_endmntent()->streamp_in,
                       buf_p,
                       size);
}

void endmntent_mock_set_streamp_in_assert(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_endmntent_t *nala_params_p;

    nala_params_p = nala_get_params_endmntent();

    if (nala_params_p->streamp_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "endmntent_mock_set_streamp_in() must be called "
                "before endmntent_mock_set_streamp_in_assert().\n"));
    }

    nala_params_p->streamp_in_assert = callback;
}

void endmntent_mock_set_streamp_in_pointer(FILE *streamp)
{
    struct nala_params_endmntent_t *nala_params_p;

    nala_params_p = nala_get_params_endmntent();
    nala_params_p->ignore_streamp_in = false;
    nala_params_p->streamp = streamp;
}

void endmntent_mock_set_streamp_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_endmntent()->streamp_out,
                       buf_p,
                       size);
}

void endmntent_mock_set_streamp_out_copy(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_endmntent_t *nala_params_p;

    nala_params_p = nala_get_params_endmntent();

    if (nala_params_p->streamp_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "endmntent_mock_set_streamp_out() must be called "
                "before endmntent_mock_set_streamp_out_copy().\n"));
    }

    nala_params_p->streamp_out_copy = callback;
}

void endmntent_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_endmntent);
    nala_mock_endmntent.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_endmntent.data.traceback);
}

void endmntent_mock_implementation(int (*implementation)(FILE *streamp))
{
    CHECK_NO_INSTANCES(nala_mock_endmntent);
    nala_mock_endmntent.state.mode = MODE_IMPLEMENTATION;
    nala_mock_endmntent.data.implementation = implementation;
    nala_traceback(&nala_mock_endmntent.data.traceback);
}

void endmntent_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_endmntent);
    nala_mock_endmntent.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_endmntent.data.traceback);
}

void endmntent_mock_real_once(void)
{
    struct nala_instance_endmntent_t *instance_p;

    nala_mock_endmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_endmntent.instances,
                          instance_p);
}

void endmntent_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_endmntent.state);
}

void endmntent_mock_resume(void)
{
    nala_state_resume(&nala_mock_endmntent.state);
}

void endmntent_mock_reset(void)
{
    struct nala_instance_endmntent_t *current_p;
    struct nala_instance_endmntent_t *tmp_p;

    NALA_STATE_RESET(nala_mock_endmntent, current_p, tmp_p);
}

void endmntent_mock_assert_completed(void)
{
    if (nala_mock_endmntent.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked endmntent() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_endmntent.instances.length),
                &nala_mock_endmntent.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION enum_param

void __real_enum_param(enum enum_param_type value);

struct nala_params_enum_param_t {
    enum enum_param_type value;
    bool ignore_value_in;
};

struct nala_data_enum_param_t {
    struct nala_params_enum_param_t params;
    int errno_value;
    void (*implementation)(enum enum_param_type value);
    void (*callback)(enum enum_param_type value);
    struct nala_traceback_t traceback;
    struct nala_enum_param_params_t params_in;
};

struct nala_instance_enum_param_t {
    int mode;
    int handle;
    struct nala_data_enum_param_t data;
    struct nala_instance_enum_param_t *next_p;
};

struct nala_instances_enum_param_t {
    struct nala_instance_enum_param_t *head_p;
    struct nala_instance_enum_param_t *next_p;
    struct nala_instance_enum_param_t *tail_p;
    int length;
};

struct nala_mock_enum_param_t {
    struct nala_state_t state;
    struct nala_data_enum_param_t data;
    struct nala_instances_enum_param_t instances;
};

static struct nala_mock_enum_param_t nala_mock_enum_param = {
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

struct nala_data_enum_param_t *nala_get_data_enum_param()
{
    if (nala_mock_enum_param.instances.tail_p != NULL) {
        return (&nala_mock_enum_param.instances.tail_p->data);
    } else {
        return (&nala_mock_enum_param.data);
    }
}

struct nala_params_enum_param_t *nala_get_params_enum_param()
{
    return (&nala_get_data_enum_param()->params);
}

void __wrap_enum_param(enum enum_param_type value)
{
    struct nala_instance_enum_param_t *nala_instance_p;
    struct nala_data_enum_param_t *nala_data_p;

    nala_print_call("enum_param", &nala_mock_enum_param.state);

    switch (nala_mock_enum_param.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_enum_param.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_enum_param.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked enum_param() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_enum_param.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, enum_param, value);
            nala_data_p->params_in.value = value;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(value);
            }

        } else {
            __real_enum_param(value);
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_enum_param.data.implementation(value);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_enum_param.data.traceback,
                            "enum_param");
        exit(1);
        break;

    default:
        __real_enum_param(value);
        break;
    }

    return;
}

void enum_param_mock(enum enum_param_type value)
{
    CHECK_NO_INSTANCES(nala_mock_enum_param);
    nala_mock_enum_param.state.mode = MODE_MOCK;
    nala_mock_enum_param.data.params.value = value;
    nala_mock_enum_param.data.params.ignore_value_in = false;
    nala_mock_enum_param.data.errno_value = 0;
    nala_mock_enum_param.data.callback = NULL;
    nala_traceback(&nala_mock_enum_param.data.traceback);
}

int enum_param_mock_once(enum enum_param_type value)
{
    struct nala_instance_enum_param_t *nala_instance_p;

    nala_mock_enum_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.value = value;
    nala_instance_p->data.params.ignore_value_in = false;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_enum_param.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void enum_param_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_enum_param);
    nala_mock_enum_param.state.mode = MODE_MOCK;
    nala_mock_enum_param.data.params.ignore_value_in = true;
    nala_mock_enum_param.data.errno_value = 0;
    nala_mock_enum_param.data.callback = NULL;
}

int enum_param_mock_ignore_in_once(void)
{
    struct nala_instance_enum_param_t *instance_p;

    nala_mock_enum_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_value_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_enum_param.instances,
                          instance_p);

    return (instance_p->handle);
}

void enum_param_mock_set_errno(int errno_value)
{
    nala_get_data_enum_param()->errno_value = errno_value;
}

void enum_param_mock_set_callback(void (*callback)(enum enum_param_type value))
{
    nala_get_data_enum_param()->callback = callback;
}

struct nala_enum_param_params_t *enum_param_mock_get_params_in(int handle)
{
    struct nala_instance_enum_param_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_enum_param.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "enum_param() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void enum_param_mock_ignore_value_in(void)
{
    nala_get_params_enum_param()->ignore_value_in = true;
}

void enum_param_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_enum_param);
    nala_mock_enum_param.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_enum_param.data.traceback);
}

void enum_param_mock_implementation(void (*implementation)(enum enum_param_type value))
{
    CHECK_NO_INSTANCES(nala_mock_enum_param);
    nala_mock_enum_param.state.mode = MODE_IMPLEMENTATION;
    nala_mock_enum_param.data.implementation = implementation;
    nala_traceback(&nala_mock_enum_param.data.traceback);
}

void enum_param_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_enum_param);
    nala_mock_enum_param.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_enum_param.data.traceback);
}

void enum_param_mock_real_once(void)
{
    struct nala_instance_enum_param_t *instance_p;

    nala_mock_enum_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_enum_param.instances,
                          instance_p);
}

void enum_param_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_enum_param.state);
}

void enum_param_mock_resume(void)
{
    nala_state_resume(&nala_mock_enum_param.state);
}

void enum_param_mock_reset(void)
{
    struct nala_instance_enum_param_t *current_p;
    struct nala_instance_enum_param_t *tmp_p;

    NALA_STATE_RESET(nala_mock_enum_param, current_p, tmp_p);
}

void enum_param_mock_assert_completed(void)
{
    if (nala_mock_enum_param.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked enum_param() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_enum_param.instances.length),
                &nala_mock_enum_param.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fclose

int __real_fclose(FILE *stream);

struct nala_params_fclose_t {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fclose_t {
    struct nala_params_fclose_t params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_fclose_params_t params_in;
};

struct nala_instance_fclose_t {
    int mode;
    int handle;
    struct nala_data_fclose_t data;
    struct nala_instance_fclose_t *next_p;
};

struct nala_instances_fclose_t {
    struct nala_instance_fclose_t *head_p;
    struct nala_instance_fclose_t *next_p;
    struct nala_instance_fclose_t *tail_p;
    int length;
};

struct nala_mock_fclose_t {
    struct nala_state_t state;
    struct nala_data_fclose_t data;
    struct nala_instances_fclose_t instances;
};

static struct nala_mock_fclose_t nala_mock_fclose = {
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

struct nala_data_fclose_t *nala_get_data_fclose()
{
    if (nala_mock_fclose.instances.tail_p != NULL) {
        return (&nala_mock_fclose.instances.tail_p->data);
    } else {
        return (&nala_mock_fclose.data);
    }
}

struct nala_params_fclose_t *nala_get_params_fclose()
{
    return (&nala_get_data_fclose()->params);
}

int __wrap_fclose(FILE *stream)
{
    struct nala_instance_fclose_t *nala_instance_p;
    struct nala_data_fclose_t *nala_data_p;
    int return_value;

    nala_print_call("fclose", &nala_mock_fclose.state);

    switch (nala_mock_fclose.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fclose.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fclose.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fclose() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fclose.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fclose, stream);
            nala_data_p->params_in.stream = stream;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fclose,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fclose(stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fclose.data.implementation(stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fclose.data.traceback,
                            "fclose");
        exit(1);
        break;

    default:
        return_value =
        __real_fclose(stream);
        break;
    }

    return return_value;
}

void fclose_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fclose);
    nala_mock_fclose.state.mode = MODE_MOCK;
    nala_mock_fclose.data.params.stream = NULL;
    nala_mock_fclose.data.params.ignore_stream_in = true;
    nala_mock_fclose.data.return_value = return_value;
    nala_mock_fclose.data.errno_value = 0;
    nala_mock_fclose.data.callback = NULL;
    nala_traceback(&nala_mock_fclose.data.traceback);
}

int fclose_mock_once(int return_value)
{
    struct nala_instance_fclose_t *nala_instance_p;

    nala_mock_fclose.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fclose.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fclose_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fclose);
    nala_mock_fclose.state.mode = MODE_MOCK;
    nala_mock_fclose.data.params.ignore_stream_in = true;
    nala_mock_fclose.data.return_value = return_value;
    nala_mock_fclose.data.errno_value = 0;
    nala_mock_fclose.data.callback = NULL;
}

int fclose_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fclose_t *instance_p;

    nala_mock_fclose.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fclose.instances,
                          instance_p);

    return (instance_p->handle);
}

void fclose_mock_set_errno(int errno_value)
{
    nala_get_data_fclose()->errno_value = errno_value;
}

void fclose_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fclose()->callback = callback;
}

struct nala_fclose_params_t *fclose_mock_get_params_in(int handle)
{
    struct nala_instance_fclose_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fclose.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fclose() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fclose_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fclose()->stream_in,
                       buf_p,
                       size);
}

void fclose_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fclose_t *nala_params_p;

    nala_params_p = nala_get_params_fclose();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fclose_mock_set_stream_in() must be called "
                "before fclose_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fclose_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fclose_t *nala_params_p;

    nala_params_p = nala_get_params_fclose();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fclose_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fclose()->stream_out,
                       buf_p,
                       size);
}

void fclose_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fclose_t *nala_params_p;

    nala_params_p = nala_get_params_fclose();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fclose_mock_set_stream_out() must be called "
                "before fclose_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fclose_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fclose);
    nala_mock_fclose.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fclose.data.traceback);
}

void fclose_mock_implementation(int (*implementation)(FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_fclose);
    nala_mock_fclose.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fclose.data.implementation = implementation;
    nala_traceback(&nala_mock_fclose.data.traceback);
}

void fclose_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fclose);
    nala_mock_fclose.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fclose.data.traceback);
}

void fclose_mock_real_once(void)
{
    struct nala_instance_fclose_t *instance_p;

    nala_mock_fclose.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fclose.instances,
                          instance_p);
}

void fclose_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fclose.state);
}

void fclose_mock_resume(void)
{
    nala_state_resume(&nala_mock_fclose.state);
}

void fclose_mock_reset(void)
{
    struct nala_instance_fclose_t *current_p;
    struct nala_instance_fclose_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fclose, current_p, tmp_p);
}

void fclose_mock_assert_completed(void)
{
    if (nala_mock_fclose.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fclose() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fclose.instances.length),
                &nala_mock_fclose.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fflush

int __real_fflush(FILE *stream);

struct nala_params_fflush_t {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fflush_t {
    struct nala_params_fflush_t params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_fflush_params_t params_in;
};

struct nala_instance_fflush_t {
    int mode;
    int handle;
    struct nala_data_fflush_t data;
    struct nala_instance_fflush_t *next_p;
};

struct nala_instances_fflush_t {
    struct nala_instance_fflush_t *head_p;
    struct nala_instance_fflush_t *next_p;
    struct nala_instance_fflush_t *tail_p;
    int length;
};

struct nala_mock_fflush_t {
    struct nala_state_t state;
    struct nala_data_fflush_t data;
    struct nala_instances_fflush_t instances;
};

static struct nala_mock_fflush_t nala_mock_fflush = {
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

struct nala_data_fflush_t *nala_get_data_fflush()
{
    if (nala_mock_fflush.instances.tail_p != NULL) {
        return (&nala_mock_fflush.instances.tail_p->data);
    } else {
        return (&nala_mock_fflush.data);
    }
}

struct nala_params_fflush_t *nala_get_params_fflush()
{
    return (&nala_get_data_fflush()->params);
}

int __wrap_fflush(FILE *stream)
{
    struct nala_instance_fflush_t *nala_instance_p;
    struct nala_data_fflush_t *nala_data_p;
    int return_value;

    nala_print_call("fflush", &nala_mock_fflush.state);

    switch (nala_mock_fflush.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fflush.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fflush.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fflush() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fflush.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fflush, stream);
            nala_data_p->params_in.stream = stream;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fflush,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fflush(stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fflush.data.implementation(stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fflush.data.traceback,
                            "fflush");
        exit(1);
        break;

    default:
        return_value =
        __real_fflush(stream);
        break;
    }

    return return_value;
}

void fflush_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fflush);
    nala_mock_fflush.state.mode = MODE_MOCK;
    nala_mock_fflush.data.params.stream = NULL;
    nala_mock_fflush.data.params.ignore_stream_in = true;
    nala_mock_fflush.data.return_value = return_value;
    nala_mock_fflush.data.errno_value = 0;
    nala_mock_fflush.data.callback = NULL;
    nala_traceback(&nala_mock_fflush.data.traceback);
}

int fflush_mock_once(int return_value)
{
    struct nala_instance_fflush_t *nala_instance_p;

    nala_mock_fflush.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fflush.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fflush_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fflush);
    nala_mock_fflush.state.mode = MODE_MOCK;
    nala_mock_fflush.data.params.ignore_stream_in = true;
    nala_mock_fflush.data.return_value = return_value;
    nala_mock_fflush.data.errno_value = 0;
    nala_mock_fflush.data.callback = NULL;
}

int fflush_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fflush_t *instance_p;

    nala_mock_fflush.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fflush.instances,
                          instance_p);

    return (instance_p->handle);
}

void fflush_mock_set_errno(int errno_value)
{
    nala_get_data_fflush()->errno_value = errno_value;
}

void fflush_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fflush()->callback = callback;
}

struct nala_fflush_params_t *fflush_mock_get_params_in(int handle)
{
    struct nala_instance_fflush_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fflush.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fflush() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fflush_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fflush()->stream_in,
                       buf_p,
                       size);
}

void fflush_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fflush_t *nala_params_p;

    nala_params_p = nala_get_params_fflush();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fflush_mock_set_stream_in() must be called "
                "before fflush_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fflush_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fflush_t *nala_params_p;

    nala_params_p = nala_get_params_fflush();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fflush_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fflush()->stream_out,
                       buf_p,
                       size);
}

void fflush_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fflush_t *nala_params_p;

    nala_params_p = nala_get_params_fflush();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fflush_mock_set_stream_out() must be called "
                "before fflush_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fflush_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fflush);
    nala_mock_fflush.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fflush.data.traceback);
}

void fflush_mock_implementation(int (*implementation)(FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_fflush);
    nala_mock_fflush.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fflush.data.implementation = implementation;
    nala_traceback(&nala_mock_fflush.data.traceback);
}

void fflush_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fflush);
    nala_mock_fflush.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fflush.data.traceback);
}

void fflush_mock_real_once(void)
{
    struct nala_instance_fflush_t *instance_p;

    nala_mock_fflush.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fflush.instances,
                          instance_p);
}

void fflush_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fflush.state);
}

void fflush_mock_resume(void)
{
    nala_state_resume(&nala_mock_fflush.state);
}

void fflush_mock_reset(void)
{
    struct nala_instance_fflush_t *current_p;
    struct nala_instance_fflush_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fflush, current_p, tmp_p);
}

void fflush_mock_assert_completed(void)
{
    if (nala_mock_fflush.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fflush() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fflush.instances.length),
                &nala_mock_fflush.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fileno

int __real_fileno(FILE *stream);

struct nala_params_fileno_t {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fileno_t {
    struct nala_params_fileno_t params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_fileno_params_t params_in;
};

struct nala_instance_fileno_t {
    int mode;
    int handle;
    struct nala_data_fileno_t data;
    struct nala_instance_fileno_t *next_p;
};

struct nala_instances_fileno_t {
    struct nala_instance_fileno_t *head_p;
    struct nala_instance_fileno_t *next_p;
    struct nala_instance_fileno_t *tail_p;
    int length;
};

struct nala_mock_fileno_t {
    struct nala_state_t state;
    struct nala_data_fileno_t data;
    struct nala_instances_fileno_t instances;
};

static struct nala_mock_fileno_t nala_mock_fileno = {
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

struct nala_data_fileno_t *nala_get_data_fileno()
{
    if (nala_mock_fileno.instances.tail_p != NULL) {
        return (&nala_mock_fileno.instances.tail_p->data);
    } else {
        return (&nala_mock_fileno.data);
    }
}

struct nala_params_fileno_t *nala_get_params_fileno()
{
    return (&nala_get_data_fileno()->params);
}

int __wrap_fileno(FILE *stream)
{
    struct nala_instance_fileno_t *nala_instance_p;
    struct nala_data_fileno_t *nala_data_p;
    int return_value;

    nala_print_call("fileno", &nala_mock_fileno.state);

    switch (nala_mock_fileno.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fileno.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fileno.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fileno() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fileno.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fileno, stream);
            nala_data_p->params_in.stream = stream;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fileno,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fileno(stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fileno.data.implementation(stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fileno.data.traceback,
                            "fileno");
        exit(1);
        break;

    default:
        return_value =
        __real_fileno(stream);
        break;
    }

    return return_value;
}

void fileno_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fileno);
    nala_mock_fileno.state.mode = MODE_MOCK;
    nala_mock_fileno.data.params.stream = NULL;
    nala_mock_fileno.data.params.ignore_stream_in = true;
    nala_mock_fileno.data.return_value = return_value;
    nala_mock_fileno.data.errno_value = 0;
    nala_mock_fileno.data.callback = NULL;
    nala_traceback(&nala_mock_fileno.data.traceback);
}

int fileno_mock_once(int return_value)
{
    struct nala_instance_fileno_t *nala_instance_p;

    nala_mock_fileno.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fileno.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fileno_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fileno);
    nala_mock_fileno.state.mode = MODE_MOCK;
    nala_mock_fileno.data.params.ignore_stream_in = true;
    nala_mock_fileno.data.return_value = return_value;
    nala_mock_fileno.data.errno_value = 0;
    nala_mock_fileno.data.callback = NULL;
}

int fileno_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fileno_t *instance_p;

    nala_mock_fileno.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fileno.instances,
                          instance_p);

    return (instance_p->handle);
}

void fileno_mock_set_errno(int errno_value)
{
    nala_get_data_fileno()->errno_value = errno_value;
}

void fileno_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fileno()->callback = callback;
}

struct nala_fileno_params_t *fileno_mock_get_params_in(int handle)
{
    struct nala_instance_fileno_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fileno.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fileno() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fileno_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fileno()->stream_in,
                       buf_p,
                       size);
}

void fileno_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fileno_t *nala_params_p;

    nala_params_p = nala_get_params_fileno();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fileno_mock_set_stream_in() must be called "
                "before fileno_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fileno_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fileno_t *nala_params_p;

    nala_params_p = nala_get_params_fileno();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fileno_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fileno()->stream_out,
                       buf_p,
                       size);
}

void fileno_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fileno_t *nala_params_p;

    nala_params_p = nala_get_params_fileno();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fileno_mock_set_stream_out() must be called "
                "before fileno_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fileno_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fileno);
    nala_mock_fileno.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fileno.data.traceback);
}

void fileno_mock_implementation(int (*implementation)(FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_fileno);
    nala_mock_fileno.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fileno.data.implementation = implementation;
    nala_traceback(&nala_mock_fileno.data.traceback);
}

void fileno_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fileno);
    nala_mock_fileno.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fileno.data.traceback);
}

void fileno_mock_real_once(void)
{
    struct nala_instance_fileno_t *instance_p;

    nala_mock_fileno.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fileno.instances,
                          instance_p);
}

void fileno_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fileno.state);
}

void fileno_mock_resume(void)
{
    nala_state_resume(&nala_mock_fileno.state);
}

void fileno_mock_reset(void)
{
    struct nala_instance_fileno_t *current_p;
    struct nala_instance_fileno_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fileno, current_p, tmp_p);
}

void fileno_mock_assert_completed(void)
{
    if (nala_mock_fileno.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fileno() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fileno.instances.length),
                &nala_mock_fileno.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fopen

FILE *__real_fopen(const char *path, const char *mode);

struct nala_params_fopen_t {
    const char *path;
    const char *mode;
    bool ignore_path_in;
    struct nala_set_param path_in;
    void (*path_in_assert)(const char *path, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param path_out;
    void (*path_out_copy)(const char *path, const void *nala_buf_p, size_t nala_size);
    bool ignore_mode_in;
    struct nala_set_param mode_in;
    void (*mode_in_assert)(const char *mode, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param mode_out;
    void (*mode_out_copy)(const char *mode, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fopen_t {
    struct nala_params_fopen_t params;
    FILE *return_value;
    int errno_value;
    FILE *(*implementation)(const char *path, const char *mode);
    void (*callback)(const char *path, const char *mode);
    struct nala_traceback_t traceback;
    struct nala_fopen_params_t params_in;
};

struct nala_instance_fopen_t {
    int mode;
    int handle;
    struct nala_data_fopen_t data;
    struct nala_instance_fopen_t *next_p;
};

struct nala_instances_fopen_t {
    struct nala_instance_fopen_t *head_p;
    struct nala_instance_fopen_t *next_p;
    struct nala_instance_fopen_t *tail_p;
    int length;
};

struct nala_mock_fopen_t {
    struct nala_state_t state;
    struct nala_data_fopen_t data;
    struct nala_instances_fopen_t instances;
};

static struct nala_mock_fopen_t nala_mock_fopen = {
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

struct nala_data_fopen_t *nala_get_data_fopen()
{
    if (nala_mock_fopen.instances.tail_p != NULL) {
        return (&nala_mock_fopen.instances.tail_p->data);
    } else {
        return (&nala_mock_fopen.data);
    }
}

struct nala_params_fopen_t *nala_get_params_fopen()
{
    return (&nala_get_data_fopen()->params);
}

FILE *__wrap_fopen(const char *path, const char *mode)
{
    struct nala_instance_fopen_t *nala_instance_p;
    struct nala_data_fopen_t *nala_data_p;
    FILE *return_value;

    nala_print_call("fopen", &nala_mock_fopen.state);

    switch (nala_mock_fopen.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fopen.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fopen.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fopen() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fopen.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, fopen, path);
            nala_data_p->params_in.path = path;
            MOCK_ASSERT_IN_EQ(nala_data_p, fopen, mode);
            nala_data_p->params_in.mode = mode;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       fopen,
                                       path);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       fopen,
                                       mode);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(path, mode);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fopen(path, mode);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fopen.data.implementation(path, mode);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fopen.data.traceback,
                            "fopen");
        exit(1);
        break;

    default:
        return_value =
        __real_fopen(path, mode);
        break;
    }

    return return_value;
}

void fopen_mock(const char *path, const char *mode, FILE *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fopen);
    nala_mock_fopen.state.mode = MODE_MOCK;
    nala_mock_fopen.data.params.path = NULL;
    nala_mock_fopen.data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_mock_fopen.data.params.path_in,
                              path);
    } else {
        nala_mock_fopen.data.params.ignore_path_in = false;
    }

    nala_mock_fopen.data.params.mode = NULL;
    nala_mock_fopen.data.params.ignore_mode_in = true;

    if (mode != NULL) {
        nala_set_param_string(&nala_mock_fopen.data.params.mode_in,
                              mode);
    } else {
        nala_mock_fopen.data.params.ignore_mode_in = false;
    }

    nala_mock_fopen.data.return_value = return_value;
    nala_mock_fopen.data.errno_value = 0;
    nala_mock_fopen.data.callback = NULL;
    nala_traceback(&nala_mock_fopen.data.traceback);
}

int fopen_mock_once(const char *path, const char *mode, FILE *return_value)
{
    struct nala_instance_fopen_t *nala_instance_p;

    nala_mock_fopen.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.path_out);
    nala_set_param_init(&nala_instance_p->data.params.path_in);
    nala_instance_p->data.params.path_in_assert = NULL;
    nala_instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.mode_out);
    nala_set_param_init(&nala_instance_p->data.params.mode_in);
    nala_instance_p->data.params.mode_in_assert = NULL;
    nala_instance_p->data.params.mode_out_copy = NULL;
    nala_instance_p->data.params.path = NULL;
    nala_instance_p->data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.path_in,
                              path);
    } else {
        nala_instance_p->data.params.ignore_path_in = false;
    }

    nala_instance_p->data.params.mode = NULL;
    nala_instance_p->data.params.ignore_mode_in = true;

    if (mode != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.mode_in,
                              mode);
    } else {
        nala_instance_p->data.params.ignore_mode_in = false;
    }

    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fopen.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fopen_mock_ignore_in(FILE *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fopen);
    nala_mock_fopen.state.mode = MODE_MOCK;
    nala_mock_fopen.data.params.ignore_path_in = true;
    nala_mock_fopen.data.params.ignore_mode_in = true;
    nala_mock_fopen.data.return_value = return_value;
    nala_mock_fopen.data.errno_value = 0;
    nala_mock_fopen.data.callback = NULL;
}

int fopen_mock_ignore_in_once(FILE *return_value)
{
    struct nala_instance_fopen_t *instance_p;

    nala_mock_fopen.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.path_out);
    nala_set_param_init(&instance_p->data.params.path_in);
    instance_p->data.params.path_in_assert = NULL;
    instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.mode_out);
    nala_set_param_init(&instance_p->data.params.mode_in);
    instance_p->data.params.mode_in_assert = NULL;
    instance_p->data.params.mode_out_copy = NULL;
    instance_p->data.params.path = NULL;
    instance_p->data.params.ignore_path_in = true;
    instance_p->data.params.mode = NULL;
    instance_p->data.params.ignore_mode_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fopen.instances,
                          instance_p);

    return (instance_p->handle);
}

void fopen_mock_set_errno(int errno_value)
{
    nala_get_data_fopen()->errno_value = errno_value;
}

void fopen_mock_set_callback(void (*callback)(const char *path, const char *mode))
{
    nala_get_data_fopen()->callback = callback;
}

struct nala_fopen_params_t *fopen_mock_get_params_in(int handle)
{
    struct nala_instance_fopen_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fopen.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fopen() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fopen_mock_ignore_path_in(void)
{
    nala_get_params_fopen()->ignore_path_in = true;
}

void fopen_mock_ignore_mode_in(void)
{
    nala_get_params_fopen()->ignore_mode_in = true;
}

void fopen_mock_set_path_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->path_in,
                       buf_p,
                       size);
}

void fopen_mock_set_path_in_assert(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();

    if (nala_params_p->path_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fopen_mock_set_path_in() must be called "
                "before fopen_mock_set_path_in_assert().\n"));
    }

    nala_params_p->path_in_assert = callback;
}

void fopen_mock_set_path_in_pointer(const char *path)
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();
    nala_params_p->ignore_path_in = false;
    nala_params_p->path = path;
}

void fopen_mock_set_path_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->path_out,
                       buf_p,
                       size);
}

void fopen_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();

    if (nala_params_p->path_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fopen_mock_set_path_out() must be called "
                "before fopen_mock_set_path_out_copy().\n"));
    }

    nala_params_p->path_out_copy = callback;
}

void fopen_mock_set_mode_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->mode_in,
                       buf_p,
                       size);
}

void fopen_mock_set_mode_in_assert(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();

    if (nala_params_p->mode_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fopen_mock_set_mode_in() must be called "
                "before fopen_mock_set_mode_in_assert().\n"));
    }

    nala_params_p->mode_in_assert = callback;
}

void fopen_mock_set_mode_in_pointer(const char *mode)
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();
    nala_params_p->ignore_mode_in = false;
    nala_params_p->mode = mode;
}

void fopen_mock_set_mode_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->mode_out,
                       buf_p,
                       size);
}

void fopen_mock_set_mode_out_copy(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fopen_t *nala_params_p;

    nala_params_p = nala_get_params_fopen();

    if (nala_params_p->mode_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fopen_mock_set_mode_out() must be called "
                "before fopen_mock_set_mode_out_copy().\n"));
    }

    nala_params_p->mode_out_copy = callback;
}

void fopen_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fopen);
    nala_mock_fopen.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fopen.data.traceback);
}

void fopen_mock_implementation(FILE *(*implementation)(const char *path, const char *mode))
{
    CHECK_NO_INSTANCES(nala_mock_fopen);
    nala_mock_fopen.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fopen.data.implementation = implementation;
    nala_traceback(&nala_mock_fopen.data.traceback);
}

void fopen_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fopen);
    nala_mock_fopen.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fopen.data.traceback);
}

void fopen_mock_real_once(void)
{
    struct nala_instance_fopen_t *instance_p;

    nala_mock_fopen.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fopen.instances,
                          instance_p);
}

void fopen_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fopen.state);
}

void fopen_mock_resume(void)
{
    nala_state_resume(&nala_mock_fopen.state);
}

void fopen_mock_reset(void)
{
    struct nala_instance_fopen_t *current_p;
    struct nala_instance_fopen_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fopen, current_p, tmp_p);
}

void fopen_mock_assert_completed(void)
{
    if (nala_mock_fopen.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fopen() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fopen.instances.length),
                &nala_mock_fopen.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fread

size_t __real_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

struct nala_params_fread_t {
    void *ptr;
    size_t size;
    size_t nmemb;
    FILE *stream;
    bool ignore_ptr_in;
    struct nala_set_param ptr_in;
    void (*ptr_in_assert)(void *ptr, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param ptr_out;
    void (*ptr_out_copy)(void *ptr, const void *nala_buf_p, size_t nala_size);
    bool ignore_size_in;
    bool ignore_nmemb_in;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fread_t {
    struct nala_params_fread_t params;
    size_t return_value;
    int errno_value;
    size_t (*implementation)(void *ptr, size_t size, size_t nmemb, FILE *stream);
    void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_fread_params_t params_in;
};

struct nala_instance_fread_t {
    int mode;
    int handle;
    struct nala_data_fread_t data;
    struct nala_instance_fread_t *next_p;
};

struct nala_instances_fread_t {
    struct nala_instance_fread_t *head_p;
    struct nala_instance_fread_t *next_p;
    struct nala_instance_fread_t *tail_p;
    int length;
};

struct nala_mock_fread_t {
    struct nala_state_t state;
    struct nala_data_fread_t data;
    struct nala_instances_fread_t instances;
};

static struct nala_mock_fread_t nala_mock_fread = {
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

struct nala_data_fread_t *nala_get_data_fread()
{
    if (nala_mock_fread.instances.tail_p != NULL) {
        return (&nala_mock_fread.instances.tail_p->data);
    } else {
        return (&nala_mock_fread.data);
    }
}

struct nala_params_fread_t *nala_get_params_fread()
{
    return (&nala_get_data_fread()->params);
}

size_t __wrap_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    struct nala_instance_fread_t *nala_instance_p;
    struct nala_data_fread_t *nala_data_p;
    size_t return_value;

    nala_print_call("fread", &nala_mock_fread.state);

    switch (nala_mock_fread.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fread.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fread.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fread() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fread.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fread, ptr);
            nala_data_p->params_in.ptr = ptr;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fread, stream);
            nala_data_p->params_in.stream = stream;
            MOCK_ASSERT_IN_EQ(nala_data_p, fread, size);
            nala_data_p->params_in.size = size;
            MOCK_ASSERT_IN_EQ(nala_data_p, fread, nmemb);
            nala_data_p->params_in.nmemb = nmemb;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fread,
                                       ptr);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fread,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(ptr, size, nmemb, stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fread(ptr, size, nmemb, stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fread.data.implementation(ptr, size, nmemb, stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fread.data.traceback,
                            "fread");
        exit(1);
        break;

    default:
        return_value =
        __real_fread(ptr, size, nmemb, stream);
        break;
    }

    return return_value;
}

void fread_mock(size_t size, size_t nmemb, size_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fread);
    nala_mock_fread.state.mode = MODE_MOCK;
    nala_mock_fread.data.params.ptr = NULL;
    nala_mock_fread.data.params.ignore_ptr_in = true;
    nala_mock_fread.data.params.stream = NULL;
    nala_mock_fread.data.params.ignore_stream_in = true;
    nala_mock_fread.data.params.size = size;
    nala_mock_fread.data.params.ignore_size_in = false;
    nala_mock_fread.data.params.nmemb = nmemb;
    nala_mock_fread.data.params.ignore_nmemb_in = false;
    nala_mock_fread.data.return_value = return_value;
    nala_mock_fread.data.errno_value = 0;
    nala_mock_fread.data.callback = NULL;
    nala_traceback(&nala_mock_fread.data.traceback);
}

int fread_mock_once(size_t size, size_t nmemb, size_t return_value)
{
    struct nala_instance_fread_t *nala_instance_p;

    nala_mock_fread.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&nala_instance_p->data.params.ptr_in);
    nala_instance_p->data.params.ptr_in_assert = NULL;
    nala_instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.ptr = NULL;
    nala_instance_p->data.params.ignore_ptr_in = true;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.params.size = size;
    nala_instance_p->data.params.ignore_size_in = false;
    nala_instance_p->data.params.nmemb = nmemb;
    nala_instance_p->data.params.ignore_nmemb_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fread.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fread_mock_ignore_in(size_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fread);
    nala_mock_fread.state.mode = MODE_MOCK;
    nala_mock_fread.data.params.ignore_ptr_in = true;
    nala_mock_fread.data.params.ignore_stream_in = true;
    nala_mock_fread.data.params.ignore_size_in = true;
    nala_mock_fread.data.params.ignore_nmemb_in = true;
    nala_mock_fread.data.return_value = return_value;
    nala_mock_fread.data.errno_value = 0;
    nala_mock_fread.data.callback = NULL;
}

int fread_mock_ignore_in_once(size_t return_value)
{
    struct nala_instance_fread_t *instance_p;

    nala_mock_fread.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.ptr_out);
    nala_set_param_init(&instance_p->data.params.ptr_in);
    instance_p->data.params.ptr_in_assert = NULL;
    instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.ptr = NULL;
    instance_p->data.params.ignore_ptr_in = true;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.params.ignore_size_in = true;
    instance_p->data.params.ignore_nmemb_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fread.instances,
                          instance_p);

    return (instance_p->handle);
}

void fread_mock_set_errno(int errno_value)
{
    nala_get_data_fread()->errno_value = errno_value;
}

void fread_mock_set_callback(void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_get_data_fread()->callback = callback;
}

struct nala_fread_params_t *fread_mock_get_params_in(int handle)
{
    struct nala_instance_fread_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fread.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fread() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fread_mock_ignore_size_in(void)
{
    nala_get_params_fread()->ignore_size_in = true;
}

void fread_mock_ignore_nmemb_in(void)
{
    nala_get_params_fread()->ignore_nmemb_in = true;
}

void fread_mock_set_ptr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->ptr_in,
                       buf_p,
                       size);
}

void fread_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();

    if (nala_params_p->ptr_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fread_mock_set_ptr_in() must be called "
                "before fread_mock_set_ptr_in_assert().\n"));
    }

    nala_params_p->ptr_in_assert = callback;
}

void fread_mock_set_ptr_in_pointer(void *ptr)
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();
    nala_params_p->ignore_ptr_in = false;
    nala_params_p->ptr = ptr;
}

void fread_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->ptr_out,
                       buf_p,
                       size);
}

void fread_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();

    if (nala_params_p->ptr_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fread_mock_set_ptr_out() must be called "
                "before fread_mock_set_ptr_out_copy().\n"));
    }

    nala_params_p->ptr_out_copy = callback;
}

void fread_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->stream_in,
                       buf_p,
                       size);
}

void fread_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fread_mock_set_stream_in() must be called "
                "before fread_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fread_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fread_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->stream_out,
                       buf_p,
                       size);
}

void fread_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fread_t *nala_params_p;

    nala_params_p = nala_get_params_fread();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fread_mock_set_stream_out() must be called "
                "before fread_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fread_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fread);
    nala_mock_fread.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fread.data.traceback);
}

void fread_mock_implementation(size_t (*implementation)(void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_fread);
    nala_mock_fread.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fread.data.implementation = implementation;
    nala_traceback(&nala_mock_fread.data.traceback);
}

void fread_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fread);
    nala_mock_fread.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fread.data.traceback);
}

void fread_mock_real_once(void)
{
    struct nala_instance_fread_t *instance_p;

    nala_mock_fread.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fread.instances,
                          instance_p);
}

void fread_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fread.state);
}

void fread_mock_resume(void)
{
    nala_state_resume(&nala_mock_fread.state);
}

void fread_mock_reset(void)
{
    struct nala_instance_fread_t *current_p;
    struct nala_instance_fread_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fread, current_p, tmp_p);
}

void fread_mock_assert_completed(void)
{
    if (nala_mock_fread.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fread() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fread.instances.length),
                &nala_mock_fread.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION free

void __real_free(void *ptr);

struct nala_params_free_t {
    void *ptr;
    bool ignore_ptr_in;
    struct nala_set_param ptr_in;
    void (*ptr_in_assert)(void *ptr, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param ptr_out;
    void (*ptr_out_copy)(void *ptr, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_free_t {
    struct nala_params_free_t params;
    int errno_value;
    void (*implementation)(void *ptr);
    void (*callback)(void *ptr);
    struct nala_traceback_t traceback;
    struct nala_free_params_t params_in;
};

struct nala_instance_free_t {
    int mode;
    int handle;
    struct nala_data_free_t data;
    struct nala_instance_free_t *next_p;
};

struct nala_instances_free_t {
    struct nala_instance_free_t *head_p;
    struct nala_instance_free_t *next_p;
    struct nala_instance_free_t *tail_p;
    int length;
};

struct nala_mock_free_t {
    struct nala_state_t state;
    struct nala_data_free_t data;
    struct nala_instances_free_t instances;
};

static struct nala_mock_free_t nala_mock_free = {
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

struct nala_data_free_t *nala_get_data_free()
{
    if (nala_mock_free.instances.tail_p != NULL) {
        return (&nala_mock_free.instances.tail_p->data);
    } else {
        return (&nala_mock_free.data);
    }
}

struct nala_params_free_t *nala_get_params_free()
{
    return (&nala_get_data_free()->params);
}

void __wrap_free(void *ptr)
{
    struct nala_instance_free_t *nala_instance_p;
    struct nala_data_free_t *nala_data_p;

    nala_print_call("free", &nala_mock_free.state);

    switch (nala_mock_free.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_free.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_free.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked free() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_free.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, free, ptr);
            nala_data_p->params_in.ptr = ptr;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       free,
                                       ptr);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(ptr);
            }

        } else {
            __real_free(ptr);
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_free.data.implementation(ptr);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_free.data.traceback,
                            "free");
        exit(1);
        break;

    default:
        __real_free(ptr);
        break;
    }

    return;
}

void free_mock()
{
    CHECK_NO_INSTANCES(nala_mock_free);
    nala_mock_free.state.mode = MODE_MOCK;
    nala_mock_free.data.params.ptr = NULL;
    nala_mock_free.data.params.ignore_ptr_in = true;
    nala_mock_free.data.errno_value = 0;
    nala_mock_free.data.callback = NULL;
    nala_traceback(&nala_mock_free.data.traceback);
}

int free_mock_once()
{
    struct nala_instance_free_t *nala_instance_p;

    nala_mock_free.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&nala_instance_p->data.params.ptr_in);
    nala_instance_p->data.params.ptr_in_assert = NULL;
    nala_instance_p->data.params.ptr_out_copy = NULL;
    nala_instance_p->data.params.ptr = NULL;
    nala_instance_p->data.params.ignore_ptr_in = true;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_free.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void free_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_free);
    nala_mock_free.state.mode = MODE_MOCK;
    nala_mock_free.data.params.ignore_ptr_in = true;
    nala_mock_free.data.errno_value = 0;
    nala_mock_free.data.callback = NULL;
}

int free_mock_ignore_in_once(void)
{
    struct nala_instance_free_t *instance_p;

    nala_mock_free.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.ptr_out);
    nala_set_param_init(&instance_p->data.params.ptr_in);
    instance_p->data.params.ptr_in_assert = NULL;
    instance_p->data.params.ptr_out_copy = NULL;
    instance_p->data.params.ptr = NULL;
    instance_p->data.params.ignore_ptr_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_free.instances,
                          instance_p);

    return (instance_p->handle);
}

void free_mock_set_errno(int errno_value)
{
    nala_get_data_free()->errno_value = errno_value;
}

void free_mock_set_callback(void (*callback)(void *ptr))
{
    nala_get_data_free()->callback = callback;
}

struct nala_free_params_t *free_mock_get_params_in(int handle)
{
    struct nala_instance_free_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_free.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "free() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void free_mock_set_ptr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_free()->ptr_in,
                       buf_p,
                       size);
}

void free_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_free_t *nala_params_p;

    nala_params_p = nala_get_params_free();

    if (nala_params_p->ptr_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "free_mock_set_ptr_in() must be called "
                "before free_mock_set_ptr_in_assert().\n"));
    }

    nala_params_p->ptr_in_assert = callback;
}

void free_mock_set_ptr_in_pointer(void *ptr)
{
    struct nala_params_free_t *nala_params_p;

    nala_params_p = nala_get_params_free();
    nala_params_p->ignore_ptr_in = false;
    nala_params_p->ptr = ptr;
}

void free_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_free()->ptr_out,
                       buf_p,
                       size);
}

void free_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_free_t *nala_params_p;

    nala_params_p = nala_get_params_free();

    if (nala_params_p->ptr_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "free_mock_set_ptr_out() must be called "
                "before free_mock_set_ptr_out_copy().\n"));
    }

    nala_params_p->ptr_out_copy = callback;
}

void free_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_free);
    nala_mock_free.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_free.data.traceback);
}

void free_mock_implementation(void (*implementation)(void *ptr))
{
    CHECK_NO_INSTANCES(nala_mock_free);
    nala_mock_free.state.mode = MODE_IMPLEMENTATION;
    nala_mock_free.data.implementation = implementation;
    nala_traceback(&nala_mock_free.data.traceback);
}

void free_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_free);
    nala_mock_free.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_free.data.traceback);
}

void free_mock_real_once(void)
{
    struct nala_instance_free_t *instance_p;

    nala_mock_free.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_free.instances,
                          instance_p);
}

void free_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_free.state);
}

void free_mock_resume(void)
{
    nala_state_resume(&nala_mock_free.state);
}

void free_mock_reset(void)
{
    struct nala_instance_free_t *current_p;
    struct nala_instance_free_t *tmp_p;

    NALA_STATE_RESET(nala_mock_free, current_p, tmp_p);
}

void free_mock_assert_completed(void)
{
    if (nala_mock_free.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked free() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_free.instances.length),
                &nala_mock_free.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fseek

int __real_fseek(FILE *stream, long int offset, int whence);

struct nala_params_fseek_t {
    FILE *stream;
    long int offset;
    int whence;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    bool ignore_offset_in;
    bool ignore_whence_in;
};

struct nala_data_fseek_t {
    struct nala_params_fseek_t params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream, long int offset, int whence);
    void (*callback)(FILE *stream, long int offset, int whence);
    struct nala_traceback_t traceback;
    struct nala_fseek_params_t params_in;
};

struct nala_instance_fseek_t {
    int mode;
    int handle;
    struct nala_data_fseek_t data;
    struct nala_instance_fseek_t *next_p;
};

struct nala_instances_fseek_t {
    struct nala_instance_fseek_t *head_p;
    struct nala_instance_fseek_t *next_p;
    struct nala_instance_fseek_t *tail_p;
    int length;
};

struct nala_mock_fseek_t {
    struct nala_state_t state;
    struct nala_data_fseek_t data;
    struct nala_instances_fseek_t instances;
};

static struct nala_mock_fseek_t nala_mock_fseek = {
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

struct nala_data_fseek_t *nala_get_data_fseek()
{
    if (nala_mock_fseek.instances.tail_p != NULL) {
        return (&nala_mock_fseek.instances.tail_p->data);
    } else {
        return (&nala_mock_fseek.data);
    }
}

struct nala_params_fseek_t *nala_get_params_fseek()
{
    return (&nala_get_data_fseek()->params);
}

int __wrap_fseek(FILE *stream, long int offset, int whence)
{
    struct nala_instance_fseek_t *nala_instance_p;
    struct nala_data_fseek_t *nala_data_p;
    int return_value;

    nala_print_call("fseek", &nala_mock_fseek.state);

    switch (nala_mock_fseek.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fseek.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fseek.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fseek() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fseek.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fseek, stream);
            nala_data_p->params_in.stream = stream;
            MOCK_ASSERT_IN_EQ(nala_data_p, fseek, offset);
            nala_data_p->params_in.offset = offset;
            MOCK_ASSERT_IN_EQ(nala_data_p, fseek, whence);
            nala_data_p->params_in.whence = whence;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fseek,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream, offset, whence);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fseek(stream, offset, whence);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fseek.data.implementation(stream, offset, whence);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fseek.data.traceback,
                            "fseek");
        exit(1);
        break;

    default:
        return_value =
        __real_fseek(stream, offset, whence);
        break;
    }

    return return_value;
}

void fseek_mock(long int offset, int whence, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fseek);
    nala_mock_fseek.state.mode = MODE_MOCK;
    nala_mock_fseek.data.params.stream = NULL;
    nala_mock_fseek.data.params.ignore_stream_in = true;
    nala_mock_fseek.data.params.offset = offset;
    nala_mock_fseek.data.params.ignore_offset_in = false;
    nala_mock_fseek.data.params.whence = whence;
    nala_mock_fseek.data.params.ignore_whence_in = false;
    nala_mock_fseek.data.return_value = return_value;
    nala_mock_fseek.data.errno_value = 0;
    nala_mock_fseek.data.callback = NULL;
    nala_traceback(&nala_mock_fseek.data.traceback);
}

int fseek_mock_once(long int offset, int whence, int return_value)
{
    struct nala_instance_fseek_t *nala_instance_p;

    nala_mock_fseek.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.params.offset = offset;
    nala_instance_p->data.params.ignore_offset_in = false;
    nala_instance_p->data.params.whence = whence;
    nala_instance_p->data.params.ignore_whence_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fseek.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fseek_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fseek);
    nala_mock_fseek.state.mode = MODE_MOCK;
    nala_mock_fseek.data.params.ignore_stream_in = true;
    nala_mock_fseek.data.params.ignore_offset_in = true;
    nala_mock_fseek.data.params.ignore_whence_in = true;
    nala_mock_fseek.data.return_value = return_value;
    nala_mock_fseek.data.errno_value = 0;
    nala_mock_fseek.data.callback = NULL;
}

int fseek_mock_ignore_in_once(int return_value)
{
    struct nala_instance_fseek_t *instance_p;

    nala_mock_fseek.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.params.ignore_offset_in = true;
    instance_p->data.params.ignore_whence_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fseek.instances,
                          instance_p);

    return (instance_p->handle);
}

void fseek_mock_set_errno(int errno_value)
{
    nala_get_data_fseek()->errno_value = errno_value;
}

void fseek_mock_set_callback(void (*callback)(FILE *stream, long int offset, int whence))
{
    nala_get_data_fseek()->callback = callback;
}

struct nala_fseek_params_t *fseek_mock_get_params_in(int handle)
{
    struct nala_instance_fseek_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fseek.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fseek() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fseek_mock_ignore_offset_in(void)
{
    nala_get_params_fseek()->ignore_offset_in = true;
}

void fseek_mock_ignore_whence_in(void)
{
    nala_get_params_fseek()->ignore_whence_in = true;
}

void fseek_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fseek()->stream_in,
                       buf_p,
                       size);
}

void fseek_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fseek_t *nala_params_p;

    nala_params_p = nala_get_params_fseek();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fseek_mock_set_stream_in() must be called "
                "before fseek_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fseek_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fseek_t *nala_params_p;

    nala_params_p = nala_get_params_fseek();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fseek_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fseek()->stream_out,
                       buf_p,
                       size);
}

void fseek_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fseek_t *nala_params_p;

    nala_params_p = nala_get_params_fseek();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fseek_mock_set_stream_out() must be called "
                "before fseek_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fseek_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fseek);
    nala_mock_fseek.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fseek.data.traceback);
}

void fseek_mock_implementation(int (*implementation)(FILE *stream, long int offset, int whence))
{
    CHECK_NO_INSTANCES(nala_mock_fseek);
    nala_mock_fseek.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fseek.data.implementation = implementation;
    nala_traceback(&nala_mock_fseek.data.traceback);
}

void fseek_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fseek);
    nala_mock_fseek.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fseek.data.traceback);
}

void fseek_mock_real_once(void)
{
    struct nala_instance_fseek_t *instance_p;

    nala_mock_fseek.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fseek.instances,
                          instance_p);
}

void fseek_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fseek.state);
}

void fseek_mock_resume(void)
{
    nala_state_resume(&nala_mock_fseek.state);
}

void fseek_mock_reset(void)
{
    struct nala_instance_fseek_t *current_p;
    struct nala_instance_fseek_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fseek, current_p, tmp_p);
}

void fseek_mock_assert_completed(void)
{
    if (nala_mock_fseek.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fseek() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fseek.instances.length),
                &nala_mock_fseek.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION ftell

long int __real_ftell(FILE *stream);

struct nala_params_ftell_t {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_ftell_t {
    struct nala_params_ftell_t params;
    long int return_value;
    int errno_value;
    long int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_ftell_params_t params_in;
};

struct nala_instance_ftell_t {
    int mode;
    int handle;
    struct nala_data_ftell_t data;
    struct nala_instance_ftell_t *next_p;
};

struct nala_instances_ftell_t {
    struct nala_instance_ftell_t *head_p;
    struct nala_instance_ftell_t *next_p;
    struct nala_instance_ftell_t *tail_p;
    int length;
};

struct nala_mock_ftell_t {
    struct nala_state_t state;
    struct nala_data_ftell_t data;
    struct nala_instances_ftell_t instances;
};

static struct nala_mock_ftell_t nala_mock_ftell = {
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

struct nala_data_ftell_t *nala_get_data_ftell()
{
    if (nala_mock_ftell.instances.tail_p != NULL) {
        return (&nala_mock_ftell.instances.tail_p->data);
    } else {
        return (&nala_mock_ftell.data);
    }
}

struct nala_params_ftell_t *nala_get_params_ftell()
{
    return (&nala_get_data_ftell()->params);
}

long int __wrap_ftell(FILE *stream)
{
    struct nala_instance_ftell_t *nala_instance_p;
    struct nala_data_ftell_t *nala_data_p;
    long int return_value;

    nala_print_call("ftell", &nala_mock_ftell.state);

    switch (nala_mock_ftell.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_ftell.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_ftell.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked ftell() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_ftell.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, ftell, stream);
            nala_data_p->params_in.stream = stream;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       ftell,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_ftell(stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_ftell.data.implementation(stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_ftell.data.traceback,
                            "ftell");
        exit(1);
        break;

    default:
        return_value =
        __real_ftell(stream);
        break;
    }

    return return_value;
}

void ftell_mock(long int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_ftell);
    nala_mock_ftell.state.mode = MODE_MOCK;
    nala_mock_ftell.data.params.stream = NULL;
    nala_mock_ftell.data.params.ignore_stream_in = true;
    nala_mock_ftell.data.return_value = return_value;
    nala_mock_ftell.data.errno_value = 0;
    nala_mock_ftell.data.callback = NULL;
    nala_traceback(&nala_mock_ftell.data.traceback);
}

int ftell_mock_once(long int return_value)
{
    struct nala_instance_ftell_t *nala_instance_p;

    nala_mock_ftell.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_ftell.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void ftell_mock_ignore_in(long int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_ftell);
    nala_mock_ftell.state.mode = MODE_MOCK;
    nala_mock_ftell.data.params.ignore_stream_in = true;
    nala_mock_ftell.data.return_value = return_value;
    nala_mock_ftell.data.errno_value = 0;
    nala_mock_ftell.data.callback = NULL;
}

int ftell_mock_ignore_in_once(long int return_value)
{
    struct nala_instance_ftell_t *instance_p;

    nala_mock_ftell.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_ftell.instances,
                          instance_p);

    return (instance_p->handle);
}

void ftell_mock_set_errno(int errno_value)
{
    nala_get_data_ftell()->errno_value = errno_value;
}

void ftell_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_ftell()->callback = callback;
}

struct nala_ftell_params_t *ftell_mock_get_params_in(int handle)
{
    struct nala_instance_ftell_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_ftell.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "ftell() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void ftell_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_ftell()->stream_in,
                       buf_p,
                       size);
}

void ftell_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_ftell_t *nala_params_p;

    nala_params_p = nala_get_params_ftell();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "ftell_mock_set_stream_in() must be called "
                "before ftell_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void ftell_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_ftell_t *nala_params_p;

    nala_params_p = nala_get_params_ftell();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void ftell_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_ftell()->stream_out,
                       buf_p,
                       size);
}

void ftell_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_ftell_t *nala_params_p;

    nala_params_p = nala_get_params_ftell();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "ftell_mock_set_stream_out() must be called "
                "before ftell_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void ftell_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_ftell);
    nala_mock_ftell.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_ftell.data.traceback);
}

void ftell_mock_implementation(long int (*implementation)(FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_ftell);
    nala_mock_ftell.state.mode = MODE_IMPLEMENTATION;
    nala_mock_ftell.data.implementation = implementation;
    nala_traceback(&nala_mock_ftell.data.traceback);
}

void ftell_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_ftell);
    nala_mock_ftell.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_ftell.data.traceback);
}

void ftell_mock_real_once(void)
{
    struct nala_instance_ftell_t *instance_p;

    nala_mock_ftell.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_ftell.instances,
                          instance_p);
}

void ftell_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_ftell.state);
}

void ftell_mock_resume(void)
{
    nala_state_resume(&nala_mock_ftell.state);
}

void ftell_mock_reset(void)
{
    struct nala_instance_ftell_t *current_p;
    struct nala_instance_ftell_t *tmp_p;

    NALA_STATE_RESET(nala_mock_ftell, current_p, tmp_p);
}

void ftell_mock_assert_completed(void)
{
    if (nala_mock_ftell.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked ftell() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_ftell.instances.length),
                &nala_mock_ftell.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION fwrite

size_t __real_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

struct nala_params_fwrite_t {
    const void *ptr;
    size_t size;
    size_t nmemb;
    FILE *stream;
    bool ignore_ptr_in;
    struct nala_set_param ptr_in;
    void (*ptr_in_assert)(const void *ptr, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param ptr_out;
    void (*ptr_out_copy)(const void *ptr, const void *nala_buf_p, size_t nala_size);
    bool ignore_size_in;
    bool ignore_nmemb_in;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_fwrite_t {
    struct nala_params_fwrite_t params;
    size_t return_value;
    int errno_value;
    size_t (*implementation)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_fwrite_params_t params_in;
};

struct nala_instance_fwrite_t {
    int mode;
    int handle;
    struct nala_data_fwrite_t data;
    struct nala_instance_fwrite_t *next_p;
};

struct nala_instances_fwrite_t {
    struct nala_instance_fwrite_t *head_p;
    struct nala_instance_fwrite_t *next_p;
    struct nala_instance_fwrite_t *tail_p;
    int length;
};

struct nala_mock_fwrite_t {
    struct nala_state_t state;
    struct nala_data_fwrite_t data;
    struct nala_instances_fwrite_t instances;
};

static struct nala_mock_fwrite_t nala_mock_fwrite = {
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

struct nala_data_fwrite_t *nala_get_data_fwrite()
{
    if (nala_mock_fwrite.instances.tail_p != NULL) {
        return (&nala_mock_fwrite.instances.tail_p->data);
    } else {
        return (&nala_mock_fwrite.data);
    }
}

struct nala_params_fwrite_t *nala_get_params_fwrite()
{
    return (&nala_get_data_fwrite()->params);
}

size_t __wrap_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    struct nala_instance_fwrite_t *nala_instance_p;
    struct nala_data_fwrite_t *nala_data_p;
    size_t return_value;

    nala_print_call("fwrite", &nala_mock_fwrite.state);

    switch (nala_mock_fwrite.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_fwrite.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_fwrite.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked fwrite() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_fwrite.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fwrite, ptr);
            nala_data_p->params_in.ptr = ptr;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, fwrite, stream);
            nala_data_p->params_in.stream = stream;
            MOCK_ASSERT_IN_EQ(nala_data_p, fwrite, size);
            nala_data_p->params_in.size = size;
            MOCK_ASSERT_IN_EQ(nala_data_p, fwrite, nmemb);
            nala_data_p->params_in.nmemb = nmemb;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fwrite,
                                       ptr);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       fwrite,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(ptr, size, nmemb, stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_fwrite(ptr, size, nmemb, stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_fwrite.data.implementation(ptr, size, nmemb, stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_fwrite.data.traceback,
                            "fwrite");
        exit(1);
        break;

    default:
        return_value =
        __real_fwrite(ptr, size, nmemb, stream);
        break;
    }

    return return_value;
}

void fwrite_mock(size_t size, size_t nmemb, size_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fwrite);
    nala_mock_fwrite.state.mode = MODE_MOCK;
    nala_mock_fwrite.data.params.ptr = NULL;
    nala_mock_fwrite.data.params.ignore_ptr_in = true;
    nala_mock_fwrite.data.params.stream = NULL;
    nala_mock_fwrite.data.params.ignore_stream_in = true;
    nala_mock_fwrite.data.params.size = size;
    nala_mock_fwrite.data.params.ignore_size_in = false;
    nala_mock_fwrite.data.params.nmemb = nmemb;
    nala_mock_fwrite.data.params.ignore_nmemb_in = false;
    nala_mock_fwrite.data.return_value = return_value;
    nala_mock_fwrite.data.errno_value = 0;
    nala_mock_fwrite.data.callback = NULL;
    nala_traceback(&nala_mock_fwrite.data.traceback);
}

int fwrite_mock_once(size_t size, size_t nmemb, size_t return_value)
{
    struct nala_instance_fwrite_t *nala_instance_p;

    nala_mock_fwrite.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&nala_instance_p->data.params.ptr_in);
    nala_instance_p->data.params.ptr_in_assert = NULL;
    nala_instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.ptr = NULL;
    nala_instance_p->data.params.ignore_ptr_in = true;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.params.size = size;
    nala_instance_p->data.params.ignore_size_in = false;
    nala_instance_p->data.params.nmemb = nmemb;
    nala_instance_p->data.params.ignore_nmemb_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fwrite.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void fwrite_mock_ignore_in(size_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_fwrite);
    nala_mock_fwrite.state.mode = MODE_MOCK;
    nala_mock_fwrite.data.params.ignore_ptr_in = true;
    nala_mock_fwrite.data.params.ignore_stream_in = true;
    nala_mock_fwrite.data.params.ignore_size_in = true;
    nala_mock_fwrite.data.params.ignore_nmemb_in = true;
    nala_mock_fwrite.data.return_value = return_value;
    nala_mock_fwrite.data.errno_value = 0;
    nala_mock_fwrite.data.callback = NULL;
}

int fwrite_mock_ignore_in_once(size_t return_value)
{
    struct nala_instance_fwrite_t *instance_p;

    nala_mock_fwrite.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.ptr_out);
    nala_set_param_init(&instance_p->data.params.ptr_in);
    instance_p->data.params.ptr_in_assert = NULL;
    instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.ptr = NULL;
    instance_p->data.params.ignore_ptr_in = true;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.params.ignore_size_in = true;
    instance_p->data.params.ignore_nmemb_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_fwrite.instances,
                          instance_p);

    return (instance_p->handle);
}

void fwrite_mock_set_errno(int errno_value)
{
    nala_get_data_fwrite()->errno_value = errno_value;
}

void fwrite_mock_set_callback(void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_get_data_fwrite()->callback = callback;
}

struct nala_fwrite_params_t *fwrite_mock_get_params_in(int handle)
{
    struct nala_instance_fwrite_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_fwrite.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "fwrite() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void fwrite_mock_ignore_size_in(void)
{
    nala_get_params_fwrite()->ignore_size_in = true;
}

void fwrite_mock_ignore_nmemb_in(void)
{
    nala_get_params_fwrite()->ignore_nmemb_in = true;
}

void fwrite_mock_set_ptr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->ptr_in,
                       buf_p,
                       size);
}

void fwrite_mock_set_ptr_in_assert(void (*callback)(const void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();

    if (nala_params_p->ptr_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fwrite_mock_set_ptr_in() must be called "
                "before fwrite_mock_set_ptr_in_assert().\n"));
    }

    nala_params_p->ptr_in_assert = callback;
}

void fwrite_mock_set_ptr_in_pointer(const void *ptr)
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();
    nala_params_p->ignore_ptr_in = false;
    nala_params_p->ptr = ptr;
}

void fwrite_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->ptr_out,
                       buf_p,
                       size);
}

void fwrite_mock_set_ptr_out_copy(void (*callback)(const void *ptr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();

    if (nala_params_p->ptr_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fwrite_mock_set_ptr_out() must be called "
                "before fwrite_mock_set_ptr_out_copy().\n"));
    }

    nala_params_p->ptr_out_copy = callback;
}

void fwrite_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->stream_in,
                       buf_p,
                       size);
}

void fwrite_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fwrite_mock_set_stream_in() must be called "
                "before fwrite_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void fwrite_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void fwrite_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->stream_out,
                       buf_p,
                       size);
}

void fwrite_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_fwrite_t *nala_params_p;

    nala_params_p = nala_get_params_fwrite();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "fwrite_mock_set_stream_out() must be called "
                "before fwrite_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void fwrite_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_fwrite);
    nala_mock_fwrite.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_fwrite.data.traceback);
}

void fwrite_mock_implementation(size_t (*implementation)(const void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_fwrite);
    nala_mock_fwrite.state.mode = MODE_IMPLEMENTATION;
    nala_mock_fwrite.data.implementation = implementation;
    nala_traceback(&nala_mock_fwrite.data.traceback);
}

void fwrite_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_fwrite);
    nala_mock_fwrite.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_fwrite.data.traceback);
}

void fwrite_mock_real_once(void)
{
    struct nala_instance_fwrite_t *instance_p;

    nala_mock_fwrite.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_fwrite.instances,
                          instance_p);
}

void fwrite_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_fwrite.state);
}

void fwrite_mock_resume(void)
{
    nala_state_resume(&nala_mock_fwrite.state);
}

void fwrite_mock_reset(void)
{
    struct nala_instance_fwrite_t *current_p;
    struct nala_instance_fwrite_t *tmp_p;

    NALA_STATE_RESET(nala_mock_fwrite, current_p, tmp_p);
}

void fwrite_mock_assert_completed(void)
{
    if (nala_mock_fwrite.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked fwrite() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_fwrite.instances.length),
                &nala_mock_fwrite.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION getmntent

struct mntent *__real_getmntent(FILE *stream);

struct nala_params_getmntent_t {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_getmntent_t {
    struct nala_params_getmntent_t params;
    struct mntent *return_value;
    int errno_value;
    struct mntent *(*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
    struct nala_getmntent_params_t params_in;
};

struct nala_instance_getmntent_t {
    int mode;
    int handle;
    struct nala_data_getmntent_t data;
    struct nala_instance_getmntent_t *next_p;
};

struct nala_instances_getmntent_t {
    struct nala_instance_getmntent_t *head_p;
    struct nala_instance_getmntent_t *next_p;
    struct nala_instance_getmntent_t *tail_p;
    int length;
};

struct nala_mock_getmntent_t {
    struct nala_state_t state;
    struct nala_data_getmntent_t data;
    struct nala_instances_getmntent_t instances;
};

static struct nala_mock_getmntent_t nala_mock_getmntent = {
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

struct nala_data_getmntent_t *nala_get_data_getmntent()
{
    if (nala_mock_getmntent.instances.tail_p != NULL) {
        return (&nala_mock_getmntent.instances.tail_p->data);
    } else {
        return (&nala_mock_getmntent.data);
    }
}

struct nala_params_getmntent_t *nala_get_params_getmntent()
{
    return (&nala_get_data_getmntent()->params);
}

struct mntent *__wrap_getmntent(FILE *stream)
{
    struct nala_instance_getmntent_t *nala_instance_p;
    struct nala_data_getmntent_t *nala_data_p;
    struct mntent *return_value;

    nala_print_call("getmntent", &nala_mock_getmntent.state);

    switch (nala_mock_getmntent.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_getmntent.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_getmntent.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked getmntent() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_getmntent.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, getmntent, stream);
            nala_data_p->params_in.stream = stream;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       getmntent,
                                       stream);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(stream);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_getmntent(stream);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_getmntent.data.implementation(stream);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_getmntent.data.traceback,
                            "getmntent");
        exit(1);
        break;

    default:
        return_value =
        __real_getmntent(stream);
        break;
    }

    return return_value;
}

void getmntent_mock(struct mntent *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_getmntent);
    nala_mock_getmntent.state.mode = MODE_MOCK;
    nala_mock_getmntent.data.params.stream = NULL;
    nala_mock_getmntent.data.params.ignore_stream_in = true;
    nala_mock_getmntent.data.return_value = return_value;
    nala_mock_getmntent.data.errno_value = 0;
    nala_mock_getmntent.data.callback = NULL;
    nala_traceback(&nala_mock_getmntent.data.traceback);
}

int getmntent_mock_once(struct mntent *return_value)
{
    struct nala_instance_getmntent_t *nala_instance_p;

    nala_mock_getmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.stream_out);
    nala_set_param_init(&nala_instance_p->data.params.stream_in);
    nala_instance_p->data.params.stream_in_assert = NULL;
    nala_instance_p->data.params.stream_out_copy = NULL;
    nala_instance_p->data.params.stream = NULL;
    nala_instance_p->data.params.ignore_stream_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_getmntent.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void getmntent_mock_ignore_in(struct mntent *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_getmntent);
    nala_mock_getmntent.state.mode = MODE_MOCK;
    nala_mock_getmntent.data.params.ignore_stream_in = true;
    nala_mock_getmntent.data.return_value = return_value;
    nala_mock_getmntent.data.errno_value = 0;
    nala_mock_getmntent.data.callback = NULL;
}

int getmntent_mock_ignore_in_once(struct mntent *return_value)
{
    struct nala_instance_getmntent_t *instance_p;

    nala_mock_getmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_getmntent.instances,
                          instance_p);

    return (instance_p->handle);
}

void getmntent_mock_set_errno(int errno_value)
{
    nala_get_data_getmntent()->errno_value = errno_value;
}

void getmntent_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_getmntent()->callback = callback;
}

struct nala_getmntent_params_t *getmntent_mock_get_params_in(int handle)
{
    struct nala_instance_getmntent_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_getmntent.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "getmntent() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void getmntent_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_getmntent()->stream_in,
                       buf_p,
                       size);
}

void getmntent_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_getmntent_t *nala_params_p;

    nala_params_p = nala_get_params_getmntent();

    if (nala_params_p->stream_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "getmntent_mock_set_stream_in() must be called "
                "before getmntent_mock_set_stream_in_assert().\n"));
    }

    nala_params_p->stream_in_assert = callback;
}

void getmntent_mock_set_stream_in_pointer(FILE *stream)
{
    struct nala_params_getmntent_t *nala_params_p;

    nala_params_p = nala_get_params_getmntent();
    nala_params_p->ignore_stream_in = false;
    nala_params_p->stream = stream;
}

void getmntent_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_getmntent()->stream_out,
                       buf_p,
                       size);
}

void getmntent_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_getmntent_t *nala_params_p;

    nala_params_p = nala_get_params_getmntent();

    if (nala_params_p->stream_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "getmntent_mock_set_stream_out() must be called "
                "before getmntent_mock_set_stream_out_copy().\n"));
    }

    nala_params_p->stream_out_copy = callback;
}

void getmntent_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_getmntent);
    nala_mock_getmntent.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_getmntent.data.traceback);
}

void getmntent_mock_implementation(struct mntent *(*implementation)(FILE *stream))
{
    CHECK_NO_INSTANCES(nala_mock_getmntent);
    nala_mock_getmntent.state.mode = MODE_IMPLEMENTATION;
    nala_mock_getmntent.data.implementation = implementation;
    nala_traceback(&nala_mock_getmntent.data.traceback);
}

void getmntent_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_getmntent);
    nala_mock_getmntent.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_getmntent.data.traceback);
}

void getmntent_mock_real_once(void)
{
    struct nala_instance_getmntent_t *instance_p;

    nala_mock_getmntent.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_getmntent.instances,
                          instance_p);
}

void getmntent_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_getmntent.state);
}

void getmntent_mock_resume(void)
{
    nala_state_resume(&nala_mock_getmntent.state);
}

void getmntent_mock_reset(void)
{
    struct nala_instance_getmntent_t *current_p;
    struct nala_instance_getmntent_t *tmp_p;

    NALA_STATE_RESET(nala_mock_getmntent, current_p, tmp_p);
}

void getmntent_mock_assert_completed(void)
{
    if (nala_mock_getmntent.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked getmntent() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_getmntent.instances.length),
                &nala_mock_getmntent.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION in_out

void __real_in_out(int *buf_p);

struct nala_params_in_out_t {
    int *buf_p;
    bool ignore_buf_p_in;
    struct nala_set_param buf_p_in;
    void (*buf_p_in_assert)(int *buf_p, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_p_out;
    void (*buf_p_out_copy)(int *buf_p, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_in_out_t {
    struct nala_params_in_out_t params;
    int errno_value;
    void (*implementation)(int *buf_p);
    void (*callback)(int *buf_p);
    struct nala_traceback_t traceback;
    struct nala_in_out_params_t params_in;
};

struct nala_instance_in_out_t {
    int mode;
    int handle;
    struct nala_data_in_out_t data;
    struct nala_instance_in_out_t *next_p;
};

struct nala_instances_in_out_t {
    struct nala_instance_in_out_t *head_p;
    struct nala_instance_in_out_t *next_p;
    struct nala_instance_in_out_t *tail_p;
    int length;
};

struct nala_mock_in_out_t {
    struct nala_state_t state;
    struct nala_data_in_out_t data;
    struct nala_instances_in_out_t instances;
};

static struct nala_mock_in_out_t nala_mock_in_out = {
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

struct nala_data_in_out_t *nala_get_data_in_out()
{
    if (nala_mock_in_out.instances.tail_p != NULL) {
        return (&nala_mock_in_out.instances.tail_p->data);
    } else {
        return (&nala_mock_in_out.data);
    }
}

struct nala_params_in_out_t *nala_get_params_in_out()
{
    return (&nala_get_data_in_out()->params);
}

void __wrap_in_out(int *buf_p)
{
    struct nala_instance_in_out_t *nala_instance_p;
    struct nala_data_in_out_t *nala_data_p;

    nala_print_call("in_out", &nala_mock_in_out.state);

    switch (nala_mock_in_out.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_in_out.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_in_out.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked in_out() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_in_out.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, in_out, buf_p);
            nala_data_p->params_in.buf_p = buf_p;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       in_out,
                                       buf_p);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(buf_p);
            }

        } else {
            __real_in_out(buf_p);
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_in_out.data.implementation(buf_p);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_in_out.data.traceback,
                            "in_out");
        exit(1);
        break;

    default:
        __real_in_out(buf_p);
        break;
    }

    return;
}

void in_out_mock()
{
    CHECK_NO_INSTANCES(nala_mock_in_out);
    nala_mock_in_out.state.mode = MODE_MOCK;
    nala_mock_in_out.data.params.buf_p = NULL;
    nala_mock_in_out.data.params.ignore_buf_p_in = true;
    nala_mock_in_out.data.errno_value = 0;
    nala_mock_in_out.data.callback = NULL;
    nala_traceback(&nala_mock_in_out.data.traceback);
}

int in_out_mock_once()
{
    struct nala_instance_in_out_t *nala_instance_p;

    nala_mock_in_out.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.buf_p_out);
    nala_set_param_init(&nala_instance_p->data.params.buf_p_in);
    nala_instance_p->data.params.buf_p_in_assert = NULL;
    nala_instance_p->data.params.buf_p_out_copy = NULL;
    nala_instance_p->data.params.buf_p = NULL;
    nala_instance_p->data.params.ignore_buf_p_in = true;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_in_out.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void in_out_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_in_out);
    nala_mock_in_out.state.mode = MODE_MOCK;
    nala_mock_in_out.data.params.ignore_buf_p_in = true;
    nala_mock_in_out.data.errno_value = 0;
    nala_mock_in_out.data.callback = NULL;
}

int in_out_mock_ignore_in_once(void)
{
    struct nala_instance_in_out_t *instance_p;

    nala_mock_in_out.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.buf_p_out);
    nala_set_param_init(&instance_p->data.params.buf_p_in);
    instance_p->data.params.buf_p_in_assert = NULL;
    instance_p->data.params.buf_p_out_copy = NULL;
    instance_p->data.params.buf_p = NULL;
    instance_p->data.params.ignore_buf_p_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_in_out.instances,
                          instance_p);

    return (instance_p->handle);
}

void in_out_mock_set_errno(int errno_value)
{
    nala_get_data_in_out()->errno_value = errno_value;
}

void in_out_mock_set_callback(void (*callback)(int *buf_p))
{
    nala_get_data_in_out()->callback = callback;
}

struct nala_in_out_params_t *in_out_mock_get_params_in(int handle)
{
    struct nala_instance_in_out_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_in_out.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "in_out() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void in_out_mock_set_buf_p_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_in_out()->buf_p_in,
                       buf_p,
                       size);
}

void in_out_mock_set_buf_p_in_assert(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_in_out_t *nala_params_p;

    nala_params_p = nala_get_params_in_out();

    if (nala_params_p->buf_p_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "in_out_mock_set_buf_p_in() must be called "
                "before in_out_mock_set_buf_p_in_assert().\n"));
    }

    nala_params_p->buf_p_in_assert = callback;
}

void in_out_mock_set_buf_p_in_pointer(int *buf_p)
{
    struct nala_params_in_out_t *nala_params_p;

    nala_params_p = nala_get_params_in_out();
    nala_params_p->ignore_buf_p_in = false;
    nala_params_p->buf_p = buf_p;
}

void in_out_mock_set_buf_p_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_in_out()->buf_p_out,
                       buf_p,
                       size);
}

void in_out_mock_set_buf_p_out_copy(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_in_out_t *nala_params_p;

    nala_params_p = nala_get_params_in_out();

    if (nala_params_p->buf_p_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "in_out_mock_set_buf_p_out() must be called "
                "before in_out_mock_set_buf_p_out_copy().\n"));
    }

    nala_params_p->buf_p_out_copy = callback;
}

void in_out_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_in_out);
    nala_mock_in_out.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_in_out.data.traceback);
}

void in_out_mock_implementation(void (*implementation)(int *buf_p))
{
    CHECK_NO_INSTANCES(nala_mock_in_out);
    nala_mock_in_out.state.mode = MODE_IMPLEMENTATION;
    nala_mock_in_out.data.implementation = implementation;
    nala_traceback(&nala_mock_in_out.data.traceback);
}

void in_out_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_in_out);
    nala_mock_in_out.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_in_out.data.traceback);
}

void in_out_mock_real_once(void)
{
    struct nala_instance_in_out_t *instance_p;

    nala_mock_in_out.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_in_out.instances,
                          instance_p);
}

void in_out_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_in_out.state);
}

void in_out_mock_resume(void)
{
    nala_state_resume(&nala_mock_in_out.state);
}

void in_out_mock_reset(void)
{
    struct nala_instance_in_out_t *current_p;
    struct nala_instance_in_out_t *tmp_p;

    NALA_STATE_RESET(nala_mock_in_out, current_p, tmp_p);
}

void in_out_mock_assert_completed(void)
{
    if (nala_mock_in_out.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked in_out() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_in_out.instances.length),
                &nala_mock_in_out.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION io_control

struct nala_params_io_control_t {
    int kind;
    const char *vafmt_p;
    bool ignore_kind_in;
    struct nala_va_arg_list_t nala_va_arg_list;
};

struct nala_data_io_control_t {
    struct nala_params_io_control_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int kind, va_list __nala_va_list);
    void (*callback)(int kind, va_list __nala_va_list);
    struct nala_traceback_t traceback;
    struct nala_io_control_params_t params_in;
};

struct nala_instance_io_control_t {
    int mode;
    int handle;
    struct nala_data_io_control_t data;
    struct nala_instance_io_control_t *next_p;
};

struct nala_instances_io_control_t {
    struct nala_instance_io_control_t *head_p;
    struct nala_instance_io_control_t *next_p;
    struct nala_instance_io_control_t *tail_p;
    int length;
};

struct nala_mock_io_control_t {
    struct nala_state_t state;
    struct nala_data_io_control_t data;
    struct nala_instances_io_control_t instances;
};

static struct nala_mock_io_control_t nala_mock_io_control = {
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

struct nala_data_io_control_t *nala_get_data_io_control()
{
    if (nala_mock_io_control.instances.tail_p != NULL) {
        return (&nala_mock_io_control.instances.tail_p->data);
    } else {
        return (&nala_mock_io_control.data);
    }
}

struct nala_params_io_control_t *nala_get_params_io_control()
{
    return (&nala_get_data_io_control()->params);
}

int __wrap_io_control(int kind, ...)
{
    struct nala_instance_io_control_t *nala_instance_p;
    struct nala_data_io_control_t *nala_data_p;
    int return_value;

    nala_print_call("io_control", &nala_mock_io_control.state);

    switch (nala_mock_io_control.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_io_control.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_io_control.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked io_control() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_io_control.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, io_control, kind);
            nala_data_p->params_in.kind = kind;


            {
                va_list nala_vl;
                va_start(nala_vl, kind);
                nala_va_arg_list_assert(&nala_data_p->params.nala_va_arg_list, nala_vl);
                va_end(nala_vl);
                nala_va_arg_list_destroy(&nala_data_p->params.nala_va_arg_list);
            }

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                va_list nala_vl;
                va_start(nala_vl, kind);
                nala_data_p->callback(kind, nala_vl);
                va_end(nala_vl);
            }

            return_value = nala_data_p->return_value;
        } else {
            nala_test_failure(nala_format("Real io_control() not available.\n"));
        }
        break;

    case MODE_IMPLEMENTATION:
        {
            va_list nala_vl;
            va_start(nala_vl, kind);
            return_value =
            nala_mock_io_control.data.implementation(kind, nala_vl);
            va_end(nala_vl);
        }
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_io_control.data.traceback,
                            "io_control");
        exit(1);
        break;

    default:
        nala_test_failure(nala_format("Real io_control() not available.\n"));
        break;
    }

    return return_value;
}

void io_control_mock(int kind, int return_value, const char *vafmt_p, ...)
{
    CHECK_NO_INSTANCES(nala_mock_io_control);
    nala_mock_io_control.state.mode = MODE_MOCK;
    nala_mock_io_control.data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&nala_mock_io_control.data.params.nala_va_arg_list,
                          &nala_mock_io_control.data.traceback,
                          "io_control");
    va_list nala_vl;
    va_start(nala_vl, vafmt_p);
    nala_parse_va_list(&nala_mock_io_control.data.params.nala_va_arg_list,
                       vafmt_p,
                       nala_vl);
    va_end(nala_vl);
    nala_mock_io_control.data.params.kind = kind;
    nala_mock_io_control.data.params.ignore_kind_in = false;
    nala_mock_io_control.data.return_value = return_value;
    nala_mock_io_control.data.errno_value = 0;
    nala_mock_io_control.data.callback = NULL;
    nala_traceback(&nala_mock_io_control.data.traceback);
}

int io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...)
{
    struct nala_instance_io_control_t *nala_instance_p;

    nala_mock_io_control.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&nala_instance_p->data.params.nala_va_arg_list,
                          &nala_instance_p->data.traceback,
                          "io_control");
    va_list nala_vl;
    va_start(nala_vl, vafmt_p);
    nala_parse_va_list(&nala_instance_p->data.params.nala_va_arg_list,
                       vafmt_p,
                       nala_vl);
    va_end(nala_vl);
    nala_instance_p->data.params.kind = kind;
    nala_instance_p->data.params.ignore_kind_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_io_control.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void io_control_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_io_control);
    nala_mock_io_control.state.mode = MODE_MOCK;
    nala_mock_io_control.data.params.ignore_kind_in = true;
    nala_mock_io_control.data.return_value = return_value;
    nala_mock_io_control.data.errno_value = 0;
    nala_mock_io_control.data.callback = NULL;
}

int io_control_mock_ignore_in_once(int return_value)
{
    struct nala_instance_io_control_t *instance_p;

    nala_mock_io_control.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.vafmt_p = "";
    nala_va_arg_list_init(&instance_p->data.params.nala_va_arg_list,
                          &instance_p->data.traceback,
                          "io_control");
    instance_p->data.params.ignore_kind_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_io_control.instances,
                          instance_p);

    return (instance_p->handle);
}

void io_control_mock_set_errno(int errno_value)
{
    nala_get_data_io_control()->errno_value = errno_value;
}

void io_control_mock_set_callback(void (*callback)(int kind, va_list __nala_va_list))
{
    nala_get_data_io_control()->callback = callback;
}

struct nala_io_control_params_t *io_control_mock_get_params_in(int handle)
{
    struct nala_instance_io_control_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_io_control.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "io_control() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void io_control_mock_ignore_kind_in(void)
{
    nala_get_params_io_control()->ignore_kind_in = true;
}

void io_control_mock_ignore_va_arg_in_at(unsigned int index)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    nala_va_arg_list_get(va_arg_list_p, index)->ignore_in = true;
}

void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    nala_va_arg_list_set_param_buf_in_at(va_arg_list_p, index, buf_p, size);
}

void io_control_mock_set_va_arg_in_assert_at(unsigned int index, nala_mock_in_assert_t in_assert)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "io_control_mock_set_va_arg_in_at() must be called "
                "before io_control_mock_set_va_arg_in_assert_at().\n"));
    }

    item_p->in_assert = in_assert;
}

void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->type != nala_va_arg_item_type_p_t) {
        nala_test_failure(
            nala_format("Only variadic pointer parameters can be set."));

    }

    item_p->ignore_in = false;
    item_p->p_p = buf_p;
}

void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    nala_va_arg_list_set_param_buf_out_at(va_arg_list_p, index, buf_p, size);
}

void io_control_mock_set_va_arg_out_copy_at(unsigned int index, nala_mock_out_copy_t out_copy)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_io_control()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "io_control_mock_set_va_arg_out_at() must be called "
                "before io_control_mock_set_va_arg_out_copy_at().\n"));
    }

    item_p->out_copy = out_copy;
}

void io_control_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_io_control);
    nala_mock_io_control.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_io_control.data.traceback);
}

void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list))
{
    CHECK_NO_INSTANCES(nala_mock_io_control);
    nala_mock_io_control.state.mode = MODE_IMPLEMENTATION;
    nala_mock_io_control.data.implementation = implementation;
    nala_traceback(&nala_mock_io_control.data.traceback);
}

void io_control_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_io_control);
    nala_mock_io_control.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_io_control.data.traceback);
}

void io_control_mock_real_once(void)
{
    struct nala_instance_io_control_t *instance_p;

    nala_mock_io_control.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_io_control.instances,
                          instance_p);
}

void io_control_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_io_control.state);
}

void io_control_mock_resume(void)
{
    nala_state_resume(&nala_mock_io_control.state);
}

void io_control_mock_reset(void)
{
    struct nala_instance_io_control_t *current_p;
    struct nala_instance_io_control_t *tmp_p;

    NALA_STATE_RESET(nala_mock_io_control, current_p, tmp_p);
}

void io_control_mock_assert_completed(void)
{
    if (nala_mock_io_control.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked io_control() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_io_control.instances.length),
                &nala_mock_io_control.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION io_vcontrol

int __real_io_vcontrol(int kind, va_list ap);

struct nala_params_io_vcontrol_t {
    int kind;
    va_list ap;
    bool ignore_kind_in;
};

struct nala_data_io_vcontrol_t {
    struct nala_params_io_vcontrol_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int kind, va_list ap);
    void (*callback)(int kind, va_list ap);
    struct nala_traceback_t traceback;
    struct nala_io_vcontrol_params_t params_in;
};

struct nala_instance_io_vcontrol_t {
    int mode;
    int handle;
    struct nala_data_io_vcontrol_t data;
    struct nala_instance_io_vcontrol_t *next_p;
};

struct nala_instances_io_vcontrol_t {
    struct nala_instance_io_vcontrol_t *head_p;
    struct nala_instance_io_vcontrol_t *next_p;
    struct nala_instance_io_vcontrol_t *tail_p;
    int length;
};

struct nala_mock_io_vcontrol_t {
    struct nala_state_t state;
    struct nala_data_io_vcontrol_t data;
    struct nala_instances_io_vcontrol_t instances;
};

static struct nala_mock_io_vcontrol_t nala_mock_io_vcontrol = {
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

struct nala_data_io_vcontrol_t *nala_get_data_io_vcontrol()
{
    if (nala_mock_io_vcontrol.instances.tail_p != NULL) {
        return (&nala_mock_io_vcontrol.instances.tail_p->data);
    } else {
        return (&nala_mock_io_vcontrol.data);
    }
}

struct nala_params_io_vcontrol_t *nala_get_params_io_vcontrol()
{
    return (&nala_get_data_io_vcontrol()->params);
}

int __wrap_io_vcontrol(int kind, va_list ap)
{
    struct nala_instance_io_vcontrol_t *nala_instance_p;
    struct nala_data_io_vcontrol_t *nala_data_p;
    int return_value;

    nala_print_call("io_vcontrol", &nala_mock_io_vcontrol.state);

    switch (nala_mock_io_vcontrol.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_io_vcontrol.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_io_vcontrol.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked io_vcontrol() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_io_vcontrol.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, io_vcontrol, kind);
            nala_data_p->params_in.kind = kind;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(kind, ap);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_io_vcontrol(kind, ap);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_io_vcontrol.data.implementation(kind, ap);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_io_vcontrol.data.traceback,
                            "io_vcontrol");
        exit(1);
        break;

    default:
        return_value =
        __real_io_vcontrol(kind, ap);
        break;
    }

    return return_value;
}

void io_vcontrol_mock(int kind, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_io_vcontrol);
    nala_mock_io_vcontrol.state.mode = MODE_MOCK;
    nala_mock_io_vcontrol.data.params.kind = kind;
    nala_mock_io_vcontrol.data.params.ignore_kind_in = false;
    nala_mock_io_vcontrol.data.return_value = return_value;
    nala_mock_io_vcontrol.data.errno_value = 0;
    nala_mock_io_vcontrol.data.callback = NULL;
    nala_traceback(&nala_mock_io_vcontrol.data.traceback);
}

int io_vcontrol_mock_once(int kind, int return_value)
{
    struct nala_instance_io_vcontrol_t *nala_instance_p;

    nala_mock_io_vcontrol.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.kind = kind;
    nala_instance_p->data.params.ignore_kind_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_io_vcontrol.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void io_vcontrol_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_io_vcontrol);
    nala_mock_io_vcontrol.state.mode = MODE_MOCK;
    nala_mock_io_vcontrol.data.params.ignore_kind_in = true;
    nala_mock_io_vcontrol.data.return_value = return_value;
    nala_mock_io_vcontrol.data.errno_value = 0;
    nala_mock_io_vcontrol.data.callback = NULL;
}

int io_vcontrol_mock_ignore_in_once(int return_value)
{
    struct nala_instance_io_vcontrol_t *instance_p;

    nala_mock_io_vcontrol.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_kind_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_io_vcontrol.instances,
                          instance_p);

    return (instance_p->handle);
}

void io_vcontrol_mock_set_errno(int errno_value)
{
    nala_get_data_io_vcontrol()->errno_value = errno_value;
}

void io_vcontrol_mock_set_callback(void (*callback)(int kind, va_list ap))
{
    nala_get_data_io_vcontrol()->callback = callback;
}

struct nala_io_vcontrol_params_t *io_vcontrol_mock_get_params_in(int handle)
{
    struct nala_instance_io_vcontrol_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_io_vcontrol.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "io_vcontrol() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void io_vcontrol_mock_ignore_kind_in(void)
{
    nala_get_params_io_vcontrol()->ignore_kind_in = true;
}

void io_vcontrol_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_io_vcontrol);
    nala_mock_io_vcontrol.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_io_vcontrol.data.traceback);
}

void io_vcontrol_mock_implementation(int (*implementation)(int kind, va_list ap))
{
    CHECK_NO_INSTANCES(nala_mock_io_vcontrol);
    nala_mock_io_vcontrol.state.mode = MODE_IMPLEMENTATION;
    nala_mock_io_vcontrol.data.implementation = implementation;
    nala_traceback(&nala_mock_io_vcontrol.data.traceback);
}

void io_vcontrol_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_io_vcontrol);
    nala_mock_io_vcontrol.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_io_vcontrol.data.traceback);
}

void io_vcontrol_mock_real_once(void)
{
    struct nala_instance_io_vcontrol_t *instance_p;

    nala_mock_io_vcontrol.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_io_vcontrol.instances,
                          instance_p);
}

void io_vcontrol_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_io_vcontrol.state);
}

void io_vcontrol_mock_resume(void)
{
    nala_state_resume(&nala_mock_io_vcontrol.state);
}

void io_vcontrol_mock_reset(void)
{
    struct nala_instance_io_vcontrol_t *current_p;
    struct nala_instance_io_vcontrol_t *tmp_p;

    NALA_STATE_RESET(nala_mock_io_vcontrol, current_p, tmp_p);
}

void io_vcontrol_mock_assert_completed(void)
{
    if (nala_mock_io_vcontrol.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked io_vcontrol() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_io_vcontrol.instances.length),
                &nala_mock_io_vcontrol.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION malloc

void *__real_malloc(size_t size);

struct nala_params_malloc_t {
    size_t size;
    bool ignore_size_in;
};

struct nala_data_malloc_t {
    struct nala_params_malloc_t params;
    void *return_value;
    int errno_value;
    void *(*implementation)(size_t size);
    void (*callback)(size_t size);
    struct nala_traceback_t traceback;
    struct nala_malloc_params_t params_in;
};

struct nala_instance_malloc_t {
    int mode;
    int handle;
    struct nala_data_malloc_t data;
    struct nala_instance_malloc_t *next_p;
};

struct nala_instances_malloc_t {
    struct nala_instance_malloc_t *head_p;
    struct nala_instance_malloc_t *next_p;
    struct nala_instance_malloc_t *tail_p;
    int length;
};

struct nala_mock_malloc_t {
    struct nala_state_t state;
    struct nala_data_malloc_t data;
    struct nala_instances_malloc_t instances;
};

static struct nala_mock_malloc_t nala_mock_malloc = {
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

struct nala_data_malloc_t *nala_get_data_malloc()
{
    if (nala_mock_malloc.instances.tail_p != NULL) {
        return (&nala_mock_malloc.instances.tail_p->data);
    } else {
        return (&nala_mock_malloc.data);
    }
}

struct nala_params_malloc_t *nala_get_params_malloc()
{
    return (&nala_get_data_malloc()->params);
}

void *__wrap_malloc(size_t size)
{
    struct nala_instance_malloc_t *nala_instance_p;
    struct nala_data_malloc_t *nala_data_p;
    void *return_value;

    nala_print_call("malloc", &nala_mock_malloc.state);

    switch (nala_mock_malloc.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_malloc.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_malloc.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked malloc() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_malloc.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, malloc, size);
            nala_data_p->params_in.size = size;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(size);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_malloc(size);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_malloc.data.implementation(size);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_malloc.data.traceback,
                            "malloc");
        exit(1);
        break;

    default:
        return_value =
        __real_malloc(size);
        break;
    }

    return return_value;
}

void malloc_mock(size_t size, void *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_malloc);
    nala_mock_malloc.state.mode = MODE_MOCK;
    nala_mock_malloc.data.params.size = size;
    nala_mock_malloc.data.params.ignore_size_in = false;
    nala_mock_malloc.data.return_value = return_value;
    nala_mock_malloc.data.errno_value = 0;
    nala_mock_malloc.data.callback = NULL;
    nala_traceback(&nala_mock_malloc.data.traceback);
}

int malloc_mock_once(size_t size, void *return_value)
{
    struct nala_instance_malloc_t *nala_instance_p;

    nala_mock_malloc.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.size = size;
    nala_instance_p->data.params.ignore_size_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_malloc.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void malloc_mock_ignore_in(void *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_malloc);
    nala_mock_malloc.state.mode = MODE_MOCK;
    nala_mock_malloc.data.params.ignore_size_in = true;
    nala_mock_malloc.data.return_value = return_value;
    nala_mock_malloc.data.errno_value = 0;
    nala_mock_malloc.data.callback = NULL;
}

int malloc_mock_ignore_in_once(void *return_value)
{
    struct nala_instance_malloc_t *instance_p;

    nala_mock_malloc.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_size_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_malloc.instances,
                          instance_p);

    return (instance_p->handle);
}

void malloc_mock_set_errno(int errno_value)
{
    nala_get_data_malloc()->errno_value = errno_value;
}

void malloc_mock_set_callback(void (*callback)(size_t size))
{
    nala_get_data_malloc()->callback = callback;
}

struct nala_malloc_params_t *malloc_mock_get_params_in(int handle)
{
    struct nala_instance_malloc_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_malloc.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "malloc() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void malloc_mock_ignore_size_in(void)
{
    nala_get_params_malloc()->ignore_size_in = true;
}

void malloc_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_malloc);
    nala_mock_malloc.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_malloc.data.traceback);
}

void malloc_mock_implementation(void *(*implementation)(size_t size))
{
    CHECK_NO_INSTANCES(nala_mock_malloc);
    nala_mock_malloc.state.mode = MODE_IMPLEMENTATION;
    nala_mock_malloc.data.implementation = implementation;
    nala_traceback(&nala_mock_malloc.data.traceback);
}

void malloc_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_malloc);
    nala_mock_malloc.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_malloc.data.traceback);
}

void malloc_mock_real_once(void)
{
    struct nala_instance_malloc_t *instance_p;

    nala_mock_malloc.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_malloc.instances,
                          instance_p);
}

void malloc_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_malloc.state);
}

void malloc_mock_resume(void)
{
    nala_state_resume(&nala_mock_malloc.state);
}

void malloc_mock_reset(void)
{
    struct nala_instance_malloc_t *current_p;
    struct nala_instance_malloc_t *tmp_p;

    NALA_STATE_RESET(nala_mock_malloc, current_p, tmp_p);
}

void malloc_mock_assert_completed(void)
{
    if (nala_mock_malloc.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked malloc() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_malloc.instances.length),
                &nala_mock_malloc.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION mount

int __real_mount(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);

struct nala_params_mount_t {
    const char *source;
    const char *target;
    const char *filesystemtype;
    unsigned long int mountflags;
    const void *data;
    bool ignore_source_in;
    struct nala_set_param source_in;
    void (*source_in_assert)(const char *source, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param source_out;
    void (*source_out_copy)(const char *source, const void *nala_buf_p, size_t nala_size);
    bool ignore_target_in;
    struct nala_set_param target_in;
    void (*target_in_assert)(const char *target, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param target_out;
    void (*target_out_copy)(const char *target, const void *nala_buf_p, size_t nala_size);
    bool ignore_filesystemtype_in;
    struct nala_set_param filesystemtype_in;
    void (*filesystemtype_in_assert)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param filesystemtype_out;
    void (*filesystemtype_out_copy)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size);
    bool ignore_mountflags_in;
    bool ignore_data_in;
    struct nala_set_param data_in;
    void (*data_in_assert)(const void *data, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param data_out;
    void (*data_out_copy)(const void *data, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_mount_t {
    struct nala_params_mount_t params;
    int return_value;
    int errno_value;
    int (*implementation)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);
    void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);
    struct nala_traceback_t traceback;
    struct nala_mount_params_t params_in;
};

struct nala_instance_mount_t {
    int mode;
    int handle;
    struct nala_data_mount_t data;
    struct nala_instance_mount_t *next_p;
};

struct nala_instances_mount_t {
    struct nala_instance_mount_t *head_p;
    struct nala_instance_mount_t *next_p;
    struct nala_instance_mount_t *tail_p;
    int length;
};

struct nala_mock_mount_t {
    struct nala_state_t state;
    struct nala_data_mount_t data;
    struct nala_instances_mount_t instances;
};

static struct nala_mock_mount_t nala_mock_mount = {
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

struct nala_data_mount_t *nala_get_data_mount()
{
    if (nala_mock_mount.instances.tail_p != NULL) {
        return (&nala_mock_mount.instances.tail_p->data);
    } else {
        return (&nala_mock_mount.data);
    }
}

struct nala_params_mount_t *nala_get_params_mount()
{
    return (&nala_get_data_mount()->params);
}

int __wrap_mount(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data)
{
    struct nala_instance_mount_t *nala_instance_p;
    struct nala_data_mount_t *nala_data_p;
    int return_value;

    nala_print_call("mount", &nala_mock_mount.state);

    switch (nala_mock_mount.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_mount.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_mount.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked mount() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_mount.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, mount, source);
            nala_data_p->params_in.source = source;
            MOCK_ASSERT_IN_EQ(nala_data_p, mount, target);
            nala_data_p->params_in.target = target;
            MOCK_ASSERT_IN_EQ(nala_data_p, mount, filesystemtype);
            nala_data_p->params_in.filesystemtype = filesystemtype;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, mount, data);
            nala_data_p->params_in.data = data;
            MOCK_ASSERT_IN_EQ(nala_data_p, mount, mountflags);
            nala_data_p->params_in.mountflags = mountflags;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       mount,
                                       source);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       mount,
                                       target);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       mount,
                                       filesystemtype);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       mount,
                                       data);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(source, target, filesystemtype, mountflags, data);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_mount(source, target, filesystemtype, mountflags, data);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_mount.data.implementation(source, target, filesystemtype, mountflags, data);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_mount.data.traceback,
                            "mount");
        exit(1);
        break;

    default:
        return_value =
        __real_mount(source, target, filesystemtype, mountflags, data);
        break;
    }

    return return_value;
}

void mount_mock(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_mount);
    nala_mock_mount.state.mode = MODE_MOCK;
    nala_mock_mount.data.params.source = NULL;
    nala_mock_mount.data.params.ignore_source_in = true;

    if (source != NULL) {
        nala_set_param_string(&nala_mock_mount.data.params.source_in,
                              source);
    } else {
        nala_mock_mount.data.params.ignore_source_in = false;
    }

    nala_mock_mount.data.params.target = NULL;
    nala_mock_mount.data.params.ignore_target_in = true;

    if (target != NULL) {
        nala_set_param_string(&nala_mock_mount.data.params.target_in,
                              target);
    } else {
        nala_mock_mount.data.params.ignore_target_in = false;
    }

    nala_mock_mount.data.params.filesystemtype = NULL;
    nala_mock_mount.data.params.ignore_filesystemtype_in = true;

    if (filesystemtype != NULL) {
        nala_set_param_string(&nala_mock_mount.data.params.filesystemtype_in,
                              filesystemtype);
    } else {
        nala_mock_mount.data.params.ignore_filesystemtype_in = false;
    }

    nala_mock_mount.data.params.data = NULL;
    nala_mock_mount.data.params.ignore_data_in = true;
    nala_mock_mount.data.params.mountflags = mountflags;
    nala_mock_mount.data.params.ignore_mountflags_in = false;
    nala_mock_mount.data.return_value = return_value;
    nala_mock_mount.data.errno_value = 0;
    nala_mock_mount.data.callback = NULL;
    nala_traceback(&nala_mock_mount.data.traceback);
}

int mount_mock_once(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value)
{
    struct nala_instance_mount_t *nala_instance_p;

    nala_mock_mount.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.source_out);
    nala_set_param_init(&nala_instance_p->data.params.source_in);
    nala_instance_p->data.params.source_in_assert = NULL;
    nala_instance_p->data.params.source_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.target_out);
    nala_set_param_init(&nala_instance_p->data.params.target_in);
    nala_instance_p->data.params.target_in_assert = NULL;
    nala_instance_p->data.params.target_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.filesystemtype_out);
    nala_set_param_init(&nala_instance_p->data.params.filesystemtype_in);
    nala_instance_p->data.params.filesystemtype_in_assert = NULL;
    nala_instance_p->data.params.filesystemtype_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.data_out);
    nala_set_param_init(&nala_instance_p->data.params.data_in);
    nala_instance_p->data.params.data_in_assert = NULL;
    nala_instance_p->data.params.data_out_copy = NULL;
    nala_instance_p->data.params.source = NULL;
    nala_instance_p->data.params.ignore_source_in = true;

    if (source != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.source_in,
                              source);
    } else {
        nala_instance_p->data.params.ignore_source_in = false;
    }

    nala_instance_p->data.params.target = NULL;
    nala_instance_p->data.params.ignore_target_in = true;

    if (target != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.target_in,
                              target);
    } else {
        nala_instance_p->data.params.ignore_target_in = false;
    }

    nala_instance_p->data.params.filesystemtype = NULL;
    nala_instance_p->data.params.ignore_filesystemtype_in = true;

    if (filesystemtype != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.filesystemtype_in,
                              filesystemtype);
    } else {
        nala_instance_p->data.params.ignore_filesystemtype_in = false;
    }

    nala_instance_p->data.params.data = NULL;
    nala_instance_p->data.params.ignore_data_in = true;
    nala_instance_p->data.params.mountflags = mountflags;
    nala_instance_p->data.params.ignore_mountflags_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_mount.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void mount_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_mount);
    nala_mock_mount.state.mode = MODE_MOCK;
    nala_mock_mount.data.params.ignore_source_in = true;
    nala_mock_mount.data.params.ignore_target_in = true;
    nala_mock_mount.data.params.ignore_filesystemtype_in = true;
    nala_mock_mount.data.params.ignore_data_in = true;
    nala_mock_mount.data.params.ignore_mountflags_in = true;
    nala_mock_mount.data.return_value = return_value;
    nala_mock_mount.data.errno_value = 0;
    nala_mock_mount.data.callback = NULL;
}

int mount_mock_ignore_in_once(int return_value)
{
    struct nala_instance_mount_t *instance_p;

    nala_mock_mount.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.source_out);
    nala_set_param_init(&instance_p->data.params.source_in);
    instance_p->data.params.source_in_assert = NULL;
    instance_p->data.params.source_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.target_out);
    nala_set_param_init(&instance_p->data.params.target_in);
    instance_p->data.params.target_in_assert = NULL;
    instance_p->data.params.target_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.filesystemtype_out);
    nala_set_param_init(&instance_p->data.params.filesystemtype_in);
    instance_p->data.params.filesystemtype_in_assert = NULL;
    instance_p->data.params.filesystemtype_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.data_out);
    nala_set_param_init(&instance_p->data.params.data_in);
    instance_p->data.params.data_in_assert = NULL;
    instance_p->data.params.data_out_copy = NULL;
    instance_p->data.params.source = NULL;
    instance_p->data.params.ignore_source_in = true;
    instance_p->data.params.target = NULL;
    instance_p->data.params.ignore_target_in = true;
    instance_p->data.params.filesystemtype = NULL;
    instance_p->data.params.ignore_filesystemtype_in = true;
    instance_p->data.params.data = NULL;
    instance_p->data.params.ignore_data_in = true;
    instance_p->data.params.ignore_mountflags_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_mount.instances,
                          instance_p);

    return (instance_p->handle);
}

void mount_mock_set_errno(int errno_value)
{
    nala_get_data_mount()->errno_value = errno_value;
}

void mount_mock_set_callback(void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data))
{
    nala_get_data_mount()->callback = callback;
}

struct nala_mount_params_t *mount_mock_get_params_in(int handle)
{
    struct nala_instance_mount_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_mount.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void mount_mock_ignore_source_in(void)
{
    nala_get_params_mount()->ignore_source_in = true;
}

void mount_mock_ignore_target_in(void)
{
    nala_get_params_mount()->ignore_target_in = true;
}

void mount_mock_ignore_filesystemtype_in(void)
{
    nala_get_params_mount()->ignore_filesystemtype_in = true;
}

void mount_mock_ignore_mountflags_in(void)
{
    nala_get_params_mount()->ignore_mountflags_in = true;
}

void mount_mock_set_source_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->source_in,
                       buf_p,
                       size);
}

void mount_mock_set_source_in_assert(void (*callback)(const char *source, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->source_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_source_in() must be called "
                "before mount_mock_set_source_in_assert().\n"));
    }

    nala_params_p->source_in_assert = callback;
}

void mount_mock_set_source_in_pointer(const char *source)
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();
    nala_params_p->ignore_source_in = false;
    nala_params_p->source = source;
}

void mount_mock_set_source_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->source_out,
                       buf_p,
                       size);
}

void mount_mock_set_source_out_copy(void (*callback)(const char *source, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->source_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_source_out() must be called "
                "before mount_mock_set_source_out_copy().\n"));
    }

    nala_params_p->source_out_copy = callback;
}

void mount_mock_set_target_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->target_in,
                       buf_p,
                       size);
}

void mount_mock_set_target_in_assert(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->target_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_target_in() must be called "
                "before mount_mock_set_target_in_assert().\n"));
    }

    nala_params_p->target_in_assert = callback;
}

void mount_mock_set_target_in_pointer(const char *target)
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();
    nala_params_p->ignore_target_in = false;
    nala_params_p->target = target;
}

void mount_mock_set_target_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->target_out,
                       buf_p,
                       size);
}

void mount_mock_set_target_out_copy(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->target_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_target_out() must be called "
                "before mount_mock_set_target_out_copy().\n"));
    }

    nala_params_p->target_out_copy = callback;
}

void mount_mock_set_filesystemtype_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->filesystemtype_in,
                       buf_p,
                       size);
}

void mount_mock_set_filesystemtype_in_assert(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->filesystemtype_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_filesystemtype_in() must be called "
                "before mount_mock_set_filesystemtype_in_assert().\n"));
    }

    nala_params_p->filesystemtype_in_assert = callback;
}

void mount_mock_set_filesystemtype_in_pointer(const char *filesystemtype)
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();
    nala_params_p->ignore_filesystemtype_in = false;
    nala_params_p->filesystemtype = filesystemtype;
}

void mount_mock_set_filesystemtype_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->filesystemtype_out,
                       buf_p,
                       size);
}

void mount_mock_set_filesystemtype_out_copy(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->filesystemtype_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_filesystemtype_out() must be called "
                "before mount_mock_set_filesystemtype_out_copy().\n"));
    }

    nala_params_p->filesystemtype_out_copy = callback;
}

void mount_mock_set_data_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->data_in,
                       buf_p,
                       size);
}

void mount_mock_set_data_in_assert(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->data_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_data_in() must be called "
                "before mount_mock_set_data_in_assert().\n"));
    }

    nala_params_p->data_in_assert = callback;
}

void mount_mock_set_data_in_pointer(const void *data)
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();
    nala_params_p->ignore_data_in = false;
    nala_params_p->data = data;
}

void mount_mock_set_data_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->data_out,
                       buf_p,
                       size);
}

void mount_mock_set_data_out_copy(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_mount_t *nala_params_p;

    nala_params_p = nala_get_params_mount();

    if (nala_params_p->data_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "mount_mock_set_data_out() must be called "
                "before mount_mock_set_data_out_copy().\n"));
    }

    nala_params_p->data_out_copy = callback;
}

void mount_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_mount);
    nala_mock_mount.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_mount.data.traceback);
}

void mount_mock_implementation(int (*implementation)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data))
{
    CHECK_NO_INSTANCES(nala_mock_mount);
    nala_mock_mount.state.mode = MODE_IMPLEMENTATION;
    nala_mock_mount.data.implementation = implementation;
    nala_traceback(&nala_mock_mount.data.traceback);
}

void mount_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_mount);
    nala_mock_mount.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_mount.data.traceback);
}

void mount_mock_real_once(void)
{
    struct nala_instance_mount_t *instance_p;

    nala_mock_mount.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_mount.instances,
                          instance_p);
}

void mount_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_mount.state);
}

void mount_mock_resume(void)
{
    nala_state_resume(&nala_mock_mount.state);
}

void mount_mock_reset(void)
{
    struct nala_instance_mount_t *current_p;
    struct nala_instance_mount_t *tmp_p;

    NALA_STATE_RESET(nala_mock_mount, current_p, tmp_p);
}

void mount_mock_assert_completed(void)
{
    if (nala_mock_mount.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked mount() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_mount.instances.length),
                &nala_mock_mount.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION output_message

void __real_output_message(const char *message);

struct nala_params_output_message_t {
    const char *message;
    bool ignore_message_in;
    struct nala_set_param message_in;
    void (*message_in_assert)(const char *message, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param message_out;
    void (*message_out_copy)(const char *message, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_output_message_t {
    struct nala_params_output_message_t params;
    int errno_value;
    void (*implementation)(const char *message);
    void (*callback)(const char *message);
    struct nala_traceback_t traceback;
    struct nala_output_message_params_t params_in;
};

struct nala_instance_output_message_t {
    int mode;
    int handle;
    struct nala_data_output_message_t data;
    struct nala_instance_output_message_t *next_p;
};

struct nala_instances_output_message_t {
    struct nala_instance_output_message_t *head_p;
    struct nala_instance_output_message_t *next_p;
    struct nala_instance_output_message_t *tail_p;
    int length;
};

struct nala_mock_output_message_t {
    struct nala_state_t state;
    struct nala_data_output_message_t data;
    struct nala_instances_output_message_t instances;
};

static struct nala_mock_output_message_t nala_mock_output_message = {
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

struct nala_data_output_message_t *nala_get_data_output_message()
{
    if (nala_mock_output_message.instances.tail_p != NULL) {
        return (&nala_mock_output_message.instances.tail_p->data);
    } else {
        return (&nala_mock_output_message.data);
    }
}

struct nala_params_output_message_t *nala_get_params_output_message()
{
    return (&nala_get_data_output_message()->params);
}

void __wrap_output_message(const char *message)
{
    struct nala_instance_output_message_t *nala_instance_p;
    struct nala_data_output_message_t *nala_data_p;

    nala_print_call("output_message", &nala_mock_output_message.state);

    switch (nala_mock_output_message.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_output_message.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_output_message.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked output_message() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_output_message.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, output_message, message);
            nala_data_p->params_in.message = message;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       output_message,
                                       message);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(message);
            }

        } else {
            __real_output_message(message);
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_output_message.data.implementation(message);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_output_message.data.traceback,
                            "output_message");
        exit(1);
        break;

    default:
        __real_output_message(message);
        break;
    }

    return;
}

void output_message_mock(const char *message)
{
    CHECK_NO_INSTANCES(nala_mock_output_message);
    nala_mock_output_message.state.mode = MODE_MOCK;
    nala_mock_output_message.data.params.message = NULL;
    nala_mock_output_message.data.params.ignore_message_in = true;

    if (message != NULL) {
        nala_set_param_string(&nala_mock_output_message.data.params.message_in,
                              message);
    } else {
        nala_mock_output_message.data.params.ignore_message_in = false;
    }

    nala_mock_output_message.data.errno_value = 0;
    nala_mock_output_message.data.callback = NULL;
    nala_traceback(&nala_mock_output_message.data.traceback);
}

int output_message_mock_once(const char *message)
{
    struct nala_instance_output_message_t *nala_instance_p;

    nala_mock_output_message.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.message_out);
    nala_set_param_init(&nala_instance_p->data.params.message_in);
    nala_instance_p->data.params.message_in_assert = NULL;
    nala_instance_p->data.params.message_out_copy = NULL;
    nala_instance_p->data.params.message = NULL;
    nala_instance_p->data.params.ignore_message_in = true;

    if (message != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.message_in,
                              message);
    } else {
        nala_instance_p->data.params.ignore_message_in = false;
    }

    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_output_message.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void output_message_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_output_message);
    nala_mock_output_message.state.mode = MODE_MOCK;
    nala_mock_output_message.data.params.ignore_message_in = true;
    nala_mock_output_message.data.errno_value = 0;
    nala_mock_output_message.data.callback = NULL;
}

int output_message_mock_ignore_in_once(void)
{
    struct nala_instance_output_message_t *instance_p;

    nala_mock_output_message.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.message_out);
    nala_set_param_init(&instance_p->data.params.message_in);
    instance_p->data.params.message_in_assert = NULL;
    instance_p->data.params.message_out_copy = NULL;
    instance_p->data.params.message = NULL;
    instance_p->data.params.ignore_message_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_output_message.instances,
                          instance_p);

    return (instance_p->handle);
}

void output_message_mock_set_errno(int errno_value)
{
    nala_get_data_output_message()->errno_value = errno_value;
}

void output_message_mock_set_callback(void (*callback)(const char *message))
{
    nala_get_data_output_message()->callback = callback;
}

struct nala_output_message_params_t *output_message_mock_get_params_in(int handle)
{
    struct nala_instance_output_message_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_output_message.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "output_message() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void output_message_mock_ignore_message_in(void)
{
    nala_get_params_output_message()->ignore_message_in = true;
}

void output_message_mock_set_message_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_output_message()->message_in,
                       buf_p,
                       size);
}

void output_message_mock_set_message_in_assert(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_output_message_t *nala_params_p;

    nala_params_p = nala_get_params_output_message();

    if (nala_params_p->message_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "output_message_mock_set_message_in() must be called "
                "before output_message_mock_set_message_in_assert().\n"));
    }

    nala_params_p->message_in_assert = callback;
}

void output_message_mock_set_message_in_pointer(const char *message)
{
    struct nala_params_output_message_t *nala_params_p;

    nala_params_p = nala_get_params_output_message();
    nala_params_p->ignore_message_in = false;
    nala_params_p->message = message;
}

void output_message_mock_set_message_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_output_message()->message_out,
                       buf_p,
                       size);
}

void output_message_mock_set_message_out_copy(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_output_message_t *nala_params_p;

    nala_params_p = nala_get_params_output_message();

    if (nala_params_p->message_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "output_message_mock_set_message_out() must be called "
                "before output_message_mock_set_message_out_copy().\n"));
    }

    nala_params_p->message_out_copy = callback;
}

void output_message_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_output_message);
    nala_mock_output_message.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_output_message.data.traceback);
}

void output_message_mock_implementation(void (*implementation)(const char *message))
{
    CHECK_NO_INSTANCES(nala_mock_output_message);
    nala_mock_output_message.state.mode = MODE_IMPLEMENTATION;
    nala_mock_output_message.data.implementation = implementation;
    nala_traceback(&nala_mock_output_message.data.traceback);
}

void output_message_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_output_message);
    nala_mock_output_message.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_output_message.data.traceback);
}

void output_message_mock_real_once(void)
{
    struct nala_instance_output_message_t *instance_p;

    nala_mock_output_message.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_output_message.instances,
                          instance_p);
}

void output_message_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_output_message.state);
}

void output_message_mock_resume(void)
{
    nala_state_resume(&nala_mock_output_message.state);
}

void output_message_mock_reset(void)
{
    struct nala_instance_output_message_t *current_p;
    struct nala_instance_output_message_t *tmp_p;

    NALA_STATE_RESET(nala_mock_output_message, current_p, tmp_p);
}

void output_message_mock_assert_completed(void)
{
    if (nala_mock_output_message.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked output_message() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_output_message.instances.length),
                &nala_mock_output_message.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION pipe

int __real_pipe(int pipefd[2]);

struct nala_params_pipe_t {
    int *pipefd;
    bool ignore_pipefd_in;
    struct nala_set_param pipefd_in;
    void (*pipefd_in_assert)(int pipefd[2], const void *nala_buf_p, size_t nala_size);
    struct nala_set_param pipefd_out;
    void (*pipefd_out_copy)(int pipefd[2], const void *nala_buf_p, size_t nala_size);
};

struct nala_data_pipe_t {
    struct nala_params_pipe_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int pipefd[2]);
    void (*callback)(int pipefd[2]);
    struct nala_traceback_t traceback;
    struct nala_pipe_params_t params_in;
};

struct nala_instance_pipe_t {
    int mode;
    int handle;
    struct nala_data_pipe_t data;
    struct nala_instance_pipe_t *next_p;
};

struct nala_instances_pipe_t {
    struct nala_instance_pipe_t *head_p;
    struct nala_instance_pipe_t *next_p;
    struct nala_instance_pipe_t *tail_p;
    int length;
};

struct nala_mock_pipe_t {
    struct nala_state_t state;
    struct nala_data_pipe_t data;
    struct nala_instances_pipe_t instances;
};

static struct nala_mock_pipe_t nala_mock_pipe = {
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

struct nala_data_pipe_t *nala_get_data_pipe()
{
    if (nala_mock_pipe.instances.tail_p != NULL) {
        return (&nala_mock_pipe.instances.tail_p->data);
    } else {
        return (&nala_mock_pipe.data);
    }
}

struct nala_params_pipe_t *nala_get_params_pipe()
{
    return (&nala_get_data_pipe()->params);
}

int __wrap_pipe(int pipefd[2])
{
    struct nala_instance_pipe_t *nala_instance_p;
    struct nala_data_pipe_t *nala_data_p;
    int return_value;

    nala_print_call("pipe", &nala_mock_pipe.state);

    switch (nala_mock_pipe.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_pipe.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_pipe.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked pipe() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_pipe.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, pipe, pipefd);
            nala_data_p->params_in.pipefd = pipefd;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       pipe,
                                       pipefd);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(pipefd);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_pipe(pipefd);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_pipe.data.implementation(pipefd);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_pipe.data.traceback,
                            "pipe");
        exit(1);
        break;

    default:
        return_value =
        __real_pipe(pipefd);
        break;
    }

    return return_value;
}

void pipe_mock(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_pipe);
    nala_mock_pipe.state.mode = MODE_MOCK;
    nala_mock_pipe.data.params.pipefd = NULL;
    nala_mock_pipe.data.params.ignore_pipefd_in = true;
    nala_mock_pipe.data.return_value = return_value;
    nala_mock_pipe.data.errno_value = 0;
    nala_mock_pipe.data.callback = NULL;
    nala_traceback(&nala_mock_pipe.data.traceback);
}

int pipe_mock_once(int return_value)
{
    struct nala_instance_pipe_t *nala_instance_p;

    nala_mock_pipe.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.pipefd_out);
    nala_set_param_init(&nala_instance_p->data.params.pipefd_in);
    nala_instance_p->data.params.pipefd_in_assert = NULL;
    nala_instance_p->data.params.pipefd_out_copy = NULL;
    nala_instance_p->data.params.pipefd = NULL;
    nala_instance_p->data.params.ignore_pipefd_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_pipe.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void pipe_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_pipe);
    nala_mock_pipe.state.mode = MODE_MOCK;
    nala_mock_pipe.data.params.ignore_pipefd_in = true;
    nala_mock_pipe.data.return_value = return_value;
    nala_mock_pipe.data.errno_value = 0;
    nala_mock_pipe.data.callback = NULL;
}

int pipe_mock_ignore_in_once(int return_value)
{
    struct nala_instance_pipe_t *instance_p;

    nala_mock_pipe.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.pipefd_out);
    nala_set_param_init(&instance_p->data.params.pipefd_in);
    instance_p->data.params.pipefd_in_assert = NULL;
    instance_p->data.params.pipefd_out_copy = NULL;
    instance_p->data.params.pipefd = NULL;
    instance_p->data.params.ignore_pipefd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_pipe.instances,
                          instance_p);

    return (instance_p->handle);
}

void pipe_mock_set_errno(int errno_value)
{
    nala_get_data_pipe()->errno_value = errno_value;
}

void pipe_mock_set_callback(void (*callback)(int pipefd[2]))
{
    nala_get_data_pipe()->callback = callback;
}

struct nala_pipe_params_t *pipe_mock_get_params_in(int handle)
{
    struct nala_instance_pipe_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_pipe.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "pipe() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void pipe_mock_set_pipefd_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_pipe()->pipefd_in,
                       buf_p,
                       size);
}

void pipe_mock_set_pipefd_in_assert(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_pipe_t *nala_params_p;

    nala_params_p = nala_get_params_pipe();

    if (nala_params_p->pipefd_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "pipe_mock_set_pipefd_in() must be called "
                "before pipe_mock_set_pipefd_in_assert().\n"));
    }

    nala_params_p->pipefd_in_assert = callback;
}

void pipe_mock_set_pipefd_in_pointer(int pipefd[2])
{
    struct nala_params_pipe_t *nala_params_p;

    nala_params_p = nala_get_params_pipe();
    nala_params_p->ignore_pipefd_in = false;
    nala_params_p->pipefd = pipefd;
}

void pipe_mock_set_pipefd_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_pipe()->pipefd_out,
                       buf_p,
                       size);
}

void pipe_mock_set_pipefd_out_copy(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_pipe_t *nala_params_p;

    nala_params_p = nala_get_params_pipe();

    if (nala_params_p->pipefd_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "pipe_mock_set_pipefd_out() must be called "
                "before pipe_mock_set_pipefd_out_copy().\n"));
    }

    nala_params_p->pipefd_out_copy = callback;
}

void pipe_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_pipe);
    nala_mock_pipe.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_pipe.data.traceback);
}

void pipe_mock_implementation(int (*implementation)(int pipefd[2]))
{
    CHECK_NO_INSTANCES(nala_mock_pipe);
    nala_mock_pipe.state.mode = MODE_IMPLEMENTATION;
    nala_mock_pipe.data.implementation = implementation;
    nala_traceback(&nala_mock_pipe.data.traceback);
}

void pipe_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_pipe);
    nala_mock_pipe.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_pipe.data.traceback);
}

void pipe_mock_real_once(void)
{
    struct nala_instance_pipe_t *instance_p;

    nala_mock_pipe.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_pipe.instances,
                          instance_p);
}

void pipe_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_pipe.state);
}

void pipe_mock_resume(void)
{
    nala_state_resume(&nala_mock_pipe.state);
}

void pipe_mock_reset(void)
{
    struct nala_instance_pipe_t *current_p;
    struct nala_instance_pipe_t *tmp_p;

    NALA_STATE_RESET(nala_mock_pipe, current_p, tmp_p);
}

void pipe_mock_assert_completed(void)
{
    if (nala_mock_pipe.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked pipe() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_pipe.instances.length),
                &nala_mock_pipe.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION poll

int __real_poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct nala_params_poll_t {
    struct pollfd *fds;
    nfds_t nfds;
    int timeout;
    bool ignore_fds_in;
    struct nala_set_param fds_in;
    void (*fds_in_assert)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param fds_out;
    void (*fds_out_copy)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size);
    bool ignore_nfds_in;
    bool ignore_timeout_in;
};

struct nala_data_poll_t {
    struct nala_params_poll_t params;
    int return_value;
    int errno_value;
    int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout);
    void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout);
    struct nala_traceback_t traceback;
    struct nala_poll_params_t params_in;
};

struct nala_instance_poll_t {
    int mode;
    int handle;
    struct nala_data_poll_t data;
    struct nala_instance_poll_t *next_p;
};

struct nala_instances_poll_t {
    struct nala_instance_poll_t *head_p;
    struct nala_instance_poll_t *next_p;
    struct nala_instance_poll_t *tail_p;
    int length;
};

struct nala_mock_poll_t {
    struct nala_state_t state;
    struct nala_data_poll_t data;
    struct nala_instances_poll_t instances;
};

static struct nala_mock_poll_t nala_mock_poll = {
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

struct nala_data_poll_t *nala_get_data_poll()
{
    if (nala_mock_poll.instances.tail_p != NULL) {
        return (&nala_mock_poll.instances.tail_p->data);
    } else {
        return (&nala_mock_poll.data);
    }
}

struct nala_params_poll_t *nala_get_params_poll()
{
    return (&nala_get_data_poll()->params);
}

int __wrap_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    struct nala_instance_poll_t *nala_instance_p;
    struct nala_data_poll_t *nala_data_p;
    int return_value;

    nala_print_call("poll", &nala_mock_poll.state);

    switch (nala_mock_poll.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_poll.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_poll.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked poll() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_poll.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, poll, fds);
            nala_data_p->params_in.fds = fds;
            MOCK_ASSERT_IN_EQ(nala_data_p, poll, nfds);
            nala_data_p->params_in.nfds = nfds;
            MOCK_ASSERT_IN_EQ(nala_data_p, poll, timeout);
            nala_data_p->params_in.timeout = timeout;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_pollfd,
                                       poll,
                                       fds);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(fds, nfds, timeout);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_poll(fds, nfds, timeout);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_poll.data.implementation(fds, nfds, timeout);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_poll.data.traceback,
                            "poll");
        exit(1);
        break;

    default:
        return_value =
        __real_poll(fds, nfds, timeout);
        break;
    }

    return return_value;
}

void poll_mock(nfds_t nfds, int timeout, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_poll);
    nala_mock_poll.state.mode = MODE_MOCK;
    nala_mock_poll.data.params.fds = NULL;
    nala_mock_poll.data.params.ignore_fds_in = true;
    nala_mock_poll.data.params.nfds = nfds;
    nala_mock_poll.data.params.ignore_nfds_in = false;
    nala_mock_poll.data.params.timeout = timeout;
    nala_mock_poll.data.params.ignore_timeout_in = false;
    nala_mock_poll.data.return_value = return_value;
    nala_mock_poll.data.errno_value = 0;
    nala_mock_poll.data.callback = NULL;
    nala_traceback(&nala_mock_poll.data.traceback);
}

int poll_mock_once(nfds_t nfds, int timeout, int return_value)
{
    struct nala_instance_poll_t *nala_instance_p;

    nala_mock_poll.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.fds_out);
    nala_set_param_init(&nala_instance_p->data.params.fds_in);
    nala_instance_p->data.params.fds_in_assert = NULL;
    nala_instance_p->data.params.fds_out_copy = NULL;
    nala_instance_p->data.params.fds = NULL;
    nala_instance_p->data.params.ignore_fds_in = true;
    nala_instance_p->data.params.nfds = nfds;
    nala_instance_p->data.params.ignore_nfds_in = false;
    nala_instance_p->data.params.timeout = timeout;
    nala_instance_p->data.params.ignore_timeout_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_poll.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void poll_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_poll);
    nala_mock_poll.state.mode = MODE_MOCK;
    nala_mock_poll.data.params.ignore_fds_in = true;
    nala_mock_poll.data.params.ignore_nfds_in = true;
    nala_mock_poll.data.params.ignore_timeout_in = true;
    nala_mock_poll.data.return_value = return_value;
    nala_mock_poll.data.errno_value = 0;
    nala_mock_poll.data.callback = NULL;
}

int poll_mock_ignore_in_once(int return_value)
{
    struct nala_instance_poll_t *instance_p;

    nala_mock_poll.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.fds_out);
    nala_set_param_init(&instance_p->data.params.fds_in);
    instance_p->data.params.fds_in_assert = NULL;
    instance_p->data.params.fds_out_copy = NULL;
    instance_p->data.params.fds = NULL;
    instance_p->data.params.ignore_fds_in = true;
    instance_p->data.params.ignore_nfds_in = true;
    instance_p->data.params.ignore_timeout_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_poll.instances,
                          instance_p);

    return (instance_p->handle);
}

void poll_mock_set_errno(int errno_value)
{
    nala_get_data_poll()->errno_value = errno_value;
}

void poll_mock_set_callback(void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout))
{
    nala_get_data_poll()->callback = callback;
}

struct nala_poll_params_t *poll_mock_get_params_in(int handle)
{
    struct nala_instance_poll_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_poll.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "poll() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void poll_mock_ignore_nfds_in(void)
{
    nala_get_params_poll()->ignore_nfds_in = true;
}

void poll_mock_ignore_timeout_in(void)
{
    nala_get_params_poll()->ignore_timeout_in = true;
}

void poll_mock_set_fds_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_poll()->fds_in,
                       buf_p,
                       size);
}

void poll_mock_set_fds_in_assert(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_poll_t *nala_params_p;

    nala_params_p = nala_get_params_poll();

    if (nala_params_p->fds_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "poll_mock_set_fds_in() must be called "
                "before poll_mock_set_fds_in_assert().\n"));
    }

    nala_params_p->fds_in_assert = callback;
}

void poll_mock_set_fds_in_pointer(struct pollfd *fds)
{
    struct nala_params_poll_t *nala_params_p;

    nala_params_p = nala_get_params_poll();
    nala_params_p->ignore_fds_in = false;
    nala_params_p->fds = fds;
}

void poll_mock_set_fds_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_poll()->fds_out,
                       buf_p,
                       size);
}

void poll_mock_set_fds_out_copy(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_poll_t *nala_params_p;

    nala_params_p = nala_get_params_poll();

    if (nala_params_p->fds_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "poll_mock_set_fds_out() must be called "
                "before poll_mock_set_fds_out_copy().\n"));
    }

    nala_params_p->fds_out_copy = callback;
}

void poll_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_poll);
    nala_mock_poll.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_poll.data.traceback);
}

void poll_mock_implementation(int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout))
{
    CHECK_NO_INSTANCES(nala_mock_poll);
    nala_mock_poll.state.mode = MODE_IMPLEMENTATION;
    nala_mock_poll.data.implementation = implementation;
    nala_traceback(&nala_mock_poll.data.traceback);
}

void poll_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_poll);
    nala_mock_poll.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_poll.data.traceback);
}

void poll_mock_real_once(void)
{
    struct nala_instance_poll_t *instance_p;

    nala_mock_poll.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_poll.instances,
                          instance_p);
}

void poll_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_poll.state);
}

void poll_mock_resume(void)
{
    nala_state_resume(&nala_mock_poll.state);
}

void poll_mock_reset(void)
{
    struct nala_instance_poll_t *current_p;
    struct nala_instance_poll_t *tmp_p;

    NALA_STATE_RESET(nala_mock_poll, current_p, tmp_p);
}

void poll_mock_assert_completed(void)
{
    if (nala_mock_poll.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked poll() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_poll.instances.length),
                &nala_mock_poll.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION print_hello

void __real_print_hello();

struct nala_params_print_hello_t {
    int dummy;
};

struct nala_data_print_hello_t {
    struct nala_params_print_hello_t params;
    int errno_value;
    void (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
    struct nala_print_hello_params_t params_in;
};

struct nala_instance_print_hello_t {
    int mode;
    int handle;
    struct nala_data_print_hello_t data;
    struct nala_instance_print_hello_t *next_p;
};

struct nala_instances_print_hello_t {
    struct nala_instance_print_hello_t *head_p;
    struct nala_instance_print_hello_t *next_p;
    struct nala_instance_print_hello_t *tail_p;
    int length;
};

struct nala_mock_print_hello_t {
    struct nala_state_t state;
    struct nala_data_print_hello_t data;
    struct nala_instances_print_hello_t instances;
};

static struct nala_mock_print_hello_t nala_mock_print_hello = {
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

struct nala_data_print_hello_t *nala_get_data_print_hello()
{
    if (nala_mock_print_hello.instances.tail_p != NULL) {
        return (&nala_mock_print_hello.instances.tail_p->data);
    } else {
        return (&nala_mock_print_hello.data);
    }
}

struct nala_params_print_hello_t *nala_get_params_print_hello()
{
    return (&nala_get_data_print_hello()->params);
}

void __wrap_print_hello()
{
    struct nala_instance_print_hello_t *nala_instance_p;
    struct nala_data_print_hello_t *nala_data_p;

    nala_print_call("print_hello", &nala_mock_print_hello.state);

    switch (nala_mock_print_hello.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_print_hello.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_print_hello.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked print_hello() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_print_hello.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

        } else {
            __real_print_hello();
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_print_hello.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_print_hello.data.traceback,
                            "print_hello");
        exit(1);
        break;

    default:
        __real_print_hello();
        break;
    }

    return;
}

void print_hello_mock()
{
    CHECK_NO_INSTANCES(nala_mock_print_hello);
    nala_mock_print_hello.state.mode = MODE_MOCK;
    nala_mock_print_hello.data.errno_value = 0;
    nala_mock_print_hello.data.callback = NULL;
    nala_traceback(&nala_mock_print_hello.data.traceback);
}

int print_hello_mock_once()
{
    struct nala_instance_print_hello_t *nala_instance_p;

    nala_mock_print_hello.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_print_hello.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void print_hello_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_print_hello);
    nala_mock_print_hello.state.mode = MODE_MOCK;
    nala_mock_print_hello.data.errno_value = 0;
    nala_mock_print_hello.data.callback = NULL;
}

int print_hello_mock_ignore_in_once(void)
{
    struct nala_instance_print_hello_t *instance_p;

    nala_mock_print_hello.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_print_hello.instances,
                          instance_p);

    return (instance_p->handle);
}

void print_hello_mock_set_errno(int errno_value)
{
    nala_get_data_print_hello()->errno_value = errno_value;
}

void print_hello_mock_set_callback(void (*callback)())
{
    nala_get_data_print_hello()->callback = callback;
}

struct nala_print_hello_params_t *print_hello_mock_get_params_in(int handle)
{
    struct nala_instance_print_hello_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_print_hello.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "print_hello() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void print_hello_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_print_hello);
    nala_mock_print_hello.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_print_hello.data.traceback);
}

void print_hello_mock_implementation(void (*implementation)())
{
    CHECK_NO_INSTANCES(nala_mock_print_hello);
    nala_mock_print_hello.state.mode = MODE_IMPLEMENTATION;
    nala_mock_print_hello.data.implementation = implementation;
    nala_traceback(&nala_mock_print_hello.data.traceback);
}

void print_hello_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_print_hello);
    nala_mock_print_hello.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_print_hello.data.traceback);
}

void print_hello_mock_real_once(void)
{
    struct nala_instance_print_hello_t *instance_p;

    nala_mock_print_hello.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_print_hello.instances,
                          instance_p);
}

void print_hello_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_print_hello.state);
}

void print_hello_mock_resume(void)
{
    nala_state_resume(&nala_mock_print_hello.state);
}

void print_hello_mock_reset(void)
{
    struct nala_instance_print_hello_t *current_p;
    struct nala_instance_print_hello_t *tmp_p;

    NALA_STATE_RESET(nala_mock_print_hello, current_p, tmp_p);
}

void print_hello_mock_assert_completed(void)
{
    if (nala_mock_print_hello.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked print_hello() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_print_hello.instances.length),
                &nala_mock_print_hello.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION read

ssize_t __real_read(int fd, void *buf, size_t count);

struct nala_params_read_t {
    int fd;
    void *buf;
    size_t count;
    bool ignore_fd_in;
    bool ignore_buf_in;
    struct nala_set_param buf_in;
    void (*buf_in_assert)(void *buf, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_out;
    void (*buf_out_copy)(void *buf, const void *nala_buf_p, size_t nala_size);
    bool ignore_count_in;
};

struct nala_data_read_t {
    struct nala_params_read_t params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int fd, void *buf, size_t count);
    void (*callback)(int fd, void *buf, size_t count);
    struct nala_traceback_t traceback;
    struct nala_read_params_t params_in;
};

struct nala_instance_read_t {
    int mode;
    int handle;
    struct nala_data_read_t data;
    struct nala_instance_read_t *next_p;
};

struct nala_instances_read_t {
    struct nala_instance_read_t *head_p;
    struct nala_instance_read_t *next_p;
    struct nala_instance_read_t *tail_p;
    int length;
};

struct nala_mock_read_t {
    struct nala_state_t state;
    struct nala_data_read_t data;
    struct nala_instances_read_t instances;
};

static struct nala_mock_read_t nala_mock_read = {
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

struct nala_data_read_t *nala_get_data_read()
{
    if (nala_mock_read.instances.tail_p != NULL) {
        return (&nala_mock_read.instances.tail_p->data);
    } else {
        return (&nala_mock_read.data);
    }
}

struct nala_params_read_t *nala_get_params_read()
{
    return (&nala_get_data_read()->params);
}

ssize_t __wrap_read(int fd, void *buf, size_t count)
{
    struct nala_instance_read_t *nala_instance_p;
    struct nala_data_read_t *nala_data_p;
    ssize_t return_value;

    nala_print_call("read", &nala_mock_read.state);

    switch (nala_mock_read.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_read.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_read.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked read() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_read.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, read, buf);
            nala_data_p->params_in.buf = buf;
            MOCK_ASSERT_IN_EQ(nala_data_p, read, fd);
            nala_data_p->params_in.fd = fd;
            MOCK_ASSERT_IN_EQ(nala_data_p, read, count);
            nala_data_p->params_in.count = count;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       read,
                                       buf);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(fd, buf, count);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_read(fd, buf, count);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_read.data.implementation(fd, buf, count);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_read.data.traceback,
                            "read");
        exit(1);
        break;

    default:
        return_value =
        __real_read(fd, buf, count);
        break;
    }

    return return_value;
}

void read_mock(int fd, size_t count, ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_read);
    nala_mock_read.state.mode = MODE_MOCK;
    nala_mock_read.data.params.buf = NULL;
    nala_mock_read.data.params.ignore_buf_in = true;
    nala_mock_read.data.params.fd = fd;
    nala_mock_read.data.params.ignore_fd_in = false;
    nala_mock_read.data.params.count = count;
    nala_mock_read.data.params.ignore_count_in = false;
    nala_mock_read.data.return_value = return_value;
    nala_mock_read.data.errno_value = 0;
    nala_mock_read.data.callback = NULL;
    nala_traceback(&nala_mock_read.data.traceback);
}

int read_mock_once(int fd, size_t count, ssize_t return_value)
{
    struct nala_instance_read_t *nala_instance_p;

    nala_mock_read.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.buf_out);
    nala_set_param_init(&nala_instance_p->data.params.buf_in);
    nala_instance_p->data.params.buf_in_assert = NULL;
    nala_instance_p->data.params.buf_out_copy = NULL;
    nala_instance_p->data.params.buf = NULL;
    nala_instance_p->data.params.ignore_buf_in = true;
    nala_instance_p->data.params.fd = fd;
    nala_instance_p->data.params.ignore_fd_in = false;
    nala_instance_p->data.params.count = count;
    nala_instance_p->data.params.ignore_count_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_read.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void read_mock_ignore_in(ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_read);
    nala_mock_read.state.mode = MODE_MOCK;
    nala_mock_read.data.params.ignore_buf_in = true;
    nala_mock_read.data.params.ignore_fd_in = true;
    nala_mock_read.data.params.ignore_count_in = true;
    nala_mock_read.data.return_value = return_value;
    nala_mock_read.data.errno_value = 0;
    nala_mock_read.data.callback = NULL;
}

int read_mock_ignore_in_once(ssize_t return_value)
{
    struct nala_instance_read_t *instance_p;

    nala_mock_read.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.buf_out);
    nala_set_param_init(&instance_p->data.params.buf_in);
    instance_p->data.params.buf_in_assert = NULL;
    instance_p->data.params.buf_out_copy = NULL;
    instance_p->data.params.buf = NULL;
    instance_p->data.params.ignore_buf_in = true;
    instance_p->data.params.ignore_fd_in = true;
    instance_p->data.params.ignore_count_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_read.instances,
                          instance_p);

    return (instance_p->handle);
}

void read_mock_set_errno(int errno_value)
{
    nala_get_data_read()->errno_value = errno_value;
}

void read_mock_set_callback(void (*callback)(int fd, void *buf, size_t count))
{
    nala_get_data_read()->callback = callback;
}

struct nala_read_params_t *read_mock_get_params_in(int handle)
{
    struct nala_instance_read_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_read.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "read() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void read_mock_ignore_fd_in(void)
{
    nala_get_params_read()->ignore_fd_in = true;
}

void read_mock_ignore_count_in(void)
{
    nala_get_params_read()->ignore_count_in = true;
}

void read_mock_set_buf_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_read()->buf_in,
                       buf_p,
                       size);
}

void read_mock_set_buf_in_assert(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_read_t *nala_params_p;

    nala_params_p = nala_get_params_read();

    if (nala_params_p->buf_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "read_mock_set_buf_in() must be called "
                "before read_mock_set_buf_in_assert().\n"));
    }

    nala_params_p->buf_in_assert = callback;
}

void read_mock_set_buf_in_pointer(void *buf)
{
    struct nala_params_read_t *nala_params_p;

    nala_params_p = nala_get_params_read();
    nala_params_p->ignore_buf_in = false;
    nala_params_p->buf = buf;
}

void read_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_read()->buf_out,
                       buf_p,
                       size);
}

void read_mock_set_buf_out_copy(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_read_t *nala_params_p;

    nala_params_p = nala_get_params_read();

    if (nala_params_p->buf_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "read_mock_set_buf_out() must be called "
                "before read_mock_set_buf_out_copy().\n"));
    }

    nala_params_p->buf_out_copy = callback;
}

void read_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_read);
    nala_mock_read.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_read.data.traceback);
}

void read_mock_implementation(ssize_t (*implementation)(int fd, void *buf, size_t count))
{
    CHECK_NO_INSTANCES(nala_mock_read);
    nala_mock_read.state.mode = MODE_IMPLEMENTATION;
    nala_mock_read.data.implementation = implementation;
    nala_traceback(&nala_mock_read.data.traceback);
}

void read_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_read);
    nala_mock_read.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_read.data.traceback);
}

void read_mock_real_once(void)
{
    struct nala_instance_read_t *instance_p;

    nala_mock_read.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_read.instances,
                          instance_p);
}

void read_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_read.state);
}

void read_mock_resume(void)
{
    nala_state_resume(&nala_mock_read.state);
}

void read_mock_reset(void)
{
    struct nala_instance_read_t *current_p;
    struct nala_instance_read_t *tmp_p;

    NALA_STATE_RESET(nala_mock_read, current_p, tmp_p);
}

void read_mock_assert_completed(void)
{
    if (nala_mock_read.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked read() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_read.instances.length),
                &nala_mock_read.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION sendto

ssize_t __real_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

struct nala_params_sendto_t {
    int sockfd;
    const void *buf;
    size_t len;
    int flags;
    const struct sockaddr *dest_addr;
    socklen_t addrlen;
    bool ignore_sockfd_in;
    bool ignore_buf_in;
    struct nala_set_param buf_in;
    void (*buf_in_assert)(const void *buf, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_out;
    void (*buf_out_copy)(const void *buf, const void *nala_buf_p, size_t nala_size);
    bool ignore_len_in;
    bool ignore_flags_in;
    bool ignore_dest_addr_in;
    struct nala_set_param dest_addr_in;
    void (*dest_addr_in_assert)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param dest_addr_out;
    void (*dest_addr_out_copy)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size);
    bool ignore_addrlen_in;
};

struct nala_data_sendto_t {
    struct nala_params_sendto_t params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    struct nala_traceback_t traceback;
    struct nala_sendto_params_t params_in;
};

struct nala_instance_sendto_t {
    int mode;
    int handle;
    struct nala_data_sendto_t data;
    struct nala_instance_sendto_t *next_p;
};

struct nala_instances_sendto_t {
    struct nala_instance_sendto_t *head_p;
    struct nala_instance_sendto_t *next_p;
    struct nala_instance_sendto_t *tail_p;
    int length;
};

struct nala_mock_sendto_t {
    struct nala_state_t state;
    struct nala_data_sendto_t data;
    struct nala_instances_sendto_t instances;
};

static struct nala_mock_sendto_t nala_mock_sendto = {
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

struct nala_data_sendto_t *nala_get_data_sendto()
{
    if (nala_mock_sendto.instances.tail_p != NULL) {
        return (&nala_mock_sendto.instances.tail_p->data);
    } else {
        return (&nala_mock_sendto.data);
    }
}

struct nala_params_sendto_t *nala_get_params_sendto()
{
    return (&nala_get_data_sendto()->params);
}

ssize_t __wrap_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    struct nala_instance_sendto_t *nala_instance_p;
    struct nala_data_sendto_t *nala_data_p;
    ssize_t return_value;

    nala_print_call("sendto", &nala_mock_sendto.state);

    switch (nala_mock_sendto.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_sendto.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_sendto.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked sendto() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_sendto.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, sendto, buf);
            nala_data_p->params_in.buf = buf;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, sendto, dest_addr);
            nala_data_p->params_in.dest_addr = dest_addr;
            MOCK_ASSERT_IN_EQ(nala_data_p, sendto, sockfd);
            nala_data_p->params_in.sockfd = sockfd;
            MOCK_ASSERT_IN_EQ(nala_data_p, sendto, len);
            nala_data_p->params_in.len = len;
            MOCK_ASSERT_IN_EQ(nala_data_p, sendto, flags);
            nala_data_p->params_in.flags = flags;
            MOCK_ASSERT_IN_EQ(nala_data_p, sendto, addrlen);
            nala_data_p->params_in.addrlen = addrlen;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       sendto,
                                       buf);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_sockaddr,
                                       sendto,
                                       dest_addr);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(sockfd, buf, len, flags, dest_addr, addrlen);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_sendto.data.implementation(sockfd, buf, len, flags, dest_addr, addrlen);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_sendto.data.traceback,
                            "sendto");
        exit(1);
        break;

    default:
        return_value =
        __real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
        break;
    }

    return return_value;
}

void sendto_mock(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_sendto);
    nala_mock_sendto.state.mode = MODE_MOCK;
    nala_mock_sendto.data.params.buf = NULL;
    nala_mock_sendto.data.params.ignore_buf_in = true;
    nala_mock_sendto.data.params.dest_addr = NULL;
    nala_mock_sendto.data.params.ignore_dest_addr_in = true;
    nala_mock_sendto.data.params.sockfd = sockfd;
    nala_mock_sendto.data.params.ignore_sockfd_in = false;
    nala_mock_sendto.data.params.len = len;
    nala_mock_sendto.data.params.ignore_len_in = false;
    nala_mock_sendto.data.params.flags = flags;
    nala_mock_sendto.data.params.ignore_flags_in = false;
    nala_mock_sendto.data.params.addrlen = addrlen;
    nala_mock_sendto.data.params.ignore_addrlen_in = false;
    nala_mock_sendto.data.return_value = return_value;
    nala_mock_sendto.data.errno_value = 0;
    nala_mock_sendto.data.callback = NULL;
    nala_traceback(&nala_mock_sendto.data.traceback);
}

int sendto_mock_once(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value)
{
    struct nala_instance_sendto_t *nala_instance_p;

    nala_mock_sendto.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.buf_out);
    nala_set_param_init(&nala_instance_p->data.params.buf_in);
    nala_instance_p->data.params.buf_in_assert = NULL;
    nala_instance_p->data.params.buf_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.dest_addr_out);
    nala_set_param_init(&nala_instance_p->data.params.dest_addr_in);
    nala_instance_p->data.params.dest_addr_in_assert = NULL;
    nala_instance_p->data.params.dest_addr_out_copy = NULL;
    nala_instance_p->data.params.buf = NULL;
    nala_instance_p->data.params.ignore_buf_in = true;
    nala_instance_p->data.params.dest_addr = NULL;
    nala_instance_p->data.params.ignore_dest_addr_in = true;
    nala_instance_p->data.params.sockfd = sockfd;
    nala_instance_p->data.params.ignore_sockfd_in = false;
    nala_instance_p->data.params.len = len;
    nala_instance_p->data.params.ignore_len_in = false;
    nala_instance_p->data.params.flags = flags;
    nala_instance_p->data.params.ignore_flags_in = false;
    nala_instance_p->data.params.addrlen = addrlen;
    nala_instance_p->data.params.ignore_addrlen_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_sendto.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void sendto_mock_ignore_in(ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_sendto);
    nala_mock_sendto.state.mode = MODE_MOCK;
    nala_mock_sendto.data.params.ignore_buf_in = true;
    nala_mock_sendto.data.params.ignore_dest_addr_in = true;
    nala_mock_sendto.data.params.ignore_sockfd_in = true;
    nala_mock_sendto.data.params.ignore_len_in = true;
    nala_mock_sendto.data.params.ignore_flags_in = true;
    nala_mock_sendto.data.params.ignore_addrlen_in = true;
    nala_mock_sendto.data.return_value = return_value;
    nala_mock_sendto.data.errno_value = 0;
    nala_mock_sendto.data.callback = NULL;
}

int sendto_mock_ignore_in_once(ssize_t return_value)
{
    struct nala_instance_sendto_t *instance_p;

    nala_mock_sendto.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.buf_out);
    nala_set_param_init(&instance_p->data.params.buf_in);
    instance_p->data.params.buf_in_assert = NULL;
    instance_p->data.params.buf_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.dest_addr_out);
    nala_set_param_init(&instance_p->data.params.dest_addr_in);
    instance_p->data.params.dest_addr_in_assert = NULL;
    instance_p->data.params.dest_addr_out_copy = NULL;
    instance_p->data.params.buf = NULL;
    instance_p->data.params.ignore_buf_in = true;
    instance_p->data.params.dest_addr = NULL;
    instance_p->data.params.ignore_dest_addr_in = true;
    instance_p->data.params.ignore_sockfd_in = true;
    instance_p->data.params.ignore_len_in = true;
    instance_p->data.params.ignore_flags_in = true;
    instance_p->data.params.ignore_addrlen_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_sendto.instances,
                          instance_p);

    return (instance_p->handle);
}

void sendto_mock_set_errno(int errno_value)
{
    nala_get_data_sendto()->errno_value = errno_value;
}

void sendto_mock_set_callback(void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen))
{
    nala_get_data_sendto()->callback = callback;
}

struct nala_sendto_params_t *sendto_mock_get_params_in(int handle)
{
    struct nala_instance_sendto_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_sendto.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "sendto() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void sendto_mock_ignore_sockfd_in(void)
{
    nala_get_params_sendto()->ignore_sockfd_in = true;
}

void sendto_mock_ignore_len_in(void)
{
    nala_get_params_sendto()->ignore_len_in = true;
}

void sendto_mock_ignore_flags_in(void)
{
    nala_get_params_sendto()->ignore_flags_in = true;
}

void sendto_mock_ignore_addrlen_in(void)
{
    nala_get_params_sendto()->ignore_addrlen_in = true;
}

void sendto_mock_set_buf_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->buf_in,
                       buf_p,
                       size);
}

void sendto_mock_set_buf_in_assert(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();

    if (nala_params_p->buf_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "sendto_mock_set_buf_in() must be called "
                "before sendto_mock_set_buf_in_assert().\n"));
    }

    nala_params_p->buf_in_assert = callback;
}

void sendto_mock_set_buf_in_pointer(const void *buf)
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();
    nala_params_p->ignore_buf_in = false;
    nala_params_p->buf = buf;
}

void sendto_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->buf_out,
                       buf_p,
                       size);
}

void sendto_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();

    if (nala_params_p->buf_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "sendto_mock_set_buf_out() must be called "
                "before sendto_mock_set_buf_out_copy().\n"));
    }

    nala_params_p->buf_out_copy = callback;
}

void sendto_mock_set_dest_addr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->dest_addr_in,
                       buf_p,
                       size);
}

void sendto_mock_set_dest_addr_in_assert(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();

    if (nala_params_p->dest_addr_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "sendto_mock_set_dest_addr_in() must be called "
                "before sendto_mock_set_dest_addr_in_assert().\n"));
    }

    nala_params_p->dest_addr_in_assert = callback;
}

void sendto_mock_set_dest_addr_in_pointer(const struct sockaddr *dest_addr)
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();
    nala_params_p->ignore_dest_addr_in = false;
    nala_params_p->dest_addr = dest_addr;
}

void sendto_mock_set_dest_addr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->dest_addr_out,
                       buf_p,
                       size);
}

void sendto_mock_set_dest_addr_out_copy(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_sendto_t *nala_params_p;

    nala_params_p = nala_get_params_sendto();

    if (nala_params_p->dest_addr_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "sendto_mock_set_dest_addr_out() must be called "
                "before sendto_mock_set_dest_addr_out_copy().\n"));
    }

    nala_params_p->dest_addr_out_copy = callback;
}

void sendto_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_sendto);
    nala_mock_sendto.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_sendto.data.traceback);
}

void sendto_mock_implementation(ssize_t (*implementation)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen))
{
    CHECK_NO_INSTANCES(nala_mock_sendto);
    nala_mock_sendto.state.mode = MODE_IMPLEMENTATION;
    nala_mock_sendto.data.implementation = implementation;
    nala_traceback(&nala_mock_sendto.data.traceback);
}

void sendto_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_sendto);
    nala_mock_sendto.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_sendto.data.traceback);
}

void sendto_mock_real_once(void)
{
    struct nala_instance_sendto_t *instance_p;

    nala_mock_sendto.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_sendto.instances,
                          instance_p);
}

void sendto_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_sendto.state);
}

void sendto_mock_resume(void)
{
    nala_state_resume(&nala_mock_sendto.state);
}

void sendto_mock_reset(void)
{
    struct nala_instance_sendto_t *current_p;
    struct nala_instance_sendto_t *tmp_p;

    NALA_STATE_RESET(nala_mock_sendto, current_p, tmp_p);
}

void sendto_mock_assert_completed(void)
{
    if (nala_mock_sendto.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked sendto() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_sendto.instances.length),
                &nala_mock_sendto.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION setsockopt

int __real_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

struct nala_params_setsockopt_t {
    int sockfd;
    int level;
    int optname;
    const void *optval;
    socklen_t optlen;
    bool ignore_sockfd_in;
    bool ignore_level_in;
    bool ignore_optname_in;
    bool ignore_optval_in;
    struct nala_set_param optval_in;
    void (*optval_in_assert)(const void *optval, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param optval_out;
    void (*optval_out_copy)(const void *optval, const void *nala_buf_p, size_t nala_size);
    bool ignore_optlen_in;
};

struct nala_data_setsockopt_t {
    struct nala_params_setsockopt_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    struct nala_traceback_t traceback;
    struct nala_setsockopt_params_t params_in;
};

struct nala_instance_setsockopt_t {
    int mode;
    int handle;
    struct nala_data_setsockopt_t data;
    struct nala_instance_setsockopt_t *next_p;
};

struct nala_instances_setsockopt_t {
    struct nala_instance_setsockopt_t *head_p;
    struct nala_instance_setsockopt_t *next_p;
    struct nala_instance_setsockopt_t *tail_p;
    int length;
};

struct nala_mock_setsockopt_t {
    struct nala_state_t state;
    struct nala_data_setsockopt_t data;
    struct nala_instances_setsockopt_t instances;
};

static struct nala_mock_setsockopt_t nala_mock_setsockopt = {
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

struct nala_data_setsockopt_t *nala_get_data_setsockopt()
{
    if (nala_mock_setsockopt.instances.tail_p != NULL) {
        return (&nala_mock_setsockopt.instances.tail_p->data);
    } else {
        return (&nala_mock_setsockopt.data);
    }
}

struct nala_params_setsockopt_t *nala_get_params_setsockopt()
{
    return (&nala_get_data_setsockopt()->params);
}

int __wrap_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    struct nala_instance_setsockopt_t *nala_instance_p;
    struct nala_data_setsockopt_t *nala_data_p;
    int return_value;

    nala_print_call("setsockopt", &nala_mock_setsockopt.state);

    switch (nala_mock_setsockopt.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_setsockopt.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_setsockopt.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked setsockopt() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_setsockopt.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, setsockopt, optval);
            nala_data_p->params_in.optval = optval;
            MOCK_ASSERT_IN_EQ(nala_data_p, setsockopt, sockfd);
            nala_data_p->params_in.sockfd = sockfd;
            MOCK_ASSERT_IN_EQ(nala_data_p, setsockopt, level);
            nala_data_p->params_in.level = level;
            MOCK_ASSERT_IN_EQ(nala_data_p, setsockopt, optname);
            nala_data_p->params_in.optname = optname;
            MOCK_ASSERT_IN_EQ(nala_data_p, setsockopt, optlen);
            nala_data_p->params_in.optlen = optlen;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       setsockopt,
                                       optval);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(sockfd, level, optname, optval, optlen);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_setsockopt(sockfd, level, optname, optval, optlen);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_setsockopt.data.implementation(sockfd, level, optname, optval, optlen);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_setsockopt.data.traceback,
                            "setsockopt");
        exit(1);
        break;

    default:
        return_value =
        __real_setsockopt(sockfd, level, optname, optval, optlen);
        break;
    }

    return return_value;
}

void setsockopt_mock(int sockfd, int level, int optname, socklen_t optlen, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_setsockopt);
    nala_mock_setsockopt.state.mode = MODE_MOCK;
    nala_mock_setsockopt.data.params.optval = NULL;
    nala_mock_setsockopt.data.params.ignore_optval_in = true;
    nala_mock_setsockopt.data.params.sockfd = sockfd;
    nala_mock_setsockopt.data.params.ignore_sockfd_in = false;
    nala_mock_setsockopt.data.params.level = level;
    nala_mock_setsockopt.data.params.ignore_level_in = false;
    nala_mock_setsockopt.data.params.optname = optname;
    nala_mock_setsockopt.data.params.ignore_optname_in = false;
    nala_mock_setsockopt.data.params.optlen = optlen;
    nala_mock_setsockopt.data.params.ignore_optlen_in = false;
    nala_mock_setsockopt.data.return_value = return_value;
    nala_mock_setsockopt.data.errno_value = 0;
    nala_mock_setsockopt.data.callback = NULL;
    nala_traceback(&nala_mock_setsockopt.data.traceback);
}

int setsockopt_mock_once(int sockfd, int level, int optname, socklen_t optlen, int return_value)
{
    struct nala_instance_setsockopt_t *nala_instance_p;

    nala_mock_setsockopt.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.optval_out);
    nala_set_param_init(&nala_instance_p->data.params.optval_in);
    nala_instance_p->data.params.optval_in_assert = NULL;
    nala_instance_p->data.params.optval_out_copy = NULL;
    nala_instance_p->data.params.optval = NULL;
    nala_instance_p->data.params.ignore_optval_in = true;
    nala_instance_p->data.params.sockfd = sockfd;
    nala_instance_p->data.params.ignore_sockfd_in = false;
    nala_instance_p->data.params.level = level;
    nala_instance_p->data.params.ignore_level_in = false;
    nala_instance_p->data.params.optname = optname;
    nala_instance_p->data.params.ignore_optname_in = false;
    nala_instance_p->data.params.optlen = optlen;
    nala_instance_p->data.params.ignore_optlen_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_setsockopt.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void setsockopt_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_setsockopt);
    nala_mock_setsockopt.state.mode = MODE_MOCK;
    nala_mock_setsockopt.data.params.ignore_optval_in = true;
    nala_mock_setsockopt.data.params.ignore_sockfd_in = true;
    nala_mock_setsockopt.data.params.ignore_level_in = true;
    nala_mock_setsockopt.data.params.ignore_optname_in = true;
    nala_mock_setsockopt.data.params.ignore_optlen_in = true;
    nala_mock_setsockopt.data.return_value = return_value;
    nala_mock_setsockopt.data.errno_value = 0;
    nala_mock_setsockopt.data.callback = NULL;
}

int setsockopt_mock_ignore_in_once(int return_value)
{
    struct nala_instance_setsockopt_t *instance_p;

    nala_mock_setsockopt.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.optval_out);
    nala_set_param_init(&instance_p->data.params.optval_in);
    instance_p->data.params.optval_in_assert = NULL;
    instance_p->data.params.optval_out_copy = NULL;
    instance_p->data.params.optval = NULL;
    instance_p->data.params.ignore_optval_in = true;
    instance_p->data.params.ignore_sockfd_in = true;
    instance_p->data.params.ignore_level_in = true;
    instance_p->data.params.ignore_optname_in = true;
    instance_p->data.params.ignore_optlen_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_setsockopt.instances,
                          instance_p);

    return (instance_p->handle);
}

void setsockopt_mock_set_errno(int errno_value)
{
    nala_get_data_setsockopt()->errno_value = errno_value;
}

void setsockopt_mock_set_callback(void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen))
{
    nala_get_data_setsockopt()->callback = callback;
}

struct nala_setsockopt_params_t *setsockopt_mock_get_params_in(int handle)
{
    struct nala_instance_setsockopt_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_setsockopt.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "setsockopt() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void setsockopt_mock_ignore_sockfd_in(void)
{
    nala_get_params_setsockopt()->ignore_sockfd_in = true;
}

void setsockopt_mock_ignore_level_in(void)
{
    nala_get_params_setsockopt()->ignore_level_in = true;
}

void setsockopt_mock_ignore_optname_in(void)
{
    nala_get_params_setsockopt()->ignore_optname_in = true;
}

void setsockopt_mock_ignore_optlen_in(void)
{
    nala_get_params_setsockopt()->ignore_optlen_in = true;
}

void setsockopt_mock_set_optval_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_setsockopt()->optval_in,
                       buf_p,
                       size);
}

void setsockopt_mock_set_optval_in_assert(void (*callback)(const void *optval, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_setsockopt_t *nala_params_p;

    nala_params_p = nala_get_params_setsockopt();

    if (nala_params_p->optval_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "setsockopt_mock_set_optval_in() must be called "
                "before setsockopt_mock_set_optval_in_assert().\n"));
    }

    nala_params_p->optval_in_assert = callback;
}

void setsockopt_mock_set_optval_in_pointer(const void *optval)
{
    struct nala_params_setsockopt_t *nala_params_p;

    nala_params_p = nala_get_params_setsockopt();
    nala_params_p->ignore_optval_in = false;
    nala_params_p->optval = optval;
}

void setsockopt_mock_set_optval_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_setsockopt()->optval_out,
                       buf_p,
                       size);
}

void setsockopt_mock_set_optval_out_copy(void (*callback)(const void *optval, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_setsockopt_t *nala_params_p;

    nala_params_p = nala_get_params_setsockopt();

    if (nala_params_p->optval_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "setsockopt_mock_set_optval_out() must be called "
                "before setsockopt_mock_set_optval_out_copy().\n"));
    }

    nala_params_p->optval_out_copy = callback;
}

void setsockopt_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_setsockopt);
    nala_mock_setsockopt.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_setsockopt.data.traceback);
}

void setsockopt_mock_implementation(int (*implementation)(int sockfd, int level, int optname, const void *optval, socklen_t optlen))
{
    CHECK_NO_INSTANCES(nala_mock_setsockopt);
    nala_mock_setsockopt.state.mode = MODE_IMPLEMENTATION;
    nala_mock_setsockopt.data.implementation = implementation;
    nala_traceback(&nala_mock_setsockopt.data.traceback);
}

void setsockopt_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_setsockopt);
    nala_mock_setsockopt.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_setsockopt.data.traceback);
}

void setsockopt_mock_real_once(void)
{
    struct nala_instance_setsockopt_t *instance_p;

    nala_mock_setsockopt.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_setsockopt.instances,
                          instance_p);
}

void setsockopt_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_setsockopt.state);
}

void setsockopt_mock_resume(void)
{
    nala_state_resume(&nala_mock_setsockopt.state);
}

void setsockopt_mock_reset(void)
{
    struct nala_instance_setsockopt_t *current_p;
    struct nala_instance_setsockopt_t *tmp_p;

    NALA_STATE_RESET(nala_mock_setsockopt, current_p, tmp_p);
}

void setsockopt_mock_assert_completed(void)
{
    if (nala_mock_setsockopt.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked setsockopt() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_setsockopt.instances.length),
                &nala_mock_setsockopt.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION sleep

unsigned int __real_sleep(unsigned int seconds);

struct nala_params_sleep_t {
    unsigned int seconds;
    bool ignore_seconds_in;
};

struct nala_data_sleep_t {
    struct nala_params_sleep_t params;
    unsigned int return_value;
    int errno_value;
    unsigned int (*implementation)(unsigned int seconds);
    void (*callback)(unsigned int seconds);
    struct nala_traceback_t traceback;
    struct nala_sleep_params_t params_in;
};

struct nala_instance_sleep_t {
    int mode;
    int handle;
    struct nala_data_sleep_t data;
    struct nala_instance_sleep_t *next_p;
};

struct nala_instances_sleep_t {
    struct nala_instance_sleep_t *head_p;
    struct nala_instance_sleep_t *next_p;
    struct nala_instance_sleep_t *tail_p;
    int length;
};

struct nala_mock_sleep_t {
    struct nala_state_t state;
    struct nala_data_sleep_t data;
    struct nala_instances_sleep_t instances;
};

static struct nala_mock_sleep_t nala_mock_sleep = {
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

struct nala_data_sleep_t *nala_get_data_sleep()
{
    if (nala_mock_sleep.instances.tail_p != NULL) {
        return (&nala_mock_sleep.instances.tail_p->data);
    } else {
        return (&nala_mock_sleep.data);
    }
}

struct nala_params_sleep_t *nala_get_params_sleep()
{
    return (&nala_get_data_sleep()->params);
}

unsigned int __wrap_sleep(unsigned int seconds)
{
    struct nala_instance_sleep_t *nala_instance_p;
    struct nala_data_sleep_t *nala_data_p;
    unsigned int return_value;

    nala_print_call("sleep", &nala_mock_sleep.state);

    switch (nala_mock_sleep.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_sleep.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_sleep.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked sleep() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_sleep.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, sleep, seconds);
            nala_data_p->params_in.seconds = seconds;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(seconds);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_sleep(seconds);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_sleep.data.implementation(seconds);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_sleep.data.traceback,
                            "sleep");
        exit(1);
        break;

    default:
        return_value =
        __real_sleep(seconds);
        break;
    }

    return return_value;
}

void sleep_mock(unsigned int seconds, unsigned int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_sleep);
    nala_mock_sleep.state.mode = MODE_MOCK;
    nala_mock_sleep.data.params.seconds = seconds;
    nala_mock_sleep.data.params.ignore_seconds_in = false;
    nala_mock_sleep.data.return_value = return_value;
    nala_mock_sleep.data.errno_value = 0;
    nala_mock_sleep.data.callback = NULL;
    nala_traceback(&nala_mock_sleep.data.traceback);
}

int sleep_mock_once(unsigned int seconds, unsigned int return_value)
{
    struct nala_instance_sleep_t *nala_instance_p;

    nala_mock_sleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.seconds = seconds;
    nala_instance_p->data.params.ignore_seconds_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_sleep.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void sleep_mock_ignore_in(unsigned int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_sleep);
    nala_mock_sleep.state.mode = MODE_MOCK;
    nala_mock_sleep.data.params.ignore_seconds_in = true;
    nala_mock_sleep.data.return_value = return_value;
    nala_mock_sleep.data.errno_value = 0;
    nala_mock_sleep.data.callback = NULL;
}

int sleep_mock_ignore_in_once(unsigned int return_value)
{
    struct nala_instance_sleep_t *instance_p;

    nala_mock_sleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_seconds_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_sleep.instances,
                          instance_p);

    return (instance_p->handle);
}

void sleep_mock_set_errno(int errno_value)
{
    nala_get_data_sleep()->errno_value = errno_value;
}

void sleep_mock_set_callback(void (*callback)(unsigned int seconds))
{
    nala_get_data_sleep()->callback = callback;
}

struct nala_sleep_params_t *sleep_mock_get_params_in(int handle)
{
    struct nala_instance_sleep_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_sleep.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "sleep() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void sleep_mock_ignore_seconds_in(void)
{
    nala_get_params_sleep()->ignore_seconds_in = true;
}

void sleep_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_sleep);
    nala_mock_sleep.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_sleep.data.traceback);
}

void sleep_mock_implementation(unsigned int (*implementation)(unsigned int seconds))
{
    CHECK_NO_INSTANCES(nala_mock_sleep);
    nala_mock_sleep.state.mode = MODE_IMPLEMENTATION;
    nala_mock_sleep.data.implementation = implementation;
    nala_traceback(&nala_mock_sleep.data.traceback);
}

void sleep_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_sleep);
    nala_mock_sleep.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_sleep.data.traceback);
}

void sleep_mock_real_once(void)
{
    struct nala_instance_sleep_t *instance_p;

    nala_mock_sleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_sleep.instances,
                          instance_p);
}

void sleep_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_sleep.state);
}

void sleep_mock_resume(void)
{
    nala_state_resume(&nala_mock_sleep.state);
}

void sleep_mock_reset(void)
{
    struct nala_instance_sleep_t *current_p;
    struct nala_instance_sleep_t *tmp_p;

    NALA_STATE_RESET(nala_mock_sleep, current_p, tmp_p);
}

void sleep_mock_assert_completed(void)
{
    if (nala_mock_sleep.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked sleep() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_sleep.instances.length),
                &nala_mock_sleep.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION statvfs

int __real_statvfs(const char *path, struct statvfs *buf);

struct nala_params_statvfs_t {
    const char *path;
    struct statvfs *buf;
    bool ignore_path_in;
    struct nala_set_param path_in;
    void (*path_in_assert)(const char *path, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param path_out;
    void (*path_out_copy)(const char *path, const void *nala_buf_p, size_t nala_size);
    bool ignore_buf_in;
    struct nala_set_param buf_in;
    void (*buf_in_assert)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_out;
    void (*buf_out_copy)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_statvfs_t {
    struct nala_params_statvfs_t params;
    int return_value;
    int errno_value;
    int (*implementation)(const char *path, struct statvfs *buf);
    void (*callback)(const char *path, struct statvfs *buf);
    struct nala_traceback_t traceback;
    struct nala_statvfs_params_t params_in;
};

struct nala_instance_statvfs_t {
    int mode;
    int handle;
    struct nala_data_statvfs_t data;
    struct nala_instance_statvfs_t *next_p;
};

struct nala_instances_statvfs_t {
    struct nala_instance_statvfs_t *head_p;
    struct nala_instance_statvfs_t *next_p;
    struct nala_instance_statvfs_t *tail_p;
    int length;
};

struct nala_mock_statvfs_t {
    struct nala_state_t state;
    struct nala_data_statvfs_t data;
    struct nala_instances_statvfs_t instances;
};

static struct nala_mock_statvfs_t nala_mock_statvfs = {
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

struct nala_data_statvfs_t *nala_get_data_statvfs()
{
    if (nala_mock_statvfs.instances.tail_p != NULL) {
        return (&nala_mock_statvfs.instances.tail_p->data);
    } else {
        return (&nala_mock_statvfs.data);
    }
}

struct nala_params_statvfs_t *nala_get_params_statvfs()
{
    return (&nala_get_data_statvfs()->params);
}

int __wrap_statvfs(const char *path, struct statvfs *buf)
{
    struct nala_instance_statvfs_t *nala_instance_p;
    struct nala_data_statvfs_t *nala_data_p;
    int return_value;

    nala_print_call("statvfs", &nala_mock_statvfs.state);

    switch (nala_mock_statvfs.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_statvfs.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_statvfs.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked statvfs() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_statvfs.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, statvfs, path);
            nala_data_p->params_in.path = path;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, statvfs, buf);
            nala_data_p->params_in.buf = buf;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       statvfs,
                                       path);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_statvfs,
                                       statvfs,
                                       buf);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(path, buf);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_statvfs(path, buf);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_statvfs.data.implementation(path, buf);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_statvfs.data.traceback,
                            "statvfs");
        exit(1);
        break;

    default:
        return_value =
        __real_statvfs(path, buf);
        break;
    }

    return return_value;
}

void statvfs_mock(const char *path, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_statvfs);
    nala_mock_statvfs.state.mode = MODE_MOCK;
    nala_mock_statvfs.data.params.path = NULL;
    nala_mock_statvfs.data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_mock_statvfs.data.params.path_in,
                              path);
    } else {
        nala_mock_statvfs.data.params.ignore_path_in = false;
    }

    nala_mock_statvfs.data.params.buf = NULL;
    nala_mock_statvfs.data.params.ignore_buf_in = true;
    nala_mock_statvfs.data.return_value = return_value;
    nala_mock_statvfs.data.errno_value = 0;
    nala_mock_statvfs.data.callback = NULL;
    nala_traceback(&nala_mock_statvfs.data.traceback);
}

int statvfs_mock_once(const char *path, int return_value)
{
    struct nala_instance_statvfs_t *nala_instance_p;

    nala_mock_statvfs.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.path_out);
    nala_set_param_init(&nala_instance_p->data.params.path_in);
    nala_instance_p->data.params.path_in_assert = NULL;
    nala_instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.buf_out);
    nala_set_param_init(&nala_instance_p->data.params.buf_in);
    nala_instance_p->data.params.buf_in_assert = NULL;
    nala_instance_p->data.params.buf_out_copy = NULL;
    nala_instance_p->data.params.path = NULL;
    nala_instance_p->data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.path_in,
                              path);
    } else {
        nala_instance_p->data.params.ignore_path_in = false;
    }

    nala_instance_p->data.params.buf = NULL;
    nala_instance_p->data.params.ignore_buf_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_statvfs.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void statvfs_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_statvfs);
    nala_mock_statvfs.state.mode = MODE_MOCK;
    nala_mock_statvfs.data.params.ignore_path_in = true;
    nala_mock_statvfs.data.params.ignore_buf_in = true;
    nala_mock_statvfs.data.return_value = return_value;
    nala_mock_statvfs.data.errno_value = 0;
    nala_mock_statvfs.data.callback = NULL;
}

int statvfs_mock_ignore_in_once(int return_value)
{
    struct nala_instance_statvfs_t *instance_p;

    nala_mock_statvfs.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.path_out);
    nala_set_param_init(&instance_p->data.params.path_in);
    instance_p->data.params.path_in_assert = NULL;
    instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.buf_out);
    nala_set_param_init(&instance_p->data.params.buf_in);
    instance_p->data.params.buf_in_assert = NULL;
    instance_p->data.params.buf_out_copy = NULL;
    instance_p->data.params.path = NULL;
    instance_p->data.params.ignore_path_in = true;
    instance_p->data.params.buf = NULL;
    instance_p->data.params.ignore_buf_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_statvfs.instances,
                          instance_p);

    return (instance_p->handle);
}

void statvfs_mock_set_errno(int errno_value)
{
    nala_get_data_statvfs()->errno_value = errno_value;
}

void statvfs_mock_set_callback(void (*callback)(const char *path, struct statvfs *buf))
{
    nala_get_data_statvfs()->callback = callback;
}

struct nala_statvfs_params_t *statvfs_mock_get_params_in(int handle)
{
    struct nala_instance_statvfs_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_statvfs.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "statvfs() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void statvfs_mock_ignore_path_in(void)
{
    nala_get_params_statvfs()->ignore_path_in = true;
}

void statvfs_mock_set_path_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->path_in,
                       buf_p,
                       size);
}

void statvfs_mock_set_path_in_assert(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();

    if (nala_params_p->path_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "statvfs_mock_set_path_in() must be called "
                "before statvfs_mock_set_path_in_assert().\n"));
    }

    nala_params_p->path_in_assert = callback;
}

void statvfs_mock_set_path_in_pointer(const char *path)
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();
    nala_params_p->ignore_path_in = false;
    nala_params_p->path = path;
}

void statvfs_mock_set_path_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->path_out,
                       buf_p,
                       size);
}

void statvfs_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();

    if (nala_params_p->path_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "statvfs_mock_set_path_out() must be called "
                "before statvfs_mock_set_path_out_copy().\n"));
    }

    nala_params_p->path_out_copy = callback;
}

void statvfs_mock_set_buf_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->buf_in,
                       buf_p,
                       size);
}

void statvfs_mock_set_buf_in_assert(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();

    if (nala_params_p->buf_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "statvfs_mock_set_buf_in() must be called "
                "before statvfs_mock_set_buf_in_assert().\n"));
    }

    nala_params_p->buf_in_assert = callback;
}

void statvfs_mock_set_buf_in_pointer(struct statvfs *buf)
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();
    nala_params_p->ignore_buf_in = false;
    nala_params_p->buf = buf;
}

void statvfs_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->buf_out,
                       buf_p,
                       size);
}

void statvfs_mock_set_buf_out_copy(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_statvfs_t *nala_params_p;

    nala_params_p = nala_get_params_statvfs();

    if (nala_params_p->buf_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "statvfs_mock_set_buf_out() must be called "
                "before statvfs_mock_set_buf_out_copy().\n"));
    }

    nala_params_p->buf_out_copy = callback;
}

void statvfs_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_statvfs);
    nala_mock_statvfs.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_statvfs.data.traceback);
}

void statvfs_mock_implementation(int (*implementation)(const char *path, struct statvfs *buf))
{
    CHECK_NO_INSTANCES(nala_mock_statvfs);
    nala_mock_statvfs.state.mode = MODE_IMPLEMENTATION;
    nala_mock_statvfs.data.implementation = implementation;
    nala_traceback(&nala_mock_statvfs.data.traceback);
}

void statvfs_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_statvfs);
    nala_mock_statvfs.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_statvfs.data.traceback);
}

void statvfs_mock_real_once(void)
{
    struct nala_instance_statvfs_t *instance_p;

    nala_mock_statvfs.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_statvfs.instances,
                          instance_p);
}

void statvfs_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_statvfs.state);
}

void statvfs_mock_resume(void)
{
    nala_state_resume(&nala_mock_statvfs.state);
}

void statvfs_mock_reset(void)
{
    struct nala_instance_statvfs_t *current_p;
    struct nala_instance_statvfs_t *tmp_p;

    NALA_STATE_RESET(nala_mock_statvfs, current_p, tmp_p);
}

void statvfs_mock_assert_completed(void)
{
    if (nala_mock_statvfs.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked statvfs() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_statvfs.instances.length),
                &nala_mock_statvfs.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION struct_param

void __real_struct_param(struct struct_param_type *data);

struct nala_params_struct_param_t {
    struct struct_param_type *data;
    bool ignore_data_in;
    struct nala_set_param data_in;
    void (*data_in_assert)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param data_out;
    void (*data_out_copy)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_struct_param_t {
    struct nala_params_struct_param_t params;
    int errno_value;
    void (*implementation)(struct struct_param_type *data);
    void (*callback)(struct struct_param_type *data);
    struct nala_traceback_t traceback;
    struct nala_struct_param_params_t params_in;
};

struct nala_instance_struct_param_t {
    int mode;
    int handle;
    struct nala_data_struct_param_t data;
    struct nala_instance_struct_param_t *next_p;
};

struct nala_instances_struct_param_t {
    struct nala_instance_struct_param_t *head_p;
    struct nala_instance_struct_param_t *next_p;
    struct nala_instance_struct_param_t *tail_p;
    int length;
};

struct nala_mock_struct_param_t {
    struct nala_state_t state;
    struct nala_data_struct_param_t data;
    struct nala_instances_struct_param_t instances;
};

static struct nala_mock_struct_param_t nala_mock_struct_param = {
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

struct nala_data_struct_param_t *nala_get_data_struct_param()
{
    if (nala_mock_struct_param.instances.tail_p != NULL) {
        return (&nala_mock_struct_param.instances.tail_p->data);
    } else {
        return (&nala_mock_struct_param.data);
    }
}

struct nala_params_struct_param_t *nala_get_params_struct_param()
{
    return (&nala_get_data_struct_param()->params);
}

void __wrap_struct_param(struct struct_param_type *data)
{
    struct nala_instance_struct_param_t *nala_instance_p;
    struct nala_data_struct_param_t *nala_data_p;

    nala_print_call("struct_param", &nala_mock_struct_param.state);

    switch (nala_mock_struct_param.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_struct_param.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_struct_param.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked struct_param() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_struct_param.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, struct_param, data);
            nala_data_p->params_in.data = data;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_struct_param_type,
                                       struct_param,
                                       data);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(data);
            }

        } else {
            __real_struct_param(data);
        }
        break;

    case MODE_IMPLEMENTATION:
        nala_mock_struct_param.data.implementation(data);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_struct_param.data.traceback,
                            "struct_param");
        exit(1);
        break;

    default:
        __real_struct_param(data);
        break;
    }

    return;
}

void struct_param_mock()
{
    CHECK_NO_INSTANCES(nala_mock_struct_param);
    nala_mock_struct_param.state.mode = MODE_MOCK;
    nala_mock_struct_param.data.params.data = NULL;
    nala_mock_struct_param.data.params.ignore_data_in = true;
    nala_mock_struct_param.data.errno_value = 0;
    nala_mock_struct_param.data.callback = NULL;
    nala_traceback(&nala_mock_struct_param.data.traceback);
}

int struct_param_mock_once()
{
    struct nala_instance_struct_param_t *nala_instance_p;

    nala_mock_struct_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.data_out);
    nala_set_param_init(&nala_instance_p->data.params.data_in);
    nala_instance_p->data.params.data_in_assert = NULL;
    nala_instance_p->data.params.data_out_copy = NULL;
    nala_instance_p->data.params.data = NULL;
    nala_instance_p->data.params.ignore_data_in = true;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_struct_param.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void struct_param_mock_ignore_in(void)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param);
    nala_mock_struct_param.state.mode = MODE_MOCK;
    nala_mock_struct_param.data.params.ignore_data_in = true;
    nala_mock_struct_param.data.errno_value = 0;
    nala_mock_struct_param.data.callback = NULL;
}

int struct_param_mock_ignore_in_once(void)
{
    struct nala_instance_struct_param_t *instance_p;

    nala_mock_struct_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.data_out);
    nala_set_param_init(&instance_p->data.params.data_in);
    instance_p->data.params.data_in_assert = NULL;
    instance_p->data.params.data_out_copy = NULL;
    instance_p->data.params.data = NULL;
    instance_p->data.params.ignore_data_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_struct_param.instances,
                          instance_p);

    return (instance_p->handle);
}

void struct_param_mock_set_errno(int errno_value)
{
    nala_get_data_struct_param()->errno_value = errno_value;
}

void struct_param_mock_set_callback(void (*callback)(struct struct_param_type *data))
{
    nala_get_data_struct_param()->callback = callback;
}

struct nala_struct_param_params_t *struct_param_mock_get_params_in(int handle)
{
    struct nala_instance_struct_param_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_struct_param.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "struct_param() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void struct_param_mock_set_data_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_struct_param()->data_in,
                       buf_p,
                       size);
}

void struct_param_mock_set_data_in_assert(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_struct_param_t *nala_params_p;

    nala_params_p = nala_get_params_struct_param();

    if (nala_params_p->data_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "struct_param_mock_set_data_in() must be called "
                "before struct_param_mock_set_data_in_assert().\n"));
    }

    nala_params_p->data_in_assert = callback;
}

void struct_param_mock_set_data_in_pointer(struct struct_param_type *data)
{
    struct nala_params_struct_param_t *nala_params_p;

    nala_params_p = nala_get_params_struct_param();
    nala_params_p->ignore_data_in = false;
    nala_params_p->data = data;
}

void struct_param_mock_set_data_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_struct_param()->data_out,
                       buf_p,
                       size);
}

void struct_param_mock_set_data_out_copy(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_struct_param_t *nala_params_p;

    nala_params_p = nala_get_params_struct_param();

    if (nala_params_p->data_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "struct_param_mock_set_data_out() must be called "
                "before struct_param_mock_set_data_out_copy().\n"));
    }

    nala_params_p->data_out_copy = callback;
}

void struct_param_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param);
    nala_mock_struct_param.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_struct_param.data.traceback);
}

void struct_param_mock_implementation(void (*implementation)(struct struct_param_type *data))
{
    CHECK_NO_INSTANCES(nala_mock_struct_param);
    nala_mock_struct_param.state.mode = MODE_IMPLEMENTATION;
    nala_mock_struct_param.data.implementation = implementation;
    nala_traceback(&nala_mock_struct_param.data.traceback);
}

void struct_param_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param);
    nala_mock_struct_param.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_struct_param.data.traceback);
}

void struct_param_mock_real_once(void)
{
    struct nala_instance_struct_param_t *instance_p;

    nala_mock_struct_param.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_struct_param.instances,
                          instance_p);
}

void struct_param_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_struct_param.state);
}

void struct_param_mock_resume(void)
{
    nala_state_resume(&nala_mock_struct_param.state);
}

void struct_param_mock_reset(void)
{
    struct nala_instance_struct_param_t *current_p;
    struct nala_instance_struct_param_t *tmp_p;

    NALA_STATE_RESET(nala_mock_struct_param, current_p, tmp_p);
}

void struct_param_mock_assert_completed(void)
{
    if (nala_mock_struct_param.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked struct_param() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_struct_param.instances.length),
                &nala_mock_struct_param.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION struct_param_and_return_type

struct struct_param_type __real_struct_param_and_return_type(struct struct_param_type arg);

struct nala_params_struct_param_and_return_type_t {
    struct struct_param_type arg;
};

struct nala_data_struct_param_and_return_type_t {
    struct nala_params_struct_param_and_return_type_t params;
    struct struct_param_type return_value;
    int errno_value;
    struct struct_param_type (*implementation)(struct struct_param_type arg);
    void (*callback)(struct struct_param_type arg);
    struct nala_traceback_t traceback;
    struct nala_struct_param_and_return_type_params_t params_in;
};

struct nala_instance_struct_param_and_return_type_t {
    int mode;
    int handle;
    struct nala_data_struct_param_and_return_type_t data;
    struct nala_instance_struct_param_and_return_type_t *next_p;
};

struct nala_instances_struct_param_and_return_type_t {
    struct nala_instance_struct_param_and_return_type_t *head_p;
    struct nala_instance_struct_param_and_return_type_t *next_p;
    struct nala_instance_struct_param_and_return_type_t *tail_p;
    int length;
};

struct nala_mock_struct_param_and_return_type_t {
    struct nala_state_t state;
    struct nala_data_struct_param_and_return_type_t data;
    struct nala_instances_struct_param_and_return_type_t instances;
};

static struct nala_mock_struct_param_and_return_type_t nala_mock_struct_param_and_return_type = {
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

struct nala_data_struct_param_and_return_type_t *nala_get_data_struct_param_and_return_type()
{
    if (nala_mock_struct_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_mock_struct_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_mock_struct_param_and_return_type.data);
    }
}

struct nala_params_struct_param_and_return_type_t *nala_get_params_struct_param_and_return_type()
{
    return (&nala_get_data_struct_param_and_return_type()->params);
}

struct struct_param_type __wrap_struct_param_and_return_type(struct struct_param_type arg)
{
    struct nala_instance_struct_param_and_return_type_t *nala_instance_p;
    struct nala_data_struct_param_and_return_type_t *nala_data_p;
    struct struct_param_type return_value;

    nala_print_call("struct_param_and_return_type", &nala_mock_struct_param_and_return_type.state);

    switch (nala_mock_struct_param_and_return_type.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_struct_param_and_return_type.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_struct_param_and_return_type.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked struct_param_and_return_type() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_struct_param_and_return_type.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(arg);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_struct_param_and_return_type(arg);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_struct_param_and_return_type.data.implementation(arg);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_struct_param_and_return_type.data.traceback,
                            "struct_param_and_return_type");
        exit(1);
        break;

    default:
        return_value =
        __real_struct_param_and_return_type(arg);
        break;
    }

    return return_value;
}

void struct_param_and_return_type_mock(struct struct_param_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param_and_return_type);
    nala_mock_struct_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_struct_param_and_return_type.data.return_value = return_value;
    nala_mock_struct_param_and_return_type.data.errno_value = 0;
    nala_mock_struct_param_and_return_type.data.callback = NULL;
    nala_traceback(&nala_mock_struct_param_and_return_type.data.traceback);
}

int struct_param_and_return_type_mock_once(struct struct_param_type return_value)
{
    struct nala_instance_struct_param_and_return_type_t *nala_instance_p;

    nala_mock_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_struct_param_and_return_type.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void struct_param_and_return_type_mock_ignore_in(struct struct_param_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param_and_return_type);
    nala_mock_struct_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_struct_param_and_return_type.data.return_value = return_value;
    nala_mock_struct_param_and_return_type.data.errno_value = 0;
    nala_mock_struct_param_and_return_type.data.callback = NULL;
}

int struct_param_and_return_type_mock_ignore_in_once(struct struct_param_type return_value)
{
    struct nala_instance_struct_param_and_return_type_t *instance_p;

    nala_mock_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_struct_param_and_return_type.instances,
                          instance_p);

    return (instance_p->handle);
}

void struct_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_struct_param_and_return_type()->errno_value = errno_value;
}

void struct_param_and_return_type_mock_set_callback(void (*callback)(struct struct_param_type arg))
{
    nala_get_data_struct_param_and_return_type()->callback = callback;
}

struct nala_struct_param_and_return_type_params_t *struct_param_and_return_type_mock_get_params_in(int handle)
{
    struct nala_instance_struct_param_and_return_type_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_struct_param_and_return_type.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "struct_param_and_return_type() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void struct_param_and_return_type_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param_and_return_type);
    nala_mock_struct_param_and_return_type.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_struct_param_and_return_type.data.traceback);
}

void struct_param_and_return_type_mock_implementation(struct struct_param_type (*implementation)(struct struct_param_type arg))
{
    CHECK_NO_INSTANCES(nala_mock_struct_param_and_return_type);
    nala_mock_struct_param_and_return_type.state.mode = MODE_IMPLEMENTATION;
    nala_mock_struct_param_and_return_type.data.implementation = implementation;
    nala_traceback(&nala_mock_struct_param_and_return_type.data.traceback);
}

void struct_param_and_return_type_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_struct_param_and_return_type);
    nala_mock_struct_param_and_return_type.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_struct_param_and_return_type.data.traceback);
}

void struct_param_and_return_type_mock_real_once(void)
{
    struct nala_instance_struct_param_and_return_type_t *instance_p;

    nala_mock_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_struct_param_and_return_type.instances,
                          instance_p);
}

void struct_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_struct_param_and_return_type.state);
}

void struct_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_mock_struct_param_and_return_type.state);
}

void struct_param_and_return_type_mock_reset(void)
{
    struct nala_instance_struct_param_and_return_type_t *current_p;
    struct nala_instance_struct_param_and_return_type_t *tmp_p;

    NALA_STATE_RESET(nala_mock_struct_param_and_return_type, current_p, tmp_p);
}

void struct_param_and_return_type_mock_assert_completed(void)
{
    if (nala_mock_struct_param_and_return_type.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked struct_param_and_return_type() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_struct_param_and_return_type.instances.length),
                &nala_mock_struct_param_and_return_type.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION time

time_t __real_time(time_t *tloc);

struct nala_params_time_t {
    time_t *tloc;
    bool ignore_tloc_in;
    struct nala_set_param tloc_in;
    void (*tloc_in_assert)(time_t *tloc, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param tloc_out;
    void (*tloc_out_copy)(time_t *tloc, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_time_t {
    struct nala_params_time_t params;
    time_t return_value;
    int errno_value;
    time_t (*implementation)(time_t *tloc);
    void (*callback)(time_t *tloc);
    struct nala_traceback_t traceback;
    struct nala_time_params_t params_in;
};

struct nala_instance_time_t {
    int mode;
    int handle;
    struct nala_data_time_t data;
    struct nala_instance_time_t *next_p;
};

struct nala_instances_time_t {
    struct nala_instance_time_t *head_p;
    struct nala_instance_time_t *next_p;
    struct nala_instance_time_t *tail_p;
    int length;
};

struct nala_mock_time_t {
    struct nala_state_t state;
    struct nala_data_time_t data;
    struct nala_instances_time_t instances;
};

static struct nala_mock_time_t nala_mock_time = {
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

struct nala_data_time_t *nala_get_data_time()
{
    if (nala_mock_time.instances.tail_p != NULL) {
        return (&nala_mock_time.instances.tail_p->data);
    } else {
        return (&nala_mock_time.data);
    }
}

struct nala_params_time_t *nala_get_params_time()
{
    return (&nala_get_data_time()->params);
}

time_t __wrap_time(time_t *tloc)
{
    struct nala_instance_time_t *nala_instance_p;
    struct nala_data_time_t *nala_data_p;
    time_t return_value;

    nala_print_call("time", &nala_mock_time.state);

    switch (nala_mock_time.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_time.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_time.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked time() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_time.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, time, tloc);
            nala_data_p->params_in.tloc = tloc;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       time,
                                       tloc);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(tloc);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_time(tloc);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_time.data.implementation(tloc);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_time.data.traceback,
                            "time");
        exit(1);
        break;

    default:
        return_value =
        __real_time(tloc);
        break;
    }

    return return_value;
}

void time_mock(time_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_time);
    nala_mock_time.state.mode = MODE_MOCK;
    nala_mock_time.data.params.tloc = NULL;
    nala_mock_time.data.params.ignore_tloc_in = true;
    nala_mock_time.data.return_value = return_value;
    nala_mock_time.data.errno_value = 0;
    nala_mock_time.data.callback = NULL;
    nala_traceback(&nala_mock_time.data.traceback);
}

int time_mock_once(time_t return_value)
{
    struct nala_instance_time_t *nala_instance_p;

    nala_mock_time.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.tloc_out);
    nala_set_param_init(&nala_instance_p->data.params.tloc_in);
    nala_instance_p->data.params.tloc_in_assert = NULL;
    nala_instance_p->data.params.tloc_out_copy = NULL;
    nala_instance_p->data.params.tloc = NULL;
    nala_instance_p->data.params.ignore_tloc_in = true;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_time.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void time_mock_ignore_in(time_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_time);
    nala_mock_time.state.mode = MODE_MOCK;
    nala_mock_time.data.params.ignore_tloc_in = true;
    nala_mock_time.data.return_value = return_value;
    nala_mock_time.data.errno_value = 0;
    nala_mock_time.data.callback = NULL;
}

int time_mock_ignore_in_once(time_t return_value)
{
    struct nala_instance_time_t *instance_p;

    nala_mock_time.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.tloc_out);
    nala_set_param_init(&instance_p->data.params.tloc_in);
    instance_p->data.params.tloc_in_assert = NULL;
    instance_p->data.params.tloc_out_copy = NULL;
    instance_p->data.params.tloc = NULL;
    instance_p->data.params.ignore_tloc_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_time.instances,
                          instance_p);

    return (instance_p->handle);
}

void time_mock_set_errno(int errno_value)
{
    nala_get_data_time()->errno_value = errno_value;
}

void time_mock_set_callback(void (*callback)(time_t *tloc))
{
    nala_get_data_time()->callback = callback;
}

struct nala_time_params_t *time_mock_get_params_in(int handle)
{
    struct nala_instance_time_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_time.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "time() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void time_mock_set_tloc_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_time()->tloc_in,
                       buf_p,
                       size);
}

void time_mock_set_tloc_in_assert(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_time_t *nala_params_p;

    nala_params_p = nala_get_params_time();

    if (nala_params_p->tloc_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "time_mock_set_tloc_in() must be called "
                "before time_mock_set_tloc_in_assert().\n"));
    }

    nala_params_p->tloc_in_assert = callback;
}

void time_mock_set_tloc_in_pointer(time_t *tloc)
{
    struct nala_params_time_t *nala_params_p;

    nala_params_p = nala_get_params_time();
    nala_params_p->ignore_tloc_in = false;
    nala_params_p->tloc = tloc;
}

void time_mock_set_tloc_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_time()->tloc_out,
                       buf_p,
                       size);
}

void time_mock_set_tloc_out_copy(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_time_t *nala_params_p;

    nala_params_p = nala_get_params_time();

    if (nala_params_p->tloc_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "time_mock_set_tloc_out() must be called "
                "before time_mock_set_tloc_out_copy().\n"));
    }

    nala_params_p->tloc_out_copy = callback;
}

void time_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_time);
    nala_mock_time.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_time.data.traceback);
}

void time_mock_implementation(time_t (*implementation)(time_t *tloc))
{
    CHECK_NO_INSTANCES(nala_mock_time);
    nala_mock_time.state.mode = MODE_IMPLEMENTATION;
    nala_mock_time.data.implementation = implementation;
    nala_traceback(&nala_mock_time.data.traceback);
}

void time_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_time);
    nala_mock_time.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_time.data.traceback);
}

void time_mock_real_once(void)
{
    struct nala_instance_time_t *instance_p;

    nala_mock_time.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_time.instances,
                          instance_p);
}

void time_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_time.state);
}

void time_mock_resume(void)
{
    nala_state_resume(&nala_mock_time.state);
}

void time_mock_reset(void)
{
    struct nala_instance_time_t *current_p;
    struct nala_instance_time_t *tmp_p;

    NALA_STATE_RESET(nala_mock_time, current_p, tmp_p);
}

void time_mock_assert_completed(void)
{
    if (nala_mock_time.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked time() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_time.instances.length),
                &nala_mock_time.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION timerfd_settime

int __real_timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

struct nala_params_timerfd_settime_t {
    int fd;
    int flags;
    const struct itimerspec *new_value;
    struct itimerspec *old_value;
    bool ignore_fd_in;
    bool ignore_flags_in;
    bool ignore_new_value_in;
    struct nala_set_param new_value_in;
    void (*new_value_in_assert)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param new_value_out;
    void (*new_value_out_copy)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size);
    bool ignore_old_value_in;
    struct nala_set_param old_value_in;
    void (*old_value_in_assert)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param old_value_out;
    void (*old_value_out_copy)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size);
};

struct nala_data_timerfd_settime_t {
    struct nala_params_timerfd_settime_t params;
    int return_value;
    int errno_value;
    int (*implementation)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
    void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
    struct nala_traceback_t traceback;
    struct nala_timerfd_settime_params_t params_in;
};

struct nala_instance_timerfd_settime_t {
    int mode;
    int handle;
    struct nala_data_timerfd_settime_t data;
    struct nala_instance_timerfd_settime_t *next_p;
};

struct nala_instances_timerfd_settime_t {
    struct nala_instance_timerfd_settime_t *head_p;
    struct nala_instance_timerfd_settime_t *next_p;
    struct nala_instance_timerfd_settime_t *tail_p;
    int length;
};

struct nala_mock_timerfd_settime_t {
    struct nala_state_t state;
    struct nala_data_timerfd_settime_t data;
    struct nala_instances_timerfd_settime_t instances;
};

static struct nala_mock_timerfd_settime_t nala_mock_timerfd_settime = {
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

struct nala_data_timerfd_settime_t *nala_get_data_timerfd_settime()
{
    if (nala_mock_timerfd_settime.instances.tail_p != NULL) {
        return (&nala_mock_timerfd_settime.instances.tail_p->data);
    } else {
        return (&nala_mock_timerfd_settime.data);
    }
}

struct nala_params_timerfd_settime_t *nala_get_params_timerfd_settime()
{
    return (&nala_get_data_timerfd_settime()->params);
}

int __wrap_timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value)
{
    struct nala_instance_timerfd_settime_t *nala_instance_p;
    struct nala_data_timerfd_settime_t *nala_data_p;
    int return_value;

    nala_print_call("timerfd_settime", &nala_mock_timerfd_settime.state);

    switch (nala_mock_timerfd_settime.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_timerfd_settime.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_timerfd_settime.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked timerfd_settime() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_timerfd_settime.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, timerfd_settime, new_value);
            nala_data_p->params_in.new_value = new_value;
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, timerfd_settime, old_value);
            nala_data_p->params_in.old_value = old_value;
            MOCK_ASSERT_IN_EQ(nala_data_p, timerfd_settime, fd);
            nala_data_p->params_in.fd = fd;
            MOCK_ASSERT_IN_EQ(nala_data_p, timerfd_settime, flags);
            nala_data_p->params_in.flags = flags;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_itimerspec,
                                       timerfd_settime,
                                       new_value);
            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_in_struct_itimerspec,
                                       timerfd_settime,
                                       old_value);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(fd, flags, new_value, old_value);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_timerfd_settime(fd, flags, new_value, old_value);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_timerfd_settime.data.implementation(fd, flags, new_value, old_value);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_timerfd_settime.data.traceback,
                            "timerfd_settime");
        exit(1);
        break;

    default:
        return_value =
        __real_timerfd_settime(fd, flags, new_value, old_value);
        break;
    }

    return return_value;
}

void timerfd_settime_mock(int fd, int flags, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_timerfd_settime);
    nala_mock_timerfd_settime.state.mode = MODE_MOCK;
    nala_mock_timerfd_settime.data.params.new_value = NULL;
    nala_mock_timerfd_settime.data.params.ignore_new_value_in = true;
    nala_mock_timerfd_settime.data.params.old_value = NULL;
    nala_mock_timerfd_settime.data.params.ignore_old_value_in = true;
    nala_mock_timerfd_settime.data.params.fd = fd;
    nala_mock_timerfd_settime.data.params.ignore_fd_in = false;
    nala_mock_timerfd_settime.data.params.flags = flags;
    nala_mock_timerfd_settime.data.params.ignore_flags_in = false;
    nala_mock_timerfd_settime.data.return_value = return_value;
    nala_mock_timerfd_settime.data.errno_value = 0;
    nala_mock_timerfd_settime.data.callback = NULL;
    nala_traceback(&nala_mock_timerfd_settime.data.traceback);
}

int timerfd_settime_mock_once(int fd, int flags, int return_value)
{
    struct nala_instance_timerfd_settime_t *nala_instance_p;

    nala_mock_timerfd_settime.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.new_value_out);
    nala_set_param_init(&nala_instance_p->data.params.new_value_in);
    nala_instance_p->data.params.new_value_in_assert = NULL;
    nala_instance_p->data.params.new_value_out_copy = NULL;
    nala_set_param_init(&nala_instance_p->data.params.old_value_out);
    nala_set_param_init(&nala_instance_p->data.params.old_value_in);
    nala_instance_p->data.params.old_value_in_assert = NULL;
    nala_instance_p->data.params.old_value_out_copy = NULL;
    nala_instance_p->data.params.new_value = NULL;
    nala_instance_p->data.params.ignore_new_value_in = true;
    nala_instance_p->data.params.old_value = NULL;
    nala_instance_p->data.params.ignore_old_value_in = true;
    nala_instance_p->data.params.fd = fd;
    nala_instance_p->data.params.ignore_fd_in = false;
    nala_instance_p->data.params.flags = flags;
    nala_instance_p->data.params.ignore_flags_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_timerfd_settime.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void timerfd_settime_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_timerfd_settime);
    nala_mock_timerfd_settime.state.mode = MODE_MOCK;
    nala_mock_timerfd_settime.data.params.ignore_new_value_in = true;
    nala_mock_timerfd_settime.data.params.ignore_old_value_in = true;
    nala_mock_timerfd_settime.data.params.ignore_fd_in = true;
    nala_mock_timerfd_settime.data.params.ignore_flags_in = true;
    nala_mock_timerfd_settime.data.return_value = return_value;
    nala_mock_timerfd_settime.data.errno_value = 0;
    nala_mock_timerfd_settime.data.callback = NULL;
}

int timerfd_settime_mock_ignore_in_once(int return_value)
{
    struct nala_instance_timerfd_settime_t *instance_p;

    nala_mock_timerfd_settime.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.new_value_out);
    nala_set_param_init(&instance_p->data.params.new_value_in);
    instance_p->data.params.new_value_in_assert = NULL;
    instance_p->data.params.new_value_out_copy = NULL;
    nala_set_param_init(&instance_p->data.params.old_value_out);
    nala_set_param_init(&instance_p->data.params.old_value_in);
    instance_p->data.params.old_value_in_assert = NULL;
    instance_p->data.params.old_value_out_copy = NULL;
    instance_p->data.params.new_value = NULL;
    instance_p->data.params.ignore_new_value_in = true;
    instance_p->data.params.old_value = NULL;
    instance_p->data.params.ignore_old_value_in = true;
    instance_p->data.params.ignore_fd_in = true;
    instance_p->data.params.ignore_flags_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_timerfd_settime.instances,
                          instance_p);

    return (instance_p->handle);
}

void timerfd_settime_mock_set_errno(int errno_value)
{
    nala_get_data_timerfd_settime()->errno_value = errno_value;
}

void timerfd_settime_mock_set_callback(void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value))
{
    nala_get_data_timerfd_settime()->callback = callback;
}

struct nala_timerfd_settime_params_t *timerfd_settime_mock_get_params_in(int handle)
{
    struct nala_instance_timerfd_settime_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_timerfd_settime.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "timerfd_settime() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void timerfd_settime_mock_ignore_fd_in(void)
{
    nala_get_params_timerfd_settime()->ignore_fd_in = true;
}

void timerfd_settime_mock_ignore_flags_in(void)
{
    nala_get_params_timerfd_settime()->ignore_flags_in = true;
}

void timerfd_settime_mock_set_new_value_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->new_value_in,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_new_value_in_assert(void (*callback)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();

    if (nala_params_p->new_value_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "timerfd_settime_mock_set_new_value_in() must be called "
                "before timerfd_settime_mock_set_new_value_in_assert().\n"));
    }

    nala_params_p->new_value_in_assert = callback;
}

void timerfd_settime_mock_set_new_value_in_pointer(const struct itimerspec *new_value)
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();
    nala_params_p->ignore_new_value_in = false;
    nala_params_p->new_value = new_value;
}

void timerfd_settime_mock_set_new_value_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->new_value_out,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_new_value_out_copy(void (*callback)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();

    if (nala_params_p->new_value_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "timerfd_settime_mock_set_new_value_out() must be called "
                "before timerfd_settime_mock_set_new_value_out_copy().\n"));
    }

    nala_params_p->new_value_out_copy = callback;
}

void timerfd_settime_mock_set_old_value_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->old_value_in,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_old_value_in_assert(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();

    if (nala_params_p->old_value_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "timerfd_settime_mock_set_old_value_in() must be called "
                "before timerfd_settime_mock_set_old_value_in_assert().\n"));
    }

    nala_params_p->old_value_in_assert = callback;
}

void timerfd_settime_mock_set_old_value_in_pointer(struct itimerspec *old_value)
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();
    nala_params_p->ignore_old_value_in = false;
    nala_params_p->old_value = old_value;
}

void timerfd_settime_mock_set_old_value_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->old_value_out,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_old_value_out_copy(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_timerfd_settime_t *nala_params_p;

    nala_params_p = nala_get_params_timerfd_settime();

    if (nala_params_p->old_value_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "timerfd_settime_mock_set_old_value_out() must be called "
                "before timerfd_settime_mock_set_old_value_out_copy().\n"));
    }

    nala_params_p->old_value_out_copy = callback;
}

void timerfd_settime_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_timerfd_settime);
    nala_mock_timerfd_settime.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_timerfd_settime.data.traceback);
}

void timerfd_settime_mock_implementation(int (*implementation)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value))
{
    CHECK_NO_INSTANCES(nala_mock_timerfd_settime);
    nala_mock_timerfd_settime.state.mode = MODE_IMPLEMENTATION;
    nala_mock_timerfd_settime.data.implementation = implementation;
    nala_traceback(&nala_mock_timerfd_settime.data.traceback);
}

void timerfd_settime_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_timerfd_settime);
    nala_mock_timerfd_settime.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_timerfd_settime.data.traceback);
}

void timerfd_settime_mock_real_once(void)
{
    struct nala_instance_timerfd_settime_t *instance_p;

    nala_mock_timerfd_settime.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_timerfd_settime.instances,
                          instance_p);
}

void timerfd_settime_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_timerfd_settime.state);
}

void timerfd_settime_mock_resume(void)
{
    nala_state_resume(&nala_mock_timerfd_settime.state);
}

void timerfd_settime_mock_reset(void)
{
    struct nala_instance_timerfd_settime_t *current_p;
    struct nala_instance_timerfd_settime_t *tmp_p;

    NALA_STATE_RESET(nala_mock_timerfd_settime, current_p, tmp_p);
}

void timerfd_settime_mock_assert_completed(void)
{
    if (nala_mock_timerfd_settime.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked timerfd_settime() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_timerfd_settime.instances.length),
                &nala_mock_timerfd_settime.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION tmpfile

FILE *__real_tmpfile(void);

struct nala_params_tmpfile_t {
    int dummy;
};

struct nala_data_tmpfile_t {
    struct nala_params_tmpfile_t params;
    FILE *return_value;
    int errno_value;
    FILE *(*implementation)(void);
    void (*callback)(void);
    struct nala_traceback_t traceback;
    struct nala_tmpfile_params_t params_in;
};

struct nala_instance_tmpfile_t {
    int mode;
    int handle;
    struct nala_data_tmpfile_t data;
    struct nala_instance_tmpfile_t *next_p;
};

struct nala_instances_tmpfile_t {
    struct nala_instance_tmpfile_t *head_p;
    struct nala_instance_tmpfile_t *next_p;
    struct nala_instance_tmpfile_t *tail_p;
    int length;
};

struct nala_mock_tmpfile_t {
    struct nala_state_t state;
    struct nala_data_tmpfile_t data;
    struct nala_instances_tmpfile_t instances;
};

static struct nala_mock_tmpfile_t nala_mock_tmpfile = {
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

struct nala_data_tmpfile_t *nala_get_data_tmpfile()
{
    if (nala_mock_tmpfile.instances.tail_p != NULL) {
        return (&nala_mock_tmpfile.instances.tail_p->data);
    } else {
        return (&nala_mock_tmpfile.data);
    }
}

struct nala_params_tmpfile_t *nala_get_params_tmpfile()
{
    return (&nala_get_data_tmpfile()->params);
}

FILE *__wrap_tmpfile(void)
{
    struct nala_instance_tmpfile_t *nala_instance_p;
    struct nala_data_tmpfile_t *nala_data_p;
    FILE *return_value;

    nala_print_call("tmpfile", &nala_mock_tmpfile.state);

    switch (nala_mock_tmpfile.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_tmpfile.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_tmpfile.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked tmpfile() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_tmpfile.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback();
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_tmpfile();
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_tmpfile.data.implementation();
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_tmpfile.data.traceback,
                            "tmpfile");
        exit(1);
        break;

    default:
        return_value =
        __real_tmpfile();
        break;
    }

    return return_value;
}

void tmpfile_mock(FILE *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_tmpfile);
    nala_mock_tmpfile.state.mode = MODE_MOCK;
    nala_mock_tmpfile.data.return_value = return_value;
    nala_mock_tmpfile.data.errno_value = 0;
    nala_mock_tmpfile.data.callback = NULL;
    nala_traceback(&nala_mock_tmpfile.data.traceback);
}

int tmpfile_mock_once(FILE *return_value)
{
    struct nala_instance_tmpfile_t *nala_instance_p;

    nala_mock_tmpfile.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_tmpfile.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void tmpfile_mock_ignore_in(FILE *return_value)
{
    CHECK_NO_INSTANCES(nala_mock_tmpfile);
    nala_mock_tmpfile.state.mode = MODE_MOCK;
    nala_mock_tmpfile.data.return_value = return_value;
    nala_mock_tmpfile.data.errno_value = 0;
    nala_mock_tmpfile.data.callback = NULL;
}

int tmpfile_mock_ignore_in_once(FILE *return_value)
{
    struct nala_instance_tmpfile_t *instance_p;

    nala_mock_tmpfile.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_tmpfile.instances,
                          instance_p);

    return (instance_p->handle);
}

void tmpfile_mock_set_errno(int errno_value)
{
    nala_get_data_tmpfile()->errno_value = errno_value;
}

void tmpfile_mock_set_callback(void (*callback)(void))
{
    nala_get_data_tmpfile()->callback = callback;
}

struct nala_tmpfile_params_t *tmpfile_mock_get_params_in(int handle)
{
    struct nala_instance_tmpfile_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_tmpfile.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "tmpfile() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void tmpfile_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_tmpfile);
    nala_mock_tmpfile.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_tmpfile.data.traceback);
}

void tmpfile_mock_implementation(FILE *(*implementation)(void))
{
    CHECK_NO_INSTANCES(nala_mock_tmpfile);
    nala_mock_tmpfile.state.mode = MODE_IMPLEMENTATION;
    nala_mock_tmpfile.data.implementation = implementation;
    nala_traceback(&nala_mock_tmpfile.data.traceback);
}

void tmpfile_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_tmpfile);
    nala_mock_tmpfile.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_tmpfile.data.traceback);
}

void tmpfile_mock_real_once(void)
{
    struct nala_instance_tmpfile_t *instance_p;

    nala_mock_tmpfile.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_tmpfile.instances,
                          instance_p);
}

void tmpfile_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_tmpfile.state);
}

void tmpfile_mock_resume(void)
{
    nala_state_resume(&nala_mock_tmpfile.state);
}

void tmpfile_mock_reset(void)
{
    struct nala_instance_tmpfile_t *current_p;
    struct nala_instance_tmpfile_t *tmp_p;

    NALA_STATE_RESET(nala_mock_tmpfile, current_p, tmp_p);
}

void tmpfile_mock_assert_completed(void)
{
    if (nala_mock_tmpfile.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked tmpfile() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_tmpfile.instances.length),
                &nala_mock_tmpfile.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION typedef_struct_param_and_return_type

struct_param_type __real_typedef_struct_param_and_return_type(struct_param_type arg);

struct nala_params_typedef_struct_param_and_return_type_t {
    struct_param_type arg;
};

struct nala_data_typedef_struct_param_and_return_type_t {
    struct nala_params_typedef_struct_param_and_return_type_t params;
    struct_param_type return_value;
    int errno_value;
    struct_param_type (*implementation)(struct_param_type arg);
    void (*callback)(struct_param_type arg);
    struct nala_traceback_t traceback;
    struct nala_typedef_struct_param_and_return_type_params_t params_in;
};

struct nala_instance_typedef_struct_param_and_return_type_t {
    int mode;
    int handle;
    struct nala_data_typedef_struct_param_and_return_type_t data;
    struct nala_instance_typedef_struct_param_and_return_type_t *next_p;
};

struct nala_instances_typedef_struct_param_and_return_type_t {
    struct nala_instance_typedef_struct_param_and_return_type_t *head_p;
    struct nala_instance_typedef_struct_param_and_return_type_t *next_p;
    struct nala_instance_typedef_struct_param_and_return_type_t *tail_p;
    int length;
};

struct nala_mock_typedef_struct_param_and_return_type_t {
    struct nala_state_t state;
    struct nala_data_typedef_struct_param_and_return_type_t data;
    struct nala_instances_typedef_struct_param_and_return_type_t instances;
};

static struct nala_mock_typedef_struct_param_and_return_type_t nala_mock_typedef_struct_param_and_return_type = {
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

struct nala_data_typedef_struct_param_and_return_type_t *nala_get_data_typedef_struct_param_and_return_type()
{
    if (nala_mock_typedef_struct_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_mock_typedef_struct_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_mock_typedef_struct_param_and_return_type.data);
    }
}

struct nala_params_typedef_struct_param_and_return_type_t *nala_get_params_typedef_struct_param_and_return_type()
{
    return (&nala_get_data_typedef_struct_param_and_return_type()->params);
}

struct_param_type __wrap_typedef_struct_param_and_return_type(struct_param_type arg)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *nala_instance_p;
    struct nala_data_typedef_struct_param_and_return_type_t *nala_data_p;
    struct_param_type return_value;

    nala_print_call("typedef_struct_param_and_return_type", &nala_mock_typedef_struct_param_and_return_type.state);

    switch (nala_mock_typedef_struct_param_and_return_type.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_typedef_struct_param_and_return_type.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_typedef_struct_param_and_return_type.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked typedef_struct_param_and_return_type() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_typedef_struct_param_and_return_type.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(arg);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_typedef_struct_param_and_return_type(arg);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_typedef_struct_param_and_return_type.data.implementation(arg);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_typedef_struct_param_and_return_type.data.traceback,
                            "typedef_struct_param_and_return_type");
        exit(1);
        break;

    default:
        return_value =
        __real_typedef_struct_param_and_return_type(arg);
        break;
    }

    return return_value;
}

void typedef_struct_param_and_return_type_mock(struct_param_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_struct_param_and_return_type);
    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_typedef_struct_param_and_return_type.data.return_value = return_value;
    nala_mock_typedef_struct_param_and_return_type.data.errno_value = 0;
    nala_mock_typedef_struct_param_and_return_type.data.callback = NULL;
    nala_traceback(&nala_mock_typedef_struct_param_and_return_type.data.traceback);
}

int typedef_struct_param_and_return_type_mock_once(struct_param_type return_value)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *nala_instance_p;

    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_typedef_struct_param_and_return_type.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void typedef_struct_param_and_return_type_mock_ignore_in(struct_param_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_struct_param_and_return_type);
    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_typedef_struct_param_and_return_type.data.return_value = return_value;
    nala_mock_typedef_struct_param_and_return_type.data.errno_value = 0;
    nala_mock_typedef_struct_param_and_return_type.data.callback = NULL;
}

int typedef_struct_param_and_return_type_mock_ignore_in_once(struct_param_type return_value)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *instance_p;

    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_typedef_struct_param_and_return_type.instances,
                          instance_p);

    return (instance_p->handle);
}

void typedef_struct_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_typedef_struct_param_and_return_type()->errno_value = errno_value;
}

void typedef_struct_param_and_return_type_mock_set_callback(void (*callback)(struct_param_type arg))
{
    nala_get_data_typedef_struct_param_and_return_type()->callback = callback;
}

struct nala_typedef_struct_param_and_return_type_params_t *typedef_struct_param_and_return_type_mock_get_params_in(int handle)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_typedef_struct_param_and_return_type.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "typedef_struct_param_and_return_type() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void typedef_struct_param_and_return_type_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_struct_param_and_return_type);
    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_typedef_struct_param_and_return_type.data.traceback);
}

void typedef_struct_param_and_return_type_mock_implementation(struct_param_type (*implementation)(struct_param_type arg))
{
    CHECK_NO_INSTANCES(nala_mock_typedef_struct_param_and_return_type);
    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_IMPLEMENTATION;
    nala_mock_typedef_struct_param_and_return_type.data.implementation = implementation;
    nala_traceback(&nala_mock_typedef_struct_param_and_return_type.data.traceback);
}

void typedef_struct_param_and_return_type_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_struct_param_and_return_type);
    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_typedef_struct_param_and_return_type.data.traceback);
}

void typedef_struct_param_and_return_type_mock_real_once(void)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *instance_p;

    nala_mock_typedef_struct_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_typedef_struct_param_and_return_type.instances,
                          instance_p);
}

void typedef_struct_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_typedef_struct_param_and_return_type.state);
}

void typedef_struct_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_mock_typedef_struct_param_and_return_type.state);
}

void typedef_struct_param_and_return_type_mock_reset(void)
{
    struct nala_instance_typedef_struct_param_and_return_type_t *current_p;
    struct nala_instance_typedef_struct_param_and_return_type_t *tmp_p;

    NALA_STATE_RESET(nala_mock_typedef_struct_param_and_return_type, current_p, tmp_p);
}

void typedef_struct_param_and_return_type_mock_assert_completed(void)
{
    if (nala_mock_typedef_struct_param_and_return_type.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked typedef_struct_param_and_return_type() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_typedef_struct_param_and_return_type.instances.length),
                &nala_mock_typedef_struct_param_and_return_type.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION typedef_union_param_and_return_type

union_type __real_typedef_union_param_and_return_type(union_type arg);

struct nala_params_typedef_union_param_and_return_type_t {
    union_type arg;
};

struct nala_data_typedef_union_param_and_return_type_t {
    struct nala_params_typedef_union_param_and_return_type_t params;
    union_type return_value;
    int errno_value;
    union_type (*implementation)(union_type arg);
    void (*callback)(union_type arg);
    struct nala_traceback_t traceback;
    struct nala_typedef_union_param_and_return_type_params_t params_in;
};

struct nala_instance_typedef_union_param_and_return_type_t {
    int mode;
    int handle;
    struct nala_data_typedef_union_param_and_return_type_t data;
    struct nala_instance_typedef_union_param_and_return_type_t *next_p;
};

struct nala_instances_typedef_union_param_and_return_type_t {
    struct nala_instance_typedef_union_param_and_return_type_t *head_p;
    struct nala_instance_typedef_union_param_and_return_type_t *next_p;
    struct nala_instance_typedef_union_param_and_return_type_t *tail_p;
    int length;
};

struct nala_mock_typedef_union_param_and_return_type_t {
    struct nala_state_t state;
    struct nala_data_typedef_union_param_and_return_type_t data;
    struct nala_instances_typedef_union_param_and_return_type_t instances;
};

static struct nala_mock_typedef_union_param_and_return_type_t nala_mock_typedef_union_param_and_return_type = {
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

struct nala_data_typedef_union_param_and_return_type_t *nala_get_data_typedef_union_param_and_return_type()
{
    if (nala_mock_typedef_union_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_mock_typedef_union_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_mock_typedef_union_param_and_return_type.data);
    }
}

struct nala_params_typedef_union_param_and_return_type_t *nala_get_params_typedef_union_param_and_return_type()
{
    return (&nala_get_data_typedef_union_param_and_return_type()->params);
}

union_type __wrap_typedef_union_param_and_return_type(union_type arg)
{
    struct nala_instance_typedef_union_param_and_return_type_t *nala_instance_p;
    struct nala_data_typedef_union_param_and_return_type_t *nala_data_p;
    union_type return_value;

    nala_print_call("typedef_union_param_and_return_type", &nala_mock_typedef_union_param_and_return_type.state);

    switch (nala_mock_typedef_union_param_and_return_type.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_typedef_union_param_and_return_type.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_typedef_union_param_and_return_type.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked typedef_union_param_and_return_type() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_typedef_union_param_and_return_type.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(arg);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_typedef_union_param_and_return_type(arg);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_typedef_union_param_and_return_type.data.implementation(arg);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_typedef_union_param_and_return_type.data.traceback,
                            "typedef_union_param_and_return_type");
        exit(1);
        break;

    default:
        return_value =
        __real_typedef_union_param_and_return_type(arg);
        break;
    }

    return return_value;
}

void typedef_union_param_and_return_type_mock(union_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_union_param_and_return_type);
    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_typedef_union_param_and_return_type.data.return_value = return_value;
    nala_mock_typedef_union_param_and_return_type.data.errno_value = 0;
    nala_mock_typedef_union_param_and_return_type.data.callback = NULL;
    nala_traceback(&nala_mock_typedef_union_param_and_return_type.data.traceback);
}

int typedef_union_param_and_return_type_mock_once(union_type return_value)
{
    struct nala_instance_typedef_union_param_and_return_type_t *nala_instance_p;

    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_typedef_union_param_and_return_type.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void typedef_union_param_and_return_type_mock_ignore_in(union_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_union_param_and_return_type);
    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_typedef_union_param_and_return_type.data.return_value = return_value;
    nala_mock_typedef_union_param_and_return_type.data.errno_value = 0;
    nala_mock_typedef_union_param_and_return_type.data.callback = NULL;
}

int typedef_union_param_and_return_type_mock_ignore_in_once(union_type return_value)
{
    struct nala_instance_typedef_union_param_and_return_type_t *instance_p;

    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_typedef_union_param_and_return_type.instances,
                          instance_p);

    return (instance_p->handle);
}

void typedef_union_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_typedef_union_param_and_return_type()->errno_value = errno_value;
}

void typedef_union_param_and_return_type_mock_set_callback(void (*callback)(union_type arg))
{
    nala_get_data_typedef_union_param_and_return_type()->callback = callback;
}

struct nala_typedef_union_param_and_return_type_params_t *typedef_union_param_and_return_type_mock_get_params_in(int handle)
{
    struct nala_instance_typedef_union_param_and_return_type_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_typedef_union_param_and_return_type.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "typedef_union_param_and_return_type() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void typedef_union_param_and_return_type_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_union_param_and_return_type);
    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_typedef_union_param_and_return_type.data.traceback);
}

void typedef_union_param_and_return_type_mock_implementation(union_type (*implementation)(union_type arg))
{
    CHECK_NO_INSTANCES(nala_mock_typedef_union_param_and_return_type);
    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_IMPLEMENTATION;
    nala_mock_typedef_union_param_and_return_type.data.implementation = implementation;
    nala_traceback(&nala_mock_typedef_union_param_and_return_type.data.traceback);
}

void typedef_union_param_and_return_type_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_typedef_union_param_and_return_type);
    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_typedef_union_param_and_return_type.data.traceback);
}

void typedef_union_param_and_return_type_mock_real_once(void)
{
    struct nala_instance_typedef_union_param_and_return_type_t *instance_p;

    nala_mock_typedef_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_typedef_union_param_and_return_type.instances,
                          instance_p);
}

void typedef_union_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_typedef_union_param_and_return_type.state);
}

void typedef_union_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_mock_typedef_union_param_and_return_type.state);
}

void typedef_union_param_and_return_type_mock_reset(void)
{
    struct nala_instance_typedef_union_param_and_return_type_t *current_p;
    struct nala_instance_typedef_union_param_and_return_type_t *tmp_p;

    NALA_STATE_RESET(nala_mock_typedef_union_param_and_return_type, current_p, tmp_p);
}

void typedef_union_param_and_return_type_mock_assert_completed(void)
{
    if (nala_mock_typedef_union_param_and_return_type.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked typedef_union_param_and_return_type() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_typedef_union_param_and_return_type.instances.length),
                &nala_mock_typedef_union_param_and_return_type.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION union_param_and_return_type

union union_type __real_union_param_and_return_type(union union_type arg);

struct nala_params_union_param_and_return_type_t {
    union union_type arg;
};

struct nala_data_union_param_and_return_type_t {
    struct nala_params_union_param_and_return_type_t params;
    union union_type return_value;
    int errno_value;
    union union_type (*implementation)(union union_type arg);
    void (*callback)(union union_type arg);
    struct nala_traceback_t traceback;
    struct nala_union_param_and_return_type_params_t params_in;
};

struct nala_instance_union_param_and_return_type_t {
    int mode;
    int handle;
    struct nala_data_union_param_and_return_type_t data;
    struct nala_instance_union_param_and_return_type_t *next_p;
};

struct nala_instances_union_param_and_return_type_t {
    struct nala_instance_union_param_and_return_type_t *head_p;
    struct nala_instance_union_param_and_return_type_t *next_p;
    struct nala_instance_union_param_and_return_type_t *tail_p;
    int length;
};

struct nala_mock_union_param_and_return_type_t {
    struct nala_state_t state;
    struct nala_data_union_param_and_return_type_t data;
    struct nala_instances_union_param_and_return_type_t instances;
};

static struct nala_mock_union_param_and_return_type_t nala_mock_union_param_and_return_type = {
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

struct nala_data_union_param_and_return_type_t *nala_get_data_union_param_and_return_type()
{
    if (nala_mock_union_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_mock_union_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_mock_union_param_and_return_type.data);
    }
}

struct nala_params_union_param_and_return_type_t *nala_get_params_union_param_and_return_type()
{
    return (&nala_get_data_union_param_and_return_type()->params);
}

union union_type __wrap_union_param_and_return_type(union union_type arg)
{
    struct nala_instance_union_param_and_return_type_t *nala_instance_p;
    struct nala_data_union_param_and_return_type_t *nala_data_p;
    union union_type return_value;

    nala_print_call("union_param_and_return_type", &nala_mock_union_param_and_return_type.state);

    switch (nala_mock_union_param_and_return_type.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_union_param_and_return_type.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_union_param_and_return_type.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked union_param_and_return_type() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_union_param_and_return_type.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(arg);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_union_param_and_return_type(arg);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_union_param_and_return_type.data.implementation(arg);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_union_param_and_return_type.data.traceback,
                            "union_param_and_return_type");
        exit(1);
        break;

    default:
        return_value =
        __real_union_param_and_return_type(arg);
        break;
    }

    return return_value;
}

void union_param_and_return_type_mock(union union_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_union_param_and_return_type);
    nala_mock_union_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_union_param_and_return_type.data.return_value = return_value;
    nala_mock_union_param_and_return_type.data.errno_value = 0;
    nala_mock_union_param_and_return_type.data.callback = NULL;
    nala_traceback(&nala_mock_union_param_and_return_type.data.traceback);
}

int union_param_and_return_type_mock_once(union union_type return_value)
{
    struct nala_instance_union_param_and_return_type_t *nala_instance_p;

    nala_mock_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_union_param_and_return_type.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void union_param_and_return_type_mock_ignore_in(union union_type return_value)
{
    CHECK_NO_INSTANCES(nala_mock_union_param_and_return_type);
    nala_mock_union_param_and_return_type.state.mode = MODE_MOCK;
    nala_mock_union_param_and_return_type.data.return_value = return_value;
    nala_mock_union_param_and_return_type.data.errno_value = 0;
    nala_mock_union_param_and_return_type.data.callback = NULL;
}

int union_param_and_return_type_mock_ignore_in_once(union union_type return_value)
{
    struct nala_instance_union_param_and_return_type_t *instance_p;

    nala_mock_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_union_param_and_return_type.instances,
                          instance_p);

    return (instance_p->handle);
}

void union_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_union_param_and_return_type()->errno_value = errno_value;
}

void union_param_and_return_type_mock_set_callback(void (*callback)(union union_type arg))
{
    nala_get_data_union_param_and_return_type()->callback = callback;
}

struct nala_union_param_and_return_type_params_t *union_param_and_return_type_mock_get_params_in(int handle)
{
    struct nala_instance_union_param_and_return_type_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_union_param_and_return_type.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "union_param_and_return_type() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void union_param_and_return_type_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_union_param_and_return_type);
    nala_mock_union_param_and_return_type.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_union_param_and_return_type.data.traceback);
}

void union_param_and_return_type_mock_implementation(union union_type (*implementation)(union union_type arg))
{
    CHECK_NO_INSTANCES(nala_mock_union_param_and_return_type);
    nala_mock_union_param_and_return_type.state.mode = MODE_IMPLEMENTATION;
    nala_mock_union_param_and_return_type.data.implementation = implementation;
    nala_traceback(&nala_mock_union_param_and_return_type.data.traceback);
}

void union_param_and_return_type_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_union_param_and_return_type);
    nala_mock_union_param_and_return_type.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_union_param_and_return_type.data.traceback);
}

void union_param_and_return_type_mock_real_once(void)
{
    struct nala_instance_union_param_and_return_type_t *instance_p;

    nala_mock_union_param_and_return_type.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_union_param_and_return_type.instances,
                          instance_p);
}

void union_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_union_param_and_return_type.state);
}

void union_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_mock_union_param_and_return_type.state);
}

void union_param_and_return_type_mock_reset(void)
{
    struct nala_instance_union_param_and_return_type_t *current_p;
    struct nala_instance_union_param_and_return_type_t *tmp_p;

    NALA_STATE_RESET(nala_mock_union_param_and_return_type, current_p, tmp_p);
}

void union_param_and_return_type_mock_assert_completed(void)
{
    if (nala_mock_union_param_and_return_type.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked union_param_and_return_type() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_union_param_and_return_type.instances.length),
                &nala_mock_union_param_and_return_type.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION usleep

int __real_usleep(__useconds_t usec);

struct nala_params_usleep_t {
    __useconds_t usec;
    bool ignore_usec_in;
};

struct nala_data_usleep_t {
    struct nala_params_usleep_t params;
    int return_value;
    int errno_value;
    int (*implementation)(__useconds_t usec);
    void (*callback)(__useconds_t usec);
    struct nala_traceback_t traceback;
    struct nala_usleep_params_t params_in;
};

struct nala_instance_usleep_t {
    int mode;
    int handle;
    struct nala_data_usleep_t data;
    struct nala_instance_usleep_t *next_p;
};

struct nala_instances_usleep_t {
    struct nala_instance_usleep_t *head_p;
    struct nala_instance_usleep_t *next_p;
    struct nala_instance_usleep_t *tail_p;
    int length;
};

struct nala_mock_usleep_t {
    struct nala_state_t state;
    struct nala_data_usleep_t data;
    struct nala_instances_usleep_t instances;
};

static struct nala_mock_usleep_t nala_mock_usleep = {
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

struct nala_data_usleep_t *nala_get_data_usleep()
{
    if (nala_mock_usleep.instances.tail_p != NULL) {
        return (&nala_mock_usleep.instances.tail_p->data);
    } else {
        return (&nala_mock_usleep.data);
    }
}

struct nala_params_usleep_t *nala_get_params_usleep()
{
    return (&nala_get_data_usleep()->params);
}

int __wrap_usleep(__useconds_t usec)
{
    struct nala_instance_usleep_t *nala_instance_p;
    struct nala_data_usleep_t *nala_data_p;
    int return_value;

    nala_print_call("usleep", &nala_mock_usleep.state);

    switch (nala_mock_usleep.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_usleep.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_usleep.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked usleep() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_usleep.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, usleep, usec);
            nala_data_p->params_in.usec = usec;


            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(usec);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_usleep(usec);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_usleep.data.implementation(usec);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_usleep.data.traceback,
                            "usleep");
        exit(1);
        break;

    default:
        return_value =
        __real_usleep(usec);
        break;
    }

    return return_value;
}

void usleep_mock(__useconds_t usec, int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_usleep);
    nala_mock_usleep.state.mode = MODE_MOCK;
    nala_mock_usleep.data.params.usec = usec;
    nala_mock_usleep.data.params.ignore_usec_in = false;
    nala_mock_usleep.data.return_value = return_value;
    nala_mock_usleep.data.errno_value = 0;
    nala_mock_usleep.data.callback = NULL;
    nala_traceback(&nala_mock_usleep.data.traceback);
}

int usleep_mock_once(__useconds_t usec, int return_value)
{
    struct nala_instance_usleep_t *nala_instance_p;

    nala_mock_usleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_instance_p->data.params.usec = usec;
    nala_instance_p->data.params.ignore_usec_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_usleep.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void usleep_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_usleep);
    nala_mock_usleep.state.mode = MODE_MOCK;
    nala_mock_usleep.data.params.ignore_usec_in = true;
    nala_mock_usleep.data.return_value = return_value;
    nala_mock_usleep.data.errno_value = 0;
    nala_mock_usleep.data.callback = NULL;
}

int usleep_mock_ignore_in_once(int return_value)
{
    struct nala_instance_usleep_t *instance_p;

    nala_mock_usleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    instance_p->data.params.ignore_usec_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_usleep.instances,
                          instance_p);

    return (instance_p->handle);
}

void usleep_mock_set_errno(int errno_value)
{
    nala_get_data_usleep()->errno_value = errno_value;
}

void usleep_mock_set_callback(void (*callback)(__useconds_t usec))
{
    nala_get_data_usleep()->callback = callback;
}

struct nala_usleep_params_t *usleep_mock_get_params_in(int handle)
{
    struct nala_instance_usleep_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_usleep.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "usleep() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void usleep_mock_ignore_usec_in(void)
{
    nala_get_params_usleep()->ignore_usec_in = true;
}

void usleep_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_usleep);
    nala_mock_usleep.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_usleep.data.traceback);
}

void usleep_mock_implementation(int (*implementation)(__useconds_t usec))
{
    CHECK_NO_INSTANCES(nala_mock_usleep);
    nala_mock_usleep.state.mode = MODE_IMPLEMENTATION;
    nala_mock_usleep.data.implementation = implementation;
    nala_traceback(&nala_mock_usleep.data.traceback);
}

void usleep_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_usleep);
    nala_mock_usleep.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_usleep.data.traceback);
}

void usleep_mock_real_once(void)
{
    struct nala_instance_usleep_t *instance_p;

    nala_mock_usleep.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_usleep.instances,
                          instance_p);
}

void usleep_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_usleep.state);
}

void usleep_mock_resume(void)
{
    nala_state_resume(&nala_mock_usleep.state);
}

void usleep_mock_reset(void)
{
    struct nala_instance_usleep_t *current_p;
    struct nala_instance_usleep_t *tmp_p;

    NALA_STATE_RESET(nala_mock_usleep, current_p, tmp_p);
}

void usleep_mock_assert_completed(void)
{
    if (nala_mock_usleep.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked usleep() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_usleep.instances.length),
                &nala_mock_usleep.instances.head_p->data.traceback));
    }
}

// NALA_IMPLEMENTATION write

ssize_t __real_write(int fd, const void *buf, size_t count);

struct nala_params_write_t {
    int fd;
    const void *buf;
    size_t count;
    bool ignore_fd_in;
    bool ignore_buf_in;
    struct nala_set_param buf_in;
    void (*buf_in_assert)(const void *buf, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_out;
    void (*buf_out_copy)(const void *buf, const void *nala_buf_p, size_t nala_size);
    bool ignore_count_in;
};

struct nala_data_write_t {
    struct nala_params_write_t params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int fd, const void *buf, size_t count);
    void (*callback)(int fd, const void *buf, size_t count);
    struct nala_traceback_t traceback;
    struct nala_write_params_t params_in;
};

struct nala_instance_write_t {
    int mode;
    int handle;
    struct nala_data_write_t data;
    struct nala_instance_write_t *next_p;
};

struct nala_instances_write_t {
    struct nala_instance_write_t *head_p;
    struct nala_instance_write_t *next_p;
    struct nala_instance_write_t *tail_p;
    int length;
};

struct nala_mock_write_t {
    struct nala_state_t state;
    struct nala_data_write_t data;
    struct nala_instances_write_t instances;
};

static struct nala_mock_write_t nala_mock_write = {
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

struct nala_data_write_t *nala_get_data_write()
{
    if (nala_mock_write.instances.tail_p != NULL) {
        return (&nala_mock_write.instances.tail_p->data);
    } else {
        return (&nala_mock_write.data);
    }
}

struct nala_params_write_t *nala_get_params_write()
{
    return (&nala_get_data_write()->params);
}

ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
    struct nala_instance_write_t *nala_instance_p;
    struct nala_data_write_t *nala_data_p;
    ssize_t return_value;

    nala_print_call("write", &nala_mock_write.state);

    switch (nala_mock_write.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_write.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_write.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked write() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_write.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_POINTERS_IN_EQ(nala_data_p, write, buf);
            nala_data_p->params_in.buf = buf;
            MOCK_ASSERT_IN_EQ(nala_data_p, write, fd);
            nala_data_p->params_in.fd = fd;
            MOCK_ASSERT_IN_EQ(nala_data_p, write, count);
            nala_data_p->params_in.count = count;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_memory,
                                       write,
                                       buf);

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                nala_data_p->callback(fd, buf, count);
            }

            return_value = nala_data_p->return_value;
        } else {
            return_value =
            __real_write(fd, buf, count);
        }
        break;

    case MODE_IMPLEMENTATION:
        return_value =
        nala_mock_write.data.implementation(fd, buf, count);
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_write.data.traceback,
                            "write");
        exit(1);
        break;

    default:
        return_value =
        __real_write(fd, buf, count);
        break;
    }

    return return_value;
}

void write_mock(int fd, size_t count, ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_write);
    nala_mock_write.state.mode = MODE_MOCK;
    nala_mock_write.data.params.buf = NULL;
    nala_mock_write.data.params.ignore_buf_in = true;
    nala_mock_write.data.params.fd = fd;
    nala_mock_write.data.params.ignore_fd_in = false;
    nala_mock_write.data.params.count = count;
    nala_mock_write.data.params.ignore_count_in = false;
    nala_mock_write.data.return_value = return_value;
    nala_mock_write.data.errno_value = 0;
    nala_mock_write.data.callback = NULL;
    nala_traceback(&nala_mock_write.data.traceback);
}

int write_mock_once(int fd, size_t count, ssize_t return_value)
{
    struct nala_instance_write_t *nala_instance_p;

    nala_mock_write.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.buf_out);
    nala_set_param_init(&nala_instance_p->data.params.buf_in);
    nala_instance_p->data.params.buf_in_assert = NULL;
    nala_instance_p->data.params.buf_out_copy = NULL;
    nala_instance_p->data.params.buf = NULL;
    nala_instance_p->data.params.ignore_buf_in = true;
    nala_instance_p->data.params.fd = fd;
    nala_instance_p->data.params.ignore_fd_in = false;
    nala_instance_p->data.params.count = count;
    nala_instance_p->data.params.ignore_count_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_write.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void write_mock_ignore_in(ssize_t return_value)
{
    CHECK_NO_INSTANCES(nala_mock_write);
    nala_mock_write.state.mode = MODE_MOCK;
    nala_mock_write.data.params.ignore_buf_in = true;
    nala_mock_write.data.params.ignore_fd_in = true;
    nala_mock_write.data.params.ignore_count_in = true;
    nala_mock_write.data.return_value = return_value;
    nala_mock_write.data.errno_value = 0;
    nala_mock_write.data.callback = NULL;
}

int write_mock_ignore_in_once(ssize_t return_value)
{
    struct nala_instance_write_t *instance_p;

    nala_mock_write.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.buf_out);
    nala_set_param_init(&instance_p->data.params.buf_in);
    instance_p->data.params.buf_in_assert = NULL;
    instance_p->data.params.buf_out_copy = NULL;
    instance_p->data.params.buf = NULL;
    instance_p->data.params.ignore_buf_in = true;
    instance_p->data.params.ignore_fd_in = true;
    instance_p->data.params.ignore_count_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_write.instances,
                          instance_p);

    return (instance_p->handle);
}

void write_mock_set_errno(int errno_value)
{
    nala_get_data_write()->errno_value = errno_value;
}

void write_mock_set_callback(void (*callback)(int fd, const void *buf, size_t count))
{
    nala_get_data_write()->callback = callback;
}

struct nala_write_params_t *write_mock_get_params_in(int handle)
{
    struct nala_instance_write_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_write.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "write() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void write_mock_ignore_fd_in(void)
{
    nala_get_params_write()->ignore_fd_in = true;
}

void write_mock_ignore_count_in(void)
{
    nala_get_params_write()->ignore_count_in = true;
}

void write_mock_set_buf_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_write()->buf_in,
                       buf_p,
                       size);
}

void write_mock_set_buf_in_assert(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_write_t *nala_params_p;

    nala_params_p = nala_get_params_write();

    if (nala_params_p->buf_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "write_mock_set_buf_in() must be called "
                "before write_mock_set_buf_in_assert().\n"));
    }

    nala_params_p->buf_in_assert = callback;
}

void write_mock_set_buf_in_pointer(const void *buf)
{
    struct nala_params_write_t *nala_params_p;

    nala_params_p = nala_get_params_write();
    nala_params_p->ignore_buf_in = false;
    nala_params_p->buf = buf;
}

void write_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_write()->buf_out,
                       buf_p,
                       size);
}

void write_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_write_t *nala_params_p;

    nala_params_p = nala_get_params_write();

    if (nala_params_p->buf_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "write_mock_set_buf_out() must be called "
                "before write_mock_set_buf_out_copy().\n"));
    }

    nala_params_p->buf_out_copy = callback;
}

void write_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_write);
    nala_mock_write.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_write.data.traceback);
}

void write_mock_implementation(ssize_t (*implementation)(int fd, const void *buf, size_t count))
{
    CHECK_NO_INSTANCES(nala_mock_write);
    nala_mock_write.state.mode = MODE_IMPLEMENTATION;
    nala_mock_write.data.implementation = implementation;
    nala_traceback(&nala_mock_write.data.traceback);
}

void write_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_write);
    nala_mock_write.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_write.data.traceback);
}

void write_mock_real_once(void)
{
    struct nala_instance_write_t *instance_p;

    nala_mock_write.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_write.instances,
                          instance_p);
}

void write_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_write.state);
}

void write_mock_resume(void)
{
    nala_state_resume(&nala_mock_write.state);
}

void write_mock_reset(void)
{
    struct nala_instance_write_t *current_p;
    struct nala_instance_write_t *tmp_p;

    NALA_STATE_RESET(nala_mock_write, current_p, tmp_p);
}

void write_mock_assert_completed(void)
{
    if (nala_mock_write.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked write() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_write.instances.length),
                &nala_mock_write.instances.head_p->data.traceback));
    }
}

