#!/bin/sh

#
# Copyright (c) 2020-2023 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#

# Script which executes Python tests for each Python package and generates coverage information.
# This is executed by the GitHub Actions CI build but also can be run locally.
# Make sure to install python_tests/requirements.txt before running the tests!

set -e

PYTHON=python3

# Comma separated list of directories to exclude from coverage
COVERAGE_EXCLUDES='--omit=python_tests/*'

echo ">> Running pytest for mbed_tools package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -m pytest python_tests/mbed_tools

# For everything after the first command, we pass the "-a" argument to coverage
# so that it appends to the existing coverage database
echo ">> Running pytest for memap package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m pytest python_tests/memap

echo ">> Running pytest for scancode_evaluate package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m pytest python_tests/scancode_evaluate

# Note: For some reason, they decided not to add a "_test" suffix on the
# test case filenames for some packages.  So, the "-p *.py" argument is needed to
# make it look for any files as tests, not just ones ending in _test.py.

echo ">> Running unittests for mbed_greentea package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m unittest discover -p '*.py' python_tests.mbed_greentea

echo ">> Running unittests for mbed_host_tests package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m unittest discover -p '*.py' python_tests.mbed_host_tests

echo ">> Running unittests for mbed_lstools package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m unittest discover -p '*.py' python_tests.mbed_lstools

echo ">> Running unittests for mbed_os_tools package"
$PYTHON -m coverage run "$COVERAGE_EXCLUDES" -a -m unittest discover -p '*.py' python_tests.mbed_os_tools

echo ">> Generating coverage report under htmlcov/"
$PYTHON -m coverage html
