SRC = nala_mocks.c nala.c main.c
NALA = nala

test: nala_mocks.h
	$(CC) $$(cat nala_mocks.ld) $(SRC)
	./a.out

nala_mocks.h: main.c
	[ -f nala_mocks.h ] || touch nala_mocks.h
	$(CC) -E main.c | $(NALA) generate_mocks
