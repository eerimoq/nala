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

typedef void (*nala_mock_assert_in_t)(
    struct nala_traceback_t *traceback_p,
    const char *func_p,
    const char *param_p,
    const void *left_p,
    const void *right_p,
    size_t size);

void nala_suspend_all_mocks(void);
void nala_resume_all_mocks(void);
void nala_reset_all_mocks(void);
void nala_assert_all_mocks_completed(void);

// NALA_DECLARATION add

struct nala_add_params_t {
    int x;
    int y;
};

int add_mock(int x, int y, int return_value);
int add_mock_once(int x, int y, int return_value);
int add_mock_ignore_in(int return_value);
int add_mock_ignore_in_once(int return_value);
void add_mock_set_errno(int errno_value);
void add_mock_set_callback(void (*callback)(int x, int y));
struct nala_add_params_t *add_mock_get_params_in(int handle);
void add_mock_ignore_x_in(void);
void add_mock_ignore_y_in(void);
void add_mock_none(void);
void add_mock_implementation(int (*implementation)(int x, int y));
void add_mock_real(void);
void add_mock_real_once(void);
void add_mock_suspend(void);
void add_mock_resume(void);
void add_mock_reset(void);
void add_mock_assert_completed(void);

// NALA_DECLARATION call

struct nala_call_params_t {
    int (*callback)(int value);
};

int call_mock(int return_value);
int call_mock_once(int return_value);
int call_mock_ignore_in(int return_value);
int call_mock_ignore_in_once(int return_value);
void call_mock_set_errno(int errno_value);
void call_mock_set_callback(void (*callback)(int (*callback)(int value)));
struct nala_call_params_t *call_mock_get_params_in(int handle);
void call_mock_set_callback_in(const void *buf_p, size_t size);
void call_mock_set_callback_in_assert(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size));
void call_mock_set_callback_in_pointer(int (*callback)(int value));
void call_mock_set_callback_out(const void *buf_p, size_t size);
void call_mock_set_callback_out_copy(void (*callback)(int (*callback)(int value), const void *nala_buf_p, size_t nala_size));
void call_mock_none(void);
void call_mock_implementation(int (*implementation)(int (*callback)(int value)));
void call_mock_real(void);
void call_mock_real_once(void);
void call_mock_suspend(void);
void call_mock_resume(void);
void call_mock_reset(void);
void call_mock_assert_completed(void);

// NALA_DECLARATION close

struct nala_close_params_t {
    int fd;
};

int close_mock(int fd, int return_value);
int close_mock_once(int fd, int return_value);
int close_mock_ignore_in(int return_value);
int close_mock_ignore_in_once(int return_value);
void close_mock_set_errno(int errno_value);
void close_mock_set_callback(void (*callback)(int fd));
struct nala_close_params_t *close_mock_get_params_in(int handle);
void close_mock_ignore_fd_in(void);
void close_mock_none(void);
void close_mock_implementation(int (*implementation)(int fd));
void close_mock_real(void);
void close_mock_real_once(void);
void close_mock_suspend(void);
void close_mock_resume(void);
void close_mock_reset(void);
void close_mock_assert_completed(void);

// NALA_DECLARATION compose_twice

struct nala_compose_twice_params_t {
    DummyStruct *dummy_struct;
    DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct);
};

int compose_twice_mock(DummyStruct *return_value);
int compose_twice_mock_once(DummyStruct *return_value);
int compose_twice_mock_ignore_in(DummyStruct *return_value);
int compose_twice_mock_ignore_in_once(DummyStruct *return_value);
void compose_twice_mock_set_errno(int errno_value);
void compose_twice_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, DummyStruct *(*dummy_struct_modifier)(DummyStruct *dummy_struct)));
struct nala_compose_twice_params_t *compose_twice_mock_get_params_in(int handle);
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
void compose_twice_mock_real(void);
void compose_twice_mock_real_once(void);
void compose_twice_mock_suspend(void);
void compose_twice_mock_resume(void);
void compose_twice_mock_reset(void);
void compose_twice_mock_assert_completed(void);

// NALA_DECLARATION double_pointer

struct nala_double_pointer_params_t {
    int **value_pp;
};

int double_pointer_mock(int return_value);
int double_pointer_mock_once(int return_value);
int double_pointer_mock_ignore_in(int return_value);
int double_pointer_mock_ignore_in_once(int return_value);
void double_pointer_mock_set_errno(int errno_value);
void double_pointer_mock_set_callback(void (*callback)(int **value_pp));
struct nala_double_pointer_params_t *double_pointer_mock_get_params_in(int handle);
void double_pointer_mock_set_value_pp_in(const void *buf_p, size_t size);
void double_pointer_mock_set_value_pp_in_assert(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size));
void double_pointer_mock_set_value_pp_in_pointer(int **value_pp);
void double_pointer_mock_set_value_pp_out(const void *buf_p, size_t size);
void double_pointer_mock_set_value_pp_out_copy(void (*callback)(int **value_pp, const void *nala_buf_p, size_t nala_size));
void double_pointer_mock_none(void);
void double_pointer_mock_implementation(int (*implementation)(int **value_pp));
void double_pointer_mock_real(void);
void double_pointer_mock_real_once(void);
void double_pointer_mock_suspend(void);
void double_pointer_mock_resume(void);
void double_pointer_mock_reset(void);
void double_pointer_mock_assert_completed(void);

// NALA_DECLARATION dup

struct nala_dup_params_t {
    int oldfd;
};

int dup_mock(int oldfd, int return_value);
int dup_mock_once(int oldfd, int return_value);
int dup_mock_ignore_in(int return_value);
int dup_mock_ignore_in_once(int return_value);
void dup_mock_set_errno(int errno_value);
void dup_mock_set_callback(void (*callback)(int oldfd));
struct nala_dup_params_t *dup_mock_get_params_in(int handle);
void dup_mock_ignore_oldfd_in(void);
void dup_mock_none(void);
void dup_mock_implementation(int (*implementation)(int oldfd));
void dup_mock_real(void);
void dup_mock_real_once(void);
void dup_mock_suspend(void);
void dup_mock_resume(void);
void dup_mock_reset(void);
void dup_mock_assert_completed(void);

