ASAN_FLAGS = -fsanitize=address
INC += -I../../nala/dist
CFLAGS = \
	$(INC) \
	-Wextra \
	-Wconversion \
	-g \
	-Og \
	-no-pie \
	$(ASAN_FLAGS) \
	-fprofile-arcs \
	-ftest-coverage
NALA ?= PYTHONPATH=../.. python3 -m nala

test: nala_mocks.h
	$(CC) $(CFLAGS) $$(cat nala_mocks.ld) *.c ../../nala/dist/nala.c -o main
	./main

nala_mocks.h: main.c
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -E main.c | $(NALA) generate_mocks
