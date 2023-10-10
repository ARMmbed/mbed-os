#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Helpers for python language related functions."""
from typing import Iterable, List


def flatten_nested(input_iter: Iterable) -> List:
    """Flatten a nested Iterable with arbitrary levels of nesting.

    If the input is an iterator then this function will exhaust it.

    Args:
        input_iter: The input Iterable which may or may not be nested.

    Returns:
        A flat list created from the input_iter.
        If input_iter has no nesting its elements are appended to a list and returned.
    """
    output = []
    for elem in input_iter:
        if isinstance(elem, Iterable) and not isinstance(elem, str):
            output += flatten_nested(elem)
        else:
            output.append(elem)

    return output
