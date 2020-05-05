#include "nala.h"
#include "dummy_functions.h"

TEST(once_in_and_in_pointer)
{
    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };
    DummyStruct *dummy_ptr = edit_number(&array[0], 42);

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[0]);
    edit_number_mock_set_dummy_struct_in(&array[0], sizeof(array));

    dummy_ptr = edit_number(&array[0], 5);

    ASSERT_EQ(dummy_ptr, &array[1]);
}