// NALA_DECLARATION dup2

struct nala_dup2_params_t {
    int oldfd;
    int newfd;
};

int dup2_mock(int oldfd, int newfd, int return_value);
int dup2_mock_once(int oldfd, int newfd, int return_value);
int dup2_mock_ignore_in(int return_value);
int dup2_mock_ignore_in_once(int return_value);
void dup2_mock_set_errno(int errno_value);
void dup2_mock_set_callback(void (*callback)(int oldfd, int newfd));
struct nala_dup2_params_t *dup2_mock_get_params_in(int handle);
void dup2_mock_ignore_oldfd_in(void);
void dup2_mock_ignore_newfd_in(void);
void dup2_mock_none(void);
void dup2_mock_implementation(int (*implementation)(int oldfd, int newfd));
void dup2_mock_real(void);
void dup2_mock_real_once(void);
void dup2_mock_suspend(void);
void dup2_mock_resume(void);
void dup2_mock_reset(void);
void dup2_mock_assert_completed(void);

// NALA_DECLARATION edit_number

struct nala_edit_number_params_t {
    DummyStruct *dummy_struct;
    int number;
};

int edit_number_mock(int number, DummyStruct *return_value);
int edit_number_mock_once(int number, DummyStruct *return_value);
int edit_number_mock_ignore_in(DummyStruct *return_value);
int edit_number_mock_ignore_in_once(DummyStruct *return_value);
void edit_number_mock_set_errno(int errno_value);
void edit_number_mock_set_callback(void (*callback)(DummyStruct *dummy_struct, int number));
struct nala_edit_number_params_t *edit_number_mock_get_params_in(int handle);
void edit_number_mock_ignore_number_in(void);
void edit_number_mock_set_dummy_struct_in(const void *buf_p, size_t size);
void edit_number_mock_set_dummy_struct_in_assert(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void edit_number_mock_set_dummy_struct_in_pointer(DummyStruct *dummy_struct);
void edit_number_mock_set_dummy_struct_out(const void *buf_p, size_t size);
void edit_number_mock_set_dummy_struct_out_copy(void (*callback)(DummyStruct *dummy_struct, const void *nala_buf_p, size_t nala_size));
void edit_number_mock_none(void);
void edit_number_mock_implementation(DummyStruct *(*implementation)(DummyStruct *dummy_struct, int number));
void edit_number_mock_real(void);
void edit_number_mock_real_once(void);
void edit_number_mock_suspend(void);
void edit_number_mock_resume(void);
void edit_number_mock_reset(void);
void edit_number_mock_assert_completed(void);

// NALA_DECLARATION endmntent

struct nala_endmntent_params_t {
    FILE *streamp;
};

int endmntent_mock(int return_value);
int endmntent_mock_once(int return_value);
int endmntent_mock_ignore_in(int return_value);
int endmntent_mock_ignore_in_once(int return_value);
void endmntent_mock_set_errno(int errno_value);
void endmntent_mock_set_callback(void (*callback)(FILE *streamp));
struct nala_endmntent_params_t *endmntent_mock_get_params_in(int handle);
void endmntent_mock_set_streamp_in(const void *buf_p, size_t size);
void endmntent_mock_set_streamp_in_assert(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size));
void endmntent_mock_set_streamp_in_pointer(FILE *streamp);
void endmntent_mock_set_streamp_out(const void *buf_p, size_t size);
void endmntent_mock_set_streamp_out_copy(void (*callback)(FILE *streamp, const void *nala_buf_p, size_t nala_size));
void endmntent_mock_none(void);
void endmntent_mock_implementation(int (*implementation)(FILE *streamp));
void endmntent_mock_real(void);
void endmntent_mock_real_once(void);
void endmntent_mock_suspend(void);
void endmntent_mock_resume(void);
void endmntent_mock_reset(void);
void endmntent_mock_assert_completed(void);

// NALA_DECLARATION enum_param

struct nala_enum_param_params_t {
    enum enum_param_type value;
};

int enum_param_mock(enum enum_param_type value);
int enum_param_mock_once(enum enum_param_type value);
int enum_param_mock_ignore_in(void);
int enum_param_mock_ignore_in_once(void);
void enum_param_mock_set_errno(int errno_value);
void enum_param_mock_set_callback(void (*callback)(enum enum_param_type value));
struct nala_enum_param_params_t *enum_param_mock_get_params_in(int handle);
void enum_param_mock_ignore_value_in(void);
void enum_param_mock_none(void);
void enum_param_mock_implementation(void (*implementation)(enum enum_param_type value));
void enum_param_mock_real(void);
void enum_param_mock_real_once(void);
void enum_param_mock_suspend(void);
void enum_param_mock_resume(void);
void enum_param_mock_reset(void);
void enum_param_mock_assert_completed(void);

// NALA_DECLARATION fclose

struct nala_fclose_params_t {
    FILE *stream;
};

int fclose_mock(int return_value);
int fclose_mock_once(int return_value);
int fclose_mock_ignore_in(int return_value);
int fclose_mock_ignore_in_once(int return_value);
void fclose_mock_set_errno(int errno_value);
void fclose_mock_set_callback(void (*callback)(FILE *stream));
struct nala_fclose_params_t *fclose_mock_get_params_in(int handle);
void fclose_mock_set_stream_in(const void *buf_p, size_t size);
void fclose_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fclose_mock_set_stream_in_pointer(FILE *stream);
void fclose_mock_set_stream_out(const void *buf_p, size_t size);
void fclose_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fclose_mock_none(void);
void fclose_mock_implementation(int (*implementation)(FILE *stream));
void fclose_mock_real(void);
void fclose_mock_real_once(void);
void fclose_mock_suspend(void);
void fclose_mock_resume(void);
void fclose_mock_reset(void);
void fclose_mock_assert_completed(void);

