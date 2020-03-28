TESTS += collect
TESTS += dummy_functions

all:
	for f in $(TESTS) ; do \
	    gcc -E -I../../nala/dist test_$${f}_tests.c > test_$${f}_tests.pp.c ; \
	done
	gcc -c test_wrap_internal_symbols.c
	gcc -c \
	    -ffunction-sections \
	    -o test_wrap_internal_symbols_function_sections.o \
	    test_wrap_internal_symbols.c
