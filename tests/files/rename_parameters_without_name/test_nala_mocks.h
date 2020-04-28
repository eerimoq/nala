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
#include "../tests/files/rename_parameters_without_name/test.h"

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

// NALA_DECLARATION rename_parameters_with_name

struct nala_rename_parameters_with_name_params_t {
    const char *path;
    int flags;
};

void rename_parameters_with_name_mock(const char *path, int flags);
int rename_parameters_with_name_mock_once(const char *path, int flags);
void rename_parameters_with_name_mock_ignore_in(void);
int rename_parameters_with_name_mock_ignore_in_once(void);
void rename_parameters_with_name_mock_set_errno(int errno_value);
void rename_parameters_with_name_mock_set_callback(void (*callback)(const char *path, int flags));
struct nala_rename_parameters_with_name_params_t *rename_parameters_with_name_mock_get_params_in(int handle);
void rename_parameters_with_name_mock_ignore_path_in(void);
void rename_parameters_with_name_mock_ignore_flags_in(void);
void rename_parameters_with_name_mock_set_path_in(const char *buf_p, size_t size);
void rename_parameters_with_name_mock_set_path_in_assert(void (*callback)(const char *actual_p, const char *expected_p, size_t size));
void rename_parameters_with_name_mock_set_path_in_pointer(const char *path);
void rename_parameters_with_name_mock_set_path_out(const char *buf_p, size_t size);
void rename_parameters_with_name_mock_set_path_out_copy(void (*callback)(const char *dst_p, const char *src_p, size_t size));
void rename_parameters_with_name_mock_none(void);
void rename_parameters_with_name_mock_implementation(void (*implementation)(const char *path, int flags));
void rename_parameters_with_name_mock_real(void);
void rename_parameters_with_name_mock_real_once(void);
void rename_parameters_with_name_mock_suspend(void);
void rename_parameters_with_name_mock_resume(void);
void rename_parameters_with_name_mock_reset(void);
void rename_parameters_with_name_mock_assert_completed(void);

// NALA_DECLARATION rename_parameters_without_name

struct nala_rename_parameters_without_name_params_t {
    const char *path;
    int flags;
};

void rename_parameters_without_name_mock(const char *path, int flags);
int rename_parameters_without_name_mock_once(const char *path, int flags);
void rename_parameters_without_name_mock_ignore_in(void);
int rename_parameters_without_name_mock_ignore_in_once(void);
void rename_parameters_without_name_mock_set_errno(int errno_value);
void rename_parameters_without_name_mock_set_callback(void (*callback)(const char *path, int flags));
struct nala_rename_parameters_without_name_params_t *rename_parameters_without_name_mock_get_params_in(int handle);
void rename_parameters_without_name_mock_ignore_path_in(void);
void rename_parameters_without_name_mock_ignore_flags_in(void);
void rename_parameters_without_name_mock_set_path_in(const char *buf_p, size_t size);
void rename_parameters_without_name_mock_set_path_in_assert(void (*callback)(const char *actual_p, const char *expected_p, size_t size));
void rename_parameters_without_name_mock_set_path_in_pointer(const char *path);
void rename_parameters_without_name_mock_set_path_out(const char *buf_p, size_t size);
void rename_parameters_without_name_mock_set_path_out_copy(void (*callback)(const char *dst_p, const char *src_p, size_t size));
void rename_parameters_without_name_mock_none(void);
void rename_parameters_without_name_mock_implementation(void (*implementation)(const char *path, int flags));
void rename_parameters_without_name_mock_real(void);
void rename_parameters_without_name_mock_real_once(void);
void rename_parameters_without_name_mock_suspend(void);
void rename_parameters_without_name_mock_resume(void);
void rename_parameters_without_name_mock_reset(void);
void rename_parameters_without_name_mock_assert_completed(void);

