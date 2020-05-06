#ifndef FOO_H
#define FOO_H

typedef void (*callback_t)(void);

int foo(int value);

void bar(callback_t callback);

void fum(int *value_p);

#endif
