|buildstatus|_
|coverage|_
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
- Traceback(s) on failure
- Amalgamated source and header files ready to drop in your project
- Mocking support
- Works well with errors reported by sanitizers
- Test isolation with fork()
- JSON test report

Installation
============

It's recommended to install `Nala` with ``pip``.

.. code-block:: bash

   $ pip install nala

Alternatively, if mocking is not needed, you can download the
`amalgamated`_ header and source files:

- `nala.h`_
- `nala.c`_

Drop the two files in your project, make sure ``nala.c`` is compiled
and linked just like the other source files of your test program and
you should be good to go.

Example
=======

Use ``nala init foo`` to create a test folder called ``foo``.

.. code-block:: bash

   $ nala init foo
   Run 'make -C foo' to build and run all tests!

Two test files are created, ``foo/test_assertions.c`` and
``foo/test_time.c``. The first uses all assertions and captures
output, and the second mocks the time function.

The assertions tests:

.. code-block:: c

   #include "nala.h"

   TEST(assertions)
   {
       int array[] = { 1, 5, 2 };

       ASSERT_EQ(NULL, NULL);
       ASSERT_NE(1, 2);
       ASSERT_LT(1.0, 2.0);
       ASSERT_LE(1, 1);
       ASSERT_GT(2L, 1L);
       ASSERT_GE(1, 1);
       ASSERT_SUBSTRING("12345", "34");
       ASSERT_NOT_SUBSTRING("12345", "4567");
       ASSERT_MEMORY("abcd", "abcd", 5);
       ASSERT_ARRAY(array, array, sizeof(array));
       ASSERT(1 == 1);

       CAPTURE_OUTPUT(output, errput) {
           printf("std!\n");
           fprintf(stderr, "err!\n");
       }

       ASSERT_EQ(output, "std!\n");
       ASSERT_EQ(errput, "err!\n");
   }

The time tests:

.. code-block:: c

   #include <time.h>
   #include "nala.h"
   #include "nala_mocks.h"

   TEST(mock_time)
   {
       time_mock_once(42);

       ASSERT_EQ(time(NULL), 42);
   }

Build and run all tests.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run.png

Build all tests but only run those whose name contains ``time``.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run-one-test.png

Now, make the time test fail to see what an error report looks like.

.. code-block:: c

   #include <time.h>
   #include "nala.h"
   #include "nala_mocks.h"

   TEST(mock_time)
   {
       time_mock_once(-1);

       ASSERT_EQ(time(NULL), 42);
   }

Build and run all tests.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run-assert-eq-fail.png

Create a code coverage report and open it with firefox.

.. code-block::

   $ make -s -C foo coverage
   Code coverage report: /home/erik/workspace/nala/foo/build/coverage/index.html
   $ firefox /home/erik/workspace/nala/foo/build/coverage/index.html

Compiler flags
==============

Pass ``-no-pie -g -O0 -fsanitize=address`` to the compiler for better
error reporting.

Read more about sanitizers here: https://en.wikipedia.org/wiki/AddressSanitizer

Debugging tips
==============

Nala executes each test in its own process. This means that
following the execution of a test with a debugger can be a bit tricky
because debuggers like GDB can only follow a single process at a time.

If you're using GDB, set a breakpoint at ``<test>_before_fork`` and
then run the program until it stops at the breakpoint. Before
continuing the program execution, tell GDB to follow the forked test
process by setting ``follow-fork-mode`` to ``child``.

All commands are shown below for the ``assertions`` test in the
example above.

.. code-block::

   $ gdb ./a.out
   (gdb) b assertions_before_fork
   (gdb) r
   (gdb) set follow-fork-mode child
   (gdb) c

The test program takes optional arguments as below, which also can be
helpful when debugging, especially ``--print-all-calls``.

.. code-block::

   $ ./a.out
   usage: ./a.out [-h] [-v] [-c] [-a] [<test-pattern>]

   Run tests.

   positional arguments:
     test-pattern                  Only run tests containing given pattern.

   optional arguments:
     -h, --help                    Show this help message and exit.
     -v, --version                 Print version information.
     -c, --continue-on-failure     Always run all tests.
     -a, --print-all-calls         Print all calls to ease debugging.
     -r, --report-json-file        JSON test report file (default: report.json).

Mocking
=======

Generating mocks
----------------

The ``nala generate_mocks`` command finds the functions mocked in your
code and generates ``nala_mocks.h``, ``nala_mocks.c`` and
``nala_mocks.ldflags``. The first two files declare and define mocks,
while the last file contains linker flags.

Use ``--rename-parameters-file`` to rename function parameters, often
useful when mocking standard library functions. If not given, Nala
renames `a few function parameters`_ by default.

Use ``--no-rename-parameters`` not to rename any function
parameters. Overrides ``--rename-parameters-file``.

Use ``--no-implementation`` when the implementation of functions
starting with given pattern are not available in the binary (and
therefore should not be called by the generated code). This option may
be given multipe times.

Use ``--no-real-variadic-functions`` not to add real variadic
functions. Nala adds `a few variadic functions`_ by default, given
that they are mocked.

.. code-block:: bash

   $ cat *.c | gcc -DNALA_GENERATE_MOCKS -x c -E - | nala generate_mocks

