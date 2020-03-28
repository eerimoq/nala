TESTS += collect
TESTS += dummy_functions

all:
	for f in $(TESTS) ; do \
	    gcc -E -I../../nala/dist test_$${f}_tests.c > test_$${f}_tests.pp.c ; \
	done
	gcc -c -o foo.elf foo.c
