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

#define NALA_STATE_RESET(_state)                \
    (_state).state.mode = 0;                    \
    (_state).state.suspended.count = 0;         \
    (_state).instances.head_p = NULL;           \
    (_state).instances.tail_p = NULL;           \
    (_state).instances.length = 0;

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

struct nala_suspended_t {
    int count;
    int mode;
};

struct nala_state_t {
    int mode;
    struct nala_suspended_t suspended;
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
            nala_test_failure(nala_format("Nala internal failure.\n"));
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

char *format_mock_traceback(const char *message_p,
                            struct nala_traceback_t *traceback_p)
{
    FILE *file_p;
    char *buf_p;
    size_t file_size;
    char *formatted_traceback_p;

    formatted_traceback_p = nala_mock_traceback_format(
        &traceback_p->addresses[0],
        traceback_p->depth);
    file_p = open_memstream(&buf_p, &file_size);
    fprintf(file_p, "%s%s", message_p, formatted_traceback_p);
    fputc('\0', file_p);
    fclose(file_p);
    free((void *)message_p);
    free(formatted_traceback_p);

    return (buf_p);
}

#define MOCK_ASSERT_IN_EQ(data_p, func, param)                          \
    if (!(data_p)->params.ignore_ ## param ## _in) {                    \
        if (!NALA_CHECK_EQ((data_p)->params.param, param)) {            \
            nala_reset_all_mocks();                                     \
            char _nala_assert_format[512];                              \
            snprintf(&_nala_assert_format[0],                           \
                     sizeof(_nala_assert_format),                       \
                     "Mocked " #func "(" #param "): %s != %s\n\n",      \
                     NALA_PRINT_FORMAT((data_p)->params.param),         \
                     NALA_PRINT_FORMAT(param));                         \
            nala_test_failure(                                          \
                format_mock_traceback(                                  \
                    NALA_FORMAT_EQ(&_nala_assert_format[0],             \
                                   (data_p)->params.param,              \
                                   param),                              \
                    &(data_p)->traceback));                             \
        }                                                               \
    }

#define MOCK_ASSERT_PARAM_IN_EQ(format_p, left, right)  \
    NALA_BINARY_ASSERTION(left,                         \
                          right,                        \
                          NALA_CHECK_EQ,                \
                          format_p,                     \
                          NALA_FORMAT_EQ);

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
        nala_reset_all_mocks();
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

void nala_state_suspend(struct nala_state_t *state_p)
{
    if (state_p->suspended.count == 0) {
        state_p->suspended.mode = state_p->mode;
        state_p->mode = 0;
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

void nala_mock_none_fail()
{
    FAIL();
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

    case 0:
        mode_p = "real";
        break;

    case 1:
        mode_p = "once";
        break;

    case 2:
        mode_p = "impl";
        break;

    case 3:
        mode_p = "mock";
        break;

    case 4:
        mode_p = "none";
        break;

    default:
        mode_p = "unknown";
        break;
    }

    printf("%s: %s()\n", mode_p, function_name_p);
}

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

struct _nala_data_params_for_add {
    int x;
    int y;
    bool ignore_x_in;
    bool ignore_y_in;
};

struct _nala_data_type_for_add {
    struct _nala_data_params_for_add params;
    int return_value;
    int errno_value;
    int (*implementation)(int x, int y);
    void (*callback)(int x, int y);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_add {
    struct _nala_data_type_for_add data;
    struct _nala_instance_type_for_add *next_p;
};

struct _nala_instances_type_for_add {
    struct _nala_instance_type_for_add *head_p;
    struct _nala_instance_type_for_add *tail_p;
    int length;
};

struct nala_state_type_for_add {
    struct nala_state_t state;
    struct _nala_data_type_for_add data;
    struct _nala_instances_type_for_add instances;
};

static struct nala_state_type_for_add nala_state_for_add = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_add *nala_get_data_add()
{
    if (nala_state_for_add.instances.tail_p != NULL) {
        return (&nala_state_for_add.instances.tail_p->data);
    } else {
        return (&nala_state_for_add.data);
    }
}

struct _nala_data_params_for_add *nala_get_params_add()
{
    return (&nala_get_data_add()->params);
}

int __wrap_add(int x, int y)
{
    struct _nala_instance_type_for_add *_nala_instance_p;
    struct _nala_data_type_for_add *_nala_data_p;
    int return_value;

    nala_print_call("add", &nala_state_for_add.state);

    switch (nala_state_for_add.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_add.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_add.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked add() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_add.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, add, x);
        MOCK_ASSERT_IN_EQ(_nala_data_p, add, y);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(x, y);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_add.data.implementation(x, y);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_add.state.mode = 3;
    nala_state_for_add.data.params.x = x;
    nala_state_for_add.data.params.ignore_x_in = false;
    nala_state_for_add.data.params.y = y;
    nala_state_for_add.data.params.ignore_y_in = false;
    nala_state_for_add.data.return_value = return_value;
    nala_state_for_add.data.errno_value = 0;
    nala_state_for_add.data.callback = NULL;
}

void add_mock_once(int x, int y, int return_value)
{
    struct _nala_instance_type_for_add *_nala_instance_p;

    nala_state_for_add.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.x = x;
    _nala_instance_p->data.params.ignore_x_in = false;
    _nala_instance_p->data.params.y = y;
    _nala_instance_p->data.params.ignore_y_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_add.instances,
                          _nala_instance_p);
}

void add_mock_ignore_in(int return_value)
{
    nala_state_for_add.state.mode = 3;
    nala_state_for_add.data.params.ignore_x_in = true;
    nala_state_for_add.data.params.ignore_y_in = true;
    nala_state_for_add.data.return_value = return_value;
    nala_state_for_add.data.errno_value = 0;
    nala_state_for_add.data.callback = NULL;
}

void add_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_add *instance_p;

    nala_state_for_add.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_x_in = true;
    instance_p->data.params.ignore_y_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_add.instances,
                          instance_p);
}

void add_mock_set_errno(int errno_value)
{
    nala_get_data_add()->errno_value = errno_value;
}

void add_mock_set_callback(void (*callback)(int x, int y))
{
    nala_get_data_add()->callback = callback;
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
    nala_state_for_add.state.mode = 4;
}

void add_mock_implementation(int (*implementation)(int x, int y))
{
    nala_state_for_add.state.mode = 2;
    nala_state_for_add.data.implementation = implementation;
}

void add_mock_disable(void)
{
    nala_state_for_add.state.mode = 0;
}

void add_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_add.state);
}

void add_mock_resume(void)
{
    nala_state_resume(&nala_state_for_add.state);
}

void add_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_add);
}

void add_mock_assert_completed(void)
{
    if (nala_state_for_add.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked add() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_add.instances.length));
    }
}

// NALA_IMPLEMENTATION call

int __real_call(int (*callback)(int value));

struct _nala_data_params_for_call {
    int (*callback)(int value);
    bool ignore_callback_in;
    struct nala_set_param callback_in;
    void (*callback_in_assert)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size);
    struct nala_set_param callback_out;
    void (*callback_out_copy)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_call {
    struct _nala_data_params_for_call params;
    int return_value;
    int errno_value;
    int (*implementation)(int (*callback)(int value));
    void (*callback)(int (*callback)(int value));
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_call {
    struct _nala_data_type_for_call data;
    struct _nala_instance_type_for_call *next_p;
};

struct _nala_instances_type_for_call {
    struct _nala_instance_type_for_call *head_p;
    struct _nala_instance_type_for_call *tail_p;
    int length;
};

struct nala_state_type_for_call {
    struct nala_state_t state;
    struct _nala_data_type_for_call data;
    struct _nala_instances_type_for_call instances;
};

static struct nala_state_type_for_call nala_state_for_call = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_call *nala_get_data_call()
{
    if (nala_state_for_call.instances.tail_p != NULL) {
        return (&nala_state_for_call.instances.tail_p->data);
    } else {
        return (&nala_state_for_call.data);
    }
}

struct _nala_data_params_for_call *nala_get_params_call()
{
    return (&nala_get_data_call()->params);
}

int __wrap_call(int (*callback)(int value))
{
    struct _nala_instance_type_for_call *_nala_instance_p;
    struct _nala_data_type_for_call *_nala_data_p;
    int return_value;

    nala_print_call("call", &nala_state_for_call.state);

    switch (nala_state_for_call.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_call.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_call.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked call() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_call.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, call, callback);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   call,
                                   callback);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(callback);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_call.data.implementation(callback);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_call.state.mode = 3;
    nala_state_for_call.data.params.callback = NULL;
    nala_state_for_call.data.params.ignore_callback_in = true;
    nala_state_for_call.data.return_value = return_value;
    nala_state_for_call.data.errno_value = 0;
    nala_state_for_call.data.callback = NULL;
}

void call_mock_once(int return_value)
{
    struct _nala_instance_type_for_call *_nala_instance_p;

    nala_state_for_call.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.callback_out);
    nala_set_param_init(&_nala_instance_p->data.params.callback_in);
    _nala_instance_p->data.params.callback_in_assert = NULL;
    _nala_instance_p->data.params.callback_out_copy = NULL;
    _nala_instance_p->data.params.callback = NULL;
    _nala_instance_p->data.params.ignore_callback_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_call.instances,
                          _nala_instance_p);
}

void call_mock_ignore_in(int return_value)
{
    nala_state_for_call.state.mode = 3;
    nala_state_for_call.data.params.ignore_callback_in = true;
    nala_state_for_call.data.return_value = return_value;
    nala_state_for_call.data.errno_value = 0;
    nala_state_for_call.data.callback = NULL;
}

void call_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_call *instance_p;

    nala_state_for_call.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.callback_out);
    nala_set_param_init(&instance_p->data.params.callback_in);
    instance_p->data.params.callback_in_assert = NULL;
    instance_p->data.params.callback_out_copy = NULL;
    instance_p->data.params.callback = NULL;
    instance_p->data.params.ignore_callback_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_call.instances,
                          instance_p);
}

void call_mock_set_errno(int errno_value)
{
    nala_get_data_call()->errno_value = errno_value;
}

void call_mock_set_callback(void (*callback)(int (*callback)(int value)))
{
    nala_get_data_call()->callback = callback;
}

void call_mock_set_callback_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_call()->callback_in,
                       buf_p,
                       size);
}

void call_mock_set_callback_in_assert(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_call()->callback_in_assert = callback;
}

void call_mock_set_callback_in_pointer(int (*callback)(int value))
{
    struct _nala_data_params_for_call *_nala_params_p;

    _nala_params_p = nala_get_params_call();
    _nala_params_p->ignore_callback_in = false;
    _nala_params_p->callback = callback;
}

void call_mock_set_callback_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_call()->callback_out,
                       buf_p,
                       size);
}

void call_mock_set_callback_out_copy(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_call()->callback_out_copy = callback;
}

void call_mock_none(void)
{
    nala_state_for_call.state.mode = 4;
}

void call_mock_implementation(int (*implementation)(int (*callback)(int value)))
{
    nala_state_for_call.state.mode = 2;
    nala_state_for_call.data.implementation = implementation;
}

void call_mock_disable(void)
{
    nala_state_for_call.state.mode = 0;
}

void call_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_call.state);
}

void call_mock_resume(void)
{
    nala_state_resume(&nala_state_for_call.state);
}

void call_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_call);
}

void call_mock_assert_completed(void)
{
    if (nala_state_for_call.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked call() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_call.instances.length));
    }
}

// NALA_IMPLEMENTATION close

int __real_close(int fd);

struct _nala_data_params_for_close {
    int fd;
    bool ignore_fd_in;
};

struct _nala_data_type_for_close {
    struct _nala_data_params_for_close params;
    int return_value;
    int errno_value;
    int (*implementation)(int fd);
    void (*callback)(int fd);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_close {
    struct _nala_data_type_for_close data;
    struct _nala_instance_type_for_close *next_p;
};

struct _nala_instances_type_for_close {
    struct _nala_instance_type_for_close *head_p;
    struct _nala_instance_type_for_close *tail_p;
    int length;
};

struct nala_state_type_for_close {
    struct nala_state_t state;
    struct _nala_data_type_for_close data;
    struct _nala_instances_type_for_close instances;
};

static struct nala_state_type_for_close nala_state_for_close = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_close *nala_get_data_close()
{
    if (nala_state_for_close.instances.tail_p != NULL) {
        return (&nala_state_for_close.instances.tail_p->data);
    } else {
        return (&nala_state_for_close.data);
    }
}

struct _nala_data_params_for_close *nala_get_params_close()
{
    return (&nala_get_data_close()->params);
}

int __wrap_close(int fd)
{
    struct _nala_instance_type_for_close *_nala_instance_p;
    struct _nala_data_type_for_close *_nala_data_p;
    int return_value;

    nala_print_call("close", &nala_state_for_close.state);

    switch (nala_state_for_close.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_close.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_close.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked close() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_close.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, close, fd);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(fd);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_close.data.implementation(fd);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_close.state.mode = 3;
    nala_state_for_close.data.params.fd = fd;
    nala_state_for_close.data.params.ignore_fd_in = false;
    nala_state_for_close.data.return_value = return_value;
    nala_state_for_close.data.errno_value = 0;
    nala_state_for_close.data.callback = NULL;
}

void close_mock_once(int fd, int return_value)
{
    struct _nala_instance_type_for_close *_nala_instance_p;

    nala_state_for_close.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.fd = fd;
    _nala_instance_p->data.params.ignore_fd_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_close.instances,
                          _nala_instance_p);
}

void close_mock_ignore_in(int return_value)
{
    nala_state_for_close.state.mode = 3;
    nala_state_for_close.data.params.ignore_fd_in = true;
    nala_state_for_close.data.return_value = return_value;
    nala_state_for_close.data.errno_value = 0;
    nala_state_for_close.data.callback = NULL;
}

void close_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_close *instance_p;

    nala_state_for_close.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_fd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_close.instances,
                          instance_p);
}

void close_mock_set_errno(int errno_value)
{
    nala_get_data_close()->errno_value = errno_value;
}

void close_mock_set_callback(void (*callback)(int fd))
{
    nala_get_data_close()->callback = callback;
}

void close_mock_ignore_fd_in(void)
{
    nala_get_params_close()->ignore_fd_in = true;
}

void close_mock_none(void)
{
    nala_state_for_close.state.mode = 4;
}

void close_mock_implementation(int (*implementation)(int fd))
{
    nala_state_for_close.state.mode = 2;
    nala_state_for_close.data.implementation = implementation;
}

void close_mock_disable(void)
{
    nala_state_for_close.state.mode = 0;
}

void close_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_close.state);
}

void close_mock_resume(void)
{
    nala_state_resume(&nala_state_for_close.state);
}

void close_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_close);
}

void close_mock_assert_completed(void)
{
    if (nala_state_for_close.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked close() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_close.instances.length));
    }
}

// NALA_IMPLEMENTATION compose_twice

DummyStruct *__real_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));

struct _nala_data_params_for_compose_twice {
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

struct _nala_data_type_for_compose_twice {
    struct _nala_data_params_for_compose_twice params;
    DummyStruct *return_value;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
    void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_compose_twice {
    struct _nala_data_type_for_compose_twice data;
    struct _nala_instance_type_for_compose_twice *next_p;
};

struct _nala_instances_type_for_compose_twice {
    struct _nala_instance_type_for_compose_twice *head_p;
    struct _nala_instance_type_for_compose_twice *tail_p;
    int length;
};

struct nala_state_type_for_compose_twice {
    struct nala_state_t state;
    struct _nala_data_type_for_compose_twice data;
    struct _nala_instances_type_for_compose_twice instances;
};

static struct nala_state_type_for_compose_twice nala_state_for_compose_twice = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_compose_twice *nala_get_data_compose_twice()
{
    if (nala_state_for_compose_twice.instances.tail_p != NULL) {
        return (&nala_state_for_compose_twice.instances.tail_p->data);
    } else {
        return (&nala_state_for_compose_twice.data);
    }
}

struct _nala_data_params_for_compose_twice *nala_get_params_compose_twice()
{
    return (&nala_get_data_compose_twice()->params);
}

DummyStruct *__wrap_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct _nala_instance_type_for_compose_twice *_nala_instance_p;
    struct _nala_data_type_for_compose_twice *_nala_data_p;
    DummyStruct *return_value;

    nala_print_call("compose_twice", &nala_state_for_compose_twice.state);

    switch (nala_state_for_compose_twice.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_compose_twice.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_compose_twice.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked compose_twice() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_compose_twice.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, compose_twice, dummy_struct);
        MOCK_ASSERT_IN_EQ(_nala_data_p, compose_twice, dummy_struct_modifier);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   compose_twice,
                                   dummy_struct);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   compose_twice,
                                   dummy_struct_modifier);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(dummy_struct, dummy_struct_modifier);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_compose_twice.data.implementation(dummy_struct, dummy_struct_modifier);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_compose_twice.state.mode = 3;
    nala_state_for_compose_twice.data.params.dummy_struct = NULL;
    nala_state_for_compose_twice.data.params.ignore_dummy_struct_in = true;
    nala_state_for_compose_twice.data.params.dummy_struct_modifier = NULL;
    nala_state_for_compose_twice.data.params.ignore_dummy_struct_modifier_in = true;
    nala_state_for_compose_twice.data.return_value = return_value;
    nala_state_for_compose_twice.data.errno_value = 0;
    nala_state_for_compose_twice.data.callback = NULL;
}

void compose_twice_mock_once(DummyStruct *return_value)
{
    struct _nala_instance_type_for_compose_twice *_nala_instance_p;

    nala_state_for_compose_twice.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_in);
    _nala_instance_p->data.params.dummy_struct_in_assert = NULL;
    _nala_instance_p->data.params.dummy_struct_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_modifier_out);
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_modifier_in);
    _nala_instance_p->data.params.dummy_struct_modifier_in_assert = NULL;
    _nala_instance_p->data.params.dummy_struct_modifier_out_copy = NULL;
    _nala_instance_p->data.params.dummy_struct = NULL;
    _nala_instance_p->data.params.ignore_dummy_struct_in = true;
    _nala_instance_p->data.params.dummy_struct_modifier = NULL;
    _nala_instance_p->data.params.ignore_dummy_struct_modifier_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_compose_twice.instances,
                          _nala_instance_p);
}

void compose_twice_mock_ignore_in(DummyStruct *return_value)
{
    nala_state_for_compose_twice.state.mode = 3;
    nala_state_for_compose_twice.data.params.ignore_dummy_struct_in = true;
    nala_state_for_compose_twice.data.params.ignore_dummy_struct_modifier_in = true;
    nala_state_for_compose_twice.data.return_value = return_value;
    nala_state_for_compose_twice.data.errno_value = 0;
    nala_state_for_compose_twice.data.callback = NULL;
}

void compose_twice_mock_ignore_in_once(DummyStruct *return_value)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    nala_state_for_compose_twice.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_compose_twice.instances,
                          instance_p);
}

void compose_twice_mock_set_errno(int errno_value)
{
    nala_get_data_compose_twice()->errno_value = errno_value;
}

void compose_twice_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)))
{
    nala_get_data_compose_twice()->callback = callback;
}

void compose_twice_mock_set_dummy_struct_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_in,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_compose_twice()->dummy_struct_in_assert = callback;
}

void compose_twice_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct _nala_data_params_for_compose_twice *_nala_params_p;

    _nala_params_p = nala_get_params_compose_twice();
    _nala_params_p->ignore_dummy_struct_in = false;
    _nala_params_p->dummy_struct = dummy_struct;
}

void compose_twice_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_out,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_compose_twice()->dummy_struct_out_copy = callback;
}

void compose_twice_mock_set_dummy_struct_modifier_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_modifier_in,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_modifier_in_assert(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_compose_twice()->dummy_struct_modifier_in_assert = callback;
}

void compose_twice_mock_set_dummy_struct_modifier_in_pointer(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct _nala_data_params_for_compose_twice *_nala_params_p;

    _nala_params_p = nala_get_params_compose_twice();
    _nala_params_p->ignore_dummy_struct_modifier_in = false;
    _nala_params_p->dummy_struct_modifier = dummy_struct_modifier;
}

void compose_twice_mock_set_dummy_struct_modifier_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_compose_twice()->dummy_struct_modifier_out,
                       buf_p,
                       size);
}

void compose_twice_mock_set_dummy_struct_modifier_out_copy(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_compose_twice()->dummy_struct_modifier_out_copy = callback;
}

void compose_twice_mock_none(void)
{
    nala_state_for_compose_twice.state.mode = 4;
}

void compose_twice_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)))
{
    nala_state_for_compose_twice.state.mode = 2;
    nala_state_for_compose_twice.data.implementation = implementation;
}

void compose_twice_mock_disable(void)
{
    nala_state_for_compose_twice.state.mode = 0;
}

void compose_twice_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_compose_twice.state);
}

void compose_twice_mock_resume(void)
{
    nala_state_resume(&nala_state_for_compose_twice.state);
}

void compose_twice_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_compose_twice);
}

void compose_twice_mock_assert_completed(void)
{
    if (nala_state_for_compose_twice.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked compose_twice() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_compose_twice.instances.length));
    }
}

// NALA_IMPLEMENTATION double_pointer

int __real_double_pointer(int **value_pp);

struct _nala_data_params_for_double_pointer {
    int **value_pp;
    bool ignore_value_pp_in;
    struct nala_set_param value_pp_in;
    void (*value_pp_in_assert)(int **value_pp, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param value_pp_out;
    void (*value_pp_out_copy)(int **value_pp, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_double_pointer {
    struct _nala_data_params_for_double_pointer params;
    int return_value;
    int errno_value;
    int (*implementation)(int **value_pp);
    void (*callback)(int **value_pp);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_double_pointer {
    struct _nala_data_type_for_double_pointer data;
    struct _nala_instance_type_for_double_pointer *next_p;
};

struct _nala_instances_type_for_double_pointer {
    struct _nala_instance_type_for_double_pointer *head_p;
    struct _nala_instance_type_for_double_pointer *tail_p;
    int length;
};

struct nala_state_type_for_double_pointer {
    struct nala_state_t state;
    struct _nala_data_type_for_double_pointer data;
    struct _nala_instances_type_for_double_pointer instances;
};

static struct nala_state_type_for_double_pointer nala_state_for_double_pointer = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_double_pointer *nala_get_data_double_pointer()
{
    if (nala_state_for_double_pointer.instances.tail_p != NULL) {
        return (&nala_state_for_double_pointer.instances.tail_p->data);
    } else {
        return (&nala_state_for_double_pointer.data);
    }
}

struct _nala_data_params_for_double_pointer *nala_get_params_double_pointer()
{
    return (&nala_get_data_double_pointer()->params);
}

int __wrap_double_pointer(int **value_pp)
{
    struct _nala_instance_type_for_double_pointer *_nala_instance_p;
    struct _nala_data_type_for_double_pointer *_nala_data_p;
    int return_value;

    nala_print_call("double_pointer", &nala_state_for_double_pointer.state);

    switch (nala_state_for_double_pointer.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_double_pointer.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_double_pointer.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked double_pointer() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_double_pointer.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, double_pointer, value_pp);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   double_pointer,
                                   value_pp);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(value_pp);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_double_pointer.data.implementation(value_pp);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_double_pointer.state.mode = 3;
    nala_state_for_double_pointer.data.params.value_pp = NULL;
    nala_state_for_double_pointer.data.params.ignore_value_pp_in = true;
    nala_state_for_double_pointer.data.return_value = return_value;
    nala_state_for_double_pointer.data.errno_value = 0;
    nala_state_for_double_pointer.data.callback = NULL;
}

void double_pointer_mock_once(int return_value)
{
    struct _nala_instance_type_for_double_pointer *_nala_instance_p;

    nala_state_for_double_pointer.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.value_pp_out);
    nala_set_param_init(&_nala_instance_p->data.params.value_pp_in);
    _nala_instance_p->data.params.value_pp_in_assert = NULL;
    _nala_instance_p->data.params.value_pp_out_copy = NULL;
    _nala_instance_p->data.params.value_pp = NULL;
    _nala_instance_p->data.params.ignore_value_pp_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_double_pointer.instances,
                          _nala_instance_p);
}

void double_pointer_mock_ignore_in(int return_value)
{
    nala_state_for_double_pointer.state.mode = 3;
    nala_state_for_double_pointer.data.params.ignore_value_pp_in = true;
    nala_state_for_double_pointer.data.return_value = return_value;
    nala_state_for_double_pointer.data.errno_value = 0;
    nala_state_for_double_pointer.data.callback = NULL;
}

void double_pointer_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_double_pointer *instance_p;

    nala_state_for_double_pointer.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.value_pp_out);
    nala_set_param_init(&instance_p->data.params.value_pp_in);
    instance_p->data.params.value_pp_in_assert = NULL;
    instance_p->data.params.value_pp_out_copy = NULL;
    instance_p->data.params.value_pp = NULL;
    instance_p->data.params.ignore_value_pp_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_double_pointer.instances,
                          instance_p);
}

void double_pointer_mock_set_errno(int errno_value)
{
    nala_get_data_double_pointer()->errno_value = errno_value;
}

void double_pointer_mock_set_callback(void (*callback)(int **value_pp))
{
    nala_get_data_double_pointer()->callback = callback;
}

void double_pointer_mock_set_value_pp_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_double_pointer()->value_pp_in,
                       buf_p,
                       size);
}

void double_pointer_mock_set_value_pp_in_assert(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_double_pointer()->value_pp_in_assert = callback;
}

void double_pointer_mock_set_value_pp_in_pointer(int **value_pp)
{
    struct _nala_data_params_for_double_pointer *_nala_params_p;

    _nala_params_p = nala_get_params_double_pointer();
    _nala_params_p->ignore_value_pp_in = false;
    _nala_params_p->value_pp = value_pp;
}

void double_pointer_mock_set_value_pp_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_double_pointer()->value_pp_out,
                       buf_p,
                       size);
}

void double_pointer_mock_set_value_pp_out_copy(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_double_pointer()->value_pp_out_copy = callback;
}

void double_pointer_mock_none(void)
{
    nala_state_for_double_pointer.state.mode = 4;
}

void double_pointer_mock_implementation(int (*implementation)(int **value_pp))
{
    nala_state_for_double_pointer.state.mode = 2;
    nala_state_for_double_pointer.data.implementation = implementation;
}

void double_pointer_mock_disable(void)
{
    nala_state_for_double_pointer.state.mode = 0;
}

void double_pointer_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_double_pointer.state);
}

void double_pointer_mock_resume(void)
{
    nala_state_resume(&nala_state_for_double_pointer.state);
}

void double_pointer_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_double_pointer);
}

void double_pointer_mock_assert_completed(void)
{
    if (nala_state_for_double_pointer.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked double_pointer() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_double_pointer.instances.length));
    }
}

// NALA_IMPLEMENTATION dup

int __real_dup(int oldfd);

struct _nala_data_params_for_dup {
    int oldfd;
    bool ignore_oldfd_in;
};

struct _nala_data_type_for_dup {
    struct _nala_data_params_for_dup params;
    int return_value;
    int errno_value;
    int (*implementation)(int oldfd);
    void (*callback)(int oldfd);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_dup {
    struct _nala_data_type_for_dup data;
    struct _nala_instance_type_for_dup *next_p;
};

struct _nala_instances_type_for_dup {
    struct _nala_instance_type_for_dup *head_p;
    struct _nala_instance_type_for_dup *tail_p;
    int length;
};

struct nala_state_type_for_dup {
    struct nala_state_t state;
    struct _nala_data_type_for_dup data;
    struct _nala_instances_type_for_dup instances;
};

static struct nala_state_type_for_dup nala_state_for_dup = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_dup *nala_get_data_dup()
{
    if (nala_state_for_dup.instances.tail_p != NULL) {
        return (&nala_state_for_dup.instances.tail_p->data);
    } else {
        return (&nala_state_for_dup.data);
    }
}

struct _nala_data_params_for_dup *nala_get_params_dup()
{
    return (&nala_get_data_dup()->params);
}

int __wrap_dup(int oldfd)
{
    struct _nala_instance_type_for_dup *_nala_instance_p;
    struct _nala_data_type_for_dup *_nala_data_p;
    int return_value;

    nala_print_call("dup", &nala_state_for_dup.state);

    switch (nala_state_for_dup.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_dup.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_dup.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked dup() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_dup.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, dup, oldfd);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(oldfd);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_dup.data.implementation(oldfd);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_dup.state.mode = 3;
    nala_state_for_dup.data.params.oldfd = oldfd;
    nala_state_for_dup.data.params.ignore_oldfd_in = false;
    nala_state_for_dup.data.return_value = return_value;
    nala_state_for_dup.data.errno_value = 0;
    nala_state_for_dup.data.callback = NULL;
}

void dup_mock_once(int oldfd, int return_value)
{
    struct _nala_instance_type_for_dup *_nala_instance_p;

    nala_state_for_dup.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.oldfd = oldfd;
    _nala_instance_p->data.params.ignore_oldfd_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_dup.instances,
                          _nala_instance_p);
}

