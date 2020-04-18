TESTS += main.c
TESTS += test_filter_nala.c
INC += ../../include
SRC += ../../src/nala.c
SRC += ../../src/diff/diff.c
SRC += ../../src/utils.c
SRC += ../../src/hexdump/hexdump.c
SRC += ../../src/subprocess.c
SRC += ../../src/traceback.c
SRC += ../../src/hf.c
SRC += subprocess.c
# Tests that nala_mocks.c compiles without errors when no functions
# are mocked.
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wconversion
CFLAGS += -Wpedantic
CFLAGS += -fprofile-arcs
CFLAGS += -ftest-coverage
MOCKGENFLAGS += --no-cache

.PHONY: build/nala_mocks.ldflags

default:
	$(MAKE) -C subtest SANITIZE=no build
	$(MAKE) all

include ../../make/test.mk
