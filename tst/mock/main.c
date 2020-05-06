#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>
#include <mntent.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <syslog.h>

#include "subprocess.h"
#include "nala.h"
/* Must be included before dummy_functions.h to find the foo-struct
   before all mocked functions. */
#include "dummy_functions_not_mocked.h"
#include "dummy_functions.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#define B(...) ANSI_RESET ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define C(color, ...) ANSI_RESET ANSI_COLOR_##color __VA_ARGS__ ANSI_RESET
#define CR(...) C(RED, __VA_ARGS__)
#define CM(...) C(MAGENTA, __VA_ARGS__)

#define CB(color, ...)                                                  \
    ANSI_RESET ANSI_COLOR_##color ANSI_BOLD __VA_ARGS__ ANSI_RESET
#define CBR(...) CB(RED, __VA_ARGS__)

static void function_error_in_subprocess(void (*entry)(void *arg_p),
                                         const char *expected_p)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(entry, NULL);
    ASSERT_NE(result_p->exit_code, 0);

    if (expected_p != NULL ) {
        ASSERT_SUBSTRING(result_p->stdout.buf_p, expected_p);
    }

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

    add_mock_real();
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

static void add_function_error_wrong_x_two_tracebacks_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_once(1, 2, 42);
    add(3, 2);
}

TEST(add_function_error_wrong_x_two_tracebacks)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(
        add_function_error_wrong_x_two_tracebacks_entry,
        NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked add(x): 3 is not equal to 1 (0x3, 0x1)");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mock traceback (most recent call first):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Assert traceback (most recent call first):");

    subprocess_result_free(result_p);
}

static void add_function_error_wrong_result_one_traceback_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_once(3, 2, 42);
    ASSERT_EQ(add(3, 2), 41);
}

TEST(add_function_error_wrong_result_one_traceback)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(
        add_function_error_wrong_result_one_traceback_entry,
        NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "42 is not equal to 41 (0x2a, 0x29)");
    ASSERT_NOT_SUBSTRING(result_p->stdout.buf_p,
                     "Mock traceback (most recent call first):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Assert traceback (most recent call first):");

    subprocess_result_free(result_p);
}

static void add_function_error_mock_with_mock_once_enqueued_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_once(1, 2, 42);
    add_mock(1, 2, 42);
}

TEST(add_function_error_mock_with_mock_once_enqueued)
{
    function_error_in_subprocess(
        add_function_error_mock_with_mock_once_enqueued_entry,
        "Cannot change mock mode with mock instances enqueued.");
}

static void add_function_error_none_entry(void *arg_p)
{
    (void)arg_p;

    add_mock_none();
    add(3, 2);
}

TEST(add_function_none)
{
    function_error_in_subprocess(add_function_error_none_entry,
                                 "Mocked add() called unexpectedly.");
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

TEST(output_message_function)
{
    output_message_mock_once("a");
    output_message("a");
}

static void output_message_function_error_mock_null_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once(NULL);
    output_message("a");
}

TEST(output_message_function_error_mock_null)
{
    function_error_in_subprocess(
        output_message_function_error_mock_null_entry,
        "Mocked output_message(message): 0x");
}

TEST(output_message_function_ignore_in_set_out)
{
    char buf[16];

    output_message_mock_once(NULL);
    output_message_mock_ignore_message_in();
    output_message_mock_set_message_out("hello", 6);
    buf[0] = '\0';
    output_message(&buf[0]);
    ASSERT_EQ(&buf[0], "hello");
}

TEST(output_message_function_check_pointers)
{
    char buf[16];

    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in_pointer(&buf[0]);
    output_message(&buf[0]);

    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in_pointer(NULL);
    output_message(NULL);
}

TEST(output_message_function_check_non_string)
{
    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in("\x00\x01\x02", 3);
    /* Only first three should be compared, make sure 4th is not
       zero. */
    output_message("\x00\x01\x02\x03");
}

static void output_message_function_check_non_string_error_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in("\x31\x31\x31", 3);
    /* Only first three should be compared, make sure 4th is not
       zero. */
    output_message("\x30\x31\x32\x33");
}

TEST(output_message_function_check_non_string_error)
{
    function_error_in_subprocess(
        output_message_function_check_non_string_error_entry,
        "Memory mismatch. See diff for details.");
}

TEST(output_message_function_check_non_string_2)
{
    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in("\x00\x01\x02\x03", 3);
    /* Only first three should be compared, make sure 4th is not
       zero. */
    output_message("\x00\x01\x02\x04");
}

TEST(output_message_function_check_non_string_string)
{
    output_message_mock_once("");
    output_message_mock_set_message_in("\x00\x01\x02", 3);
    /* Only first three should be compared, make sure 4th is not
       zero. */
    output_message("\x00\x01\x02\x03");
}

static void output_message_function_check_non_string_string_error_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("");
    output_message_mock_set_message_in("\x31\x31\x31", 3);
    /* Only first three should be compared, make sure 4th is not
       zero. */
    output_message("\x30\x31\x32\x33");
}