void dup_mock_ignore_in(int return_value)
{
    nala_state_for_dup.state.mode = 3;
    nala_state_for_dup.data.params.ignore_oldfd_in = true;
    nala_state_for_dup.data.return_value = return_value;
    nala_state_for_dup.data.errno_value = 0;
    nala_state_for_dup.data.callback = NULL;
}

void dup_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_dup *instance_p;

    nala_state_for_dup.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_oldfd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_dup.instances,
                          instance_p);
}

void dup_mock_set_errno(int errno_value)
{
    nala_get_data_dup()->errno_value = errno_value;
}

void dup_mock_set_callback(void (*callback)(int oldfd))
{
    nala_get_data_dup()->callback = callback;
}

void dup_mock_ignore_oldfd_in(void)
{
    nala_get_params_dup()->ignore_oldfd_in = true;
}

void dup_mock_none(void)
{
    nala_state_for_dup.state.mode = 4;
}

void dup_mock_implementation(int (*implementation)(int oldfd))
{
    nala_state_for_dup.state.mode = 2;
    nala_state_for_dup.data.implementation = implementation;
}

void dup_mock_disable(void)
{
    nala_state_for_dup.state.mode = 0;
}

void dup_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_dup.state);
}

void dup_mock_resume(void)
{
    nala_state_resume(&nala_state_for_dup.state);
}

void dup_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_dup);
}

void dup_mock_assert_completed(void)
{
    if (nala_state_for_dup.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked dup() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_dup.instances.length));
    }
}

// NALA_IMPLEMENTATION dup2

int __real_dup2(int oldfd, int newfd);

struct _nala_data_params_for_dup2 {
    int oldfd;
    int newfd;
    bool ignore_oldfd_in;
    bool ignore_newfd_in;
};

struct _nala_data_type_for_dup2 {
    struct _nala_data_params_for_dup2 params;
    int return_value;
    int errno_value;
    int (*implementation)(int oldfd, int newfd);
    void (*callback)(int oldfd, int newfd);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_dup2 {
    struct _nala_data_type_for_dup2 data;
    struct _nala_instance_type_for_dup2 *next_p;
};

struct _nala_instances_type_for_dup2 {
    struct _nala_instance_type_for_dup2 *head_p;
    struct _nala_instance_type_for_dup2 *tail_p;
    int length;
};

struct nala_state_type_for_dup2 {
    struct nala_state_t state;
    struct _nala_data_type_for_dup2 data;
    struct _nala_instances_type_for_dup2 instances;
};

static struct nala_state_type_for_dup2 nala_state_for_dup2 = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_dup2 *nala_get_data_dup2()
{
    if (nala_state_for_dup2.instances.tail_p != NULL) {
        return (&nala_state_for_dup2.instances.tail_p->data);
    } else {
        return (&nala_state_for_dup2.data);
    }
}

struct _nala_data_params_for_dup2 *nala_get_params_dup2()
{
    return (&nala_get_data_dup2()->params);
}

int __wrap_dup2(int oldfd, int newfd)
{
    struct _nala_instance_type_for_dup2 *_nala_instance_p;
    struct _nala_data_type_for_dup2 *_nala_data_p;
    int return_value;

    nala_print_call("dup2", &nala_state_for_dup2.state);

    switch (nala_state_for_dup2.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_dup2.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_dup2.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked dup2() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_dup2.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, dup2, oldfd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, dup2, newfd);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(oldfd, newfd);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_dup2.data.implementation(oldfd, newfd);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_dup2.state.mode = 3;
    nala_state_for_dup2.data.params.oldfd = oldfd;
    nala_state_for_dup2.data.params.ignore_oldfd_in = false;
    nala_state_for_dup2.data.params.newfd = newfd;
    nala_state_for_dup2.data.params.ignore_newfd_in = false;
    nala_state_for_dup2.data.return_value = return_value;
    nala_state_for_dup2.data.errno_value = 0;
    nala_state_for_dup2.data.callback = NULL;
}

void dup2_mock_once(int oldfd, int newfd, int return_value)
{
    struct _nala_instance_type_for_dup2 *_nala_instance_p;

    nala_state_for_dup2.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.oldfd = oldfd;
    _nala_instance_p->data.params.ignore_oldfd_in = false;
    _nala_instance_p->data.params.newfd = newfd;
    _nala_instance_p->data.params.ignore_newfd_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_dup2.instances,
                          _nala_instance_p);
}

void dup2_mock_ignore_in(int return_value)
{
    nala_state_for_dup2.state.mode = 3;
    nala_state_for_dup2.data.params.ignore_oldfd_in = true;
    nala_state_for_dup2.data.params.ignore_newfd_in = true;
    nala_state_for_dup2.data.return_value = return_value;
    nala_state_for_dup2.data.errno_value = 0;
    nala_state_for_dup2.data.callback = NULL;
}

void dup2_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_dup2 *instance_p;

    nala_state_for_dup2.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_oldfd_in = true;
    instance_p->data.params.ignore_newfd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_dup2.instances,
                          instance_p);
}

void dup2_mock_set_errno(int errno_value)
{
    nala_get_data_dup2()->errno_value = errno_value;
}

void dup2_mock_set_callback(void (*callback)(int oldfd, int newfd))
{
    nala_get_data_dup2()->callback = callback;
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
    nala_state_for_dup2.state.mode = 4;
}

void dup2_mock_implementation(int (*implementation)(int oldfd, int newfd))
{
    nala_state_for_dup2.state.mode = 2;
    nala_state_for_dup2.data.implementation = implementation;
}

void dup2_mock_disable(void)
{
    nala_state_for_dup2.state.mode = 0;
}

void dup2_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_dup2.state);
}

void dup2_mock_resume(void)
{
    nala_state_resume(&nala_state_for_dup2.state);
}

void dup2_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_dup2);
}

void dup2_mock_assert_completed(void)
{
    if (nala_state_for_dup2.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked dup2() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_dup2.instances.length));
    }
}

// NALA_IMPLEMENTATION edit_number

DummyStruct *__real_edit_number(DummyStruct *dummy_struct, int number);

struct _nala_data_params_for_edit_number {
    DummyStruct *dummy_struct;
    int number;
    bool ignore_dummy_struct_in;
    struct nala_set_param dummy_struct_in;
    void (*dummy_struct_in_assert)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param dummy_struct_out;
    void (*dummy_struct_out_copy)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size);
    bool ignore_number_in;
};

struct _nala_data_type_for_edit_number {
    struct _nala_data_params_for_edit_number params;
    DummyStruct *return_value;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number);
    void (*callback)(DummyStruct *dummy_struct, int number);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_edit_number {
    struct _nala_data_type_for_edit_number data;
    struct _nala_instance_type_for_edit_number *next_p;
};

struct _nala_instances_type_for_edit_number {
    struct _nala_instance_type_for_edit_number *head_p;
    struct _nala_instance_type_for_edit_number *tail_p;
    int length;
};

struct nala_state_type_for_edit_number {
    struct nala_state_t state;
    struct _nala_data_type_for_edit_number data;
    struct _nala_instances_type_for_edit_number instances;
};

static struct nala_state_type_for_edit_number nala_state_for_edit_number = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_edit_number *nala_get_data_edit_number()
{
    if (nala_state_for_edit_number.instances.tail_p != NULL) {
        return (&nala_state_for_edit_number.instances.tail_p->data);
    } else {
        return (&nala_state_for_edit_number.data);
    }
}

struct _nala_data_params_for_edit_number *nala_get_params_edit_number()
{
    return (&nala_get_data_edit_number()->params);
}

DummyStruct *__wrap_edit_number(DummyStruct *dummy_struct, int number)
{
    struct _nala_instance_type_for_edit_number *_nala_instance_p;
    struct _nala_data_type_for_edit_number *_nala_data_p;
    DummyStruct *return_value;

    nala_print_call("edit_number", &nala_state_for_edit_number.state);

    switch (nala_state_for_edit_number.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_edit_number.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_edit_number.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked edit_number() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_edit_number.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, edit_number, dummy_struct);
        MOCK_ASSERT_IN_EQ(_nala_data_p, edit_number, number);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   edit_number,
                                   dummy_struct);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(dummy_struct, number);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_edit_number.data.implementation(dummy_struct, number);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_edit_number.state.mode = 3;
    nala_state_for_edit_number.data.params.dummy_struct = NULL;
    nala_state_for_edit_number.data.params.ignore_dummy_struct_in = true;
    nala_state_for_edit_number.data.params.number = number;
    nala_state_for_edit_number.data.params.ignore_number_in = false;
    nala_state_for_edit_number.data.return_value = return_value;
    nala_state_for_edit_number.data.errno_value = 0;
    nala_state_for_edit_number.data.callback = NULL;
}

void edit_number_mock_once(int number, DummyStruct *return_value)
{
    struct _nala_instance_type_for_edit_number *_nala_instance_p;

    nala_state_for_edit_number.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_out);
    nala_set_param_init(&_nala_instance_p->data.params.dummy_struct_in);
    _nala_instance_p->data.params.dummy_struct_in_assert = NULL;
    _nala_instance_p->data.params.dummy_struct_out_copy = NULL;
    _nala_instance_p->data.params.dummy_struct = NULL;
    _nala_instance_p->data.params.ignore_dummy_struct_in = true;
    _nala_instance_p->data.params.number = number;
    _nala_instance_p->data.params.ignore_number_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_edit_number.instances,
                          _nala_instance_p);
}

void edit_number_mock_ignore_in(DummyStruct *return_value)
{
    nala_state_for_edit_number.state.mode = 3;
    nala_state_for_edit_number.data.params.ignore_dummy_struct_in = true;
    nala_state_for_edit_number.data.params.ignore_number_in = true;
    nala_state_for_edit_number.data.return_value = return_value;
    nala_state_for_edit_number.data.errno_value = 0;
    nala_state_for_edit_number.data.callback = NULL;
}

void edit_number_mock_ignore_in_once(DummyStruct *return_value)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    nala_state_for_edit_number.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_edit_number.instances,
                          instance_p);
}

void edit_number_mock_set_errno(int errno_value)
{
    nala_get_data_edit_number()->errno_value = errno_value;
}

void edit_number_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, int number))
{
    nala_get_data_edit_number()->callback = callback;
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
    nala_get_params_edit_number()->dummy_struct_in_assert = callback;
}

void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct _nala_data_params_for_edit_number *_nala_params_p;

    _nala_params_p = nala_get_params_edit_number();
    _nala_params_p->ignore_dummy_struct_in = false;
    _nala_params_p->dummy_struct = dummy_struct;
}

void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_edit_number()->dummy_struct_out,
                       buf_p,
                       size);
}

void edit_number_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_edit_number()->dummy_struct_out_copy = callback;
}

void edit_number_mock_none(void)
{
    nala_state_for_edit_number.state.mode = 4;
}

void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number))
{
    nala_state_for_edit_number.state.mode = 2;
    nala_state_for_edit_number.data.implementation = implementation;
}

void edit_number_mock_disable(void)
{
    nala_state_for_edit_number.state.mode = 0;
}

void edit_number_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_edit_number.state);
}

void edit_number_mock_resume(void)
{
    nala_state_resume(&nala_state_for_edit_number.state);
}

void edit_number_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_edit_number);
}

void edit_number_mock_assert_completed(void)
{
    if (nala_state_for_edit_number.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked edit_number() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_edit_number.instances.length));
    }
}

// NALA_IMPLEMENTATION endmntent

int __real_endmntent(FILE *streamp);

struct _nala_data_params_for_endmntent {
    FILE *streamp;
    bool ignore_streamp_in;
    struct nala_set_param streamp_in;
    void (*streamp_in_assert)(FILE *streamp, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param streamp_out;
    void (*streamp_out_copy)(FILE *streamp, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_endmntent {
    struct _nala_data_params_for_endmntent params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *streamp);
    void (*callback)(FILE *streamp);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_endmntent {
    struct _nala_data_type_for_endmntent data;
    struct _nala_instance_type_for_endmntent *next_p;
};

struct _nala_instances_type_for_endmntent {
    struct _nala_instance_type_for_endmntent *head_p;
    struct _nala_instance_type_for_endmntent *tail_p;
    int length;
};

struct nala_state_type_for_endmntent {
    struct nala_state_t state;
    struct _nala_data_type_for_endmntent data;
    struct _nala_instances_type_for_endmntent instances;
};

static struct nala_state_type_for_endmntent nala_state_for_endmntent = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_endmntent *nala_get_data_endmntent()
{
    if (nala_state_for_endmntent.instances.tail_p != NULL) {
        return (&nala_state_for_endmntent.instances.tail_p->data);
    } else {
        return (&nala_state_for_endmntent.data);
    }
}

struct _nala_data_params_for_endmntent *nala_get_params_endmntent()
{
    return (&nala_get_data_endmntent()->params);
}

int __wrap_endmntent(FILE *streamp)
{
    struct _nala_instance_type_for_endmntent *_nala_instance_p;
    struct _nala_data_type_for_endmntent *_nala_data_p;
    int return_value;

    nala_print_call("endmntent", &nala_state_for_endmntent.state);

    switch (nala_state_for_endmntent.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_endmntent.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_endmntent.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked endmntent() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_endmntent.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, endmntent, streamp);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   endmntent,
                                   streamp);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(streamp);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_endmntent.data.implementation(streamp);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_endmntent.state.mode = 3;
    nala_state_for_endmntent.data.params.streamp = NULL;
    nala_state_for_endmntent.data.params.ignore_streamp_in = true;
    nala_state_for_endmntent.data.return_value = return_value;
    nala_state_for_endmntent.data.errno_value = 0;
    nala_state_for_endmntent.data.callback = NULL;
}

void endmntent_mock_once(int return_value)
{
    struct _nala_instance_type_for_endmntent *_nala_instance_p;

    nala_state_for_endmntent.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.streamp_out);
    nala_set_param_init(&_nala_instance_p->data.params.streamp_in);
    _nala_instance_p->data.params.streamp_in_assert = NULL;
    _nala_instance_p->data.params.streamp_out_copy = NULL;
    _nala_instance_p->data.params.streamp = NULL;
    _nala_instance_p->data.params.ignore_streamp_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_endmntent.instances,
                          _nala_instance_p);
}

void endmntent_mock_ignore_in(int return_value)
{
    nala_state_for_endmntent.state.mode = 3;
    nala_state_for_endmntent.data.params.ignore_streamp_in = true;
    nala_state_for_endmntent.data.return_value = return_value;
    nala_state_for_endmntent.data.errno_value = 0;
    nala_state_for_endmntent.data.callback = NULL;
}

void endmntent_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_endmntent *instance_p;

    nala_state_for_endmntent.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.streamp_out);
    nala_set_param_init(&instance_p->data.params.streamp_in);
    instance_p->data.params.streamp_in_assert = NULL;
    instance_p->data.params.streamp_out_copy = NULL;
    instance_p->data.params.streamp = NULL;
    instance_p->data.params.ignore_streamp_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_endmntent.instances,
                          instance_p);
}

void endmntent_mock_set_errno(int errno_value)
{
    nala_get_data_endmntent()->errno_value = errno_value;
}

void endmntent_mock_set_callback(void (*callback)(FILE *streamp))
{
    nala_get_data_endmntent()->callback = callback;
}

void endmntent_mock_set_streamp_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_endmntent()->streamp_in,
                       buf_p,
                       size);
}

void endmntent_mock_set_streamp_in_assert(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_endmntent()->streamp_in_assert = callback;
}

void endmntent_mock_set_streamp_in_pointer(FILE *streamp)
{
    struct _nala_data_params_for_endmntent *_nala_params_p;

    _nala_params_p = nala_get_params_endmntent();
    _nala_params_p->ignore_streamp_in = false;
    _nala_params_p->streamp = streamp;
}

void endmntent_mock_set_streamp_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_endmntent()->streamp_out,
                       buf_p,
                       size);
}

void endmntent_mock_set_streamp_out_copy(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_endmntent()->streamp_out_copy = callback;
}

void endmntent_mock_none(void)
{
    nala_state_for_endmntent.state.mode = 4;
}

void endmntent_mock_implementation(int (*implementation)(FILE *streamp))
{
    nala_state_for_endmntent.state.mode = 2;
    nala_state_for_endmntent.data.implementation = implementation;
}

void endmntent_mock_disable(void)
{
    nala_state_for_endmntent.state.mode = 0;
}

void endmntent_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_endmntent.state);
}

void endmntent_mock_resume(void)
{
    nala_state_resume(&nala_state_for_endmntent.state);
}

void endmntent_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_endmntent);
}

void endmntent_mock_assert_completed(void)
{
    if (nala_state_for_endmntent.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked endmntent() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_endmntent.instances.length));
    }
}

// NALA_IMPLEMENTATION enum_param

void __real_enum_param(enum enum_param_type value);

struct _nala_data_params_for_enum_param {
    enum enum_param_type value;
    bool ignore_value_in;
};

struct _nala_data_type_for_enum_param {
    struct _nala_data_params_for_enum_param params;
    int errno_value;
    void (*implementation)(enum enum_param_type value);
    void (*callback)(enum enum_param_type value);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_enum_param {
    struct _nala_data_type_for_enum_param data;
    struct _nala_instance_type_for_enum_param *next_p;
};

struct _nala_instances_type_for_enum_param {
    struct _nala_instance_type_for_enum_param *head_p;
    struct _nala_instance_type_for_enum_param *tail_p;
    int length;
};

struct nala_state_type_for_enum_param {
    struct nala_state_t state;
    struct _nala_data_type_for_enum_param data;
    struct _nala_instances_type_for_enum_param instances;
};

static struct nala_state_type_for_enum_param nala_state_for_enum_param = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_enum_param *nala_get_data_enum_param()
{
    if (nala_state_for_enum_param.instances.tail_p != NULL) {
        return (&nala_state_for_enum_param.instances.tail_p->data);
    } else {
        return (&nala_state_for_enum_param.data);
    }
}

struct _nala_data_params_for_enum_param *nala_get_params_enum_param()
{
    return (&nala_get_data_enum_param()->params);
}

void __wrap_enum_param(enum enum_param_type value)
{
    struct _nala_instance_type_for_enum_param *_nala_instance_p;
    struct _nala_data_type_for_enum_param *_nala_data_p;

    nala_print_call("enum_param", &nala_state_for_enum_param.state);

    switch (nala_state_for_enum_param.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_enum_param.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_enum_param.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked enum_param() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_enum_param.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, enum_param, value);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(value);
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_enum_param.data.implementation(value);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_enum_param.state.mode = 3;
    nala_state_for_enum_param.data.params.value = value;
    nala_state_for_enum_param.data.params.ignore_value_in = false;
    nala_state_for_enum_param.data.errno_value = 0;
    nala_state_for_enum_param.data.callback = NULL;
}

void enum_param_mock_once(enum enum_param_type value)
{
    struct _nala_instance_type_for_enum_param *_nala_instance_p;

    nala_state_for_enum_param.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.value = value;
    _nala_instance_p->data.params.ignore_value_in = false;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_enum_param.instances,
                          _nala_instance_p);
}

void enum_param_mock_ignore_in(void)
{
    nala_state_for_enum_param.state.mode = 3;
    nala_state_for_enum_param.data.params.ignore_value_in = true;
    nala_state_for_enum_param.data.errno_value = 0;
    nala_state_for_enum_param.data.callback = NULL;
}

void enum_param_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_enum_param *instance_p;

    nala_state_for_enum_param.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_value_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_enum_param.instances,
                          instance_p);
}

void enum_param_mock_set_errno(int errno_value)
{
    nala_get_data_enum_param()->errno_value = errno_value;
}

void enum_param_mock_set_callback(void (*callback)(enum enum_param_type value))
{
    nala_get_data_enum_param()->callback = callback;
}

void enum_param_mock_ignore_value_in(void)
{
    nala_get_params_enum_param()->ignore_value_in = true;
}

void enum_param_mock_none(void)
{
    nala_state_for_enum_param.state.mode = 4;
}

void enum_param_mock_implementation(void (*implementation)(enum enum_param_type value))
{
    nala_state_for_enum_param.state.mode = 2;
    nala_state_for_enum_param.data.implementation = implementation;
}

void enum_param_mock_disable(void)
{
    nala_state_for_enum_param.state.mode = 0;
}

void enum_param_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_enum_param.state);
}

void enum_param_mock_resume(void)
{
    nala_state_resume(&nala_state_for_enum_param.state);
}

void enum_param_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_enum_param);
}

void enum_param_mock_assert_completed(void)
{
    if (nala_state_for_enum_param.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked enum_param() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_enum_param.instances.length));
    }
}

// NALA_IMPLEMENTATION fclose

int __real_fclose(FILE *stream);

struct _nala_data_params_for_fclose {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_fclose {
    struct _nala_data_params_for_fclose params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fclose {
    struct _nala_data_type_for_fclose data;
    struct _nala_instance_type_for_fclose *next_p;
};

struct _nala_instances_type_for_fclose {
    struct _nala_instance_type_for_fclose *head_p;
    struct _nala_instance_type_for_fclose *tail_p;
    int length;
};

struct nala_state_type_for_fclose {
    struct nala_state_t state;
    struct _nala_data_type_for_fclose data;
    struct _nala_instances_type_for_fclose instances;
};

static struct nala_state_type_for_fclose nala_state_for_fclose = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fclose *nala_get_data_fclose()
{
    if (nala_state_for_fclose.instances.tail_p != NULL) {
        return (&nala_state_for_fclose.instances.tail_p->data);
    } else {
        return (&nala_state_for_fclose.data);
    }
}

struct _nala_data_params_for_fclose *nala_get_params_fclose()
{
    return (&nala_get_data_fclose()->params);
}

int __wrap_fclose(FILE *stream)
{
    struct _nala_instance_type_for_fclose *_nala_instance_p;
    struct _nala_data_type_for_fclose *_nala_data_p;
    int return_value;

    nala_print_call("fclose", &nala_state_for_fclose.state);

    switch (nala_state_for_fclose.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fclose.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fclose.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fclose() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fclose.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fclose, stream);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fclose,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fclose.data.implementation(stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fclose.state.mode = 3;
    nala_state_for_fclose.data.params.stream = NULL;
    nala_state_for_fclose.data.params.ignore_stream_in = true;
    nala_state_for_fclose.data.return_value = return_value;
    nala_state_for_fclose.data.errno_value = 0;
    nala_state_for_fclose.data.callback = NULL;
}

void fclose_mock_once(int return_value)
{
    struct _nala_instance_type_for_fclose *_nala_instance_p;

    nala_state_for_fclose.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fclose.instances,
                          _nala_instance_p);
}

void fclose_mock_ignore_in(int return_value)
{
    nala_state_for_fclose.state.mode = 3;
    nala_state_for_fclose.data.params.ignore_stream_in = true;
    nala_state_for_fclose.data.return_value = return_value;
    nala_state_for_fclose.data.errno_value = 0;
    nala_state_for_fclose.data.callback = NULL;
}

void fclose_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fclose *instance_p;

    nala_state_for_fclose.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fclose.instances,
                          instance_p);
}

void fclose_mock_set_errno(int errno_value)
{
    nala_get_data_fclose()->errno_value = errno_value;
}

void fclose_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fclose()->callback = callback;
}

void fclose_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fclose()->stream_in,
                       buf_p,
                       size);
}

void fclose_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fclose()->stream_in_assert = callback;
}

void fclose_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fclose *_nala_params_p;

    _nala_params_p = nala_get_params_fclose();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fclose_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fclose()->stream_out,
                       buf_p,
                       size);
}

void fclose_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fclose()->stream_out_copy = callback;
}

void fclose_mock_none(void)
{
    nala_state_for_fclose.state.mode = 4;
}

void fclose_mock_implementation(int (*implementation)(FILE *stream))
{
    nala_state_for_fclose.state.mode = 2;
    nala_state_for_fclose.data.implementation = implementation;
}

void fclose_mock_disable(void)
{
    nala_state_for_fclose.state.mode = 0;
}

void fclose_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fclose.state);
}

void fclose_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fclose.state);
}

void fclose_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fclose);
}

void fclose_mock_assert_completed(void)
{
    if (nala_state_for_fclose.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fclose() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fclose.instances.length));
    }
}

// NALA_IMPLEMENTATION fflush

int __real_fflush(FILE *stream);

struct _nala_data_params_for_fflush {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_fflush {
    struct _nala_data_params_for_fflush params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fflush {
    struct _nala_data_type_for_fflush data;
    struct _nala_instance_type_for_fflush *next_p;
};

struct _nala_instances_type_for_fflush {
    struct _nala_instance_type_for_fflush *head_p;
    struct _nala_instance_type_for_fflush *tail_p;
    int length;
};

struct nala_state_type_for_fflush {
    struct nala_state_t state;
    struct _nala_data_type_for_fflush data;
    struct _nala_instances_type_for_fflush instances;
};

static struct nala_state_type_for_fflush nala_state_for_fflush = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fflush *nala_get_data_fflush()
{
    if (nala_state_for_fflush.instances.tail_p != NULL) {
        return (&nala_state_for_fflush.instances.tail_p->data);
    } else {
        return (&nala_state_for_fflush.data);
    }
}

struct _nala_data_params_for_fflush *nala_get_params_fflush()
{
    return (&nala_get_data_fflush()->params);
}

int __wrap_fflush(FILE *stream)
{
    struct _nala_instance_type_for_fflush *_nala_instance_p;
    struct _nala_data_type_for_fflush *_nala_data_p;
    int return_value;

    nala_print_call("fflush", &nala_state_for_fflush.state);

    switch (nala_state_for_fflush.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fflush.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fflush.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fflush() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fflush.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fflush, stream);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fflush,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fflush.data.implementation(stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fflush.state.mode = 3;
    nala_state_for_fflush.data.params.stream = NULL;
    nala_state_for_fflush.data.params.ignore_stream_in = true;
    nala_state_for_fflush.data.return_value = return_value;
    nala_state_for_fflush.data.errno_value = 0;
    nala_state_for_fflush.data.callback = NULL;
}

void fflush_mock_once(int return_value)
{
    struct _nala_instance_type_for_fflush *_nala_instance_p;

    nala_state_for_fflush.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fflush.instances,
                          _nala_instance_p);
}

void fflush_mock_ignore_in(int return_value)
{
    nala_state_for_fflush.state.mode = 3;
    nala_state_for_fflush.data.params.ignore_stream_in = true;
    nala_state_for_fflush.data.return_value = return_value;
    nala_state_for_fflush.data.errno_value = 0;
    nala_state_for_fflush.data.callback = NULL;
}

void fflush_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fflush *instance_p;

    nala_state_for_fflush.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fflush.instances,
                          instance_p);
}

void fflush_mock_set_errno(int errno_value)
{
    nala_get_data_fflush()->errno_value = errno_value;
}

void fflush_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fflush()->callback = callback;
}

void fflush_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fflush()->stream_in,
                       buf_p,
                       size);
}

void fflush_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fflush()->stream_in_assert = callback;
}

void fflush_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fflush *_nala_params_p;

    _nala_params_p = nala_get_params_fflush();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fflush_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fflush()->stream_out,
                       buf_p,
                       size);
}

void fflush_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fflush()->stream_out_copy = callback;
}

void fflush_mock_none(void)
{
    nala_state_for_fflush.state.mode = 4;
}

void fflush_mock_implementation(int (*implementation)(FILE *stream))
{
    nala_state_for_fflush.state.mode = 2;
    nala_state_for_fflush.data.implementation = implementation;
}

void fflush_mock_disable(void)
{
    nala_state_for_fflush.state.mode = 0;
}

void fflush_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fflush.state);
}

void fflush_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fflush.state);
}

void fflush_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fflush);
}

void fflush_mock_assert_completed(void)
{
    if (nala_state_for_fflush.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fflush() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fflush.instances.length));
    }
}

