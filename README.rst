|buildstatus|_
|coverage|_
|codecov|_

🦁 Nala
=======

A delightful test framework for `C` projects.

Based on `Narwhal`_ and `Narmock`_.

Features
========

- Automatic test discovery
- Use the same generic assertions everywhere
- Assertion failures reported as diffs
- Powerful mocking
- Easy-to-use output capturing utilities
- Traceback(s) on failure
- Works well with errors reported by sanitizers
- Test isolation with ``fork()``
- Only run tests matching given pattern
- Automatic build and run on file change
- Optional parallel test execution to save time
- Amalgamated source and header files ready to drop in your project
- JSON test report

Installation
============

It's recommended to install `Nala` with ``pip``. Only Python 3 is
supported.

.. code-block:: bash

   $ pip install nala

Alternatively, if mocking is not needed, you can download the
`amalgamated`_ header and source files:

- `nala.h`_
- `nala.c`_

Drop the two files in your project, make sure ``nala.c`` is compiled
and linked just like the other source files of your test program and
you should be good to go.

Getting started
===============

|tryit|_

Use ``nala init foo`` to create a folder ``foo`` with two test files,
``test_assertions.c`` and ``test_time.c``. The first uses all
assertions and captures output, and the second mocks the time
function.

.. code-block:: bash

   $ nala init foo
   Run 'make -C foo' to build and run all tests!

The assertions tests looks like this:

.. code-block:: c

   #include "nala.h"

   TEST(assertions)
   {
       int i;
       int array[] = { 1, 5, 2 };

       ASSERT_EQ(NULL, NULL);
       ASSERT_NE(1, 2);
       ASSERT_LT(1.0, 2.0);
       ASSERT_LE(1, 1);
       ASSERT_GT(2L, 1L);
       ASSERT_GE(1, 1);
       ASSERT_TRUE(true);
       ASSERT_FALSE(false);
       ASSERT_SUBSTRING("12345", "34");
       ASSERT_NOT_SUBSTRING("12345", "4567");
       ASSERT_MEMORY_EQ("abcd", "abcd", 5);
       ASSERT_FILE_EQ("test_assertions.c", "test_assertions.c");
       ASSERT_ARRAY_EQ(array, array, sizeof(array));
       ASSERT_FUNCTION_POINTER_EQ(assertions, assertions);
       ASSERT_FUNCTION_POINTER_NE(assertions, NULL);
       ASSERT(1 == 1);

       CAPTURE_OUTPUT(output, errput) {
           printf("output!\n");
           fprintf(stderr, "errput!\n");
       }

       ASSERT_EQ(output, "output!\n");
       ASSERT_EQ(errput, "errput!\n");

       for (i = 0; i < 3; i++) {
           WITH_MESSAGE("i: %d", i) {
               ASSERT_EQ(array[i], array[i]);
           }
       }
   }

And the time tests:

.. code-block:: c

   #include <time.h>
   #include "nala.h"

   TEST(mock_time)
   {
       time_mock_once(42);

       ASSERT_EQ(time(NULL), 42);
   }

Build and run all tests with ``make -s -C foo``.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run.png

Build all tests but only run those whose name contains ``time``. This
is done by giving ``ARGS=time``.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run-one-test.png

Run ``make -s -C foo auto`` to build and run all tests automatically
when any source file in your project is modified. With the command
running, make the time test fail by returning ``41`` instead of ``42``
from the time mock.

.. code-block:: c

   #include <time.h>
   #include "nala.h"

   TEST(mock_time)
   {
       time_mock_once(41);

       ASSERT_EQ(time(NULL), 42);
   }

Notice how the test is built and run automatically.

.. image:: https://github.com/eerimoq/nala/raw/master/docs/build-and-run-assert-eq-fail.png

Create a code coverage report and open it with Firefox.

