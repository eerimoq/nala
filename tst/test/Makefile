CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage \
	-Wall \
	-Wextra \
	-Og \
	-g
SRC = \
	../../src/nala.c \
	../../src/diff/diff.c \
	../../src/utils.c \
	../../src/hexdump/hexdump.c \
	../../src/subprocess.c \
	../../src/traceback.c \
	../../src/hf.c \
	subprocess.c \
	main.c

all:
	$(CC) -I../../include $(SRC) $(CFLAGS) -o main
	./main
