SRC = \
	../../src/nala.c \
	../../src/diff/diff.c \
	../../src/utils.c \
	../../src/hexdump/hexdump.c \
	../../src/subprocess.c \
	../../src/traceback.c \
	subprocess.c \
	main.c

all:
	$(CC) -I../../include $(SRC) -Wall -Wextra -Og -g -o main
	./main
