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
NALA = nala

all:
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

$(BUILD)/nala_mocks.ldflags: $(TESTS)
	echo "MOCKGEN $^"
	mkdir -p $(BUILD)
	[ -f $(BUILD)/nala_mocks.h ] || touch $(BUILD)/nala_mocks.h
	$(CC) $(INC:%=-I%) -DNALA_GENERATE_MOCKS -E $(TESTS) \
	    | $(NALA) generate_mocks -o $(BUILD)
	touch $@