// NALA_IMPLEMENTATION fileno

int __real_fileno(FILE *stream);

struct _nala_data_params_for_fileno {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_fileno {
    struct _nala_data_params_for_fileno params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fileno {
    struct _nala_data_type_for_fileno data;
    struct _nala_instance_type_for_fileno *next_p;
};

struct _nala_instances_type_for_fileno {
    struct _nala_instance_type_for_fileno *head_p;
    struct _nala_instance_type_for_fileno *tail_p;
    int length;
};

struct nala_state_type_for_fileno {
    struct nala_state_t state;
    struct _nala_data_type_for_fileno data;
    struct _nala_instances_type_for_fileno instances;
};

static struct nala_state_type_for_fileno nala_state_for_fileno = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fileno *nala_get_data_fileno()
{
    if (nala_state_for_fileno.instances.tail_p != NULL) {
        return (&nala_state_for_fileno.instances.tail_p->data);
    } else {
        return (&nala_state_for_fileno.data);
    }
}

struct _nala_data_params_for_fileno *nala_get_params_fileno()
{
    return (&nala_get_data_fileno()->params);
}

int __wrap_fileno(FILE *stream)
{
    struct _nala_instance_type_for_fileno *_nala_instance_p;
    struct _nala_data_type_for_fileno *_nala_data_p;
    int return_value;

    nala_print_call("fileno", &nala_state_for_fileno.state);

    switch (nala_state_for_fileno.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fileno.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fileno.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fileno() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fileno.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fileno, stream);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fileno,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fileno.data.implementation(stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fileno.state.mode = 3;
    nala_state_for_fileno.data.params.stream = NULL;
    nala_state_for_fileno.data.params.ignore_stream_in = true;
    nala_state_for_fileno.data.return_value = return_value;
    nala_state_for_fileno.data.errno_value = 0;
    nala_state_for_fileno.data.callback = NULL;
}

void fileno_mock_once(int return_value)
{
    struct _nala_instance_type_for_fileno *_nala_instance_p;

    nala_state_for_fileno.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fileno.instances,
                          _nala_instance_p);
}

void fileno_mock_ignore_in(int return_value)
{
    nala_state_for_fileno.state.mode = 3;
    nala_state_for_fileno.data.params.ignore_stream_in = true;
    nala_state_for_fileno.data.return_value = return_value;
    nala_state_for_fileno.data.errno_value = 0;
    nala_state_for_fileno.data.callback = NULL;
}

void fileno_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fileno *instance_p;

    nala_state_for_fileno.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fileno.instances,
                          instance_p);
}

void fileno_mock_set_errno(int errno_value)
{
    nala_get_data_fileno()->errno_value = errno_value;
}

void fileno_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_fileno()->callback = callback;
}

void fileno_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fileno()->stream_in,
                       buf_p,
                       size);
}

void fileno_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fileno()->stream_in_assert = callback;
}

void fileno_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fileno *_nala_params_p;

    _nala_params_p = nala_get_params_fileno();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fileno_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fileno()->stream_out,
                       buf_p,
                       size);
}

void fileno_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fileno()->stream_out_copy = callback;
}

void fileno_mock_none(void)
{
    nala_state_for_fileno.state.mode = 4;
}

void fileno_mock_implementation(int (*implementation)(FILE *stream))
{
    nala_state_for_fileno.state.mode = 2;
    nala_state_for_fileno.data.implementation = implementation;
}

void fileno_mock_disable(void)
{
    nala_state_for_fileno.state.mode = 0;
}

void fileno_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fileno.state);
}

void fileno_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fileno.state);
}

void fileno_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fileno);
}

void fileno_mock_assert_completed(void)
{
    if (nala_state_for_fileno.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fileno() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fileno.instances.length));
    }
}

// NALA_IMPLEMENTATION fopen

FILE *__real_fopen(const char *path, const char *mode);

struct _nala_data_params_for_fopen {
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

struct _nala_data_type_for_fopen {
    struct _nala_data_params_for_fopen params;
    FILE *return_value;
    int errno_value;
    FILE *(*implementation)(const char *path, const char *mode);
    void (*callback)(const char *path, const char *mode);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fopen {
    struct _nala_data_type_for_fopen data;
    struct _nala_instance_type_for_fopen *next_p;
};

struct _nala_instances_type_for_fopen {
    struct _nala_instance_type_for_fopen *head_p;
    struct _nala_instance_type_for_fopen *tail_p;
    int length;
};

struct nala_state_type_for_fopen {
    struct nala_state_t state;
    struct _nala_data_type_for_fopen data;
    struct _nala_instances_type_for_fopen instances;
};

static struct nala_state_type_for_fopen nala_state_for_fopen = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fopen *nala_get_data_fopen()
{
    if (nala_state_for_fopen.instances.tail_p != NULL) {
        return (&nala_state_for_fopen.instances.tail_p->data);
    } else {
        return (&nala_state_for_fopen.data);
    }
}

struct _nala_data_params_for_fopen *nala_get_params_fopen()
{
    return (&nala_get_data_fopen()->params);
}

FILE *__wrap_fopen(const char *path, const char *mode)
{
    struct _nala_instance_type_for_fopen *_nala_instance_p;
    struct _nala_data_type_for_fopen *_nala_data_p;
    FILE *return_value;

    nala_print_call("fopen", &nala_state_for_fopen.state);

    switch (nala_state_for_fopen.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fopen.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fopen.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fopen() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fopen.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fopen, path);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fopen, mode);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fopen,
                                   path);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fopen,
                                   mode);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(path, mode);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fopen.data.implementation(path, mode);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fopen.state.mode = 3;
    nala_state_for_fopen.data.params.path = NULL;
    nala_state_for_fopen.data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_state_for_fopen.data.params.path_in,
                              path);
    } else {
        nala_state_for_fopen.data.params.ignore_path_in = false;
    }

    nala_state_for_fopen.data.params.mode = NULL;
    nala_state_for_fopen.data.params.ignore_mode_in = true;

    if (mode != NULL) {
        nala_set_param_string(&nala_state_for_fopen.data.params.mode_in,
                              mode);
    } else {
        nala_state_for_fopen.data.params.ignore_mode_in = false;
    }

    nala_state_for_fopen.data.return_value = return_value;
    nala_state_for_fopen.data.errno_value = 0;
    nala_state_for_fopen.data.callback = NULL;
}

void fopen_mock_once(const char *path, const char *mode, FILE *return_value)
{
    struct _nala_instance_type_for_fopen *_nala_instance_p;

    nala_state_for_fopen.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.path_out);
    nala_set_param_init(&_nala_instance_p->data.params.path_in);
    _nala_instance_p->data.params.path_in_assert = NULL;
    _nala_instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.mode_out);
    nala_set_param_init(&_nala_instance_p->data.params.mode_in);
    _nala_instance_p->data.params.mode_in_assert = NULL;
    _nala_instance_p->data.params.mode_out_copy = NULL;
    _nala_instance_p->data.params.path = NULL;
    _nala_instance_p->data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.path_in,
                              path);
    } else {
        _nala_instance_p->data.params.ignore_path_in = false;
    }

    _nala_instance_p->data.params.mode = NULL;
    _nala_instance_p->data.params.ignore_mode_in = true;

    if (mode != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.mode_in,
                              mode);
    } else {
        _nala_instance_p->data.params.ignore_mode_in = false;
    }

    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fopen.instances,
                          _nala_instance_p);
}

void fopen_mock_ignore_in(FILE *return_value)
{
    nala_state_for_fopen.state.mode = 3;
    nala_state_for_fopen.data.params.ignore_path_in = true;
    nala_state_for_fopen.data.params.ignore_mode_in = true;
    nala_state_for_fopen.data.return_value = return_value;
    nala_state_for_fopen.data.errno_value = 0;
    nala_state_for_fopen.data.callback = NULL;
}

void fopen_mock_ignore_in_once(FILE *return_value)
{
    struct _nala_instance_type_for_fopen *instance_p;

    nala_state_for_fopen.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fopen.instances,
                          instance_p);
}

void fopen_mock_set_errno(int errno_value)
{
    nala_get_data_fopen()->errno_value = errno_value;
}

void fopen_mock_set_callback(void (*callback)(const char *path, const char *mode))
{
    nala_get_data_fopen()->callback = callback;
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
    nala_get_params_fopen()->path_in_assert = callback;
}

void fopen_mock_set_path_in_pointer(const char *path)
{
    struct _nala_data_params_for_fopen *_nala_params_p;

    _nala_params_p = nala_get_params_fopen();
    _nala_params_p->ignore_path_in = false;
    _nala_params_p->path = path;
}

void fopen_mock_set_path_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->path_out,
                       buf_p,
                       size);
}

void fopen_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fopen()->path_out_copy = callback;
}

void fopen_mock_set_mode_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->mode_in,
                       buf_p,
                       size);
}

void fopen_mock_set_mode_in_assert(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fopen()->mode_in_assert = callback;
}

void fopen_mock_set_mode_in_pointer(const char *mode)
{
    struct _nala_data_params_for_fopen *_nala_params_p;

    _nala_params_p = nala_get_params_fopen();
    _nala_params_p->ignore_mode_in = false;
    _nala_params_p->mode = mode;
}

void fopen_mock_set_mode_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fopen()->mode_out,
                       buf_p,
                       size);
}

void fopen_mock_set_mode_out_copy(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fopen()->mode_out_copy = callback;
}

void fopen_mock_none(void)
{
    nala_state_for_fopen.state.mode = 4;
}

void fopen_mock_implementation(FILE *(*implementation)(const char *path, const char *mode))
{
    nala_state_for_fopen.state.mode = 2;
    nala_state_for_fopen.data.implementation = implementation;
}

void fopen_mock_disable(void)
{
    nala_state_for_fopen.state.mode = 0;
}

void fopen_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fopen.state);
}

void fopen_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fopen.state);
}

void fopen_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fopen);
}

void fopen_mock_assert_completed(void)
{
    if (nala_state_for_fopen.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fopen() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fopen.instances.length));
    }
}

// NALA_IMPLEMENTATION fread

size_t __real_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

struct _nala_data_params_for_fread {
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

struct _nala_data_type_for_fread {
    struct _nala_data_params_for_fread params;
    size_t return_value;
    int errno_value;
    size_t (*implementation)(void *ptr, size_t size, size_t nmemb, FILE *stream);
    void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fread {
    struct _nala_data_type_for_fread data;
    struct _nala_instance_type_for_fread *next_p;
};

struct _nala_instances_type_for_fread {
    struct _nala_instance_type_for_fread *head_p;
    struct _nala_instance_type_for_fread *tail_p;
    int length;
};

struct nala_state_type_for_fread {
    struct nala_state_t state;
    struct _nala_data_type_for_fread data;
    struct _nala_instances_type_for_fread instances;
};

static struct nala_state_type_for_fread nala_state_for_fread = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fread *nala_get_data_fread()
{
    if (nala_state_for_fread.instances.tail_p != NULL) {
        return (&nala_state_for_fread.instances.tail_p->data);
    } else {
        return (&nala_state_for_fread.data);
    }
}

struct _nala_data_params_for_fread *nala_get_params_fread()
{
    return (&nala_get_data_fread()->params);
}

size_t __wrap_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    struct _nala_instance_type_for_fread *_nala_instance_p;
    struct _nala_data_type_for_fread *_nala_data_p;
    size_t return_value;

    nala_print_call("fread", &nala_state_for_fread.state);

    switch (nala_state_for_fread.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fread.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fread.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fread() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fread.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fread, ptr);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fread, stream);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fread, size);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fread, nmemb);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fread,
                                   ptr);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fread,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(ptr, size, nmemb, stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fread.data.implementation(ptr, size, nmemb, stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fread.state.mode = 3;
    nala_state_for_fread.data.params.ptr = NULL;
    nala_state_for_fread.data.params.ignore_ptr_in = true;
    nala_state_for_fread.data.params.stream = NULL;
    nala_state_for_fread.data.params.ignore_stream_in = true;
    nala_state_for_fread.data.params.size = size;
    nala_state_for_fread.data.params.ignore_size_in = false;
    nala_state_for_fread.data.params.nmemb = nmemb;
    nala_state_for_fread.data.params.ignore_nmemb_in = false;
    nala_state_for_fread.data.return_value = return_value;
    nala_state_for_fread.data.errno_value = 0;
    nala_state_for_fread.data.callback = NULL;
}

void fread_mock_once(size_t size, size_t nmemb, size_t return_value)
{
    struct _nala_instance_type_for_fread *_nala_instance_p;

    nala_state_for_fread.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_in);
    _nala_instance_p->data.params.ptr_in_assert = NULL;
    _nala_instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.ptr = NULL;
    _nala_instance_p->data.params.ignore_ptr_in = true;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.params.size = size;
    _nala_instance_p->data.params.ignore_size_in = false;
    _nala_instance_p->data.params.nmemb = nmemb;
    _nala_instance_p->data.params.ignore_nmemb_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fread.instances,
                          _nala_instance_p);
}

void fread_mock_ignore_in(size_t return_value)
{
    nala_state_for_fread.state.mode = 3;
    nala_state_for_fread.data.params.ignore_ptr_in = true;
    nala_state_for_fread.data.params.ignore_stream_in = true;
    nala_state_for_fread.data.params.ignore_size_in = true;
    nala_state_for_fread.data.params.ignore_nmemb_in = true;
    nala_state_for_fread.data.return_value = return_value;
    nala_state_for_fread.data.errno_value = 0;
    nala_state_for_fread.data.callback = NULL;
}

void fread_mock_ignore_in_once(size_t return_value)
{
    struct _nala_instance_type_for_fread *instance_p;

    nala_state_for_fread.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fread.instances,
                          instance_p);
}

void fread_mock_set_errno(int errno_value)
{
    nala_get_data_fread()->errno_value = errno_value;
}

void fread_mock_set_callback(void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_get_data_fread()->callback = callback;
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
    nala_get_params_fread()->ptr_in_assert = callback;
}

void fread_mock_set_ptr_in_pointer(void *ptr)
{
    struct _nala_data_params_for_fread *_nala_params_p;

    _nala_params_p = nala_get_params_fread();
    _nala_params_p->ignore_ptr_in = false;
    _nala_params_p->ptr = ptr;
}

void fread_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->ptr_out,
                       buf_p,
                       size);
}

void fread_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fread()->ptr_out_copy = callback;
}

void fread_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->stream_in,
                       buf_p,
                       size);
}

void fread_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fread()->stream_in_assert = callback;
}

void fread_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fread *_nala_params_p;

    _nala_params_p = nala_get_params_fread();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fread_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fread()->stream_out,
                       buf_p,
                       size);
}

void fread_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fread()->stream_out_copy = callback;
}

void fread_mock_none(void)
{
    nala_state_for_fread.state.mode = 4;
}

void fread_mock_implementation(size_t (*implementation)(void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_state_for_fread.state.mode = 2;
    nala_state_for_fread.data.implementation = implementation;
}

void fread_mock_disable(void)
{
    nala_state_for_fread.state.mode = 0;
}

void fread_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fread.state);
}

void fread_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fread.state);
}

void fread_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fread);
}

void fread_mock_assert_completed(void)
{
    if (nala_state_for_fread.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fread() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fread.instances.length));
    }
}

// NALA_IMPLEMENTATION free

void __real_free(void *ptr);

struct _nala_data_params_for_free {
    void *ptr;
    bool ignore_ptr_in;
    struct nala_set_param ptr_in;
    void (*ptr_in_assert)(void *ptr, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param ptr_out;
    void (*ptr_out_copy)(void *ptr, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_free {
    struct _nala_data_params_for_free params;
    int errno_value;
    void (*implementation)(void *ptr);
    void (*callback)(void *ptr);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_free {
    struct _nala_data_type_for_free data;
    struct _nala_instance_type_for_free *next_p;
};

struct _nala_instances_type_for_free {
    struct _nala_instance_type_for_free *head_p;
    struct _nala_instance_type_for_free *tail_p;
    int length;
};

struct nala_state_type_for_free {
    struct nala_state_t state;
    struct _nala_data_type_for_free data;
    struct _nala_instances_type_for_free instances;
};

static struct nala_state_type_for_free nala_state_for_free = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_free *nala_get_data_free()
{
    if (nala_state_for_free.instances.tail_p != NULL) {
        return (&nala_state_for_free.instances.tail_p->data);
    } else {
        return (&nala_state_for_free.data);
    }
}

struct _nala_data_params_for_free *nala_get_params_free()
{
    return (&nala_get_data_free()->params);
}

void __wrap_free(void *ptr)
{
    struct _nala_instance_type_for_free *_nala_instance_p;
    struct _nala_data_type_for_free *_nala_data_p;

    nala_print_call("free", &nala_state_for_free.state);

    switch (nala_state_for_free.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_free.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_free.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked free() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_free.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, free, ptr);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   free,
                                   ptr);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(ptr);
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_free.data.implementation(ptr);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_free.state.mode = 3;
    nala_state_for_free.data.params.ptr = NULL;
    nala_state_for_free.data.params.ignore_ptr_in = true;
    nala_state_for_free.data.errno_value = 0;
    nala_state_for_free.data.callback = NULL;
}

void free_mock_once()
{
    struct _nala_instance_type_for_free *_nala_instance_p;

    nala_state_for_free.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_in);
    _nala_instance_p->data.params.ptr_in_assert = NULL;
    _nala_instance_p->data.params.ptr_out_copy = NULL;
    _nala_instance_p->data.params.ptr = NULL;
    _nala_instance_p->data.params.ignore_ptr_in = true;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_free.instances,
                          _nala_instance_p);
}

void free_mock_ignore_in(void)
{
    nala_state_for_free.state.mode = 3;
    nala_state_for_free.data.params.ignore_ptr_in = true;
    nala_state_for_free.data.errno_value = 0;
    nala_state_for_free.data.callback = NULL;
}

void free_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_free *instance_p;

    nala_state_for_free.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.ptr_out);
    nala_set_param_init(&instance_p->data.params.ptr_in);
    instance_p->data.params.ptr_in_assert = NULL;
    instance_p->data.params.ptr_out_copy = NULL;
    instance_p->data.params.ptr = NULL;
    instance_p->data.params.ignore_ptr_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_free.instances,
                          instance_p);
}

void free_mock_set_errno(int errno_value)
{
    nala_get_data_free()->errno_value = errno_value;
}

void free_mock_set_callback(void (*callback)(void *ptr))
{
    nala_get_data_free()->callback = callback;
}

void free_mock_set_ptr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_free()->ptr_in,
                       buf_p,
                       size);
}

void free_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_free()->ptr_in_assert = callback;
}

void free_mock_set_ptr_in_pointer(void *ptr)
{
    struct _nala_data_params_for_free *_nala_params_p;

    _nala_params_p = nala_get_params_free();
    _nala_params_p->ignore_ptr_in = false;
    _nala_params_p->ptr = ptr;
}

void free_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_free()->ptr_out,
                       buf_p,
                       size);
}

void free_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_free()->ptr_out_copy = callback;
}

void free_mock_none(void)
{
    nala_state_for_free.state.mode = 4;
}

void free_mock_implementation(void (*implementation)(void *ptr))
{
    nala_state_for_free.state.mode = 2;
    nala_state_for_free.data.implementation = implementation;
}

void free_mock_disable(void)
{
    nala_state_for_free.state.mode = 0;
}

void free_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_free.state);
}

void free_mock_resume(void)
{
    nala_state_resume(&nala_state_for_free.state);
}

void free_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_free);
}

void free_mock_assert_completed(void)
{
    if (nala_state_for_free.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked free() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_free.instances.length));
    }
}

// NALA_IMPLEMENTATION fseek

int __real_fseek(FILE *stream, long int offset, int whence);

struct _nala_data_params_for_fseek {
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

struct _nala_data_type_for_fseek {
    struct _nala_data_params_for_fseek params;
    int return_value;
    int errno_value;
    int (*implementation)(FILE *stream, long int offset, int whence);
    void (*callback)(FILE *stream, long int offset, int whence);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fseek {
    struct _nala_data_type_for_fseek data;
    struct _nala_instance_type_for_fseek *next_p;
};

struct _nala_instances_type_for_fseek {
    struct _nala_instance_type_for_fseek *head_p;
    struct _nala_instance_type_for_fseek *tail_p;
    int length;
};

struct nala_state_type_for_fseek {
    struct nala_state_t state;
    struct _nala_data_type_for_fseek data;
    struct _nala_instances_type_for_fseek instances;
};

static struct nala_state_type_for_fseek nala_state_for_fseek = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fseek *nala_get_data_fseek()
{
    if (nala_state_for_fseek.instances.tail_p != NULL) {
        return (&nala_state_for_fseek.instances.tail_p->data);
    } else {
        return (&nala_state_for_fseek.data);
    }
}

struct _nala_data_params_for_fseek *nala_get_params_fseek()
{
    return (&nala_get_data_fseek()->params);
}

int __wrap_fseek(FILE *stream, long int offset, int whence)
{
    struct _nala_instance_type_for_fseek *_nala_instance_p;
    struct _nala_data_type_for_fseek *_nala_data_p;
    int return_value;

    nala_print_call("fseek", &nala_state_for_fseek.state);

    switch (nala_state_for_fseek.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fseek.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fseek.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fseek() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fseek.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fseek, stream);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fseek, offset);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fseek, whence);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fseek,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream, offset, whence);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fseek.data.implementation(stream, offset, whence);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fseek.state.mode = 3;
    nala_state_for_fseek.data.params.stream = NULL;
    nala_state_for_fseek.data.params.ignore_stream_in = true;
    nala_state_for_fseek.data.params.offset = offset;
    nala_state_for_fseek.data.params.ignore_offset_in = false;
    nala_state_for_fseek.data.params.whence = whence;
    nala_state_for_fseek.data.params.ignore_whence_in = false;
    nala_state_for_fseek.data.return_value = return_value;
    nala_state_for_fseek.data.errno_value = 0;
    nala_state_for_fseek.data.callback = NULL;
}

void fseek_mock_once(long int offset, int whence, int return_value)
{
    struct _nala_instance_type_for_fseek *_nala_instance_p;

    nala_state_for_fseek.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.params.offset = offset;
    _nala_instance_p->data.params.ignore_offset_in = false;
    _nala_instance_p->data.params.whence = whence;
    _nala_instance_p->data.params.ignore_whence_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fseek.instances,
                          _nala_instance_p);
}

void fseek_mock_ignore_in(int return_value)
{
    nala_state_for_fseek.state.mode = 3;
    nala_state_for_fseek.data.params.ignore_stream_in = true;
    nala_state_for_fseek.data.params.ignore_offset_in = true;
    nala_state_for_fseek.data.params.ignore_whence_in = true;
    nala_state_for_fseek.data.return_value = return_value;
    nala_state_for_fseek.data.errno_value = 0;
    nala_state_for_fseek.data.callback = NULL;
}

void fseek_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_fseek *instance_p;

    nala_state_for_fseek.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fseek.instances,
                          instance_p);
}

void fseek_mock_set_errno(int errno_value)
{
    nala_get_data_fseek()->errno_value = errno_value;
}

void fseek_mock_set_callback(void (*callback)(FILE *stream, long int offset, int whence))
{
    nala_get_data_fseek()->callback = callback;
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
    nala_get_params_fseek()->stream_in_assert = callback;
}

void fseek_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fseek *_nala_params_p;

    _nala_params_p = nala_get_params_fseek();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fseek_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fseek()->stream_out,
                       buf_p,
                       size);
}

void fseek_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fseek()->stream_out_copy = callback;
}

void fseek_mock_none(void)
{
    nala_state_for_fseek.state.mode = 4;
}

void fseek_mock_implementation(int (*implementation)(FILE *stream, long int offset, int whence))
{
    nala_state_for_fseek.state.mode = 2;
    nala_state_for_fseek.data.implementation = implementation;
}

void fseek_mock_disable(void)
{
    nala_state_for_fseek.state.mode = 0;
}

void fseek_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fseek.state);
}

void fseek_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fseek.state);
}

void fseek_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fseek);
}

void fseek_mock_assert_completed(void)
{
    if (nala_state_for_fseek.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fseek() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fseek.instances.length));
    }
}

// NALA_IMPLEMENTATION ftell

long int __real_ftell(FILE *stream);

struct _nala_data_params_for_ftell {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_ftell {
    struct _nala_data_params_for_ftell params;
    long int return_value;
    int errno_value;
    long int (*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_ftell {
    struct _nala_data_type_for_ftell data;
    struct _nala_instance_type_for_ftell *next_p;
};

struct _nala_instances_type_for_ftell {
    struct _nala_instance_type_for_ftell *head_p;
    struct _nala_instance_type_for_ftell *tail_p;
    int length;
};

struct nala_state_type_for_ftell {
    struct nala_state_t state;
    struct _nala_data_type_for_ftell data;
    struct _nala_instances_type_for_ftell instances;
};

static struct nala_state_type_for_ftell nala_state_for_ftell = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_ftell *nala_get_data_ftell()
{
    if (nala_state_for_ftell.instances.tail_p != NULL) {
        return (&nala_state_for_ftell.instances.tail_p->data);
    } else {
        return (&nala_state_for_ftell.data);
    }
}

struct _nala_data_params_for_ftell *nala_get_params_ftell()
{
    return (&nala_get_data_ftell()->params);
}

long int __wrap_ftell(FILE *stream)
{
    struct _nala_instance_type_for_ftell *_nala_instance_p;
    struct _nala_data_type_for_ftell *_nala_data_p;
    long int return_value;

    nala_print_call("ftell", &nala_state_for_ftell.state);

    switch (nala_state_for_ftell.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_ftell.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_ftell.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked ftell() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_ftell.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, ftell, stream);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   ftell,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_ftell.data.implementation(stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_ftell.state.mode = 3;
    nala_state_for_ftell.data.params.stream = NULL;
    nala_state_for_ftell.data.params.ignore_stream_in = true;
    nala_state_for_ftell.data.return_value = return_value;
    nala_state_for_ftell.data.errno_value = 0;
    nala_state_for_ftell.data.callback = NULL;
}

void ftell_mock_once(long int return_value)
{
    struct _nala_instance_type_for_ftell *_nala_instance_p;

    nala_state_for_ftell.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_ftell.instances,
                          _nala_instance_p);
}

void ftell_mock_ignore_in(long int return_value)
{
    nala_state_for_ftell.state.mode = 3;
    nala_state_for_ftell.data.params.ignore_stream_in = true;
    nala_state_for_ftell.data.return_value = return_value;
    nala_state_for_ftell.data.errno_value = 0;
    nala_state_for_ftell.data.callback = NULL;
}

void ftell_mock_ignore_in_once(long int return_value)
{
    struct _nala_instance_type_for_ftell *instance_p;

    nala_state_for_ftell.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_ftell.instances,
                          instance_p);
}

void ftell_mock_set_errno(int errno_value)
{
    nala_get_data_ftell()->errno_value = errno_value;
}

void ftell_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_ftell()->callback = callback;
}

void ftell_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_ftell()->stream_in,
                       buf_p,
                       size);
}

void ftell_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_ftell()->stream_in_assert = callback;
}

void ftell_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_ftell *_nala_params_p;

    _nala_params_p = nala_get_params_ftell();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void ftell_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_ftell()->stream_out,
                       buf_p,
                       size);
}

void ftell_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_ftell()->stream_out_copy = callback;
}

void ftell_mock_none(void)
{
    nala_state_for_ftell.state.mode = 4;
}

void ftell_mock_implementation(long int (*implementation)(FILE *stream))
{
    nala_state_for_ftell.state.mode = 2;
    nala_state_for_ftell.data.implementation = implementation;
}

void ftell_mock_disable(void)
{
    nala_state_for_ftell.state.mode = 0;
}

void ftell_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_ftell.state);
}

void ftell_mock_resume(void)
{
    nala_state_resume(&nala_state_for_ftell.state);
}

void ftell_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_ftell);
}

void ftell_mock_assert_completed(void)
{
    if (nala_state_for_ftell.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked ftell() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_ftell.instances.length));
    }
}

// NALA_IMPLEMENTATION fwrite

size_t __real_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

