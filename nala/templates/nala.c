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
