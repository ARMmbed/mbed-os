#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pytest

from mbed_tools.lib.python_helpers import flatten_nested


@pytest.mark.parametrize(
    "input_list, expected_result",
    (
        ([1, 2, 3, 4, 5], [1, 2, 3, 4, 5]),
        ([1, [2], 3, 4, 5], [1, 2, 3, 4, 5]),
        ([[1, 2, 3], [4, 5]], [1, 2, 3, 4, 5]),
        ([(1, 2, 3), (4, 5)], [1, 2, 3, 4, 5]),
        ([[1, [[2, [3]]]], [4, 5]], [1, 2, 3, 4, 5]),
        ([["alan", [["bob", ["sally"]]]], ["jim", "jenny"]], ["alan", "bob", "sally", "jim", "jenny"]),
    ),
)
def test_flatten_nested_list(input_list, expected_result):
    assert flatten_nested(input_list) == expected_result
