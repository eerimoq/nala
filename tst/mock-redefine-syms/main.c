#include <time.h>
#include <sys/mount.h>
#include "nala.h"
#include "nala_mocks.h"

TEST(test_time)
{
    time_mock_once(5);
    ASSERT_EQ(time(NULL), 5);
}

TEST(mount_function)
{
    ASSERT_EQ(mount("a", "b", "c", 0, ""), -1);

    mount_mock_once("a", "b", "c", 0, 0);
    mount_mock_set_data_in("", 1);

    ASSERT_EQ(mount("a", "b", "c", 0, ""), 0);

    mount_mock_once(NULL, "b", "c", 0, 0);
    mount_mock_set_data_in("", 1);

    ASSERT_EQ(mount(NULL, "b", "c", 0, ""), 0);
}
