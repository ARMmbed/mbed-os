#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib
from unittest import TestCase
from mbed_tools.devices._internal.candidate_device import CandidateDevice


def build_candidate_data(**overrides):
    defaults = {
        "product_id": "0x1234",
        "vendor_id": "0x5678",
        "mount_points": (pathlib.Path("./foo"),),
        "serial_number": "qwer",
        "serial_port": "COM1",
    }
    return {**defaults, **overrides}


class TestCandidateDevice(TestCase):
    def test_produces_a_valid_candidate(self):
        candidate_data = build_candidate_data()
        candidate = CandidateDevice(**candidate_data)

        self.assertEqual(candidate.product_id, candidate_data["product_id"])
        self.assertEqual(candidate.vendor_id, candidate_data["vendor_id"])
        self.assertEqual(candidate.mount_points, candidate_data["mount_points"])
        self.assertEqual(candidate.serial_number, candidate_data["serial_number"])
        self.assertEqual(candidate.serial_port, candidate_data["serial_port"])

    def test_raises_when_product_id_is_empty(self):
        candidate_data = build_candidate_data(product_id="")
        with self.assertRaisesRegex(ValueError, "product_id"):
            CandidateDevice(**candidate_data)

    def test_raises_when_product_id_is_not_hex(self):
        candidate_data = build_candidate_data(product_id="TEST")
        with self.assertRaisesRegex(ValueError, "product_id"):
            CandidateDevice(**candidate_data)

    def test_prefixes_product_id_hex_value(self):
        candidate_data = build_candidate_data(product_id="ff01")
        candidate = CandidateDevice(**candidate_data)
        self.assertEqual(candidate.product_id, "0xff01")

    def test_raises_when_vendor_id_is_empty(self):
        candidate_data = build_candidate_data(vendor_id="")
        with self.assertRaisesRegex(ValueError, "vendor_id"):
            CandidateDevice(**candidate_data)

    def test_raises_when_vendor_id_is_not_hex(self):
        candidate_data = build_candidate_data(vendor_id="TEST")
        with self.assertRaisesRegex(ValueError, "vendor_id"):
            CandidateDevice(**candidate_data)

    def test_prefixes_vendor_id_hex_value(self):
        candidate_data = build_candidate_data(vendor_id="cbad")
        candidate = CandidateDevice(**candidate_data)
        self.assertEqual(candidate.vendor_id, "0xcbad")

    def test_raises_when_mount_points_are_empty(self):
        with self.assertRaisesRegex(ValueError, "mount_points"):
            CandidateDevice(product_id="1234", vendor_id="1234", mount_points=[], serial_number="1234")

    def test_raises_when_serial_number_is_empty(self):
        candidate_data = build_candidate_data(serial_number="")
        with self.assertRaisesRegex(ValueError, "serial_number"):
            CandidateDevice(**candidate_data)
