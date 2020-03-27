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
ifeq ($(SANITIZE), yes)
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=undefined
endif
NALA ?= PYTHONPATH=../.. python3 -m nala
SRC += main.c
SRC += ../../nala/dist/nala.c
SRC += dummy_functions.c
SRC += nala_mocks.c
SRC += subprocess.c

test:
	$(MAKE) -C ../.. dist
	[ -f nala_mocks.h ] || (touch nala_mocks.h)
	$(CC) $(INC) -DNALA_GENERATE_MOCKS -E main.c \
	    | $(NALA) generate_mocks --no-cache \
				     -n no_implementation \
				     -n another \
				     -n io_control_no_implementation \
				     -n ioctl
	$(CC) $(CFLAGS) @nala_mocks.ldflags $(SRC) -o main
	./main
