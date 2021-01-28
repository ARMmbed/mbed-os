# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

"""Script to check if a Python module/package is installed."""

import sys

try:
    __import__(sys.argv[1])
except ImportError:
    exit(1)

exit(0)