// NALA_DECLARATION fflush

struct nala_fflush_params_t {
    FILE *stream;
};

int fflush_mock(int return_value);
int fflush_mock_once(int return_value);
int fflush_mock_ignore_in(int return_value);
int fflush_mock_ignore_in_once(int return_value);
void fflush_mock_set_errno(int errno_value);
void fflush_mock_set_callback(void (*callback)(FILE *stream));
struct nala_fflush_params_t *fflush_mock_get_params_in(int handle);
void fflush_mock_set_stream_in(const void *buf_p, size_t size);
void fflush_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fflush_mock_set_stream_in_pointer(FILE *stream);
void fflush_mock_set_stream_out(const void *buf_p, size_t size);
void fflush_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fflush_mock_none(void);
void fflush_mock_implementation(int (*implementation)(FILE *stream));
void fflush_mock_real(void);
void fflush_mock_real_once(void);
void fflush_mock_suspend(void);
void fflush_mock_resume(void);
void fflush_mock_reset(void);
void fflush_mock_assert_completed(void);

// NALA_DECLARATION fileno

struct nala_fileno_params_t {
    FILE *stream;
};

int fileno_mock(int return_value);
int fileno_mock_once(int return_value);
int fileno_mock_ignore_in(int return_value);
int fileno_mock_ignore_in_once(int return_value);
void fileno_mock_set_errno(int errno_value);
void fileno_mock_set_callback(void (*callback)(FILE *stream));
struct nala_fileno_params_t *fileno_mock_get_params_in(int handle);
void fileno_mock_set_stream_in(const void *buf_p, size_t size);
void fileno_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fileno_mock_set_stream_in_pointer(FILE *stream);
void fileno_mock_set_stream_out(const void *buf_p, size_t size);
void fileno_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fileno_mock_none(void);
void fileno_mock_implementation(int (*implementation)(FILE *stream));
void fileno_mock_real(void);
void fileno_mock_real_once(void);
void fileno_mock_suspend(void);
void fileno_mock_resume(void);
void fileno_mock_reset(void);
void fileno_mock_assert_completed(void);

// NALA_DECLARATION fopen

struct nala_fopen_params_t {
    const char *path;
    const char *mode;
};

int fopen_mock(const char *path, const char *mode, FILE *return_value);
int fopen_mock_once(const char *path, const char *mode, FILE *return_value);
int fopen_mock_ignore_in(FILE *return_value);
int fopen_mock_ignore_in_once(FILE *return_value);
void fopen_mock_set_errno(int errno_value);
void fopen_mock_set_callback(void (*callback)(const char *path, const char *mode));
struct nala_fopen_params_t *fopen_mock_get_params_in(int handle);
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
void fopen_mock_real(void);
void fopen_mock_real_once(void);
void fopen_mock_suspend(void);
void fopen_mock_resume(void);
void fopen_mock_reset(void);
void fopen_mock_assert_completed(void);

// NALA_DECLARATION fread

struct nala_fread_params_t {
    void *ptr;
    size_t size;
    size_t nmemb;
    FILE *stream;
};

int fread_mock(size_t size, size_t nmemb, size_t return_value);
int fread_mock_once(size_t size, size_t nmemb, size_t return_value);
int fread_mock_ignore_in(size_t return_value);
int fread_mock_ignore_in_once(size_t return_value);
void fread_mock_set_errno(int errno_value);
void fread_mock_set_callback(void (*callback)(void *ptr, size_t size, size_t nmemb, FILE *stream));
struct nala_fread_params_t *fread_mock_get_params_in(int handle);
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
void fread_mock_real(void);
void fread_mock_real_once(void);
void fread_mock_suspend(void);
void fread_mock_resume(void);
void fread_mock_reset(void);
void fread_mock_assert_completed(void);

// NALA_DECLARATION free

struct nala_free_params_t {
    void *ptr;
};

