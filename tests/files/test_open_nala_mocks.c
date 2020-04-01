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

void nala_va_arg_list_init(struct nala_va_arg_list_t *self_p)
{
    self_p->head_p = NULL;
    self_p->tail_p = NULL;
    self_p->length = 0;
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

    nala_va_arg_list_init(list_p);

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

void nala_va_arg_list_assert_d(struct nala_va_arg_item_t *item_p,
                               int value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->d, value);
    }
}

void nala_va_arg_list_assert_u(struct nala_va_arg_item_t *item_p,
                               unsigned int value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->u, value);
    }
}

void nala_va_arg_list_assert_ld(struct nala_va_arg_item_t *item_p,
                                long value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->ld, value);
    }
}

void nala_va_arg_list_assert_lu(struct nala_va_arg_item_t *item_p,
                                unsigned long value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->lu, value);
    }
}

void nala_va_arg_list_assert_p(struct nala_va_arg_item_t *item_p,
                               void *value_p)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->p_p, value_p);
    }

    if (item_p->in.buf_p != NULL) {
        if (item_p->in_assert != NULL) {
            item_p->in_assert(value_p, item_p->in.buf_p, item_p->in.size);
        } else {
            ASSERT_MEMORY(value_p, item_p->in.buf_p, item_p->in.size);
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

void nala_va_arg_list_assert_s(struct nala_va_arg_item_t *item_p,
                               char *value_p)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->s_p, value_p);
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
            nala_va_arg_list_assert_d(item_p, va_arg(vl, int));
            break;

        case nala_va_arg_item_type_u_t:
            nala_va_arg_list_assert_u(item_p, va_arg(vl, unsigned int));
            break;

        case nala_va_arg_item_type_ld_t:
            nala_va_arg_list_assert_ld(item_p, va_arg(vl, long));
            break;

        case nala_va_arg_item_type_lu_t:
            nala_va_arg_list_assert_lu(item_p, va_arg(vl, unsigned long));
            break;

        case nala_va_arg_item_type_p_t:
            nala_va_arg_list_assert_p(item_p, va_arg(vl, void *));
            break;

        case nala_va_arg_item_type_s_t:
            nala_va_arg_list_assert_s(item_p, va_arg(vl, char *));
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

#define FORMAT_EQ(format, actual, expected)     \
    nala_format(format, (actual), (expected))

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

#define MOCK_ASSERT_IN_EQ(data_p, func, param)                          \
    if (!(data_p)->params.ignore_ ## param ## _in) {                    \
        if ((data_p)->params.param != param) {                          \
            nala_suspend_all_mocks();                                   \
            char _nala_assert_format[512];                              \
            snprintf(&_nala_assert_format[0],                           \
                     sizeof(_nala_assert_format),                       \
                     "Mocked " #func "(" #param "): %s != %s\n\n",      \
                     PRINT_FORMAT((data_p)->params.param),              \
                     PRINT_FORMAT(param));                              \
            nala_test_failure(                                          \
                format_mock_traceback(                                  \
                    nala_format(&_nala_assert_format[0],                \
                                (data_p)->params.param,                 \
                                param),                                 \
                    &(data_p)->traceback));                             \
        }                                                               \
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
    open_mock_suspend();
}

void nala_resume_all_mocks(void)
{
    open_mock_resume();
}

void nala_reset_all_mocks(void)
{
    open_mock_reset();
}

void nala_assert_all_mocks_completed(void)
{
    open_mock_assert_completed();
}

// NALA_IMPLEMENTATION open

struct nala_params_open_t {
    const char *pathname;
    int flags;
    const char *vafmt_p;
    bool ignore_pathname_in;
    struct nala_set_param pathname_in;
    void (*pathname_in_assert)(const char *pathname, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param pathname_out;
    void (*pathname_out_copy)(const char *pathname, const void *nala_buf_p, size_t nala_size);
    bool ignore_flags_in;
    struct nala_va_arg_list_t nala_va_arg_list;
};

struct nala_data_open_t {
    struct nala_params_open_t params;
    int return_value;
    int errno_value;
    int (*implementation)(const char *pathname, int flags, va_list __nala_va_list);
    void (*callback)(const char *pathname, int flags, va_list __nala_va_list);
    struct nala_traceback_t traceback;
    struct nala_open_params_t params_in;
};

struct nala_instance_open_t {
    int mode;
    int handle;
    struct nala_data_open_t data;
    struct nala_instance_open_t *next_p;
};

struct nala_instances_open_t {
    struct nala_instance_open_t *head_p;
    struct nala_instance_open_t *next_p;
    struct nala_instance_open_t *tail_p;
    int length;
};

struct nala_mock_open_t {
    struct nala_state_t state;
    struct nala_data_open_t data;
    struct nala_instances_open_t instances;
};

static struct nala_mock_open_t nala_mock_open = {
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

struct nala_data_open_t *nala_get_data_open()
{
    if (nala_mock_open.instances.tail_p != NULL) {
        return (&nala_mock_open.instances.tail_p->data);
    } else {
        return (&nala_mock_open.data);
    }
}

struct nala_params_open_t *nala_get_params_open()
{
    return (&nala_get_data_open()->params);
}

int __wrap_open(const char *pathname, int flags, ...)
{
    struct nala_instance_open_t *nala_instance_p;
    struct nala_data_open_t *nala_data_p;
    int return_value;

    nala_print_call("open", &nala_mock_open.state);

    switch (nala_mock_open.state.mode) {

    case MODE_MOCK_ONCE:
    case MODE_MOCK:
        if (nala_mock_open.state.mode == MODE_MOCK_ONCE) {
            NALA_INSTANCES_POP(nala_mock_open.instances, &nala_instance_p);

            if (nala_instance_p == NULL) {
                nala_test_failure(nala_format(
                        "Mocked open() called more times than expected.\n"));
            }

            nala_data_p = &nala_instance_p->data;
        } else {
            nala_instance_p = NULL;
            nala_data_p = &nala_mock_open.data;
        }

        if (nala_instance_p == NULL || nala_instance_p->mode == INSTANCE_MODE_NORMAL) {
            MOCK_ASSERT_IN_EQ(nala_data_p, open, pathname);
            nala_data_p->params_in.pathname = pathname;
            MOCK_ASSERT_IN_EQ(nala_data_p, open, flags);
            nala_data_p->params_in.flags = flags;

            MOCK_ASSERT_COPY_SET_PARAM(nala_instance_p,
                                       nala_data_p,
                                       nala_mock_assert_string,
                                       open,
                                       pathname);

            {
                va_list nala_vl;
                va_start(nala_vl, flags);
                nala_va_arg_list_assert(&nala_data_p->params.nala_va_arg_list, nala_vl);
                va_end(nala_vl);
                nala_va_arg_list_destroy(&nala_data_p->params.nala_va_arg_list);
            }

            errno = nala_data_p->errno_value;

            if (nala_data_p->callback != NULL) {
                va_list nala_vl;
                va_start(nala_vl, flags);
                nala_data_p->callback(pathname, flags, nala_vl);
                va_end(nala_vl);
            }

            return_value = nala_data_p->return_value;
        } else {
            nala_test_failure(nala_format("Real open() not available.\n"));
        }
        break;

    case MODE_IMPLEMENTATION:
        {
            va_list nala_vl;
            va_start(nala_vl, flags);
            return_value =
            nala_mock_open.data.implementation(pathname, flags, nala_vl);
            va_end(nala_vl);
        }
        break;

    case MODE_NONE:
        nala_mock_none_fail(&nala_mock_open.data.traceback,
                            "open");
        exit(1);
        break;

    default:
        nala_test_failure(nala_format("Real open() not available.\n"));
        break;
    }

    return return_value;
}

void open_mock(const char *pathname, int flags, int return_value, const char *vafmt_p, ...)
{
    CHECK_NO_INSTANCES(nala_mock_open);
    nala_mock_open.state.mode = MODE_MOCK;

    if (vafmt_p == NULL) {
        nala_test_failure(nala_format("Variadic format cannot be NULL.\n"));
    }

    nala_mock_open.data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&nala_mock_open.data.params.nala_va_arg_list);
    va_list nala_vl;
    va_start(nala_vl, vafmt_p);
    nala_parse_va_list(&nala_mock_open.data.params.nala_va_arg_list,
                       vafmt_p,
                       nala_vl);
    va_end(nala_vl);
    nala_mock_open.data.params.pathname = NULL;
    nala_mock_open.data.params.ignore_pathname_in = true;

    if (pathname != NULL) {
        nala_set_param_string(&nala_mock_open.data.params.pathname_in,
                              pathname);
    } else {
        nala_mock_open.data.params.ignore_pathname_in = false;
    }

    nala_mock_open.data.params.flags = flags;
    nala_mock_open.data.params.ignore_flags_in = false;
    nala_mock_open.data.return_value = return_value;
    nala_mock_open.data.errno_value = 0;
    nala_mock_open.data.callback = NULL;
    nala_traceback(&nala_mock_open.data.traceback);
}

int open_mock_once(const char *pathname, int flags, int return_value, const char *vafmt_p, ...)
{
    struct nala_instance_open_t *nala_instance_p;

    nala_mock_open.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(nala_instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&nala_instance_p->data.params.pathname_out);
    nala_set_param_init(&nala_instance_p->data.params.pathname_in);
    nala_instance_p->data.params.pathname_in_assert = NULL;
    nala_instance_p->data.params.pathname_out_copy = NULL;

    if (vafmt_p == NULL) {
        nala_test_failure(nala_format("Variadic format cannot be NULL.\n"));
    }

    nala_instance_p->data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&nala_instance_p->data.params.nala_va_arg_list);
    va_list nala_vl;
    va_start(nala_vl, vafmt_p);
    nala_parse_va_list(&nala_instance_p->data.params.nala_va_arg_list,
                       vafmt_p,
                       nala_vl);
    va_end(nala_vl);
    nala_instance_p->data.params.pathname = NULL;
    nala_instance_p->data.params.ignore_pathname_in = true;

    if (pathname != NULL) {
        nala_set_param_string(&nala_instance_p->data.params.pathname_in,
                              pathname);
    } else {
        nala_instance_p->data.params.ignore_pathname_in = false;
    }

    nala_instance_p->data.params.flags = flags;
    nala_instance_p->data.params.ignore_flags_in = false;
    nala_instance_p->data.return_value = return_value;
    nala_instance_p->data.errno_value = 0;
    nala_instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_open.instances,
                          nala_instance_p);

    return (nala_instance_p->handle);
}

