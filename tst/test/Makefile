CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Og \
	-g \
	-no-pie
SRC += ../../src/nala.c
SRC += ../../src/diff/diff.c
SRC += ../../src/utils.c
SRC += ../../src/hexdump/hexdump.c
SRC += ../../src/subprocess.c
SRC += ../../src/traceback.c
SRC += ../../src/hf.c
SRC += subprocess.c
SRC += main.c

all:
	$(CC) -I../../include $(SRC) $(CFLAGS) -o main
	./main