struct _nala_data_params_for_fwrite {
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

struct _nala_data_type_for_fwrite {
    struct _nala_data_params_for_fwrite params;
    size_t return_value;
    int errno_value;
    size_t (*implementation)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_fwrite {
    struct _nala_data_type_for_fwrite data;
    struct _nala_instance_type_for_fwrite *next_p;
};

struct _nala_instances_type_for_fwrite {
    struct _nala_instance_type_for_fwrite *head_p;
    struct _nala_instance_type_for_fwrite *tail_p;
    int length;
};

struct nala_state_type_for_fwrite {
    struct nala_state_t state;
    struct _nala_data_type_for_fwrite data;
    struct _nala_instances_type_for_fwrite instances;
};

static struct nala_state_type_for_fwrite nala_state_for_fwrite = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_fwrite *nala_get_data_fwrite()
{
    if (nala_state_for_fwrite.instances.tail_p != NULL) {
        return (&nala_state_for_fwrite.instances.tail_p->data);
    } else {
        return (&nala_state_for_fwrite.data);
    }
}

struct _nala_data_params_for_fwrite *nala_get_params_fwrite()
{
    return (&nala_get_data_fwrite()->params);
}

size_t __wrap_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    struct _nala_instance_type_for_fwrite *_nala_instance_p;
    struct _nala_data_type_for_fwrite *_nala_data_p;
    size_t return_value;

    nala_print_call("fwrite", &nala_state_for_fwrite.state);

    switch (nala_state_for_fwrite.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_fwrite.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_fwrite.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked fwrite() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_fwrite.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, fwrite, ptr);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fwrite, stream);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fwrite, size);
        MOCK_ASSERT_IN_EQ(_nala_data_p, fwrite, nmemb);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fwrite,
                                   ptr);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   fwrite,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(ptr, size, nmemb, stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_fwrite.data.implementation(ptr, size, nmemb, stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_fwrite.state.mode = 3;
    nala_state_for_fwrite.data.params.ptr = NULL;
    nala_state_for_fwrite.data.params.ignore_ptr_in = true;
    nala_state_for_fwrite.data.params.stream = NULL;
    nala_state_for_fwrite.data.params.ignore_stream_in = true;
    nala_state_for_fwrite.data.params.size = size;
    nala_state_for_fwrite.data.params.ignore_size_in = false;
    nala_state_for_fwrite.data.params.nmemb = nmemb;
    nala_state_for_fwrite.data.params.ignore_nmemb_in = false;
    nala_state_for_fwrite.data.return_value = return_value;
    nala_state_for_fwrite.data.errno_value = 0;
    nala_state_for_fwrite.data.callback = NULL;
}

void fwrite_mock_once(size_t size, size_t nmemb, size_t return_value)
{
    struct _nala_instance_type_for_fwrite *_nala_instance_p;

    nala_state_for_fwrite.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_out);
    nala_set_param_init(&_nala_instance_p->data.params.ptr_in);
    _nala_instance_p->data.params.ptr_in_assert = NULL;
    _nala_instance_p->data.params.ptr_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.ptr = NULL;
    _nala_instance_p->data.params.ignore_ptr_in = true;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.params.size = size;
    _nala_instance_p->data.params.ignore_size_in = false;
    _nala_instance_p->data.params.nmemb = nmemb;
    _nala_instance_p->data.params.ignore_nmemb_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fwrite.instances,
                          _nala_instance_p);
}

void fwrite_mock_ignore_in(size_t return_value)
{
    nala_state_for_fwrite.state.mode = 3;
    nala_state_for_fwrite.data.params.ignore_ptr_in = true;
    nala_state_for_fwrite.data.params.ignore_stream_in = true;
    nala_state_for_fwrite.data.params.ignore_size_in = true;
    nala_state_for_fwrite.data.params.ignore_nmemb_in = true;
    nala_state_for_fwrite.data.return_value = return_value;
    nala_state_for_fwrite.data.errno_value = 0;
    nala_state_for_fwrite.data.callback = NULL;
}

void fwrite_mock_ignore_in_once(size_t return_value)
{
    struct _nala_instance_type_for_fwrite *instance_p;

    nala_state_for_fwrite.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_fwrite.instances,
                          instance_p);
}

void fwrite_mock_set_errno(int errno_value)
{
    nala_get_data_fwrite()->errno_value = errno_value;
}

void fwrite_mock_set_callback(void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_get_data_fwrite()->callback = callback;
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
    nala_get_params_fwrite()->ptr_in_assert = callback;
}

void fwrite_mock_set_ptr_in_pointer(const void *ptr)
{
    struct _nala_data_params_for_fwrite *_nala_params_p;

    _nala_params_p = nala_get_params_fwrite();
    _nala_params_p->ignore_ptr_in = false;
    _nala_params_p->ptr = ptr;
}

void fwrite_mock_set_ptr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->ptr_out,
                       buf_p,
                       size);
}

void fwrite_mock_set_ptr_out_copy(void (*callback)(const void *ptr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fwrite()->ptr_out_copy = callback;
}

void fwrite_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->stream_in,
                       buf_p,
                       size);
}

void fwrite_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fwrite()->stream_in_assert = callback;
}

void fwrite_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_fwrite *_nala_params_p;

    _nala_params_p = nala_get_params_fwrite();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void fwrite_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_fwrite()->stream_out,
                       buf_p,
                       size);
}

void fwrite_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_fwrite()->stream_out_copy = callback;
}

void fwrite_mock_none(void)
{
    nala_state_for_fwrite.state.mode = 4;
}

void fwrite_mock_implementation(size_t (*implementation)(const void *ptr, size_t size, size_t nmemb, FILE *stream))
{
    nala_state_for_fwrite.state.mode = 2;
    nala_state_for_fwrite.data.implementation = implementation;
}

void fwrite_mock_disable(void)
{
    nala_state_for_fwrite.state.mode = 0;
}

void fwrite_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_fwrite.state);
}

void fwrite_mock_resume(void)
{
    nala_state_resume(&nala_state_for_fwrite.state);
}

void fwrite_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_fwrite);
}

void fwrite_mock_assert_completed(void)
{
    if (nala_state_for_fwrite.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked fwrite() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_fwrite.instances.length));
    }
}

// NALA_IMPLEMENTATION getmntent

struct mntent *__real_getmntent(FILE *stream);

struct _nala_data_params_for_getmntent {
    FILE *stream;
    bool ignore_stream_in;
    struct nala_set_param stream_in;
    void (*stream_in_assert)(FILE *stream, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param stream_out;
    void (*stream_out_copy)(FILE *stream, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_getmntent {
    struct _nala_data_params_for_getmntent params;
    struct mntent *return_value;
    int errno_value;
    struct mntent *(*implementation)(FILE *stream);
    void (*callback)(FILE *stream);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_getmntent {
    struct _nala_data_type_for_getmntent data;
    struct _nala_instance_type_for_getmntent *next_p;
};

struct _nala_instances_type_for_getmntent {
    struct _nala_instance_type_for_getmntent *head_p;
    struct _nala_instance_type_for_getmntent *tail_p;
    int length;
};

struct nala_state_type_for_getmntent {
    struct nala_state_t state;
    struct _nala_data_type_for_getmntent data;
    struct _nala_instances_type_for_getmntent instances;
};

static struct nala_state_type_for_getmntent nala_state_for_getmntent = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_getmntent *nala_get_data_getmntent()
{
    if (nala_state_for_getmntent.instances.tail_p != NULL) {
        return (&nala_state_for_getmntent.instances.tail_p->data);
    } else {
        return (&nala_state_for_getmntent.data);
    }
}

struct _nala_data_params_for_getmntent *nala_get_params_getmntent()
{
    return (&nala_get_data_getmntent()->params);
}

struct mntent *__wrap_getmntent(FILE *stream)
{
    struct _nala_instance_type_for_getmntent *_nala_instance_p;
    struct _nala_data_type_for_getmntent *_nala_data_p;
    struct mntent *return_value;

    nala_print_call("getmntent", &nala_state_for_getmntent.state);

    switch (nala_state_for_getmntent.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_getmntent.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_getmntent.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked getmntent() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_getmntent.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, getmntent, stream);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   getmntent,
                                   stream);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(stream);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_getmntent.data.implementation(stream);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_getmntent.state.mode = 3;
    nala_state_for_getmntent.data.params.stream = NULL;
    nala_state_for_getmntent.data.params.ignore_stream_in = true;
    nala_state_for_getmntent.data.return_value = return_value;
    nala_state_for_getmntent.data.errno_value = 0;
    nala_state_for_getmntent.data.callback = NULL;
}

void getmntent_mock_once(struct mntent *return_value)
{
    struct _nala_instance_type_for_getmntent *_nala_instance_p;

    nala_state_for_getmntent.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.stream_out);
    nala_set_param_init(&_nala_instance_p->data.params.stream_in);
    _nala_instance_p->data.params.stream_in_assert = NULL;
    _nala_instance_p->data.params.stream_out_copy = NULL;
    _nala_instance_p->data.params.stream = NULL;
    _nala_instance_p->data.params.ignore_stream_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_getmntent.instances,
                          _nala_instance_p);
}

void getmntent_mock_ignore_in(struct mntent *return_value)
{
    nala_state_for_getmntent.state.mode = 3;
    nala_state_for_getmntent.data.params.ignore_stream_in = true;
    nala_state_for_getmntent.data.return_value = return_value;
    nala_state_for_getmntent.data.errno_value = 0;
    nala_state_for_getmntent.data.callback = NULL;
}

void getmntent_mock_ignore_in_once(struct mntent *return_value)
{
    struct _nala_instance_type_for_getmntent *instance_p;

    nala_state_for_getmntent.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.stream_out);
    nala_set_param_init(&instance_p->data.params.stream_in);
    instance_p->data.params.stream_in_assert = NULL;
    instance_p->data.params.stream_out_copy = NULL;
    instance_p->data.params.stream = NULL;
    instance_p->data.params.ignore_stream_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_getmntent.instances,
                          instance_p);
}

void getmntent_mock_set_errno(int errno_value)
{
    nala_get_data_getmntent()->errno_value = errno_value;
}

void getmntent_mock_set_callback(void (*callback)(FILE *stream))
{
    nala_get_data_getmntent()->callback = callback;
}

void getmntent_mock_set_stream_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_getmntent()->stream_in,
                       buf_p,
                       size);
}

void getmntent_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_getmntent()->stream_in_assert = callback;
}

void getmntent_mock_set_stream_in_pointer(FILE *stream)
{
    struct _nala_data_params_for_getmntent *_nala_params_p;

    _nala_params_p = nala_get_params_getmntent();
    _nala_params_p->ignore_stream_in = false;
    _nala_params_p->stream = stream;
}

void getmntent_mock_set_stream_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_getmntent()->stream_out,
                       buf_p,
                       size);
}

void getmntent_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_getmntent()->stream_out_copy = callback;
}

void getmntent_mock_none(void)
{
    nala_state_for_getmntent.state.mode = 4;
}

void getmntent_mock_implementation(struct mntent *(*implementation)(FILE *stream))
{
    nala_state_for_getmntent.state.mode = 2;
    nala_state_for_getmntent.data.implementation = implementation;
}

void getmntent_mock_disable(void)
{
    nala_state_for_getmntent.state.mode = 0;
}

void getmntent_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_getmntent.state);
}

void getmntent_mock_resume(void)
{
    nala_state_resume(&nala_state_for_getmntent.state);
}

void getmntent_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_getmntent);
}

void getmntent_mock_assert_completed(void)
{
    if (nala_state_for_getmntent.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked getmntent() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_getmntent.instances.length));
    }
}

// NALA_IMPLEMENTATION in_out

void __real_in_out(int *buf_p);

struct _nala_data_params_for_in_out {
    int *buf_p;
    bool ignore_buf_p_in;
    struct nala_set_param buf_p_in;
    void (*buf_p_in_assert)(int *buf_p, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param buf_p_out;
    void (*buf_p_out_copy)(int *buf_p, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_in_out {
    struct _nala_data_params_for_in_out params;
    int errno_value;
    void (*implementation)(int *buf_p);
    void (*callback)(int *buf_p);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_in_out {
    struct _nala_data_type_for_in_out data;
    struct _nala_instance_type_for_in_out *next_p;
};

struct _nala_instances_type_for_in_out {
    struct _nala_instance_type_for_in_out *head_p;
    struct _nala_instance_type_for_in_out *tail_p;
    int length;
};

struct nala_state_type_for_in_out {
    struct nala_state_t state;
    struct _nala_data_type_for_in_out data;
    struct _nala_instances_type_for_in_out instances;
};

static struct nala_state_type_for_in_out nala_state_for_in_out = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_in_out *nala_get_data_in_out()
{
    if (nala_state_for_in_out.instances.tail_p != NULL) {
        return (&nala_state_for_in_out.instances.tail_p->data);
    } else {
        return (&nala_state_for_in_out.data);
    }
}

struct _nala_data_params_for_in_out *nala_get_params_in_out()
{
    return (&nala_get_data_in_out()->params);
}

void __wrap_in_out(int *buf_p)
{
    struct _nala_instance_type_for_in_out *_nala_instance_p;
    struct _nala_data_type_for_in_out *_nala_data_p;

    nala_print_call("in_out", &nala_state_for_in_out.state);

    switch (nala_state_for_in_out.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_in_out.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_in_out.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked in_out() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_in_out.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, in_out, buf_p);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   in_out,
                                   buf_p);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(buf_p);
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_in_out.data.implementation(buf_p);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_in_out.state.mode = 3;
    nala_state_for_in_out.data.params.buf_p = NULL;
    nala_state_for_in_out.data.params.ignore_buf_p_in = true;
    nala_state_for_in_out.data.errno_value = 0;
    nala_state_for_in_out.data.callback = NULL;
}

void in_out_mock_once()
{
    struct _nala_instance_type_for_in_out *_nala_instance_p;

    nala_state_for_in_out.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.buf_p_out);
    nala_set_param_init(&_nala_instance_p->data.params.buf_p_in);
    _nala_instance_p->data.params.buf_p_in_assert = NULL;
    _nala_instance_p->data.params.buf_p_out_copy = NULL;
    _nala_instance_p->data.params.buf_p = NULL;
    _nala_instance_p->data.params.ignore_buf_p_in = true;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_in_out.instances,
                          _nala_instance_p);
}

void in_out_mock_ignore_in(void)
{
    nala_state_for_in_out.state.mode = 3;
    nala_state_for_in_out.data.params.ignore_buf_p_in = true;
    nala_state_for_in_out.data.errno_value = 0;
    nala_state_for_in_out.data.callback = NULL;
}

void in_out_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_in_out *instance_p;

    nala_state_for_in_out.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.buf_p_out);
    nala_set_param_init(&instance_p->data.params.buf_p_in);
    instance_p->data.params.buf_p_in_assert = NULL;
    instance_p->data.params.buf_p_out_copy = NULL;
    instance_p->data.params.buf_p = NULL;
    instance_p->data.params.ignore_buf_p_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_in_out.instances,
                          instance_p);
}

void in_out_mock_set_errno(int errno_value)
{
    nala_get_data_in_out()->errno_value = errno_value;
}

void in_out_mock_set_callback(void (*callback)(int *buf_p))
{
    nala_get_data_in_out()->callback = callback;
}

void in_out_mock_set_buf_p_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_in_out()->buf_p_in,
                       buf_p,
                       size);
}

void in_out_mock_set_buf_p_in_assert(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_in_out()->buf_p_in_assert = callback;
}

void in_out_mock_set_buf_p_in_pointer(int *buf_p)
{
    struct _nala_data_params_for_in_out *_nala_params_p;

    _nala_params_p = nala_get_params_in_out();
    _nala_params_p->ignore_buf_p_in = false;
    _nala_params_p->buf_p = buf_p;
}

void in_out_mock_set_buf_p_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_in_out()->buf_p_out,
                       buf_p,
                       size);
}

void in_out_mock_set_buf_p_out_copy(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_in_out()->buf_p_out_copy = callback;
}

void in_out_mock_none(void)
{
    nala_state_for_in_out.state.mode = 4;
}

void in_out_mock_implementation(void (*implementation)(int *buf_p))
{
    nala_state_for_in_out.state.mode = 2;
    nala_state_for_in_out.data.implementation = implementation;
}

void in_out_mock_disable(void)
{
    nala_state_for_in_out.state.mode = 0;
}

void in_out_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_in_out.state);
}

void in_out_mock_resume(void)
{
    nala_state_resume(&nala_state_for_in_out.state);
}

void in_out_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_in_out);
}

void in_out_mock_assert_completed(void)
{
    if (nala_state_for_in_out.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked in_out() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_in_out.instances.length));
    }
}

// NALA_IMPLEMENTATION io_control

struct _nala_data_params_for_io_control {
    int kind;
    const char *vafmt_p;
    bool ignore_kind_in;
    struct nala_va_arg_list_t _nala_va_arg_list;
};

struct _nala_data_type_for_io_control {
    struct _nala_data_params_for_io_control params;
    int return_value;
    int errno_value;
    int (*implementation)(int kind, va_list __nala_va_list);
    void (*callback)(int kind, va_list __nala_va_list);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_io_control {
    struct _nala_data_type_for_io_control data;
    struct _nala_instance_type_for_io_control *next_p;
};

struct _nala_instances_type_for_io_control {
    struct _nala_instance_type_for_io_control *head_p;
    struct _nala_instance_type_for_io_control *tail_p;
    int length;
};

struct nala_state_type_for_io_control {
    struct nala_state_t state;
    struct _nala_data_type_for_io_control data;
    struct _nala_instances_type_for_io_control instances;
};

static struct nala_state_type_for_io_control nala_state_for_io_control = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_io_control *nala_get_data_io_control()
{
    if (nala_state_for_io_control.instances.tail_p != NULL) {
        return (&nala_state_for_io_control.instances.tail_p->data);
    } else {
        return (&nala_state_for_io_control.data);
    }
}

struct _nala_data_params_for_io_control *nala_get_params_io_control()
{
    return (&nala_get_data_io_control()->params);
}

int __wrap_io_control(int kind, ...)
{
    struct _nala_instance_type_for_io_control *_nala_instance_p;
    struct _nala_data_type_for_io_control *_nala_data_p;
    int return_value;

    nala_print_call("io_control", &nala_state_for_io_control.state);

    switch (nala_state_for_io_control.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_io_control.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_io_control.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked io_control() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_io_control.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, io_control, kind);


        {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            nala_va_arg_list_assert(&_nala_data_p->params._nala_va_arg_list, __nala_vl);
            va_end(__nala_vl);
            nala_va_arg_list_destroy(&_nala_data_p->params._nala_va_arg_list);
        }

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            _nala_data_p->callback(kind, __nala_vl);
            va_end(__nala_vl);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            return_value =
            nala_state_for_io_control.data.implementation(kind, __nala_vl);
            va_end(__nala_vl);
        }
        break;

    case 4:
        nala_mock_none_fail();
        exit(1);
        break;

    default:
        {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            return_value =
            io_control_mock_va_arg_real(kind, __nala_vl);
            va_end(__nala_vl);
        }
        break;
    }

    return return_value;
}

void io_control_mock(int kind, int return_value, const char *vafmt_p, ...)
{
    nala_state_for_io_control.state.mode = 3;

    if (vafmt_p == NULL) {
        NALA_TEST_FAILURE(nala_format("Variadic format cannot be NULL.\n"));
    }

    nala_state_for_io_control.data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&nala_state_for_io_control.data.params._nala_va_arg_list);
    va_list __nala_vl;
    va_start(__nala_vl, vafmt_p);
    nala_parse_va_list(&nala_state_for_io_control.data.params._nala_va_arg_list,
                       vafmt_p,
                       __nala_vl);
    va_end(__nala_vl);
    nala_state_for_io_control.data.params.kind = kind;
    nala_state_for_io_control.data.params.ignore_kind_in = false;
    nala_state_for_io_control.data.return_value = return_value;
    nala_state_for_io_control.data.errno_value = 0;
    nala_state_for_io_control.data.callback = NULL;
}

void io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...)
{
    struct _nala_instance_type_for_io_control *_nala_instance_p;

    nala_state_for_io_control.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);

    if (vafmt_p == NULL) {
        NALA_TEST_FAILURE(nala_format("Variadic format cannot be NULL.\n"));
    }

    _nala_instance_p->data.params.vafmt_p = vafmt_p;
    nala_va_arg_list_init(&_nala_instance_p->data.params._nala_va_arg_list);
    va_list __nala_vl;
    va_start(__nala_vl, vafmt_p);
    nala_parse_va_list(&_nala_instance_p->data.params._nala_va_arg_list,
                       vafmt_p,
                       __nala_vl);
    va_end(__nala_vl);
    _nala_instance_p->data.params.kind = kind;
    _nala_instance_p->data.params.ignore_kind_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_io_control.instances,
                          _nala_instance_p);
}

void io_control_mock_ignore_in(int return_value)
{
    nala_state_for_io_control.state.mode = 3;
    nala_state_for_io_control.data.params.ignore_kind_in = true;
    nala_state_for_io_control.data.return_value = return_value;
    nala_state_for_io_control.data.errno_value = 0;
    nala_state_for_io_control.data.callback = NULL;
}

void io_control_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_io_control *instance_p;

    nala_state_for_io_control.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.vafmt_p = "";
    nala_va_arg_list_init(&instance_p->data.params._nala_va_arg_list);
    instance_p->data.params.ignore_kind_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_io_control.instances,
                          instance_p);
}

void io_control_mock_set_errno(int errno_value)
{
    nala_get_data_io_control()->errno_value = errno_value;
}

void io_control_mock_set_callback(void (*callback)(int kind, va_list __nala_va_list))
{
    nala_get_data_io_control()->callback = callback;
}

void io_control_mock_ignore_kind_in(void)
{
    nala_get_params_io_control()->ignore_kind_in = true;
}

void io_control_mock_ignore_va_arg_in_at(unsigned int index)
{
    struct nala_va_arg_list_t *va_arg_list_p;

    va_arg_list_p = &nala_get_params_io_control()->_nala_va_arg_list;
    nala_va_arg_list_get(va_arg_list_p, index)->ignore_in = true;
}

void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_io_control()->_nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);
    nala_set_param_buf(&item_p->in, buf_p, size);
}

void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p)
{
    (void)index;
    (void)buf_p;

    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = nala_state_for_io_control.instances.tail_p;

    if (instance_p == NULL) {
        NALA_TEST_FAILURE(nala_format(
            "io_control_mock_set_va_arg_in_pointer_at(...) not implemented "
            "for mock state.\n"));
    }
}

void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size)
{
    struct nala_va_arg_list_t *va_arg_list_p;
    struct nala_va_arg_item_t *item_p;

    va_arg_list_p = &nala_get_params_io_control()->_nala_va_arg_list;
    item_p = nala_va_arg_list_get(va_arg_list_p, index);
    nala_set_param_buf(&item_p->out, buf_p, size);
}

void io_control_mock_none(void)
{
    nala_state_for_io_control.state.mode = 4;
}

void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list))
{
    nala_state_for_io_control.state.mode = 2;
    nala_state_for_io_control.data.implementation = implementation;
}

void io_control_mock_disable(void)
{
    nala_state_for_io_control.state.mode = 0;
}

void io_control_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_io_control.state);
}

void io_control_mock_resume(void)
{
    nala_state_resume(&nala_state_for_io_control.state);
}

void io_control_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_io_control);
}

void io_control_mock_assert_completed(void)
{
    if (nala_state_for_io_control.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked io_control() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_io_control.instances.length));
    }
}

// NALA_IMPLEMENTATION io_vcontrol

int __real_io_vcontrol(int kind, va_list ap);

struct _nala_data_params_for_io_vcontrol {
    int kind;
    va_list ap;
    bool ignore_kind_in;
};

struct _nala_data_type_for_io_vcontrol {
    struct _nala_data_params_for_io_vcontrol params;
    int return_value;
    int errno_value;
    int (*implementation)(int kind, va_list ap);
    void (*callback)(int kind, va_list ap);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_io_vcontrol {
    struct _nala_data_type_for_io_vcontrol data;
    struct _nala_instance_type_for_io_vcontrol *next_p;
};

struct _nala_instances_type_for_io_vcontrol {
    struct _nala_instance_type_for_io_vcontrol *head_p;
    struct _nala_instance_type_for_io_vcontrol *tail_p;
    int length;
};

struct nala_state_type_for_io_vcontrol {
    struct nala_state_t state;
    struct _nala_data_type_for_io_vcontrol data;
    struct _nala_instances_type_for_io_vcontrol instances;
};

static struct nala_state_type_for_io_vcontrol nala_state_for_io_vcontrol = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_io_vcontrol *nala_get_data_io_vcontrol()
{
    if (nala_state_for_io_vcontrol.instances.tail_p != NULL) {
        return (&nala_state_for_io_vcontrol.instances.tail_p->data);
    } else {
        return (&nala_state_for_io_vcontrol.data);
    }
}

struct _nala_data_params_for_io_vcontrol *nala_get_params_io_vcontrol()
{
    return (&nala_get_data_io_vcontrol()->params);
}

int __wrap_io_vcontrol(int kind, va_list ap)
{
    struct _nala_instance_type_for_io_vcontrol *_nala_instance_p;
    struct _nala_data_type_for_io_vcontrol *_nala_data_p;
    int return_value;

    nala_print_call("io_vcontrol", &nala_state_for_io_vcontrol.state);

    switch (nala_state_for_io_vcontrol.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_io_vcontrol.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_io_vcontrol.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked io_vcontrol() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_io_vcontrol.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, io_vcontrol, kind);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(kind, ap);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_io_vcontrol.data.implementation(kind, ap);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_io_vcontrol.state.mode = 3;
    nala_state_for_io_vcontrol.data.params.kind = kind;
    nala_state_for_io_vcontrol.data.params.ignore_kind_in = false;
    nala_state_for_io_vcontrol.data.return_value = return_value;
    nala_state_for_io_vcontrol.data.errno_value = 0;
    nala_state_for_io_vcontrol.data.callback = NULL;
}

void io_vcontrol_mock_once(int kind, int return_value)
{
    struct _nala_instance_type_for_io_vcontrol *_nala_instance_p;

    nala_state_for_io_vcontrol.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.kind = kind;
    _nala_instance_p->data.params.ignore_kind_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_io_vcontrol.instances,
                          _nala_instance_p);
}

void io_vcontrol_mock_ignore_in(int return_value)
{
    nala_state_for_io_vcontrol.state.mode = 3;
    nala_state_for_io_vcontrol.data.params.ignore_kind_in = true;
    nala_state_for_io_vcontrol.data.return_value = return_value;
    nala_state_for_io_vcontrol.data.errno_value = 0;
    nala_state_for_io_vcontrol.data.callback = NULL;
}

void io_vcontrol_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_io_vcontrol *instance_p;

    nala_state_for_io_vcontrol.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_kind_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_io_vcontrol.instances,
                          instance_p);
}

void io_vcontrol_mock_set_errno(int errno_value)
{
    nala_get_data_io_vcontrol()->errno_value = errno_value;
}

void io_vcontrol_mock_set_callback(void (*callback)(int kind, va_list ap))
{
    nala_get_data_io_vcontrol()->callback = callback;
}

void io_vcontrol_mock_ignore_kind_in(void)
{
    nala_get_params_io_vcontrol()->ignore_kind_in = true;
}

void io_vcontrol_mock_none(void)
{
    nala_state_for_io_vcontrol.state.mode = 4;
}

void io_vcontrol_mock_implementation(int (*implementation)(int kind, va_list ap))
{
    nala_state_for_io_vcontrol.state.mode = 2;
    nala_state_for_io_vcontrol.data.implementation = implementation;
}

void io_vcontrol_mock_disable(void)
{
    nala_state_for_io_vcontrol.state.mode = 0;
}

void io_vcontrol_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_io_vcontrol.state);
}

void io_vcontrol_mock_resume(void)
{
    nala_state_resume(&nala_state_for_io_vcontrol.state);
}

void io_vcontrol_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_io_vcontrol);
}

void io_vcontrol_mock_assert_completed(void)
{
    if (nala_state_for_io_vcontrol.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked io_vcontrol() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_io_vcontrol.instances.length));
    }
}

// NALA_IMPLEMENTATION malloc

void *__real_malloc(size_t size);

struct _nala_data_params_for_malloc {
    size_t size;
    bool ignore_size_in;
};

