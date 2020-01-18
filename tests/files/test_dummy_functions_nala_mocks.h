/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>
#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <x86_64-linux-gnu/sys/mount.h>
#include <x86_64-linux-gnu/sys/timerfd.h>
#include <x86_64-linux-gnu/sys/socket.h>
#include <x86_64-linux-gnu/sys/statvfs.h>
#include <unistd.h>
#include <poll.h>
#include <mntent.h>
#include "../nala/dist/nala.h"
#include "../tests/files/test_dummy_functions.h"

#endif

struct nala_traceback_t {
    void *addresses[32];
    int depth;
};

void nala_suspend_all_mocks(void);
void nala_resume_all_mocks(void);
void nala_reset_all_mocks(void);
void nala_assert_all_mocks_completed(void);

// NALA_DECLARATION add

void add_mock(int x, int y, int return_value);
void add_mock_once(int x, int y, int return_value);
void add_mock_ignore_in(int return_value);
void add_mock_ignore_in_once(int return_value);
void add_mock_set_errno(int errno_value);
void add_mock_set_callback(void (*callback)(int x, int y));
void add_mock_ignore_x_in(void);
void add_mock_ignore_y_in(void);
void add_mock_none(void);
void add_mock_implementation(int (*implementation)(int x, int y));
void add_mock_disable(void);
void add_mock_suspend(void);
void add_mock_resume(void);
void add_mock_reset(void);
void add_mock_assert_completed(void);

// NALA_DECLARATION call

void call_mock(int return_value);
void call_mock_once(int return_value);
void call_mock_ignore_in(int return_value);
void call_mock_ignore_in_once(int return_value);
void call_mock_set_errno(int errno_value);
void call_mock_set_callback(void (*callback)(int (*callback)(int value)));
void call_mock_set_callback_in(const void *buf_p, size_t size);
void call_mock_set_callback_in_assert(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size));
void call_mock_set_callback_in_pointer(int (*callback)(int value));
void call_mock_set_callback_out(const void *buf_p, size_t size);
void call_mock_set_callback_out_copy(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size));
void call_mock_none(void);
void call_mock_implementation(int (*implementation)(int (*callback)(int value)));
void call_mock_disable(void);
void call_mock_suspend(void);
void call_mock_resume(void);
void call_mock_reset(void);
void call_mock_assert_completed(void);

// NALA_DECLARATION close

void close_mock(int fd, int return_value);
void close_mock_once(int fd, int return_value);
void close_mock_ignore_in(int return_value);
void close_mock_ignore_in_once(int return_value);
void close_mock_set_errno(int errno_value);
void close_mock_set_callback(void (*callback)(int fd));
void close_mock_ignore_fd_in(void);
void close_mock_none(void);
void close_mock_implementation(int (*implementation)(int fd));
void close_mock_disable(void);
void close_mock_suspend(void);
void close_mock_resume(void);
void close_mock_reset(void);
void close_mock_assert_completed(void);

// NALA_DECLARATION compose_twice

void compose_twice_mock(DummyStruct *return_value);
void compose_twice_mock_once(DummyStruct *return_value);
void compose_twice_mock_ignore_in(DummyStruct *return_value);
void compose_twice_mock_ignore_in_once(DummyStruct *return_value);
void compose_twice_mock_set_errno(int errno_value);
void compose_twice_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)));
void compose_twice_mock_set_dummy_struct_in(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void compose_twice_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct);
void compose_twice_mock_set_dummy_struct_out(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void compose_twice_mock_set_dummy_struct_modifier_in(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_modifier_in_assert(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size));
void compose_twice_mock_set_dummy_struct_modifier_in_pointer(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct));
void compose_twice_mock_set_dummy_struct_modifier_out(const void *buf_p, size_t size);
void compose_twice_mock_set_dummy_struct_modifier_out_copy(void (*callback)(DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct), const void *nala_buf_p, size_t nala_size));
void compose_twice_mock_none(void);
void compose_twice_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)));
void compose_twice_mock_disable(void);
void compose_twice_mock_suspend(void);
void compose_twice_mock_resume(void);
void compose_twice_mock_reset(void);
void compose_twice_mock_assert_completed(void);

// NALA_DECLARATION double_pointer

void double_pointer_mock(int return_value);
void double_pointer_mock_once(int return_value);
void double_pointer_mock_ignore_in(int return_value);
void double_pointer_mock_ignore_in_once(int return_value);
void double_pointer_mock_set_errno(int errno_value);
void double_pointer_mock_set_callback(void (*callback)(int **value_pp));
void double_pointer_mock_set_value_pp_in(const void *buf_p, size_t size);
void double_pointer_mock_set_value_pp_in_assert(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size));
void double_pointer_mock_set_value_pp_in_pointer(int **value_pp);
void double_pointer_mock_set_value_pp_out(const void *buf_p, size_t size);
void double_pointer_mock_set_value_pp_out_copy(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size));
void double_pointer_mock_none(void);
void double_pointer_mock_implementation(int (*implementation)(int **value_pp));
void double_pointer_mock_disable(void);
void double_pointer_mock_suspend(void);
void double_pointer_mock_resume(void);
void double_pointer_mock_reset(void);
void double_pointer_mock_assert_completed(void);

// NALA_DECLARATION dup

void dup_mock(int oldfd, int return_value);
void dup_mock_once(int oldfd, int return_value);
void dup_mock_ignore_in(int return_value);
void dup_mock_ignore_in_once(int return_value);
void dup_mock_set_errno(int errno_value);
void dup_mock_set_callback(void (*callback)(int oldfd));
void dup_mock_ignore_oldfd_in(void);
void dup_mock_none(void);
void dup_mock_implementation(int (*implementation)(int oldfd));
void dup_mock_disable(void);
void dup_mock_suspend(void);
void dup_mock_resume(void);
void dup_mock_reset(void);
void dup_mock_assert_completed(void);

