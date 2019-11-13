ASAN_FLAGS = -fsanitize=address
INC += -I../../nala/dist
CFLAGS += $(INC)
CFLAGS += -Wextra
CFLAGS += -g
CFLAGS += -O0
CFLAGS += $(ASAN_FLAGS)
NALA ?= PYTHONPATH=../.. python3 -m nala

test: nala_mocks.h
	$(CC) $(CFLAGS) $$(cat nala_mocks.ld) *.c ../../nala/dist/nala.c -o main
	./main

nala_mocks.h: main.c
	[ -f nala_mocks.h ] || touch nala_mocks.h
	$(CC) $(INC) -E main.c | $(NALA) generate_mocks