.. code-block::

   $ make -s -C foo coverage
   Code coverage report: /home/erik/workspace/nala/foo/build/coverage/index.html
   $ firefox /home/erik/workspace/nala/foo/build/coverage/index.html

API
===

Below is a list of all macros and functions in the API. They are all
defined/declared in `include/nala.h`_.

Assertions
----------

.. code-block:: c

   ASSERT_EQ(actual, expected);                  // Assert that given characters, numbers, pointers or
                                                 // strings are equal.
   ASSERT_NE(actual, expected);                  // Assert that given characters, numbers, pointers or
                                                 // strings are not equal.
   ASSERT_LT(actual, expected);                  // Assert that actual is less than expected.
   ASSERT_LE(actual, expected);                  // Assert that actual is less than or equal to expected.
   ASSERT_GT(actual, expected);                  // Assert that actual is greater than expected.
   ASSERT_GE(actual, expected);                  // Assert that actual is greater than or equal to
                                                 // expected.
   ASSERT_TRUE(actual);                          // Assert that given value is true.
   ASSERT_FALSE(actual);                         // Assert that given value is false.
   ASSERT_SUBSTRING(haystack, needle);           // Assert that given haystack string contains given
                                                 // needle string.
   ASSERT_NOT_SUBSTRING(haystack, needle);       // Assert that given haystack string does not contain
                                                 // given needle string.
   ASSERT_MEMORY_EQ(actual, expected, size);     // Assert that given memory regions are equal.
   ASSERT_FILE_EQ(actual, expected);             // Assert that given files are equal.
   ASSERT_ARRAY_EQ(actual, expected, size);      // Assert that given arrays are equal.
   ASSERT_FUNCTION_POINTER_EQ(actual, expected); // Assert that given function pointers are equal.
   ASSERT_FUNCTION_POINTER_NE(actual, expected); // Assert that given function pointers are not equal.
   ASSERT(cond);                                 // Assert that given condition is true.

Other macros
------------

.. code-block:: c

   FAIL(message);                                // Fail current test with given message.
   CAPTURE_OUTPUT(stdout_name, stderr_name);     // A capture output block.
   WITH_MESSAGE(format, ...);                    // Additional message on error block. May be nested.

Functions
---------

.. code-block:: c

   void *nala_alloc(size_t size);                // Allocate a memory buffer of given size that is
                                                 // automatically freed after the test. Always returns
                                                 // a valid pointer.
   void nala_auto_free(void *buf_p);             // Automatically free given buffer after the test.
                                                 // free() is called to free the buffer.
   void nala_exit(int status);                   // Performs post-test checks and cleanup, and then
                                                 // exits with status 0. The status parameter is
                                                 // ignored.

Mocking
=======

Generating mocks
----------------

The ``nala generate_mocks`` command finds mocked functions in your
test code and generates ``nala_mocks.h``, ``nala_mocks.c`` and
``nala_mocks.ldflags``. The first two files declare and define mocks,
while the last file contains linker flags.

Use ``--rename-parameters-file`` to rename function parameters, often
useful when mocking standard library functions. If not given, Nala
renames `a few function parameters`_ by default. Also, any
``__``-prefix is removed from all parameters.

Use ``--no-rename-parameters`` not to rename any function
parameters. Overrides ``--rename-parameters-file``.

Use ``--no-implementation`` when the implementation of functions
matching given Unix shell-style wildcards pattern are not available in
the binary (and therefore should not be called by the generated code,
as the linker would give a relocation error). This option may be given
multipe times. An alternative to using this option is to manually
implement the missing functions. Here is an example implementation of
``foo()`` that makes the test fail if called.

.. code-block:: c

   int foo()
   {
       FAIL("No real implementation available!\n");

       return (0);
   }

Use ``--no-real-variadic-functions`` not to add any real variadic
functions. Nala adds `a few variadic functions`_ by default, given
that they are mocked.

Here is an example of how to generate mocks:

.. code-block:: bash

   $ nala cat *.c | gcc -DNALA_GENERATE_MOCKS -x c -E - | nala generate_mocks

``nala cat *.c`` should only concatenate test source files, not any
other source files in your project.

Nala requires test source code to be expanded by the preprocessor. You
can directly pipe the output of ``gcc -DNALA_GENERATE_MOCKS -x c -E
-`` to the command-line utility.

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

Variadic functions will *not* call the real implementation by
default. Give ``--implementation`` to ``nala generate_mocks`` to
generate calls to the real function (taking a ``va_list`` instead of
``...``).

There are plenty of mock-examples in the `examples folder`_. All
inline examples below can also be found in the `mock_api_examples`_
example.

For all functions
^^^^^^^^^^^^^^^^^

``<params>`` is all char-pointer (string) and primitive type
parameters of the mocked function.

Same behaviour for every call.

.. code-block:: c

   void FUNC_mock(<params>, <res>);     // check parameters and return
   void FUNC_mock_ignore_in(<res>);     // ignore parameters and return
   void FUNC_mock_none();               // no calls allowed
   void FUNC_mock_implementation(*);    // replace implementation
   void FUNC_mock_real();               // real implementation

An example:

.. code-block:: c

   /* int foo(int value); */

   TEST(foo_every_call)
   {
       foo_mock(1, 2);

       /* All calls to foo() expects its parameter to be 1 and returns 2. */
       ASSERT_EQ(foo(1), 2);
       ASSERT_EQ(foo(1), 2);
   }

Per call control.

.. code-block:: c

   int FUNC_mock_once(<params>, <res>); // check parameters and return once (per call)
                                        // returns a mock instance handle
   int FUNC_mock_ignore_in_once(<res>); // ignore parameters and return once (per call)
                                        // returns a mock instance handle
   void FUNC_mock_real_once();          // real implementation once (per call)

An example:

.. code-block:: c

   /* int foo(int value); */

   TEST(foo_per_call)
   {
       foo_mock_once(1, 2);
       foo_mock_once(4, 5);

       /* First call to foo() expects its parameter to be 1 and returns 2. */
       ASSERT_EQ(foo(1), 2);

       /* Second call to foo() expects its parameter to be 4 and returns 5. */
       ASSERT_EQ(foo(4), 5);

       /* Third call will fail and the test will end. */
       foo(10);
   }

Changes the behaviour of currect mock (most recent ``*_mock()`` or
``*_mock_once()`` call). Works for both per call and every call
functions above.

.. code-block:: c

   void FUNC_mock_set_errno(int);       // errno on return, 0 by default
   void FUNC_mock_set_callback(*);      // additional checks and/or actions
                                        // called just before returning from the mock

An example:

.. code-block:: c

   /* int foo(int value); */

   TEST(foo_set_errno)
   {
       foo_mock_once(1, 2);
       foo_mock_set_errno(EINVAL);

       ASSERT_EQ(foo(1), 2);
       ASSERT_EQ(errno, EINVAL);
   }

Get per call input parameters.

.. code-block:: c

   *FUNC_mock_get_params_in(int);       // get input parameters for given mock instance
                                        // handle

An example:

.. code-block:: c

   /* typedef void (*callback_t)(void); */
   /* void bar(callback_t callback); */

   static void fie(void)
   {
       printf("fie() called!\n");
   }

   TEST(bar_get_params_call_callback)
   {
       int handle;

       handle = bar_mock_once();

       bar(fie);

       /* Call the callback (calls fie()). */
       bar_mock_get_params_in(handle)->callback();
   }

For pointer and array function parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Changes the behaviour of currect mock (most recent ``*_mock()`` or
``*_mock_once()`` call). Works for both per call and every call
functions above.

