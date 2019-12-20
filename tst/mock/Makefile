ASAN_FLAGS = -fsanitize=address
INC += -I../../nala/dist
CFLAGS = \
	$(INC) \
	-Werror \
	-Wall \
	-Wextra \
	-Wconversion \
	-g \
	-Og \
	-no-pie \
	-fsanitize=address \
	-Wpedantic \
	$(ASAN_FLAGS) \
	-fprofile-arcs \
	-ftest-coverage
NALA ?= PYTHONPATH=../.. python3 -m nala
SRC += main.c
SRC += ../../nala/dist/nala.c
SRC += dummy_functions.c
SRC += nala_mocks.c
SRC += subprocess.c

test: nala_mocks.h
	$(CC) $(CFLAGS) $$(cat nala_mocks.ld) $(SRC) -o main
	./main

nala_mocks.h: main.c
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -E main.c > main.pp.c
	cat main.pp.c | $(NALA) generate_mocks
