/*
Mocks source file

Generated with Nala version 0.63.0 (https://github.com/eerimoq/nala)
Do not edit manually
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "nala.h"

#include <errno.h>
#include <stddef.h>

#include "nala_mocks.h"

#include <execinfo.h>

#define NALA_INSTANCES_APPEND(list, item_p)     \
    do {                                        \
        if ((list).head_p == NULL) {            \
            (list).head_p = item_p;             \
        } else {                                \
            (list).tail_p->next_p = item_p;     \
        }                                       \
                                                \
        (list).tail_p = item_p;                 \
        (list).length++;                        \
    } while (0);

#define NALA_INSTANCES_POP(list, instance_pp)           \
    do {                                                \
        *(instance_pp) = (list).head_p;                 \
                                                        \
        if (*(instance_pp) != NULL) {                   \
            (list).head_p = (*(instance_pp))->next_p;   \
                                                        \
            if ((*(instance_pp))->next_p == NULL) {     \
                (list).tail_p = NULL;                   \
            }                                           \
                                                        \
            (list).length--;                            \
        }                                               \
    } while (0);

#define NALA_STATE_SUSPEND(state)               \
    if ((state).suspended.count == 0) {         \
        (state).suspended.mode = (state).mode;  \
        (state).mode = 0;                       \
    }                                           \
    (state).suspended.count++;

#define NALA_STATE_RESUME(state)                 \
    (state).suspended.count--;                   \
    if ((state).suspended.count == 0) {          \
        (state).mode = (state).suspended.mode;   \
    }

#define NALA_STATE_RESET(state)                 \
    do {                                        \
        (state).mode = 0;                       \
        (state).instances.head_p = NULL;        \
        (state).instances.tail_p = NULL;        \
        (state).instances.length = 0;           \
    } while (0);

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
    nala_va_arg_item_type_p_t
};

struct nala_va_arg_item_t {
    enum nala_va_arg_item_type_t type;
    bool ignore_in;
    union {
        int d;
        unsigned int u;
        long ld;
        unsigned long lu;
        void *p_p;
    };
    struct nala_set_param in;
    struct nala_set_param out;
    struct nala_va_arg_item_t *next_p;
};

struct nala_va_arg_list_t {
    struct nala_va_arg_item_t *head_p;
    struct nala_va_arg_item_t *tail_p;
    unsigned int length;
};

struct nala_traceback_t {
    void *addresses[32];
    int depth;
};

struct nala_suspended_t {
    int count;
    int mode;
};

struct nala_instances_t {
    void *head_p;
    void *tail_p;
    int length;
};

struct nala_state_type_t {
    int mode;
    struct nala_suspended_t suspended;
    struct nala_instances_t instances;
};

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
        NALA_TEST_FAILURE(
            nala_format(
                "Trying to access variable argument at index %u when only %u exists.\n",
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
        NALA_TEST_FAILURE(
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

    default:
        nala_free(item_p);
        NALA_TEST_FAILURE(
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

    if (**format_pp == 'l') {
        (*format_pp)++;
        nala_parse_va_arg_long(format_pp, vl, item_p);
    } else {
        nala_parse_va_arg_non_long(format_pp, vl, item_p);
    }

    return (item_p);
}

int nala_parse_va_list(struct nala_va_arg_list_t *list_p,
                       const char *format_p,
                       va_list vl)
{
    int res;
    struct nala_va_arg_item_t *item_p;

    res = 1;
    nala_va_arg_list_init(list_p);

    while (res == 1) {
        if (*format_p == '\0') {
            res = 0;
        } else if (*format_p == '%') {
            format_p++;
            item_p = nala_parse_va_arg(&format_p, vl);

            if (item_p == NULL) {
                res = -1;
            } else {
                nala_va_arg_list_append(list_p, item_p);
            }
        } else {
            res = -1;
        }
    }

    return (res);
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
        ASSERT_MEMORY(value_p, item_p->in.buf_p, item_p->in.size);
    }

    if (item_p->out.buf_p != NULL) {
        memcpy(value_p, item_p->out.buf_p, item_p->out.size);
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

        default:
            NALA_TEST_FAILURE(nala_format("Nala internal failure.\n"));
            exit(1);
            break;
        }

        item_p = item_p->next_p;
    }
}

void nala_set_param_init(struct nala_set_param *self_p)
{
    self_p->buf_p = NULL;
    self_p->size = 0;
}

void nala_set_param_buf(struct nala_set_param *self_p, const void *buf_p, size_t size)
{
    self_p->buf_p = nala_xmalloc(size);
    self_p->size = size;
    memcpy(self_p->buf_p, buf_p, size);
}

void nala_set_param_string(struct nala_set_param *self_p, const char *string_p)
{
    nala_set_param_buf(self_p, string_p, strlen(string_p) + 1);
}

void nala_traceback(struct nala_traceback_t *traceback_p)
{
    traceback_p->depth = backtrace(&traceback_p->addresses[0], 32);
}

#define NALA_MOCK_BINARY_ASSERTION(data_p, param, check, format, formatter) \
    if (!(data_p)->params.ignore_ ## param ## _in) {                    \
        NALA_BINARY_ASSERTION((data_p)->params.param,                   \
                              param,                                    \
                              check,                                    \
                              format,                                   \
                              formatter);                               \
    }

#define MOCK_ASSERT_EQ(data_p, func, param)                             \
    NALA_MOCK_BINARY_ASSERTION(data_p,                                  \
                               param,                                   \
                               NALA_CHECK_EQ,                           \
                               "Mocked " #func "(" #param "): %s != %s\n", \
                               NALA_FORMAT_EQ)

#define MOCK_ASSERT_MEMORY(left, right, size, func, param)      \
    if (!nala_check_memory(left, right, size)) {                \
        nala_reset_all_mocks();                                 \
        NALA_TEST_FAILURE(nala_format_memory(                   \
                              "Mocked " #func "(" #param "): ", \
                              (left),                           \
                              (right),                          \
                              (size)));                         \
    }

#define MOCK_ASSERT_PARAM_IN(params_p, func, name)              \
    if ((params_p)->name ## _in_assert == NULL) {               \
        MOCK_ASSERT_MEMORY((const void *)(uintptr_t)name,       \
                           (params_p)->name ## _in.buf_p,       \
                           (params_p)->name ## _in.size,        \
                           func,                                \
                           name);                               \
    } else {                                                    \
        (params_p)->name ## _in_assert(                         \
            name,                                               \
            (params_p)->name ## _in.buf_p,                      \
            (params_p)->name ## _in.size);                      \
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

struct _nala_data_params_for_bar {
    int dummy;
};

struct _nala_data_type_for_bar {
    struct _nala_data_params_for_bar params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_bar {
    struct _nala_data_type_for_bar data;
    struct _nala_instance_type_for_bar *next_p;
};

struct _nala_instances_type_for_bar {
    struct _nala_instance_type_for_bar *head_p;
    struct _nala_instance_type_for_bar *tail_p;
    int length;
};

struct nala_state_type_for_bar {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_bar data;
    struct _nala_instances_type_for_bar instances;
};

static struct nala_state_type_for_bar nala_state_for_bar = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_bar *nala_get_data_bar()
{
    if (nala_state_for_bar.instances.tail_p != NULL) {
        return (&nala_state_for_bar.instances.tail_p->data);
    } else {
        return (&nala_state_for_bar.data);
    }
}

struct _nala_data_params_for_bar *nala_get_params_bar()
{
    return (&nala_get_data_bar()->params);
}

#define MOCK_ASSERT_bar(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_bar()
{
    struct _nala_instance_type_for_bar *_nala_instance_p;
    int return_value;

    switch (nala_state_for_bar.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_bar.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked bar() called more times than expected.\n"));
        }

        MOCK_ASSERT_bar(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_bar.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_bar(&nala_state_for_bar.data);
        return_value = nala_state_for_bar.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_bar.mode = 3;
    nala_state_for_bar.data.return_value = return_value;
    nala_state_for_bar.data.errno_value = 0;
    nala_state_for_bar.data.callback = NULL;
}

void bar_mock_once(int return_value)
{
    struct _nala_instance_type_for_bar *_nala_instance_p;

    nala_state_for_bar.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_bar.instances,
                          _nala_instance_p);
}

void bar_mock_ignore_in(int return_value)
{
    nala_state_for_bar.mode = 3;
    nala_state_for_bar.data.return_value = return_value;
    nala_state_for_bar.data.errno_value = 0;
    nala_state_for_bar.data.callback = NULL;
}

void bar_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_bar *instance_p;

    nala_state_for_bar.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_bar.instances,
                          instance_p);
}

void bar_mock_set_errno(int errno_value)
{
    nala_get_data_bar()->errno_value = errno_value;
}

void bar_mock_set_callback(void (*callback)())
{
    nala_get_data_bar()->callback = callback;
}

void bar_mock_none(void)
{
    nala_state_for_bar.mode = 4;
}

void bar_mock_implementation(int (*implementation)())
{
    nala_state_for_bar.mode = 2;
    nala_state_for_bar.data.implementation = implementation;
}

void bar_mock_disable(void)
{
    nala_state_for_bar.mode = 0;
}

void bar_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_bar);
}

void bar_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_bar);
}

void bar_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_bar);
}

void bar_mock_assert_completed(void)
{
    if (nala_state_for_bar.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked bar() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_bar.instances.length));
    }
}

// NALA_IMPLEMENTATION fie

int __real_fie();

struct _nala_data_params_for_fie {
    int dummy;
};

struct _nala_data_type_for_fie {
    struct _nala_data_params_for_fie params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fie {
    struct _nala_data_type_for_fie data;
    struct _nala_instance_type_for_fie *next_p;
};

struct _nala_instances_type_for_fie {
    struct _nala_instance_type_for_fie *head_p;
    struct _nala_instance_type_for_fie *tail_p;
    int length;
};

struct nala_state_type_for_fie {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_fie data;
    struct _nala_instances_type_for_fie instances;
};

static struct nala_state_type_for_fie nala_state_for_fie = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fie *nala_get_data_fie()
{
    if (nala_state_for_fie.instances.tail_p != NULL) {
        return (&nala_state_for_fie.instances.tail_p->data);
    } else {
        return (&nala_state_for_fie.data);
    }
}

struct _nala_data_params_for_fie *nala_get_params_fie()
{
    return (&nala_get_data_fie()->params);
}

#define MOCK_ASSERT_fie(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_fie()
{
    struct _nala_instance_type_for_fie *_nala_instance_p;
    int return_value;

    switch (nala_state_for_fie.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_fie.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked fie() called more times than expected.\n"));
        }

        MOCK_ASSERT_fie(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_fie.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_fie(&nala_state_for_fie.data);
        return_value = nala_state_for_fie.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_fie.mode = 3;
    nala_state_for_fie.data.return_value = return_value;
    nala_state_for_fie.data.errno_value = 0;
    nala_state_for_fie.data.callback = NULL;
}

void fie_mock_once(int return_value)
{
    struct _nala_instance_type_for_fie *_nala_instance_p;

    nala_state_for_fie.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fie.instances,
                          _nala_instance_p);
}

void fie_mock_ignore_in(int return_value)
{
    nala_state_for_fie.mode = 3;
    nala_state_for_fie.data.return_value = return_value;
    nala_state_for_fie.data.errno_value = 0;
    nala_state_for_fie.data.callback = NULL;
}

void fie_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fie *instance_p;

    nala_state_for_fie.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fie.instances,
                          instance_p);
}

void fie_mock_set_errno(int errno_value)
{
    nala_get_data_fie()->errno_value = errno_value;
}

void fie_mock_set_callback(void (*callback)())
{
    nala_get_data_fie()->callback = callback;
}

void fie_mock_none(void)
{
    nala_state_for_fie.mode = 4;
}

void fie_mock_implementation(int (*implementation)())
{
    nala_state_for_fie.mode = 2;
    nala_state_for_fie.data.implementation = implementation;
}

void fie_mock_disable(void)
{
    nala_state_for_fie.mode = 0;
}

void fie_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_fie);
}

void fie_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_fie);
}

void fie_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fie);
}

void fie_mock_assert_completed(void)
{
    if (nala_state_for_fie.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fie() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fie.instances.length));
    }
}

// NALA_IMPLEMENTATION foo

int __real_foo();

struct _nala_data_params_for_foo {
    int dummy;
};

struct _nala_data_type_for_foo {
    struct _nala_data_params_for_foo params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_foo {
    struct _nala_data_type_for_foo data;
    struct _nala_instance_type_for_foo *next_p;
};

struct _nala_instances_type_for_foo {
    struct _nala_instance_type_for_foo *head_p;
    struct _nala_instance_type_for_foo *tail_p;
    int length;
};

struct nala_state_type_for_foo {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_foo data;
    struct _nala_instances_type_for_foo instances;
};

static struct nala_state_type_for_foo nala_state_for_foo = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_foo *nala_get_data_foo()
{
    if (nala_state_for_foo.instances.tail_p != NULL) {
        return (&nala_state_for_foo.instances.tail_p->data);
    } else {
        return (&nala_state_for_foo.data);
    }
}

struct _nala_data_params_for_foo *nala_get_params_foo()
{
    return (&nala_get_data_foo()->params);
}

#define MOCK_ASSERT_foo(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_foo()
{
    struct _nala_instance_type_for_foo *_nala_instance_p;
    int return_value;

    switch (nala_state_for_foo.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_foo.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked foo() called more times than expected.\n"));
        }

        MOCK_ASSERT_foo(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_foo.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_foo(&nala_state_for_foo.data);
        return_value = nala_state_for_foo.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_foo.mode = 3;
    nala_state_for_foo.data.return_value = return_value;
    nala_state_for_foo.data.errno_value = 0;
    nala_state_for_foo.data.callback = NULL;
}

void foo_mock_once(int return_value)
{
    struct _nala_instance_type_for_foo *_nala_instance_p;

    nala_state_for_foo.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_foo.instances,
                          _nala_instance_p);
}

void foo_mock_ignore_in(int return_value)
{
    nala_state_for_foo.mode = 3;
    nala_state_for_foo.data.return_value = return_value;
    nala_state_for_foo.data.errno_value = 0;
    nala_state_for_foo.data.callback = NULL;
}

void foo_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_foo *instance_p;

    nala_state_for_foo.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_foo.instances,
                          instance_p);
}

void foo_mock_set_errno(int errno_value)
{
    nala_get_data_foo()->errno_value = errno_value;
}

void foo_mock_set_callback(void (*callback)())
{
    nala_get_data_foo()->callback = callback;
}

void foo_mock_none(void)
{
    nala_state_for_foo.mode = 4;
}

void foo_mock_implementation(int (*implementation)())
{
    nala_state_for_foo.mode = 2;
    nala_state_for_foo.data.implementation = implementation;
}

void foo_mock_disable(void)
{
    nala_state_for_foo.mode = 0;
}

void foo_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_foo);
}

void foo_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_foo);
}

void foo_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_foo);
}

void foo_mock_assert_completed(void)
{
    if (nala_state_for_foo.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked foo() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_foo.instances.length));
    }
}

// NALA_IMPLEMENTATION fum

int __real_fum();

struct _nala_data_params_for_fum {
    int dummy;
};

struct _nala_data_type_for_fum {
    struct _nala_data_params_for_fum params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fum {
    struct _nala_data_type_for_fum data;
    struct _nala_instance_type_for_fum *next_p;
};

struct _nala_instances_type_for_fum {
    struct _nala_instance_type_for_fum *head_p;
    struct _nala_instance_type_for_fum *tail_p;
    int length;
};

struct nala_state_type_for_fum {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_fum data;
    struct _nala_instances_type_for_fum instances;
};

static struct nala_state_type_for_fum nala_state_for_fum = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fum *nala_get_data_fum()
{
    if (nala_state_for_fum.instances.tail_p != NULL) {
        return (&nala_state_for_fum.instances.tail_p->data);
    } else {
        return (&nala_state_for_fum.data);
    }
}

struct _nala_data_params_for_fum *nala_get_params_fum()
{
    return (&nala_get_data_fum()->params);
}

#define MOCK_ASSERT_fum(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_fum()
{
    struct _nala_instance_type_for_fum *_nala_instance_p;
    int return_value;

    switch (nala_state_for_fum.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_fum.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked fum() called more times than expected.\n"));
        }

        MOCK_ASSERT_fum(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_fum.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_fum(&nala_state_for_fum.data);
        return_value = nala_state_for_fum.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_fum.mode = 3;
    nala_state_for_fum.data.return_value = return_value;
    nala_state_for_fum.data.errno_value = 0;
    nala_state_for_fum.data.callback = NULL;
}

void fum_mock_once(int return_value)
{
    struct _nala_instance_type_for_fum *_nala_instance_p;

    nala_state_for_fum.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fum.instances,
                          _nala_instance_p);
}

void fum_mock_ignore_in(int return_value)
{
    nala_state_for_fum.mode = 3;
    nala_state_for_fum.data.return_value = return_value;
    nala_state_for_fum.data.errno_value = 0;
    nala_state_for_fum.data.callback = NULL;
}

void fum_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fum *instance_p;

    nala_state_for_fum.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fum.instances,
                          instance_p);
}

void fum_mock_set_errno(int errno_value)
{
    nala_get_data_fum()->errno_value = errno_value;
}

void fum_mock_set_callback(void (*callback)())
{
    nala_get_data_fum()->callback = callback;
}

void fum_mock_none(void)
{
    nala_state_for_fum.mode = 4;
}

void fum_mock_implementation(int (*implementation)())
{
    nala_state_for_fum.mode = 2;
    nala_state_for_fum.data.implementation = implementation;
}

void fum_mock_disable(void)
{
    nala_state_for_fum.mode = 0;
}

void fum_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_fum);
}

void fum_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_fum);
}

void fum_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fum);
}

void fum_mock_assert_completed(void)
{
    if (nala_state_for_fum.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fum() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fum.instances.length));
    }
}

// NALA_IMPLEMENTATION gam

int __real_gam();

struct _nala_data_params_for_gam {
    int dummy;
};

struct _nala_data_type_for_gam {
    struct _nala_data_params_for_gam params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_gam {
    struct _nala_data_type_for_gam data;
    struct _nala_instance_type_for_gam *next_p;
};

struct _nala_instances_type_for_gam {
    struct _nala_instance_type_for_gam *head_p;
    struct _nala_instance_type_for_gam *tail_p;
    int length;
};

struct nala_state_type_for_gam {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_gam data;
    struct _nala_instances_type_for_gam instances;
};

static struct nala_state_type_for_gam nala_state_for_gam = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_gam *nala_get_data_gam()
{
    if (nala_state_for_gam.instances.tail_p != NULL) {
        return (&nala_state_for_gam.instances.tail_p->data);
    } else {
        return (&nala_state_for_gam.data);
    }
}

struct _nala_data_params_for_gam *nala_get_params_gam()
{
    return (&nala_get_data_gam()->params);
}

#define MOCK_ASSERT_gam(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_gam()
{
    struct _nala_instance_type_for_gam *_nala_instance_p;
    int return_value;

    switch (nala_state_for_gam.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_gam.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked gam() called more times than expected.\n"));
        }

        MOCK_ASSERT_gam(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_gam.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_gam(&nala_state_for_gam.data);
        return_value = nala_state_for_gam.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_gam.mode = 3;
    nala_state_for_gam.data.return_value = return_value;
    nala_state_for_gam.data.errno_value = 0;
    nala_state_for_gam.data.callback = NULL;
}

void gam_mock_once(int return_value)
{
    struct _nala_instance_type_for_gam *_nala_instance_p;

    nala_state_for_gam.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_gam.instances,
                          _nala_instance_p);
}

void gam_mock_ignore_in(int return_value)
{
    nala_state_for_gam.mode = 3;
    nala_state_for_gam.data.return_value = return_value;
    nala_state_for_gam.data.errno_value = 0;
    nala_state_for_gam.data.callback = NULL;
}

void gam_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_gam *instance_p;

    nala_state_for_gam.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_gam.instances,
                          instance_p);
}

void gam_mock_set_errno(int errno_value)
{
    nala_get_data_gam()->errno_value = errno_value;
}

void gam_mock_set_callback(void (*callback)())
{
    nala_get_data_gam()->callback = callback;
}

void gam_mock_none(void)
{
    nala_state_for_gam.mode = 4;
}

void gam_mock_implementation(int (*implementation)())
{
    nala_state_for_gam.mode = 2;
    nala_state_for_gam.data.implementation = implementation;
}

void gam_mock_disable(void)
{
    nala_state_for_gam.mode = 0;
}

void gam_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_gam);
}

void gam_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_gam);
}

void gam_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_gam);
}

void gam_mock_assert_completed(void)
{
    if (nala_state_for_gam.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked gam() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_gam.instances.length));
    }
}

// NALA_IMPLEMENTATION hit

int __real_hit();

struct _nala_data_params_for_hit {
    int dummy;
};

struct _nala_data_type_for_hit {
    struct _nala_data_params_for_hit params;
    int return_value;
    int errno_value;
    int (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_hit {
    struct _nala_data_type_for_hit data;
    struct _nala_instance_type_for_hit *next_p;
};

struct _nala_instances_type_for_hit {
    struct _nala_instance_type_for_hit *head_p;
    struct _nala_instance_type_for_hit *tail_p;
    int length;
};

struct nala_state_type_for_hit {
    int mode;
    struct nala_suspended_t suspended;
    struct _nala_data_type_for_hit data;
    struct _nala_instances_type_for_hit instances;
};

static struct nala_state_type_for_hit nala_state_for_hit = {
    .mode = 0,
    .suspended = {
        .count = 0,
        .mode = 0
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_hit *nala_get_data_hit()
{
    if (nala_state_for_hit.instances.tail_p != NULL) {
        return (&nala_state_for_hit.instances.tail_p->data);
    } else {
        return (&nala_state_for_hit.data);
    }
}

struct _nala_data_params_for_hit *nala_get_params_hit()
{
    return (&nala_get_data_hit()->params);
}

#define MOCK_ASSERT_hit(_nala_data_p) \
 \
    errno = (_nala_data_p)->errno_value; \
 \
    if ((_nala_data_p)->callback != NULL) { \
        (_nala_data_p)->callback(); \
    }

int __wrap_hit()
{
    struct _nala_instance_type_for_hit *_nala_instance_p;
    int return_value;

    switch (nala_state_for_hit.mode) {

    case 1:
        NALA_INSTANCES_POP(nala_state_for_hit.instances, &_nala_instance_p);

        if (_nala_instance_p == NULL) {
            NALA_TEST_FAILURE(nala_format(
                    "Mocked hit() called more times than expected.\n"));
        }

        MOCK_ASSERT_hit(&_nala_instance_p->data);
        return_value = _nala_instance_p->data.return_value;
        nala_free(_nala_instance_p);
        break;

    case 2:
        return_value =
        nala_state_for_hit.data.implementation();
        break;

    case 3:
        MOCK_ASSERT_hit(&nala_state_for_hit.data);
        return_value = nala_state_for_hit.data.return_value;
        break;

    case 4:
        FAIL();
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
    nala_state_for_hit.mode = 3;
    nala_state_for_hit.data.return_value = return_value;
    nala_state_for_hit.data.errno_value = 0;
    nala_state_for_hit.data.callback = NULL;
}

void hit_mock_once(int return_value)
{
    struct _nala_instance_type_for_hit *_nala_instance_p;

    nala_state_for_hit.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_hit.instances,
                          _nala_instance_p);
}

void hit_mock_ignore_in(int return_value)
{
    nala_state_for_hit.mode = 3;
    nala_state_for_hit.data.return_value = return_value;
    nala_state_for_hit.data.errno_value = 0;
    nala_state_for_hit.data.callback = NULL;
}

void hit_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_hit *instance_p;

    nala_state_for_hit.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_hit.instances,
                          instance_p);
}

void hit_mock_set_errno(int errno_value)
{
    nala_get_data_hit()->errno_value = errno_value;
}

void hit_mock_set_callback(void (*callback)())
{
    nala_get_data_hit()->callback = callback;
}

void hit_mock_none(void)
{
    nala_state_for_hit.mode = 4;
}

void hit_mock_implementation(int (*implementation)())
{
    nala_state_for_hit.mode = 2;
    nala_state_for_hit.data.implementation = implementation;
}

void hit_mock_disable(void)
{
    nala_state_for_hit.mode = 0;
}

void hit_mock_suspend(void)
{
    NALA_STATE_SUSPEND(nala_state_for_hit);
}

void hit_mock_resume(void)
{
    NALA_STATE_RESUME(nala_state_for_hit);
}

void hit_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_hit);
}

void hit_mock_assert_completed(void)
{
    if (nala_state_for_hit.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked hit() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_hit.instances.length));
    }
}