TEST(output_message_function_check_non_string_string_error)
{
    function_error_in_subprocess(
        output_message_function_check_non_string_string_error_entry,
        "Memory mismatch. See diff for details.");
}

static void output_message_function_error_check_pointers_entry(void *arg_p)
{
    (void)arg_p;

    char buf[16];

    output_message_mock_ignore_in_once();
    output_message_mock_set_message_in_pointer(&buf[0] + 1);
    output_message(&buf[0]);
}

TEST(output_message_function_error_check_pointers)
{
    function_error_in_subprocess(
        output_message_function_error_check_pointers_entry,
        "Mocked output_message(message): 0x");
}

TEST(output_message_function_ignore_message)
{
    /* ignore_message_in() should ignore both NULL and "". */
    output_message_mock_once(NULL);
    output_message_mock_ignore_message_in();
    output_message("bar");

    output_message_mock_once("foo");
    output_message_mock_ignore_message_in();
    output_message("bar");
}

static void output_message_function_error_call_null_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("a");
    output_message(NULL);
}

TEST(output_message_function_error_call_null)
{
    function_error_in_subprocess(
        output_message_function_error_call_null_entry,
        "Actual memory pointer is NULL.");
}

static void output_message_function_error_mismatch_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("a");
    output_message("b");
}

TEST(output_message_function_error_mismatch)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(output_message_function_error_mismatch_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "The strings are not equal. See diff for details.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, B("a"));
    ASSERT_SUBSTRING(result_p->stdout.buf_p, CBR("b"));

    subprocess_result_free(result_p);
}

static void output_message_function_error_mismatch_2_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("abc");
    output_message("b");
}

TEST(output_message_function_error_mismatch_2)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(output_message_function_error_mismatch_2_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "The strings are not equal. See diff for details.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, B("a")"b"B("c")"\n");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "b\n");

    subprocess_result_free(result_p);
}

static void output_message_function_error_mismatch_3_entry(void *arg_p)
{
    (void)arg_p;

    output_message_mock_once("a");
    output_message("abc");
}

TEST(output_message_function_error_mismatch_3)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(output_message_function_error_mismatch_3_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);

    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "The strings are not equal. See diff for details.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "a\n");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "a"CBR("b ...")"\n");

    subprocess_result_free(result_p);
}

TEST(time_function)
{
    time_t start = time(NULL);

    time_mock_once(0);
    ASSERT_EQ(time(NULL), 0);

    time_mock_once(0);
    time_mock_set_tloc_in_pointer(NULL);
    ASSERT_EQ(time(NULL), 0);

    time_mock_real();
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
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array[0]));
    edit_number(&array[0], 5);
}

TEST(once_in_error)
{
    function_error_in_subprocess(once_in_error_entry, "1 is not equal to 2 (0x1, 0x2)");
}

static void in_error_entry(void *arg_p)
{
    (void)arg_p;

    DummyStruct array[] = { { .number = 1 }, { .number = 2 } };

    edit_number_mock(5, &array[1]);
    edit_number_mock_set_dummy_struct_in(&array[1], sizeof(array[0]));
    edit_number(&array[0], 5);
}

TEST(in_error)
{
    function_error_in_subprocess(in_error_entry, "1 is not equal to 2 (0x1, 0x2)");
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
    function_error_in_subprocess(in_pointer_error_entry,
                                 "Mocked edit_number(dummy_struct): ");
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
    function_error_in_subprocess(once_in_pointer_error_entry,
                                 "Mocked edit_number(dummy_struct): ");
}