void open_mock_ignore_in(int return_value)
{
    CHECK_NO_INSTANCES(nala_mock_open);
    nala_mock_open.state.mode = MODE_MOCK;
    nala_mock_open.data.params.ignore_pathname_in = true;
    nala_mock_open.data.params.ignore_flags_in = true;
    nala_mock_open.data.return_value = return_value;
    nala_mock_open.data.errno_value = 0;
    nala_mock_open.data.callback = NULL;
}

int open_mock_ignore_in_once(int return_value)
{
    struct nala_instance_open_t *instance_p;

    nala_mock_open.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_NORMAL);
    nala_set_param_init(&instance_p->data.params.pathname_out);
    nala_set_param_init(&instance_p->data.params.pathname_in);
    instance_p->data.params.pathname_in_assert = NULL;
    instance_p->data.params.pathname_out_copy = NULL;
    instance_p->data.params.vafmt_p = "";
    nala_va_arg_list_init(&instance_p->data.params.nala_va_arg_list);
    instance_p->data.params.pathname = NULL;
    instance_p->data.params.ignore_pathname_in = true;
    instance_p->data.params.ignore_flags_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    NALA_INSTANCES_APPEND(nala_mock_open.instances,
                          instance_p);

    return (instance_p->handle);
}

void open_mock_set_errno(int errno_value)
{
    nala_get_data_open()->errno_value = errno_value;
}

