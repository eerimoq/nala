#include <string.h>
#include "foo.h"
#include "nala.h"
#include "nala_mocks.h"

static void foo_p_out_copy(struct foo_t *dst_p,
                           struct foo_t *src_p,
                           size_t size)
{
    (void)size;

    dst_p->a = src_p->a;
    /* Leave b unmodified. */
}

TEST(struct_member_out_copy)
{
    struct foo_t data;

    memset(&data, 0, sizeof(data));
    data.a = 10;

    foo_mock_once();
    foo_mock_set_foo_p_out(&data, sizeof(data));
    foo_mock_set_foo_p_out_copy(foo_p_out_copy);

    data.a = 5;
    data.b = 20;

    foo(&data);

    ASSERT_EQ(data.a, 10);
    ASSERT_EQ(data.b, 20);
}
