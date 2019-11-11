|buildstatus|_
|codecov|_

🦁 Nala
=======

A test framework for `C` projects.

Based on `narwhal`_ and `narmock`_.

Features
========

- Automatic test discovery
- Use the same generic assertions everywhere
- Assertion failures reported as diffs
- Easy-to-use output capturing utilities
- Amalgamated source file and header ready to drop in your project
- Mocking support
- Works well with errors reported by sanitizers

Installation
============

Nala is distributed as an `amalgamated`_ source file and header:

- `nala.h`_
- `nala.c`_

Drop the two files in your project, make sure `nala.c` is compiled
and linked just like the other source files of your test program and
you should be good to go.

Example
=======

First of all, download ``nala.[hc]``.

.. code-block:: text

   $ wget https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.h
   $ wget https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.c

Create ``main.c`` and write a test case using all assertions and
capturing output.

.. code-block:: c

   #include "nala.h"

   TEST(assertions)
   {
       ASSERT_EQ(NULL, NULL);
       ASSERT_NE(1, 2);
       ASSERT_LT(1.0, 2.0);
       ASSERT_LE(1, 1);
       ASSERT_GT(2L, 1L);
       ASSERT_GE(1, 1);
       ASSERT_SUBSTRING("12345", "34");
       ASSERT_NOT_SUBSTRING("12345", "4567");
       ASSERT_MEMORY("abcd", "abcd", 5);
       ASSERT(1 == 1);

       CAPTURE_OUTPUT(stdoutput, stderrput) {
           printf("std!\n");
           fprintf(stderr, "err!\n");
       }

       ASSERT_EQ(stdoutput, "std!\n");
       ASSERT_EQ(stderrput, "err!\n");
   }

Build and run the test.

.. code-block:: text

   $ gcc *.c && ./a.out
   std!
   err!

   Test results:

     PASSED assertions (0.38 ms)

   Tests: 1 passed, 1 total
   Time: 0.38 ms

Debugging tips
==============

Nala executes each test in its own process. This means that
following the execution of a test with a debugger can be a bit tricky
because debuggers like GDB can only follow a single process at a time.

If you're using GDB, set a breakpoint at ``<test>_before_fork`` and
then run the program until it stops at the breakpoint. Before
continuing the program execution, tell GDB to follow the forked test
process by setting ``follow-fork-mode`` to ``child``.

All commands are shown below for the test called ``foo``.

.. code-block::

   $ gdb ./a.out
   (gdb) b foo_before_fork
   (gdb) r
   (gdb) set follow-fork-mode child
   (gdb) c

Mocking
=======

Nala finds the functions mocked in your tests and generates mocks with
a slick API.

.. code-block:: c

   #include <time.h>

   #include "__mocks__.h"
   #include "narwhal.h"

   TEST(example)
   {
       time_mock_once(42);

       ASSERT_EQ(time(NULL), 42);
   }

Installation
------------

Clone the repository and run the narwhal script from the clone. Also
install dependencies with `pip`.

.. code-block::

   $ pip install nala

Getting started
---------------

The command-line utility provides two essential commands that should
make it possible to integrate Nala in any kind of build system.

.. code-block::

   usage: nala [-h] (-g [<code>] | -f) [-d <directory>]

   A minimal mocking utility for C projects.

   optional arguments:
     -h, --help      show this help message and exit
     -g [<code>]     generate mocks
     -f              output linker flags
     -d <directory>  mocks directory

Generating mocks
----------------

The `nala -g` command finds the functions mocked in your code and
generates a `__mocks__.c` file and a `__mocks__.h` file that
respectively define and declare all the required mocks.

.. code-block:: bash

   $ gcc -E *.c | nala -g

Nala requires source code to be expanded by the preprocessor. You
can directly pipe the output of `gcc -E` to the command-line utility.

Retrieving linker flags
-----------------------

The `nala -f` command reads the generated `__mocks__.h` file and
outputs the necessary linker flags for linking all your source files
together.

.. code-block:: bash

   $ gcc $(nala -f) *.c

Mock API
--------

The created mocks provides the following functions.

For all functions
%%%%%%%%%%%%%%%%%

.. code-block::

   <func>_mock(<params>, <res>)      - check parameters and return
   <func>_mock_once(<params>, <res>) - check parameters and return once (per call)
   <func>_mock_ignore_in(<res>)      - ignore parameters and return
   <func>_mock_ignore_in_once(<res>) - ignore parameters and return once (per call)
   <func>_mock_none()                - no calls allowed
   <func>_mock_set_errno(int)        - errno on return
   <func>_mock_implementation(*)     - replace implementation
   <func>_mock_disable()             - call real implementation
   <func>_mock_reset()               - mock reset
   <func>_mock_assert_completed()    - completion checks

For selected function parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

.. code-block::

   <func>_mock_ignore_<param>_in()               - ignore on input
   <func>_mock_set_<param>_in(*, size_t)         - check on input
   <func>_mock_set_<param>_in_pointer(*, size_t) - check pointer (the address) on input
   <func>_mock_set_<param>_out(*, size_t)        - value on return

For variadic functions
%%%%%%%%%%%%%%%%%%%%%%

.. code-block::

   <func>_mock_ignore_va_arg_in_at(uint)          - ignore on input
   <func>_mock_set_va_arg_in_at(uint, *, size_t)  - check on input
   <func>_mock_set_va_arg_in_pointer_at(uint, *)  - check pointer on input
   <func>_mock_set_va_arg_out_at(uint, *, size_t) - value on return

Module functions
%%%%%%%%%%%%%%%%

.. code-block::

   nala_reset_all_mocks()            - reset everything
   nala_assert_all_mocks_completed() - completion checks

.. |buildstatus| image:: https://travis-ci.org/eerimoq/nala.svg?branch=master
.. _buildstatus: https://travis-ci.org/eerimoq/nala

.. |codecov| image:: https://codecov.io/gh/eerimoq/nala/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/nala

.. _narwhal: https://github.com/vberlier/narwhal
.. _narmock: https://github.com/vberlier/narmock

.. _amalgamated: https://sqlite.org/amalgamation.html
.. _nala.h: https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.h
.. _nala.c: https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.c