TEST(set_errno)
{
    fopen_mock_once(__FILE__, "r", NULL);
    fopen_mock_set_errno(ENOENT);

    FILE *f = fopen(__FILE__, "r");

    ASSERT_EQ(f, NULL);
    ASSERT_EQ(errno, ENOENT);

    fopen_mock_real();

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
    char string[8];

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

    /* The mock should make a copy of foo. */
    strcpy(&string[0], "foo");
    io_control_mock_once(1, 6, "%s", &string[0]);
    strcpy(&string[0], "bar");
    ASSERT_EQ(io_control(1, "foo"), 6);

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

static void variadic_function_in_assert(const void *actual_p,
                                        const void *expected_p,
                                        size_t size)
{
    const struct variadic_function_outer_t *actual_outer_p;
    const struct variadic_function_t *actual_inner_p;
    const struct variadic_function_outer_t *expected_outer_p;
    const struct variadic_function_t *expected_inner_p;

    ASSERT_EQ(size, sizeof(struct variadic_function_outer_t));

    actual_outer_p = actual_p;
    actual_inner_p = actual_outer_p->inner_p;
    expected_outer_p = expected_p;
    expected_inner_p = expected_outer_p->inner_p;

    ASSERT_EQ(actual_outer_p->a, expected_outer_p->a);
    ASSERT_EQ(actual_inner_p->a, expected_inner_p->a);
}

static void variadic_function_out_copy(void *dst_p,
                                       const void *src_p,
                                       size_t size)
{
    struct variadic_function_t *dst_inner_p;
    struct variadic_function_t *src_inner_p;

    ASSERT_EQ(size, sizeof(struct variadic_function_outer_t));

    dst_inner_p = ((struct variadic_function_outer_t *)dst_p)->inner_p;
    src_inner_p = ((struct variadic_function_outer_t *)src_p)->inner_p;

    dst_inner_p->b = src_inner_p->b;
}

TEST(variadic_function_in_assert_copy_out)
{
    struct variadic_function_outer_t outer;
    struct variadic_function_t inner_in;
    struct variadic_function_t inner_out;

    outer.a = 1;
    io_control_mock_once(1, 10, "%p");
    outer.inner_p = &inner_in;
    inner_in.a = 2;
    inner_in.b = 5;
    io_control_mock_set_va_arg_in_at(0, &outer, sizeof(outer));
    io_control_mock_set_va_arg_in_assert_at(0, variadic_function_in_assert);
    outer.inner_p = &inner_out;
    inner_out.a = 2;
    inner_out.b = 6;
    io_control_mock_set_va_arg_out_at(0, &outer, sizeof(outer));
    io_control_mock_set_va_arg_out_copy_at(0, variadic_function_out_copy);

    outer.inner_p = &inner_in;
    ASSERT_EQ(io_control(1, &outer), 10);

    ASSERT_EQ(inner_in.a, 2);
    ASSERT_EQ(inner_in.b, 6);
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
    function_error_in_subprocess(
        variadic_function_error_integer_entry,
        "Mocked io_control(...): 6 is not equal to 5 (0x6, 0x5)");
}

static void variadic_function_error_string_entry(void *arg_p)
{
    (void)arg_p;

    io_control_mock_once(1, 0, "%s", "kalle");
    io_control(1, "billy");
}

TEST(variadic_function_error_string)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(variadic_function_error_string_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked io_control(...): See diff for details.");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, B("ka")"ll"B("e"));
    ASSERT_SUBSTRING(result_p->stdout.buf_p, CBR("bi")"ll"CBR("y"));

    subprocess_result_free(result_p);
}

TEST(variadic_function_string_out)
{
    char name[16];

    io_control_mock_once(1, 0, "%s", NULL);
    io_control_mock_ignore_va_arg_in_at(0);
    io_control_mock_set_va_arg_out_at(0, "fia", 4);

    ASSERT_EQ(io_control(1, &name[0]), 0);
    ASSERT_EQ(name, "fia");
}

TEST(variadic_function_ignore_in)
{
    io_control_mock_ignore_in(0, "%s");

    ASSERT_EQ(io_control(1, NULL), 0);
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
    function_error_in_subprocess(variadic_function_error_va_arg_in_entry, NULL);
}

static void variadic_function_error_bad_format_string_entry(void *arg_p)
{
    (void)arg_p;

    io_control_mock_once(3, 0, "foo");
}

TEST(variadic_function_error_bad_format_string)
{
    function_error_in_subprocess(variadic_function_error_bad_format_string_entry,
                                 "Bad format string 'foo'.");
}

static void variadic_function_pointer_error_wrong_call_order_entry(void *arg_p)
{
    (void)arg_p;
    int a;
    int b;

    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, &a);
    io_control_mock_once(3, 0, "%p");
    io_control_mock_set_va_arg_in_pointer_at(0, &b);
    io_control(3, &b);
    io_control(3, &a);
}

TEST(io_control_no_implementation_function)
{
    io_control_no_implementation_mock_ignore_in_once(2, "");
    ASSERT_EQ(io_control_no_implementation(1), 2);
}

TEST(variadic_function_pointer_error_wrong_call_order)
{
    function_error_in_subprocess(
        variadic_function_pointer_error_wrong_call_order_entry,
        " is not equal to 0x");
}

static void variadic_function_error_set_in_for_integer_entry(void *arg_p)
{
    (void)arg_p;

    int value;

    io_control_mock_once(1, 0, "%p%p%d", 5);
    value = 0;
    io_control_mock_set_va_arg_in_at(0, &value, sizeof(value));
    io_control_mock_set_va_arg_in_at(1, &value, sizeof(value));
    io_control_mock_set_va_arg_in_at(2, &value, sizeof(value));
}

TEST(variadic_function_error_set_in_for_integer)
{
    function_error_in_subprocess(
        variadic_function_error_set_in_for_integer_entry,
        "Cannot set input for '%d' at index 2. Only '%p' can be set.");
}

static void variadic_function_error_set_out_for_unsigned_long_entry(void *arg_p)
{
    (void)arg_p;

    unsigned long value;

    io_control_mock_once(1, 0, "%p%p%lu%u", 5lu, 3);
    value = 0;
    io_control_mock_set_va_arg_out_at(0, &value, sizeof(value));
    io_control_mock_set_va_arg_out_at(1, &value, sizeof(value));
    io_control_mock_set_va_arg_out_at(2, &value, sizeof(value));
}