int free_mock();
int free_mock_once();
int free_mock_ignore_in(void);
int free_mock_ignore_in_once(void);
void free_mock_set_errno(int errno_value);
void free_mock_set_callback(void (*callback)(void *ptr));
struct nala_free_params_t *free_mock_get_params_in(int handle);
void free_mock_set_ptr_in(const void *buf_p, size_t size);
void free_mock_set_ptr_in_assert(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void free_mock_set_ptr_in_pointer(void *ptr);
void free_mock_set_ptr_out(const void *buf_p, size_t size);
void free_mock_set_ptr_out_copy(void (*callback)(void *ptr, const void *nala_buf_p, size_t nala_size));
void free_mock_none(void);
void free_mock_implementation(void (*implementation)(void *ptr));
void free_mock_real(void);
void free_mock_real_once(void);
void free_mock_suspend(void);
void free_mock_resume(void);
void free_mock_reset(void);
void free_mock_assert_completed(void);

// NALA_DECLARATION fseek

struct nala_fseek_params_t {
    FILE *stream;
    long int offset;
    int whence;
};

int fseek_mock(long int offset, int whence, int return_value);
int fseek_mock_once(long int offset, int whence, int return_value);
int fseek_mock_ignore_in(int return_value);
int fseek_mock_ignore_in_once(int return_value);
void fseek_mock_set_errno(int errno_value);
void fseek_mock_set_callback(void (*callback)(FILE *stream, long int offset, int whence));
struct nala_fseek_params_t *fseek_mock_get_params_in(int handle);
void fseek_mock_ignore_offset_in(void);
void fseek_mock_ignore_whence_in(void);
void fseek_mock_set_stream_in(const void *buf_p, size_t size);
void fseek_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fseek_mock_set_stream_in_pointer(FILE *stream);
void fseek_mock_set_stream_out(const void *buf_p, size_t size);
void fseek_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void fseek_mock_none(void);
void fseek_mock_implementation(int (*implementation)(FILE *stream, long int offset, int whence));
void fseek_mock_real(void);
void fseek_mock_real_once(void);
void fseek_mock_suspend(void);
void fseek_mock_resume(void);
void fseek_mock_reset(void);
void fseek_mock_assert_completed(void);

// NALA_DECLARATION ftell

struct nala_ftell_params_t {
    FILE *stream;
};

int ftell_mock(long int return_value);
int ftell_mock_once(long int return_value);
int ftell_mock_ignore_in(long int return_value);
int ftell_mock_ignore_in_once(long int return_value);
void ftell_mock_set_errno(int errno_value);
void ftell_mock_set_callback(void (*callback)(FILE *stream));
struct nala_ftell_params_t *ftell_mock_get_params_in(int handle);
void ftell_mock_set_stream_in(const void *buf_p, size_t size);
void ftell_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void ftell_mock_set_stream_in_pointer(FILE *stream);
void ftell_mock_set_stream_out(const void *buf_p, size_t size);
void ftell_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void ftell_mock_none(void);
void ftell_mock_implementation(long int (*implementation)(FILE *stream));
void ftell_mock_real(void);
void ftell_mock_real_once(void);
void ftell_mock_suspend(void);
void ftell_mock_resume(void);
void ftell_mock_reset(void);
void ftell_mock_assert_completed(void);

// NALA_DECLARATION fwrite

struct nala_fwrite_params_t {
    const void *ptr;
    size_t size;
    size_t nmemb;
    FILE *stream;
};

int fwrite_mock(size_t size, size_t nmemb, size_t return_value);
int fwrite_mock_once(size_t size, size_t nmemb, size_t return_value);
int fwrite_mock_ignore_in(size_t return_value);
int fwrite_mock_ignore_in_once(size_t return_value);
void fwrite_mock_set_errno(int errno_value);
void fwrite_mock_set_callback(void (*callback)(const void *ptr, size_t size, size_t nmemb, FILE *stream));
struct nala_fwrite_params_t *fwrite_mock_get_params_in(int handle);
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
void fwrite_mock_real(void);
void fwrite_mock_real_once(void);
void fwrite_mock_suspend(void);
void fwrite_mock_resume(void);
void fwrite_mock_reset(void);
void fwrite_mock_assert_completed(void);

// NALA_DECLARATION getmntent

struct nala_getmntent_params_t {
    FILE *stream;
};

int getmntent_mock(struct mntent *return_value);
int getmntent_mock_once(struct mntent *return_value);
int getmntent_mock_ignore_in(struct mntent *return_value);
int getmntent_mock_ignore_in_once(struct mntent *return_value);
void getmntent_mock_set_errno(int errno_value);
void getmntent_mock_set_callback(void (*callback)(FILE *stream));
struct nala_getmntent_params_t *getmntent_mock_get_params_in(int handle);
void getmntent_mock_set_stream_in(const void *buf_p, size_t size);
void getmntent_mock_set_stream_in_assert(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void getmntent_mock_set_stream_in_pointer(FILE *stream);
void getmntent_mock_set_stream_out(const void *buf_p, size_t size);
void getmntent_mock_set_stream_out_copy(void (*callback)(FILE *stream, const void *nala_buf_p, size_t nala_size));
void getmntent_mock_none(void);
void getmntent_mock_implementation(struct mntent *(*implementation)(FILE *stream));
void getmntent_mock_real(void);
void getmntent_mock_real_once(void);
void getmntent_mock_suspend(void);
void getmntent_mock_resume(void);
void getmntent_mock_reset(void);
void getmntent_mock_assert_completed(void);

// NALA_DECLARATION in_out

struct nala_in_out_params_t {
    int *buf_p;
};

int in_out_mock();
int in_out_mock_once();
int in_out_mock_ignore_in(void);
int in_out_mock_ignore_in_once(void);
void in_out_mock_set_errno(int errno_value);
void in_out_mock_set_callback(void (*callback)(int *buf_p));
struct nala_in_out_params_t *in_out_mock_get_params_in(int handle);
void in_out_mock_set_buf_p_in(const void *buf_p, size_t size);
void in_out_mock_set_buf_p_in_assert(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size));
void in_out_mock_set_buf_p_in_pointer(int *buf_p);
void in_out_mock_set_buf_p_out(const void *buf_p, size_t size);
void in_out_mock_set_buf_p_out_copy(void (*callback)(int *buf_p, const void *nala_buf_p, size_t nala_size));
void in_out_mock_none(void);
void in_out_mock_implementation(void (*implementation)(int *buf_p));
void in_out_mock_real(void);
void in_out_mock_real_once(void);
void in_out_mock_suspend(void);
void in_out_mock_resume(void);
void in_out_mock_reset(void);
void in_out_mock_assert_completed(void);

// NALA_DECLARATION io_control

struct nala_io_control_params_t {
    int kind;
    const char *vafmt_p;
};

int io_control_mock_va_arg_real(int kind, va_list __nala_va_list);
int io_control_mock(int kind, int return_value, const char *vafmt_p, ...);
int io_control_mock_once(int kind, int return_value, const char *vafmt_p, ...);
int io_control_mock_ignore_in(int return_value);
int io_control_mock_ignore_in_once(int return_value);
void io_control_mock_set_errno(int errno_value);
void io_control_mock_set_callback(void (*callback)(int kind, va_list __nala_va_list));
struct nala_io_control_params_t *io_control_mock_get_params_in(int handle);
void io_control_mock_ignore_kind_in(void);
void io_control_mock_ignore_va_arg_in_at(unsigned int index);
void io_control_mock_set_va_arg_in_at(unsigned int index, const void *buf_p, size_t size, nala_mock_assert_in_t assert_in);
void io_control_mock_set_va_arg_in_pointer_at(unsigned int index, const void *buf_p);
void io_control_mock_set_va_arg_out_at(unsigned int index, const void *buf_p, size_t size);
void io_control_mock_none(void);
void io_control_mock_implementation(int (*implementation)(int kind, va_list __nala_va_list));
void io_control_mock_real(void);
void io_control_mock_real_once(void);
void io_control_mock_suspend(void);
void io_control_mock_resume(void);
void io_control_mock_reset(void);
void io_control_mock_assert_completed(void);

// NALA_DECLARATION io_vcontrol

struct nala_io_vcontrol_params_t {
    int kind;
    va_list ap;
};

int io_vcontrol_mock(int kind, int return_value);
int io_vcontrol_mock_once(int kind, int return_value);
int io_vcontrol_mock_ignore_in(int return_value);
int io_vcontrol_mock_ignore_in_once(int return_value);
void io_vcontrol_mock_set_errno(int errno_value);
void io_vcontrol_mock_set_callback(void (*callback)(int kind, va_list ap));
struct nala_io_vcontrol_params_t *io_vcontrol_mock_get_params_in(int handle);
void io_vcontrol_mock_ignore_kind_in(void);
void io_vcontrol_mock_none(void);
void io_vcontrol_mock_implementation(int (*implementation)(int kind, va_list ap));
void io_vcontrol_mock_real(void);
void io_vcontrol_mock_real_once(void);
void io_vcontrol_mock_suspend(void);
void io_vcontrol_mock_resume(void);
void io_vcontrol_mock_reset(void);
void io_vcontrol_mock_assert_completed(void);

// NALA_DECLARATION malloc

struct nala_malloc_params_t {
    size_t size;
};

int malloc_mock(size_t size, void *return_value);
int malloc_mock_once(size_t size, void *return_value);
int malloc_mock_ignore_in(void *return_value);
int malloc_mock_ignore_in_once(void *return_value);
void malloc_mock_set_errno(int errno_value);
void malloc_mock_set_callback(void (*callback)(size_t size));
struct nala_malloc_params_t *malloc_mock_get_params_in(int handle);
void malloc_mock_ignore_size_in(void);
void malloc_mock_none(void);
void malloc_mock_implementation(void *(*implementation)(size_t size));
void malloc_mock_real(void);
void malloc_mock_real_once(void);
void malloc_mock_suspend(void);
void malloc_mock_resume(void);
void malloc_mock_reset(void);
void malloc_mock_assert_completed(void);

// NALA_DECLARATION mount

struct nala_mount_params_t {
    const char *source;
    const char *target;
    const char *filesystemtype;
    unsigned long int mountflags;
    const void *data;
};

int mount_mock(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value);
int mount_mock_once(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, int return_value);
int mount_mock_ignore_in(int return_value);
int mount_mock_ignore_in_once(int return_value);
void mount_mock_set_errno(int errno_value);
void mount_mock_set_callback(void (*callback)(const char *source, const char *target, const char *filesystemtype, unsigned long int mountflags, const void *data));
struct nala_mount_params_t *mount_mock_get_params_in(int handle);
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
void mount_mock_real(void);
void mount_mock_real_once(void);
void mount_mock_suspend(void);
void mount_mock_resume(void);
void mount_mock_reset(void);
void mount_mock_assert_completed(void);

// NALA_DECLARATION output_message

struct nala_output_message_params_t {
    const char *message;
};

int output_message_mock(const char *message);
int output_message_mock_once(const char *message);
int output_message_mock_ignore_in(void);
int output_message_mock_ignore_in_once(void);
void output_message_mock_set_errno(int errno_value);
void output_message_mock_set_callback(void (*callback)(const char *message));
struct nala_output_message_params_t *output_message_mock_get_params_in(int handle);
void output_message_mock_ignore_message_in(void);
void output_message_mock_set_message_in(const void *buf_p, size_t size);
void output_message_mock_set_message_in_assert(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size));
void output_message_mock_set_message_in_pointer(const char *message);
void output_message_mock_set_message_out(const void *buf_p, size_t size);
void output_message_mock_set_message_out_copy(void (*callback)(const char *message, const void *nala_buf_p, size_t nala_size));
void output_message_mock_none(void);
void output_message_mock_implementation(void (*implementation)(const char *message));
void output_message_mock_real(void);
void output_message_mock_real_once(void);
void output_message_mock_suspend(void);
void output_message_mock_resume(void);
void output_message_mock_reset(void);
void output_message_mock_assert_completed(void);

