/*
Mocks source file

Generated with Nala version 0.6.0 (https://github.com/eerimoq/nala)
Do not edit manually
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "nala.h"

#include <errno.h>
#include <stddef.h>

#include "__mocks__.h"

#define _NALA_INSTANCES_APPEND(list, item_p)    \
    do {                                        \
        if ((list).head_p == NULL) {            \
            (list).head_p = instance_p;         \
        } else {                                \
            (list).tail_p->next_p = instance_p; \
        }                                       \
                                                \
        (list).tail_p = instance_p;             \
    } while (0);

#define _NALA_INSTANCES_POP(list, instance_pp)          \
    do {                                                \
        *(instance_pp) = (list).head_p;                 \
                                                        \
        if (*(instance_pp) != NULL) {                   \
            (list).head_p = (*(instance_pp))->next_p;   \
                                                        \
            if ((*(instance_pp))->next_p == NULL) {     \
                (list).tail_p = NULL;                   \
            }                                           \
        }                                               \
    } while (0);

typedef struct _nala_set_param _nala_set_param;

struct _nala_set_param {
    void *buf_p;
    size_t size;
};

static void *_nala_malloc(size_t size)
{
    void *buf_p;

    buf_p = malloc(size);

    if (buf_p == NULL) {
        perror("malloc");
        exit(1);
    }

    return (buf_p);
}

enum _nala_va_arg_item_type_t {
    _nala_va_arg_item_type_d_t = 0,
    _nala_va_arg_item_type_u_t,
    _nala_va_arg_item_type_p_t
};

struct _nala_va_arg_item_t {
    enum _nala_va_arg_item_type_t type;
    bool ignore_in;
    union {
        int d;
        unsigned int u;
        void *p_p;
    };
    _nala_set_param in;
    _nala_set_param out;
    struct _nala_va_arg_item_t *next_p;
};

struct _nala_va_arg_list_t {
    struct _nala_va_arg_item_t *head_p;
    struct _nala_va_arg_item_t *tail_p;
    unsigned int length;
};

void _nala_va_arg_list_init(struct _nala_va_arg_list_t *self_p)
{
    self_p->head_p = NULL;
    self_p->tail_p = NULL;
    self_p->length = 0;
}

void _nala_va_arg_list_destroy(struct _nala_va_arg_list_t *self_p)
{
    struct _nala_va_arg_item_t *item_p;
    struct _nala_va_arg_item_t *tmp_p;

    item_p = self_p->head_p;

    while (item_p != NULL) {
        if (item_p->in.buf_p != NULL) {
            free(item_p->in.buf_p);
        }

        if (item_p->out.buf_p) {
            free(item_p->out.buf_p);
        }

        tmp_p = item_p;
        item_p = tmp_p->next_p;
        free(tmp_p);
    }
}

void _nala_va_arg_list_append(struct _nala_va_arg_list_t *self_p,
                                 struct _nala_va_arg_item_t *item_p)
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

struct _nala_va_arg_item_t *_nala_va_arg_list_get(
    struct _nala_va_arg_list_t *self_p,
    unsigned int index)
{
    unsigned int i;
    struct _nala_va_arg_item_t *item_p;

    if (index >= self_p->length) {
        fprintf(stderr,
                "error: Trying to access variable argument at index %u when\n"
                "       only %u exists.\n",
                index,
                self_p->length);
        FAIL();
    }

    item_p = self_p->head_p;

    for (i = 0; (i < index) && (item_p != NULL); i++) {
        item_p = item_p->next_p;
    }

    return (item_p);
}

struct _nala_va_arg_item_t *_nala_parse_va_arg(const char **format_pp,
                                                     va_list vl)
{
    struct _nala_va_arg_item_t *item_p;

    item_p = _nala_malloc(sizeof(*item_p));
    item_p->in.buf_p = NULL;
    item_p->out.buf_p = NULL;

    switch (**format_pp) {

    case 'd':
        item_p->type = _nala_va_arg_item_type_d_t;
        item_p->ignore_in = false;
        item_p->d = va_arg(vl, int);
        break;

    case 'u':
        item_p->type = _nala_va_arg_item_type_u_t;
        item_p->ignore_in = false;
        item_p->u = va_arg(vl, unsigned int);
        break;

    case 'p':
        item_p->type = _nala_va_arg_item_type_p_t;
        item_p->ignore_in = true;
        item_p->p_p = NULL;
        break;

    default:
        free(item_p);
        FAIL();
        exit(1);
        break;
    }

    (*format_pp)++;

    return (item_p);
}

int _nala_parse_va_list(struct _nala_va_arg_list_t *list_p,
                           const char *format_p,
                           va_list vl)
{
    int res;
    struct _nala_va_arg_item_t *item_p;

    res = 1;
    _nala_va_arg_list_init(list_p);

    while (res == 1) {
        if (*format_p == '\0') {
            res = 0;
        } else if (*format_p == '%') {
            format_p++;
            item_p = _nala_parse_va_arg(&format_p, vl);

            if (item_p == NULL) {
                res = -1;
            } else {
                _nala_va_arg_list_append(list_p, item_p);
            }
        } else {
            res = -1;
        }
    }

    return (res);
}

void _nala_va_arg_list_assert_d(struct _nala_va_arg_item_t *item_p,
                                   int value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->d, value);
    }
}

void _nala_va_arg_list_assert_u(struct _nala_va_arg_item_t *item_p,
                                   unsigned int value)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->u, value);
    }
}

void _nala_va_arg_list_assert_p(struct _nala_va_arg_item_t *item_p,
                                   void *value_p)
{
    if (!item_p->ignore_in) {
        ASSERT_EQ(item_p->p_p, value_p);
    }

    if (item_p->out.buf_p != NULL) {
        memcpy(value_p, item_p->out.buf_p, item_p->out.size);
    }

    if (item_p->in.buf_p != NULL) {
        ASSERT_MEMORY(value_p, item_p->in.buf_p, item_p->in.size);
    }
}

void _nala_va_arg_list_assert(struct _nala_va_arg_list_t *self_p,
                                 va_list vl)
{
    unsigned int i;
    struct _nala_va_arg_item_t *item_p;

    item_p = self_p->head_p;

    for (i = 0; i < self_p->length; i++) {
        switch (item_p->type) {

        case _nala_va_arg_item_type_d_t:
            _nala_va_arg_list_assert_d(item_p, va_arg(vl, int));
            break;

        case _nala_va_arg_item_type_u_t:
            _nala_va_arg_list_assert_u(item_p, va_arg(vl, unsigned int));
            break;

        case _nala_va_arg_item_type_p_t:
            _nala_va_arg_list_assert_p(item_p, va_arg(vl, void *));
            break;

        default:
            FAIL();
            exit(1);
            break;
        }

        item_p = item_p->next_p;
    }
}

void nala_reset_all_mocks(void)
{
    add_mock_reset();
    compose_twice_mock_reset();
    edit_number_mock_reset();
    fopen_mock_reset();
    io_control_mock_reset();
    mount_mock_reset();
    pipe_mock_reset();
    time_mock_reset();
}

void nala_assert_all_mocks_completed(void)
{
    add_mock_assert_completed();
    compose_twice_mock_assert_completed();
    edit_number_mock_assert_completed();
    fopen_mock_assert_completed();
    io_control_mock_assert_completed();
    mount_mock_assert_completed();
    pipe_mock_assert_completed();
    time_mock_assert_completed();
}

// NALA_IMPLEMENTATION add

int __real_add(int x, int y);

typedef struct _nala_data_type_for_add _nala_data_type_for_add;

struct _nala_data_type_for_add
{
    int x;
    int y;
    int return_value;
    bool ignore_x_in;
    bool ignore_y_in;
    int errno_value;
    int (*implementation)(int x, int y);
};

typedef struct _nala_instance_type_for_add _nala_instance_type_for_add;

struct _nala_instance_type_for_add
{
    _nala_data_type_for_add data;
    _nala_instance_type_for_add *next_p;
};

typedef struct _nala_instances_type_for_add _nala_instances_type_for_add;

struct _nala_instances_type_for_add {
    _nala_instance_type_for_add *head_p;
    _nala_instance_type_for_add *tail_p;
};

typedef struct _nala_state_type_for_add _nala_state_type_for_add;

struct _nala_state_type_for_add
{
    int mode;
    _nala_data_type_for_add data;
    _nala_instances_type_for_add instances;
};

static _nala_state_type_for_add _nala_state_for_add =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

int __wrap_add(int x, int y)
{
    struct _nala_instance_type_for_add *instance_p;
    int return_value;

    switch (_nala_state_for_add.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_add.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "add() called more times than expected.\n");
        }

        if (!instance_p->data.ignore_x_in) {
            ASSERT_EQ(x, instance_p->data.x);
        }

        if (!instance_p->data.ignore_y_in) {
            ASSERT_EQ(y, instance_p->data.y);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_add.data.implementation(x, y);
        break;

    case 3:
        return_value = _nala_state_for_add.data.return_value;
        errno = _nala_state_for_add.data.errno_value;
        break;

    case 4:
        FAIL();
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
    _nala_state_for_add.mode = 3;
    _nala_state_for_add.data.x = x;
    _nala_state_for_add.data.ignore_x_in = false;
    _nala_state_for_add.data.y = y;
    _nala_state_for_add.data.ignore_y_in = false;
    _nala_state_for_add.data.return_value = return_value;
    _nala_state_for_add.data.errno_value = 0;
}

void add_mock_once(int x, int y, int return_value)
{
    struct _nala_instance_type_for_add *instance_p;

    _nala_state_for_add.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.x = x;
    instance_p->data.ignore_x_in = false;
    instance_p->data.y = y;
    instance_p->data.ignore_y_in = false;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_add.instances,
                              instance_p);
}

void add_mock_ignore_in(int return_value)
{
    _nala_state_for_add.mode = 3;
    _nala_state_for_add.data.ignore_x_in = true;
    _nala_state_for_add.data.ignore_y_in = true;
    _nala_state_for_add.data.return_value = return_value;
    _nala_state_for_add.data.errno_value = 0;
}

void add_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_add *instance_p;

    instance_p = _nala_state_for_add.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_add.data.errno_value = errno_value;
    }
}

void add_mock_ignore_x_in(void)
{
    struct _nala_instance_type_for_add *instance_p;

    instance_p = _nala_state_for_add.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_x_in = true;
    } else {
        fprintf(stderr,
                "error: add_mock_ignore_x_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void add_mock_ignore_y_in(void)
{
    struct _nala_instance_type_for_add *instance_p;

    instance_p = _nala_state_for_add.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_y_in = true;
    } else {
        fprintf(stderr,
                "error: add_mock_ignore_y_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void add_mock_none(void)
{
    _nala_state_for_add.mode = 4;
}

void add_mock_implementation(int (*implementation)(int x, int y))
{
    _nala_state_for_add.mode = 2;
    _nala_state_for_add.data.implementation = implementation;
}

void add_mock_disable(void)
{
    _nala_state_for_add.mode = 0;
}

void add_mock_reset(void)
{
    _nala_state_for_add.mode = 0;
    _nala_state_for_add.instances.head_p = NULL;
    _nala_state_for_add.instances.tail_p = NULL;
}

void add_mock_assert_completed(void)
{
    if (_nala_state_for_add.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "add() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION compose_twice

DummyStruct *__real_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));

typedef struct _nala_data_type_for_compose_twice _nala_data_type_for_compose_twice;

struct _nala_data_type_for_compose_twice
{
    DummyStruct *dummy_struct;
    DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct);
    DummyStruct *return_value;
    bool ignore_dummy_struct_in;
    _nala_set_param dummy_struct_in;
    _nala_set_param dummy_struct_out;
    bool ignore_dummy_struct_modifier_in;
    _nala_set_param dummy_struct_modifier_in;
    _nala_set_param dummy_struct_modifier_out;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
};

typedef struct _nala_instance_type_for_compose_twice _nala_instance_type_for_compose_twice;

struct _nala_instance_type_for_compose_twice
{
    _nala_data_type_for_compose_twice data;
    _nala_instance_type_for_compose_twice *next_p;
};

typedef struct _nala_instances_type_for_compose_twice _nala_instances_type_for_compose_twice;

struct _nala_instances_type_for_compose_twice {
    _nala_instance_type_for_compose_twice *head_p;
    _nala_instance_type_for_compose_twice *tail_p;
};

typedef struct _nala_state_type_for_compose_twice _nala_state_type_for_compose_twice;

struct _nala_state_type_for_compose_twice
{
    int mode;
    _nala_data_type_for_compose_twice data;
    _nala_instances_type_for_compose_twice instances;
};

static _nala_state_type_for_compose_twice _nala_state_for_compose_twice =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

DummyStruct *__wrap_compose_twice(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct _nala_instance_type_for_compose_twice *instance_p;
    DummyStruct *return_value;

    switch (_nala_state_for_compose_twice.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_compose_twice.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "compose_twice() called more times than expected.\n");
        }

        if (!instance_p->data.ignore_dummy_struct_in) {
            ASSERT_EQ(dummy_struct, instance_p->data.dummy_struct);
        }

        if (!instance_p->data.ignore_dummy_struct_modifier_in) {
            ASSERT_EQ(dummy_struct_modifier, instance_p->data.dummy_struct_modifier);
        }

        if (instance_p->data.dummy_struct_out.buf_p != NULL) {
            memcpy((void *)dummy_struct,
                   instance_p->data.dummy_struct_out.buf_p,
                   instance_p->data.dummy_struct_out.size);
            free(instance_p->data.dummy_struct_out.buf_p);
        }

        if (instance_p->data.dummy_struct_in.buf_p != NULL) {
            ASSERT_MEMORY(dummy_struct,
                          instance_p->data.dummy_struct_in.buf_p,
                          instance_p->data.dummy_struct_in.size);
            free(instance_p->data.dummy_struct_in.buf_p);
        }

        if (instance_p->data.dummy_struct_modifier_out.buf_p != NULL) {
            memcpy((void *)dummy_struct_modifier,
                   instance_p->data.dummy_struct_modifier_out.buf_p,
                   instance_p->data.dummy_struct_modifier_out.size);
            free(instance_p->data.dummy_struct_modifier_out.buf_p);
        }

        if (instance_p->data.dummy_struct_modifier_in.buf_p != NULL) {
            ASSERT_MEMORY(dummy_struct_modifier,
                          instance_p->data.dummy_struct_modifier_in.buf_p,
                          instance_p->data.dummy_struct_modifier_in.size);
            free(instance_p->data.dummy_struct_modifier_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_compose_twice.data.implementation(dummy_struct, dummy_struct_modifier);
        break;

    case 3:
        return_value = _nala_state_for_compose_twice.data.return_value;
        errno = _nala_state_for_compose_twice.data.errno_value;
        break;

    case 4:
        FAIL();
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
    _nala_state_for_compose_twice.mode = 3;
    _nala_state_for_compose_twice.data.dummy_struct = NULL;
    _nala_state_for_compose_twice.data.ignore_dummy_struct_in = true;
    _nala_state_for_compose_twice.data.dummy_struct_modifier = NULL;
    _nala_state_for_compose_twice.data.ignore_dummy_struct_modifier_in = true;
    _nala_state_for_compose_twice.data.return_value = return_value;
    _nala_state_for_compose_twice.data.errno_value = 0;
}

void compose_twice_mock_once(DummyStruct *return_value)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    _nala_state_for_compose_twice.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.dummy_struct_out.buf_p = NULL;
    instance_p->data.dummy_struct_out.size = 0;
    instance_p->data.dummy_struct_in.buf_p = NULL;
    instance_p->data.dummy_struct_in.size = 0;
    instance_p->data.dummy_struct_modifier_out.buf_p = NULL;
    instance_p->data.dummy_struct_modifier_out.size = 0;
    instance_p->data.dummy_struct_modifier_in.buf_p = NULL;
    instance_p->data.dummy_struct_modifier_in.size = 0;
    instance_p->data.dummy_struct = NULL;
    instance_p->data.ignore_dummy_struct_in = true;
    instance_p->data.dummy_struct_modifier = NULL;
    instance_p->data.ignore_dummy_struct_modifier_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_compose_twice.instances,
                              instance_p);
}

void compose_twice_mock_ignore_in(DummyStruct *return_value)
{
    _nala_state_for_compose_twice.mode = 3;
    _nala_state_for_compose_twice.data.ignore_dummy_struct_in = true;
    _nala_state_for_compose_twice.data.ignore_dummy_struct_modifier_in = true;
    _nala_state_for_compose_twice.data.return_value = return_value;
    _nala_state_for_compose_twice.data.errno_value = 0;
}

void compose_twice_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_compose_twice.data.errno_value = errno_value;
    }
}

void compose_twice_mock_set_dummy_struct_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_in.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_in.size = size;
        memcpy(instance_p->data.dummy_struct_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_dummy_struct_in = false;
        instance_p->data.dummy_struct = dummy_struct;
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_out.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_out.size = size;
        memcpy(instance_p->data.dummy_struct_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_set_dummy_struct_modifier_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_modifier_in.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_modifier_in.size = size;
        memcpy(instance_p->data.dummy_struct_modifier_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_modifier_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_set_dummy_struct_modifier_in_pointer(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_dummy_struct_modifier_in = false;
        instance_p->data.dummy_struct_modifier = dummy_struct_modifier;
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_modifier_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_set_dummy_struct_modifier_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_compose_twice *instance_p;

    instance_p = _nala_state_for_compose_twice.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_modifier_out.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_modifier_out.size = size;
        memcpy(instance_p->data.dummy_struct_modifier_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: compose_twice_mock_set_dummy_struct_modifier_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void compose_twice_mock_none(void)
{
    _nala_state_for_compose_twice.mode = 4;
}

void compose_twice_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)))
{
    _nala_state_for_compose_twice.mode = 2;
    _nala_state_for_compose_twice.data.implementation = implementation;
}

void compose_twice_mock_disable(void)
{
    _nala_state_for_compose_twice.mode = 0;
}

void compose_twice_mock_reset(void)
{
    _nala_state_for_compose_twice.mode = 0;
    _nala_state_for_compose_twice.instances.head_p = NULL;
    _nala_state_for_compose_twice.instances.tail_p = NULL;
}

void compose_twice_mock_assert_completed(void)
{
    if (_nala_state_for_compose_twice.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "compose_twice() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION edit_number

DummyStruct *__real_edit_number(DummyStruct *dummy_struct, int number);

typedef struct _nala_data_type_for_edit_number _nala_data_type_for_edit_number;

struct _nala_data_type_for_edit_number
{
    DummyStruct *dummy_struct;
    int number;
    DummyStruct *return_value;
    bool ignore_dummy_struct_in;
    _nala_set_param dummy_struct_in;
    _nala_set_param dummy_struct_out;
    bool ignore_number_in;
    int errno_value;
    DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number);
};

typedef struct _nala_instance_type_for_edit_number _nala_instance_type_for_edit_number;

struct _nala_instance_type_for_edit_number
{
    _nala_data_type_for_edit_number data;
    _nala_instance_type_for_edit_number *next_p;
};

typedef struct _nala_instances_type_for_edit_number _nala_instances_type_for_edit_number;

struct _nala_instances_type_for_edit_number {
    _nala_instance_type_for_edit_number *head_p;
    _nala_instance_type_for_edit_number *tail_p;
};

typedef struct _nala_state_type_for_edit_number _nala_state_type_for_edit_number;

struct _nala_state_type_for_edit_number
{
    int mode;
    _nala_data_type_for_edit_number data;
    _nala_instances_type_for_edit_number instances;
};

static _nala_state_type_for_edit_number _nala_state_for_edit_number =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

DummyStruct *__wrap_edit_number(DummyStruct *dummy_struct, int number)
{
    struct _nala_instance_type_for_edit_number *instance_p;
    DummyStruct *return_value;

    switch (_nala_state_for_edit_number.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_edit_number.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "edit_number() called more times than expected.\n");
        }

        if (!instance_p->data.ignore_dummy_struct_in) {
            ASSERT_EQ(dummy_struct, instance_p->data.dummy_struct);
        }

        if (!instance_p->data.ignore_number_in) {
            ASSERT_EQ(number, instance_p->data.number);
        }

        if (instance_p->data.dummy_struct_out.buf_p != NULL) {
            memcpy((void *)dummy_struct,
                   instance_p->data.dummy_struct_out.buf_p,
                   instance_p->data.dummy_struct_out.size);
            free(instance_p->data.dummy_struct_out.buf_p);
        }

        if (instance_p->data.dummy_struct_in.buf_p != NULL) {
            ASSERT_MEMORY(dummy_struct,
                          instance_p->data.dummy_struct_in.buf_p,
                          instance_p->data.dummy_struct_in.size);
            free(instance_p->data.dummy_struct_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_edit_number.data.implementation(dummy_struct, number);
        break;

    case 3:
        return_value = _nala_state_for_edit_number.data.return_value;
        errno = _nala_state_for_edit_number.data.errno_value;
        break;

    case 4:
        FAIL();
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
    _nala_state_for_edit_number.mode = 3;
    _nala_state_for_edit_number.data.dummy_struct = NULL;
    _nala_state_for_edit_number.data.ignore_dummy_struct_in = true;
    _nala_state_for_edit_number.data.number = number;
    _nala_state_for_edit_number.data.ignore_number_in = false;
    _nala_state_for_edit_number.data.return_value = return_value;
    _nala_state_for_edit_number.data.errno_value = 0;
}

void edit_number_mock_once(int number, DummyStruct *return_value)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    _nala_state_for_edit_number.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.dummy_struct_out.buf_p = NULL;
    instance_p->data.dummy_struct_out.size = 0;
    instance_p->data.dummy_struct_in.buf_p = NULL;
    instance_p->data.dummy_struct_in.size = 0;
    instance_p->data.dummy_struct = NULL;
    instance_p->data.ignore_dummy_struct_in = true;
    instance_p->data.number = number;
    instance_p->data.ignore_number_in = false;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_edit_number.instances,
                              instance_p);
}

void edit_number_mock_ignore_in(DummyStruct *return_value)
{
    _nala_state_for_edit_number.mode = 3;
    _nala_state_for_edit_number.data.ignore_dummy_struct_in = true;
    _nala_state_for_edit_number.data.ignore_number_in = true;
    _nala_state_for_edit_number.data.return_value = return_value;
    _nala_state_for_edit_number.data.errno_value = 0;
}

void edit_number_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    instance_p = _nala_state_for_edit_number.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_edit_number.data.errno_value = errno_value;
    }
}

void edit_number_mock_ignore_number_in(void)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    instance_p = _nala_state_for_edit_number.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_number_in = true;
    } else {
        fprintf(stderr,
                "error: edit_number_mock_ignore_number_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void edit_number_mock_set_dummy_struct_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    instance_p = _nala_state_for_edit_number.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_in.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_in.size = size;
        memcpy(instance_p->data.dummy_struct_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: edit_number_mock_set_dummy_struct_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    instance_p = _nala_state_for_edit_number.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_dummy_struct_in = false;
        instance_p->data.dummy_struct = dummy_struct;
    } else {
        fprintf(stderr,
                "error: edit_number_mock_set_dummy_struct_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_edit_number *instance_p;

    instance_p = _nala_state_for_edit_number.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.dummy_struct_out.buf_p = _nala_malloc(size);
        instance_p->data.dummy_struct_out.size = size;
        memcpy(instance_p->data.dummy_struct_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: edit_number_mock_set_dummy_struct_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void edit_number_mock_none(void)
{
    _nala_state_for_edit_number.mode = 4;
}

void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number))
{
    _nala_state_for_edit_number.mode = 2;
    _nala_state_for_edit_number.data.implementation = implementation;
}

void edit_number_mock_disable(void)
{
    _nala_state_for_edit_number.mode = 0;
}

void edit_number_mock_reset(void)
{
    _nala_state_for_edit_number.mode = 0;
    _nala_state_for_edit_number.instances.head_p = NULL;
    _nala_state_for_edit_number.instances.tail_p = NULL;
}

void edit_number_mock_assert_completed(void)
{
    if (_nala_state_for_edit_number.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "edit_number() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION fopen

FILE *__real_fopen(const char *__filename, const char *__modes);

typedef struct _nala_data_type_for_fopen _nala_data_type_for_fopen;

struct _nala_data_type_for_fopen
{
    const char *__filename;
    const char *__modes;
    FILE *return_value;
    bool ignore___filename_in;
    _nala_set_param __filename_in;
    _nala_set_param __filename_out;
    bool ignore___modes_in;
    _nala_set_param __modes_in;
    _nala_set_param __modes_out;
    int errno_value;
    FILE *(*implementation)(const char *__filename, const char *__modes);
};

typedef struct _nala_instance_type_for_fopen _nala_instance_type_for_fopen;

struct _nala_instance_type_for_fopen
{
    _nala_data_type_for_fopen data;
    _nala_instance_type_for_fopen *next_p;
};

typedef struct _nala_instances_type_for_fopen _nala_instances_type_for_fopen;

struct _nala_instances_type_for_fopen {
    _nala_instance_type_for_fopen *head_p;
    _nala_instance_type_for_fopen *tail_p;
};

typedef struct _nala_state_type_for_fopen _nala_state_type_for_fopen;

struct _nala_state_type_for_fopen
{
    int mode;
    _nala_data_type_for_fopen data;
    _nala_instances_type_for_fopen instances;
};

static _nala_state_type_for_fopen _nala_state_for_fopen =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

FILE *__wrap_fopen(const char *__filename, const char *__modes)
{
    struct _nala_instance_type_for_fopen *instance_p;
    FILE *return_value;

    switch (_nala_state_for_fopen.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_fopen.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "fopen() called more times than expected.\n");
        }

        if (!instance_p->data.ignore___filename_in) {
            ASSERT_EQ((const void *)__filename,
                      (const void *)instance_p->data.__filename);
        }

        if (!instance_p->data.ignore___modes_in) {
            ASSERT_EQ((const void *)__modes,
                      (const void *)instance_p->data.__modes);
        }

        if (instance_p->data.__filename_out.buf_p != NULL) {
            memcpy((void *)__filename,
                   instance_p->data.__filename_out.buf_p,
                   instance_p->data.__filename_out.size);
            free(instance_p->data.__filename_out.buf_p);
        }

        if (instance_p->data.__filename_in.buf_p != NULL) {
            ASSERT_MEMORY(__filename,
                          instance_p->data.__filename_in.buf_p,
                          instance_p->data.__filename_in.size);
            free(instance_p->data.__filename_in.buf_p);
        }

        if (instance_p->data.__modes_out.buf_p != NULL) {
            memcpy((void *)__modes,
                   instance_p->data.__modes_out.buf_p,
                   instance_p->data.__modes_out.size);
            free(instance_p->data.__modes_out.buf_p);
        }

        if (instance_p->data.__modes_in.buf_p != NULL) {
            ASSERT_MEMORY(__modes,
                          instance_p->data.__modes_in.buf_p,
                          instance_p->data.__modes_in.size);
            free(instance_p->data.__modes_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_fopen.data.implementation(__filename, __modes);
        break;

    case 3:
        return_value = _nala_state_for_fopen.data.return_value;
        errno = _nala_state_for_fopen.data.errno_value;
        break;

    case 4:
        FAIL();
        exit(1);
        break;

    default:
        return_value =
        __real_fopen(__filename, __modes);
        break;
    }

    return return_value;
}

void fopen_mock(const char *__filename, const char *__modes, FILE *return_value)
{
    _nala_state_for_fopen.mode = 3;
    _nala_state_for_fopen.data.__filename = NULL;
    _nala_state_for_fopen.data.ignore___filename_in = true;

    if (__filename != NULL) {
        _nala_state_for_fopen.data.__filename_in.size = (strlen(__filename) + 1);
        _nala_state_for_fopen.data.__filename_in.buf_p = _nala_malloc(_nala_state_for_fopen.data.__filename_in.size);
        strcpy(_nala_state_for_fopen.data.__filename_in.buf_p, __filename);
    } else {
        _nala_state_for_fopen.data.ignore___filename_in = false;
    }

    _nala_state_for_fopen.data.__modes = NULL;
    _nala_state_for_fopen.data.ignore___modes_in = true;

    if (__modes != NULL) {
        _nala_state_for_fopen.data.__modes_in.size = (strlen(__modes) + 1);
        _nala_state_for_fopen.data.__modes_in.buf_p = _nala_malloc(_nala_state_for_fopen.data.__modes_in.size);
        strcpy(_nala_state_for_fopen.data.__modes_in.buf_p, __modes);
    } else {
        _nala_state_for_fopen.data.ignore___modes_in = false;
    }

    _nala_state_for_fopen.data.return_value = return_value;
    _nala_state_for_fopen.data.errno_value = 0;
}

void fopen_mock_once(const char *__filename, const char *__modes, FILE *return_value)
{
    struct _nala_instance_type_for_fopen *instance_p;

    _nala_state_for_fopen.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.__filename_out.buf_p = NULL;
    instance_p->data.__filename_out.size = 0;
    instance_p->data.__filename_in.buf_p = NULL;
    instance_p->data.__filename_in.size = 0;
    instance_p->data.__modes_out.buf_p = NULL;
    instance_p->data.__modes_out.size = 0;
    instance_p->data.__modes_in.buf_p = NULL;
    instance_p->data.__modes_in.size = 0;
    instance_p->data.__filename = NULL;
    instance_p->data.ignore___filename_in = true;

    if (__filename != NULL) {
        instance_p->data.__filename_in.size = (strlen(__filename) + 1);
        instance_p->data.__filename_in.buf_p = _nala_malloc(instance_p->data.__filename_in.size);
        strcpy(instance_p->data.__filename_in.buf_p, __filename);
    } else {
        instance_p->data.ignore___filename_in = false;
    }

    instance_p->data.__modes = NULL;
    instance_p->data.ignore___modes_in = true;

    if (__modes != NULL) {
        instance_p->data.__modes_in.size = (strlen(__modes) + 1);
        instance_p->data.__modes_in.buf_p = _nala_malloc(instance_p->data.__modes_in.size);
        strcpy(instance_p->data.__modes_in.buf_p, __modes);
    } else {
        instance_p->data.ignore___modes_in = false;
    }

    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_fopen.instances,
                              instance_p);
}

void fopen_mock_ignore_in(FILE *return_value)
{
    _nala_state_for_fopen.mode = 3;
    _nala_state_for_fopen.data.ignore___filename_in = true;
    _nala_state_for_fopen.data.ignore___modes_in = true;
    _nala_state_for_fopen.data.return_value = return_value;
    _nala_state_for_fopen.data.errno_value = 0;
}

void fopen_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_fopen.data.errno_value = errno_value;
    }
}

void fopen_mock_ignore___filename_in(void)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___filename_in = true;
    } else {
        fprintf(stderr,
                "error: fopen_mock_ignore___filename_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_ignore___modes_in(void)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___modes_in = true;
    } else {
        fprintf(stderr,
                "error: fopen_mock_ignore___modes_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___filename_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__filename_in.buf_p = _nala_malloc(size);
        instance_p->data.__filename_in.size = size;
        memcpy(instance_p->data.__filename_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___filename_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___filename_in_pointer(const char *__filename)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___filename_in = false;
        instance_p->data.__filename = __filename;
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___filename_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___filename_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__filename_out.buf_p = _nala_malloc(size);
        instance_p->data.__filename_out.size = size;
        memcpy(instance_p->data.__filename_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___filename_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___modes_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__modes_in.buf_p = _nala_malloc(size);
        instance_p->data.__modes_in.size = size;
        memcpy(instance_p->data.__modes_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___modes_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___modes_in_pointer(const char *__modes)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___modes_in = false;
        instance_p->data.__modes = __modes;
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___modes_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_set___modes_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_fopen *instance_p;

    instance_p = _nala_state_for_fopen.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__modes_out.buf_p = _nala_malloc(size);
        instance_p->data.__modes_out.size = size;
        memcpy(instance_p->data.__modes_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: fopen_mock_set___modes_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void fopen_mock_none(void)
{
    _nala_state_for_fopen.mode = 4;
}

void fopen_mock_implementation(FILE *(*implementation)(const char *__filename, const char *__modes))
{
    _nala_state_for_fopen.mode = 2;
    _nala_state_for_fopen.data.implementation = implementation;
}

void fopen_mock_disable(void)
{
    _nala_state_for_fopen.mode = 0;
}

void fopen_mock_reset(void)
{
    _nala_state_for_fopen.mode = 0;
    _nala_state_for_fopen.instances.head_p = NULL;
    _nala_state_for_fopen.instances.tail_p = NULL;
}

void fopen_mock_assert_completed(void)
{
    if (_nala_state_for_fopen.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "fopen() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION io_control

typedef struct _nala_data_type_for_io_control _nala_data_type_for_io_control;

struct _nala_data_type_for_io_control
{
    int kind;
    const char *vafmt_p;
    int return_value;
    bool ignore_kind_in;
    struct _nala_va_arg_list_t va_arg_list;
    int errno_value;
    int (*implementation)(int kind, va_list __nala_va_list);
};

typedef struct _nala_instance_type_for_io_control _nala_instance_type_for_io_control;

struct _nala_instance_type_for_io_control
{
    _nala_data_type_for_io_control data;
    _nala_instance_type_for_io_control *next_p;
};

typedef struct _nala_instances_type_for_io_control _nala_instances_type_for_io_control;

struct _nala_instances_type_for_io_control {
    _nala_instance_type_for_io_control *head_p;
    _nala_instance_type_for_io_control *tail_p;
};

typedef struct _nala_state_type_for_io_control _nala_state_type_for_io_control;

struct _nala_state_type_for_io_control
{
    int mode;
    _nala_data_type_for_io_control data;
    _nala_instances_type_for_io_control instances;
};

static _nala_state_type_for_io_control _nala_state_for_io_control =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

int __wrap_io_control(int kind, ...)
{
    struct _nala_instance_type_for_io_control *instance_p;
    int return_value;

    switch (_nala_state_for_io_control.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_io_control.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "io_control() called more times than expected.\n");
        }

        if (!instance_p->data.ignore_kind_in) {
            ASSERT_EQ(kind, instance_p->data.kind);
        }

        {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            _nala_va_arg_list_assert(&instance_p->data.va_arg_list, __nala_vl);
            va_end(__nala_vl);
            _nala_va_arg_list_destroy(&instance_p->data.va_arg_list);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        {
            va_list __nala_vl;
            va_start(__nala_vl, kind);
            return_value =
            _nala_state_for_io_control.data.implementation(kind, __nala_vl);
            va_end(__nala_vl);
        }
        break;

    case 3:
        return_value = _nala_state_for_io_control.data.return_value;
        errno = _nala_state_for_io_control.data.errno_value;
        break;

    case 4:
        FAIL();
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
    _nala_state_for_io_control.mode = 3;

    if (vafmt_p == NULL) {
        FAIL();
    }

    _nala_state_for_io_control.data.vafmt_p = vafmt_p;
    _nala_va_arg_list_init(&_nala_state_for_io_control.data.va_arg_list);
    va_list __nala_vl;
    va_start(__nala_vl, vafmt_p);
    _nala_parse_va_list(&_nala_state_for_io_control.data.va_arg_list,
                           vafmt_p,
                           __nala_vl);
    va_end(__nala_vl);
    _nala_state_for_io_control.data.kind = kind;
    _nala_state_for_io_control.data.ignore_kind_in = false;
    _nala_state_for_io_control.data.return_value = return_value;
    _nala_state_for_io_control.data.errno_value = 0;
}

void io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...)
{
    struct _nala_instance_type_for_io_control *instance_p;

    _nala_state_for_io_control.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));

    if (vafmt_p == NULL) {
        FAIL();
    }

    instance_p->data.vafmt_p = vafmt_p;
    _nala_va_arg_list_init(&instance_p->data.va_arg_list);
    va_list __nala_vl;
    va_start(__nala_vl, vafmt_p);
    _nala_parse_va_list(&instance_p->data.va_arg_list,
                           vafmt_p,
                           __nala_vl);
    va_end(__nala_vl);
    instance_p->data.kind = kind;
    instance_p->data.ignore_kind_in = false;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_io_control.instances,
                              instance_p);
}

void io_control_mock_ignore_in(int return_value)
{
    _nala_state_for_io_control.mode = 3;
    _nala_state_for_io_control.data.ignore_kind_in = true;
    _nala_state_for_io_control.data.return_value = return_value;
    _nala_state_for_io_control.data.errno_value = 0;
}

void io_control_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_io_control.data.errno_value = errno_value;
    }
}

void io_control_mock_ignore_kind_in(void)
{
    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore_kind_in = true;
    } else {
        fprintf(stderr,
                "error: io_control_mock_ignore_kind_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void io_control_mock_ignore_va_arg_in_at(unsigned int index)
{
    struct _nala_va_arg_item_t *item_p;
    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p != NULL) {
        item_p = _nala_va_arg_list_get(&instance_p->data.va_arg_list, index);
        item_p->ignore_in = true;
    } else {
        fprintf(stderr,
                "error: io_control_mock_ignore_va_arg_in_at(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size)
{
    struct _nala_va_arg_item_t *item_p;
    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p != NULL) {
        item_p = _nala_va_arg_list_get(&instance_p->data.va_arg_list, index);
        item_p->in.buf_p = _nala_malloc(size);
        item_p->in.size = size;
        memcpy(item_p->in.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: io_control_mock_set_va_arg_in_at(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p)
{
    (void)index;
    (void)buf_p;

    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p == NULL) {
        fprintf(stderr,
                "error: io_control_mock_set_va_arg_in_pointer_at(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size)
{
    struct _nala_va_arg_item_t *item_p;
    struct _nala_instance_type_for_io_control *instance_p;

    instance_p = _nala_state_for_io_control.instances.tail_p;

    if (instance_p != NULL) {
        item_p = _nala_va_arg_list_get(&instance_p->data.va_arg_list, index);
        item_p->out.buf_p = _nala_malloc(size);
        item_p->out.size = size;
        memcpy(item_p->out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: io_control_mock_set_va_arg_out_at(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void io_control_mock_none(void)
{
    _nala_state_for_io_control.mode = 4;
}

void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list))
{
    _nala_state_for_io_control.mode = 2;
    _nala_state_for_io_control.data.implementation = implementation;
}

void io_control_mock_disable(void)
{
    _nala_state_for_io_control.mode = 0;
}

void io_control_mock_reset(void)
{
    _nala_state_for_io_control.mode = 0;
    _nala_state_for_io_control.instances.head_p = NULL;
    _nala_state_for_io_control.instances.tail_p = NULL;
}

void io_control_mock_assert_completed(void)
{
    if (_nala_state_for_io_control.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "io_control() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION mount

int __real_mount(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const void *__data);

typedef struct _nala_data_type_for_mount _nala_data_type_for_mount;

struct _nala_data_type_for_mount
{
    const char *__special_file;
    const char *__dir;
    const char *__fstype;
    unsigned long int __rwflag;
    const void *__data;
    int return_value;
    bool ignore___special_file_in;
    _nala_set_param __special_file_in;
    _nala_set_param __special_file_out;
    bool ignore___dir_in;
    _nala_set_param __dir_in;
    _nala_set_param __dir_out;
    bool ignore___fstype_in;
    _nala_set_param __fstype_in;
    _nala_set_param __fstype_out;
    bool ignore___rwflag_in;
    bool ignore___data_in;
    _nala_set_param __data_in;
    _nala_set_param __data_out;
    int errno_value;
    int (*implementation)(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const void *__data);
};

typedef struct _nala_instance_type_for_mount _nala_instance_type_for_mount;

struct _nala_instance_type_for_mount
{
    _nala_data_type_for_mount data;
    _nala_instance_type_for_mount *next_p;
};

typedef struct _nala_instances_type_for_mount _nala_instances_type_for_mount;

struct _nala_instances_type_for_mount {
    _nala_instance_type_for_mount *head_p;
    _nala_instance_type_for_mount *tail_p;
};

typedef struct _nala_state_type_for_mount _nala_state_type_for_mount;

struct _nala_state_type_for_mount
{
    int mode;
    _nala_data_type_for_mount data;
    _nala_instances_type_for_mount instances;
};

static _nala_state_type_for_mount _nala_state_for_mount =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

int __wrap_mount(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const void *__data)
{
    struct _nala_instance_type_for_mount *instance_p;
    int return_value;

    switch (_nala_state_for_mount.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_mount.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "mount() called more times than expected.\n");
        }

        if (!instance_p->data.ignore___special_file_in) {
            ASSERT_EQ((const void *)__special_file,
                      (const void *)instance_p->data.__special_file);
        }

        if (!instance_p->data.ignore___dir_in) {
            ASSERT_EQ((const void *)__dir,
                      (const void *)instance_p->data.__dir);
        }

        if (!instance_p->data.ignore___fstype_in) {
            ASSERT_EQ((const void *)__fstype,
                      (const void *)instance_p->data.__fstype);
        }

        if (!instance_p->data.ignore___data_in) {
            ASSERT_EQ(__data, instance_p->data.__data);
        }

        if (!instance_p->data.ignore___rwflag_in) {
            ASSERT_EQ(__rwflag, instance_p->data.__rwflag);
        }

        if (instance_p->data.__special_file_out.buf_p != NULL) {
            memcpy((void *)__special_file,
                   instance_p->data.__special_file_out.buf_p,
                   instance_p->data.__special_file_out.size);
            free(instance_p->data.__special_file_out.buf_p);
        }

        if (instance_p->data.__special_file_in.buf_p != NULL) {
            ASSERT_MEMORY(__special_file,
                          instance_p->data.__special_file_in.buf_p,
                          instance_p->data.__special_file_in.size);
            free(instance_p->data.__special_file_in.buf_p);
        }

        if (instance_p->data.__dir_out.buf_p != NULL) {
            memcpy((void *)__dir,
                   instance_p->data.__dir_out.buf_p,
                   instance_p->data.__dir_out.size);
            free(instance_p->data.__dir_out.buf_p);
        }

        if (instance_p->data.__dir_in.buf_p != NULL) {
            ASSERT_MEMORY(__dir,
                          instance_p->data.__dir_in.buf_p,
                          instance_p->data.__dir_in.size);
            free(instance_p->data.__dir_in.buf_p);
        }

        if (instance_p->data.__fstype_out.buf_p != NULL) {
            memcpy((void *)__fstype,
                   instance_p->data.__fstype_out.buf_p,
                   instance_p->data.__fstype_out.size);
            free(instance_p->data.__fstype_out.buf_p);
        }

        if (instance_p->data.__fstype_in.buf_p != NULL) {
            ASSERT_MEMORY(__fstype,
                          instance_p->data.__fstype_in.buf_p,
                          instance_p->data.__fstype_in.size);
            free(instance_p->data.__fstype_in.buf_p);
        }

        if (instance_p->data.__data_out.buf_p != NULL) {
            memcpy((void *)__data,
                   instance_p->data.__data_out.buf_p,
                   instance_p->data.__data_out.size);
            free(instance_p->data.__data_out.buf_p);
        }

        if (instance_p->data.__data_in.buf_p != NULL) {
            ASSERT_MEMORY(__data,
                          instance_p->data.__data_in.buf_p,
                          instance_p->data.__data_in.size);
            free(instance_p->data.__data_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_mount.data.implementation(__special_file, __dir, __fstype, __rwflag, __data);
        break;

    case 3:
        return_value = _nala_state_for_mount.data.return_value;
        errno = _nala_state_for_mount.data.errno_value;
        break;

    case 4:
        FAIL();
        exit(1);
        break;

    default:
        return_value =
        __real_mount(__special_file, __dir, __fstype, __rwflag, __data);
        break;
    }

    return return_value;
}

void mount_mock(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, int return_value)
{
    _nala_state_for_mount.mode = 3;
    _nala_state_for_mount.data.__special_file = NULL;
    _nala_state_for_mount.data.ignore___special_file_in = true;

    if (__special_file != NULL) {
        _nala_state_for_mount.data.__special_file_in.size = (strlen(__special_file) + 1);
        _nala_state_for_mount.data.__special_file_in.buf_p = _nala_malloc(_nala_state_for_mount.data.__special_file_in.size);
        strcpy(_nala_state_for_mount.data.__special_file_in.buf_p, __special_file);
    } else {
        _nala_state_for_mount.data.ignore___special_file_in = false;
    }

    _nala_state_for_mount.data.__dir = NULL;
    _nala_state_for_mount.data.ignore___dir_in = true;

    if (__dir != NULL) {
        _nala_state_for_mount.data.__dir_in.size = (strlen(__dir) + 1);
        _nala_state_for_mount.data.__dir_in.buf_p = _nala_malloc(_nala_state_for_mount.data.__dir_in.size);
        strcpy(_nala_state_for_mount.data.__dir_in.buf_p, __dir);
    } else {
        _nala_state_for_mount.data.ignore___dir_in = false;
    }

    _nala_state_for_mount.data.__fstype = NULL;
    _nala_state_for_mount.data.ignore___fstype_in = true;

    if (__fstype != NULL) {
        _nala_state_for_mount.data.__fstype_in.size = (strlen(__fstype) + 1);
        _nala_state_for_mount.data.__fstype_in.buf_p = _nala_malloc(_nala_state_for_mount.data.__fstype_in.size);
        strcpy(_nala_state_for_mount.data.__fstype_in.buf_p, __fstype);
    } else {
        _nala_state_for_mount.data.ignore___fstype_in = false;
    }

    _nala_state_for_mount.data.__data = NULL;
    _nala_state_for_mount.data.ignore___data_in = true;
    _nala_state_for_mount.data.__rwflag = __rwflag;
    _nala_state_for_mount.data.ignore___rwflag_in = false;
    _nala_state_for_mount.data.return_value = return_value;
    _nala_state_for_mount.data.errno_value = 0;
}

void mount_mock_once(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, int return_value)
{
    struct _nala_instance_type_for_mount *instance_p;

    _nala_state_for_mount.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.__special_file_out.buf_p = NULL;
    instance_p->data.__special_file_out.size = 0;
    instance_p->data.__special_file_in.buf_p = NULL;
    instance_p->data.__special_file_in.size = 0;
    instance_p->data.__dir_out.buf_p = NULL;
    instance_p->data.__dir_out.size = 0;
    instance_p->data.__dir_in.buf_p = NULL;
    instance_p->data.__dir_in.size = 0;
    instance_p->data.__fstype_out.buf_p = NULL;
    instance_p->data.__fstype_out.size = 0;
    instance_p->data.__fstype_in.buf_p = NULL;
    instance_p->data.__fstype_in.size = 0;
    instance_p->data.__data_out.buf_p = NULL;
    instance_p->data.__data_out.size = 0;
    instance_p->data.__data_in.buf_p = NULL;
    instance_p->data.__data_in.size = 0;
    instance_p->data.__special_file = NULL;
    instance_p->data.ignore___special_file_in = true;

    if (__special_file != NULL) {
        instance_p->data.__special_file_in.size = (strlen(__special_file) + 1);
        instance_p->data.__special_file_in.buf_p = _nala_malloc(instance_p->data.__special_file_in.size);
        strcpy(instance_p->data.__special_file_in.buf_p, __special_file);
    } else {
        instance_p->data.ignore___special_file_in = false;
    }

    instance_p->data.__dir = NULL;
    instance_p->data.ignore___dir_in = true;

    if (__dir != NULL) {
        instance_p->data.__dir_in.size = (strlen(__dir) + 1);
        instance_p->data.__dir_in.buf_p = _nala_malloc(instance_p->data.__dir_in.size);
        strcpy(instance_p->data.__dir_in.buf_p, __dir);
    } else {
        instance_p->data.ignore___dir_in = false;
    }

    instance_p->data.__fstype = NULL;
    instance_p->data.ignore___fstype_in = true;

    if (__fstype != NULL) {
        instance_p->data.__fstype_in.size = (strlen(__fstype) + 1);
        instance_p->data.__fstype_in.buf_p = _nala_malloc(instance_p->data.__fstype_in.size);
        strcpy(instance_p->data.__fstype_in.buf_p, __fstype);
    } else {
        instance_p->data.ignore___fstype_in = false;
    }

    instance_p->data.__data = NULL;
    instance_p->data.ignore___data_in = true;
    instance_p->data.__rwflag = __rwflag;
    instance_p->data.ignore___rwflag_in = false;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_mount.instances,
                              instance_p);
}

void mount_mock_ignore_in(int return_value)
{
    _nala_state_for_mount.mode = 3;
    _nala_state_for_mount.data.ignore___special_file_in = true;
    _nala_state_for_mount.data.ignore___dir_in = true;
    _nala_state_for_mount.data.ignore___fstype_in = true;
    _nala_state_for_mount.data.ignore___data_in = true;
    _nala_state_for_mount.data.ignore___rwflag_in = true;
    _nala_state_for_mount.data.return_value = return_value;
    _nala_state_for_mount.data.errno_value = 0;
}

void mount_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_mount.data.errno_value = errno_value;
    }
}

void mount_mock_ignore___special_file_in(void)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___special_file_in = true;
    } else {
        fprintf(stderr,
                "error: mount_mock_ignore___special_file_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_ignore___dir_in(void)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___dir_in = true;
    } else {
        fprintf(stderr,
                "error: mount_mock_ignore___dir_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_ignore___fstype_in(void)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___fstype_in = true;
    } else {
        fprintf(stderr,
                "error: mount_mock_ignore___fstype_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_ignore___rwflag_in(void)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___rwflag_in = true;
    } else {
        fprintf(stderr,
                "error: mount_mock_ignore___rwflag_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___special_file_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__special_file_in.buf_p = _nala_malloc(size);
        instance_p->data.__special_file_in.size = size;
        memcpy(instance_p->data.__special_file_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___special_file_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___special_file_in_pointer(const char *__special_file)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___special_file_in = false;
        instance_p->data.__special_file = __special_file;
    } else {
        fprintf(stderr,
                "error: mount_mock_set___special_file_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___special_file_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__special_file_out.buf_p = _nala_malloc(size);
        instance_p->data.__special_file_out.size = size;
        memcpy(instance_p->data.__special_file_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___special_file_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___dir_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__dir_in.buf_p = _nala_malloc(size);
        instance_p->data.__dir_in.size = size;
        memcpy(instance_p->data.__dir_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___dir_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___dir_in_pointer(const char *__dir)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___dir_in = false;
        instance_p->data.__dir = __dir;
    } else {
        fprintf(stderr,
                "error: mount_mock_set___dir_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___dir_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__dir_out.buf_p = _nala_malloc(size);
        instance_p->data.__dir_out.size = size;
        memcpy(instance_p->data.__dir_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___dir_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___fstype_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__fstype_in.buf_p = _nala_malloc(size);
        instance_p->data.__fstype_in.size = size;
        memcpy(instance_p->data.__fstype_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___fstype_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___fstype_in_pointer(const char *__fstype)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___fstype_in = false;
        instance_p->data.__fstype = __fstype;
    } else {
        fprintf(stderr,
                "error: mount_mock_set___fstype_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___fstype_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__fstype_out.buf_p = _nala_malloc(size);
        instance_p->data.__fstype_out.size = size;
        memcpy(instance_p->data.__fstype_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___fstype_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___data_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__data_in.buf_p = _nala_malloc(size);
        instance_p->data.__data_in.size = size;
        memcpy(instance_p->data.__data_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___data_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___data_in_pointer(const void *__data)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___data_in = false;
        instance_p->data.__data = __data;
    } else {
        fprintf(stderr,
                "error: mount_mock_set___data_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_set___data_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_mount *instance_p;

    instance_p = _nala_state_for_mount.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__data_out.buf_p = _nala_malloc(size);
        instance_p->data.__data_out.size = size;
        memcpy(instance_p->data.__data_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: mount_mock_set___data_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void mount_mock_none(void)
{
    _nala_state_for_mount.mode = 4;
}

void mount_mock_implementation(int (*implementation)(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const void *__data))
{
    _nala_state_for_mount.mode = 2;
    _nala_state_for_mount.data.implementation = implementation;
}

void mount_mock_disable(void)
{
    _nala_state_for_mount.mode = 0;
}

void mount_mock_reset(void)
{
    _nala_state_for_mount.mode = 0;
    _nala_state_for_mount.instances.head_p = NULL;
    _nala_state_for_mount.instances.tail_p = NULL;
}

void mount_mock_assert_completed(void)
{
    if (_nala_state_for_mount.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "mount() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION pipe

int __real_pipe(int __pipedes[2]);

typedef struct _nala_data_type_for_pipe _nala_data_type_for_pipe;

struct _nala_data_type_for_pipe
{
    int *__pipedes;
    int return_value;
    bool ignore___pipedes_in;
    _nala_set_param __pipedes_in;
    _nala_set_param __pipedes_out;
    int errno_value;
    int (*implementation)(int __pipedes[2]);
};

typedef struct _nala_instance_type_for_pipe _nala_instance_type_for_pipe;

struct _nala_instance_type_for_pipe
{
    _nala_data_type_for_pipe data;
    _nala_instance_type_for_pipe *next_p;
};

typedef struct _nala_instances_type_for_pipe _nala_instances_type_for_pipe;

struct _nala_instances_type_for_pipe {
    _nala_instance_type_for_pipe *head_p;
    _nala_instance_type_for_pipe *tail_p;
};

typedef struct _nala_state_type_for_pipe _nala_state_type_for_pipe;

struct _nala_state_type_for_pipe
{
    int mode;
    _nala_data_type_for_pipe data;
    _nala_instances_type_for_pipe instances;
};

static _nala_state_type_for_pipe _nala_state_for_pipe =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

int __wrap_pipe(int __pipedes[2])
{
    struct _nala_instance_type_for_pipe *instance_p;
    int return_value;

    switch (_nala_state_for_pipe.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_pipe.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "pipe() called more times than expected.\n");
        }

        if (!instance_p->data.ignore___pipedes_in) {
            ASSERT_EQ(__pipedes, instance_p->data.__pipedes);
        }

        if (instance_p->data.__pipedes_out.buf_p != NULL) {
            memcpy((void *)__pipedes,
                   instance_p->data.__pipedes_out.buf_p,
                   instance_p->data.__pipedes_out.size);
            free(instance_p->data.__pipedes_out.buf_p);
        }

        if (instance_p->data.__pipedes_in.buf_p != NULL) {
            ASSERT_MEMORY(__pipedes,
                          instance_p->data.__pipedes_in.buf_p,
                          instance_p->data.__pipedes_in.size);
            free(instance_p->data.__pipedes_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_pipe.data.implementation(__pipedes);
        break;

    case 3:
        return_value = _nala_state_for_pipe.data.return_value;
        errno = _nala_state_for_pipe.data.errno_value;
        break;

    case 4:
        FAIL();
        exit(1);
        break;

    default:
        return_value =
        __real_pipe(__pipedes);
        break;
    }

    return return_value;
}

void pipe_mock(int return_value)
{
    _nala_state_for_pipe.mode = 3;
    _nala_state_for_pipe.data.__pipedes = NULL;
    _nala_state_for_pipe.data.ignore___pipedes_in = true;
    _nala_state_for_pipe.data.return_value = return_value;
    _nala_state_for_pipe.data.errno_value = 0;
}

void pipe_mock_once(int return_value)
{
    struct _nala_instance_type_for_pipe *instance_p;

    _nala_state_for_pipe.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.__pipedes_out.buf_p = NULL;
    instance_p->data.__pipedes_out.size = 0;
    instance_p->data.__pipedes_in.buf_p = NULL;
    instance_p->data.__pipedes_in.size = 0;
    instance_p->data.__pipedes = NULL;
    instance_p->data.ignore___pipedes_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_pipe.instances,
                              instance_p);
}

void pipe_mock_ignore_in(int return_value)
{
    _nala_state_for_pipe.mode = 3;
    _nala_state_for_pipe.data.ignore___pipedes_in = true;
    _nala_state_for_pipe.data.return_value = return_value;
    _nala_state_for_pipe.data.errno_value = 0;
}

void pipe_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_pipe *instance_p;

    instance_p = _nala_state_for_pipe.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_pipe.data.errno_value = errno_value;
    }
}

void pipe_mock_set___pipedes_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_pipe *instance_p;

    instance_p = _nala_state_for_pipe.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__pipedes_in.buf_p = _nala_malloc(size);
        instance_p->data.__pipedes_in.size = size;
        memcpy(instance_p->data.__pipedes_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: pipe_mock_set___pipedes_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void pipe_mock_set___pipedes_in_pointer(int __pipedes[2])
{
    struct _nala_instance_type_for_pipe *instance_p;

    instance_p = _nala_state_for_pipe.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___pipedes_in = false;
        instance_p->data.__pipedes = __pipedes;
    } else {
        fprintf(stderr,
                "error: pipe_mock_set___pipedes_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void pipe_mock_set___pipedes_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_pipe *instance_p;

    instance_p = _nala_state_for_pipe.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__pipedes_out.buf_p = _nala_malloc(size);
        instance_p->data.__pipedes_out.size = size;
        memcpy(instance_p->data.__pipedes_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: pipe_mock_set___pipedes_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void pipe_mock_none(void)
{
    _nala_state_for_pipe.mode = 4;
}

void pipe_mock_implementation(int (*implementation)(int __pipedes[2]))
{
    _nala_state_for_pipe.mode = 2;
    _nala_state_for_pipe.data.implementation = implementation;
}

void pipe_mock_disable(void)
{
    _nala_state_for_pipe.mode = 0;
}

void pipe_mock_reset(void)
{
    _nala_state_for_pipe.mode = 0;
    _nala_state_for_pipe.instances.head_p = NULL;
    _nala_state_for_pipe.instances.tail_p = NULL;
}

void pipe_mock_assert_completed(void)
{
    if (_nala_state_for_pipe.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "pipe() called fewer times than expected.\n");
    }
}

// NALA_IMPLEMENTATION time

time_t __real_time(time_t *__timer);

typedef struct _nala_data_type_for_time _nala_data_type_for_time;

struct _nala_data_type_for_time
{
    time_t *__timer;
    time_t return_value;
    bool ignore___timer_in;
    _nala_set_param __timer_in;
    _nala_set_param __timer_out;
    int errno_value;
    time_t (*implementation)(time_t *__timer);
};

typedef struct _nala_instance_type_for_time _nala_instance_type_for_time;

struct _nala_instance_type_for_time
{
    _nala_data_type_for_time data;
    _nala_instance_type_for_time *next_p;
};

typedef struct _nala_instances_type_for_time _nala_instances_type_for_time;

struct _nala_instances_type_for_time {
    _nala_instance_type_for_time *head_p;
    _nala_instance_type_for_time *tail_p;
};

typedef struct _nala_state_type_for_time _nala_state_type_for_time;

struct _nala_state_type_for_time
{
    int mode;
    _nala_data_type_for_time data;
    _nala_instances_type_for_time instances;
};

static _nala_state_type_for_time _nala_state_for_time =
{
    .mode = 0,
    .instances = {
        .head_p = NULL,
        .tail_p = NULL
    }
};

time_t __wrap_time(time_t *__timer)
{
    struct _nala_instance_type_for_time *instance_p;
    time_t return_value;

    switch (_nala_state_for_time.mode) {

    case 1:
        _NALA_INSTANCES_POP(_nala_state_for_time.instances, &instance_p);

        if (instance_p == NULL) {
            NALA_TEST_FAILURE(
                    "time() called more times than expected.\n");
        }

        if (!instance_p->data.ignore___timer_in) {
            ASSERT_EQ(__timer, instance_p->data.__timer);
        }

        if (instance_p->data.__timer_out.buf_p != NULL) {
            memcpy((void *)__timer,
                   instance_p->data.__timer_out.buf_p,
                   instance_p->data.__timer_out.size);
            free(instance_p->data.__timer_out.buf_p);
        }

        if (instance_p->data.__timer_in.buf_p != NULL) {
            ASSERT_MEMORY(__timer,
                          instance_p->data.__timer_in.buf_p,
                          instance_p->data.__timer_in.size);
            free(instance_p->data.__timer_in.buf_p);
        }

        errno = instance_p->data.errno_value;
        return_value = instance_p->data.return_value;
        free(instance_p);
        break;

    case 2:
        return_value =
        _nala_state_for_time.data.implementation(__timer);
        break;

    case 3:
        return_value = _nala_state_for_time.data.return_value;
        errno = _nala_state_for_time.data.errno_value;
        break;

    case 4:
        FAIL();
        exit(1);
        break;

    default:
        return_value =
        __real_time(__timer);
        break;
    }

    return return_value;
}

void time_mock(time_t return_value)
{
    _nala_state_for_time.mode = 3;
    _nala_state_for_time.data.__timer = NULL;
    _nala_state_for_time.data.ignore___timer_in = true;
    _nala_state_for_time.data.return_value = return_value;
    _nala_state_for_time.data.errno_value = 0;
}

void time_mock_once(time_t return_value)
{
    struct _nala_instance_type_for_time *instance_p;

    _nala_state_for_time.mode = 1;
    instance_p = _nala_malloc(sizeof(*instance_p));
    instance_p->data.__timer_out.buf_p = NULL;
    instance_p->data.__timer_out.size = 0;
    instance_p->data.__timer_in.buf_p = NULL;
    instance_p->data.__timer_in.size = 0;
    instance_p->data.__timer = NULL;
    instance_p->data.ignore___timer_in = true;
    instance_p->data.return_value = return_value;
    instance_p->data.errno_value = 0;
    instance_p->next_p = NULL;

    _NALA_INSTANCES_APPEND(_nala_state_for_time.instances,
                              instance_p);
}

void time_mock_ignore_in(time_t return_value)
{
    _nala_state_for_time.mode = 3;
    _nala_state_for_time.data.ignore___timer_in = true;
    _nala_state_for_time.data.return_value = return_value;
    _nala_state_for_time.data.errno_value = 0;
}

void time_mock_set_errno(int errno_value)
{
    struct _nala_instance_type_for_time *instance_p;

    instance_p = _nala_state_for_time.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.errno_value = errno_value;
    } else {
        _nala_state_for_time.data.errno_value = errno_value;
    }
}

void time_mock_set___timer_in(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_time *instance_p;

    instance_p = _nala_state_for_time.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__timer_in.buf_p = _nala_malloc(size);
        instance_p->data.__timer_in.size = size;
        memcpy(instance_p->data.__timer_in.buf_p,
               buf_p,
               size);
    } else {
        fprintf(stderr,
                "error: time_mock_set___timer_in(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void time_mock_set___timer_in_pointer(time_t *__timer)
{
    struct _nala_instance_type_for_time *instance_p;

    instance_p = _nala_state_for_time.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.ignore___timer_in = false;
        instance_p->data.__timer = __timer;
    } else {
        fprintf(stderr,
                "error: time_mock_set___timer_in_pointer(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void time_mock_set___timer_out(const void *buf_p, size_t size)
{
    struct _nala_instance_type_for_time *instance_p;

    instance_p = _nala_state_for_time.instances.tail_p;

    if (instance_p != NULL) {
        instance_p->data.__timer_out.buf_p = _nala_malloc(size);
        instance_p->data.__timer_out.size = size;
        memcpy(instance_p->data.__timer_out.buf_p, buf_p, size);
    } else {
        fprintf(stderr,
                "error: time_mock_set___timer_out(...) not\n"
                "       implemented for mock state.\n");
        FAIL();
    }
}

void time_mock_none(void)
{
    _nala_state_for_time.mode = 4;
}

void time_mock_implementation(time_t (*implementation)(time_t *__timer))
{
    _nala_state_for_time.mode = 2;
    _nala_state_for_time.data.implementation = implementation;
}

void time_mock_disable(void)
{
    _nala_state_for_time.mode = 0;
}

void time_mock_reset(void)
{
    _nala_state_for_time.mode = 0;
    _nala_state_for_time.instances.head_p = NULL;
    _nala_state_for_time.instances.tail_p = NULL;
}

void time_mock_assert_completed(void)
{
    if (_nala_state_for_time.instances.head_p != NULL) {
        NALA_TEST_FAILURE(
            "time() called fewer times than expected.\n");
    }
}