// NALA_DECLARATION dup2

void dup2_mock(int oldfd, int newfd, int return_value);
void dup2_mock_once(int oldfd, int newfd, int return_value);
void dup2_mock_ignore_in(int return_value);
void dup2_mock_ignore_in_once(int return_value);
void dup2_mock_set_errno(int errno_value);
void dup2_mock_set_callback(void (*callback)(int oldfd, int newfd));
void dup2_mock_ignore_oldfd_in(void);
void dup2_mock_ignore_newfd_in(void);
void dup2_mock_none(void);
void dup2_mock_implementation(int (*implementation)(int oldfd, int newfd));
void dup2_mock_disable(void);
void dup2_mock_suspend(void);
void dup2_mock_resume(void);
void dup2_mock_reset(void);
void dup2_mock_assert_completed(void);

// NALA_DECLARATION edit_number

void edit_number_mock(int number, DummyStruct *return_value);
void edit_number_mock_once(int number, DummyStruct *return_value);
void edit_number_mock_ignore_in(DummyStruct *return_value);
void edit_number_mock_ignore_in_once(DummyStruct *return_value);
void edit_number_mock_set_errno(int errno_value);
void edit_number_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, int number));
void edit_number_mock_ignore_number_in(void);
void edit_number_mock_set_dummy_struct_in(const void *buf_p, size_t size);
void edit_number_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct);
void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size);
void edit_number_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void edit_number_mock_none(void);
void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number));
void edit_number_mock_disable(void);
void edit_number_mock_suspend(void);
void edit_number_mock_resume(void);
void edit_number_mock_reset(void);
void edit_number_mock_assert_completed(void);

// NALA_DECLARATION endmntent

void endmntent_mock(int return_value);
void endmntent_mock_once(int return_value);
void endmntent_mock_ignore_in(int return_value);
void endmntent_mock_ignore_in_once(int return_value);
void endmntent_mock_set_errno(int errno_value);
void endmntent_mock_set_callback(void (*callback)(FILE *streamp));
void endmntent_mock_set_streamp_in(const void *buf_p, size_t size);
void endmntent_mock_set_streamp_in_assert(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size));
void endmntent_mock_set_streamp_in_pointer(FILE *streamp);
void endmntent_mock_set_streamp_out(const void *buf_p, size_t size);
void endmntent_mock_set_streamp_out_copy(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size));
void endmntent_mock_none(void);
void endmntent_mock_implementation(int (*implementation)(FILE *streamp));
void endmntent_mock_disable(void);
void endmntent_mock_suspend(void);
void endmntent_mock_resume(void);
void endmntent_mock_reset(void);
void endmntent_mock_assert_completed(void);

// NALA_DECLARATION enum_param

void enum_param_mock(enum enum_param_type value);
void enum_param_mock_once(enum enum_param_type value);
void enum_param_mock_ignore_in(void);
void enum_param_mock_ignore_in_once(void);
void enum_param_mock_set_errno(int errno_value);
void enum_param_mock_set_callback(void (*callback)(enum enum_param_type value));
void enum_param_mock_ignore_value_in(void);
void enum_param_mock_none(void);
void enum_param_mock_implementation(void (*implementation)(enum enum_param_type value));
void enum_param_mock_disable(void);
void enum_param_mock_suspend(void);
void enum_param_mock_resume(void);
void enum_param_mock_reset(void);
void enum_param_mock_assert_completed(void);

// NALA_DECLARATION fclose

void fclose_mock(int return_value);
void fclose_mock_once(int return_value);
void fclose_mock_ignore_in(int return_value);
void fclose_mock_ignore_in_once(int return_value);
void fclose_mock_set_errno(int errno_value);
void fclose_mock_set_callback(void (*callback)(FILE *stream));
void fclose_mock_set_stream_in(const void *buf_p, size_t size);
void fclose_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fclose_mock_set_stream_in_pointer(FILE *stream);
void fclose_mock_set_stream_out(const void *buf_p, size_t size);
void fclose_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fclose_mock_none(void);
void fclose_mock_implementation(int (*implementation)(FILE *stream));
void fclose_mock_disable(void);
void fclose_mock_suspend(void);
void fclose_mock_resume(void);
void fclose_mock_reset(void);
void fclose_mock_assert_completed(void);

// NALA_DECLARATION fflush

void fflush_mock(int return_value);
void fflush_mock_once(int return_value);
void fflush_mock_ignore_in(int return_value);
void fflush_mock_ignore_in_once(int return_value);
void fflush_mock_set_errno(int errno_value);
void fflush_mock_set_callback(void (*callback)(FILE *stream));
void fflush_mock_set_stream_in(const void *buf_p, size_t size);
void fflush_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fflush_mock_set_stream_in_pointer(FILE *stream);
void fflush_mock_set_stream_out(const void *buf_p, size_t size);
void fflush_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fflush_mock_none(void);
void fflush_mock_implementation(int (*implementation)(FILE *stream));
void fflush_mock_disable(void);
void fflush_mock_suspend(void);
void fflush_mock_resume(void);
void fflush_mock_reset(void);
void fflush_mock_assert_completed(void);

// NALA_DECLARATION fileno

