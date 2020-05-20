#ifndef FOO_H
#define FOO_H

typedef void (*callback_t)(void);

struct foo_t {
    char *string_p;
};

int foo(int value);

void bar(callback_t callback);

void fum(int *value_p, struct foo_t *foo_p);

#endif