// NALA_DECLARATION pipe

struct nala_pipe_params_t {
    int *pipefd;
};

int pipe_mock(int return_value);
int pipe_mock_once(int return_value);
int pipe_mock_ignore_in(int return_value);
int pipe_mock_ignore_in_once(int return_value);
void pipe_mock_set_errno(int errno_value);
void pipe_mock_set_callback(void (*callback)(int pipefd[2]));
struct nala_pipe_params_t *pipe_mock_get_params_in(int handle);
void pipe_mock_set_pipefd_in(const void *buf_p, size_t size);
void pipe_mock_set_pipefd_in_assert(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size));
void pipe_mock_set_pipefd_in_pointer(int pipefd[2]);
void pipe_mock_set_pipefd_out(const void *buf_p, size_t size);
void pipe_mock_set_pipefd_out_copy(void (*callback)(int pipefd[2], const void *nala_buf_p, size_t nala_size));
void pipe_mock_none(void);
void pipe_mock_implementation(int (*implementation)(int pipefd[2]));
void pipe_mock_real(void);
void pipe_mock_real_once(void);
void pipe_mock_suspend(void);
void pipe_mock_resume(void);
void pipe_mock_reset(void);
void pipe_mock_assert_completed(void);

// NALA_DECLARATION poll

struct nala_poll_params_t {
    struct pollfd *fds;
    nfds_t nfds;
    int timeout;
};