void fileno_mock(int return_value);
void fileno_mock_once(int return_value);
void fileno_mock_ignore_in(int return_value);
void fileno_mock_ignore_in_once(int return_value);
void fileno_mock_set_errno(int errno_value);
void fileno_mock_set_callback(void (*callback)(FILE *stream));
void fileno_mock_set_stream_in(const void *buf_p, size_t size);
void fileno_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fileno_mock_set_stream_in_pointer(FILE *stream);
void fileno_mock_set_stream_out(const void *buf_p, size_t size);
void fileno_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fileno_mock_none(void);
void fileno_mock_implementation(int (*implementation)(FILE *stream));
void fileno_mock_disable(void);
void fileno_mock_suspend(void);
void fileno_mock_resume(void);
void fileno_mock_reset(void);
void fileno_mock_assert_completed(void);

// NALA_DECLARATION fopen

void fopen_mock(const char *path, const char *mode, FILE *return_value);
void fopen_mock_once(const char *path, const char *mode, FILE *return_value);
void fopen_mock_ignore_in(FILE *return_value);
void fopen_mock_ignore_in_once(FILE *return_value);
void fopen_mock_set_errno(int errno_value);
void fopen_mock_set_callback(void (*callback)(const char *path, const char *mode));
void fopen_mock_ignore_path_in(void);
void fopen_mock_ignore_mode_in(void);
void fopen_mock_set_path_in(const void *buf_p, size_t size);
void fopen_mock_set_path_in_assert(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size));
void fopen_mock_set_path_in_pointer(const char *path);
void fopen_mock_set_path_out(const void *buf_p, size_t size);
void fopen_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size));
void fopen_mock_set_mode_in(const void *buf_p, size_t size);
void fopen_mock_set_mode_in_assert(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size));
void fopen_mock_set_mode_in_pointer(const char *mode);
void fopen_mock_set_mode_out(const void *buf_p, size_t size);
void fopen_mock_set_mode_out_copy(void (*callback)(const char *mode, const void *nala_buf_p, size_t nala_size));
void fopen_mock_none(void);
void fopen_mock_implementation(FILE *(*implementation)(const char *path, const char *mode));
void fopen_mock_disable(void);
void fopen_mock_suspend(void);
void fopen_mock_resume(void);
void fopen_mock_reset(void);
void fopen_mock_assert_completed(void);

// NALA_DECLARATION fread

void fread_mock(size_t size, size_t nmemb, size_t return_value);
void fread_mock_once(size_t size, size_t nmemb, size_t return_value);
void fread_mock_ignore_in(size_t return_value);
void fread_mock_ignore_in_once(size_t return_value);
void fread_mock_set_errno(int errno_value);
void fread_mock_set_callback(void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream));
void fread_mock_ignore_size_in(void);
void fread_mock_ignore_nmemb_in(void);
void fread_mock_set_ptr_in(const void *buf_p, size_t size);
void fread_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void fread_mock_set_ptr_in_pointer(void *ptr);
void fread_mock_set_ptr_out(const void *buf_p, size_t size);
void fread_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void fread_mock_set_stream_in(const void *buf_p, size_t size);
void fread_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fread_mock_set_stream_in_pointer(FILE *stream);
void fread_mock_set_stream_out(const void *buf_p, size_t size);
void fread_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fread_mock_none(void);
void fread_mock_implementation(size_t (*implementation)(void *ptr, size_t size, size_t nmemb, FILE *stream));
void fread_mock_disable(void);
void fread_mock_suspend(void);
void fread_mock_resume(void);
void fread_mock_reset(void);
void fread_mock_assert_completed(void);

// NALA_DECLARATION free

void free_mock();
void free_mock_once();
void free_mock_ignore_in(void);
void free_mock_ignore_in_once(void);
void free_mock_set_errno(int errno_value);
void free_mock_set_callback(void (*callback)(void *ptr));
void free_mock_set_ptr_in(const void *buf_p, size_t size);
void free_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void free_mock_set_ptr_in_pointer(void *ptr);
void free_mock_set_ptr_out(const void *buf_p, size_t size);
void free_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void free_mock_none(void);
void free_mock_implementation(void (*implementation)(void *ptr));
void free_mock_disable(void);
void free_mock_suspend(void);
void free_mock_resume(void);
void free_mock_reset(void);
void free_mock_assert_completed(void);

// NALA_DECLARATION fseek

void fseek_mock(long int offset, int whence, int return_value);
void fseek_mock_once(long int offset, int whence, int return_value);
void fseek_mock_ignore_in(int return_value);
void fseek_mock_ignore_in_once(int return_value);
void fseek_mock_set_errno(int errno_value);
void fseek_mock_set_callback(void (*callback)(FILE *stream, long int offset, int whence));
void fseek_mock_ignore_offset_in(void);
void fseek_mock_ignore_whence_in(void);
void fseek_mock_set_stream_in(const void *buf_p, size_t size);
void fseek_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fseek_mock_set_stream_in_pointer(FILE *stream);
void fseek_mock_set_stream_out(const void *buf_p, size_t size);
void fseek_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fseek_mock_none(void);
void fseek_mock_implementation(int (*implementation)(FILE *stream, long int offset, int whence));
void fseek_mock_disable(void);
void fseek_mock_suspend(void);
void fseek_mock_resume(void);
void fseek_mock_reset(void);
void fseek_mock_assert_completed(void);

// NALA_DECLARATION ftell

void ftell_mock(long int return_value);
void ftell_mock_once(long int return_value);
void ftell_mock_ignore_in(long int return_value);
void ftell_mock_ignore_in_once(long int return_value);
void ftell_mock_set_errno(int errno_value);
void ftell_mock_set_callback(void (*callback)(FILE *stream));
void ftell_mock_set_stream_in(const void *buf_p, size_t size);
void ftell_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void ftell_mock_set_stream_in_pointer(FILE *stream);
void ftell_mock_set_stream_out(const void *buf_p, size_t size);
void ftell_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void ftell_mock_none(void);
void ftell_mock_implementation(long int (*implementation)(FILE *stream));
void ftell_mock_disable(void);
void ftell_mock_suspend(void);
void ftell_mock_resume(void);
void ftell_mock_reset(void);
void ftell_mock_assert_completed(void);

