#include <stdlib.h>
#include "nala.h"
#include "nala_mocks.h"

TEST(exit_1)
{
    exit_mock_once(1);
    exit_mock_set_callback(nala_exit);
    exit(1);
}
