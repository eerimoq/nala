#ifndef TEST_DUMMY_FUNCTIONS_H
#define TEST_DUMMY_FUNCTIONS_H

typedef struct DummyStruct
{
    int number;
} DummyStruct;

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

#endif
