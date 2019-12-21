/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>
#include "../nala/dist/nala.h"
#include "../tests/files/test_collect.h"

#endif

void nala_suspend_all_mocks(void);
void nala_resume_all_mocks(void);
void nala_reset_all_mocks(void);
void nala_assert_all_mocks_completed(void);

// NALA_DECLARATION bar

void bar_mock(int return_value);
void bar_mock_once(int return_value);
void bar_mock_ignore_in(int return_value);
void bar_mock_ignore_in_once(int return_value);
void bar_mock_set_errno(int errno_value);
void bar_mock_set_callback(void (*callback)());
void bar_mock_none(void);
void bar_mock_implementation(int (*implementation)());
void bar_mock_disable(void);
void bar_mock_suspend(void);
void bar_mock_resume(void);
void bar_mock_reset(void);
void bar_mock_assert_completed(void);

// NALA_DECLARATION fie

void fie_mock(int return_value);
void fie_mock_once(int return_value);
void fie_mock_ignore_in(int return_value);
void fie_mock_ignore_in_once(int return_value);
void fie_mock_set_errno(int errno_value);
void fie_mock_set_callback(void (*callback)());
void fie_mock_none(void);
void fie_mock_implementation(int (*implementation)());
void fie_mock_disable(void);
void fie_mock_suspend(void);
void fie_mock_resume(void);
void fie_mock_reset(void);
void fie_mock_assert_completed(void);

// NALA_DECLARATION foo

void foo_mock(int return_value);
void foo_mock_once(int return_value);
void foo_mock_ignore_in(int return_value);
void foo_mock_ignore_in_once(int return_value);
void foo_mock_set_errno(int errno_value);
void foo_mock_set_callback(void (*callback)());
void foo_mock_none(void);
void foo_mock_implementation(int (*implementation)());
void foo_mock_disable(void);
void foo_mock_suspend(void);
void foo_mock_resume(void);
void foo_mock_reset(void);
void foo_mock_assert_completed(void);

// NALA_DECLARATION fum

void fum_mock(int return_value);
void fum_mock_once(int return_value);
void fum_mock_ignore_in(int return_value);
void fum_mock_ignore_in_once(int return_value);
void fum_mock_set_errno(int errno_value);
void fum_mock_set_callback(void (*callback)());
void fum_mock_none(void);
void fum_mock_implementation(int (*implementation)());
void fum_mock_disable(void);
void fum_mock_suspend(void);
void fum_mock_resume(void);
void fum_mock_reset(void);
void fum_mock_assert_completed(void);

// NALA_DECLARATION gam

void gam_mock(int return_value);
void gam_mock_once(int return_value);
void gam_mock_ignore_in(int return_value);
void gam_mock_ignore_in_once(int return_value);
void gam_mock_set_errno(int errno_value);
void gam_mock_set_callback(void (*callback)());
void gam_mock_none(void);
void gam_mock_implementation(int (*implementation)());
void gam_mock_disable(void);
void gam_mock_suspend(void);
void gam_mock_resume(void);
void gam_mock_reset(void);
void gam_mock_assert_completed(void);

// NALA_DECLARATION hit

void hit_mock(int return_value);
void hit_mock_once(int return_value);
void hit_mock_ignore_in(int return_value);
void hit_mock_ignore_in_once(int return_value);
void hit_mock_set_errno(int errno_value);
void hit_mock_set_callback(void (*callback)());
void hit_mock_none(void);
void hit_mock_implementation(int (*implementation)());
void hit_mock_disable(void);
void hit_mock_suspend(void);
void hit_mock_resume(void);
void hit_mock_reset(void);
void hit_mock_assert_completed(void);

