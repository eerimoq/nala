INC += -I../../nala/dist
CFLAGS = \
	$(INC) \
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

test: nala_mocks.h
	$(CC) $(CFLAGS) main.c -c -o main.o
	objcopy --redefine-syms nala_mocks.syms main.o
	$(CC) $(CFLAGS) main.o nala_mocks.c ../../nala/dist/nala.c -o main
	./main

nala_mocks.h: main.c
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -E main.c | $(NALA) generate_mocks --redefine-syms
