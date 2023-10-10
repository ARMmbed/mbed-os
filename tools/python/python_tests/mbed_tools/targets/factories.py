#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from mbed_tools.targets import Board


def make_board(
    board_type="BoardType",
    board_name="BoardName",
    mbed_os_support=None,
    mbed_enabled=None,
    product_code="9999",
    slug="BoardSlug",
    target_type="TargetType",
):
    return Board(
        board_type=board_type,
        product_code=product_code,
        board_name=board_name,
        target_type=target_type,
        slug=slug,
        mbed_os_support=mbed_os_support if mbed_os_support else (),
        mbed_enabled=mbed_enabled if mbed_enabled else (),
        build_variant=(),
    )


def make_dummy_internal_board_data():
    return [dict(attributes=dict(board_type=str(i), board_name=str(i), product_code=str(i))) for i in range(10)]