int poll_mock(nfds_t nfds, int timeout, int return_value);
int poll_mock_once(nfds_t nfds, int timeout, int return_value);
int poll_mock_ignore_in(int return_value);
int poll_mock_ignore_in_once(int return_value);
void poll_mock_set_errno(int errno_value);
void poll_mock_set_callback(void (*callback)(struct pollfd *fds, nfds_t nfds, int timeout));
struct nala_poll_params_t *poll_mock_get_params_in(int handle);
void poll_mock_ignore_nfds_in(void);
void poll_mock_ignore_timeout_in(void);
void poll_mock_set_fds_in(const void *buf_p, size_t size);
void poll_mock_set_fds_in_assert(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size));
void poll_mock_set_fds_in_pointer(struct pollfd *fds);
void poll_mock_set_fds_out(const void *buf_p, size_t size);
void poll_mock_set_fds_out_copy(void (*callback)(struct pollfd *fds, const void *nala_buf_p, size_t nala_size));
void poll_mock_none(void);
void poll_mock_implementation(int (*implementation)(struct pollfd *fds, nfds_t nfds, int timeout));
void poll_mock_real(void);
void poll_mock_real_once(void);
void poll_mock_suspend(void);
void poll_mock_resume(void);
void poll_mock_reset(void);
void poll_mock_assert_completed(void);

// NALA_DECLARATION print_hello

struct nala_print_hello_params_t {
    int dummy;
};

int print_hello_mock();
int print_hello_mock_once();
int print_hello_mock_ignore_in(void);
int print_hello_mock_ignore_in_once(void);
void print_hello_mock_set_errno(int errno_value);
void print_hello_mock_set_callback(void (*callback)());
struct nala_print_hello_params_t *print_hello_mock_get_params_in(int handle);
void print_hello_mock_none(void);
void print_hello_mock_implementation(void (*implementation)());
void print_hello_mock_real(void);
void print_hello_mock_real_once(void);
void print_hello_mock_suspend(void);
void print_hello_mock_resume(void);
void print_hello_mock_reset(void);
void print_hello_mock_assert_completed(void);

// NALA_DECLARATION read

struct nala_read_params_t {
    int fd;
    void *buf;
    size_t count;
};

int read_mock(int fd, size_t count, ssize_t return_value);
int read_mock_once(int fd, size_t count, ssize_t return_value);
int read_mock_ignore_in(ssize_t return_value);
int read_mock_ignore_in_once(ssize_t return_value);
void read_mock_set_errno(int errno_value);
void read_mock_set_callback(void (*callback)(int fd, void *buf, size_t count));
struct nala_read_params_t *read_mock_get_params_in(int handle);
void read_mock_ignore_fd_in(void);
void read_mock_ignore_count_in(void);
void read_mock_set_buf_in(const void *buf_p, size_t size);
void read_mock_set_buf_in_assert(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size));
void read_mock_set_buf_in_pointer(void *buf);
void read_mock_set_buf_out(const void *buf_p, size_t size);
void read_mock_set_buf_out_copy(void (*callback)(void *buf, const void *nala_buf_p, size_t nala_size));
void read_mock_none(void);
void read_mock_implementation(ssize_t (*implementation)(int fd, void *buf, size_t count));
void read_mock_real(void);
void read_mock_real_once(void);
void read_mock_suspend(void);
void read_mock_resume(void);
void read_mock_reset(void);
void read_mock_assert_completed(void);

// NALA_DECLARATION sendto

struct nala_sendto_params_t {
    int sockfd;
    const void *buf;
    size_t len;
    int flags;
    const struct sockaddr *dest_addr;
    socklen_t addrlen;
};

int sendto_mock(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value);
int sendto_mock_once(int sockfd, size_t len, int flags, socklen_t addrlen, ssize_t return_value);
int sendto_mock_ignore_in(ssize_t return_value);
int sendto_mock_ignore_in_once(ssize_t return_value);
void sendto_mock_set_errno(int errno_value);
void sendto_mock_set_callback(void (*callback)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen));
struct nala_sendto_params_t *sendto_mock_get_params_in(int handle);
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
void sendto_mock_real(void);
void sendto_mock_real_once(void);
void sendto_mock_suspend(void);
void sendto_mock_resume(void);
void sendto_mock_reset(void);
void sendto_mock_assert_completed(void);

// NALA_DECLARATION setsockopt

struct nala_setsockopt_params_t {
    int sockfd;
    int level;
    int optname;
    const void *optval;
    socklen_t optlen;
};

int setsockopt_mock(int sockfd, int level, int optname, socklen_t optlen, int return_value);
int setsockopt_mock_once(int sockfd, int level, int optname, socklen_t optlen, int return_value);
int setsockopt_mock_ignore_in(int return_value);
int setsockopt_mock_ignore_in_once(int return_value);
void setsockopt_mock_set_errno(int errno_value);
void setsockopt_mock_set_callback(void (*callback)(int sockfd, int level, int optname, const void *optval, socklen_t optlen));
struct nala_setsockopt_params_t *setsockopt_mock_get_params_in(int handle);
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
void setsockopt_mock_real(void);
void setsockopt_mock_real_once(void);
void setsockopt_mock_suspend(void);
void setsockopt_mock_resume(void);
void setsockopt_mock_reset(void);
void setsockopt_mock_assert_completed(void);

// NALA_DECLARATION sleep

struct nala_sleep_params_t {
    unsigned int seconds;
};

int sleep_mock(unsigned int seconds, unsigned int return_value);
int sleep_mock_once(unsigned int seconds, unsigned int return_value);
int sleep_mock_ignore_in(unsigned int return_value);
int sleep_mock_ignore_in_once(unsigned int return_value);
void sleep_mock_set_errno(int errno_value);
void sleep_mock_set_callback(void (*callback)(unsigned int seconds));
struct nala_sleep_params_t *sleep_mock_get_params_in(int handle);
void sleep_mock_ignore_seconds_in(void);
void sleep_mock_none(void);
void sleep_mock_implementation(unsigned int (*implementation)(unsigned int seconds));
void sleep_mock_real(void);
void sleep_mock_real_once(void);
void sleep_mock_suspend(void);
void sleep_mock_resume(void);
void sleep_mock_reset(void);
void sleep_mock_assert_completed(void);

// NALA_DECLARATION statvfs

struct nala_statvfs_params_t {
    const char *path;
    struct statvfs *buf;
};

