TESTS += test_assertions.c
TESTS += test_time.c
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += $(CURDIR)
SRC += $(BUILD)/nala_mocks.c
SRC += nala.c
SRC += $(TESTS)
OBJ = $(SRC:%.c=%.o)
OBJDEPS = $(OBJ:%=%.d)
MOCKGENDEPS = $(BUILD)/nala_mocks.ldflags.d
DEPS = $(OBJDEPS) $(MOCKGENDEPS)
CFLAGS += $(INC:%=-I%)
CFLAGS += -g -O0 -no-pie
ifeq ($(SANITIZE), yes)
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=undefined
endif
MOCKGENFLAGS += $(IMPLEMENTATION=%=-i %)
MOCKGENFLAGS += $(NO_IMPLEMENTATION=%=-n %)
NALA = nala

all:
	$(MAKE) $(BUILD)/nala_mocks.ldflags
	$(MAKE) $(EXE)
	$(EXE) $(ARGS)

clean:
	rm -rf $(BUILD) $(OBJ)

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) $(CFLAGS) @$(BUILD)/nala_mocks.ldflags $^ -o $@

%.o: %.c
	echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $@
	$(CC) -MM $(CFLAGS) -o $@.d $<

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $(TESTS)"
	mkdir -p $(BUILD)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	cat $(TESTS) \
	    | $(CC) $(INC:%=-I%) -DNALA_GENERATE_MOCKS -x c -E - \
	    | $(NALA) generate_mocks $(MOCKGENFLAGS) -o $(BUILD)
	cat $(TESTS) \
	    | $(CC) -MM -MT $@ $(CFLAGS) -x c -o $@.d -
	touch $@

-include $(DEPS)
