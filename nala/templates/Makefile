SRC = __mocks__.c nala.c main.c
NALA = nala

test: __mocks__.h
	$(CC) $$(cat __mocks__.ld) $(SRC)
	./a.out

__mocks__.h: main.c
	$(CC) -E main.c | $(NALA) generate_mocks
