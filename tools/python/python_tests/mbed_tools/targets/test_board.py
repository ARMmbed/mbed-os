#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for `mbed_tools.targets.mbed_target`."""

from unittest import TestCase

# Import from top level as this is the expected interface for users
from mbed_tools.targets import Board


class TestBoard(TestCase):
    """Tests for the class `Board`."""

    def test_offline_database_entry(self):
        """Given an entry from the offline database, a Board is generated with the correct information."""
        board = Board.from_offline_board_entry(
            {
                "mbed_os_support": ["Mbed OS 5.15"],
                "mbed_enabled": ["Basic"],
                "board_type": "B_1",
                "board_name": "Board 1",
                "product_code": "P1",
                "target_type": "platform",
                "slug": "Le Slug",
            }
        )

        self.assertEqual("B_1", board.board_type)
        self.assertEqual("Board 1", board.board_name)
        self.assertEqual(("Mbed OS 5.15",), board.mbed_os_support)
        self.assertEqual(("Basic",), board.mbed_enabled)
        self.assertEqual("P1", board.product_code)
        self.assertEqual("platform", board.target_type)
        self.assertEqual("Le Slug", board.slug)
        self.assertEqual((), board.build_variant)

    def test_empty_database_entry(self):
        """Given no data, a Board is created with no information."""
        board = Board.from_online_board_entry({})

        self.assertEqual("", board.board_type)
        self.assertEqual("", board.board_name)
        self.assertEqual((), board.mbed_os_support)
        self.assertEqual((), board.mbed_enabled)
        self.assertEqual("", board.product_code)
        self.assertEqual("", board.target_type)
        self.assertEqual("", board.slug)

    def test_online_database_entry(self):
        online_data = {
            "type": "target",
            "id": "1",
            "attributes": {
                "features": {
                    "mbed_enabled": ["Advanced"],
                    "mbed_os_support": [
                        "Mbed OS 5.10",
                        "Mbed OS 5.11",
                        "Mbed OS 5.12",
                        "Mbed OS 5.13",
                        "Mbed OS 5.14",
                        "Mbed OS 5.15",
                        "Mbed OS 5.8",
                        "Mbed OS 5.9",
                    ],
                    "antenna": ["Connector", "Onboard"],
                    "certification": [
                        "Anatel (Brazil)",
                        "AS/NZS (Australia and New Zealand)",
                        "CE (Europe)",
                        "FCC/CFR (USA)",
                        "IC RSS (Canada)",
                        "ICASA (South Africa)",
                        "KCC (South Korea)",
                        "MIC (Japan)",
                        "NCC (Taiwan)",
                        "RoHS (Europe)",
                    ],
                    "communication": ["Bluetooth & BLE"],
                    "interface_firmware": ["DAPLink", "J-Link"],
                    "target_core": ["Cortex-M4"],
                    "mbed_studio_support": ["Build and run"],
                },
                "board_type": "k64f",
                "flash_size": 512,
                "name": "u-blox NINA-B1",
                "product_code": "0455",
                "ram_size": 64,
                "target_type": "module",
                "hidden": False,
                "device_name": "nRF52832_xxAA",
                "slug": "u-blox-nina-b1",
            },
        }
        board = Board.from_online_board_entry(online_data)

        self.assertEqual(online_data["attributes"]["board_type"].upper(), board.board_type)
        self.assertEqual(online_data["attributes"]["name"], board.board_name)
        self.assertEqual(tuple(online_data["attributes"]["features"]["mbed_os_support"]), board.mbed_os_support)
        self.assertEqual(tuple(online_data["attributes"]["features"]["mbed_enabled"]), board.mbed_enabled)
        self.assertEqual(online_data["attributes"]["product_code"], board.product_code)
        self.assertEqual(online_data["attributes"]["target_type"], board.target_type)
        self.assertEqual(online_data["attributes"]["slug"], board.slug)
        self.assertEqual(tuple(), board.build_variant)