// NALA_DECLARATION fwrite

void fwrite_mock(size_t size, size_t nmemb, size_t return_value);
void fwrite_mock_once(size_t size, size_t nmemb, size_t return_value);
void fwrite_mock_ignore_in(size_t return_value);
void fwrite_mock_ignore_in_once(size_t return_value);
void fwrite_mock_set_errno(int errno_value);
void fwrite_mock_set_callback(void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream));
void fwrite_mock_ignore_size_in(void);
void fwrite_mock_ignore_nmemb_in(void);
void fwrite_mock_set_ptr_in(const void *buf_p, size_t size);
void fwrite_mock_set_ptr_in_assert(void (*callback)(const void *ptr, const void *nala_buf_p, size_t nala_size));
void fwrite_mock_set_ptr_in_pointer(const void *ptr);
void fwrite_mock_set_ptr_out(const void *buf_p, size_t size);
void fwrite_mock_set_ptr_out_copy(void (*callback)(const void *ptr, const void *nala_buf_p, size_t nala_size));
void fwrite_mock_set_stream_in(const void *buf_p, size_t size);
void fwrite_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fwrite_mock_set_stream_in_pointer(FILE *stream);
void fwrite_mock_set_stream_out(const void *buf_p, size_t size);
void fwrite_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fwrite_mock_none(void);
void fwrite_mock_implementation(size_t (*implementation)(const void *ptr, size_t size, size_t nmemb, FILE *stream));
void fwrite_mock_disable(void);
void fwrite_mock_suspend(void);
void fwrite_mock_resume(void);
void fwrite_mock_reset(void);
void fwrite_mock_assert_completed(void);

// NALA_DECLARATION getmntent

void getmntent_mock(struct mntent *return_value);
void getmntent_mock_once(struct mntent *return_value);
void getmntent_mock_ignore_in(struct mntent *return_value);
void getmntent_mock_ignore_in_once(struct mntent *return_value);
void getmntent_mock_set_errno(int errno_value);
void getmntent_mock_set_callback(void (*callback)(FILE *stream));
void getmntent_mock_set_stream_in(const void *buf_p, size_t size);
void getmntent_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void getmntent_mock_set_stream_in_pointer(FILE *stream);
void getmntent_mock_set_stream_out(const void *buf_p, size_t size);
void getmntent_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void getmntent_mock_none(void);
void getmntent_mock_implementation(struct mntent *(*implementation)(FILE *stream));
void getmntent_mock_disable(void);
void getmntent_mock_suspend(void);
void getmntent_mock_resume(void);
void getmntent_mock_reset(void);
void getmntent_mock_assert_completed(void);

// NALA_DECLARATION in_out

void in_out_mock();
void in_out_mock_once();
void in_out_mock_ignore_in(void);
void in_out_mock_ignore_in_once(void);
void in_out_mock_set_errno(int errno_value);
void in_out_mock_set_callback(void (*callback)(int *buf_p));
void in_out_mock_set_buf_p_in(const void *buf_p, size_t size);
void in_out_mock_set_buf_p_in_assert(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size));
void in_out_mock_set_buf_p_in_pointer(int *buf_p);
void in_out_mock_set_buf_p_out(const void *buf_p, size_t size);
void in_out_mock_set_buf_p_out_copy(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size));
void in_out_mock_none(void);
void in_out_mock_implementation(void (*implementation)(int *buf_p));
void in_out_mock_disable(void);
void in_out_mock_suspend(void);
void in_out_mock_resume(void);
void in_out_mock_reset(void);
void in_out_mock_assert_completed(void);

// NALA_DECLARATION io_control

int io_control_mock_va_arg_real(int kind, va_list __nala_va_list);
void io_control_mock(int kind, int return_value, const char *vafmt_p, ...);
void io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...);
void io_control_mock_ignore_in(int return_value);
void io_control_mock_ignore_in_once(int return_value);
void io_control_mock_set_errno(int errno_value);
void io_control_mock_set_callback(void (*callback)(int kind, va_list __nala_va_list));
void io_control_mock_ignore_kind_in(void);
void io_control_mock_ignore_va_arg_in_at(unsigned int index);
void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size);
void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p);
void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size);
void io_control_mock_none(void);
void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list));
void io_control_mock_disable(void);
void io_control_mock_suspend(void);
void io_control_mock_resume(void);
void io_control_mock_reset(void);
void io_control_mock_assert_completed(void);

// NALA_DECLARATION io_vcontrol

void io_vcontrol_mock(int kind, int return_value);
void io_vcontrol_mock_once(int kind, int return_value);
void io_vcontrol_mock_ignore_in(int return_value);
void io_vcontrol_mock_ignore_in_once(int return_value);
void io_vcontrol_mock_set_errno(int errno_value);
void io_vcontrol_mock_set_callback(void (*callback)(int kind, va_list ap));
void io_vcontrol_mock_ignore_kind_in(void);
void io_vcontrol_mock_none(void);
void io_vcontrol_mock_implementation(int (*implementation)(int kind, va_list ap));
void io_vcontrol_mock_disable(void);
void io_vcontrol_mock_suspend(void);
void io_vcontrol_mock_resume(void);
void io_vcontrol_mock_reset(void);
void io_vcontrol_mock_assert_completed(void);

// NALA_DECLARATION malloc

