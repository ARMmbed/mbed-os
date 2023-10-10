#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Representation of an Mbed-Enabled Development Board and related utilities."""
from dataclasses import dataclass
from typing import Tuple


@dataclass(frozen=True, order=True)
class Board:
    """Representation of an Mbed-Enabled Development Board.

    Attributes:
        board_type: Type of board in format that allows cross-referencing with target definitions.
        board_name: Human readable name.
        product_code: Uniquely identifies a board for the online compiler.
        target_type: A confusing term that is not related to 'target' in other parts of the code.
        Identifies if a board in the online database is a `module` or a `platform` (modules are more
        complex and often have supplementary sensors, displays etc. A platform is a normal development board).
        slug: Used with target_type to identify a board's page on the mbed website.
        build_variant: Available build variants for the board.
        Can be used in conjunction with board_type for referencing targets.
        mbed_os_support: The versions of Mbed OS supported.
        mbed_enabled: Whether Mbed OS is supported or not.
    """

    board_type: str
    board_name: str
    product_code: str
    target_type: str
    slug: str
    build_variant: Tuple[str, ...]
    mbed_os_support: Tuple[str, ...]
    mbed_enabled: Tuple[str, ...]

    @classmethod
    def from_online_board_entry(cls, board_entry: dict) -> "Board":
        """Create a new instance of Board from an online database entry.

        Args:
            board_entry: A single entity retrieved from the board database API.
        """
        board_attrs = board_entry.get("attributes", {})
        board_features = board_attrs.get("features", {})

        return cls(
            # Online database has inconsistently cased board types.
            # Since this field is used to match against `targets.json`, we need to ensure consistency is maintained.
            board_type=board_attrs.get("board_type", "").upper(),
            board_name=board_attrs.get("name", ""),
            mbed_os_support=tuple(board_features.get("mbed_os_support", [])),
            mbed_enabled=tuple(board_features.get("mbed_enabled", [])),
            product_code=board_attrs.get("product_code", ""),
            target_type=board_attrs.get("target_type", ""),
            slug=board_attrs.get("slug", ""),
            # TODO: Remove this hard-coded example after demoing.
            # NOTE: Currently we hard-code the build variant for a single board type.
            # This is simply so we can demo the tools to PE. This must be removed and replaced with a proper mechanism
            # for determining the build variant for all platforms. We probably need to add this information to the
            # boards database.
            build_variant=tuple(),
        )

    @classmethod
    def from_offline_board_entry(cls, board_entry: dict) -> "Board":
        """Construct an Board with data from the offline database snapshot."""
        return cls(
            board_type=board_entry.get("board_type", ""),
            board_name=board_entry.get("board_name", ""),
            product_code=board_entry.get("product_code", ""),
            target_type=board_entry.get("target_type", ""),
            slug=board_entry.get("slug", ""),
            mbed_os_support=tuple(board_entry.get("mbed_os_support", [])),
            mbed_enabled=tuple(board_entry.get("mbed_enabled", [])),
            build_variant=tuple(board_entry.get("build_variant", [])),
        )
