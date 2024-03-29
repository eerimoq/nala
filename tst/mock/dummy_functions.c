#include "dummy_functions.h"

#include <stdio.h>

int add(int x, int y)
{
    return x + y;
}

void output_message(const char *message)
{
    printf("%s\n", message);
}

void keep_args_output_message(const char *message)
{
    output_message(message);
}

void print_hello()
{
    output_message("hello");
}

void print_world(void)
{
    output_message("world");
}

DummyStruct *edit_number(DummyStruct *dummy_struct, int number)
{
    dummy_struct->number = number;
    return dummy_struct;
}

DummyStruct *add_one(DummyStruct *dummy_struct)
{
    dummy_struct->number++;
    return dummy_struct;
}

DummyStruct *compose_twice(DummyStruct *dummy_struct,
                           DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct))
{
    return dummy_struct_modifier(dummy_struct_modifier(dummy_struct));
}

DummyStruct *add_two(DummyStruct *dummy_struct)
{
    return compose_twice(dummy_struct, add_one);
}

DummyStruct *(*return_add_one(void))(DummyStruct *dummy_struct)
{
    return add_one;
}

int io_control(int kind, ...)
{
    return (kind);
}

void io_control_return_void(int kind, ...)
{
    (void)kind;
}

int io_vcontrol(int kind, va_list ap)
{
    (void)ap;

    return (kind);
}

void struct_param(struct struct_param_type *data)
{
    (void)data;
}

void enum_param(enum enum_param_type value)
{
    (void)value;
}

void enum_param_typedef(enum_param_type_e value)
{
    (void)value;
}

int call(int (*callback)(int value))
{
    return (callback(1));
}

void in_out(int *buf_p)
{
    buf_p[1] = (2 * buf_p[0]);
}

struct struct_param_type struct_param_and_return_type(struct struct_param_type arg)
{
    arg.number++;

    return (arg);
}

void struct_param_typedef_pointer(struct_param_type_pointer arg)
{
    (void)arg;
}

union union_type union_param_and_return_type(union union_type arg)
{
    arg.a.number++;

    return (arg);
}

struct_param_type typedef_struct_param_and_return_type(struct_param_type arg)
{
    arg.number++;

    return (arg);
}

union_type typedef_union_param_and_return_type(union_type arg)
{
    arg.a.number++;

    return (arg);
}

void union_pointer(union union_type *arg)
{
    (void)arg;
}

void typedef_union_pointer(union_type_pointer arg)
{
    (void)arg;
}

int double_pointer(int **value_pp)
{
    **value_pp = 1;

    return (5);
}

void likely_undefined_padding(struct likely_undefined_padding_t *value_p)
{
    value_p->b++;
    value_p->d++;
}

int call_with_arg(call_with_arg_t func, void *arg_p)
{
    return (func(arg_p));
}

int struct_pointer_typedef(struct_pointer_typedef_t value)
{
    return (value->number);
}

int struct_pointer_typedef_2(struct_pointer_typedef_2_t value)
{
    return (value->number);
}

int struct_typedef(struct_typedef_t value)
{
    return (value.number);
}

int struct_typedef_2(struct_typedef_2_t value)
{
    return (value.number);
}

int parameter_name_omitted(int a, int *b)
{
    return (a + *b);
}

int underscore_prefixed_params(int __a, int __b)
{
    return (__a + __b);
}

int my_va_list(my_va_list_t ap)
{
    (void)ap;

    return (0);
}

int array_member(struct array_member_t *value_p)
{
    return (value_p->a[0] + value_p->a[1]);
}

void nested_types(struct nested_bar_t *bar_p)
{
    (void)bar_p;
}

void nested_types_2(nested_foo_2_t *foo_p)
{
    (void)foo_p;
}

void nested_types_pp(struct nested_bar_t **bar_pp)
{
    (void)bar_pp;
}

void nested_types_2_pp(nested_foo_2_t **foo_pp)
{
    (void)foo_pp;
}

void forward_declaration(struct apa_t *apa_p)
{
    (void)apa_p;
}

void forward_declaration_2(apa_2_t *apa_p)
{
    (void)apa_p;
}

void forward_declaration_3(banan_t *banan_p)
{
    (void)banan_p;
}

void struct_declared_in_typedef(struct banan_2_t *banan_p)
{
    (void)banan_p;
}

void my_fclose(my_file *file_p)
{
    (void)file_p;
}

void string_pointer(char **string_pp)
{
    (void)string_pp;
}

void bit_field(struct bit_field_t *value_p)
{
    (void)value_p;
}

void const_parameter(const int value)
{
    (void)value;
}

void const_typedef_parameter(cint value)
{
    (void)value;
}

void const_struct_pointer(const struct banan_2_t *banan_p)
{
    (void)banan_p;
}

void const_typedef_struct_pointer(const banan_3_t *banan_p)
{
    (void)banan_p;
}

void const_pointer_data(int * const value_p)
{
    (void)value_p;
}

void string_typedef(string_t string)
{
    (void)string;
}

void string_typedef_p(string_t *string_p)
{
    (void)string_p;
}

void char_typedef(char_t *string_p)
{
    (void)string_p;
}

void int_array(int_array_t value)
{
    (void)value;
}

void int_array_fixed(int_array_fixed_t value)
{
    (void)value;
}

void int8(int8_t *buf_p)
{
    (void)buf_p;
}

void uint8(uint8_t *buf_p)
{
    (void)buf_p;
}

void array_of_pointers(const char *argv[])
{
    (void)argv;
}
