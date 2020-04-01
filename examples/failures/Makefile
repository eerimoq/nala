TESTS += main.c
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += ../../nala/dist
SRC += $(BUILD)/nala_mocks.c
SRC += ../../nala/dist/nala.c
SRC += foo.c
SRC += $(TESTS)
OBJ = $(patsubst %,$(BUILD)%,$(abspath $(SRC:%.c=%.o)))
OBJDEPS = $(OBJ:%=%.d)
MOCKGENDEPS = $(BUILD)/nala_mocks.ldflags.d
DEPS = $(OBJDEPS) $(MOCKGENDEPS)
CFLAGS += -g -O0 -no-pie
CFLAGS += -fsanitize=address
CFLAGS += $(INC:%=-I%)
NALA ?= PYTHONPATH=../.. python3 -m nala

all:
	$(MAKE) $(BUILD)/nala_mocks.ldflags
	$(MAKE) $(EXE)
	$(EXE) $(ARGS) --continue-on-failure

clean:
	rm -rf $(BUILD)

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) $(CFLAGS) @$(BUILD)/nala_mocks.ldflags $^ -o $@

define COMPILE_template
$(patsubst %.c,$(BUILD)%.o,$(abspath $1)): $1
	@echo "CC $1"
	mkdir -p $$(@D)
	$$(CC) $$(CFLAGS) -c -o $$@ $$<
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $$@
	$(CC) -MM -MT $$@ $(CFLAGS) -o $$@.d $$<
endef
$(foreach file,$(SRC),$(eval $(call COMPILE_template,$(file))))

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