struct _nala_data_type_for_malloc {
    struct _nala_data_params_for_malloc params;
    void *return_value;
    int errno_value;
    void *(*implementation)(size_t size);
    void (*callback)(size_t size);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_malloc {
    struct _nala_data_type_for_malloc data;
    struct _nala_instance_type_for_malloc *next_p;
};

struct _nala_instances_type_for_malloc {
    struct _nala_instance_type_for_malloc *head_p;
    struct _nala_instance_type_for_malloc *tail_p;
    int length;
};

struct nala_state_type_for_malloc {
    struct nala_state_t state;
    struct _nala_data_type_for_malloc data;
    struct _nala_instances_type_for_malloc instances;
};

static struct nala_state_type_for_malloc nala_state_for_malloc = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_malloc *nala_get_data_malloc()
{
    if (nala_state_for_malloc.instances.tail_p != NULL) {
        return (&nala_state_for_malloc.instances.tail_p->data);
    } else {
        return (&nala_state_for_malloc.data);
    }
}

struct _nala_data_params_for_malloc *nala_get_params_malloc()
{
    return (&nala_get_data_malloc()->params);
}

void *__wrap_malloc(size_t size)
{
    struct _nala_instance_type_for_malloc *_nala_instance_p;
    struct _nala_data_type_for_malloc *_nala_data_p;
    void *return_value;

    nala_print_call("malloc", &nala_state_for_malloc.state);

    switch (nala_state_for_malloc.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_malloc.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_malloc.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked malloc() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_malloc.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, malloc, size);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(size);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_malloc.data.implementation(size);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_malloc.state.mode = 3;
    nala_state_for_malloc.data.params.size = size;
    nala_state_for_malloc.data.params.ignore_size_in = false;
    nala_state_for_malloc.data.return_value = return_value;
    nala_state_for_malloc.data.errno_value = 0;
    nala_state_for_malloc.data.callback = NULL;
}

void malloc_mock_once(size_t size, void *return_value)
{
    struct _nala_instance_type_for_malloc *_nala_instance_p;

    nala_state_for_malloc.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.size = size;
    _nala_instance_p->data.params.ignore_size_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_malloc.instances,
                          _nala_instance_p);
}

void malloc_mock_ignore_in(void *return_value)
{
    nala_state_for_malloc.state.mode = 3;
    nala_state_for_malloc.data.params.ignore_size_in = true;
    nala_state_for_malloc.data.return_value = return_value;
    nala_state_for_malloc.data.errno_value = 0;
    nala_state_for_malloc.data.callback = NULL;
}

void malloc_mock_ignore_in_once(void *return_value)
{
    struct _nala_instance_type_for_malloc *instance_p;

    nala_state_for_malloc.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_size_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_malloc.instances,
                          instance_p);
}

void malloc_mock_set_errno(int errno_value)
{
    nala_get_data_malloc()->errno_value = errno_value;
}

void malloc_mock_set_callback(void (*callback)(size_t size))
{
    nala_get_data_malloc()->callback = callback;
}

void malloc_mock_ignore_size_in(void)
{
    nala_get_params_malloc()->ignore_size_in = true;
}

void malloc_mock_none(void)
{
    nala_state_for_malloc.state.mode = 4;
}

void malloc_mock_implementation(void *(*implementation)(size_t size))
{
    nala_state_for_malloc.state.mode = 2;
    nala_state_for_malloc.data.implementation = implementation;
}

void malloc_mock_disable(void)
{
    nala_state_for_malloc.state.mode = 0;
}

void malloc_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_malloc.state);
}

void malloc_mock_resume(void)
{
    nala_state_resume(&nala_state_for_malloc.state);
}

void malloc_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_malloc);
}

void malloc_mock_assert_completed(void)
{
    if (nala_state_for_malloc.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked malloc() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_malloc.instances.length));
    }
}

// NALA_IMPLEMENTATION mount

int __real_mount(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);

struct _nala_data_params_for_mount {
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

struct _nala_data_type_for_mount {
    struct _nala_data_params_for_mount params;
    int return_value;
    int errno_value;
    int (*implementation)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);
    void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_mount {
    struct _nala_data_type_for_mount data;
    struct _nala_instance_type_for_mount *next_p;
};

struct _nala_instances_type_for_mount {
    struct _nala_instance_type_for_mount *head_p;
    struct _nala_instance_type_for_mount *tail_p;
    int length;
};

struct nala_state_type_for_mount {
    struct nala_state_t state;
    struct _nala_data_type_for_mount data;
    struct _nala_instances_type_for_mount instances;
};

static struct nala_state_type_for_mount nala_state_for_mount = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_mount *nala_get_data_mount()
{
    if (nala_state_for_mount.instances.tail_p != NULL) {
        return (&nala_state_for_mount.instances.tail_p->data);
    } else {
        return (&nala_state_for_mount.data);
    }
}

struct _nala_data_params_for_mount *nala_get_params_mount()
{
    return (&nala_get_data_mount()->params);
}

int __wrap_mount(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data)
{
    struct _nala_instance_type_for_mount *_nala_instance_p;
    struct _nala_data_type_for_mount *_nala_data_p;
    int return_value;

    nala_print_call("mount", &nala_state_for_mount.state);

    switch (nala_state_for_mount.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_mount.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_mount.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked mount() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_mount.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, mount, source);
        MOCK_ASSERT_IN_EQ(_nala_data_p, mount, target);
        MOCK_ASSERT_IN_EQ(_nala_data_p, mount, filesystemtype);
        MOCK_ASSERT_IN_EQ(_nala_data_p, mount, data);
        MOCK_ASSERT_IN_EQ(_nala_data_p, mount, mountflags);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   mount,
                                   source);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   mount,
                                   target);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   mount,
                                   filesystemtype);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   mount,
                                   data);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(source, target, filesystemtype, mountflags, data);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_mount.data.implementation(source, target, filesystemtype, mountflags, data);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_mount.state.mode = 3;
    nala_state_for_mount.data.params.source = NULL;
    nala_state_for_mount.data.params.ignore_source_in = true;

    if (source != NULL) {
        nala_set_param_string(&nala_state_for_mount.data.params.source_in,
                              source);
    } else {
        nala_state_for_mount.data.params.ignore_source_in = false;
    }

    nala_state_for_mount.data.params.target = NULL;
    nala_state_for_mount.data.params.ignore_target_in = true;

    if (target != NULL) {
        nala_set_param_string(&nala_state_for_mount.data.params.target_in,
                              target);
    } else {
        nala_state_for_mount.data.params.ignore_target_in = false;
    }

    nala_state_for_mount.data.params.filesystemtype = NULL;
    nala_state_for_mount.data.params.ignore_filesystemtype_in = true;

    if (filesystemtype != NULL) {
        nala_set_param_string(&nala_state_for_mount.data.params.filesystemtype_in,
                              filesystemtype);
    } else {
        nala_state_for_mount.data.params.ignore_filesystemtype_in = false;
    }

    nala_state_for_mount.data.params.data = NULL;
    nala_state_for_mount.data.params.ignore_data_in = true;
    nala_state_for_mount.data.params.mountflags = mountflags;
    nala_state_for_mount.data.params.ignore_mountflags_in = false;
    nala_state_for_mount.data.return_value = return_value;
    nala_state_for_mount.data.errno_value = 0;
    nala_state_for_mount.data.callback = NULL;
}

void mount_mock_once(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value)
{
    struct _nala_instance_type_for_mount *_nala_instance_p;

    nala_state_for_mount.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.source_out);
    nala_set_param_init(&_nala_instance_p->data.params.source_in);
    _nala_instance_p->data.params.source_in_assert = NULL;
    _nala_instance_p->data.params.source_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.target_out);
    nala_set_param_init(&_nala_instance_p->data.params.target_in);
    _nala_instance_p->data.params.target_in_assert = NULL;
    _nala_instance_p->data.params.target_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.filesystemtype_out);
    nala_set_param_init(&_nala_instance_p->data.params.filesystemtype_in);
    _nala_instance_p->data.params.filesystemtype_in_assert = NULL;
    _nala_instance_p->data.params.filesystemtype_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.data_out);
    nala_set_param_init(&_nala_instance_p->data.params.data_in);
    _nala_instance_p->data.params.data_in_assert = NULL;
    _nala_instance_p->data.params.data_out_copy = NULL;
    _nala_instance_p->data.params.source = NULL;
    _nala_instance_p->data.params.ignore_source_in = true;

    if (source != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.source_in,
                              source);
    } else {
        _nala_instance_p->data.params.ignore_source_in = false;
    }

    _nala_instance_p->data.params.target = NULL;
    _nala_instance_p->data.params.ignore_target_in = true;

    if (target != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.target_in,
                              target);
    } else {
        _nala_instance_p->data.params.ignore_target_in = false;
    }

    _nala_instance_p->data.params.filesystemtype = NULL;
    _nala_instance_p->data.params.ignore_filesystemtype_in = true;

    if (filesystemtype != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.filesystemtype_in,
                              filesystemtype);
    } else {
        _nala_instance_p->data.params.ignore_filesystemtype_in = false;
    }

    _nala_instance_p->data.params.data = NULL;
    _nala_instance_p->data.params.ignore_data_in = true;
    _nala_instance_p->data.params.mountflags = mountflags;
    _nala_instance_p->data.params.ignore_mountflags_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_mount.instances,
                          _nala_instance_p);
}

void mount_mock_ignore_in(int return_value)
{
    nala_state_for_mount.state.mode = 3;
    nala_state_for_mount.data.params.ignore_source_in = true;
    nala_state_for_mount.data.params.ignore_target_in = true;
    nala_state_for_mount.data.params.ignore_filesystemtype_in = true;
    nala_state_for_mount.data.params.ignore_data_in = true;
    nala_state_for_mount.data.params.ignore_mountflags_in = true;
    nala_state_for_mount.data.return_value = return_value;
    nala_state_for_mount.data.errno_value = 0;
    nala_state_for_mount.data.callback = NULL;
}

void mount_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_mount *instance_p;

    nala_state_for_mount.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_mount.instances,
                          instance_p);
}

void mount_mock_set_errno(int errno_value)
{
    nala_get_data_mount()->errno_value = errno_value;
}

void mount_mock_set_callback(void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data))
{
    nala_get_data_mount()->callback = callback;
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
    nala_get_params_mount()->source_in_assert = callback;
}

void mount_mock_set_source_in_pointer(const char *source)
{
    struct _nala_data_params_for_mount *_nala_params_p;

    _nala_params_p = nala_get_params_mount();
    _nala_params_p->ignore_source_in = false;
    _nala_params_p->source = source;
}

void mount_mock_set_source_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->source_out,
                       buf_p,
                       size);
}

void mount_mock_set_source_out_copy(void (*callback)(const char *source, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->source_out_copy = callback;
}

void mount_mock_set_target_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->target_in,
                       buf_p,
                       size);
}

void mount_mock_set_target_in_assert(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->target_in_assert = callback;
}

void mount_mock_set_target_in_pointer(const char *target)
{
    struct _nala_data_params_for_mount *_nala_params_p;

    _nala_params_p = nala_get_params_mount();
    _nala_params_p->ignore_target_in = false;
    _nala_params_p->target = target;
}

void mount_mock_set_target_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->target_out,
                       buf_p,
                       size);
}

void mount_mock_set_target_out_copy(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->target_out_copy = callback;
}

void mount_mock_set_filesystemtype_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->filesystemtype_in,
                       buf_p,
                       size);
}

void mount_mock_set_filesystemtype_in_assert(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->filesystemtype_in_assert = callback;
}

void mount_mock_set_filesystemtype_in_pointer(const char *filesystemtype)
{
    struct _nala_data_params_for_mount *_nala_params_p;

    _nala_params_p = nala_get_params_mount();
    _nala_params_p->ignore_filesystemtype_in = false;
    _nala_params_p->filesystemtype = filesystemtype;
}

void mount_mock_set_filesystemtype_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->filesystemtype_out,
                       buf_p,
                       size);
}

void mount_mock_set_filesystemtype_out_copy(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->filesystemtype_out_copy = callback;
}

void mount_mock_set_data_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->data_in,
                       buf_p,
                       size);
}

void mount_mock_set_data_in_assert(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->data_in_assert = callback;
}

void mount_mock_set_data_in_pointer(const void *data)
{
    struct _nala_data_params_for_mount *_nala_params_p;

    _nala_params_p = nala_get_params_mount();
    _nala_params_p->ignore_data_in = false;
    _nala_params_p->data = data;
}

void mount_mock_set_data_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_mount()->data_out,
                       buf_p,
                       size);
}

void mount_mock_set_data_out_copy(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_mount()->data_out_copy = callback;
}

void mount_mock_none(void)
{
    nala_state_for_mount.state.mode = 4;
}

void mount_mock_implementation(int (*implementation)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data))
{
    nala_state_for_mount.state.mode = 2;
    nala_state_for_mount.data.implementation = implementation;
}

void mount_mock_disable(void)
{
    nala_state_for_mount.state.mode = 0;
}

void mount_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_mount.state);
}

void mount_mock_resume(void)
{
    nala_state_resume(&nala_state_for_mount.state);
}

void mount_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_mount);
}

void mount_mock_assert_completed(void)
{
    if (nala_state_for_mount.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked mount() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_mount.instances.length));
    }
}

// NALA_IMPLEMENTATION output_message

void __real_output_message(const char *message);

struct _nala_data_params_for_output_message {
    const char *message;
    bool ignore_message_in;
    struct nala_set_param message_in;
    void (*message_in_assert)(const char *message, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param message_out;
    void (*message_out_copy)(const char *message, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_output_message {
    struct _nala_data_params_for_output_message params;
    int errno_value;
    void (*implementation)(const char *message);
    void (*callback)(const char *message);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_output_message {
    struct _nala_data_type_for_output_message data;
    struct _nala_instance_type_for_output_message *next_p;
};

struct _nala_instances_type_for_output_message {
    struct _nala_instance_type_for_output_message *head_p;
    struct _nala_instance_type_for_output_message *tail_p;
    int length;
};

struct nala_state_type_for_output_message {
    struct nala_state_t state;
    struct _nala_data_type_for_output_message data;
    struct _nala_instances_type_for_output_message instances;
};

static struct nala_state_type_for_output_message nala_state_for_output_message = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_output_message *nala_get_data_output_message()
{
    if (nala_state_for_output_message.instances.tail_p != NULL) {
        return (&nala_state_for_output_message.instances.tail_p->data);
    } else {
        return (&nala_state_for_output_message.data);
    }
}

struct _nala_data_params_for_output_message *nala_get_params_output_message()
{
    return (&nala_get_data_output_message()->params);
}

void __wrap_output_message(const char *message)
{
    struct _nala_instance_type_for_output_message *_nala_instance_p;
    struct _nala_data_type_for_output_message *_nala_data_p;

    nala_print_call("output_message", &nala_state_for_output_message.state);

    switch (nala_state_for_output_message.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_output_message.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_output_message.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked output_message() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_output_message.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, output_message, message);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   output_message,
                                   message);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(message);
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_output_message.data.implementation(message);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_output_message.state.mode = 3;
    nala_state_for_output_message.data.params.message = NULL;
    nala_state_for_output_message.data.params.ignore_message_in = true;

    if (message != NULL) {
        nala_set_param_string(&nala_state_for_output_message.data.params.message_in,
                              message);
    } else {
        nala_state_for_output_message.data.params.ignore_message_in = false;
    }

    nala_state_for_output_message.data.errno_value = 0;
    nala_state_for_output_message.data.callback = NULL;
}

void output_message_mock_once(const char *message)
{
    struct _nala_instance_type_for_output_message *_nala_instance_p;

    nala_state_for_output_message.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.message_out);
    nala_set_param_init(&_nala_instance_p->data.params.message_in);
    _nala_instance_p->data.params.message_in_assert = NULL;
    _nala_instance_p->data.params.message_out_copy = NULL;
    _nala_instance_p->data.params.message = NULL;
    _nala_instance_p->data.params.ignore_message_in = true;

    if (message != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.message_in,
                              message);
    } else {
        _nala_instance_p->data.params.ignore_message_in = false;
    }

    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_output_message.instances,
                          _nala_instance_p);
}

void output_message_mock_ignore_in(void)
{
    nala_state_for_output_message.state.mode = 3;
    nala_state_for_output_message.data.params.ignore_message_in = true;
    nala_state_for_output_message.data.errno_value = 0;
    nala_state_for_output_message.data.callback = NULL;
}

void output_message_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_output_message *instance_p;

    nala_state_for_output_message.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.message_out);
    nala_set_param_init(&instance_p->data.params.message_in);
    instance_p->data.params.message_in_assert = NULL;
    instance_p->data.params.message_out_copy = NULL;
    instance_p->data.params.message = NULL;
    instance_p->data.params.ignore_message_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_output_message.instances,
                          instance_p);
}

void output_message_mock_set_errno(int errno_value)
{
    nala_get_data_output_message()->errno_value = errno_value;
}

void output_message_mock_set_callback(void (*callback)(const char *message))
{
    nala_get_data_output_message()->callback = callback;
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
    nala_get_params_output_message()->message_in_assert = callback;
}

void output_message_mock_set_message_in_pointer(const char *message)
{
    struct _nala_data_params_for_output_message *_nala_params_p;

    _nala_params_p = nala_get_params_output_message();
    _nala_params_p->ignore_message_in = false;
    _nala_params_p->message = message;
}

void output_message_mock_set_message_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_output_message()->message_out,
                       buf_p,
                       size);
}

void output_message_mock_set_message_out_copy(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_output_message()->message_out_copy = callback;
}

void output_message_mock_none(void)
{
    nala_state_for_output_message.state.mode = 4;
}

void output_message_mock_implementation(void (*implementation)(const char *message))
{
    nala_state_for_output_message.state.mode = 2;
    nala_state_for_output_message.data.implementation = implementation;
}

void output_message_mock_disable(void)
{
    nala_state_for_output_message.state.mode = 0;
}

void output_message_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_output_message.state);
}

void output_message_mock_resume(void)
{
    nala_state_resume(&nala_state_for_output_message.state);
}

void output_message_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_output_message);
}

void output_message_mock_assert_completed(void)
{
    if (nala_state_for_output_message.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked output_message() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_output_message.instances.length));
    }
}

// NALA_IMPLEMENTATION pipe

int __real_pipe(int pipefd[2]);

struct _nala_data_params_for_pipe {
    int *pipefd;
    bool ignore_pipefd_in;
    struct nala_set_param pipefd_in;
    void (*pipefd_in_assert)(int pipefd[2], const void *nala_buf_p, size_t nala_size);
    struct nala_set_param pipefd_out;
    void (*pipefd_out_copy)(int pipefd[2], const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_pipe {
    struct _nala_data_params_for_pipe params;
    int return_value;
    int errno_value;
    int (*implementation)(int pipefd[2]);
    void (*callback)(int pipefd[2]);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_pipe {
    struct _nala_data_type_for_pipe data;
    struct _nala_instance_type_for_pipe *next_p;
};

struct _nala_instances_type_for_pipe {
    struct _nala_instance_type_for_pipe *head_p;
    struct _nala_instance_type_for_pipe *tail_p;
    int length;
};

struct nala_state_type_for_pipe {
    struct nala_state_t state;
    struct _nala_data_type_for_pipe data;
    struct _nala_instances_type_for_pipe instances;
};

static struct nala_state_type_for_pipe nala_state_for_pipe = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_pipe *nala_get_data_pipe()
{
    if (nala_state_for_pipe.instances.tail_p != NULL) {
        return (&nala_state_for_pipe.instances.tail_p->data);
    } else {
        return (&nala_state_for_pipe.data);
    }
}

struct _nala_data_params_for_pipe *nala_get_params_pipe()
{
    return (&nala_get_data_pipe()->params);
}

int __wrap_pipe(int pipefd[2])
{
    struct _nala_instance_type_for_pipe *_nala_instance_p;
    struct _nala_data_type_for_pipe *_nala_data_p;
    int return_value;

    nala_print_call("pipe", &nala_state_for_pipe.state);

    switch (nala_state_for_pipe.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_pipe.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_pipe.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked pipe() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_pipe.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, pipe, pipefd);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   pipe,
                                   pipefd);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(pipefd);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_pipe.data.implementation(pipefd);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_pipe.state.mode = 3;
    nala_state_for_pipe.data.params.pipefd = NULL;
    nala_state_for_pipe.data.params.ignore_pipefd_in = true;
    nala_state_for_pipe.data.return_value = return_value;
    nala_state_for_pipe.data.errno_value = 0;
    nala_state_for_pipe.data.callback = NULL;
}

void pipe_mock_once(int return_value)
{
    struct _nala_instance_type_for_pipe *_nala_instance_p;

    nala_state_for_pipe.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.pipefd_out);
    nala_set_param_init(&_nala_instance_p->data.params.pipefd_in);
    _nala_instance_p->data.params.pipefd_in_assert = NULL;
    _nala_instance_p->data.params.pipefd_out_copy = NULL;
    _nala_instance_p->data.params.pipefd = NULL;
    _nala_instance_p->data.params.ignore_pipefd_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_pipe.instances,
                          _nala_instance_p);
}

void pipe_mock_ignore_in(int return_value)
{
    nala_state_for_pipe.state.mode = 3;
    nala_state_for_pipe.data.params.ignore_pipefd_in = true;
    nala_state_for_pipe.data.return_value = return_value;
    nala_state_for_pipe.data.errno_value = 0;
    nala_state_for_pipe.data.callback = NULL;
}

void pipe_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_pipe *instance_p;

    nala_state_for_pipe.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.pipefd_out);
    nala_set_param_init(&instance_p->data.params.pipefd_in);
    instance_p->data.params.pipefd_in_assert = NULL;
    instance_p->data.params.pipefd_out_copy = NULL;
    instance_p->data.params.pipefd = NULL;
    instance_p->data.params.ignore_pipefd_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_pipe.instances,
                          instance_p);
}

void pipe_mock_set_errno(int errno_value)
{
    nala_get_data_pipe()->errno_value = errno_value;
}

void pipe_mock_set_callback(void (*callback)(int pipefd[2]))
{
    nala_get_data_pipe()->callback = callback;
}

void pipe_mock_set_pipefd_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_pipe()->pipefd_in,
                       buf_p,
                       size);
}

void pipe_mock_set_pipefd_in_assert(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_pipe()->pipefd_in_assert = callback;
}

void pipe_mock_set_pipefd_in_pointer(int pipefd[2])
{
    struct _nala_data_params_for_pipe *_nala_params_p;

    _nala_params_p = nala_get_params_pipe();
    _nala_params_p->ignore_pipefd_in = false;
    _nala_params_p->pipefd = pipefd;
}

void pipe_mock_set_pipefd_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_pipe()->pipefd_out,
                       buf_p,
                       size);
}

void pipe_mock_set_pipefd_out_copy(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_pipe()->pipefd_out_copy = callback;
}

void pipe_mock_none(void)
{
    nala_state_for_pipe.state.mode = 4;
}

void pipe_mock_implementation(int (*implementation)(int pipefd[2]))
{
    nala_state_for_pipe.state.mode = 2;
    nala_state_for_pipe.data.implementation = implementation;
}

void pipe_mock_disable(void)
{
    nala_state_for_pipe.state.mode = 0;
}

void pipe_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_pipe.state);
}

void pipe_mock_resume(void)
{
    nala_state_resume(&nala_state_for_pipe.state);
}

void pipe_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_pipe);
}

void pipe_mock_assert_completed(void)
{
    if (nala_state_for_pipe.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked pipe() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_pipe.instances.length));
    }
}

// NALA_IMPLEMENTATION poll

int __real_poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct _nala_data_params_for_poll {
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

struct _nala_data_type_for_poll {
    struct _nala_data_params_for_poll params;
    int return_value;
    int errno_value;
    int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout);
    void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_poll {
    struct _nala_data_type_for_poll data;
    struct _nala_instance_type_for_poll *next_p;
};

struct _nala_instances_type_for_poll {
    struct _nala_instance_type_for_poll *head_p;
    struct _nala_instance_type_for_poll *tail_p;
    int length;
};

struct nala_state_type_for_poll {
    struct nala_state_t state;
    struct _nala_data_type_for_poll data;
    struct _nala_instances_type_for_poll instances;
};

static struct nala_state_type_for_poll nala_state_for_poll = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_poll *nala_get_data_poll()
{
    if (nala_state_for_poll.instances.tail_p != NULL) {
        return (&nala_state_for_poll.instances.tail_p->data);
    } else {
        return (&nala_state_for_poll.data);
    }
}

struct _nala_data_params_for_poll *nala_get_params_poll()
{
    return (&nala_get_data_poll()->params);
}

int __wrap_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    struct _nala_instance_type_for_poll *_nala_instance_p;
    struct _nala_data_type_for_poll *_nala_data_p;
    int return_value;

    nala_print_call("poll", &nala_state_for_poll.state);

    switch (nala_state_for_poll.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_poll.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_poll.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked poll() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_poll.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, poll, fds);
        MOCK_ASSERT_IN_EQ(_nala_data_p, poll, nfds);
        MOCK_ASSERT_IN_EQ(_nala_data_p, poll, timeout);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_pollfd,
                                   poll,
                                   fds);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(fds, nfds, timeout);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_poll.data.implementation(fds, nfds, timeout);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_poll.state.mode = 3;
    nala_state_for_poll.data.params.fds = NULL;
    nala_state_for_poll.data.params.ignore_fds_in = true;
    nala_state_for_poll.data.params.nfds = nfds;
    nala_state_for_poll.data.params.ignore_nfds_in = false;
    nala_state_for_poll.data.params.timeout = timeout;
    nala_state_for_poll.data.params.ignore_timeout_in = false;
    nala_state_for_poll.data.return_value = return_value;
    nala_state_for_poll.data.errno_value = 0;
    nala_state_for_poll.data.callback = NULL;
}

void poll_mock_once(nfds_t nfds, int timeout, int return_value)
{
    struct _nala_instance_type_for_poll *_nala_instance_p;

    nala_state_for_poll.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.fds_out);
    nala_set_param_init(&_nala_instance_p->data.params.fds_in);
    _nala_instance_p->data.params.fds_in_assert = NULL;
    _nala_instance_p->data.params.fds_out_copy = NULL;
    _nala_instance_p->data.params.fds = NULL;
    _nala_instance_p->data.params.ignore_fds_in = true;
    _nala_instance_p->data.params.nfds = nfds;
    _nala_instance_p->data.params.ignore_nfds_in = false;
    _nala_instance_p->data.params.timeout = timeout;
    _nala_instance_p->data.params.ignore_timeout_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_poll.instances,
                          _nala_instance_p);
}

void poll_mock_ignore_in(int return_value)
{
    nala_state_for_poll.state.mode = 3;
    nala_state_for_poll.data.params.ignore_fds_in = true;
    nala_state_for_poll.data.params.ignore_nfds_in = true;
    nala_state_for_poll.data.params.ignore_timeout_in = true;
    nala_state_for_poll.data.return_value = return_value;
    nala_state_for_poll.data.errno_value = 0;
    nala_state_for_poll.data.callback = NULL;
}

void poll_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_poll *instance_p;

    nala_state_for_poll.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_poll.instances,
                          instance_p);
}

void poll_mock_set_errno(int errno_value)
{
    nala_get_data_poll()->errno_value = errno_value;
}

void poll_mock_set_callback(void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout))
{
    nala_get_data_poll()->callback = callback;
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
    nala_get_params_poll()->fds_in_assert = callback;
}

void poll_mock_set_fds_in_pointer(struct pollfd *fds)
{
    struct _nala_data_params_for_poll *_nala_params_p;

    _nala_params_p = nala_get_params_poll();
    _nala_params_p->ignore_fds_in = false;
    _nala_params_p->fds = fds;
}

void poll_mock_set_fds_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_poll()->fds_out,
                       buf_p,
                       size);
}

void poll_mock_set_fds_out_copy(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_poll()->fds_out_copy = callback;
}

void poll_mock_none(void)
{
    nala_state_for_poll.state.mode = 4;
}

void poll_mock_implementation(int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout))
{
    nala_state_for_poll.state.mode = 2;
    nala_state_for_poll.data.implementation = implementation;
}

void poll_mock_disable(void)
{
    nala_state_for_poll.state.mode = 0;
}

void poll_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_poll.state);
}

void poll_mock_resume(void)
{
    nala_state_resume(&nala_state_for_poll.state);
}

void poll_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_poll);
}

void poll_mock_assert_completed(void)
{
    if (nala_state_for_poll.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked poll() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_poll.instances.length));
    }
}

