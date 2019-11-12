/*
Mocks header file

Generated with Nala version 0.5.0 (https://github.com/eerimoq/nala)
Do not edit manually
*/

#ifndef MOCKS_H
#define MOCKS_H

#include <stdarg.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <x86_64-linux-gnu/sys/mount.h>

#include "dummy_functions.h"

#ifndef _NALA_RESET_ALL_MOCKS
#define _NALA_RESET_ALL_MOCKS 1
void nala_reset_all_mocks(void);
#endif

void nala_assert_all_mocks_completed(void);

// NALA_DECLARATION add
// NALA_LINKER_FLAGS -Wl,--wrap=add

void add_mock(int x, int y, int return_value);
void add_mock_once(int x, int y, int return_value);
void add_mock_ignore_in(int return_value);
void add_mock_ignore_in_once(int return_value);
void add_mock_set_errno(int errno_value);
void add_mock_ignore_x_in(void);
void add_mock_ignore_y_in(void);
void add_mock_none(void);
void add_mock_implementation(int (*implementation)(int x, int y));
void add_mock_disable(void);
void add_mock_reset(void);
void add_mock_assert_completed(void);

// NALA_DECLARATION compose_twice
// NALA_LINKER_FLAGS -Wl,--wrap=compose_twice

void compose_twice_mock(DummyStruct *return_value);
void compose_twice_mock_once(DummyStruct *return_value);
void compose_twice_mock_ignore_in(DummyStruct *return_value);
void compose_twice_mock_ignore_in_once(DummyStruct *return_value);
void compose_twice_mock_set_errno(int errno_value);
void compose_twice_mock_set_dummy_struct_in(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct);
void compose_twice_mock_set_dummy_struct_out(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_modifier_in(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_modifier_in_pointer(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
void compose_twice_mock_set_dummy_struct_modifier_out(const void *buf_p, size_t size);
void compose_twice_mock_none(void);
void compose_twice_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)));
void compose_twice_mock_disable(void);
void compose_twice_mock_reset(void);
void compose_twice_mock_assert_completed(void);

// NALA_DECLARATION edit_number
// NALA_LINKER_FLAGS -Wl,--wrap=edit_number

void edit_number_mock(int number, DummyStruct *return_value);
void edit_number_mock_once(int number, DummyStruct *return_value);
void edit_number_mock_ignore_in(DummyStruct *return_value);
void edit_number_mock_ignore_in_once(DummyStruct *return_value);
void edit_number_mock_set_errno(int errno_value);
void edit_number_mock_ignore_number_in(void);
void edit_number_mock_set_dummy_struct_in(const void *buf_p, size_t size);
void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct);
void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size);
void edit_number_mock_none(void);
void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number));
void edit_number_mock_disable(void);
void edit_number_mock_reset(void);
void edit_number_mock_assert_completed(void);

// NALA_DECLARATION fopen
// NALA_LINKER_FLAGS -Wl,--wrap=fopen

void fopen_mock(const char *__filename, const char *__modes, FILE *return_value);
void fopen_mock_once(const char *__filename, const char *__modes, FILE *return_value);
void fopen_mock_ignore_in(FILE *return_value);
void fopen_mock_ignore_in_once(FILE *return_value);
void fopen_mock_set_errno(int errno_value);
void fopen_mock_ignore___filename_in(void);
void fopen_mock_ignore___modes_in(void);
void fopen_mock_set___filename_in(const void *buf_p, size_t size);
void fopen_mock_set___filename_in_pointer(const char *__filename);
void fopen_mock_set___filename_out(const void *buf_p, size_t size);
void fopen_mock_set___modes_in(const void *buf_p, size_t size);
void fopen_mock_set___modes_in_pointer(const char *__modes);
void fopen_mock_set___modes_out(const void *buf_p, size_t size);
void fopen_mock_none(void);
void fopen_mock_implementation(FILE *(*implementation)(const char *__filename, const char *__modes));
void fopen_mock_disable(void);
void fopen_mock_reset(void);
void fopen_mock_assert_completed(void);

