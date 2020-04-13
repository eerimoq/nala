BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += $(CURDIR)
SRC += $(BUILD)/nala_mocks.c
SRC += $(TESTS)
OBJ = $(patsubst %,$(BUILD)%,$(abspath $(SRC:%.c=%.o)))
OBJDEPS = $(OBJ:%.o=%.d)
MOCKGENDEPS = $(BUILD)/nala_mocks.ldflags.d
DEPS = $(OBJDEPS) $(MOCKGENDEPS)
CFLAGS += $(INC:%=-I%)
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -no-pie
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Wduplicated-branches
CFLAGS += -Wduplicated-cond
CFLAGS += -Wjump-misses-init
CFLAGS += -Wlogical-op
CFLAGS += -Wnull-dereference
CFLAGS += -Wrestrict
CFLAGS += -Werror
CFLAGS += -Wshadow
ifeq ($(SANITIZE), yes)
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=undefined
endif
MOCKGENFLAGS += $(IMPLEMENTATION:%=-i %)
MOCKGENFLAGS += $(NO_IMPLEMENTATION:%=-n %)
REPORT_JSON = $(BUILD)/report.json
EXEARGS += $(ARGS)
EXEARGS += $(JOBS:%=-j %)
EXEARGS += $(REPORT_JSON:%=-r %)
NALA ?= PYTHONPATH=$(NALA_ROOT) python3 -m nala

.PHONY: all build generate clean coverage gdb gdb-run auto auto-run

all:
	$(MAKE) build
	$(EXE) $(EXEARGS)

auto: all
	while true ; do \
	    $(MAKE) auto-run ; \
	done

auto-run:
	for f in $(OBJDEPS) ; do \
	    ls -1 $$(cat $$f | sed s/\\\\//g | sed s/.*://g) ; \
	done | sort | uniq | grep -v $(BUILD) | entr -d -p $(MAKE)

build:
	$(MAKE) generate
	$(MAKE) $(EXE)

generate: $(BUILD)/nala_mocks.ldflags

clean:
	rm -rf $(BUILD)

# Recursive make for helpful output.
gdb:
	test_file_func=$$($(EXE) --print-test-file-func $(TEST)) && \
	$(MAKE) gdb-run TEST_FILE_FUNC=$$test_file_func

gdb-run:
	gdb \
	    -ex "b $(TEST_FILE_FUNC)_before_fork" \
	    -ex "r $(TEST)" \
	    -ex "set follow-fork-mode child" \
	    -ex c \
	    $(EXE)

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) $(CFLAGS) @$(BUILD)/nala_mocks.ldflags $^ -o $@

define COMPILE_template
$(patsubst %.c,$(BUILD)%.o,$(abspath $1)): $1
	@echo "CC $1"
	mkdir -p $$(@D)
	$$(CC) -MMD $$(CFLAGS) -c -o $$@ $$<
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $$@
endef
$(foreach file,$(SRC),$(eval $(call COMPILE_template,$(file))))

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $(TESTS)"
	mkdir -p $(@D)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	cat $(TESTS) \
	    | $(CC) $(CFLAGS) -DNALA_GENERATE_MOCKS -x c -E - \
	    | $(NALA) generate_mocks $(MOCKGENFLAGS) -o $(BUILD)
	cat $(TESTS) \
	    | $(CC) -MM -MT $@ $(CFLAGS) -x c -o $@.d -
	touch $@

-include $(DEPS)
