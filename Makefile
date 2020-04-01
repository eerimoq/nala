.PHONY: test dist

all:
	$(MAKE) dist
	$(MAKE) test
	$(MAKE) -C examples/basic clean
	$(MAKE) -C examples/basic
	$(MAKE) -C examples/mock_square clean
	$(MAKE) -C examples/mock_square
	$(MAKE) -C examples/failures clean
	! $(MAKE) -C examples/failures
	rm -rf my-suite
	PYTHONPATH=. python3 -m nala init my-suite
	$(MAKE) -C my-suite NALA="PYTHONPATH=.. python3 -m nala"
	$(MAKE) -C my-suite clean
	$(MAKE) -C my-suite NALA="PYTHONPATH=.. python3 -m nala" SANITIZE=yes

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