TEST(variadic_function_error_set_out_for_unsigned_long)
{
    function_error_in_subprocess(
        variadic_function_error_set_out_for_unsigned_long_entry,
        "Cannot set output for '%lu' at index 2. Only '%p' and '%s' can be set.");
}

static void variadic_function_error_format_null_entry(void *arg_p)
{
    (void)arg_p;

    io_control_mock_once(1, 0, NULL);
}

TEST(variadic_function_error_format_null)
{
    function_error_in_subprocess(
        variadic_function_error_format_null_entry,
        "Mocked variadic function format must be a string, not NULL.");
}

TEST(variadic_function_return_void)
{
    io_control_return_void_mock_ignore_in("");
    io_control_return_void(1);
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

TEST(enum_param_typedef_function)
{
    enum_param_typedef_mock(enum_param_type_a);
    enum_param_typedef(enum_param_type_a);
}

TEST(call_function)
{
    call_mock_once(4);
    ASSERT_EQ(call(NULL), 4);
}

static void buf_p_assert(int *actual_p, int *expected_p, size_t size)
{
    (void)size;

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
    ASSERT_EQ(values[1], 1);
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

static void double_pointer_out_copy(int **dst_pp, int **src_pp, size_t size)
{
    (void)size;

    **dst_pp = **src_pp;
}

TEST(double_pointer_function)
{
    int value_1;
    int value_2;
    int *value_p;

    value_1 = -1;
    value_p = &value_1;
    ASSERT_EQ(double_pointer(&value_p), 5);
    ASSERT_EQ(value_1, 1);

    value_1 = -1;
    value_p = &value_1;
    double_pointer_mock_once(3);
    value_1 = 2;
    double_pointer_mock_set_value_pp_out(&value_p, sizeof(value_p));
    double_pointer_mock_set_value_pp_out_copy(double_pointer_out_copy);
    value_2 = -1;
    value_p = &value_2;
    ASSERT_EQ(double_pointer(&value_p), 3);
    ASSERT_EQ(value_2, 2);
}

static void double_pointer_function_out_copy_before_out_error_entry(void *arg_p)
{
    (void)arg_p;

    double_pointer_mock_once(3);
    /* double_pointer_mock_set_value_pp_out() not called. */
    double_pointer_mock_set_value_pp_out_copy(double_pointer_out_copy);
}

TEST(double_pointer_function_out_copy_before_out_error)
{
    function_error_in_subprocess(
        double_pointer_function_out_copy_before_out_error_entry,
        "double_pointer_mock_set_value_pp_out() must be called before "
        "double_pointer_mock_set_value_pp_out_copy().");
}

TEST(rename_parameters)
{
    clock_gettime_mock_ignore_in(0);
    clock_settime_mock_ignore_in(0);
    close_mock_ignore_in(0);
    connect_mock_ignore_in(0);
    endmntent_mock_ignore_in(0);
    epoll_create_mock_ignore_in(0);
    epoll_ctl_mock_ignore_in(0);
    epoll_wait_mock_ignore_in(0);
    fclose_mock(0);
    fgets_mock_ignore_in(NULL);
    fopen_mock_ignore_in(NULL);
    /* fread_mock(ptr, size, nmemb, stream); */
    freeaddrinfo_mock_ignore_in();
    fseek_mock_ignore_in(0);
    ftell_mock_ignore_in(0);
    fwrite_mock_ignore_in(0);
    getaddrinfo_mock_ignore_in(0);
    getsockopt_mock_ignore_in(0);
    getmntent_mock_ignore_in(NULL);
    gettimeofday_mock_ignore_in(0);
    ioctl_mock_ignore_in(0, "");
    mount_mock_ignore_in(0);
    /* nftw_mock(dirpath, fn, nopenfd, flags); */
    open_mock_ignore_in(0, "");
    pipe_mock_ignore_in(0);
    poll_mock_ignore_in(0);
    read_mock_ignore_in(0);
    sendto_mock_ignore_in(0);
    setsockopt_mock_ignore_in(0);
    settimeofday_mock_ignore_in(0);
    socket_mock_ignore_in(0);
    sleep_mock_ignore_in(0);
    statvfs_mock_ignore_in(0);
    time_mock_ignore_in(0);
    timerfd_settime_mock_ignore_in(0);
    usleep_mock_ignore_in(0);
    write_mock_ignore_in(0);
}

TEST(print_hello_function)
{
    print_hello_mock_once();
    print_hello();
}

TEST(capture_output_suspend_resume)
{
    fflush_mock_once(0);
    dup_mock_once(1, 2);
    dup2_mock_once(3, 4, 5);
    tmpfile_mock_once(NULL);
    close_mock_once(1, 9);
    fileno_mock_once(5);
    ftell_mock_once(6);
    fseek_mock_once(3, 1, 5);
    fread_mock_once(7, 6, 5);
    fclose_mock_once(8);

    CAPTURE_OUTPUT(output, errput) {
        printf("Stdout!\n");
        fprintf(stderr, "Stderr!\n");
    }

    ASSERT_EQ(output, "Stdout!\n");
    ASSERT_EQ(errput, "Stderr!\n");

    ASSERT_EQ(fflush(NULL), 0);
    ASSERT_EQ(dup(1), 2);
    ASSERT_EQ(dup2(3, 4), 5);
    ASSERT_EQ(tmpfile(), NULL);
    ASSERT_EQ(close(1), 9);
    ASSERT_EQ(fileno(NULL), 5);
    ASSERT_EQ(ftell(NULL), 6);
    ASSERT_EQ(fseek(NULL, 3, 1), 5);
    ASSERT_EQ(fread(NULL, 7, 6, NULL), 5u);
    ASSERT_EQ(fclose(NULL), 8);
}

TEST(malloc_free)
{
    int a;
    int *a_p;

    malloc_mock_once(1, &a);
    free_mock_once();
    free_mock_set_ptr_in_pointer(&a);
    a_p = malloc(1);
    ASSERT_EQ(a_p, &a);
    free(a_p);
}

TEST(poll_real_once)
{
    struct pollfd fds[1];

    fds[0].revents = POLLHUP;

    poll_mock_once(1, -1, 1);
    poll_mock_set_fds_out(&fds[0], sizeof(fds));

    poll_mock_real_once();

    poll_mock_once(1, -1, 1);
    poll_mock_set_fds_out(&fds[0], sizeof(fds));

    /* Mocked call. */
    fds[0].fd = 100;
    fds[0].events = 0;
    fds[0].revents = 0;
    ASSERT_EQ(poll(&fds[0], 1, -1), 1);
    ASSERT_EQ(fds[0].revents, POLLHUP);

    /* Real call. */
    fds[0].fd = 100;
    fds[0].events = 0;
    fds[0].revents = 0;
    ASSERT_EQ(poll(&fds[0], 1, -1), 1);
    ASSERT_EQ(fds[0].revents, POLLNVAL);

    /* Mocked call. */
    fds[0].fd = 100;
    fds[0].events = 0;
    fds[0].revents = 0;
    ASSERT_EQ(poll(&fds[0], 1, -1), 1);
    ASSERT_EQ(fds[0].revents, POLLHUP);
}

TEST(assert_structs_with_likely_undefined_padding)
{
    struct likely_undefined_padding_t value;

    /* Padding -1. */
    memset(&value, -1, sizeof(value));
    value.a = 4;
    value.b = 3;
    value.c = 2;
    value.d = 1;
    likely_undefined_padding_mock_once();
    likely_undefined_padding_mock_set_value_p_in(&value, sizeof(value));

    value.a = 0;
    value.b = 2;
    value.c = 0;
    value.d = 2;
    likely_undefined_padding_mock_set_value_p_out(&value, sizeof(value));

    /* Padding 0 (different from -1). */
    memset(&value, 0, sizeof(value));
    value.a = 4;
    value.b = 3;
    value.c = 2;
    value.d = 1;
    likely_undefined_padding(&value);
    ASSERT_EQ(value.b, 2);
    ASSERT_EQ(value.d, 2);
}

static void fail_if_in_assert_is_called_before_set_in_in_assert(
    struct likely_undefined_padding_t *actual_p,
    struct likely_undefined_padding_t *expected_p,
    size_t size)
{
    (void)actual_p;
    (void)expected_p;
    (void)size;
}

static void fail_if_in_assert_is_called_before_set_in_entry(void *arg_p)
{
    (void)arg_p;

    likely_undefined_padding_mock_once();
    likely_undefined_padding_mock_set_value_p_in_assert(
        fail_if_in_assert_is_called_before_set_in_in_assert);
    likely_undefined_padding(NULL);
}

TEST(fail_if_in_assert_is_called_before_set_in)
{
    function_error_in_subprocess(
        fail_if_in_assert_is_called_before_set_in_entry,
        "likely_undefined_padding_mock_set_value_p_in() must be called "
        "before likely_undefined_padding_mock_set_value_p_in_assert().");
}

static void in_assert_error_mock_traceback_message_in_assert(
    struct likely_undefined_padding_t *actual_p,
    struct likely_undefined_padding_t *expected_p,
    size_t size)
{
    (void)size;

    ASSERT_EQ(actual_p->b, expected_p->b);
}

static void in_assert_error_mock_traceback_message_entry(void *arg_p)
{
    (void)arg_p;

    struct likely_undefined_padding_t data;

    memset(&data, 0, sizeof(data));
    data.b = 4;
    likely_undefined_padding_mock_once();
    likely_undefined_padding_mock_set_value_p_in(&data, sizeof(data));
    likely_undefined_padding_mock_set_value_p_in_assert(
        in_assert_error_mock_traceback_message_in_assert);
    data.b++;
    likely_undefined_padding(&data);
}

TEST(in_assert_error_mock_traceback_message)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(
        in_assert_error_mock_traceback_message_entry,
        NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked likely_undefined_padding(value_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "5 is not equal to 4 (0x5, 0x4)");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mock traceback (most recent call first):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Assert traceback (most recent call first):");

    subprocess_result_free(result_p);
}

static void struct_assert_error_entry(void *arg_p)
{
    (void)arg_p;

    struct likely_undefined_padding_t data;

    memset(&data, 0, sizeof(data));
    data.d = 2;
    likely_undefined_padding_mock_once();
    likely_undefined_padding_mock_set_value_p_in(&data, sizeof(data));
    data.d++;
    likely_undefined_padding(&data);
}

TEST(struct_assert_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(struct_assert_error_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked likely_undefined_padding(value_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "3 is not equal to 2 (0x3, 0x2)");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mock traceback (most recent call first):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Assert traceback (most recent call first):");

    subprocess_result_free(result_p);
}

static int call_with_arg_callback(void *arg_p)
{
    (void)arg_p;

    return (0);
}

TEST(call_with_arg_get_params_in)
{
    int handle;
    int arg;

    handle = call_with_arg_mock_ignore_in_once(9);
    ASSERT_EQ(call_with_arg(call_with_arg_callback, &arg), 9);
    ASSERT_FUNCTION_POINTER_EQ(call_with_arg_mock_get_params_in(handle)->func,
                               call_with_arg_callback);
    ASSERT_EQ(call_with_arg_mock_get_params_in(handle)->arg_p, &arg);
}

static void call_with_arg_not_called_error_entry(void *arg_p)
{
    (void)arg_p;

    int handle;

    handle = call_with_arg_mock_ignore_in_once(9);
    call_with_arg_mock_get_params_in(handle);
}

TEST(call_with_arg_not_called_error)
{
    function_error_in_subprocess(
        call_with_arg_not_called_error_entry,
        "call_with_arg() has not been called yet for given mock handle. No "
        "parameters available.");
}

TEST(struct_pointer_typedef_function)
{
    struct DummyStruct value;
    struct_pointer_typedef_t value_p;

    value.number = -5;
    value_p = &value;
    struct_pointer_typedef_mock_once(-4);
    struct_pointer_typedef_mock_set_value_in(value_p, sizeof(*value_p));

    ASSERT_EQ(struct_pointer_typedef(&value), -4);
}

TEST(struct_pointer_typedef_2_function)
{
    struct DummyStruct value;
    struct_pointer_typedef_2_t value_p;

    value.number = -5;
    value_p = &value;
    struct_pointer_typedef_2_mock_once(-4);
    struct_pointer_typedef_2_mock_set_value_in(value_p, sizeof(*value_p));

    ASSERT_EQ(struct_pointer_typedef_2(&value), -4);
}

TEST(struct_typedef_function)
{
    struct_typedef_t value;

    struct_typedef_mock_once(-4);

    ASSERT_EQ(struct_typedef(value), -4);
}

TEST(struct_typedef_2_function)
{
    struct_typedef_2_t value;

    struct_typedef_2_mock_once(-4);

    ASSERT_EQ(struct_typedef_2(value), -4);
}

TEST(parameter_name_omitted_function)
{
    parameter_name_omitted_mock_once(1, 2);

    ASSERT_EQ(parameter_name_omitted(1, NULL), 2);
}

TEST(no_implementation_function)
{
    no_implementation_mock_once(2);

    ASSERT_EQ(no_implementation(), 2);
}

TEST(another_missing_implementation_function)
{
    another_missing_implementation_mock_once(2);

    ASSERT_EQ(another_missing_implementation(), 2);
}

/* Tests that "__"-prefix is removed from parameter names. */
TEST(underscore_prefixed_params_function)
{
    underscore_prefixed_params_mock(1, 1, 2);
    underscore_prefixed_params_mock_ignore_a_in();
    underscore_prefixed_params_mock_ignore_b_in();
}

TEST(vsyslog_ignore_va_list)
{
    va_list ap;

    vsyslog_mock_once(1, "Hello");
    vsyslog(1, "Hello", ap);
}

TEST(my_va_list_function)
{
    va_list ap;

    my_va_list_mock(1);

    ASSERT_EQ(my_va_list(ap), 1);
}

static void array_member_function_error_entry(void *arg_p)
{
    (void)arg_p;

    struct array_member_t value;

    value.a[0] = 2;
    value.a[1] = 4;

    array_member_mock_once(9);
    array_member_mock_set_value_p_in(&value, sizeof(value));

    value.a[1] = 10;
    array_member(&value);
}

TEST(array_member_function_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(array_member_function_error_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked array_member(value_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "The arrays differ at index 1. See diff for details.");

    subprocess_result_free(result_p);
}

/* The generator should ignore this type as it is declared in the test
   file, which can't be included by nala_mocks.h. */
struct should_be_ignored_t {
    int a;
};

void to_be_mocked()
{
}

TEST(to_be_mocked_function)
{
    /* Only structs declared before mocked functions are used. */
    to_be_mocked_mock_ignore_in();
}

static void nested_types_error_1_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.c.a = 1;
    nested_types(&value);
}

TEST(nested_types_error_1)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_1_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void nested_types_error_2_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.e.f.g.a = 1;
    nested_types(&value);
}

TEST(nested_types_error_2)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_2_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void nested_types_error_nested_struct_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.e.f.h = 1;
    nested_types(&value);
}

TEST(nested_types_error_nested_struct)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_nested_struct_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void nested_types_error_struct_typedef_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.k.a = 1;
    nested_types(&value);
}

