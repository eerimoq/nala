.PHONY: test dist

all:
	$(MAKE) dist
	$(MAKE) test
	$(CC) nala/dist/nala.c
	$(MAKE) -C examples/basic
	! $(MAKE) -C examples/failures
	rm -rf my-suite
	PYTHONPATH=. python3 -m nala init my-suite
	$(MAKE) -C my-suite NALA="PYTHONPATH=.. python3 -m nala"

test: test-python test-c

test-python:
	python3 setup.py test

test-c:
	$(MAKE) -C tst

dist:
	amalgamate/amalgamate.py -c amalgamate/nala.h.json -s .
	amalgamate/amalgamate.py -c amalgamate/nala.c.json -s .

release-to-pypi:
	python setup.py sdist
	python setup.py bdist_wheel --universal
	twine upload dist/*.gz dist/*.whl
