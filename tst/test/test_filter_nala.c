#include "nala.h"
#include "subprocess.h"

static void traceback_not_empty_with_nala_c_file_suffix_entry(void *arg_p)
{
    (void)arg_p;

    FAIL("");
}

TEST(traceback_not_empty_with_nala_c_file_suffix)
{
    struct subprocess_result_t *result_p;

    result_p = subprocess_call_output(
        traceback_not_empty_with_nala_c_file_suffix_entry,
        NULL);
    ASSERT_EQ(result_p->exit_code, 1);
    ASSERT_SUBSTRING(result_p->stdout.buf_p, "test_filter_nala.c:");
    subprocess_result_free(result_p);
}
