#include "nala.h"
#include "test_collect.h"

TEST(all)
{
    foo_mock();
    bar_mock_once();
    fie_mock_ignore_in();
    fum_mock_ignore_in_once();
    gam_mock_none();
    hit_mock_implementation();
}
