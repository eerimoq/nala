#include <err.h>
#include <errno.h>
#include <sys/mount.h>
#include <time.h>
#include <unistd.h>

#include "subprocess.h"
#include "nala.h"
#include "__mocks__.h"
#include "dummy_functions.h"

static void function_error_in_subprocess(void (*entry)(void *arg_p))
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(entry, NULL);
    ASSERT_NE(result_p->exit_code, 0);
    subprocess_result_free(result_p);
}

int fake_add(int x, int y)
{
    return x + y + 1;
}

TEST(add_function)
{
    add_mock_once(1, 2, 42);
    ASSERT_EQ(add(1, 2), 42);

    add_mock_disable();
    ASSERT_EQ(add(1, 2), 3);

    add_mock_implementation(fake_add);
    ASSERT_EQ(add(1, 2), 4);

    add_mock_ignore_in(42);
    ASSERT_EQ(add(0, 0), 42);
    ASSERT_EQ(add(1, 0), 42);
    ASSERT_EQ(add(1, 1), 42);
}

static void add_function_error_wrong_x_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_once(1, 2, 42);
    add(3, 2);
}

TEST(add_function_error_wrong_x)
{
    function_error_in_subprocess(add_function_error_wrong_x_entry);
}

TEST(time_function)
{
    time_t start = time(NULL);

    time_mock_once(0);
    ASSERT_EQ(time(NULL), 0);

    time_mock_once(0);
    time_mock_set___timer_in_pointer(NULL);
    ASSERT_EQ(time(NULL), 0);

    time_mock_disable();
    ASSERT_GE(time(NULL), start);
}

TEST(pipe_function_set_out)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(42);
    pipe_mock_set___pipedes_out(fd, sizeof(fd));
    memset(&fd[0], 0, sizeof(fd));

    res = pipe(fd);

    ASSERT_EQ(res, 42);
    ASSERT_EQ(fd[0], 1);
    ASSERT_EQ(fd[1], 2);
}

TEST(pipe_function_set_in)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(41);
    pipe_mock_set___pipedes_in(fd, sizeof(fd));

    res = pipe(fd);

    ASSERT_EQ(res, 41);
    ASSERT_EQ(fd[0], 1);
    ASSERT_EQ(fd[1], 2);
}

TEST(pipe_function_set_errno)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(-1);
    pipe_mock_set_errno(ENOMEM);

    res = pipe(fd);

    ASSERT_EQ(res, -1);
    ASSERT_EQ(errno, ENOMEM);
}

TEST(mount_function)
{
    ASSERT_EQ(mount("a", "b", "c", 0, ""), -1);

    mount_mock_once("a", "b", "c", 0, 0);
    mount_mock_set___data_in("", 1);

    ASSERT_EQ(mount("a", "b", "c", 0, ""), 0);

    mount_mock_once(NULL, "b", "c", 0, 0);
    mount_mock_set___data_in("", 1);

    ASSERT_EQ(mount(NULL, "b", "c", 0, ""), 0);
}

DummyStruct *fake_edit_number(DummyStruct *dummy_struct, int number)
{
    dummy_struct->number = 2 * number;
    return dummy_struct + 1;
}

TEST(implementation)
{
    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };
    DummyStruct *dummy_ptr = edit_number(&array[0], 42);

    ASSERT_EQ(dummy_ptr, &array[0]);
    ASSERT_EQ(array[0].number, 42);

    edit_number_mock_implementation(fake_edit_number);

    dummy_ptr = edit_number(&array[0], -1);

    ASSERT_EQ(dummy_ptr, &array[1]);
    ASSERT_EQ(array[0].number, -2);
}

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

static void once_in_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array));
    edit_number(&array[0], 5);
}

TEST(once_in_error)
{
    function_error_in_subprocess(once_in_error_entry);
}

static void once_in_pointer_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock_once(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[1]);
    edit_number(&array[0], 5);
}

