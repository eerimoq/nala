NALA ?= nala
BUILD = build
EXE = $(BUILD)/app
INC += $(BUILD)
INC += $(CURDIR)
INC += $(shell $(NALA) include_dir)
SRC += $(BUILD)/nala_mocks.c
SRC += $(shell $(NALA) c_sources)
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
CFLAGS += -coverage
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Werror
CFLAGS += -Wno-unused-command-line-argument
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
LIBS ?=

.PHONY: all build generate clean coverage gdb gdb-run auto auto-run help

all: build
	$(EXE) $(EXEARGS)

auto:
	$(MAKE) || true
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

coverage:
	gcovr $(GCOVR_ARGS) --html-details --output index.html $(BUILD)
	mkdir -p $(BUILD)/coverage
	mv index.* $(BUILD)/coverage
	@echo "Code coverage report: $$(readlink -f $(BUILD)/coverage/index.html)"

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

help:
	@echo "TARGET        DESCRIPTION"
	@echo "---------------------------------------------------------"
	@echo "all           Build and run with given ARGS."
	@echo "auto          Build and run with given ARGS on source change."
	@echo "clean         Remove build output."
	@echo "coverage      Create the code coverage report."
	@echo "gdb           Debug given test TEST with gdb."

$(EXE): $(OBJ)
	echo "LD $@"
	$(CC) $(CFLAGS) @$(BUILD)/nala_mocks.ldflags $^ $(LIBS:%=-l%) -o $@

define COMPILE_template
$(patsubst %.c,$(BUILD)%.o,$(abspath $1)): $1 $(BUILD)/nala_mocks.ldflags
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
	$(NALA) cat $(TESTS) \
	    | $(CC) $(CFLAGS) -DNALA_GENERATE_MOCKS -x c -E - \
	    | $(NALA) generate_mocks $(MOCKGENFLAGS) -o $(BUILD)
	cat $(TESTS) \
	    | $(CC) -MM -MT $@ $(CFLAGS) -x c -o $@.d -
	touch $@

-include $(DEPS)
