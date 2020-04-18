#ifndef FOO_H
#define FOO_H

struct foo_t {
    int a;
    int b;
};

struct bar_t {
    struct foo_t c;
    struct {
        int d;
    } e;
};

int foo(struct foo_t *foo_p);

void bar(struct bar_t *bar_p);

#endif