void malloc_mock(size_t size, void *return_value);
void malloc_mock_once(size_t size, void *return_value);
void malloc_mock_ignore_in(void *return_value);
void malloc_mock_ignore_in_once(void *return_value);
void malloc_mock_set_errno(int errno_value);
void malloc_mock_set_callback(void (*callback)(size_t size));
void malloc_mock_ignore_size_in(void);
void malloc_mock_none(void);
void malloc_mock_implementation(void *(*implementation)(size_t size));
void malloc_mock_disable(void);
void malloc_mock_suspend(void);
void malloc_mock_resume(void);
void malloc_mock_reset(void);
void malloc_mock_assert_completed(void);

// NALA_DECLARATION mount

void mount_mock(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value);
void mount_mock_once(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value);
void mount_mock_ignore_in(int return_value);
void mount_mock_ignore_in_once(int return_value);
void mount_mock_set_errno(int errno_value);
void mount_mock_set_callback(void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data));
void mount_mock_ignore_source_in(void);
void mount_mock_ignore_target_in(void);
void mount_mock_ignore_filesystemtype_in(void);
void mount_mock_ignore_mountflags_in(void);
void mount_mock_set_source_in(const void *buf_p, size_t size);
void mount_mock_set_source_in_assert(void (*callback)(const char *source, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_source_in_pointer(const char *source);
void mount_mock_set_source_out(const void *buf_p, size_t size);
void mount_mock_set_source_out_copy(void (*callback)(const char *source, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_target_in(const void *buf_p, size_t size);
void mount_mock_set_target_in_assert(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_target_in_pointer(const char *target);
void mount_mock_set_target_out(const void *buf_p, size_t size);
void mount_mock_set_target_out_copy(void (*callback)(const char *target, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_filesystemtype_in(const void *buf_p, size_t size);
void mount_mock_set_filesystemtype_in_assert(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_filesystemtype_in_pointer(const char *filesystemtype);
void mount_mock_set_filesystemtype_out(const void *buf_p, size_t size);
void mount_mock_set_filesystemtype_out_copy(void (*callback)(const char *filesystemtype, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_data_in(const void *buf_p, size_t size);
void mount_mock_set_data_in_assert(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size));
void mount_mock_set_data_in_pointer(const void *data);
void mount_mock_set_data_out(const void *buf_p, size_t size);
void mount_mock_set_data_out_copy(void (*callback)(const void *data, const void *nala_buf_p, size_t nala_size));
void mount_mock_none(void);
void mount_mock_implementation(int (*implementation)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data));
void mount_mock_disable(void);
void mount_mock_suspend(void);
void mount_mock_resume(void);
void mount_mock_reset(void);
void mount_mock_assert_completed(void);

// NALA_DECLARATION output_message

void output_message_mock(const char *message);
void output_message_mock_once(const char *message);
void output_message_mock_ignore_in(void);
void output_message_mock_ignore_in_once(void);
void output_message_mock_set_errno(int errno_value);
void output_message_mock_set_callback(void (*callback)(const char *message));
void output_message_mock_ignore_message_in(void);
void output_message_mock_set_message_in(const void *buf_p, size_t size);
void output_message_mock_set_message_in_assert(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size));
void output_message_mock_set_message_in_pointer(const char *message);
void output_message_mock_set_message_out(const void *buf_p, size_t size);
void output_message_mock_set_message_out_copy(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size));
void output_message_mock_none(void);
void output_message_mock_implementation(void (*implementation)(const char *message));
void output_message_mock_disable(void);
void output_message_mock_suspend(void);
void output_message_mock_resume(void);
void output_message_mock_reset(void);
void output_message_mock_assert_completed(void);

// NALA_DECLARATION pipe

void pipe_mock(int return_value);
void pipe_mock_once(int return_value);
void pipe_mock_ignore_in(int return_value);
void pipe_mock_ignore_in_once(int return_value);
void pipe_mock_set_errno(int errno_value);
void pipe_mock_set_callback(void (*callback)(int pipefd[2]));
void pipe_mock_set_pipefd_in(const void *buf_p, size_t size);
void pipe_mock_set_pipefd_in_assert(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size));
void pipe_mock_set_pipefd_in_pointer(int pipefd[2]);
void pipe_mock_set_pipefd_out(const void *buf_p, size_t size);
void pipe_mock_set_pipefd_out_copy(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size));
void pipe_mock_none(void);
void pipe_mock_implementation(int (*implementation)(int pipefd[2]));
void pipe_mock_disable(void);
void pipe_mock_suspend(void);
void pipe_mock_resume(void);
void pipe_mock_reset(void);
void pipe_mock_assert_completed(void);

// NALA_DECLARATION poll

void poll_mock(nfds_t nfds, int timeout, int return_value);
void poll_mock_once(nfds_t nfds, int timeout, int return_value);
void poll_mock_ignore_in(int return_value);
void poll_mock_ignore_in_once(int return_value);
void poll_mock_set_errno(int errno_value);
void poll_mock_set_callback(void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout));
void poll_mock_ignore_nfds_in(void);
void poll_mock_ignore_timeout_in(void);
void poll_mock_set_fds_in(const void *buf_p, size_t size);
void poll_mock_set_fds_in_assert(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size));
void poll_mock_set_fds_in_pointer(struct pollfd *fds);
void poll_mock_set_fds_out(const void *buf_p, size_t size);
void poll_mock_set_fds_out_copy(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size));
void poll_mock_none(void);
void poll_mock_implementation(int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout));
void poll_mock_disable(void);
void poll_mock_suspend(void);
void poll_mock_resume(void);
void poll_mock_reset(void);
void poll_mock_assert_completed(void);

// NALA_DECLARATION print_hello

