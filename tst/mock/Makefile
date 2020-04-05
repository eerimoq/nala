TESTS += main.c
INC += ../../include
SRC += ../../src/nala.c
SRC += ../../src/diff/diff.c
SRC += ../../src/utils.c
SRC += ../../src/hexdump/hexdump.c
SRC += ../../src/subprocess.c
SRC += ../../src/traceback.c
SRC += ../../src/hf.c
SRC += dummy_functions.c
SRC += subprocess.c
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wconversion
CFLAGS += -Wpedantic
CFLAGS += -fprofile-arcs
CFLAGS += -ftest-coverage
# To test that wrapping of internal symbols works with function sections.
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
IMPLEMENTATION += io_control
NO_IMPLEMENTATION += no_implementation
NO_IMPLEMENTATION += another*
NO_IMPLEMENTATION += io_control_no_implementation
NO_IMPLEMENTATION += ioctl
MOCKGENFLAGS += --no-cache

include ../../make/test.mk
