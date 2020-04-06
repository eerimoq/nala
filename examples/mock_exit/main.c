#include <stdlib.h>
#include "nala.h"
#include "nala_mocks.h"

static void exit_0()
{
    nala_assert_all_mocks_completed();
    exit_mock_real();
    exit(0);
}

TEST(exit_1)
{
    exit_mock_once(1);
    exit_mock_set_callback(exit_0);
    exit(1);
}