// NALA_IMPLEMENTATION print_hello

void __real_print_hello();

struct _nala_data_params_for_print_hello {
    int dummy;
};

struct _nala_data_type_for_print_hello {
    struct _nala_data_params_for_print_hello params;
    int errno_value;
    void (*implementation)();
    void (*callback)();
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_print_hello {
    struct _nala_data_type_for_print_hello data;
    struct _nala_instance_type_for_print_hello *next_p;
};

struct _nala_instances_type_for_print_hello {
    struct _nala_instance_type_for_print_hello *head_p;
    struct _nala_instance_type_for_print_hello *tail_p;
    int length;
};

struct nala_state_type_for_print_hello {
    struct nala_state_t state;
    struct _nala_data_type_for_print_hello data;
    struct _nala_instances_type_for_print_hello instances;
};

static struct nala_state_type_for_print_hello nala_state_for_print_hello = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_print_hello *nala_get_data_print_hello()
{
    if (nala_state_for_print_hello.instances.tail_p != NULL) {
        return (&nala_state_for_print_hello.instances.tail_p->data);
    } else {
        return (&nala_state_for_print_hello.data);
    }
}

struct _nala_data_params_for_print_hello *nala_get_params_print_hello()
{
    return (&nala_get_data_print_hello()->params);
}

void __wrap_print_hello()
{
    struct _nala_instance_type_for_print_hello *_nala_instance_p;
    struct _nala_data_type_for_print_hello *_nala_data_p;

    nala_print_call("print_hello", &nala_state_for_print_hello.state);

    switch (nala_state_for_print_hello.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_print_hello.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_print_hello.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked print_hello() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_print_hello.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback();
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_print_hello.data.implementation();
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_print_hello.state.mode = 3;
    nala_state_for_print_hello.data.errno_value = 0;
    nala_state_for_print_hello.data.callback = NULL;
}

void print_hello_mock_once()
{
    struct _nala_instance_type_for_print_hello *_nala_instance_p;

    nala_state_for_print_hello.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_print_hello.instances,
                          _nala_instance_p);
}

void print_hello_mock_ignore_in(void)
{
    nala_state_for_print_hello.state.mode = 3;
    nala_state_for_print_hello.data.errno_value = 0;
    nala_state_for_print_hello.data.callback = NULL;
}

void print_hello_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_print_hello *instance_p;

    nala_state_for_print_hello.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_print_hello.instances,
                          instance_p);
}

void print_hello_mock_set_errno(int errno_value)
{
    nala_get_data_print_hello()->errno_value = errno_value;
}

void print_hello_mock_set_callback(void (*callback)())
{
    nala_get_data_print_hello()->callback = callback;
}

void print_hello_mock_none(void)
{
    nala_state_for_print_hello.state.mode = 4;
}

void print_hello_mock_implementation(void (*implementation)())
{
    nala_state_for_print_hello.state.mode = 2;
    nala_state_for_print_hello.data.implementation = implementation;
}

void print_hello_mock_disable(void)
{
    nala_state_for_print_hello.state.mode = 0;
}

void print_hello_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_print_hello.state);
}

void print_hello_mock_resume(void)
{
    nala_state_resume(&nala_state_for_print_hello.state);
}

void print_hello_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_print_hello);
}

void print_hello_mock_assert_completed(void)
{
    if (nala_state_for_print_hello.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked print_hello() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_print_hello.instances.length));
    }
}

// NALA_IMPLEMENTATION read

ssize_t __real_read(int fd, void *buf, size_t count);

struct _nala_data_params_for_read {
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

struct _nala_data_type_for_read {
    struct _nala_data_params_for_read params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int fd, void *buf, size_t count);
    void (*callback)(int fd, void *buf, size_t count);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_read {
    struct _nala_data_type_for_read data;
    struct _nala_instance_type_for_read *next_p;
};

struct _nala_instances_type_for_read {
    struct _nala_instance_type_for_read *head_p;
    struct _nala_instance_type_for_read *tail_p;
    int length;
};

struct nala_state_type_for_read {
    struct nala_state_t state;
    struct _nala_data_type_for_read data;
    struct _nala_instances_type_for_read instances;
};

static struct nala_state_type_for_read nala_state_for_read = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_read *nala_get_data_read()
{
    if (nala_state_for_read.instances.tail_p != NULL) {
        return (&nala_state_for_read.instances.tail_p->data);
    } else {
        return (&nala_state_for_read.data);
    }
}

struct _nala_data_params_for_read *nala_get_params_read()
{
    return (&nala_get_data_read()->params);
}

ssize_t __wrap_read(int fd, void *buf, size_t count)
{
    struct _nala_instance_type_for_read *_nala_instance_p;
    struct _nala_data_type_for_read *_nala_data_p;
    ssize_t return_value;

    nala_print_call("read", &nala_state_for_read.state);

    switch (nala_state_for_read.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_read.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_read.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked read() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_read.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, read, buf);
        MOCK_ASSERT_IN_EQ(_nala_data_p, read, fd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, read, count);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   read,
                                   buf);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(fd, buf, count);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_read.data.implementation(fd, buf, count);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_read.state.mode = 3;
    nala_state_for_read.data.params.buf = NULL;
    nala_state_for_read.data.params.ignore_buf_in = true;
    nala_state_for_read.data.params.fd = fd;
    nala_state_for_read.data.params.ignore_fd_in = false;
    nala_state_for_read.data.params.count = count;
    nala_state_for_read.data.params.ignore_count_in = false;
    nala_state_for_read.data.return_value = return_value;
    nala_state_for_read.data.errno_value = 0;
    nala_state_for_read.data.callback = NULL;
}

void read_mock_once(int fd, size_t count, ssize_t return_value)
{
    struct _nala_instance_type_for_read *_nala_instance_p;

    nala_state_for_read.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.buf_out);
    nala_set_param_init(&_nala_instance_p->data.params.buf_in);
    _nala_instance_p->data.params.buf_in_assert = NULL;
    _nala_instance_p->data.params.buf_out_copy = NULL;
    _nala_instance_p->data.params.buf = NULL;
    _nala_instance_p->data.params.ignore_buf_in = true;
    _nala_instance_p->data.params.fd = fd;
    _nala_instance_p->data.params.ignore_fd_in = false;
    _nala_instance_p->data.params.count = count;
    _nala_instance_p->data.params.ignore_count_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_read.instances,
                          _nala_instance_p);
}

void read_mock_ignore_in(ssize_t return_value)
{
    nala_state_for_read.state.mode = 3;
    nala_state_for_read.data.params.ignore_buf_in = true;
    nala_state_for_read.data.params.ignore_fd_in = true;
    nala_state_for_read.data.params.ignore_count_in = true;
    nala_state_for_read.data.return_value = return_value;
    nala_state_for_read.data.errno_value = 0;
    nala_state_for_read.data.callback = NULL;
}

void read_mock_ignore_in_once(ssize_t return_value)
{
    struct _nala_instance_type_for_read *instance_p;

    nala_state_for_read.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_read.instances,
                          instance_p);
}

void read_mock_set_errno(int errno_value)
{
    nala_get_data_read()->errno_value = errno_value;
}

void read_mock_set_callback(void (*callback)(int fd, void *buf, size_t count))
{
    nala_get_data_read()->callback = callback;
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
    nala_get_params_read()->buf_in_assert = callback;
}

void read_mock_set_buf_in_pointer(void *buf)
{
    struct _nala_data_params_for_read *_nala_params_p;

    _nala_params_p = nala_get_params_read();
    _nala_params_p->ignore_buf_in = false;
    _nala_params_p->buf = buf;
}

void read_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_read()->buf_out,
                       buf_p,
                       size);
}

void read_mock_set_buf_out_copy(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_read()->buf_out_copy = callback;
}

void read_mock_none(void)
{
    nala_state_for_read.state.mode = 4;
}

void read_mock_implementation(ssize_t (*implementation)(int fd, void *buf, size_t count))
{
    nala_state_for_read.state.mode = 2;
    nala_state_for_read.data.implementation = implementation;
}

void read_mock_disable(void)
{
    nala_state_for_read.state.mode = 0;
}

void read_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_read.state);
}

void read_mock_resume(void)
{
    nala_state_resume(&nala_state_for_read.state);
}

void read_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_read);
}

void read_mock_assert_completed(void)
{
    if (nala_state_for_read.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked read() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_read.instances.length));
    }
}

// NALA_IMPLEMENTATION sendto

ssize_t __real_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

struct _nala_data_params_for_sendto {
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

struct _nala_data_type_for_sendto {
    struct _nala_data_params_for_sendto params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_sendto {
    struct _nala_data_type_for_sendto data;
    struct _nala_instance_type_for_sendto *next_p;
};

struct _nala_instances_type_for_sendto {
    struct _nala_instance_type_for_sendto *head_p;
    struct _nala_instance_type_for_sendto *tail_p;
    int length;
};

struct nala_state_type_for_sendto {
    struct nala_state_t state;
    struct _nala_data_type_for_sendto data;
    struct _nala_instances_type_for_sendto instances;
};

static struct nala_state_type_for_sendto nala_state_for_sendto = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_sendto *nala_get_data_sendto()
{
    if (nala_state_for_sendto.instances.tail_p != NULL) {
        return (&nala_state_for_sendto.instances.tail_p->data);
    } else {
        return (&nala_state_for_sendto.data);
    }
}

struct _nala_data_params_for_sendto *nala_get_params_sendto()
{
    return (&nala_get_data_sendto()->params);
}

ssize_t __wrap_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    struct _nala_instance_type_for_sendto *_nala_instance_p;
    struct _nala_data_type_for_sendto *_nala_data_p;
    ssize_t return_value;

    nala_print_call("sendto", &nala_state_for_sendto.state);

    switch (nala_state_for_sendto.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_sendto.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_sendto.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked sendto() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_sendto.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, buf);
        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, dest_addr);
        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, sockfd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, len);
        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, flags);
        MOCK_ASSERT_IN_EQ(_nala_data_p, sendto, addrlen);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   sendto,
                                   buf);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_sockaddr,
                                   sendto,
                                   dest_addr);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(sockfd, buf, len, flags, dest_addr, addrlen);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_sendto.data.implementation(sockfd, buf, len, flags, dest_addr, addrlen);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_sendto.state.mode = 3;
    nala_state_for_sendto.data.params.buf = NULL;
    nala_state_for_sendto.data.params.ignore_buf_in = true;
    nala_state_for_sendto.data.params.dest_addr = NULL;
    nala_state_for_sendto.data.params.ignore_dest_addr_in = true;
    nala_state_for_sendto.data.params.sockfd = sockfd;
    nala_state_for_sendto.data.params.ignore_sockfd_in = false;
    nala_state_for_sendto.data.params.len = len;
    nala_state_for_sendto.data.params.ignore_len_in = false;
    nala_state_for_sendto.data.params.flags = flags;
    nala_state_for_sendto.data.params.ignore_flags_in = false;
    nala_state_for_sendto.data.params.addrlen = addrlen;
    nala_state_for_sendto.data.params.ignore_addrlen_in = false;
    nala_state_for_sendto.data.return_value = return_value;
    nala_state_for_sendto.data.errno_value = 0;
    nala_state_for_sendto.data.callback = NULL;
}

void sendto_mock_once(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value)
{
    struct _nala_instance_type_for_sendto *_nala_instance_p;

    nala_state_for_sendto.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.buf_out);
    nala_set_param_init(&_nala_instance_p->data.params.buf_in);
    _nala_instance_p->data.params.buf_in_assert = NULL;
    _nala_instance_p->data.params.buf_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.dest_addr_out);
    nala_set_param_init(&_nala_instance_p->data.params.dest_addr_in);
    _nala_instance_p->data.params.dest_addr_in_assert = NULL;
    _nala_instance_p->data.params.dest_addr_out_copy = NULL;
    _nala_instance_p->data.params.buf = NULL;
    _nala_instance_p->data.params.ignore_buf_in = true;
    _nala_instance_p->data.params.dest_addr = NULL;
    _nala_instance_p->data.params.ignore_dest_addr_in = true;
    _nala_instance_p->data.params.sockfd = sockfd;
    _nala_instance_p->data.params.ignore_sockfd_in = false;
    _nala_instance_p->data.params.len = len;
    _nala_instance_p->data.params.ignore_len_in = false;
    _nala_instance_p->data.params.flags = flags;
    _nala_instance_p->data.params.ignore_flags_in = false;
    _nala_instance_p->data.params.addrlen = addrlen;
    _nala_instance_p->data.params.ignore_addrlen_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_sendto.instances,
                          _nala_instance_p);
}

void sendto_mock_ignore_in(ssize_t return_value)
{
    nala_state_for_sendto.state.mode = 3;
    nala_state_for_sendto.data.params.ignore_buf_in = true;
    nala_state_for_sendto.data.params.ignore_dest_addr_in = true;
    nala_state_for_sendto.data.params.ignore_sockfd_in = true;
    nala_state_for_sendto.data.params.ignore_len_in = true;
    nala_state_for_sendto.data.params.ignore_flags_in = true;
    nala_state_for_sendto.data.params.ignore_addrlen_in = true;
    nala_state_for_sendto.data.return_value = return_value;
    nala_state_for_sendto.data.errno_value = 0;
    nala_state_for_sendto.data.callback = NULL;
}

void sendto_mock_ignore_in_once(ssize_t return_value)
{
    struct _nala_instance_type_for_sendto *instance_p;

    nala_state_for_sendto.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_sendto.instances,
                          instance_p);
}

void sendto_mock_set_errno(int errno_value)
{
    nala_get_data_sendto()->errno_value = errno_value;
}

void sendto_mock_set_callback(void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen))
{
    nala_get_data_sendto()->callback = callback;
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
    nala_get_params_sendto()->buf_in_assert = callback;
}

void sendto_mock_set_buf_in_pointer(const void *buf)
{
    struct _nala_data_params_for_sendto *_nala_params_p;

    _nala_params_p = nala_get_params_sendto();
    _nala_params_p->ignore_buf_in = false;
    _nala_params_p->buf = buf;
}

void sendto_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->buf_out,
                       buf_p,
                       size);
}

void sendto_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_sendto()->buf_out_copy = callback;
}

void sendto_mock_set_dest_addr_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->dest_addr_in,
                       buf_p,
                       size);
}

void sendto_mock_set_dest_addr_in_assert(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_sendto()->dest_addr_in_assert = callback;
}

void sendto_mock_set_dest_addr_in_pointer(const struct sockaddr *dest_addr)
{
    struct _nala_data_params_for_sendto *_nala_params_p;

    _nala_params_p = nala_get_params_sendto();
    _nala_params_p->ignore_dest_addr_in = false;
    _nala_params_p->dest_addr = dest_addr;
}

void sendto_mock_set_dest_addr_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_sendto()->dest_addr_out,
                       buf_p,
                       size);
}

void sendto_mock_set_dest_addr_out_copy(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_sendto()->dest_addr_out_copy = callback;
}

void sendto_mock_none(void)
{
    nala_state_for_sendto.state.mode = 4;
}

void sendto_mock_implementation(ssize_t (*implementation)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen))
{
    nala_state_for_sendto.state.mode = 2;
    nala_state_for_sendto.data.implementation = implementation;
}

void sendto_mock_disable(void)
{
    nala_state_for_sendto.state.mode = 0;
}

void sendto_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_sendto.state);
}

void sendto_mock_resume(void)
{
    nala_state_resume(&nala_state_for_sendto.state);
}

void sendto_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_sendto);
}

void sendto_mock_assert_completed(void)
{
    if (nala_state_for_sendto.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked sendto() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_sendto.instances.length));
    }
}

// NALA_IMPLEMENTATION setsockopt

int __real_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

struct _nala_data_params_for_setsockopt {
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

struct _nala_data_type_for_setsockopt {
    struct _nala_data_params_for_setsockopt params;
    int return_value;
    int errno_value;
    int (*implementation)(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_setsockopt {
    struct _nala_data_type_for_setsockopt data;
    struct _nala_instance_type_for_setsockopt *next_p;
};

struct _nala_instances_type_for_setsockopt {
    struct _nala_instance_type_for_setsockopt *head_p;
    struct _nala_instance_type_for_setsockopt *tail_p;
    int length;
};

struct nala_state_type_for_setsockopt {
    struct nala_state_t state;
    struct _nala_data_type_for_setsockopt data;
    struct _nala_instances_type_for_setsockopt instances;
};

static struct nala_state_type_for_setsockopt nala_state_for_setsockopt = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_setsockopt *nala_get_data_setsockopt()
{
    if (nala_state_for_setsockopt.instances.tail_p != NULL) {
        return (&nala_state_for_setsockopt.instances.tail_p->data);
    } else {
        return (&nala_state_for_setsockopt.data);
    }
}

struct _nala_data_params_for_setsockopt *nala_get_params_setsockopt()
{
    return (&nala_get_data_setsockopt()->params);
}

int __wrap_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    struct _nala_instance_type_for_setsockopt *_nala_instance_p;
    struct _nala_data_type_for_setsockopt *_nala_data_p;
    int return_value;

    nala_print_call("setsockopt", &nala_state_for_setsockopt.state);

    switch (nala_state_for_setsockopt.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_setsockopt.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_setsockopt.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked setsockopt() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_setsockopt.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, setsockopt, optval);
        MOCK_ASSERT_IN_EQ(_nala_data_p, setsockopt, sockfd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, setsockopt, level);
        MOCK_ASSERT_IN_EQ(_nala_data_p, setsockopt, optname);
        MOCK_ASSERT_IN_EQ(_nala_data_p, setsockopt, optlen);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   setsockopt,
                                   optval);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(sockfd, level, optname, optval, optlen);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_setsockopt.data.implementation(sockfd, level, optname, optval, optlen);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_setsockopt.state.mode = 3;
    nala_state_for_setsockopt.data.params.optval = NULL;
    nala_state_for_setsockopt.data.params.ignore_optval_in = true;
    nala_state_for_setsockopt.data.params.sockfd = sockfd;
    nala_state_for_setsockopt.data.params.ignore_sockfd_in = false;
    nala_state_for_setsockopt.data.params.level = level;
    nala_state_for_setsockopt.data.params.ignore_level_in = false;
    nala_state_for_setsockopt.data.params.optname = optname;
    nala_state_for_setsockopt.data.params.ignore_optname_in = false;
    nala_state_for_setsockopt.data.params.optlen = optlen;
    nala_state_for_setsockopt.data.params.ignore_optlen_in = false;
    nala_state_for_setsockopt.data.return_value = return_value;
    nala_state_for_setsockopt.data.errno_value = 0;
    nala_state_for_setsockopt.data.callback = NULL;
}

void setsockopt_mock_once(int sockfd, int level, int optname, socklen_t optlen, int return_value)
{
    struct _nala_instance_type_for_setsockopt *_nala_instance_p;

    nala_state_for_setsockopt.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.optval_out);
    nala_set_param_init(&_nala_instance_p->data.params.optval_in);
    _nala_instance_p->data.params.optval_in_assert = NULL;
    _nala_instance_p->data.params.optval_out_copy = NULL;
    _nala_instance_p->data.params.optval = NULL;
    _nala_instance_p->data.params.ignore_optval_in = true;
    _nala_instance_p->data.params.sockfd = sockfd;
    _nala_instance_p->data.params.ignore_sockfd_in = false;
    _nala_instance_p->data.params.level = level;
    _nala_instance_p->data.params.ignore_level_in = false;
    _nala_instance_p->data.params.optname = optname;
    _nala_instance_p->data.params.ignore_optname_in = false;
    _nala_instance_p->data.params.optlen = optlen;
    _nala_instance_p->data.params.ignore_optlen_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_setsockopt.instances,
                          _nala_instance_p);
}

void setsockopt_mock_ignore_in(int return_value)
{
    nala_state_for_setsockopt.state.mode = 3;
    nala_state_for_setsockopt.data.params.ignore_optval_in = true;
    nala_state_for_setsockopt.data.params.ignore_sockfd_in = true;
    nala_state_for_setsockopt.data.params.ignore_level_in = true;
    nala_state_for_setsockopt.data.params.ignore_optname_in = true;
    nala_state_for_setsockopt.data.params.ignore_optlen_in = true;
    nala_state_for_setsockopt.data.return_value = return_value;
    nala_state_for_setsockopt.data.errno_value = 0;
    nala_state_for_setsockopt.data.callback = NULL;
}

void setsockopt_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_setsockopt *instance_p;

    nala_state_for_setsockopt.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_setsockopt.instances,
                          instance_p);
}

void setsockopt_mock_set_errno(int errno_value)
{
    nala_get_data_setsockopt()->errno_value = errno_value;
}

void setsockopt_mock_set_callback(void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen))
{
    nala_get_data_setsockopt()->callback = callback;
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
    nala_get_params_setsockopt()->optval_in_assert = callback;
}

void setsockopt_mock_set_optval_in_pointer(const void *optval)
{
    struct _nala_data_params_for_setsockopt *_nala_params_p;

    _nala_params_p = nala_get_params_setsockopt();
    _nala_params_p->ignore_optval_in = false;
    _nala_params_p->optval = optval;
}

void setsockopt_mock_set_optval_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_setsockopt()->optval_out,
                       buf_p,
                       size);
}

void setsockopt_mock_set_optval_out_copy(void (*callback)(const void *optval, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_setsockopt()->optval_out_copy = callback;
}

void setsockopt_mock_none(void)
{
    nala_state_for_setsockopt.state.mode = 4;
}

void setsockopt_mock_implementation(int (*implementation)(int sockfd, int level, int optname, const void *optval, socklen_t optlen))
{
    nala_state_for_setsockopt.state.mode = 2;
    nala_state_for_setsockopt.data.implementation = implementation;
}

void setsockopt_mock_disable(void)
{
    nala_state_for_setsockopt.state.mode = 0;
}

void setsockopt_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_setsockopt.state);
}

void setsockopt_mock_resume(void)
{
    nala_state_resume(&nala_state_for_setsockopt.state);
}

void setsockopt_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_setsockopt);
}

void setsockopt_mock_assert_completed(void)
{
    if (nala_state_for_setsockopt.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked setsockopt() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_setsockopt.instances.length));
    }
}

// NALA_IMPLEMENTATION sleep

unsigned int __real_sleep(unsigned int seconds);

struct _nala_data_params_for_sleep {
    unsigned int seconds;
    bool ignore_seconds_in;
};

struct _nala_data_type_for_sleep {
    struct _nala_data_params_for_sleep params;
    unsigned int return_value;
    int errno_value;
    unsigned int (*implementation)(unsigned int seconds);
    void (*callback)(unsigned int seconds);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_sleep {
    struct _nala_data_type_for_sleep data;
    struct _nala_instance_type_for_sleep *next_p;
};

struct _nala_instances_type_for_sleep {
    struct _nala_instance_type_for_sleep *head_p;
    struct _nala_instance_type_for_sleep *tail_p;
    int length;
};

struct nala_state_type_for_sleep {
    struct nala_state_t state;
    struct _nala_data_type_for_sleep data;
    struct _nala_instances_type_for_sleep instances;
};

static struct nala_state_type_for_sleep nala_state_for_sleep = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_sleep *nala_get_data_sleep()
{
    if (nala_state_for_sleep.instances.tail_p != NULL) {
        return (&nala_state_for_sleep.instances.tail_p->data);
    } else {
        return (&nala_state_for_sleep.data);
    }
}

struct _nala_data_params_for_sleep *nala_get_params_sleep()
{
    return (&nala_get_data_sleep()->params);
}

unsigned int __wrap_sleep(unsigned int seconds)
{
    struct _nala_instance_type_for_sleep *_nala_instance_p;
    struct _nala_data_type_for_sleep *_nala_data_p;
    unsigned int return_value;

    nala_print_call("sleep", &nala_state_for_sleep.state);

    switch (nala_state_for_sleep.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_sleep.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_sleep.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked sleep() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_sleep.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, sleep, seconds);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(seconds);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_sleep.data.implementation(seconds);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_sleep.state.mode = 3;
    nala_state_for_sleep.data.params.seconds = seconds;
    nala_state_for_sleep.data.params.ignore_seconds_in = false;
    nala_state_for_sleep.data.return_value = return_value;
    nala_state_for_sleep.data.errno_value = 0;
    nala_state_for_sleep.data.callback = NULL;
}

void sleep_mock_once(unsigned int seconds, unsigned int return_value)
{
    struct _nala_instance_type_for_sleep *_nala_instance_p;

    nala_state_for_sleep.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.seconds = seconds;
    _nala_instance_p->data.params.ignore_seconds_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_sleep.instances,
                          _nala_instance_p);
}

void sleep_mock_ignore_in(unsigned int return_value)
{
    nala_state_for_sleep.state.mode = 3;
    nala_state_for_sleep.data.params.ignore_seconds_in = true;
    nala_state_for_sleep.data.return_value = return_value;
    nala_state_for_sleep.data.errno_value = 0;
    nala_state_for_sleep.data.callback = NULL;
}

void sleep_mock_ignore_in_once(unsigned int return_value)
{
    struct _nala_instance_type_for_sleep *instance_p;

    nala_state_for_sleep.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_seconds_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_sleep.instances,
                          instance_p);
}

void sleep_mock_set_errno(int errno_value)
{
    nala_get_data_sleep()->errno_value = errno_value;
}

void sleep_mock_set_callback(void (*callback)(unsigned int seconds))
{
    nala_get_data_sleep()->callback = callback;
}

void sleep_mock_ignore_seconds_in(void)
{
    nala_get_params_sleep()->ignore_seconds_in = true;
}

void sleep_mock_none(void)
{
    nala_state_for_sleep.state.mode = 4;
}

void sleep_mock_implementation(unsigned int (*implementation)(unsigned int seconds))
{
    nala_state_for_sleep.state.mode = 2;
    nala_state_for_sleep.data.implementation = implementation;
}

void sleep_mock_disable(void)
{
    nala_state_for_sleep.state.mode = 0;
}

void sleep_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_sleep.state);
}

void sleep_mock_resume(void)
{
    nala_state_resume(&nala_state_for_sleep.state);
}

void sleep_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_sleep);
}

void sleep_mock_assert_completed(void)
{
    if (nala_state_for_sleep.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked sleep() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_sleep.instances.length));
    }
}

// NALA_IMPLEMENTATION statvfs

int __real_statvfs(const char *path, struct statvfs *buf);

struct _nala_data_params_for_statvfs {
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

struct _nala_data_type_for_statvfs {
    struct _nala_data_params_for_statvfs params;
    int return_value;
    int errno_value;
    int (*implementation)(const char *path, struct statvfs *buf);
    void (*callback)(const char *path, struct statvfs *buf);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_statvfs {
    struct _nala_data_type_for_statvfs data;
    struct _nala_instance_type_for_statvfs *next_p;
};

struct _nala_instances_type_for_statvfs {
    struct _nala_instance_type_for_statvfs *head_p;
    struct _nala_instance_type_for_statvfs *tail_p;
    int length;
};

struct nala_state_type_for_statvfs {
    struct nala_state_t state;
    struct _nala_data_type_for_statvfs data;
    struct _nala_instances_type_for_statvfs instances;
};

static struct nala_state_type_for_statvfs nala_state_for_statvfs = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_statvfs *nala_get_data_statvfs()
{
    if (nala_state_for_statvfs.instances.tail_p != NULL) {
        return (&nala_state_for_statvfs.instances.tail_p->data);
    } else {
        return (&nala_state_for_statvfs.data);
    }
}

struct _nala_data_params_for_statvfs *nala_get_params_statvfs()
{
    return (&nala_get_data_statvfs()->params);
}

int __wrap_statvfs(const char *path, struct statvfs *buf)
{
    struct _nala_instance_type_for_statvfs *_nala_instance_p;
    struct _nala_data_type_for_statvfs *_nala_data_p;
    int return_value;

    nala_print_call("statvfs", &nala_state_for_statvfs.state);

    switch (nala_state_for_statvfs.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_statvfs.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_statvfs.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked statvfs() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_statvfs.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, statvfs, path);
        MOCK_ASSERT_IN_EQ(_nala_data_p, statvfs, buf);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   statvfs,
                                   path);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_statvfs,
                                   statvfs,
                                   buf);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(path, buf);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_statvfs.data.implementation(path, buf);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_statvfs.state.mode = 3;
    nala_state_for_statvfs.data.params.path = NULL;
    nala_state_for_statvfs.data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&nala_state_for_statvfs.data.params.path_in,
                              path);
    } else {
        nala_state_for_statvfs.data.params.ignore_path_in = false;
    }

    nala_state_for_statvfs.data.params.buf = NULL;
    nala_state_for_statvfs.data.params.ignore_buf_in = true;
    nala_state_for_statvfs.data.return_value = return_value;
    nala_state_for_statvfs.data.errno_value = 0;
    nala_state_for_statvfs.data.callback = NULL;
}

