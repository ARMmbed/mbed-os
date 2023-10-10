#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Resolve targets for `CandidateDevice`.

Resolving a target involves looking up an `MbedTarget` from the `mbed-targets` API, using data found in the "htm file"
located on an "Mbed Enabled" device's USB MSD.

For more information on the mbed-targets package visit https://github.com/ARMmbed/mbed-targets
"""
import logging

from typing import Optional

from mbed_tools.targets import (
    Board,
    get_board_by_product_code,
    get_board_by_online_id,
    get_board_by_jlink_slug,
)
from mbed_tools.targets.exceptions import UnknownBoard, MbedTargetsError

from mbed_tools.devices._internal.exceptions import NoBoardForCandidate, ResolveBoardError
from mbed_tools.devices._internal.file_parser import OnlineId


logger = logging.getLogger(__name__)


def resolve_board(
    product_code: Optional[str] = None, online_id: Optional[OnlineId] = None, serial_number: str = ""
) -> Board:
    """Resolves a board object from the platform database.

    We have multiple ways to identify boards from various metadata sources Mbed provides. This is because there are
    many supported Mbed device families, each with slightly different ways of identifying themselves as Mbed enabled.
    Because of this we need to try each input in turn, falling back to the next lookup method in the priority order if
    the previous one was unsuccessful.

    The rules are as follows:

    1. Use the product code from the mbed.htm file or details.txt if available
    2. Use online ID from the htm file or Board.html if available
    3. Try to use the first 4 chars of the USB serial number as the product code
    """
    if product_code:
        try:
            return get_board_by_product_code(product_code)
        except UnknownBoard:
            logger.error(f"Could not identify a board with the product code: '{product_code}'.")
        except MbedTargetsError as e:
            logger.error(
                f"There was an error looking up the product code `{product_code}` from the target database.\nError: {e}"
            )
            raise ResolveBoardError() from e

    if online_id:
        slug = online_id.slug
        target_type = online_id.target_type
        try:
            if target_type == "jlink":
                return get_board_by_jlink_slug(slug=slug)
            else:
                return get_board_by_online_id(slug=slug, target_type=target_type)
        except UnknownBoard:
            logger.error(f"Could not identify a board with the slug: '{slug}' and target type: '{target_type}'.")
        except MbedTargetsError as e:
            logger.error(
                f"There was an error looking up the online ID `{online_id!r}` from the target database.\nError: {e}"
            )
            raise ResolveBoardError() from e

    # Product code might be the first 4 characters of the serial number
    product_code = serial_number[:4]
    if product_code:
        try:
            return get_board_by_product_code(product_code)
        except UnknownBoard:
            # Most devices have a serial number so this may not be a problem
            logger.info(
                f"The device with the Serial Number: '{serial_number}' (Product Code: '{product_code}') "
                f"does not appear to be an Mbed development board."
            )
        except MbedTargetsError as e:
            logger.error(
                f"There was an error looking up the product code `{product_code}` from the target database.\nError: {e}"
            )
            raise ResolveBoardError() from e

    raise NoBoardForCandidate
