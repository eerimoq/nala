ASAN_FLAGS = -fsanitize=address
INC += -I../../nala/dist
CFLAGS = \
	$(INC) \
	-Wextra \
	-Wconversion \
	-g \
	-Og \
	$(ASAN_FLAGS) \
	-fprofile-arcs \
	-ftest-coverage
NALA ?= PYTHONPATH=../.. python3 -m nala

test:
	$(CPP) $(INC) main.c | $(NALA) generate_mocks
	$(CC) $(CFLAGS) $$(cat nala_mocks.ld) *.c ../../nala/dist/nala.c -o main
	./main
