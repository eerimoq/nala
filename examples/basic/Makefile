ASAN_FLAGS = -fsanitize=address
CFLAGS = -Wall -Wextra -g -Og $(ASAN_FLAGS)

.PHONY: test clean

test: run_tests
	./$<

clean:
	rm -f run_tests

__mocks__%c __mocks__%h: main.c
	$(CPP) $^ | nala -g

run_tests: ../../dist/nala.h ../../dist/nala.c __mocks__.h __mocks__.c main.c
	$(CC) $(CFLAGS) $$(nala -f) *.c -o $@