void print_hello_mock();
void print_hello_mock_once();
void print_hello_mock_ignore_in(void);
void print_hello_mock_ignore_in_once(void);
void print_hello_mock_set_errno(int errno_value);
void print_hello_mock_set_callback(void (*callback)());
void print_hello_mock_none(void);
void print_hello_mock_implementation(void (*implementation)());
void print_hello_mock_disable(void);
void print_hello_mock_suspend(void);
void print_hello_mock_resume(void);
void print_hello_mock_reset(void);
void print_hello_mock_assert_completed(void);

// NALA_DECLARATION read

void read_mock(int fd, size_t count, ssize_t return_value);
void read_mock_once(int fd, size_t count, ssize_t return_value);
void read_mock_ignore_in(ssize_t return_value);
void read_mock_ignore_in_once(ssize_t return_value);
void read_mock_set_errno(int errno_value);
void read_mock_set_callback(void (*callback)(int fd, void *buf, size_t count));
void read_mock_ignore_fd_in(void);
void read_mock_ignore_count_in(void);
void read_mock_set_buf_in(const void *buf_p, size_t size);
void read_mock_set_buf_in_assert(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size));
void read_mock_set_buf_in_pointer(void *buf);
void read_mock_set_buf_out(const void *buf_p, size_t size);
void read_mock_set_buf_out_copy(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size));
void read_mock_none(void);
void read_mock_implementation(ssize_t (*implementation)(int fd, void *buf, size_t count));
void read_mock_disable(void);
void read_mock_suspend(void);
void read_mock_resume(void);
void read_mock_reset(void);
void read_mock_assert_completed(void);

// NALA_DECLARATION sendto

void sendto_mock(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value);
void sendto_mock_once(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value);
void sendto_mock_ignore_in(ssize_t return_value);
void sendto_mock_ignore_in_once(ssize_t return_value);
void sendto_mock_set_errno(int errno_value);
void sendto_mock_set_callback(void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen));
void sendto_mock_ignore_sockfd_in(void);
void sendto_mock_ignore_len_in(void);
void sendto_mock_ignore_flags_in(void);
void sendto_mock_ignore_addrlen_in(void);
void sendto_mock_set_buf_in(const void *buf_p, size_t size);
void sendto_mock_set_buf_in_assert(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void sendto_mock_set_buf_in_pointer(const void *buf);
void sendto_mock_set_buf_out(const void *buf_p, size_t size);
void sendto_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void sendto_mock_set_dest_addr_in(const void *buf_p, size_t size);
void sendto_mock_set_dest_addr_in_assert(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size));
void sendto_mock_set_dest_addr_in_pointer(const struct sockaddr *dest_addr);
void sendto_mock_set_dest_addr_out(const void *buf_p, size_t size);
void sendto_mock_set_dest_addr_out_copy(void (*callback)(const struct sockaddr *dest_addr, const void *nala_buf_p, size_t nala_size));
void sendto_mock_none(void);
void sendto_mock_implementation(ssize_t (*implementation)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen));
void sendto_mock_disable(void);
void sendto_mock_suspend(void);
void sendto_mock_resume(void);
void sendto_mock_reset(void);
void sendto_mock_assert_completed(void);

// NALA_DECLARATION setsockopt

void setsockopt_mock(int sockfd, int level, int optname, socklen_t optlen, int return_value);
void setsockopt_mock_once(int sockfd, int level, int optname, socklen_t optlen, int return_value);
void setsockopt_mock_ignore_in(int return_value);
void setsockopt_mock_ignore_in_once(int return_value);
void setsockopt_mock_set_errno(int errno_value);
void setsockopt_mock_set_callback(void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen));
void setsockopt_mock_ignore_sockfd_in(void);
void setsockopt_mock_ignore_level_in(void);
void setsockopt_mock_ignore_optname_in(void);
void setsockopt_mock_ignore_optlen_in(void);
void setsockopt_mock_set_optval_in(const void *buf_p, size_t size);
void setsockopt_mock_set_optval_in_assert(void (*callback)(const void *optval, const void *nala_buf_p, size_t nala_size));
void setsockopt_mock_set_optval_in_pointer(const void *optval);
void setsockopt_mock_set_optval_out(const void *buf_p, size_t size);
void setsockopt_mock_set_optval_out_copy(void (*callback)(const void *optval, const void *nala_buf_p, size_t nala_size));
void setsockopt_mock_none(void);
void setsockopt_mock_implementation(int (*implementation)(int sockfd, int level, int optname, const void *optval, socklen_t optlen));
void setsockopt_mock_disable(void);
void setsockopt_mock_suspend(void);
void setsockopt_mock_resume(void);
void setsockopt_mock_reset(void);
void setsockopt_mock_assert_completed(void);

// NALA_DECLARATION sleep

void sleep_mock(unsigned int seconds, unsigned int return_value);
void sleep_mock_once(unsigned int seconds, unsigned int return_value);
void sleep_mock_ignore_in(unsigned int return_value);
void sleep_mock_ignore_in_once(unsigned int return_value);
void sleep_mock_set_errno(int errno_value);
void sleep_mock_set_callback(void (*callback)(unsigned int seconds));
void sleep_mock_ignore_seconds_in(void);
void sleep_mock_none(void);
void sleep_mock_implementation(unsigned int (*implementation)(unsigned int seconds));
void sleep_mock_disable(void);
void sleep_mock_suspend(void);
void sleep_mock_resume(void);
void sleep_mock_reset(void);
void sleep_mock_assert_completed(void);

// NALA_DECLARATION statvfs