TEST(nested_types_error_struct_typedef)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_struct_typedef_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void nested_types_error_enum_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.o = enum_param_type_b;
    nested_types(&value);
}

TEST(nested_types_error_enum)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_enum_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void nested_types_error_int32_t_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.q = 1;
    nested_types(&value);
}

TEST(nested_types_error_int32_t)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_error_int32_t_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types(bar_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

TEST(nested_types_pointer_not_asserted)
{
    struct nested_bar_t value;
    nested_foo_2_t l;
    int n;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.l = &l;
    value.n_p = &n;
    nested_types(&value);
}

TEST(nested_types_union_not_asserted)
{
    struct nested_bar_t value;

    memset(&value, 0, sizeof(value));

    nested_types_mock_once();
    nested_types_mock_set_bar_p_in(&value, sizeof(value));

    value.i.a = 1;
    nested_types(&value);
}

static void nested_types_2_error_entry(void *arg_p)
{
    (void)arg_p;

    nested_foo_2_t value;

    memset(&value, 0, sizeof(value));

    nested_types_2_mock_once();
    nested_types_2_mock_set_foo_p_in(&value, sizeof(value));

    value.a = 1;
    nested_types_2(&value);
}

TEST(nested_types_2_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_2_error_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types_2(foo_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void struct_param_typedef_pointer_error_entry(void *arg_p)
{
    (void)arg_p;

    struct struct_param_type value;

    memset(&value, 0, sizeof(value));

    struct_param_typedef_pointer_mock_once();
    struct_param_typedef_pointer_mock_set_arg_in(&value, sizeof(value));

    value.number = 1;
    struct_param_typedef_pointer(&value);
}

TEST(struct_param_typedef_pointer_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(struct_param_typedef_pointer_error_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Mocked struct_param_typedef_pointer(arg):");
    /* ToDo. */
    ASSERT_SUBSTRING(result_p->stdout.buf_p,
                     "Memory mismatch. See diff for details.");
    /* ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)"); */

    subprocess_result_free(result_p);
}

TEST(nested_types_pp_function)
{
    struct nested_bar_t value;
    struct nested_bar_t *value_p;

    value_p = &value;
    nested_types_pp_mock_once();
    nested_types_pp_mock_set_bar_pp_in(&value_p, sizeof(value_p));

    nested_types_pp(&value_p);
}

static void nested_types_pp_error_entry(void *arg_p)
{
    (void)arg_p;

    struct nested_bar_t value;
    struct nested_bar_t *value_p;

    value_p = NULL;
    nested_types_pp_mock_once();
    nested_types_pp_mock_set_bar_pp_in(&value_p, sizeof(value_p));

    value_p = &value;
    nested_types_pp(&value_p);
}

TEST(nested_types_pp_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_pp_error_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types_pp(bar_pp):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, " is not equal to (nil)");

    subprocess_result_free(result_p);
}

static void nested_types_2_pp_error_entry(void *arg_p)
{
    (void)arg_p;

    nested_foo_2_t value;
    nested_foo_2_t *value_p;

    value_p = NULL;
    nested_types_2_pp_mock_once();
    nested_types_2_pp_mock_set_foo_pp_in(&value_p, sizeof(value_p));

    value_p = &value;
    nested_types_2_pp(&value_p);
}

TEST(nested_types_2_pp_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(nested_types_2_pp_error_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked nested_types_2_pp(foo_pp):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, " is not equal to (nil)");

    subprocess_result_free(result_p);
}

TEST(forward_declaration_function)
{
    forward_declaration_mock();
}

TEST(forward_declaration_2_function)
{
    forward_declaration_2_mock();
}

TEST(forward_declaration_3_function)
{
    forward_declaration_3_mock();
}

TEST(struct_declared_in_typedef_function)
{
    struct_declared_in_typedef_mock();
}

TEST(my_fclose_mock_function)
{
    my_fclose_mock();
}

TEST(string_pointer_function)
{
    char string[] = "string";
    char *string_p;

    string_pointer_mock_once();
    string_p = &string[0];
    string_pointer_mock_set_string_pp_out(&string_p, sizeof(string_p));
    string_p = NULL;
    string_pointer(&string_p);
    ASSERT_EQ(string_p, &string[0]);

    string_pointer_mock_once();
    string_p = &string[0];
    string_pointer_mock_set_string_pp_in(&string_p, sizeof(string_p));
    string_pointer(&string_p);

    string_pointer_mock_once();
    string_pointer_mock_set_string_pp_in_pointer(NULL);
    string_pointer(NULL);
}

static void bit_field_error_uint32_t_entry(void *arg_p)
{
    (void)arg_p;

    struct bit_field_t value;

    memset(&value, 0, sizeof(value));

    bit_field_mock_once();
    bit_field_mock_set_value_p_in(&value, sizeof(value));

    value.a = 1;
    bit_field(&value);
}

TEST(bit_field_error_uint32_t)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(bit_field_error_uint32_t_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked bit_field(value_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "1 is not equal to 0 (0x1, 0x0)");

    subprocess_result_free(result_p);
}

static void bit_field_error_int_entry(void *arg_p)
{
    (void)arg_p;

    struct bit_field_t value;

    memset(&value, 0, sizeof(value));

    bit_field_mock_once();
    bit_field_mock_set_value_p_in(&value, sizeof(value));

    value.b = 2;
    bit_field(&value);
}

TEST(bit_field_error_int)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(bit_field_error_int_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked bit_field(value_p):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "2 is not equal to 0 (0x2, 0x0)");

    subprocess_result_free(result_p);
}

TEST(const_parameter_function)
{
    const_parameter_mock_once(1);
    const_parameter(1);
}

static void const_parameter_function_error_entry(void *arg_p)
{
    (void)arg_p;

    const_parameter_mock_once(1);
    const_parameter(2);
}

TEST(const_parameter_function_error)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(const_parameter_function_error_entry,
                                      NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked const_parameter(value):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "2 is not equal to 1 (0x2, 0x1)");

    subprocess_result_free(result_p);
}

/* ToDo: Support this! */

/* TEST(const_typedef_parameter_function) */
/* { */
/*     const_typedef_parameter_mock_once(1); */
/*     const_typedef_parameter(1); */
/* } */

TEST(const_struct_pointer_function)
{
    struct banan_2_t banan;

    const_struct_pointer_mock_once();
    const_struct_pointer(&banan);
}

TEST(const_typedef_struct_pointer_function)
{
    banan_3_t banan;

    const_typedef_struct_pointer_mock_once();
    const_typedef_struct_pointer(&banan);
}

TEST(const_pointer_data_function)
{
    int a;

    const_pointer_data_mock_once();

    a = 1;
    const_pointer_data(&a);
}

TEST(string_typedef_function)
{
    string_typedef_mock_once();
    string_typedef_mock_set_string_in("hello", 6);
    string_typedef("hello");
}

static void string_typedef_error_message_entry(void *arg_p)
{
    (void)arg_p;

    string_typedef_mock_once();
    string_typedef_mock_set_string_in("hello", 6);
    string_typedef("HELLO");
}

TEST(string_typedef_error_message)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(string_typedef_error_message_entry, NULL);

    ASSERT_NE(result_p->exit_code, 0);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "Mocked string_typedef(string):");
    ASSERT_SUBSTRING(result_p->stdout.buf_p, B("hello"));
    ASSERT_SUBSTRING(result_p->stdout.buf_p, CBR("HELLO"));

    subprocess_result_free(result_p);
}

