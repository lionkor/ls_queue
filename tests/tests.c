#define LS_TEST_IMPLEMENTATION
#include "ls_test.h"

#include "ls_queue.h"

LS_QUEUE_TYPE_INLINE(int, int_queue, 4)

TEST_CASE(queue_init) {
    int_queue q;
    int_queue_init(&q);
    ASSERT_EQ(q.read, 0, "%zu");
    ASSERT_EQ(q.write, 0, "%zu");
    return 0;
}

TEST_CASE(queue_push_single) {
    int_queue q;
    int_queue_init(&q);

    int result = int_queue_push(&q, 42);
    ASSERT_EQ(result, 1, "%d");
    ASSERT_EQ(q.write, 1, "%zu");
    ASSERT_EQ(q.read, 0, "%zu");

    return 0;
}

TEST_CASE(queue_pop_single) {
    int_queue q;
    int_queue_init(&q);

    int_queue_push(&q, 42);

    int val;
    int result = int_queue_pop(&q, &val);
    ASSERT_EQ(result, 1, "%d");
    ASSERT_EQ(val, 42, "%d");
    ASSERT_EQ(q.read, 1, "%zu");

    return 0;
}

TEST_CASE(queue_pop_empty) {
    int_queue q;
    int_queue_init(&q);

    int val;
    int result = int_queue_pop(&q, &val);
    ASSERT_EQ(result, 0, "%d");

    return 0;
}

TEST_CASE(queue_fill_to_capacity) {
    int_queue q;
    int_queue_init(&q);

    // Fill the queue to capacity (4 elements)
    ASSERT_EQ(int_queue_push(&q, 1), 1, "%d");
    ASSERT_EQ(int_queue_push(&q, 2), 1, "%d");
    ASSERT_EQ(int_queue_push(&q, 3), 1, "%d");
    ASSERT_EQ(int_queue_push(&q, 4), 1, "%d");

    // Try to push one more (should fail)
    int result = int_queue_push(&q, 5);
    ASSERT_EQ(result, 0, "%d");

    return 0;
}

TEST_CASE(queue_fifo_order) {
    int_queue q;
    int_queue_init(&q);

    // Push values in order
    int_queue_push(&q, 10);
    int_queue_push(&q, 20);
    int_queue_push(&q, 30);

    // Pop values and check FIFO order
    int val;
    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 10, "%d");

    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 20, "%d");

    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 30, "%d");

    return 0;
}

TEST_CASE(queue_circular_behavior) {
    int_queue q;
    int_queue_init(&q);

    // Fill queue
    int_queue_push(&q, 1);
    int_queue_push(&q, 2);
    int_queue_push(&q, 3);
    int_queue_push(&q, 4);

    // Pop some elements
    int val;
    int_queue_pop(&q, &val);
    int_queue_pop(&q, &val);

    // Should be able to push again
    ASSERT_EQ(int_queue_push(&q, 5), 1, "%d");
    ASSERT_EQ(int_queue_push(&q, 6), 1, "%d");

    // Verify order is maintained
    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 3, "%d");
    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 4, "%d");
    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 5, "%d");
    int_queue_pop(&q, &val);
    ASSERT_EQ(val, 6, "%d");

    return 0;
}

TEST_MAIN