void statvfs_mock_once(const char *path, int return_value)
{
    struct _nala_instance_type_for_statvfs *_nala_instance_p;

    nala_state_for_statvfs.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.path_out);
    nala_set_param_init(&_nala_instance_p->data.params.path_in);
    _nala_instance_p->data.params.path_in_assert = NULL;
    _nala_instance_p->data.params.path_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.buf_out);
    nala_set_param_init(&_nala_instance_p->data.params.buf_in);
    _nala_instance_p->data.params.buf_in_assert = NULL;
    _nala_instance_p->data.params.buf_out_copy = NULL;
    _nala_instance_p->data.params.path = NULL;
    _nala_instance_p->data.params.ignore_path_in = true;

    if (path != NULL) {
        nala_set_param_string(&_nala_instance_p->data.params.path_in,
                              path);
    } else {
        _nala_instance_p->data.params.ignore_path_in = false;
    }

    _nala_instance_p->data.params.buf = NULL;
    _nala_instance_p->data.params.ignore_buf_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_statvfs.instances,
                          _nala_instance_p);
}

void statvfs_mock_ignore_in(int return_value)
{
    nala_state_for_statvfs.state.mode = 3;
    nala_state_for_statvfs.data.params.ignore_path_in = true;
    nala_state_for_statvfs.data.params.ignore_buf_in = true;
    nala_state_for_statvfs.data.return_value = return_value;
    nala_state_for_statvfs.data.errno_value = 0;
    nala_state_for_statvfs.data.callback = NULL;
}

void statvfs_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_statvfs *instance_p;

    nala_state_for_statvfs.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_statvfs.instances,
                          instance_p);
}

void statvfs_mock_set_errno(int errno_value)
{
    nala_get_data_statvfs()->errno_value = errno_value;
}

void statvfs_mock_set_callback(void (*callback)(const char *path, struct statvfs *buf))
{
    nala_get_data_statvfs()->callback = callback;
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
    nala_get_params_statvfs()->path_in_assert = callback;
}

void statvfs_mock_set_path_in_pointer(const char *path)
{
    struct _nala_data_params_for_statvfs *_nala_params_p;

    _nala_params_p = nala_get_params_statvfs();
    _nala_params_p->ignore_path_in = false;
    _nala_params_p->path = path;
}

void statvfs_mock_set_path_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->path_out,
                       buf_p,
                       size);
}

void statvfs_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_statvfs()->path_out_copy = callback;
}

void statvfs_mock_set_buf_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->buf_in,
                       buf_p,
                       size);
}

void statvfs_mock_set_buf_in_assert(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_statvfs()->buf_in_assert = callback;
}

void statvfs_mock_set_buf_in_pointer(struct statvfs *buf)
{
    struct _nala_data_params_for_statvfs *_nala_params_p;

    _nala_params_p = nala_get_params_statvfs();
    _nala_params_p->ignore_buf_in = false;
    _nala_params_p->buf = buf;
}

void statvfs_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_statvfs()->buf_out,
                       buf_p,
                       size);
}

void statvfs_mock_set_buf_out_copy(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_statvfs()->buf_out_copy = callback;
}

void statvfs_mock_none(void)
{
    nala_state_for_statvfs.state.mode = 4;
}

void statvfs_mock_implementation(int (*implementation)(const char *path, struct statvfs *buf))
{
    nala_state_for_statvfs.state.mode = 2;
    nala_state_for_statvfs.data.implementation = implementation;
}

void statvfs_mock_disable(void)
{
    nala_state_for_statvfs.state.mode = 0;
}

void statvfs_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_statvfs.state);
}

void statvfs_mock_resume(void)
{
    nala_state_resume(&nala_state_for_statvfs.state);
}

void statvfs_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_statvfs);
}

void statvfs_mock_assert_completed(void)
{
    if (nala_state_for_statvfs.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked statvfs() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_statvfs.instances.length));
    }
}

// NALA_IMPLEMENTATION struct_param

void __real_struct_param(struct struct_param_type *data);

struct _nala_data_params_for_struct_param {
    struct struct_param_type *data;
    bool ignore_data_in;
    struct nala_set_param data_in;
    void (*data_in_assert)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param data_out;
    void (*data_out_copy)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_struct_param {
    struct _nala_data_params_for_struct_param params;
    int errno_value;
    void (*implementation)(struct struct_param_type *data);
    void (*callback)(struct struct_param_type *data);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_struct_param {
    struct _nala_data_type_for_struct_param data;
    struct _nala_instance_type_for_struct_param *next_p;
};

struct _nala_instances_type_for_struct_param {
    struct _nala_instance_type_for_struct_param *head_p;
    struct _nala_instance_type_for_struct_param *tail_p;
    int length;
};

struct nala_state_type_for_struct_param {
    struct nala_state_t state;
    struct _nala_data_type_for_struct_param data;
    struct _nala_instances_type_for_struct_param instances;
};

static struct nala_state_type_for_struct_param nala_state_for_struct_param = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_struct_param *nala_get_data_struct_param()
{
    if (nala_state_for_struct_param.instances.tail_p != NULL) {
        return (&nala_state_for_struct_param.instances.tail_p->data);
    } else {
        return (&nala_state_for_struct_param.data);
    }
}

struct _nala_data_params_for_struct_param *nala_get_params_struct_param()
{
    return (&nala_get_data_struct_param()->params);
}

void __wrap_struct_param(struct struct_param_type *data)
{
    struct _nala_instance_type_for_struct_param *_nala_instance_p;
    struct _nala_data_type_for_struct_param *_nala_data_p;

    nala_print_call("struct_param", &nala_state_for_struct_param.state);

    switch (nala_state_for_struct_param.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_struct_param.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_struct_param.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked struct_param() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_struct_param.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, struct_param, data);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_struct_param_type,
                                   struct_param,
                                   data);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(data);
        }


        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        nala_state_for_struct_param.data.implementation(data);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_struct_param.state.mode = 3;
    nala_state_for_struct_param.data.params.data = NULL;
    nala_state_for_struct_param.data.params.ignore_data_in = true;
    nala_state_for_struct_param.data.errno_value = 0;
    nala_state_for_struct_param.data.callback = NULL;
}

void struct_param_mock_once()
{
    struct _nala_instance_type_for_struct_param *_nala_instance_p;

    nala_state_for_struct_param.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.data_out);
    nala_set_param_init(&_nala_instance_p->data.params.data_in);
    _nala_instance_p->data.params.data_in_assert = NULL;
    _nala_instance_p->data.params.data_out_copy = NULL;
    _nala_instance_p->data.params.data = NULL;
    _nala_instance_p->data.params.ignore_data_in = true;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_struct_param.instances,
                          _nala_instance_p);
}

void struct_param_mock_ignore_in(void)
{
    nala_state_for_struct_param.state.mode = 3;
    nala_state_for_struct_param.data.params.ignore_data_in = true;
    nala_state_for_struct_param.data.errno_value = 0;
    nala_state_for_struct_param.data.callback = NULL;
}

void struct_param_mock_ignore_in_once(void)
{
    struct _nala_instance_type_for_struct_param *instance_p;

    nala_state_for_struct_param.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.data_out);
    nala_set_param_init(&instance_p->data.params.data_in);
    instance_p->data.params.data_in_assert = NULL;
    instance_p->data.params.data_out_copy = NULL;
    instance_p->data.params.data = NULL;
    instance_p->data.params.ignore_data_in = true;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_struct_param.instances,
                          instance_p);
}

void struct_param_mock_set_errno(int errno_value)
{
    nala_get_data_struct_param()->errno_value = errno_value;
}

void struct_param_mock_set_callback(void (*callback)(struct struct_param_type *data))
{
    nala_get_data_struct_param()->callback = callback;
}

void struct_param_mock_set_data_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_struct_param()->data_in,
                       buf_p,
                       size);
}

void struct_param_mock_set_data_in_assert(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_struct_param()->data_in_assert = callback;
}

void struct_param_mock_set_data_in_pointer(struct struct_param_type *data)
{
    struct _nala_data_params_for_struct_param *_nala_params_p;

    _nala_params_p = nala_get_params_struct_param();
    _nala_params_p->ignore_data_in = false;
    _nala_params_p->data = data;
}

void struct_param_mock_set_data_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_struct_param()->data_out,
                       buf_p,
                       size);
}

void struct_param_mock_set_data_out_copy(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_struct_param()->data_out_copy = callback;
}

void struct_param_mock_none(void)
{
    nala_state_for_struct_param.state.mode = 4;
}

void struct_param_mock_implementation(void (*implementation)(struct struct_param_type *data))
{
    nala_state_for_struct_param.state.mode = 2;
    nala_state_for_struct_param.data.implementation = implementation;
}

void struct_param_mock_disable(void)
{
    nala_state_for_struct_param.state.mode = 0;
}

void struct_param_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_struct_param.state);
}

void struct_param_mock_resume(void)
{
    nala_state_resume(&nala_state_for_struct_param.state);
}

void struct_param_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_struct_param);
}

void struct_param_mock_assert_completed(void)
{
    if (nala_state_for_struct_param.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked struct_param() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_struct_param.instances.length));
    }
}

// NALA_IMPLEMENTATION struct_param_and_return_type

struct struct_param_type __real_struct_param_and_return_type(struct struct_param_type arg);

struct _nala_data_params_for_struct_param_and_return_type {
    struct struct_param_type arg;
};

struct _nala_data_type_for_struct_param_and_return_type {
    struct _nala_data_params_for_struct_param_and_return_type params;
    struct struct_param_type return_value;
    int errno_value;
    struct struct_param_type (*implementation)(struct struct_param_type arg);
    void (*callback)(struct struct_param_type arg);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_struct_param_and_return_type {
    struct _nala_data_type_for_struct_param_and_return_type data;
    struct _nala_instance_type_for_struct_param_and_return_type *next_p;
};

struct _nala_instances_type_for_struct_param_and_return_type {
    struct _nala_instance_type_for_struct_param_and_return_type *head_p;
    struct _nala_instance_type_for_struct_param_and_return_type *tail_p;
    int length;
};

struct nala_state_type_for_struct_param_and_return_type {
    struct nala_state_t state;
    struct _nala_data_type_for_struct_param_and_return_type data;
    struct _nala_instances_type_for_struct_param_and_return_type instances;
};

static struct nala_state_type_for_struct_param_and_return_type nala_state_for_struct_param_and_return_type = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_struct_param_and_return_type *nala_get_data_struct_param_and_return_type()
{
    if (nala_state_for_struct_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_state_for_struct_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_state_for_struct_param_and_return_type.data);
    }
}

struct _nala_data_params_for_struct_param_and_return_type *nala_get_params_struct_param_and_return_type()
{
    return (&nala_get_data_struct_param_and_return_type()->params);
}

struct struct_param_type __wrap_struct_param_and_return_type(struct struct_param_type arg)
{
    struct _nala_instance_type_for_struct_param_and_return_type *_nala_instance_p;
    struct _nala_data_type_for_struct_param_and_return_type *_nala_data_p;
    struct struct_param_type return_value;

    nala_print_call("struct_param_and_return_type", &nala_state_for_struct_param_and_return_type.state);

    switch (nala_state_for_struct_param_and_return_type.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_struct_param_and_return_type.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_struct_param_and_return_type.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked struct_param_and_return_type() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_struct_param_and_return_type.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(arg);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_struct_param_and_return_type.data.implementation(arg);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_struct_param_and_return_type.state.mode = 3;
    nala_state_for_struct_param_and_return_type.data.return_value = return_value;
    nala_state_for_struct_param_and_return_type.data.errno_value = 0;
    nala_state_for_struct_param_and_return_type.data.callback = NULL;
}

void struct_param_and_return_type_mock_once(struct struct_param_type return_value)
{
    struct _nala_instance_type_for_struct_param_and_return_type *_nala_instance_p;

    nala_state_for_struct_param_and_return_type.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_struct_param_and_return_type.instances,
                          _nala_instance_p);
}

void struct_param_and_return_type_mock_ignore_in(struct struct_param_type return_value)
{
    nala_state_for_struct_param_and_return_type.state.mode = 3;
    nala_state_for_struct_param_and_return_type.data.return_value = return_value;
    nala_state_for_struct_param_and_return_type.data.errno_value = 0;
    nala_state_for_struct_param_and_return_type.data.callback = NULL;
}

void struct_param_and_return_type_mock_ignore_in_once(struct struct_param_type return_value)
{
    struct _nala_instance_type_for_struct_param_and_return_type *instance_p;

    nala_state_for_struct_param_and_return_type.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_struct_param_and_return_type.instances,
                          instance_p);
}

void struct_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_struct_param_and_return_type()->errno_value = errno_value;
}

void struct_param_and_return_type_mock_set_callback(void (*callback)(struct struct_param_type arg))
{
    nala_get_data_struct_param_and_return_type()->callback = callback;
}

void struct_param_and_return_type_mock_none(void)
{
    nala_state_for_struct_param_and_return_type.state.mode = 4;
}

void struct_param_and_return_type_mock_implementation(struct struct_param_type (*implementation)(struct struct_param_type arg))
{
    nala_state_for_struct_param_and_return_type.state.mode = 2;
    nala_state_for_struct_param_and_return_type.data.implementation = implementation;
}

void struct_param_and_return_type_mock_disable(void)
{
    nala_state_for_struct_param_and_return_type.state.mode = 0;
}

void struct_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_struct_param_and_return_type.state);
}

void struct_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_state_for_struct_param_and_return_type.state);
}

void struct_param_and_return_type_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_struct_param_and_return_type);
}

void struct_param_and_return_type_mock_assert_completed(void)
{
    if (nala_state_for_struct_param_and_return_type.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked struct_param_and_return_type() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_struct_param_and_return_type.instances.length));
    }
}

// NALA_IMPLEMENTATION time

time_t __real_time(time_t *tloc);

struct _nala_data_params_for_time {
    time_t *tloc;
    bool ignore_tloc_in;
    struct nala_set_param tloc_in;
    void (*tloc_in_assert)(time_t *tloc, const void *nala_buf_p, size_t nala_size);
    struct nala_set_param tloc_out;
    void (*tloc_out_copy)(time_t *tloc, const void *nala_buf_p, size_t nala_size);
};

struct _nala_data_type_for_time {
    struct _nala_data_params_for_time params;
    time_t return_value;
    int errno_value;
    time_t (*implementation)(time_t *tloc);
    void (*callback)(time_t *tloc);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_time {
    struct _nala_data_type_for_time data;
    struct _nala_instance_type_for_time *next_p;
};

struct _nala_instances_type_for_time {
    struct _nala_instance_type_for_time *head_p;
    struct _nala_instance_type_for_time *tail_p;
    int length;
};

struct nala_state_type_for_time {
    struct nala_state_t state;
    struct _nala_data_type_for_time data;
    struct _nala_instances_type_for_time instances;
};

static struct nala_state_type_for_time nala_state_for_time = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_time *nala_get_data_time()
{
    if (nala_state_for_time.instances.tail_p != NULL) {
        return (&nala_state_for_time.instances.tail_p->data);
    } else {
        return (&nala_state_for_time.data);
    }
}

struct _nala_data_params_for_time *nala_get_params_time()
{
    return (&nala_get_data_time()->params);
}

time_t __wrap_time(time_t *tloc)
{
    struct _nala_instance_type_for_time *_nala_instance_p;
    struct _nala_data_type_for_time *_nala_data_p;
    time_t return_value;

    nala_print_call("time", &nala_state_for_time.state);

    switch (nala_state_for_time.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_time.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_time.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked time() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_time.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, time, tloc);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   time,
                                   tloc);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(tloc);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_time.data.implementation(tloc);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_time.state.mode = 3;
    nala_state_for_time.data.params.tloc = NULL;
    nala_state_for_time.data.params.ignore_tloc_in = true;
    nala_state_for_time.data.return_value = return_value;
    nala_state_for_time.data.errno_value = 0;
    nala_state_for_time.data.callback = NULL;
}

void time_mock_once(time_t return_value)
{
    struct _nala_instance_type_for_time *_nala_instance_p;

    nala_state_for_time.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.tloc_out);
    nala_set_param_init(&_nala_instance_p->data.params.tloc_in);
    _nala_instance_p->data.params.tloc_in_assert = NULL;
    _nala_instance_p->data.params.tloc_out_copy = NULL;
    _nala_instance_p->data.params.tloc = NULL;
    _nala_instance_p->data.params.ignore_tloc_in = true;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_time.instances,
                          _nala_instance_p);
}

void time_mock_ignore_in(time_t return_value)
{
    nala_state_for_time.state.mode = 3;
    nala_state_for_time.data.params.ignore_tloc_in = true;
    nala_state_for_time.data.return_value = return_value;
    nala_state_for_time.data.errno_value = 0;
    nala_state_for_time.data.callback = NULL;
}

void time_mock_ignore_in_once(time_t return_value)
{
    struct _nala_instance_type_for_time *instance_p;

    nala_state_for_time.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    nala_set_param_init(&instance_p->data.params.tloc_out);
    nala_set_param_init(&instance_p->data.params.tloc_in);
    instance_p->data.params.tloc_in_assert = NULL;
    instance_p->data.params.tloc_out_copy = NULL;
    instance_p->data.params.tloc = NULL;
    instance_p->data.params.ignore_tloc_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_time.instances,
                          instance_p);
}

void time_mock_set_errno(int errno_value)
{
    nala_get_data_time()->errno_value = errno_value;
}

void time_mock_set_callback(void (*callback)(time_t *tloc))
{
    nala_get_data_time()->callback = callback;
}

void time_mock_set_tloc_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_time()->tloc_in,
                       buf_p,
                       size);
}

void time_mock_set_tloc_in_assert(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_time()->tloc_in_assert = callback;
}

void time_mock_set_tloc_in_pointer(time_t *tloc)
{
    struct _nala_data_params_for_time *_nala_params_p;

    _nala_params_p = nala_get_params_time();
    _nala_params_p->ignore_tloc_in = false;
    _nala_params_p->tloc = tloc;
}

void time_mock_set_tloc_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_time()->tloc_out,
                       buf_p,
                       size);
}

void time_mock_set_tloc_out_copy(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_time()->tloc_out_copy = callback;
}

void time_mock_none(void)
{
    nala_state_for_time.state.mode = 4;
}

void time_mock_implementation(time_t (*implementation)(time_t *tloc))
{
    nala_state_for_time.state.mode = 2;
    nala_state_for_time.data.implementation = implementation;
}

void time_mock_disable(void)
{
    nala_state_for_time.state.mode = 0;
}

void time_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_time.state);
}

void time_mock_resume(void)
{
    nala_state_resume(&nala_state_for_time.state);
}

void time_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_time);
}

void time_mock_assert_completed(void)
{
    if (nala_state_for_time.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked time() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_time.instances.length));
    }
}

// NALA_IMPLEMENTATION timerfd_settime

int __real_timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

struct _nala_data_params_for_timerfd_settime {
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

struct _nala_data_type_for_timerfd_settime {
    struct _nala_data_params_for_timerfd_settime params;
    int return_value;
    int errno_value;
    int (*implementation)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
    void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_timerfd_settime {
    struct _nala_data_type_for_timerfd_settime data;
    struct _nala_instance_type_for_timerfd_settime *next_p;
};

struct _nala_instances_type_for_timerfd_settime {
    struct _nala_instance_type_for_timerfd_settime *head_p;
    struct _nala_instance_type_for_timerfd_settime *tail_p;
    int length;
};

struct nala_state_type_for_timerfd_settime {
    struct nala_state_t state;
    struct _nala_data_type_for_timerfd_settime data;
    struct _nala_instances_type_for_timerfd_settime instances;
};

static struct nala_state_type_for_timerfd_settime nala_state_for_timerfd_settime = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_timerfd_settime *nala_get_data_timerfd_settime()
{
    if (nala_state_for_timerfd_settime.instances.tail_p != NULL) {
        return (&nala_state_for_timerfd_settime.instances.tail_p->data);
    } else {
        return (&nala_state_for_timerfd_settime.data);
    }
}

struct _nala_data_params_for_timerfd_settime *nala_get_params_timerfd_settime()
{
    return (&nala_get_data_timerfd_settime()->params);
}

int __wrap_timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value)
{
    struct _nala_instance_type_for_timerfd_settime *_nala_instance_p;
    struct _nala_data_type_for_timerfd_settime *_nala_data_p;
    int return_value;

    nala_print_call("timerfd_settime", &nala_state_for_timerfd_settime.state);

    switch (nala_state_for_timerfd_settime.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_timerfd_settime.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_timerfd_settime.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked timerfd_settime() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_timerfd_settime.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, timerfd_settime, new_value);
        MOCK_ASSERT_IN_EQ(_nala_data_p, timerfd_settime, old_value);
        MOCK_ASSERT_IN_EQ(_nala_data_p, timerfd_settime, fd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, timerfd_settime, flags);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_itimerspec,
                                   timerfd_settime,
                                   new_value);
        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_in_struct_itimerspec,
                                   timerfd_settime,
                                   old_value);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(fd, flags, new_value, old_value);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_timerfd_settime.data.implementation(fd, flags, new_value, old_value);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_timerfd_settime.state.mode = 3;
    nala_state_for_timerfd_settime.data.params.new_value = NULL;
    nala_state_for_timerfd_settime.data.params.ignore_new_value_in = true;
    nala_state_for_timerfd_settime.data.params.old_value = NULL;
    nala_state_for_timerfd_settime.data.params.ignore_old_value_in = true;
    nala_state_for_timerfd_settime.data.params.fd = fd;
    nala_state_for_timerfd_settime.data.params.ignore_fd_in = false;
    nala_state_for_timerfd_settime.data.params.flags = flags;
    nala_state_for_timerfd_settime.data.params.ignore_flags_in = false;
    nala_state_for_timerfd_settime.data.return_value = return_value;
    nala_state_for_timerfd_settime.data.errno_value = 0;
    nala_state_for_timerfd_settime.data.callback = NULL;
}

void timerfd_settime_mock_once(int fd, int flags, int return_value)
{
    struct _nala_instance_type_for_timerfd_settime *_nala_instance_p;

    nala_state_for_timerfd_settime.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.new_value_out);
    nala_set_param_init(&_nala_instance_p->data.params.new_value_in);
    _nala_instance_p->data.params.new_value_in_assert = NULL;
    _nala_instance_p->data.params.new_value_out_copy = NULL;
    nala_set_param_init(&_nala_instance_p->data.params.old_value_out);
    nala_set_param_init(&_nala_instance_p->data.params.old_value_in);
    _nala_instance_p->data.params.old_value_in_assert = NULL;
    _nala_instance_p->data.params.old_value_out_copy = NULL;
    _nala_instance_p->data.params.new_value = NULL;
    _nala_instance_p->data.params.ignore_new_value_in = true;
    _nala_instance_p->data.params.old_value = NULL;
    _nala_instance_p->data.params.ignore_old_value_in = true;
    _nala_instance_p->data.params.fd = fd;
    _nala_instance_p->data.params.ignore_fd_in = false;
    _nala_instance_p->data.params.flags = flags;
    _nala_instance_p->data.params.ignore_flags_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_timerfd_settime.instances,
                          _nala_instance_p);
}

void timerfd_settime_mock_ignore_in(int return_value)
{
    nala_state_for_timerfd_settime.state.mode = 3;
    nala_state_for_timerfd_settime.data.params.ignore_new_value_in = true;
    nala_state_for_timerfd_settime.data.params.ignore_old_value_in = true;
    nala_state_for_timerfd_settime.data.params.ignore_fd_in = true;
    nala_state_for_timerfd_settime.data.params.ignore_flags_in = true;
    nala_state_for_timerfd_settime.data.return_value = return_value;
    nala_state_for_timerfd_settime.data.errno_value = 0;
    nala_state_for_timerfd_settime.data.callback = NULL;
}

void timerfd_settime_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_timerfd_settime *instance_p;

    nala_state_for_timerfd_settime.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_timerfd_settime.instances,
                          instance_p);
}

void timerfd_settime_mock_set_errno(int errno_value)
{
    nala_get_data_timerfd_settime()->errno_value = errno_value;
}

void timerfd_settime_mock_set_callback(void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value))
{
    nala_get_data_timerfd_settime()->callback = callback;
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
    nala_get_params_timerfd_settime()->new_value_in_assert = callback;
}

void timerfd_settime_mock_set_new_value_in_pointer(const struct itimerspec *new_value)
{
    struct _nala_data_params_for_timerfd_settime *_nala_params_p;

    _nala_params_p = nala_get_params_timerfd_settime();
    _nala_params_p->ignore_new_value_in = false;
    _nala_params_p->new_value = new_value;
}

void timerfd_settime_mock_set_new_value_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->new_value_out,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_new_value_out_copy(void (*callback)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_timerfd_settime()->new_value_out_copy = callback;
}

void timerfd_settime_mock_set_old_value_in(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->old_value_in,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_old_value_in_assert(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_timerfd_settime()->old_value_in_assert = callback;
}

void timerfd_settime_mock_set_old_value_in_pointer(struct itimerspec *old_value)
{
    struct _nala_data_params_for_timerfd_settime *_nala_params_p;

    _nala_params_p = nala_get_params_timerfd_settime();
    _nala_params_p->ignore_old_value_in = false;
    _nala_params_p->old_value = old_value;
}

void timerfd_settime_mock_set_old_value_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_timerfd_settime()->old_value_out,
                       buf_p,
                       size);
}

void timerfd_settime_mock_set_old_value_out_copy(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_timerfd_settime()->old_value_out_copy = callback;
}

void timerfd_settime_mock_none(void)
{
    nala_state_for_timerfd_settime.state.mode = 4;
}

void timerfd_settime_mock_implementation(int (*implementation)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value))
{
    nala_state_for_timerfd_settime.state.mode = 2;
    nala_state_for_timerfd_settime.data.implementation = implementation;
}

void timerfd_settime_mock_disable(void)
{
    nala_state_for_timerfd_settime.state.mode = 0;
}

void timerfd_settime_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_timerfd_settime.state);
}

void timerfd_settime_mock_resume(void)
{
    nala_state_resume(&nala_state_for_timerfd_settime.state);
}

void timerfd_settime_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_timerfd_settime);
}

void timerfd_settime_mock_assert_completed(void)
{
    if (nala_state_for_timerfd_settime.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked timerfd_settime() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_timerfd_settime.instances.length));
    }
}

// NALA_IMPLEMENTATION tmpfile

FILE *__real_tmpfile(void);

struct _nala_data_params_for_tmpfile {
    int dummy;
};

struct _nala_data_type_for_tmpfile {
    struct _nala_data_params_for_tmpfile params;
    FILE *return_value;
    int errno_value;
    FILE *(*implementation)(void);
    void (*callback)(void);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_tmpfile {
    struct _nala_data_type_for_tmpfile data;
    struct _nala_instance_type_for_tmpfile *next_p;
};

struct _nala_instances_type_for_tmpfile {
    struct _nala_instance_type_for_tmpfile *head_p;
    struct _nala_instance_type_for_tmpfile *tail_p;
    int length;
};

struct nala_state_type_for_tmpfile {
    struct nala_state_t state;
    struct _nala_data_type_for_tmpfile data;
    struct _nala_instances_type_for_tmpfile instances;
};

static struct nala_state_type_for_tmpfile nala_state_for_tmpfile = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_tmpfile *nala_get_data_tmpfile()
{
    if (nala_state_for_tmpfile.instances.tail_p != NULL) {
        return (&nala_state_for_tmpfile.instances.tail_p->data);
    } else {
        return (&nala_state_for_tmpfile.data);
    }
}

struct _nala_data_params_for_tmpfile *nala_get_params_tmpfile()
{
    return (&nala_get_data_tmpfile()->params);
}

FILE *__wrap_tmpfile(void)
{
    struct _nala_instance_type_for_tmpfile *_nala_instance_p;
    struct _nala_data_type_for_tmpfile *_nala_data_p;
    FILE *return_value;

    nala_print_call("tmpfile", &nala_state_for_tmpfile.state);

    switch (nala_state_for_tmpfile.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_tmpfile.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_tmpfile.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked tmpfile() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_tmpfile.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback();
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_tmpfile.data.implementation();
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_tmpfile.state.mode = 3;
    nala_state_for_tmpfile.data.return_value = return_value;
    nala_state_for_tmpfile.data.errno_value = 0;
    nala_state_for_tmpfile.data.callback = NULL;
}

void tmpfile_mock_once(FILE *return_value)
{
    struct _nala_instance_type_for_tmpfile *_nala_instance_p;

    nala_state_for_tmpfile.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_tmpfile.instances,
                          _nala_instance_p);
}

void tmpfile_mock_ignore_in(FILE *return_value)
{
    nala_state_for_tmpfile.state.mode = 3;
    nala_state_for_tmpfile.data.return_value = return_value;
    nala_state_for_tmpfile.data.errno_value = 0;
    nala_state_for_tmpfile.data.callback = NULL;
}

void tmpfile_mock_ignore_in_once(FILE *return_value)
{
    struct _nala_instance_type_for_tmpfile *instance_p;

    nala_state_for_tmpfile.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_tmpfile.instances,
                          instance_p);
}