TEST(string_typedef_p_function)
{
    char string[] = "string";
    char *string_p;

    string_typedef_p_mock_once();
    string_p = &string[0];
    string_typedef_p_mock_set_string_p_out(&string_p, sizeof(string_p));
    string_p = NULL;
    string_typedef_p(&string_p);
    ASSERT_EQ(string_p, &string[0]);

    string_typedef_p_mock_once();
    string_p = &string[0];
    string_typedef_p_mock_set_string_p_in(&string_p, sizeof(string_p));
    string_typedef_p(&string_p);

    string_typedef_p_mock_once();
    string_typedef_p_mock_set_string_p_in_pointer(NULL);
    string_typedef_p(NULL);
}

TEST(char_typedef_function)
{
    char_typedef_mock_once();
    char_typedef_mock_set_string_p_in("hello", 6);
    char_typedef("hello");
}

TEST(int_array_function)
{
    int value[3];

    value[0] = 3;
    value[1] = 2;
    value[2] = 1;

    int_array_mock();
    int_array_mock_set_value_in(&value[0], sizeof(value));
    int_array(&value[0]);
}

TEST(int_array_fixed_function)
{
    int_array_fixed_t value;

    value[0] = 3;
    value[1] = 2;
    value[2] = 1;

    int_array_fixed_mock();
    int_array_fixed_mock_set_value_in(&value[0], sizeof(value));
    int_array_fixed(&value[0]);
}

TEST(int8_function)
{
    int8_t value[] = { 1, 2, 3 };

    int8_mock();
    int8_mock_set_buf_p_in(&value[0], 3);
    int8(&value[0]);
}

TEST(uint8_function)
{
    uint8_t value[] = { 1, 2, 3 };

    uint8_mock();
    uint8_mock_set_buf_p_in(&value[0], 3);
    uint8(&value[0]);
}

TEST(array_of_pointers_function)
{
    const char *argv[] = {
        "a",
        "b"
    };

    array_of_pointers_mock_once();
    array_of_pointers_mock_set_argv_in(&argv[0], sizeof(argv));
    array_of_pointers(&argv[0]);

    array_of_pointers_mock_once();
    array_of_pointers(NULL);
}
