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
	-Wpedantic \
	-fprofile-arcs \
	-ftest-coverage
NALA ?= PYTHONPATH=../.. python3 -m nala
SRC += main.c
SRC += ../../nala/dist/nala.c
SRC += dummy_functions.c
SRC += nala_mocks.c
SRC += subprocess.c

test:
	$(MAKE) -C ../.. dist
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -DNALA_GENERATE_MOCKS -E main.c > main.pp.c
	cat main.pp.c | $(NALA) generate_mocks --no-cache
	$(CC) $(CFLAGS) $$(cat nala_mocks.ld) $(SRC) -o main
	./main
