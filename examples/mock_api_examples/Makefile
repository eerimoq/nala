TESTS += main.c
INC += $(shell $(NALA) include_dir)
SRC += $(shell $(NALA) c_sources)
SRC += foo.c
ARGS ?= --continue-on-failure

include ../../make/test.mk
