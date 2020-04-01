/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>


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

