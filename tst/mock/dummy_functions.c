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

void struct_param(struct struct_param_type *data)
{
    (void)data;
}

void enum_param(enum enum_param_type value)
{
    (void)value;
}