// NALA_DECLARATION io_control
// NALA_LINKER_FLAGS -Wl,--wrap=io_control

int io_control_mock_va_arg_real(int kind, va_list __nala_va_list);
void io_control_mock(int kind, int return_value, const char *vafmt_p, ...);
void io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...);
void io_control_mock_ignore_in(int return_value);
void io_control_mock_ignore_in_once(int return_value);
void io_control_mock_set_errno(int errno_value);
void io_control_mock_ignore_kind_in(void);
void io_control_mock_ignore_va_arg_in_at(unsigned int index);
void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size);
void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p);
void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size);
void io_control_mock_none(void);
void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list));
void io_control_mock_disable(void);
void io_control_mock_reset(void);
void io_control_mock_assert_completed(void);

// NALA_DECLARATION mount
// NALA_LINKER_FLAGS -Wl,--wrap=mount

void mount_mock(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, int return_value);
void mount_mock_once(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, int return_value);
void mount_mock_ignore_in(int return_value);
void mount_mock_ignore_in_once(int return_value);
void mount_mock_set_errno(int errno_value);
void mount_mock_ignore___special_file_in(void);
void mount_mock_ignore___dir_in(void);
void mount_mock_ignore___fstype_in(void);
void mount_mock_ignore___rwflag_in(void);
void mount_mock_set___special_file_in(const void *buf_p, size_t size);
void mount_mock_set___special_file_in_pointer(const char *__special_file);
void mount_mock_set___special_file_out(const void *buf_p, size_t size);
void mount_mock_set___dir_in(const void *buf_p, size_t size);
void mount_mock_set___dir_in_pointer(const char *__dir);
void mount_mock_set___dir_out(const void *buf_p, size_t size);
void mount_mock_set___fstype_in(const void *buf_p, size_t size);
void mount_mock_set___fstype_in_pointer(const char *__fstype);
void mount_mock_set___fstype_out(const void *buf_p, size_t size);
void mount_mock_set___data_in(const void *buf_p, size_t size);
void mount_mock_set___data_in_pointer(const void *__data);
void mount_mock_set___data_out(const void *buf_p, size_t size);
void mount_mock_none(void);
void mount_mock_implementation(int (*implementation)(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const void *__data));
void mount_mock_disable(void);
void mount_mock_reset(void);
void mount_mock_assert_completed(void);

// NALA_DECLARATION pipe
// NALA_LINKER_FLAGS -Wl,--wrap=pipe

void pipe_mock(int return_value);
void pipe_mock_once(int return_value);
void pipe_mock_ignore_in(int return_value);
void pipe_mock_ignore_in_once(int return_value);
void pipe_mock_set_errno(int errno_value);
void pipe_mock_set___pipedes_in(const void *buf_p, size_t size);
void pipe_mock_set___pipedes_in_pointer(int __pipedes[2]);
void pipe_mock_set___pipedes_out(const void *buf_p, size_t size);
void pipe_mock_none(void);
void pipe_mock_implementation(int (*implementation)(int __pipedes[2]));
void pipe_mock_disable(void);
void pipe_mock_reset(void);
void pipe_mock_assert_completed(void);

// NALA_DECLARATION time
// NALA_LINKER_FLAGS -Wl,--wrap=time

void time_mock(time_t return_value);
void time_mock_once(time_t return_value);
void time_mock_ignore_in(time_t return_value);
void time_mock_ignore_in_once(time_t return_value);
void time_mock_set_errno(int errno_value);
void time_mock_set___timer_in(const void *buf_p, size_t size);
void time_mock_set___timer_in_pointer(time_t *__timer);
void time_mock_set___timer_out(const void *buf_p, size_t size);
void time_mock_none(void);
void time_mock_implementation(time_t (*implementation)(time_t *__timer));
void time_mock_disable(void);
void time_mock_reset(void);
void time_mock_assert_completed(void);

#endif
