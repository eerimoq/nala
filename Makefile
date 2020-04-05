.PHONY: test dist

all:
	$(MAKE) dist
	$(MAKE) test
	$(MAKE) -C examples/basic clean
	$(MAKE) -C examples/basic
	$(MAKE) -C examples/mock_square clean
	$(MAKE) -C examples/mock_square
	$(MAKE) -C examples/no_implementation clean
	$(MAKE) -C examples/no_implementation
	$(MAKE) -C examples/failures clean
	! $(MAKE) -C examples/failures
	rm -rf my-suite
	PYTHONPATH=. python3 -m nala init my-suite
	$(MAKE) -C my-suite NALA="PYTHONPATH=.. python3 -m nala"
	test -f my-suite/report.json
	! test -f my-suite/build/report.json
	rm my-suite/report.json
	$(MAKE) -C my-suite \
	    NALA="PYTHONPATH=.. python3 -m nala" \
	    ARGS="--report-json-file build/report.json"
	! test -f my-suite/report.json
	test -f my-suite/build/report.json
	rm my-suite/build/report.json
	! $(MAKE) -C my-suite \
	    NALA="PYTHONPATH=.. python3 -m nala" \
	    ARGS="--report-json-file foo/report.json"
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
