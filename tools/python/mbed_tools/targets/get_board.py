#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Interface for accessing Mbed-Enabled Development Board data.

An instance of `mbed_tools.targets.board.Board` can be retrieved by calling one of the public functions.
"""
import logging
from enum import Enum
from typing import Callable

from mbed_tools.targets.env import env
from mbed_tools.targets.exceptions import UnknownBoard, UnsupportedMode, BoardDatabaseError
from mbed_tools.targets.board import Board
from mbed_tools.targets.boards import Boards


logger = logging.getLogger(__name__)


def get_board_by_product_code(product_code: str) -> Board:
    """Returns first `mbed_tools.targets.board.Board` matching given product code.

    Args:
        product_code: the product code to look up in the database.

    Raises:
        UnknownBoard: a board with a matching product code was not found.
    """
    return get_board(lambda board: board.product_code == product_code)


def get_board_by_online_id(slug: str, target_type: str) -> Board:
    """Returns first `mbed_tools.targets.board.Board` matching given online id.

    Args:
        slug: The slug to look up in the database.
        target_type: The target type to look up in the database, normally one of `platform` or `module`.

    Raises:
        UnknownBoard: a board with a matching slug and target type could not be found.
    """
    matched_slug = slug.casefold()
    return get_board(lambda board: board.slug.casefold() == matched_slug and board.target_type == target_type)


def get_board_by_jlink_slug(slug: str) -> Board:
    """Returns first `mbed-tools.targets.board.Board` matching given slug.

    With J-Link, the slug is extracted from a board manufacturer URL, and may not match
    the Mbed slug. The J-Link slug is compared against the slug, board_name and
    board_type of entries in the board database until a match is found.

    Args:
        slug: the J-Link slug to look up in the database.

    Raises:
        UnknownBoard: a board matching the slug was not found.
    """
    matched_slug = slug.casefold()
    return get_board(
        lambda board: any(matched_slug == c.casefold() for c in [board.slug, board.board_name, board.board_type])
    )


def get_board(matching: Callable) -> Board:
    """Returns first `mbed_tools.targets.board.Board` for which `matching` is True.

    Uses database mode configured in the environment.

    Args:
        matching: A function which will be called to test matching conditions for each board in database.

    Raises:
        UnknownBoard: a board matching the criteria could not be found in the board database.
    """
    database_mode = _get_database_mode()

    if database_mode == _DatabaseMode.OFFLINE:
        logger.info("Using the offline database (only) to identify boards.")
        return Boards.from_offline_database().get_board(matching)

    if database_mode == _DatabaseMode.ONLINE:
        logger.info("Using the online database (only) to identify boards.")
        return Boards.from_online_database().get_board(matching)
    try:
        logger.info("Using the offline database to identify boards.")
        return Boards.from_offline_database().get_board(matching)
    except UnknownBoard:
        logger.info("Unable to identify a board using the offline database, trying the online database.")
        try:
            return Boards.from_online_database().get_board(matching)
        except BoardDatabaseError:
            logger.error("Unable to access the online database to identify a board.")
            raise UnknownBoard()


class _DatabaseMode(Enum):
    """Selected database mode."""

    OFFLINE = 0
    ONLINE = 1
    AUTO = 2


def _get_database_mode() -> _DatabaseMode:
    database_mode = env.MBED_DATABASE_MODE
    try:
        return _DatabaseMode[database_mode]
    except KeyError:
        raise UnsupportedMode(f"{database_mode} is not a supported database mode.")