int statvfs_mock(const char *path, int return_value);
int statvfs_mock_once(const char *path, int return_value);
int statvfs_mock_ignore_in(int return_value);
int statvfs_mock_ignore_in_once(int return_value);
void statvfs_mock_set_errno(int errno_value);
void statvfs_mock_set_callback(void (*callback)(const char *path, struct statvfs *buf));
struct nala_statvfs_params_t *statvfs_mock_get_params_in(int handle);
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
void statvfs_mock_real(void);
void statvfs_mock_real_once(void);
void statvfs_mock_suspend(void);
void statvfs_mock_resume(void);
void statvfs_mock_reset(void);
void statvfs_mock_assert_completed(void);

// NALA_DECLARATION struct_param

struct nala_struct_param_params_t {
    struct struct_param_type *data;
};

int struct_param_mock();
int struct_param_mock_once();
int struct_param_mock_ignore_in(void);
int struct_param_mock_ignore_in_once(void);
void struct_param_mock_set_errno(int errno_value);
void struct_param_mock_set_callback(void (*callback)(struct struct_param_type *data));
struct nala_struct_param_params_t *struct_param_mock_get_params_in(int handle);
void struct_param_mock_set_data_in(const void *buf_p, size_t size);
void struct_param_mock_set_data_in_assert(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size));
void struct_param_mock_set_data_in_pointer(struct struct_param_type *data);
void struct_param_mock_set_data_out(const void *buf_p, size_t size);
void struct_param_mock_set_data_out_copy(void (*callback)(struct struct_param_type *data, const void *nala_buf_p, size_t nala_size));
void struct_param_mock_none(void);
void struct_param_mock_implementation(void (*implementation)(struct struct_param_type *data));
void struct_param_mock_real(void);
void struct_param_mock_real_once(void);
void struct_param_mock_suspend(void);
void struct_param_mock_resume(void);
void struct_param_mock_reset(void);
void struct_param_mock_assert_completed(void);

// NALA_DECLARATION struct_param_and_return_type

struct nala_struct_param_and_return_type_params_t {
    struct struct_param_type arg;
};

int struct_param_and_return_type_mock(struct struct_param_type return_value);
int struct_param_and_return_type_mock_once(struct struct_param_type return_value);
int struct_param_and_return_type_mock_ignore_in(struct struct_param_type return_value);
int struct_param_and_return_type_mock_ignore_in_once(struct struct_param_type return_value);
void struct_param_and_return_type_mock_set_errno(int errno_value);
void struct_param_and_return_type_mock_set_callback(void (*callback)(struct struct_param_type arg));
struct nala_struct_param_and_return_type_params_t *struct_param_and_return_type_mock_get_params_in(int handle);
void struct_param_and_return_type_mock_none(void);
void struct_param_and_return_type_mock_implementation(struct struct_param_type (*implementation)(struct struct_param_type arg));
void struct_param_and_return_type_mock_real(void);
void struct_param_and_return_type_mock_real_once(void);
void struct_param_and_return_type_mock_suspend(void);
void struct_param_and_return_type_mock_resume(void);
void struct_param_and_return_type_mock_reset(void);
void struct_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION time

struct nala_time_params_t {
    time_t *tloc;
};

int time_mock(time_t return_value);
int time_mock_once(time_t return_value);
int time_mock_ignore_in(time_t return_value);
int time_mock_ignore_in_once(time_t return_value);
void time_mock_set_errno(int errno_value);
void time_mock_set_callback(void (*callback)(time_t *tloc));
struct nala_time_params_t *time_mock_get_params_in(int handle);
void time_mock_set_tloc_in(const void *buf_p, size_t size);
void time_mock_set_tloc_in_assert(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size));
void time_mock_set_tloc_in_pointer(time_t *tloc);
void time_mock_set_tloc_out(const void *buf_p, size_t size);
void time_mock_set_tloc_out_copy(void (*callback)(time_t *tloc, const void *nala_buf_p, size_t nala_size));
void time_mock_none(void);
void time_mock_implementation(time_t (*implementation)(time_t *tloc));
void time_mock_real(void);
void time_mock_real_once(void);
void time_mock_suspend(void);
void time_mock_resume(void);
void time_mock_reset(void);
void time_mock_assert_completed(void);

// NALA_DECLARATION timerfd_settime

struct nala_timerfd_settime_params_t {
    int fd;
    int flags;
    const struct itimerspec *new_value;
    struct itimerspec *old_value;
};

int timerfd_settime_mock(int fd, int flags, int return_value);
int timerfd_settime_mock_once(int fd, int flags, int return_value);
int timerfd_settime_mock_ignore_in(int return_value);
int timerfd_settime_mock_ignore_in_once(int return_value);
void timerfd_settime_mock_set_errno(int errno_value);
void timerfd_settime_mock_set_callback(void (*callback)(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value));
struct nala_timerfd_settime_params_t *timerfd_settime_mock_get_params_in(int handle);
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
void timerfd_settime_mock_real(void);
void timerfd_settime_mock_real_once(void);
void timerfd_settime_mock_suspend(void);
void timerfd_settime_mock_resume(void);
void timerfd_settime_mock_reset(void);
void timerfd_settime_mock_assert_completed(void);

// NALA_DECLARATION tmpfile

struct nala_tmpfile_params_t {
    int dummy;
};

int tmpfile_mock(FILE *return_value);
int tmpfile_mock_once(FILE *return_value);
int tmpfile_mock_ignore_in(FILE *return_value);
int tmpfile_mock_ignore_in_once(FILE *return_value);
void tmpfile_mock_set_errno(int errno_value);
void tmpfile_mock_set_callback(void (*callback)(void));
struct nala_tmpfile_params_t *tmpfile_mock_get_params_in(int handle);
void tmpfile_mock_none(void);
void tmpfile_mock_implementation(FILE *(*implementation)(void));
void tmpfile_mock_real(void);
void tmpfile_mock_real_once(void);
void tmpfile_mock_suspend(void);
void tmpfile_mock_resume(void);
void tmpfile_mock_reset(void);
void tmpfile_mock_assert_completed(void);

