TEST ?= all

ifneq ($(TEST), all)
TESTS = $(TEST:%=test_%.c)
else
TESTS += test_assertions.c
TESTS += test_time.c
endif

BUILD = build/$(TEST)
EXE = $(BUILD)/app
INC += $(BUILD)
INC += $(CURDIR)
SRC += $(BUILD)/nala_mocks.c
SRC += nala.c
SRC += $(TESTS)
TESTS_C = $(BUILD)/tests.c

test: $(BUILD)/nala_mocks.h
	$(CC) $(INC:%=-I%) $$(cat $(BUILD)/nala_mocks.ld) -g -O0 -no-pie $(SRC) -o $(EXE)
	$(EXE)

$(BUILD)/nala_mocks.h: $(TESTS)
	echo "MOCKGEN $^"
	mkdir -p $(BUILD)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	cat $(TESTS) > $(TESTS_C)
	$(CC) $(INC:%=-I%) -E $(TESTS_C) | nala generate_mocks -o $(BUILD)