void tmpfile_mock_set_errno(int errno_value)
{
    nala_get_data_tmpfile()->errno_value = errno_value;
}

void tmpfile_mock_set_callback(void (*callback)(void))
{
    nala_get_data_tmpfile()->callback = callback;
}

void tmpfile_mock_none(void)
{
    nala_state_for_tmpfile.state.mode = 4;
}

void tmpfile_mock_implementation(FILE *(*implementation)(void))
{
    nala_state_for_tmpfile.state.mode = 2;
    nala_state_for_tmpfile.data.implementation = implementation;
}

void tmpfile_mock_disable(void)
{
    nala_state_for_tmpfile.state.mode = 0;
}

void tmpfile_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_tmpfile.state);
}

void tmpfile_mock_resume(void)
{
    nala_state_resume(&nala_state_for_tmpfile.state);
}

void tmpfile_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_tmpfile);
}

void tmpfile_mock_assert_completed(void)
{
    if (nala_state_for_tmpfile.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked tmpfile() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_tmpfile.instances.length));
    }
}

// NALA_IMPLEMENTATION typedef_struct_param_and_return_type

struct_param_type __real_typedef_struct_param_and_return_type(struct_param_type arg);

struct _nala_data_params_for_typedef_struct_param_and_return_type {
    struct_param_type arg;
};

struct _nala_data_type_for_typedef_struct_param_and_return_type {
    struct _nala_data_params_for_typedef_struct_param_and_return_type params;
    struct_param_type return_value;
    int errno_value;
    struct_param_type (*implementation)(struct_param_type arg);
    void (*callback)(struct_param_type arg);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_typedef_struct_param_and_return_type {
    struct _nala_data_type_for_typedef_struct_param_and_return_type data;
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *next_p;
};

struct _nala_instances_type_for_typedef_struct_param_and_return_type {
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *head_p;
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *tail_p;
    int length;
};

struct nala_state_type_for_typedef_struct_param_and_return_type {
    struct nala_state_t state;
    struct _nala_data_type_for_typedef_struct_param_and_return_type data;
    struct _nala_instances_type_for_typedef_struct_param_and_return_type instances;
};

static struct nala_state_type_for_typedef_struct_param_and_return_type nala_state_for_typedef_struct_param_and_return_type = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_typedef_struct_param_and_return_type *nala_get_data_typedef_struct_param_and_return_type()
{
    if (nala_state_for_typedef_struct_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_state_for_typedef_struct_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_state_for_typedef_struct_param_and_return_type.data);
    }
}

struct _nala_data_params_for_typedef_struct_param_and_return_type *nala_get_params_typedef_struct_param_and_return_type()
{
    return (&nala_get_data_typedef_struct_param_and_return_type()->params);
}

struct_param_type __wrap_typedef_struct_param_and_return_type(struct_param_type arg)
{
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *_nala_instance_p;
    struct _nala_data_type_for_typedef_struct_param_and_return_type *_nala_data_p;
    struct_param_type return_value;

    nala_print_call("typedef_struct_param_and_return_type", &nala_state_for_typedef_struct_param_and_return_type.state);

    switch (nala_state_for_typedef_struct_param_and_return_type.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_typedef_struct_param_and_return_type.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_typedef_struct_param_and_return_type.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked typedef_struct_param_and_return_type() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_typedef_struct_param_and_return_type.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(arg);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_typedef_struct_param_and_return_type.data.implementation(arg);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_typedef_struct_param_and_return_type.state.mode = 3;
    nala_state_for_typedef_struct_param_and_return_type.data.return_value = return_value;
    nala_state_for_typedef_struct_param_and_return_type.data.errno_value = 0;
    nala_state_for_typedef_struct_param_and_return_type.data.callback = NULL;
}

void typedef_struct_param_and_return_type_mock_once(struct_param_type return_value)
{
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *_nala_instance_p;

    nala_state_for_typedef_struct_param_and_return_type.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_typedef_struct_param_and_return_type.instances,
                          _nala_instance_p);
}

void typedef_struct_param_and_return_type_mock_ignore_in(struct_param_type return_value)
{
    nala_state_for_typedef_struct_param_and_return_type.state.mode = 3;
    nala_state_for_typedef_struct_param_and_return_type.data.return_value = return_value;
    nala_state_for_typedef_struct_param_and_return_type.data.errno_value = 0;
    nala_state_for_typedef_struct_param_and_return_type.data.callback = NULL;
}

void typedef_struct_param_and_return_type_mock_ignore_in_once(struct_param_type return_value)
{
    struct _nala_instance_type_for_typedef_struct_param_and_return_type *instance_p;

    nala_state_for_typedef_struct_param_and_return_type.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_typedef_struct_param_and_return_type.instances,
                          instance_p);
}

void typedef_struct_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_typedef_struct_param_and_return_type()->errno_value = errno_value;
}

void typedef_struct_param_and_return_type_mock_set_callback(void (*callback)(struct_param_type arg))
{
    nala_get_data_typedef_struct_param_and_return_type()->callback = callback;
}

void typedef_struct_param_and_return_type_mock_none(void)
{
    nala_state_for_typedef_struct_param_and_return_type.state.mode = 4;
}

void typedef_struct_param_and_return_type_mock_implementation(struct_param_type (*implementation)(struct_param_type arg))
{
    nala_state_for_typedef_struct_param_and_return_type.state.mode = 2;
    nala_state_for_typedef_struct_param_and_return_type.data.implementation = implementation;
}

void typedef_struct_param_and_return_type_mock_disable(void)
{
    nala_state_for_typedef_struct_param_and_return_type.state.mode = 0;
}

void typedef_struct_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_typedef_struct_param_and_return_type.state);
}

void typedef_struct_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_state_for_typedef_struct_param_and_return_type.state);
}

void typedef_struct_param_and_return_type_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_typedef_struct_param_and_return_type);
}

void typedef_struct_param_and_return_type_mock_assert_completed(void)
{
    if (nala_state_for_typedef_struct_param_and_return_type.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked typedef_struct_param_and_return_type() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_typedef_struct_param_and_return_type.instances.length));
    }
}

// NALA_IMPLEMENTATION typedef_union_param_and_return_type

union_type __real_typedef_union_param_and_return_type(union_type arg);

struct _nala_data_params_for_typedef_union_param_and_return_type {
    union_type arg;
};

struct _nala_data_type_for_typedef_union_param_and_return_type {
    struct _nala_data_params_for_typedef_union_param_and_return_type params;
    union_type return_value;
    int errno_value;
    union_type (*implementation)(union_type arg);
    void (*callback)(union_type arg);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_typedef_union_param_and_return_type {
    struct _nala_data_type_for_typedef_union_param_and_return_type data;
    struct _nala_instance_type_for_typedef_union_param_and_return_type *next_p;
};

struct _nala_instances_type_for_typedef_union_param_and_return_type {
    struct _nala_instance_type_for_typedef_union_param_and_return_type *head_p;
    struct _nala_instance_type_for_typedef_union_param_and_return_type *tail_p;
    int length;
};

struct nala_state_type_for_typedef_union_param_and_return_type {
    struct nala_state_t state;
    struct _nala_data_type_for_typedef_union_param_and_return_type data;
    struct _nala_instances_type_for_typedef_union_param_and_return_type instances;
};

static struct nala_state_type_for_typedef_union_param_and_return_type nala_state_for_typedef_union_param_and_return_type = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_typedef_union_param_and_return_type *nala_get_data_typedef_union_param_and_return_type()
{
    if (nala_state_for_typedef_union_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_state_for_typedef_union_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_state_for_typedef_union_param_and_return_type.data);
    }
}

struct _nala_data_params_for_typedef_union_param_and_return_type *nala_get_params_typedef_union_param_and_return_type()
{
    return (&nala_get_data_typedef_union_param_and_return_type()->params);
}

union_type __wrap_typedef_union_param_and_return_type(union_type arg)
{
    struct _nala_instance_type_for_typedef_union_param_and_return_type *_nala_instance_p;
    struct _nala_data_type_for_typedef_union_param_and_return_type *_nala_data_p;
    union_type return_value;

    nala_print_call("typedef_union_param_and_return_type", &nala_state_for_typedef_union_param_and_return_type.state);

    switch (nala_state_for_typedef_union_param_and_return_type.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_typedef_union_param_and_return_type.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_typedef_union_param_and_return_type.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked typedef_union_param_and_return_type() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_typedef_union_param_and_return_type.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(arg);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_typedef_union_param_and_return_type.data.implementation(arg);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_typedef_union_param_and_return_type.state.mode = 3;
    nala_state_for_typedef_union_param_and_return_type.data.return_value = return_value;
    nala_state_for_typedef_union_param_and_return_type.data.errno_value = 0;
    nala_state_for_typedef_union_param_and_return_type.data.callback = NULL;
}

void typedef_union_param_and_return_type_mock_once(union_type return_value)
{
    struct _nala_instance_type_for_typedef_union_param_and_return_type *_nala_instance_p;

    nala_state_for_typedef_union_param_and_return_type.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_typedef_union_param_and_return_type.instances,
                          _nala_instance_p);
}

void typedef_union_param_and_return_type_mock_ignore_in(union_type return_value)
{
    nala_state_for_typedef_union_param_and_return_type.state.mode = 3;
    nala_state_for_typedef_union_param_and_return_type.data.return_value = return_value;
    nala_state_for_typedef_union_param_and_return_type.data.errno_value = 0;
    nala_state_for_typedef_union_param_and_return_type.data.callback = NULL;
}

void typedef_union_param_and_return_type_mock_ignore_in_once(union_type return_value)
{
    struct _nala_instance_type_for_typedef_union_param_and_return_type *instance_p;

    nala_state_for_typedef_union_param_and_return_type.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_typedef_union_param_and_return_type.instances,
                          instance_p);
}

void typedef_union_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_typedef_union_param_and_return_type()->errno_value = errno_value;
}

void typedef_union_param_and_return_type_mock_set_callback(void (*callback)(union_type arg))
{
    nala_get_data_typedef_union_param_and_return_type()->callback = callback;
}

void typedef_union_param_and_return_type_mock_none(void)
{
    nala_state_for_typedef_union_param_and_return_type.state.mode = 4;
}

void typedef_union_param_and_return_type_mock_implementation(union_type (*implementation)(union_type arg))
{
    nala_state_for_typedef_union_param_and_return_type.state.mode = 2;
    nala_state_for_typedef_union_param_and_return_type.data.implementation = implementation;
}

void typedef_union_param_and_return_type_mock_disable(void)
{
    nala_state_for_typedef_union_param_and_return_type.state.mode = 0;
}

void typedef_union_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_typedef_union_param_and_return_type.state);
}

void typedef_union_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_state_for_typedef_union_param_and_return_type.state);
}

void typedef_union_param_and_return_type_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_typedef_union_param_and_return_type);
}

void typedef_union_param_and_return_type_mock_assert_completed(void)
{
    if (nala_state_for_typedef_union_param_and_return_type.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked typedef_union_param_and_return_type() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_typedef_union_param_and_return_type.instances.length));
    }
}

// NALA_IMPLEMENTATION union_param_and_return_type

union union_type __real_union_param_and_return_type(union union_type arg);

struct _nala_data_params_for_union_param_and_return_type {
    union union_type arg;
};

struct _nala_data_type_for_union_param_and_return_type {
    struct _nala_data_params_for_union_param_and_return_type params;
    union union_type return_value;
    int errno_value;
    union union_type (*implementation)(union union_type arg);
    void (*callback)(union union_type arg);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_union_param_and_return_type {
    struct _nala_data_type_for_union_param_and_return_type data;
    struct _nala_instance_type_for_union_param_and_return_type *next_p;
};

struct _nala_instances_type_for_union_param_and_return_type {
    struct _nala_instance_type_for_union_param_and_return_type *head_p;
    struct _nala_instance_type_for_union_param_and_return_type *tail_p;
    int length;
};

struct nala_state_type_for_union_param_and_return_type {
    struct nala_state_t state;
    struct _nala_data_type_for_union_param_and_return_type data;
    struct _nala_instances_type_for_union_param_and_return_type instances;
};

static struct nala_state_type_for_union_param_and_return_type nala_state_for_union_param_and_return_type = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_union_param_and_return_type *nala_get_data_union_param_and_return_type()
{
    if (nala_state_for_union_param_and_return_type.instances.tail_p != NULL) {
        return (&nala_state_for_union_param_and_return_type.instances.tail_p->data);
    } else {
        return (&nala_state_for_union_param_and_return_type.data);
    }
}

struct _nala_data_params_for_union_param_and_return_type *nala_get_params_union_param_and_return_type()
{
    return (&nala_get_data_union_param_and_return_type()->params);
}

union union_type __wrap_union_param_and_return_type(union union_type arg)
{
    struct _nala_instance_type_for_union_param_and_return_type *_nala_instance_p;
    struct _nala_data_type_for_union_param_and_return_type *_nala_data_p;
    union union_type return_value;

    nala_print_call("union_param_and_return_type", &nala_state_for_union_param_and_return_type.state);

    switch (nala_state_for_union_param_and_return_type.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_union_param_and_return_type.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_union_param_and_return_type.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked union_param_and_return_type() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_union_param_and_return_type.data;
        }



        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(arg);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_union_param_and_return_type.data.implementation(arg);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_union_param_and_return_type.state.mode = 3;
    nala_state_for_union_param_and_return_type.data.return_value = return_value;
    nala_state_for_union_param_and_return_type.data.errno_value = 0;
    nala_state_for_union_param_and_return_type.data.callback = NULL;
}

void union_param_and_return_type_mock_once(union union_type return_value)
{
    struct _nala_instance_type_for_union_param_and_return_type *_nala_instance_p;

    nala_state_for_union_param_and_return_type.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_union_param_and_return_type.instances,
                          _nala_instance_p);
}

void union_param_and_return_type_mock_ignore_in(union union_type return_value)
{
    nala_state_for_union_param_and_return_type.state.mode = 3;
    nala_state_for_union_param_and_return_type.data.return_value = return_value;
    nala_state_for_union_param_and_return_type.data.errno_value = 0;
    nala_state_for_union_param_and_return_type.data.callback = NULL;
}

void union_param_and_return_type_mock_ignore_in_once(union union_type return_value)
{
    struct _nala_instance_type_for_union_param_and_return_type *instance_p;

    nala_state_for_union_param_and_return_type.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_union_param_and_return_type.instances,
                          instance_p);
}

void union_param_and_return_type_mock_set_errno(int errno_value)
{
    nala_get_data_union_param_and_return_type()->errno_value = errno_value;
}

void union_param_and_return_type_mock_set_callback(void (*callback)(union union_type arg))
{
    nala_get_data_union_param_and_return_type()->callback = callback;
}

void union_param_and_return_type_mock_none(void)
{
    nala_state_for_union_param_and_return_type.state.mode = 4;
}

void union_param_and_return_type_mock_implementation(union union_type (*implementation)(union union_type arg))
{
    nala_state_for_union_param_and_return_type.state.mode = 2;
    nala_state_for_union_param_and_return_type.data.implementation = implementation;
}

void union_param_and_return_type_mock_disable(void)
{
    nala_state_for_union_param_and_return_type.state.mode = 0;
}

void union_param_and_return_type_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_union_param_and_return_type.state);
}

void union_param_and_return_type_mock_resume(void)
{
    nala_state_resume(&nala_state_for_union_param_and_return_type.state);
}

void union_param_and_return_type_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_union_param_and_return_type);
}

void union_param_and_return_type_mock_assert_completed(void)
{
    if (nala_state_for_union_param_and_return_type.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked union_param_and_return_type() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_union_param_and_return_type.instances.length));
    }
}

// NALA_IMPLEMENTATION usleep

int __real_usleep(__useconds_t usec);

struct _nala_data_params_for_usleep {
    __useconds_t usec;
    bool ignore_usec_in;
};

struct _nala_data_type_for_usleep {
    struct _nala_data_params_for_usleep params;
    int return_value;
    int errno_value;
    int (*implementation)(__useconds_t usec);
    void (*callback)(__useconds_t usec);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_usleep {
    struct _nala_data_type_for_usleep data;
    struct _nala_instance_type_for_usleep *next_p;
};

struct _nala_instances_type_for_usleep {
    struct _nala_instance_type_for_usleep *head_p;
    struct _nala_instance_type_for_usleep *tail_p;
    int length;
};

struct nala_state_type_for_usleep {
    struct nala_state_t state;
    struct _nala_data_type_for_usleep data;
    struct _nala_instances_type_for_usleep instances;
};

static struct nala_state_type_for_usleep nala_state_for_usleep = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_usleep *nala_get_data_usleep()
{
    if (nala_state_for_usleep.instances.tail_p != NULL) {
        return (&nala_state_for_usleep.instances.tail_p->data);
    } else {
        return (&nala_state_for_usleep.data);
    }
}

struct _nala_data_params_for_usleep *nala_get_params_usleep()
{
    return (&nala_get_data_usleep()->params);
}

int __wrap_usleep(__useconds_t usec)
{
    struct _nala_instance_type_for_usleep *_nala_instance_p;
    struct _nala_data_type_for_usleep *_nala_data_p;
    int return_value;

    nala_print_call("usleep", &nala_state_for_usleep.state);

    switch (nala_state_for_usleep.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_usleep.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_usleep.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked usleep() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_usleep.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, usleep, usec);


        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(usec);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_usleep.data.implementation(usec);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_usleep.state.mode = 3;
    nala_state_for_usleep.data.params.usec = usec;
    nala_state_for_usleep.data.params.ignore_usec_in = false;
    nala_state_for_usleep.data.return_value = return_value;
    nala_state_for_usleep.data.errno_value = 0;
    nala_state_for_usleep.data.callback = NULL;
}

void usleep_mock_once(__useconds_t usec, int return_value)
{
    struct _nala_instance_type_for_usleep *_nala_instance_p;

    nala_state_for_usleep.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    _nala_instance_p->data.params.usec = usec;
    _nala_instance_p->data.params.ignore_usec_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_usleep.instances,
                          _nala_instance_p);
}

void usleep_mock_ignore_in(int return_value)
{
    nala_state_for_usleep.state.mode = 3;
    nala_state_for_usleep.data.params.ignore_usec_in = true;
    nala_state_for_usleep.data.return_value = return_value;
    nala_state_for_usleep.data.errno_value = 0;
    nala_state_for_usleep.data.callback = NULL;
}

void usleep_mock_ignore_in_once(int return_value)
{
    struct _nala_instance_type_for_usleep *instance_p;

    nala_state_for_usleep.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
    instance_p->data.params.ignore_usec_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->data.callback = NULL;
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_usleep.instances,
                          instance_p);
}

void usleep_mock_set_errno(int errno_value)
{
    nala_get_data_usleep()->errno_value = errno_value;
}

void usleep_mock_set_callback(void (*callback)(__useconds_t usec))
{
    nala_get_data_usleep()->callback = callback;
}

void usleep_mock_ignore_usec_in(void)
{
    nala_get_params_usleep()->ignore_usec_in = true;
}

void usleep_mock_none(void)
{
    nala_state_for_usleep.state.mode = 4;
}

void usleep_mock_implementation(int (*implementation)(__useconds_t usec))
{
    nala_state_for_usleep.state.mode = 2;
    nala_state_for_usleep.data.implementation = implementation;
}

void usleep_mock_disable(void)
{
    nala_state_for_usleep.state.mode = 0;
}

void usleep_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_usleep.state);
}

void usleep_mock_resume(void)
{
    nala_state_resume(&nala_state_for_usleep.state);
}

void usleep_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_usleep);
}

void usleep_mock_assert_completed(void)
{
    if (nala_state_for_usleep.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked usleep() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_usleep.instances.length));
    }
}

// NALA_IMPLEMENTATION write

ssize_t __real_write(int fd, const void *buf, size_t count);

struct _nala_data_params_for_write {
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

struct _nala_data_type_for_write {
    struct _nala_data_params_for_write params;
    ssize_t return_value;
    int errno_value;
    ssize_t (*implementation)(int fd, const void *buf, size_t count);
    void (*callback)(int fd, const void *buf, size_t count);
    struct nala_traceback_t traceback;
};

struct _nala_instance_type_for_write {
    struct _nala_data_type_for_write data;
    struct _nala_instance_type_for_write *next_p;
};

struct _nala_instances_type_for_write {
    struct _nala_instance_type_for_write *head_p;
    struct _nala_instance_type_for_write *tail_p;
    int length;
};

struct nala_state_type_for_write {
    struct nala_state_t state;
    struct _nala_data_type_for_write data;
    struct _nala_instances_type_for_write instances;
};

static struct nala_state_type_for_write nala_state_for_write = {
    .state = {
        .mode = 0,
        .suspended = {
            .count = 0,
            .mode = 0
        }
    },
    .instances = {
        .head_p = NULL,
        .tail_p = NULL,
        .length = 0
    }
};

struct _nala_data_type_for_write *nala_get_data_write()
{
    if (nala_state_for_write.instances.tail_p != NULL) {
        return (&nala_state_for_write.instances.tail_p->data);
    } else {
        return (&nala_state_for_write.data);
    }
}

struct _nala_data_params_for_write *nala_get_params_write()
{
    return (&nala_get_data_write()->params);
}

ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
    struct _nala_instance_type_for_write *_nala_instance_p;
    struct _nala_data_type_for_write *_nala_data_p;
    ssize_t return_value;

    nala_print_call("write", &nala_state_for_write.state);

    switch (nala_state_for_write.state.mode) {

    case 1:
    case 3:
        if (nala_state_for_write.state.mode == 1) {
            NALA_INSTANCES_POP(nala_state_for_write.instances, &_nala_instance_p);

            if (_nala_instance_p == NULL) {
                NALA_TEST_FAILURE(nala_format(
                        "Mocked write() called more times than expected.\n"));
            }

            _nala_data_p = &_nala_instance_p->data;
        } else {
            _nala_instance_p = NULL;
            _nala_data_p = &nala_state_for_write.data;
        }

        MOCK_ASSERT_IN_EQ(_nala_data_p, write, buf);
        MOCK_ASSERT_IN_EQ(_nala_data_p, write, fd);
        MOCK_ASSERT_IN_EQ(_nala_data_p, write, count);

        MOCK_ASSERT_COPY_SET_PARAM(_nala_instance_p,
                                   _nala_data_p,
                                   nala_mock_assert_memory,
                                   write,
                                   buf);

        errno = _nala_data_p->errno_value;

        if (_nala_data_p->callback != NULL) {
            _nala_data_p->callback(fd, buf, count);
        }

        return_value = _nala_data_p->return_value;

        if (_nala_instance_p != NULL) {
            nala_free(_nala_instance_p);
        }
        break;

    case 2:
        return_value =
        nala_state_for_write.data.implementation(fd, buf, count);
        break;

    case 4:
        nala_mock_none_fail();
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
    nala_state_for_write.state.mode = 3;
    nala_state_for_write.data.params.buf = NULL;
    nala_state_for_write.data.params.ignore_buf_in = true;
    nala_state_for_write.data.params.fd = fd;
    nala_state_for_write.data.params.ignore_fd_in = false;
    nala_state_for_write.data.params.count = count;
    nala_state_for_write.data.params.ignore_count_in = false;
    nala_state_for_write.data.return_value = return_value;
    nala_state_for_write.data.errno_value = 0;
    nala_state_for_write.data.callback = NULL;
}

void write_mock_once(int fd, size_t count, ssize_t return_value)
{
    struct _nala_instance_type_for_write *_nala_instance_p;

    nala_state_for_write.state.mode = 1;
    _nala_instance_p = nala_xmalloc(sizeof(*_nala_instance_p));
    nala_traceback(&_nala_instance_p->data.traceback);
    nala_set_param_init(&_nala_instance_p->data.params.buf_out);
    nala_set_param_init(&_nala_instance_p->data.params.buf_in);
    _nala_instance_p->data.params.buf_in_assert = NULL;
    _nala_instance_p->data.params.buf_out_copy = NULL;
    _nala_instance_p->data.params.buf = NULL;
    _nala_instance_p->data.params.ignore_buf_in = true;
    _nala_instance_p->data.params.fd = fd;
    _nala_instance_p->data.params.ignore_fd_in = false;
    _nala_instance_p->data.params.count = count;
    _nala_instance_p->data.params.ignore_count_in = false;
    _nala_instance_p->data.return_value = return_value;
    _nala_instance_p->data.errno_value = 0;
    _nala_instance_p->data.callback = NULL;
    _nala_instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_write.instances,
                          _nala_instance_p);
}

void write_mock_ignore_in(ssize_t return_value)
{
    nala_state_for_write.state.mode = 3;
    nala_state_for_write.data.params.ignore_buf_in = true;
    nala_state_for_write.data.params.ignore_fd_in = true;
    nala_state_for_write.data.params.ignore_count_in = true;
    nala_state_for_write.data.return_value = return_value;
    nala_state_for_write.data.errno_value = 0;
    nala_state_for_write.data.callback = NULL;
}

void write_mock_ignore_in_once(ssize_t return_value)
{
    struct _nala_instance_type_for_write *instance_p;

    nala_state_for_write.state.mode = 1;
    instance_p = nala_xmalloc(sizeof(*instance_p));
    nala_traceback(&instance_p->data.traceback);
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
    instance_p->next_p = NULL;

    NALA_INSTANCES_APPEND(nala_state_for_write.instances,
                          instance_p);
}

void write_mock_set_errno(int errno_value)
{
    nala_get_data_write()->errno_value = errno_value;
}

void write_mock_set_callback(void (*callback)(int fd, const void *buf, size_t count))
{
    nala_get_data_write()->callback = callback;
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
    nala_get_params_write()->buf_in_assert = callback;
}

void write_mock_set_buf_in_pointer(const void *buf)
{
    struct _nala_data_params_for_write *_nala_params_p;

    _nala_params_p = nala_get_params_write();
    _nala_params_p->ignore_buf_in = false;
    _nala_params_p->buf = buf;
}

void write_mock_set_buf_out(const void *buf_p, size_t size)
{
    nala_set_param_buf(&nala_get_params_write()->buf_out,
                       buf_p,
                       size);
}

void write_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size))
{
    nala_get_params_write()->buf_out_copy = callback;
}

void write_mock_none(void)
{
    nala_state_for_write.state.mode = 4;
}

void write_mock_implementation(ssize_t (*implementation)(int fd, const void *buf, size_t count))
{
    nala_state_for_write.state.mode = 2;
    nala_state_for_write.data.implementation = implementation;
}

void write_mock_disable(void)
{
    nala_state_for_write.state.mode = 0;
}

void write_mock_suspend(void)
{
    nala_state_suspend(&nala_state_for_write.state);
}

void write_mock_resume(void)
{
    nala_state_resume(&nala_state_for_write.state);
}

void write_mock_reset(void)
{
    NALA_STATE_RESET(nala_state_for_write);
}

void write_mock_assert_completed(void)
{
    if (nala_state_for_write.instances.length != 0) {
        NALA_TEST_FAILURE(nala_format(
             "Mocked write() called fewer times than expected. %d call(s) "
             "missing.\n",
             nala_state_for_write.instances.length));
    }
}

