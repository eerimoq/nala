ASAN_FLAGS = -fsanitize=address
INC += -I../../dist
CFLAGS += $(INC)
CFLAGS += -Wextra
CFLAGS += $(ASAN_FLAGS)
NALA ?= PYTHONPATH=../.. python3 -m nala

test:
	$(CPP) $(INC) main.c | $(NALA) -g
	$(CC) $(CFLAGS) $$($(NALA) -f) *.c ../../dist/nala.c -o main
	./main
