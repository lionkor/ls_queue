tests/tests: ls_queue.o tests/tests.c tests/ls_test.h
	$(CC) -o $@ ls_queue.o tests/tests.c -Itests -I.

# Usually you wouldn't do this, but for tests we want this compiled with the
# most pedantic settings.
# Dont use this.
ls_queue.o: ls_queue.h
	$(CC) -c -x c -o $@ $^ -Wall -Wextra -Wpedantic -Werror -ansi -std=c89 \
    	-DLS_QUEUE_IMPLEMENTATION \
    	-Wno-error=pragma-once-outside-header \
    	-Wno-pragma-once-outside-header

.PHONY: clean

clean:
	rm -f tests/tests
	rm -f ls_queue.o
