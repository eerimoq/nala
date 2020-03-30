TESTS += main.c
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += ../../nala/dist
SRC += $(BUILD)/nala_mocks.c
SRC += ../../nala/dist/nala.c
SRC += $(TESTS)
SRC += dummy_functions.c
SRC += subprocess.c
OBJ = $(SRC:%.c=%.o)
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wconversion
CFLAGS += -g
CFLAGS += -Og
CFLAGS += -no-pie
CFLAGS += -Wpedantic
CFLAGS += -fprofile-arcs
CFLAGS += -ftest-coverage
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
ifeq ($(SANITIZE), yes)
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=undefined
endif
IMPLEMENTATION += io_control
NO_IMPLEMENTATION += no_implementation
NO_IMPLEMENTATION += another*
NO_IMPLEMENTATION += io_control_no_implementation
NO_IMPLEMENTATION += ioctl
MOCKGENFLAGS += --no-cache
MOCKGENFLAGS += $(IMPLEMENTATION:%=-i %)
MOCKGENFLAGS += $(NO_IMPLEMENTATION:%=-n %)
NALA ?= PYTHONPATH=../.. python3 -m nala

all:
	$(MAKE) -C ../.. dist
	$(MAKE) $(BUILD)/nala_mocks.ldflags
	$(MAKE) $(EXE)
	$(EXE) $(ARGS)

clean:
	rm -rf $(BUILD) $(OBJ)

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) @$(BUILD)/nala_mocks.ldflags -g -no-pie $^ -o $@

%.o: %.c
	echo "CC $^"
	$(CC) $(INC:%=-I%) -g -O0 -no-pie -c $^ -o $@
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $@

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $^"
	mkdir -p $(BUILD)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	cat $(TESTS) \
	    | $(CC) $(INC:%=-I%) -DNALA_GENERATE_MOCKS -x c -E - \
	    | $(NALA) generate_mocks $(MOCKGENFLAGS) -o $(BUILD)
	touch $@
