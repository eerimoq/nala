all:
	echo "\n### Run 'nala init'.\n"
	rm -rf test
	PYTHONPATH=../.. python3 -m nala init test
	$(MAKE) -C test
	echo "\n### Add the foo module.\n"
	cp 1/* test
	$(MAKE) -C test
	echo "\n### Add a test of myfunction().\n"
	cp 2/* test
	$(MAKE) -C test
	echo "\n### Add a test of myfunction() with the internal function" \
	     "square() mocked.\n"
	cp 3/* test
	$(MAKE) -C test
