NALA=PYTHONPATH=$$NALA_ROOT python3 -m nala

all:
	echo "\n### Run 'nala init'.\n"
	rm -rf test
	$(NALA) init test
	$(MAKE) -C test NALA="$(NALA)"
	echo "\n### Add the foo module.\n"
	cp 1/* test
	$(MAKE) -C test NALA="$(NALA)"
	echo "\n### Add a test of myfunction().\n"
	cp 2/* test
	$(MAKE) -C test NALA="$(NALA)"
	echo "\n### Add a test of myfunction() with the internal function" \
	     "square() mocked.\n"
	cp 3/* test
	$(MAKE) -C test NALA="$(NALA)"
