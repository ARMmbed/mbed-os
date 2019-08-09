# Description

This document describes how to run minimal-printf tests.

## Running tests

You can use the following command to run tests:

`mbed test -m K64F -t GCC_ARM -n *printf* -v -c`

Do not use --profile minimal-printf so minimal-printf is not compared with itself.
