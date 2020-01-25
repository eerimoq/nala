#include <time.h>
#include <errno.h>
#include <poll.h>
#include "nala.h"
#include "nala_mocks.h"

TEST(test_time)
{
    time_mock_once(42);

    ASSERT_EQ(time(NULL), 42);
}

TEST(test_poll)
{
    struct pollfd fds[1];

    poll_mock_once(1, -1, 1);
    fds[0].revents = POLLHUP;
    poll_mock_set_fds_out(&fds[0], sizeof(fds));
    poll_mock_disable_once();

    /* Mocked call. */
    fds[0].fd = 100;
    fds[0].events = 0;
    fds[0].revents = 0;
    ASSERT_EQ(poll(&fds[0], 1, -1), 1);
    ASSERT_EQ(fds[0].revents, POLLHUP);

    /* Real call. */
    fds[0].fd = 100;
    fds[0].events = 0;
    fds[0].revents = 0;
    ASSERT_EQ(poll(&fds[0], 1, -1), 1);
    ASSERT_EQ(fds[0].revents, POLLNVAL);
}