.. code-block:: c

   void FUNC_mock_set_PARAM_in(*, size_t);  // check on input
   void FUNC_mock_set_PARAM_in_assert(*);   // custom assert function on input
   void FUNC_mock_set_PARAM_in_pointer(*);  // check pointer (the address) on input
   void FUNC_mock_set_PARAM_out(*, size_t); // value on return
   void FUNC_mock_set_PARAM_out_copy(*);    // custom output copy function

An example:

.. code-block:: c

   /* struct foo_t { char *string_p }; */

   /* void fum(int *value_p, struct foo_t *foo_p); */

   static void assert_foo_string(struct foo_t *actual_p,
                                 struct foo_t *expected_p,
                                 size_t size)
   {
       ASSERT_EQ(size, sizeof(*expected_p));
       ASSERT_EQ(actual_p->string_p, expected_p->string_p);
   }

   TEST(fum_in_out)
   {
       int value;
       struct foo_t foo;

       fum_mock_once();
       value = 1;
       fum_mock_set_value_p_in(&value, sizeof(value));
       value = 2;
       fum_mock_set_value_p_out(&value, sizeof(value));

       foo.string_p = "Hello!";
       fum_mock_set_foo_p_in(&foo, sizeof(foo));
       fum_mock_set_foo_p_in_assert(assert_foo_string);

       value = 1;
       foo.string_p = "Hello!";
       fum(&value, &foo);
       ASSERT_EQ(value, 2);
   }

For function parameters part of <params>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Changes the behaviour of currect mock (most recent ``*_mock()`` or
``*_mock_once()`` call). Works for both per call and every call
functions above.

.. code-block:: c

   void FUNC_mock_ignore_PARAM_in();        // ignore on input

An example:

.. code-block:: c

   /* void foo(int value); */

   TEST(foo_ignore_value)
   {
       foo_mock_once(1, 2);
       foo_mock_ignore_value_in();

       ASSERT_EQ(foo(9), 2);
   }

For variadic functions
^^^^^^^^^^^^^^^^^^^^^^

Variadic function mocks are slightly different from the above. Their
parameter list is extended with a format string (``format``) and an
ellipsis (``...``), as shown below.

.. code-block:: c

   void FUNC_mock(<params>, <res>, format, ...);
   void FUNC_mock_once(<params>, <res>, format, ...);
   void FUNC_mock_ignore_in(<res>, format);
   void FUNC_mock_ignore_in_once(<res>, format);

The format string supports the following specifiers.

.. code-block::

   %d  - signed integer
   %u  - unsigned integer
   %ld - signed long integer
   %lu - unsigned long integer
   %p  - pointer address
   %s  - string

The ``%p`` specifier takes no value when calling the mock function,
just like pointers are not part of the mock function parameters
list. Instead, set pointers after the mock call.

.. code-block:: c

   foo_mock(3, "%d%p%s", 5, "the-string");
   foo_mock_set_va_arg_in_pointer_at(1, NULL);

The variadic parameters are controlled by index instead of name.

.. code-block:: c

   void FUNC_mock_ignore_va_arg_in_at(uint);          // ignore on input
   void FUNC_mock_set_va_arg_in_at(uint, *, size_t);  // check on input
   void FUNC_mock_set_va_arg_in_assert_at(uint, *);   // custom assert function on input
   void FUNC_mock_set_va_arg_in_pointer_at(uint, *);  // check pointer (the address) on input
   void FUNC_mock_set_va_arg_out_at(uint, *, size_t); // value on return
   void FUNC_mock_set_va_arg_out_copy_at(uint, *);    // custom output copy function

Limitations
-----------

- Structs and unions passed by value are ignored.

- ``va_list`` parameters are ignored.

- ``malloc()`` and ``free()`` can't be mocked if forking and using
  gcov. They probably can if wrapping ``__gcov_fork()`` in an
  suspend/resume-block.

- ``static`` functions can't be mocked.

- Only primitive data type members are asserted when comparing
  structs. Pointer and union members are not asserted, and probably
  never will be.

Debugging tips
==============

