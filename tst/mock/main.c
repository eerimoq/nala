#include <err.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>
#include <mntent.h>

#include "subprocess.h"
#include "nala.h"
#include "nala_mocks.h"
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
    return (x + y + 1);
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

    add_mock_ignore_in_once(41);
    ASSERT_EQ(add(5, -1), 41);
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

static int add_function_set_callback_callback_x;
static int add_function_set_callback_callback_y;

static void add_function_set_callback_callback(int x, int y)
{
    add_function_set_callback_callback_x = x;
    add_function_set_callback_callback_y = y;
}

TEST(add_function_set_callback)
{
    add_function_set_callback_callback_x = 0;
    add_function_set_callback_callback_y = 0;

    add_mock_once(10, 13, 23);
    add_mock_set_callback(add_function_set_callback_callback);

    ASSERT_EQ(add(10, 13), 23);

    ASSERT_EQ(add_function_set_callback_callback_x, 10);
    ASSERT_EQ(add_function_set_callback_callback_y, 13);
}

TEST(time_function)
{
    time_t start = time(NULL);

    time_mock_once(0);
    ASSERT_EQ(time(NULL), 0);

    time_mock_once(0);
    time_mock_set_tloc_in_pointer(NULL);
    ASSERT_EQ(time(NULL), 0);

    time_mock_disable();
    ASSERT_GE(time(NULL), start);
}

TEST(pipe_function_set_out)
{
    int fd[2] = { 1, 2 };
    int res;

    pipe_mock_once(42);
    pipe_mock_set_pipefd_out(fd, sizeof(fd));
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
    pipe_mock_set_pipefd_in(fd, sizeof(fd));

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
    mount_mock_set_data_in("", 1);

    ASSERT_EQ(mount("a", "b", "c", 0, ""), 0);

    mount_mock_once(NULL, "b", "c", 0, 0);
    mount_mock_set_data_in("", 1);

    ASSERT_EQ(mount(NULL, "b", "c", 0, ""), 0);
}

DummyStruct *fake_edit_number(DummyStruct *dummy_struct, int number)
{
    dummy_struct->number = 2 * number;

    return (dummy_struct + 1);
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

static void in_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock(5, &array[1]);
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array));
    edit_number(&array[0], 5);
}

TEST(in_error)
{
    function_error_in_subprocess(in_error_entry);
}

static void in_pointer_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock(5, &array[1]);
    edit_number_mock_set_dummy_struct_in_pointer(&array[1]);
    edit_number(&array[0], 5);
}

