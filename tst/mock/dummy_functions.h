#ifndef TEST_DUMMY_FUNCTIONS_H
#define TEST_DUMMY_FUNCTIONS_H

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

union union_type
{
    struct struct_param_type a;
};

typedef union union_type union_type;

typedef struct struct_param_type struct_param_type;

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
void struct_param(struct struct_param_type *data);
void enum_param(enum enum_param_type value);
int call(int (*callback)(int value));
void in_out(int *buf_p);
struct struct_param_type struct_param_and_return_type(struct struct_param_type arg);
union union_type union_param_and_return_type(union union_type arg);
struct_param_type typedef_struct_param_and_return_type(struct_param_type arg);
union_type typedef_union_param_and_return_type(union_type arg);

#endif
