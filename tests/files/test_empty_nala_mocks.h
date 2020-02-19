/*
Mocks header file


Do not edit manually
*/

#ifndef OUTPUT_NALA_MOCKS_H
#define OUTPUT_NALA_MOCKS_H

#ifndef NALA_GENERATE_MOCKS

#include <stdarg.h>


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

