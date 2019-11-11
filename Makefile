.PHONY: test dist

all:
	$(MAKE) dist
	$(MAKE) test
	$(CC) dist/nala.c

test:
	$(MAKE) -C tst

dist:
	amalgamate/amalgamate.py -c amalgamate/nala.h.json -s .
	amalgamate/amalgamate.py -c amalgamate/nala.c.json -s .
