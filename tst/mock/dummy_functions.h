#ifndef TEST_DUMMY_FUNCTIONS_H
#define TEST_DUMMY_FUNCTIONS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

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
    enum_param_type_a = 0,
    enum_param_type_b
};

typedef enum enum_param_type enum_param_type_e;

union union_type
{
    struct struct_param_type a;
};

typedef union union_type union_type;

typedef union union_type *union_type_pointer;

typedef struct struct_param_type struct_param_type;

typedef struct struct_param_type *struct_param_type_pointer;

struct variadic_function_t {
    int a;
    int b;
};

struct variadic_function_outer_t {
    int a;
    struct variadic_function_t *inner_p;
};

struct likely_undefined_padding_t {
    char a;
    long b;
    char c;
    long d;
};

struct array_member_t {
    int a[2];
};

struct unnamed_member_t {
    int a : 31;
    int : 1;
};

struct nested_foo_t {
    int a;
    int b;
};

typedef struct {
    int a;
    struct nested_foo_t b;
} nested_foo_2_t;

typedef nested_foo_2_t nested_foo_3_t;
typedef nested_foo_2_t *nested_foo_4_t;

struct nested_bar_t {
    struct nested_foo_t c;
    struct {
        int d;
        struct {
            struct nested_foo_t g;
            int h;
        } f;
    } e;
    union {
        int a;
    } i;
    nested_foo_2_t j;
    nested_foo_3_t k;
    nested_foo_4_t l;
    bool m;
    int *n_p;
    enum enum_param_type o;
    enum_param_type_e p;
    int32_t q;
    int (*fp)(void);
};

typedef int (*call_with_arg_t)(void *arg_p);

typedef struct DummyStruct * struct_pointer_typedef_t;

typedef struct_pointer_typedef_t struct_pointer_typedef_2_t;

typedef struct DummyStruct struct_typedef_t;

typedef struct_typedef_t struct_typedef_2_t;

typedef va_list my_va_list_t;

struct apa_t;

typedef struct apa_t apa_2_t;

typedef struct {
    int a;
    struct apa_t *apa_p;
} banan_t;

typedef struct banan_2_t {
    int a;
    struct banan_2_t *banan_p;
} banan_3_t;

struct my_io_file;

typedef struct my_io_file my_file;

struct my_file
{
  int flags;
};

struct bit_field_t {
    uint32_t a : 1;
    int b : 5;
};

typedef const int cint;

typedef char *string_t;

typedef char char_t;

typedef int int_array_t[];

typedef int int_array_fixed_t[3];

int add(int x, int y);
void output_message(const char *message);
void keep_args_output_message(const char *message);
void print_hello();
void print_world(void);
DummyStruct *edit_number(DummyStruct *dummy_struct, int number);
DummyStruct *add_one(DummyStruct *dummy_struct);
DummyStruct *compose_twice(
    DummyStruct *dummy_struct,
    DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
DummyStruct *add_two(DummyStruct *dummy_struct);
DummyStruct *(*return_add_one(void))(DummyStruct *dummy_struct);
int io_control(int kind, ...);
void io_control_return_void(int kind, ...);
int io_vcontrol(int kind, va_list ap);
int io_control_no_implementation(int kind, ...);
void struct_param(struct struct_param_type *data);
void enum_param(enum enum_param_type value);
void enum_param_typedef(enum_param_type_e value);
int call(int (*callback)(int value));
void in_out(int *buf_p);
struct struct_param_type struct_param_and_return_type(struct struct_param_type arg);
void struct_param_typedef_pointer(struct_param_type_pointer arg);
union union_type union_param_and_return_type(union union_type arg);
struct_param_type typedef_struct_param_and_return_type(struct_param_type arg);
union_type typedef_union_param_and_return_type(union_type arg);
void union_pointer(union union_type *arg);
void typedef_union_pointer(union_type_pointer arg);
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
int underscore_prefixed_params(int __a, int __b);
int my_va_list(my_va_list_t ap);
int array_member(struct array_member_t *value_p);
void nested_types(struct nested_bar_t *bar_p);
void nested_types_2(nested_foo_2_t *foo_p);
void nested_types_pp(struct nested_bar_t **bar_pp);
void nested_types_2_pp(nested_foo_2_t **foo_pp);
void primitive_type_char_pointer(char *value_p);
void primitive_type_signed_char_pointer(signed char *value_p);
void primitive_type_unsigned_char_pointer(unsigned char *value_p);
void primitive_type_short_pointer(short *value_p);
void primitive_type_short_int_pointer(short int *value_p);
void primitive_type_signed_short_pointer(signed short *value_p);
void primitive_type_signed_short_int_pointer(signed short int *value_p);
void primitive_type_unsigned_short_pointer(unsigned short *value_p);
void primitive_type_unsigned_short_int_pointer(unsigned short int *value_p);
void primitive_type_int_pointer(int *value_p);
void primitive_type_signed_pointer(signed *value_p);
void primitive_type_signed_int_pointer(signed int *value_p);
void primitive_type_unsigned_pointer(unsigned *value_p);
void primitive_type_unsigned_int_pointer(unsigned int *value_p);
void primitive_type_long_pointer(long *value_p);
void primitive_type_long_int_pointer(long int *value_p);
void primitive_type_signed_long_pointer(signed long *value_p);
void primitive_type_signed_long_int_pointer(signed long int *value_p);
void primitive_type_unsigned_long_pointer(unsigned long *value_p);
void primitive_type_unsigned_long_int_pointer(unsigned long int *value_p);
void primitive_type_long_long_pointer(long long *value_p);
void primitive_type_long_long_int_pointer(long long int *value_p);
void primitive_type_signed_long_long_pointer(signed long long *value_p);
void primitive_type_signed_long_long_int_pointer(signed long long int *value_p);
void primitive_type_unsigned_long_long_pointer(unsigned long long *value_p);
void primitive_type_unsigned_long_long_int_pointer(unsigned long long int *value_p);
void primitive_type_float_pointer(float *value_p);
void primitive_type_double_pointer(double *value_p);
void primitive_type_long_double_pointer(long double *value_p);
void forward_declaration(struct apa_t *apa_p);
void forward_declaration_2(apa_2_t *apa_p);
void forward_declaration_3(banan_t *banan_p);
void struct_declared_in_typedef(struct banan_2_t *banan_p);
void my_fclose(my_file *file_p);
void string_pointer(char **string_pp);
void bit_field(struct bit_field_t *value_p);
void const_parameter(const int value);
void const_typedef_parameter(cint value);
void const_struct_pointer(const struct banan_2_t *banan_p);
void const_typedef_struct_pointer(const banan_3_t *banan_p);
void const_pointer_data(int * const value_p);
void string_typedef(string_t string);
void string_typedef_p(string_t *string_p);
void char_typedef(char_t *string_p);
void int_array(int_array_t value);
void int_array_fixed(int_array_fixed_t value);

#endif
