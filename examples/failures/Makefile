TESTS += main.c
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += ../../nala/dist
SRC += $(BUILD)/nala_mocks.c
SRC += ../../nala/dist/nala.c
SRC += foo.c
SRC += $(TESTS)
OBJ = $(SRC:%.c=%.o)
CFLAGS += -g -O0 -no-pie
CFLAGS += -fsanitize=address
CFLAGS += $(INC:%=-I%)
NALA ?= PYTHONPATH=../.. python3 -m nala

all:
	$(MAKE) $(BUILD)/nala_mocks.ldflags
	$(MAKE) $(EXE)
	$(EXE) $(ARGS) --continue-on-failure

clean:
	rm -rf $(BUILD) $(OBJ)

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) $(CFLAGS) @$(BUILD)/nala_mocks.ldflags $^ -o $@

%.o: %.c
	echo "CC $^"
	$(CC) $(CFLAGS) -c $^ -o $@
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $@

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $^"
	mkdir -p $(BUILD)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	cat $(TESTS) \
	    | $(CC) $(INC:%=-I%) -DNALA_GENERATE_MOCKS -x c -E - \
	    | $(NALA) generate_mocks -o $(BUILD)
	touch $@
