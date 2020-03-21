CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Werror \
	-Og \
	-g \
	-no-pie
SRC += ../../src/nala.c
SRC += ../../src/diff/diff.c
SRC += ../../src/utils.c
SRC += ../../src/hexdump/hexdump.c
SRC += ../../src/subprocess.c
SRC += ../../src/traceback.c
SRC += ../../src/hf.c
SRC += subprocess.c
SRC += main.c
# Tests that nala_mocks.c compiles without errors when no functions
# are mocked.
SRC += nala_mocks.c
INC +=  -I../../include
NALA ?= PYTHONPATH=../.. python3 -m nala

all:
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -DNALA_GENERATE_MOCKS -E main.c > main.pp.c
	cat main.pp.c | $(NALA) generate_mocks --no-cache
	$(CC) $(INC) $(CFLAGS) $$(cat nala_mocks.ld) $(SRC) -o main
	./main
