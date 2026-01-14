# LS Ring-Buffer Queue

Minimal, single-header ring-buffer queue for C.

- Type-safe via macro code generation (not macro function calls)
- ANSI C
- Header-only
- Ring-buffer (no memory copying, no allocations)
- Full coverage tests (branches, lines, functions)

## Quick Start

1. Copy `ls_queue.h` to your project.
2. Define any number of queue types:
    ```c
    LS_QUEUE_TYPE_INLINE(int, int_queue, 32)
    ```
  
    **OR** for separate declaration/implementation:
  
    ```c
    // In header:
    LS_QUEUE_TYPE_DECL(int, int_queue, 32)

    // In source file:
    LS_QUEUE_TYPE_IMPL(int, int_queue, 32)
    ```
3. Use the queue with normal C function calls:
    ```c
    int_queue q;
    int_queue_init(&q);
    if (!int_queue_push(&q, 42)) {
        // push failed, queue full, handle that here
    }
    int val;
    if (int_queue_pop(&q, &val)) {
        // use val
    }
    ```

See [`ls_queue.h`](ls_queue.h) for detailed documentation and usage patterns.

## License

MIT.
