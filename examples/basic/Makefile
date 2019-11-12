ASAN_FLAGS = -fsanitize=address
INC += -I../../nala/dist
CFLAGS += $(INC)
CFLAGS += -Wextra
CFLAGS += $(ASAN_FLAGS)
NALA ?= PYTHONPATH=../.. python3 -m nala

test:
	$(CPP) $(INC) main.c | $(NALA) generate_mocks
	$(CC) $(CFLAGS) $$(cat __mocks__.ld) *.c ../../nala/dist/nala.c -o main
	./main
