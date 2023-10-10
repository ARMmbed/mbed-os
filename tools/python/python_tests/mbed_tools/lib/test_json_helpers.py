#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import json

import pytest

from mbed_tools.lib.json_helpers import decode_json_file


def test_invalid_json(tmp_path):
    lib_json_path = tmp_path / "mbed_lib.json"
    lib_json_path.write_text("name")

    with pytest.raises(json.JSONDecodeError):
        decode_json_file(lib_json_path)
