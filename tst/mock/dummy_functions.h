#ifndef TEST_DUMMY_FUNCTIONS_H
#define TEST_DUMMY_FUNCTIONS_H

#include <stdarg.h>

typedef struct DummyStruct
{
    int number;
} DummyStruct;

struct struct_param_type
{
    int number;
};

enum enum_param_type
{
    enum_param_type_a = 0
};

typedef enum enum_param_type enum_param_type_e;

union union_type
{
    struct struct_param_type a;
};

typedef union union_type union_type;

typedef struct struct_param_type struct_param_type;

struct variadic_function_t {
    int a;
    int b;
};

struct likely_undefined_padding_t {
    char a;
    long b;
    char c;
    long d;
};

typedef int (*call_with_arg_t)(void *arg_p);

typedef struct DummyStruct * struct_pointer_typedef_t;

typedef struct_pointer_typedef_t struct_pointer_typedef_2_t;

typedef struct DummyStruct struct_typedef_t;

typedef struct_typedef_t struct_typedef_2_t;

int add(int x, int y);
void output_message(const char *message);
void keep_args_output_message(const char *message);
void print_hello();
void print_world(void);
DummyStruct *edit_number(DummyStruct *dummy_struct, int number);
DummyStruct *add_one(DummyStruct *dummy_struct);
DummyStruct *compose_twice(DummyStruct *dummy_struct,
                           DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
DummyStruct *add_two(DummyStruct *dummy_struct);
DummyStruct *(*return_add_one(void))(DummyStruct *dummy_struct);
int io_control(int kind, ...);
int io_vcontrol(int kind, va_list ap);
void struct_param(struct struct_param_type *data);
void enum_param(enum enum_param_type value);
void enum_param_typedef(enum_param_type_e value);
int call(int (*callback)(int value));
void in_out(int *buf_p);
struct struct_param_type struct_param_and_return_type(struct struct_param_type arg);
union union_type union_param_and_return_type(union union_type arg);
struct_param_type typedef_struct_param_and_return_type(struct_param_type arg);
union_type typedef_union_param_and_return_type(union_type arg);
int double_pointer(int **value_pp);
void likely_undefined_padding(struct likely_undefined_padding_t *value_p);
int call_with_arg(call_with_arg_t func, void *arg_p);
int struct_pointer_typedef(struct_pointer_typedef_t value);
int struct_pointer_typedef_2(struct_pointer_typedef_2_t value);
int struct_typedef(struct_typedef_t value);
int struct_typedef_2(struct_typedef_2_t value);
int parameter_name_omitted(int, int *);
int no_implementation(void);
int another_missing_implementation(void);

#endif
