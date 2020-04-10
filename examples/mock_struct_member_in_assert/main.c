#include <string.h>
#include "foo.h"
#include "nala.h"
#include "nala_mocks.h"

static void foo_p_in_assert(struct foo_t *actual_p,
                            struct foo_t *expected_p,
                            size_t size)
{
    (void)size;

    ASSERT_EQ(actual_p->b, expected_p->b);
}

TEST(struct_member_in_assert)
{
    struct foo_t data;

    memset(&data, 0, sizeof(data));
    data.b = 10;

    foo_mock_once();
    foo_mock_set_foo_p_in(&data, sizeof(data));
    foo_mock_set_foo_p_in_assert(foo_p_in_assert);

    foo(&data);
}
