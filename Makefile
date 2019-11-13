.PHONY: test dist

all:
	$(MAKE) dist
	$(MAKE) test
	$(CC) nala/dist/nala.c
	$(MAKE) -C examples/basic
	! $(MAKE) -C examples/failures

test:
	$(MAKE) -C tst

dist:
	amalgamate/amalgamate.py -c amalgamate/nala.h.json -s .
	amalgamate/amalgamate.py -c amalgamate/nala.c.json -s .

release-to-pypi:
	python setup.py sdist
	python setup.py bdist_wheel --universal
	twine upload dist/*.gz dist/*.whl
