"""
Copyright (c) 2017, Arm Limited and affiliates.
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os,sys
from icetea_lib.bench import Bench

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self, name = "address_read_and_write",
                        title = "MAC address and PAN id read/write test",
                        status = "development",
                        type = "smoke",
                        subtype = "",
                        execution = {
                            "skip": {
                                "value": False,
                                "reason": ""
                            }
                        },
                        author = "Valtteri Erkkila",
                        purpose = "Tests reading a MAC address from the driver, and writing to the modifiable MAC address",
                        feature = ["MLME-SET"],
                        component = ["MAC"],
                        requirements = {
                            "duts": {
                                '*': {
                                    "count":1,
                                    "type": "hardware",
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "UBLOX_EVK_ODIN_W2"],
                                    "application": {
                                       "name": "TEST_APPS-device-nanostack_mac_tester"
                                    }
                                },
                                "1":{"nick": "First"}
                        }}
        )

    def setUp(self):
        pass

    def case(self):
        self.command("First", "addr")
        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("First", "addr --16-bit 0xABCD")
        #macPANId
        self.command("First", "mlme-set --attr 0x50 --value_bytes CD:CD --value_size 2")
        self.command("First", "addr")
        self.verify_trace(1, "MAC64: 01:02:03:00:00:00:00:01")

    def tearDown(self):
        self.reset_dut()
