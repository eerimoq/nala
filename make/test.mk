NALA ?= PYTHONPATH=$(NALA_ROOT) python3 -m nala
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += $(CURDIR)
SRC += $(BUILD)/nala_mocks.c
SRC += $(TESTS)
# To evaluate once for fewer nala include_dir/c_sources calls.
INC := $(INC)
SRC := $(SRC)
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
CFLAGS += -Werror
CFLAGS += -Wshadow
CFLAGS += -Wno-unused-command-line-argument
CFLAGS_EXTRA += -Wjump-misses-init
CFLAGS_EXTRA += -Wlogical-op
CFLAGS += $(shell $(CC) -Werror $(CFLAGS_EXTRA) -c $(NALA_ROOT)/dummy.c -E 2> /dev/null \
		  && echo $(CFLAGS_EXTRA))
ifeq ($(SANITIZE), yes)
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=undefined
endif
CFLAGS += -DNALA_INCLUDE_NALA_MOCKS_H
MOCKGENFLAGS += $(IMPLEMENTATION:%=-i %)
MOCKGENFLAGS += $(NO_IMPLEMENTATION:%=-n %)
REPORT_JSON = $(BUILD)/report.json
EXEARGS += $(ARGS)
EXEARGS += $(JOBS:%=-j %)
EXEARGS += $(REPORT_JSON:%=-r %)
TESTS_PP_C = $(BUILD)/tests.pp.c
WRAP_INTERNAL_SYMBOLS ?= yes
NALA_CAT ?= $(NALA) -d cat

.PHONY: all build generate clean coverage gdb gdb-run auto auto-run

all: build
	$(EXE) $(EXEARGS)

auto: all
	while true ; do \
	    $(MAKE) auto-run ; \
	done

auto-run:
	for f in $(OBJDEPS) ; do \
	    ls -1 $$(cat $$f | sed s/\\\\//g | sed s/.*://g) ; \
	done | sort | uniq | grep -v $(BUILD) | entr -d -p $(MAKE)

build: generate
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
ifeq ($(WRAP_INTERNAL_SYMBOLS), yes)
$(patsubst %.c,$(BUILD)%.o,$(abspath $1)): $(BUILD)/nala_mocks.ldflags
endif
$(patsubst %.c,$(BUILD)%.o,$(abspath $1)): $1
	@echo "CC $1"
	mkdir -p $$(@D)
	$$(CC) -MMD $$(CFLAGS) -c -o $$@ $$<
ifeq ($(WRAP_INTERNAL_SYMBOLS), yes)
	$(NALA) wrap_internal_symbols $(BUILD)/nala_mocks.ldflags $$@
endif
endef
$(foreach file,$(SRC),$(eval $(call COMPILE_template,$(file))))

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $(TESTS)"
	mkdir -p $(@D)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	$(NALA_CAT) $(TESTS) \
	    | $(CC) $(CFLAGS) -DNALA_GENERATE_MOCKS -x c -E - > $(TESTS_PP_C)
	cat $(TESTS_PP_C) | $(NALA) -d generate_mocks $(MOCKGENFLAGS) -o $(BUILD)
	cat $(TESTS) \
	    | $(CC) -MM -MT $@ $(CFLAGS) -x c -o $@.d -
	touch $@

-include $(DEPS)