void statvfs_mock(const char *path, int return_value);
void statvfs_mock_once(const char *path, int return_value);
void statvfs_mock_ignore_in(int return_value);
void statvfs_mock_ignore_in_once(int return_value);
void statvfs_mock_set_errno(int errno_value);
void statvfs_mock_set_callback(void (*callback)(const char *path, struct statvfs *buf));
void statvfs_mock_ignore_path_in(void);
void statvfs_mock_set_path_in(const void *buf_p, size_t size);
void statvfs_mock_set_path_in_assert(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size));
void statvfs_mock_set_path_in_pointer(const char *path);
void statvfs_mock_set_path_out(const void *buf_p, size_t size);
void statvfs_mock_set_path_out_copy(void (*callback)(const char *path, const void *nala_buf_p, size_t nala_size));
void statvfs_mock_set_buf_in(const void *buf_p, size_t size);
void statvfs_mock_set_buf_in_assert(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size));
void statvfs_mock_set_buf_in_pointer(struct statvfs *buf);
void statvfs_mock_set_buf_out(const void *buf_p, size_t size);
void statvfs_mock_set_buf_out_copy(void (*callback)(struct statvfs *buf, const void *nala_buf_p, size_t nala_size));
void statvfs_mock_none(void);
void statvfs_mock_implementation(int (*implementation)(const char *path, struct statvfs *buf));
void statvfs_mock_disable(void);
void statvfs_mock_suspend(void);
void statvfs_mock_resume(void);
void statvfs_mock_reset(void);
void statvfs_mock_assert_completed(void);

// NALA_DECLARATION struct_param

void struct_param_mock();
void struct_param_mock_once();
void struct_param_mock_ignore_in(void);
void struct_param_mock_ignore_in_once(void);
void struct_param_mock_set_errno(int errno_value);
void struct_param_mock_set_callback(void (*callback)(struct struct_param_type *data));
void struct_param_mock_set_data_in(const void *buf_p, size_t size);
void struct_param_mock_set_data_in_assert(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size));
void struct_param_mock_set_data_in_pointer(struct struct_param_type *data);
void struct_param_mock_set_data_out(const void *buf_p, size_t size);
void struct_param_mock_set_data_out_copy(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size));
void struct_param_mock_none(void);
void struct_param_mock_implementation(void (*implementation)(struct struct_param_type *data));
void struct_param_mock_disable(void);
void struct_param_mock_suspend(void);
void struct_param_mock_resume(void);
void struct_param_mock_reset(void);
void struct_param_mock_assert_completed(void);

// NALA_DECLARATION struct_param_and_return_type

void struct_param_and_return_type_mock(struct struct_param_type return_value);
void struct_param_and_return_type_mock_once(struct struct_param_type return_value);
void struct_param_and_return_type_mock_ignore_in(struct struct_param_type return_value);
void struct_param_and_return_type_mock_ignore_in_once(struct struct_param_type return_value);
void struct_param_and_return_type_mock_set_errno(int errno_value);
void struct_param_and_return_type_mock_set_callback(void (*callback)(struct struct_param_type arg));
void struct_param_and_return_type_mock_none(void);
void struct_param_and_return_type_mock_implementation(struct struct_param_type (*implementation)(struct struct_param_type arg));
void struct_param_and_return_type_mock_disable(void);
void struct_param_and_return_type_mock_suspend(void);
void struct_param_and_return_type_mock_resume(void);
void struct_param_and_return_type_mock_reset(void);
void struct_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION time

void time_mock(time_t return_value);
void time_mock_once(time_t return_value);
void time_mock_ignore_in(time_t return_value);
void time_mock_ignore_in_once(time_t return_value);
void time_mock_set_errno(int errno_value);
void time_mock_set_callback(void (*callback)(time_t *tloc));
void time_mock_set_tloc_in(const void *buf_p, size_t size);
void time_mock_set_tloc_in_assert(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size));
void time_mock_set_tloc_in_pointer(time_t *tloc);
void time_mock_set_tloc_out(const void *buf_p, size_t size);
void time_mock_set_tloc_out_copy(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size));
void time_mock_none(void);
void time_mock_implementation(time_t (*implementation)(time_t *tloc));
void time_mock_disable(void);
void time_mock_suspend(void);
void time_mock_resume(void);
void time_mock_reset(void);
void time_mock_assert_completed(void);

// NALA_DECLARATION timerfd_settime

void timerfd_settime_mock(int fd, int flags, int return_value);
void timerfd_settime_mock_once(int fd, int flags, int return_value);
void timerfd_settime_mock_ignore_in(int return_value);
void timerfd_settime_mock_ignore_in_once(int return_value);
void timerfd_settime_mock_set_errno(int errno_value);
void timerfd_settime_mock_set_callback(void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value));
void timerfd_settime_mock_ignore_fd_in(void);
void timerfd_settime_mock_ignore_flags_in(void);
void timerfd_settime_mock_set_new_value_in(const void *buf_p, size_t size);
void timerfd_settime_mock_set_new_value_in_assert(void (*callback)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size));
void timerfd_settime_mock_set_new_value_in_pointer(const struct itimerspec *new_value);
void timerfd_settime_mock_set_new_value_out(const void *buf_p, size_t size);
void timerfd_settime_mock_set_new_value_out_copy(void (*callback)(const struct itimerspec *new_value, const void *nala_buf_p, size_t nala_size));
void timerfd_settime_mock_set_old_value_in(const void *buf_p, size_t size);
void timerfd_settime_mock_set_old_value_in_assert(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size));
void timerfd_settime_mock_set_old_value_in_pointer(struct itimerspec *old_value);
void timerfd_settime_mock_set_old_value_out(const void *buf_p, size_t size);
void timerfd_settime_mock_set_old_value_out_copy(void (*callback)(struct itimerspec *old_value, const void *nala_buf_p, size_t nala_size));
void timerfd_settime_mock_none(void);
void timerfd_settime_mock_implementation(int (*implementation)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value));
void timerfd_settime_mock_disable(void);
void timerfd_settime_mock_suspend(void);
void timerfd_settime_mock_resume(void);
void timerfd_settime_mock_reset(void);
void timerfd_settime_mock_assert_completed(void);