TEST(in_pointer_error)
{
    function_error_in_subprocess(in_pointer_error_entry);
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

static bool variadic_function_callback_called;

void variadic_function_callback(int kind, va_list vl)
{
    ASSERT_EQ(kind, 1);
    ASSERT_EQ(va_arg(vl, int), 10);
    variadic_function_callback_called = true;
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

    io_control_mock_once(1, 2, "%ld", 6l);
    ASSERT_EQ(io_control(1, 6l), 2);

    io_control_mock_once(2, 3, "%lu", 7ul);
    ASSERT_EQ(io_control(2, 7lu), 3);

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

    io_control_mock_once(1, 2, "%d", 10);
    io_control_mock_set_callback(variadic_function_callback);
    variadic_function_callback_called = false;
    ASSERT_EQ(io_control(1, 10), 2);
    ASSERT_EQ(variadic_function_callback_called, true);
}

static int va_list_ellipsis(int kind, ...)
{
    int res;
    va_list ap;

    va_start(ap, kind);
    res = io_vcontrol(kind, ap);
    va_end(ap);

    return (res);
}

TEST(va_list_function)
{
    io_vcontrol_mock_once(1, 3);
    ASSERT_EQ(va_list_ellipsis(1, 5), 3);
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

TEST(struct_param_function)
{
    struct_param_mock();
    struct_param(NULL);
}

TEST(enum_param_function)
{
    enum_param_mock(enum_param_type_a);
    enum_param(enum_param_type_a);
}

TEST(call_function)
{
    call_mock_once(4);
    ASSERT_EQ(call(NULL), 4);
}

static void buf_p_assert(int *actual_p, const void *buf_p, size_t size)
{
    const int *expected_p;

    expected_p = buf_p;

    ASSERT_EQ(actual_p[0], expected_p[0]);
}

TEST(in_out_function)
{
    /* First in, second out. */
    int values[2];

    in_out_mock_once();
    values[0] = 2;
    values[1] = 0;
    in_out_mock_set_buf_p_in(&values[0], sizeof(values));
    in_out_mock_set_buf_p_in_assert(buf_p_assert);
    values[0] = 2;
    values[1] = 1;
    in_out_mock_set_buf_p_out(&values[0], sizeof(values));
    values[0] = 2;
    values[1] = -1; /* Different from 0. values[1] not asserted. */
    in_out(&values[0]);
    ASSERT_EQ(values[1], 1)
}

TEST(struct_param_and_return_type_function)
{
    struct struct_param_type value;

    value.number = 1;
    value = struct_param_and_return_type(value);
    ASSERT_EQ(value.number, 2);

    value.number = 1;
    struct_param_and_return_type_mock_once(value);
    value = struct_param_and_return_type(value);
    ASSERT_EQ(value.number, 1);
}

TEST(union_param_and_return_type_function)
{
    union union_type value;

    value.a.number = 1;
    value = union_param_and_return_type(value);
    ASSERT_EQ(value.a.number, 2);

    value.a.number = 1;
    union_param_and_return_type_mock_once(value);
    value = union_param_and_return_type(value);
    ASSERT_EQ(value.a.number, 1);
}

TEST(typedef_struct_param_and_return_type_function)
{
    struct_param_type value;

    value.number = 1;
    value = typedef_struct_param_and_return_type(value);
    ASSERT_EQ(value.number, 2);

    value.number = 1;
    typedef_struct_param_and_return_type_mock_once(value);
    value = typedef_struct_param_and_return_type(value);
    ASSERT_EQ(value.number, 1);
}

TEST(typedef_union_param_and_return_type_function)
{
    union_type value;

    value.a.number = 1;
    value = typedef_union_param_and_return_type(value);
    ASSERT_EQ(value.a.number, 2);

    value.a.number = 1;
    typedef_union_param_and_return_type_mock_once(value);
    value = typedef_union_param_and_return_type(value);
    ASSERT_EQ(value.a.number, 1);
}

static void double_pointer_out_copy(int **value_pp, const void *buf_p, size_t size)
{
    **value_pp = *(const int *)buf_p;
}

TEST(double_pointer_function)
{
    int value;
    int *value_p;

    value = -1;
    value_p = &value;
    ASSERT_EQ(double_pointer(&value_p), 5);
    ASSERT_EQ(value, 1);

    value = -1;
    value_p = &value;
    double_pointer_mock_once(3);
    value = 2;
    double_pointer_mock_set_value_pp_out(&value, sizeof(value));
    double_pointer_mock_set_value_pp_out_copy(double_pointer_out_copy);
    value = -1;
    ASSERT_EQ(double_pointer(&value_p), 3);
    ASSERT_EQ(value, 2);
}

TEST(rename_parameters)
{
    close_mock_ignore_in(0);
    endmntent_mock_ignore_in(0);
    fclose_mock(0);
    fopen_mock_ignore_in(NULL);
    /* fread_mock(ptr, size, nmemb, stream); */
    fseek_mock_ignore_in(0);
    ftell_mock_ignore_in(0);
    fwrite_mock_ignore_in(0);
    getmntent_mock_ignore_in(NULL);
    mount_mock_ignore_in(0);
    /* nftw_mock(dirpath, fn, nopenfd, flags); */
    pipe_mock_ignore_in(0);
    poll_mock_ignore_in(0);
    read_mock_ignore_in(0);
    sendto_mock_ignore_in(0);
    setsockopt_mock_ignore_in(0);
    sleep_mock_ignore_in(0);
    statvfs_mock_ignore_in(0);
    time_mock_ignore_in(0);
    timerfd_settime_mock_ignore_in(0);
    usleep_mock_ignore_in(0);
    write_mock_ignore_in(0);
}
