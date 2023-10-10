#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Utilities with regards to Win32 component descriptors."""
from typing import Any, NamedTuple, Union
from collections import OrderedDict

UNKNOWN_VALUE = "Unknown"


def is_undefined_value(value: Any) -> bool:
    """Checks whether a field value is considered as undefined or not."""
    return value in [None, UNKNOWN_VALUE, 0, False]


def is_undefined_data_object(data_object: NamedTuple) -> bool:
    """Checks whether a data object is considered as undefined or not."""
    for f in data_object._fields:
        if not is_undefined_value(getattr(data_object, f)):
            is_undefined = False
            break
    else:
        is_undefined = True
    return is_undefined


def retain_value_or_default(value: Any) -> Any:
    """Returns the value if set otherwise returns a default value."""
    return UNKNOWN_VALUE if is_undefined_value(value) else value


def data_object_to_dict(data_object: NamedTuple) -> dict:
    """Gets a data object as a dictionary."""
    internal_dictionary: Union[dict, OrderedDict] = data_object._asdict()
    return dict(internal_dictionary) if isinstance(internal_dictionary, OrderedDict) else internal_dictionary
