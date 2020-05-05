#define FOO()

/* error: macro "FOO" passed 1 arguments, but takes just 0. */
int FOO(1);
