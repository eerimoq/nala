/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>
#include <stddef.h>
#include "../nala/dist/nala.h"

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

// NALA_DECLARATION foo

struct nala_foo_params_t {
    int a;
    int __;
};

void foo_mock(int a, int __, int return_value);
int foo_mock_once(int a, int __, int return_value);
void foo_mock_ignore_in(int return_value);
int foo_mock_ignore_in_once(int return_value);
void foo_mock_set_errno(int errno_value);
void foo_mock_set_callback(void (*callback)(int a, int __));
struct nala_foo_params_t *foo_mock_get_params_in(int handle);
void foo_mock_ignore_a_in(void);
void foo_mock_ignore____in(void);
void foo_mock_none(void);
void foo_mock_implementation(int (*implementation)(int a, int __));
void foo_mock_real(void);
void foo_mock_real_once(void);
void foo_mock_suspend(void);
void foo_mock_resume(void);
void foo_mock_reset(void);
void foo_mock_assert_completed(void);