// NALA_DECLARATION tmpfile

void tmpfile_mock(FILE *return_value);
void tmpfile_mock_once(FILE *return_value);
void tmpfile_mock_ignore_in(FILE *return_value);
void tmpfile_mock_ignore_in_once(FILE *return_value);
void tmpfile_mock_set_errno(int errno_value);
void tmpfile_mock_set_callback(void (*callback)(void));
void tmpfile_mock_none(void);
void tmpfile_mock_implementation(FILE *(*implementation)(void));
void tmpfile_mock_disable(void);
void tmpfile_mock_suspend(void);
void tmpfile_mock_resume(void);
void tmpfile_mock_reset(void);
void tmpfile_mock_assert_completed(void);

// NALA_DECLARATION typedef_struct_param_and_return_type

void typedef_struct_param_and_return_type_mock(struct_param_type return_value);
void typedef_struct_param_and_return_type_mock_once(struct_param_type return_value);
void typedef_struct_param_and_return_type_mock_ignore_in(struct_param_type return_value);
void typedef_struct_param_and_return_type_mock_ignore_in_once(struct_param_type return_value);
void typedef_struct_param_and_return_type_mock_set_errno(int errno_value);
void typedef_struct_param_and_return_type_mock_set_callback(void (*callback)(struct_param_type arg));
void typedef_struct_param_and_return_type_mock_none(void);
void typedef_struct_param_and_return_type_mock_implementation(struct_param_type (*implementation)(struct_param_type arg));
void typedef_struct_param_and_return_type_mock_disable(void);
void typedef_struct_param_and_return_type_mock_suspend(void);
void typedef_struct_param_and_return_type_mock_resume(void);
void typedef_struct_param_and_return_type_mock_reset(void);
void typedef_struct_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION typedef_union_param_and_return_type

void typedef_union_param_and_return_type_mock(union_type return_value);
void typedef_union_param_and_return_type_mock_once(union_type return_value);
void typedef_union_param_and_return_type_mock_ignore_in(union_type return_value);
void typedef_union_param_and_return_type_mock_ignore_in_once(union_type return_value);
void typedef_union_param_and_return_type_mock_set_errno(int errno_value);
void typedef_union_param_and_return_type_mock_set_callback(void (*callback)(union_type arg));
void typedef_union_param_and_return_type_mock_none(void);
void typedef_union_param_and_return_type_mock_implementation(union_type (*implementation)(union_type arg));
void typedef_union_param_and_return_type_mock_disable(void);
void typedef_union_param_and_return_type_mock_suspend(void);
void typedef_union_param_and_return_type_mock_resume(void);
void typedef_union_param_and_return_type_mock_reset(void);
void typedef_union_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION union_param_and_return_type

void union_param_and_return_type_mock(union union_type return_value);
void union_param_and_return_type_mock_once(union union_type return_value);
void union_param_and_return_type_mock_ignore_in(union union_type return_value);
void union_param_and_return_type_mock_ignore_in_once(union union_type return_value);
void union_param_and_return_type_mock_set_errno(int errno_value);
void union_param_and_return_type_mock_set_callback(void (*callback)(union union_type arg));
void union_param_and_return_type_mock_none(void);
void union_param_and_return_type_mock_implementation(union union_type (*implementation)(union union_type arg));
void union_param_and_return_type_mock_disable(void);
void union_param_and_return_type_mock_suspend(void);
void union_param_and_return_type_mock_resume(void);
void union_param_and_return_type_mock_reset(void);
void union_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION usleep

void usleep_mock(__useconds_t usec, int return_value);
void usleep_mock_once(__useconds_t usec, int return_value);
void usleep_mock_ignore_in(int return_value);
void usleep_mock_ignore_in_once(int return_value);
void usleep_mock_set_errno(int errno_value);
void usleep_mock_set_callback(void (*callback)(__useconds_t usec));
void usleep_mock_ignore_usec_in(void);
void usleep_mock_none(void);
void usleep_mock_implementation(int (*implementation)(__useconds_t usec));
void usleep_mock_disable(void);
void usleep_mock_suspend(void);
void usleep_mock_resume(void);
void usleep_mock_reset(void);
void usleep_mock_assert_completed(void);

// NALA_DECLARATION write

void write_mock(int fd, size_t count, ssize_t return_value);
void write_mock_once(int fd, size_t count, ssize_t return_value);
void write_mock_ignore_in(ssize_t return_value);
void write_mock_ignore_in_once(ssize_t return_value);
void write_mock_set_errno(int errno_value);
void write_mock_set_callback(void (*callback)(int fd, const void *buf, size_t count));
void write_mock_ignore_fd_in(void);
void write_mock_ignore_count_in(void);
void write_mock_set_buf_in(const void *buf_p, size_t size);
void write_mock_set_buf_in_assert(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void write_mock_set_buf_in_pointer(const void *buf);
void write_mock_set_buf_out(const void *buf_p, size_t size);
void write_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void write_mock_none(void);
void write_mock_implementation(ssize_t (*implementation)(int fd, const void *buf, size_t count));
void write_mock_disable(void);
void write_mock_suspend(void);
void write_mock_resume(void);
void write_mock_reset(void);
void write_mock_assert_completed(void);

