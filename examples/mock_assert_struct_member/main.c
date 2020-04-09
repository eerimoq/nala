#include <string.h>
#include "foo.h"
#include "nala.h"
#include "nala_mocks.h"

static void foo_p_assert(struct foo_t *foo_p, const void *buf_p, size_t size)
{
    struct foo_t *expect_p;

    expect_p = (struct foo_t *)buf_p;
    
    ASSERT_EQ(foo_p->b, expect_p->b);
}

TEST(assert_struct_member)
{
    struct foo_t data;

    memset(&data, 0, sizeof(data));
    data.b = 10;

    foo_mock_once();
    foo_mock_set_foo_p_in(&data, sizeof(data));
    foo_mock_set_foo_p_in_assert(foo_p_assert);

    foo(&data);
}
