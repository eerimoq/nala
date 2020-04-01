/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>
#include <fcntl.h>

#endif

struct nala_traceback_t {
    void *addresses[32];
    int depth;
};

typedef void (*nala_mock_in_assert_t)(const void *actual_p,
                                      const void *expected_p,
                                      size_t size);

typedef void (*nala_mock_out_copy_t)(void *dst_p,
                                     const void *src_p,
                                     size_t size);

void nala_suspend_all_mocks(void);
void nala_resume_all_mocks(void);
void nala_reset_all_mocks(void);
void nala_assert_all_mocks_completed(void);

// NALA_DECLARATION open

struct nala_open_params_t {
    const char *pathname;
    int flags;
    const char *vafmt_p;
};

int open_mock_va_arg_real(const char *pathname, int flags, va_list __nala_va_list);
void open_mock(const char *pathname, int flags, int return_value, const char *vafmt_p, ...);
int open_mock_once(const char *pathname, int flags, int return_value, const char *vafmt_p, ...);
void open_mock_ignore_in(int return_value);
int open_mock_ignore_in_once(int return_value);
void open_mock_set_errno(int errno_value);
void open_mock_set_callback(void (*callback)(const char *pathname, int flags, va_list __nala_va_list));
struct nala_open_params_t *open_mock_get_params_in(int handle);
void open_mock_ignore_pathname_in(void);
void open_mock_ignore_flags_in(void);
void open_mock_set_pathname_in(const void *buf_p, size_t size);
void open_mock_set_pathname_in_assert(void (*callback)(const char *pathname, const void *nala_buf_p, size_t nala_size));
void open_mock_set_pathname_in_pointer(const char *pathname);
void open_mock_set_pathname_out(const void *buf_p, size_t size);
void open_mock_set_pathname_out_copy(void (*callback)(const char *pathname, const void *nala_buf_p, size_t nala_size));
void open_mock_ignore_va_arg_in_at(unsigned int index);
void open_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size);
void open_mock_set_va_arg_in_assert_at(unsigned int index, nala_mock_in_assert_t in_assert);
void open_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p);
void open_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size);
void open_mock_set_va_arg_out_copy_at(unsigned int index, nala_mock_out_copy_t out_copy);
void open_mock_none(void);
void open_mock_implementation(int (*implementation)(const char *pathname, int flags, va_list __nala_va_list));
void open_mock_real(void);
void open_mock_real_once(void);
void open_mock_suspend(void);
void open_mock_resume(void);
void open_mock_reset(void);
void open_mock_assert_completed(void);