Nala requires source code to be expanded by the preprocessor. You can
directly pipe the output of ``gcc -DNALA_GENERATE_MOCKS -x c -E -`` to
the command-line utility.

Mocking object-internal function calls
--------------------------------------

The GNU linker ``ld`` wrap feature (``--wrap=<symbol>``) does not wrap
object-internal function calls. As Nala implements mocking by wrapping
functions, object-internal function calls can't be mocked just using
the linker. To mock these, after compilation, run ``nala
wrap_internal_symbols ...`` for each object file, and then pass them
to the linker.

Also, local (``static``) functions can't be mocked, only global
functions can!

.. code-block:: Makefile

   %.o: %.c
           $(CC) -o $@ $<
           nala wrap_internal_symbols nala_mocks.ldflags $@

Mock API
--------

A function mock will call the real implementation by default. Use the
functions below to control mock behaviour.

Variadic functions will *not* call the real implementation by default
because an ellipsis (``...``) can't be passed to the real function, a
`va_list` is required, which may not be available. Give
``--implementation`` to ``nala generate_mocks`` to generate calls to
the real function (taking ``va_list``).

For all functions
^^^^^^^^^^^^^^^^^

Same behaviour for every call.

.. code-block::

   void <func>_mock(<params>, <res>)     - check parameters and return
   void <func>_mock_ignore_in(<res>)     - ignore parameters and return
   void <func>_mock_none()               - no calls allowed
   void <func>_mock_implementation(*)    - replace implementation
   void <func>_mock_real()               - call real implementation

Per call control.

.. code-block::

   int <func>_mock_once(<params>, <res>) - check parameters and return once (per call)
   int <func>_mock_ignore_in_once(<res>) - ignore parameters and return once (per call)
   void <func>_mock_real_once()          - call real implementation once (per call)

Change behaviour of currect mock. Works for both per call and every
call functions above.

.. code-block::

   void <func>_mock_set_errno(int)       - errno on return
   void <func>_mock_set_callback(*)      - additional checks and/or actions

Get per call input parameters.

.. code-block::

   *<func>_mock_get_params_in(int)       - get input parameters for given handle

For selected function parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block::

   void <func>_mock_ignore_<param>_in()        - ignore on input
   void <func>_mock_set_<param>_in(*, size_t)  - check on input
   void <func>_mock_set_<param>_in_assert(*)   - custom assert function on input
   void <func>_mock_set_<param>_in_pointer(*)  - check pointer (the address) on input
   void <func>_mock_set_<param>_out(*, size_t) - value on return
   void <func>_mock_set_<param>_out_copy(*)    - custom output copy function

For variadic functions
^^^^^^^^^^^^^^^^^^^^^^

Variadic functions mocks are slightly different from the above. They
also have a format string and an ellipsis in some "every call" and
"per call" functions.

.. code-block::

   void <func>_mock(<params>, <res>, format, ...)
   void <func>_mock_once(<params>, <res>, format, ...)

   Not yet implemented:

   void <func>_mock_ignore_in(<params>, <res>, format)
   void <func>_mock_ignore_in_once(<params>, <res>, format)

The format string supports the following specifiers.

.. code-block::

   %d  - signed integer
   %u  - unsigned integer
   %ld - signed long integer
   %lu - unsigned long integer
   %p  - pointer address
   %s  - string

The variadic parameters are controlled by index instead of name.

.. code-block::

   void <func>_mock_ignore_va_arg_in_at(uint)          - ignore on input
   void <func>_mock_set_va_arg_in_at(uint, *, size_t)  - check on input
   void <func>_mock_set_va_arg_in_assert_at(uint, *)   - custom assert function on input
   void <func>_mock_set_va_arg_in_pointer_at(uint, *)  - check pointer (the address) on input
   void <func>_mock_set_va_arg_out_at(uint, *, size_t) - value on return
   void <func>_mock_set_va_arg_out_copy_at(uint, *)    - custom output copy function

Limitations
-----------

- Structs and unions passed by value are ignored.

- ``va_list`` parameters are ignored.

- ``malloc()`` and ``free()`` can't be mocked if forking and using
  gcov. They probably can if wrapping ``__gcov_fork()`` in an
  suspend/resume-block.

- ``static`` functions can't be mocked.

.. |buildstatus| image:: https://travis-ci.org/eerimoq/nala.svg?branch=master
.. _buildstatus: https://travis-ci.org/eerimoq/nala

.. |coverage| image:: https://coveralls.io/repos/github/eerimoq/nala/badge.svg?branch=master
.. _coverage: https://coveralls.io/github/eerimoq/nala

.. |codecov| image:: https://codecov.io/gh/eerimoq/nala/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/nala

.. _narwhal: https://github.com/vberlier/narwhal
.. _narmock: https://github.com/vberlier/narmock

.. _amalgamated: https://sqlite.org/amalgamation.html
.. _nala.h: https://raw.githubusercontent.com/eerimoq/nala/master/nala/dist/nala.h
.. _nala.c: https://raw.githubusercontent.com/eerimoq/nala/master/nala/dist/nala.c

.. _a few function parameters: https://github.com/eerimoq/nala/blob/master/nala/rename_parameters.txt

.. _a few variadic functions: https://github.com/eerimoq/nala/blob/master/nala/real_variadic_functions.c
