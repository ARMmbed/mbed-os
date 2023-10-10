#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""An abstraction layer describing hardware supported by Mbed OS.

Querying board database
-----------------------

For the interface to query board database, look at `mbed_tools.targets.get_board`.

Fetching target data
____________________

For the interface to extract target data from their definitions in Mbed OS,
look at `mbed_tools.targets.get_target`.

Configuration
-------------

For details about configuration of this module, look at `mbed_tools.targets.config`.
"""
from mbed_tools.targets import exceptions
from mbed_tools.targets.get_target import (
    get_target_by_name,
    get_target_by_board_type,
)
from mbed_tools.targets.get_board import (
    get_board_by_product_code,
    get_board_by_online_id,
    get_board_by_jlink_slug,
)
from mbed_tools.targets.board import Board
