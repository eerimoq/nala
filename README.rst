|buildstatus|_

About
=====

A `C` test framework.

Based on https://github.com/vberlier/nala.

Features
--------

- Automatic test discovery
- Use the same generic assertions everywhere
- Assertion failures reported as diffs
- Easy-to-use output capturing utilities
- Amalgamated source file and header ready to drop in your project
- Mocking support with [Narmock](https://github.com/eerimoq/narmock)
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

.. |buildstatus| image:: https://travis-ci.org/eerimoq/nala.svg?branch=master
.. _buildstatus: https://travis-ci.org/eerimoq/nala

.. _amalgamated: https://sqlite.org/amalgamation.html
.. _nala.h: https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.h
.. _nala.c: https://raw.githubusercontent.com/eerimoq/nala/master/dist/nala.c