// NALA_DECLARATION typedef_struct_param_and_return_type

struct nala_typedef_struct_param_and_return_type_params_t {
    struct_param_type arg;
};

int typedef_struct_param_and_return_type_mock(struct_param_type return_value);
int typedef_struct_param_and_return_type_mock_once(struct_param_type return_value);
int typedef_struct_param_and_return_type_mock_ignore_in(struct_param_type return_value);
int typedef_struct_param_and_return_type_mock_ignore_in_once(struct_param_type return_value);
void typedef_struct_param_and_return_type_mock_set_errno(int errno_value);
void typedef_struct_param_and_return_type_mock_set_callback(void (*callback)(struct_param_type arg));
struct nala_typedef_struct_param_and_return_type_params_t *typedef_struct_param_and_return_type_mock_get_params_in(int handle);
void typedef_struct_param_and_return_type_mock_none(void);
void typedef_struct_param_and_return_type_mock_implementation(struct_param_type (*implementation)(struct_param_type arg));
void typedef_struct_param_and_return_type_mock_real(void);
void typedef_struct_param_and_return_type_mock_real_once(void);
void typedef_struct_param_and_return_type_mock_suspend(void);
void typedef_struct_param_and_return_type_mock_resume(void);
void typedef_struct_param_and_return_type_mock_reset(void);
void typedef_struct_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION typedef_union_param_and_return_type

struct nala_typedef_union_param_and_return_type_params_t {
    union_type arg;
};

int typedef_union_param_and_return_type_mock(union_type return_value);
int typedef_union_param_and_return_type_mock_once(union_type return_value);
int typedef_union_param_and_return_type_mock_ignore_in(union_type return_value);
int typedef_union_param_and_return_type_mock_ignore_in_once(union_type return_value);
void typedef_union_param_and_return_type_mock_set_errno(int errno_value);
void typedef_union_param_and_return_type_mock_set_callback(void (*callback)(union_type arg));
struct nala_typedef_union_param_and_return_type_params_t *typedef_union_param_and_return_type_mock_get_params_in(int handle);
void typedef_union_param_and_return_type_mock_none(void);
void typedef_union_param_and_return_type_mock_implementation(union_type (*implementation)(union_type arg));
void typedef_union_param_and_return_type_mock_real(void);
void typedef_union_param_and_return_type_mock_real_once(void);
void typedef_union_param_and_return_type_mock_suspend(void);
void typedef_union_param_and_return_type_mock_resume(void);
void typedef_union_param_and_return_type_mock_reset(void);
void typedef_union_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION union_param_and_return_type

struct nala_union_param_and_return_type_params_t {
    union union_type arg;
};

int union_param_and_return_type_mock(union union_type return_value);
int union_param_and_return_type_mock_once(union union_type return_value);
int union_param_and_return_type_mock_ignore_in(union union_type return_value);
int union_param_and_return_type_mock_ignore_in_once(union union_type return_value);
void union_param_and_return_type_mock_set_errno(int errno_value);
void union_param_and_return_type_mock_set_callback(void (*callback)(union union_type arg));
struct nala_union_param_and_return_type_params_t *union_param_and_return_type_mock_get_params_in(int handle);
void union_param_and_return_type_mock_none(void);
void union_param_and_return_type_mock_implementation(union union_type (*implementation)(union union_type arg));
void union_param_and_return_type_mock_real(void);
void union_param_and_return_type_mock_real_once(void);
void union_param_and_return_type_mock_suspend(void);
void union_param_and_return_type_mock_resume(void);
void union_param_and_return_type_mock_reset(void);
void union_param_and_return_type_mock_assert_completed(void);

// NALA_DECLARATION usleep

struct nala_usleep_params_t {
    __useconds_t usec;
};

int usleep_mock(__useconds_t usec, int return_value);
int usleep_mock_once(__useconds_t usec, int return_value);
int usleep_mock_ignore_in(int return_value);
int usleep_mock_ignore_in_once(int return_value);
void usleep_mock_set_errno(int errno_value);
void usleep_mock_set_callback(void (*callback)(__useconds_t usec));
struct nala_usleep_params_t *usleep_mock_get_params_in(int handle);
void usleep_mock_ignore_usec_in(void);
void usleep_mock_none(void);
void usleep_mock_implementation(int (*implementation)(__useconds_t usec));
void usleep_mock_real(void);
void usleep_mock_real_once(void);
void usleep_mock_suspend(void);
void usleep_mock_resume(void);
void usleep_mock_reset(void);
void usleep_mock_assert_completed(void);

// NALA_DECLARATION write

struct nala_write_params_t {
    int fd;
    const void *buf;
    size_t count;
};

int write_mock(int fd, size_t count, ssize_t return_value);
int write_mock_once(int fd, size_t count, ssize_t return_value);
int write_mock_ignore_in(ssize_t return_value);
int write_mock_ignore_in_once(ssize_t return_value);
void write_mock_set_errno(int errno_value);
void write_mock_set_callback(void (*callback)(int fd, const void *buf, size_t count));
struct nala_write_params_t *write_mock_get_params_in(int handle);
void write_mock_ignore_fd_in(void);
void write_mock_ignore_count_in(void);
void write_mock_set_buf_in(const void *buf_p, size_t size);
void write_mock_set_buf_in_assert(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void write_mock_set_buf_in_pointer(const void *buf);
void write_mock_set_buf_out(const void *buf_p, size_t size);
void write_mock_set_buf_out_copy(void (*callback)(const void *buf, const void *nala_buf_p, size_t nala_size));
void write_mock_none(void);
void write_mock_implementation(ssize_t (*implementation)(int fd, const void *buf, size_t count));
void write_mock_real(void);
void write_mock_real_once(void);
void write_mock_suspend(void);
void write_mock_resume(void);
void write_mock_reset(void);
void write_mock_assert_completed(void);

