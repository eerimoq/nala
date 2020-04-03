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
#include "../tests/files/test_collect.h"

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

// NALA_DECLARATION bar

struct nala_bar_params_t {
    int dummy;
};

void bar_mock(int return_value);
int bar_mock_once(int return_value);
void bar_mock_ignore_in(int return_value);
int bar_mock_ignore_in_once(int return_value);
void bar_mock_set_errno(int errno_value);
void bar_mock_set_callback(void (*callback)());
struct nala_bar_params_t *bar_mock_get_params_in(int handle);
void bar_mock_none(void);
void bar_mock_implementation(int (*implementation)());
void bar_mock_real(void);
void bar_mock_real_once(void);
void bar_mock_suspend(void);
void bar_mock_resume(void);
void bar_mock_reset(void);
void bar_mock_assert_completed(void);

// NALA_DECLARATION fie

struct nala_fie_params_t {
    int dummy;
};

void fie_mock(int return_value);
int fie_mock_once(int return_value);
void fie_mock_ignore_in(int return_value);
int fie_mock_ignore_in_once(int return_value);
void fie_mock_set_errno(int errno_value);
void fie_mock_set_callback(void (*callback)());
struct nala_fie_params_t *fie_mock_get_params_in(int handle);
void fie_mock_none(void);
void fie_mock_implementation(int (*implementation)());
void fie_mock_real(void);
void fie_mock_real_once(void);
void fie_mock_suspend(void);
void fie_mock_resume(void);
void fie_mock_reset(void);
void fie_mock_assert_completed(void);

// NALA_DECLARATION foo

struct nala_foo_params_t {
    int dummy;
};

void foo_mock(int return_value);
int foo_mock_once(int return_value);
void foo_mock_ignore_in(int return_value);
int foo_mock_ignore_in_once(int return_value);
void foo_mock_set_errno(int errno_value);
void foo_mock_set_callback(void (*callback)());
struct nala_foo_params_t *foo_mock_get_params_in(int handle);
void foo_mock_none(void);
void foo_mock_implementation(int (*implementation)());
void foo_mock_real(void);
void foo_mock_real_once(void);
void foo_mock_suspend(void);
void foo_mock_resume(void);
void foo_mock_reset(void);
void foo_mock_assert_completed(void);

// NALA_DECLARATION fum

struct nala_fum_params_t {
    int dummy;
};

void fum_mock(int return_value);
int fum_mock_once(int return_value);
void fum_mock_ignore_in(int return_value);
int fum_mock_ignore_in_once(int return_value);
void fum_mock_set_errno(int errno_value);
void fum_mock_set_callback(void (*callback)());
struct nala_fum_params_t *fum_mock_get_params_in(int handle);
void fum_mock_none(void);
void fum_mock_implementation(int (*implementation)());
void fum_mock_real(void);
void fum_mock_real_once(void);
void fum_mock_suspend(void);
void fum_mock_resume(void);
void fum_mock_reset(void);
void fum_mock_assert_completed(void);

// NALA_DECLARATION gam

struct nala_gam_params_t {
    int dummy;
};

void gam_mock(int return_value);
int gam_mock_once(int return_value);
void gam_mock_ignore_in(int return_value);
int gam_mock_ignore_in_once(int return_value);
void gam_mock_set_errno(int errno_value);
void gam_mock_set_callback(void (*callback)());
struct nala_gam_params_t *gam_mock_get_params_in(int handle);
void gam_mock_none(void);
void gam_mock_implementation(int (*implementation)());
void gam_mock_real(void);
void gam_mock_real_once(void);
void gam_mock_suspend(void);
void gam_mock_resume(void);
void gam_mock_reset(void);
void gam_mock_assert_completed(void);

// NALA_DECLARATION hit

struct nala_hit_params_t {
    int dummy;
};

void hit_mock(int return_value);
int hit_mock_once(int return_value);
void hit_mock_ignore_in(int return_value);
int hit_mock_ignore_in_once(int return_value);
void hit_mock_set_errno(int errno_value);
void hit_mock_set_callback(void (*callback)());
struct nala_hit_params_t *hit_mock_get_params_in(int handle);
void hit_mock_none(void);
void hit_mock_implementation(int (*implementation)());
void hit_mock_real(void);
void hit_mock_real_once(void);
void hit_mock_suspend(void);
void hit_mock_resume(void);
void hit_mock_reset(void);
void hit_mock_assert_completed(void);