TEST(once_in_pointer_error)
{
    function_error_in_subprocess(once_in_pointer_error_entry);
}

TEST(set_errno)
{
    fopen_mock_once(__FILE__, "r", NULL);
    fopen_mock_set_errno(ENOENT);

    FILE *f = fopen(__FILE__, "r");

    ASSERT_EQ(f, NULL);
    ASSERT_EQ(errno, ENOENT);

    fopen_mock_disable();

    errno = 0;

    f = fopen(__FILE__, "r");
    ASSERT_NE(f, NULL);
    fclose(f);

    ASSERT_EQ(errno, 0);
}

TEST(variadic_function)
{
    int bar_1;
    int bar_2;

    io_control_mock_once(1, 0, "%d", 5);
    ASSERT_EQ(io_control(1, 5), 0);

    io_control_mock_once(2, 1, "%u", 5);
    ASSERT_EQ(io_control(2, 5), 1);

    io_control_mock_once(2, 1, "%u", 5);
    io_control_mock_ignore_va_arg_in_at(0);
    ASSERT_EQ(io_control(2, 6), 1);

    io_control_mock_once(3, 0, "%p");
    ASSERT_EQ(io_control(3, NULL), 0);

    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, NULL);
    ASSERT_EQ(io_control(3, NULL), 0);

    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, &bar_1);
    ASSERT_EQ(io_control(3, &bar_1), 0);

    bar_1 = 4;
    bar_2 = 4;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_at(0, &bar_1, sizeof(bar_1));
    ASSERT_EQ(io_control(3, &bar_2), 0);

    bar_1 = 3;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_out_at(0, &bar_1, sizeof(bar_1));
    bar_1 = 5;
    ASSERT_EQ(io_control(3, &bar_1), 0);
    ASSERT_EQ(bar_1, 3);

    io_control_mock_once(4, 1, "%d%u%p", 1, 2);
    ASSERT_EQ(io_control(4, 1, 2, NULL), 1);

    io_control_mock_once(5, 0, "");
    ASSERT_EQ(io_control(5), 0);
}

int io_control_mock_va_arg_real(int kind, va_list __nala_va_list)
{
    int res;

    switch (kind) {

    case 1:
        res = va_arg(__nala_va_list, int);
        res += 100;
        break;

    case 2:
        res = (int)va_arg(__nala_va_list, unsigned int);
        res += 1000;
        break;

    default:
        res = -1;
        break;
    }

    return (res);
}

TEST(variadic_function_real_1)
{
    ASSERT_EQ(io_control(1, 5), 105);
}

TEST(variadic_function_real_2)
{
    ASSERT_EQ(io_control(2, 5), 1005);
}

static void variadic_function_error_integer_entry(void *arg_p)
{
    (void)arg_p;

    io_control_mock_once(1, 0, "%d", 5);
    io_control(1, 6);
}

TEST(variadic_function_error_integer)
{
    function_error_in_subprocess(variadic_function_error_integer_entry);
}

static void variadic_function_error_va_arg_in_entry(void *arg_p)
{
    (void)arg_p;

    int bar_1;
    int bar_2;

    bar_1 = 3;
    bar_2 = 4;
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_at(0, &bar_1, sizeof(bar_1));
    io_control(3, &bar_2);
}

TEST(variadic_function_error_va_arg_in)
{
    function_error_in_subprocess(variadic_function_error_va_arg_in_entry);
}

TEST(compose_twice_function)
{
    DummyStruct dummy_struct = { .number = 1 };
    DummyStruct *dummy_ptr = compose_twice(&dummy_struct, add_one);

    ASSERT_EQ(dummy_struct.number, 3);

    compose_twice_mock_once(&dummy_struct);

    ASSERT_EQ(compose_twice(dummy_ptr, add_one), dummy_ptr);
    ASSERT_EQ(dummy_struct.number, 3);
}