Nala executes each test in its own process. This means that following
the execution of a test with a debugger can be a bit tricky because
debuggers like `GDB`_ can only follow a single process at a time.

If you're using `GDB`, set a breakpoint at ``<test>_before_fork`` and
then run the program until it stops at the breakpoint. Before
continuing the program execution, tell `GDB` to follow the forked test
process by setting ``follow-fork-mode`` to ``child``.

Below are all commands to debug the ``assertions`` test in the example
above.

.. code-block::

   $ gdb foo/build/app
   (gdb) b assertions_before_fork
   (gdb) r
   (gdb) set follow-fork-mode child
   (gdb) c

The ``gdb`` make target starts `GDB` and runs all commmands listed
above. Set the ``TEST`` make variable to the test to debug. Both
``TEST=test_assertions::assertions`` and ``TEST=assertions`` are
accepted. The test name may be partialy given, as long as it only
matches one test.

.. code-block::

   $ make -s -C foo gdb TEST=assertions

The test program takes optional arguments as below, which also can be
helpful when debugging, especially ``--print-all-calls``.

.. code-block::

   $ foo/build/app --help
   usage: foo/build/app [-h] [-v] [-c] [-a] [-r] [-f] [-j] [<test-pattern>]

   Run tests.

   positional arguments:
     test-pattern                  Only run tests matching given pattern. '^' matches
                                   the beginning and '$' matches the end of the test
                                   name.

   optional arguments:
     -h, --help                    Show this help message and exit.
     -v, --version                 Print version information.
     -c, --continue-on-failure     Continue on test failure.
     -a, --print-all-calls         Print all calls to ease debugging.
     -r, --report-json-file        JSON test report file (default: report.json).
     -f, --print-test-file-func    Print file:function for exactly one test.
     -j, --jobs                    Run given number of tests in parallel
                                   (default: 1).

Compiler flags
==============

Pass ``-no-pie -g -O0 -fsanitize=address`` to the compiler for better
error reporting.

Read more about sanitizers here: https://en.wikipedia.org/wiki/AddressSanitizer

Other unit test frameworks
==========================

Other C unit test frameworks with similar feature set as Nala.

- `CMock`_ + `Unity`_

- `cmocka`_

.. |buildstatus| image:: https://travis-ci.org/eerimoq/nala.svg?branch=master
.. _buildstatus: https://travis-ci.org/eerimoq/nala

.. |coverage| image:: https://coveralls.io/repos/github/eerimoq/nala/badge.svg?branch=master
.. _coverage: https://coveralls.io/github/eerimoq/nala

.. |codecov| image:: https://codecov.io/gh/eerimoq/nala/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/nala

.. _Narwhal: https://github.com/vberlier/narwhal
.. _Narmock: https://github.com/vberlier/narmock

.. |tryit| image:: https://img.shields.io/badge/try-online-f34b7d.svg
.. _tryit: https://repl.it/@eerimoq/nala

.. _amalgamated: https://sqlite.org/amalgamation.html
.. _nala.h: https://raw.githubusercontent.com/eerimoq/nala/master/nala/dist/nala.h
.. _nala.c: https://raw.githubusercontent.com/eerimoq/nala/master/nala/dist/nala.c

.. _a few function parameters: https://github.com/eerimoq/nala/blob/master/nala/rename_parameters.txt

.. _a few variadic functions: https://github.com/eerimoq/nala/blob/master/nala/real_variadic_functions.c

.. _include/nala.h: https://github.com/eerimoq/nala/blob/master/include/nala.h

.. _GDB: https://www.gnu.org/software/gdb/

.. _CMock: https://github.com/ThrowTheSwitch/CMock

.. _Unity: https://github.com/ThrowTheSwitch/Unity

.. _cmocka: https://cmocka.org/

.. _examples folder: https://github.com/eerimoq/nala/tree/master/examples

.. _mock_api_examples: https://github.com/eerimoq/nala/tree/master/examples/mock_api_examples