void open_mock_set_callback(void (*callback)(const char *pathname, int flags, va_list __nala_va_list))
{
    nala_get_data_open()->callback = callback;
}

struct nala_open_params_t *open_mock_get_params_in(int handle)
{
    struct nala_instance_open_t *instance_p;

    NALA_INSTANCES_FIND_USED(nala_mock_open.instances, &instance_p, handle);

    if (instance_p == NULL) {
        nala_test_failure(
            nala_format(
                "open() has not been called yet for given mock "
                "handle. No parameters available.\n"));
    }

    return (&instance_p->data.params_in);
}

void open_mock_ignore_pathname_in(void)
{
    nala_get_params_open()->ignore_pathname_in = true;
}

void open_mock_ignore_flags_in(void)
{
    nala_get_params_open()->ignore_flags_in = true;
}

void open_mock_set_pathname_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_open()->pathname_in,
                       buf_p,
                       size);
}

void open_mock_set_pathname_in_assert(void (*callback)(const char *pathname, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_open_t *nala_params_p;

    nala_params_p = nala_get_params_open();

    if (nala_params_p->pathname_in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "open_mock_set_pathname_in() must be called "
                "before open_mock_set_pathname_in_assert().\n"));
    }

    nala_params_p->pathname_in_assert = callback;
}

void open_mock_set_pathname_in_pointer(const char *pathname)
{
    struct nala_params_open_t *nala_params_p;

    nala_params_p = nala_get_params_open();
    nala_params_p->ignore_pathname_in = false;
    nala_params_p->pathname = pathname;
}

void open_mock_set_pathname_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_open()->pathname_out,
                       buf_p,
                       size);
}

void open_mock_set_pathname_out_copy(void (*callback)(const char *pathname, const void *nala_buf_p, size_t nala_size))
{
    struct nala_params_open_t *nala_params_p;

    nala_params_p = nala_get_params_open();

    if (nala_params_p->pathname_out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "open_mock_set_pathname_out() must be called "
                "before open_mock_set_pathname_out_copy().\n"));
    }

    nala_params_p->pathname_out_copy = callback;
}

void open_mock_ignore_va_arg_in_at(unsigned int index)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    nala_va_arg_list_get(va_arg_list_p, index)->ignore_in = true;
}

void open_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    nala_va_arg_list_set_param_buf_in_at(va_arg_list_p, index, buf_p, size);
}

void open_mock_set_va_arg_in_assert_at(unsigned int index, nala_mock_in_assert_t in_assert)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->in.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "open_mock_set_va_arg_in_at() must be called "
                "before open_mock_set_va_arg_in_assert_at().\n"));
    }

    item_p->in_assert = in_assert;
}

void open_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->type != nala_va_arg_item_type_p_t) {
        nala_test_failure(
            nala_format("Only variadic pointer parameters can be set."));

    }

    item_p->ignore_in = false;
    item_p->p_p = buf_p;
}

void open_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    nala_va_arg_list_set_param_buf_out_at(va_arg_list_p, index, buf_p, size);
}

void open_mock_set_va_arg_out_copy_at(unsigned int index, nala_mock_out_copy_t out_copy)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_open()->nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);

    if (item_p->out.buf_p == NULL) {
        nala_test_failure(
            nala_format(
                "open_mock_set_va_arg_out_at() must be called "
                "before open_mock_set_va_arg_out_copy_at().\n"));
    }

    item_p->out_copy = out_copy;
}

void open_mock_none(void)
{
    CHECK_NO_INSTANCES(nala_mock_open);
    nala_mock_open.state.mode = MODE_NONE;
    nala_traceback(&nala_mock_open.data.traceback);
}

void open_mock_implementation(int (*implementation)(const char *pathname, int flags, va_list __nala_va_list))
{
    CHECK_NO_INSTANCES(nala_mock_open);
    nala_mock_open.state.mode = MODE_IMPLEMENTATION;
    nala_mock_open.data.implementation = implementation;
    nala_traceback(&nala_mock_open.data.traceback);
}

void open_mock_real(void)
{
    CHECK_NO_INSTANCES(nala_mock_open);
    nala_mock_open.state.mode = MODE_REAL;
    nala_traceback(&nala_mock_open.data.traceback);
}

void open_mock_real_once(void)
{
    struct nala_instance_open_t *instance_p;

    nala_mock_open.state.mode = MODE_MOCK_ONCE;
    NALA_INSTANCE_NEW(instance_p, INSTANCE_MODE_REAL);
    NALA_INSTANCES_APPEND(nala_mock_open.instances,
                          instance_p);
}

void open_mock_suspend(void)
{
    nala_state_suspend(&nala_mock_open.state);
}

void open_mock_resume(void)
{
    nala_state_resume(&nala_mock_open.state);
}

void open_mock_reset(void)
{
    struct nala_instance_open_t *current_p;
    struct nala_instance_open_t *tmp_p;

    NALA_STATE_RESET(nala_mock_open, current_p, tmp_p);
}

void open_mock_assert_completed(void)
{
    if (nala_mock_open.instances.length != 0) {
        nala_test_failure(
            format_mock_traceback(
                nala_format(
                    "Mocked open() called fewer times than "
                    "expected. %d call(s) missing.\n\n",
                    nala_mock_open.instances.length),
                &nala_mock_open.instances.head_p->data.traceback));
    }
}
