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
from icetea_lib.TestStepError import TestStepFail

class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self, name = "address_read_and_write",
                        title = "MAC address and PAN id read/write test",
                        status = "released",
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
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "KW41Z"],
                                    "application": {
                                       "name": "TEST_APPS-device-nanostack_mac_tester"
                                    }
                                },
                                "1":{"nick": "First"}
                        }}
        )

    def setUp(self):
        pass

    def do_test_iteration(self):
        self.command("First", "mlme-reset")
        self.command("First", "addr")
        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("First", "addr --16-bit 0xABCD")
        # Set MAC PAN ID
        self.command("First", "mlme-set --attr 0x50 --value_bytes CD:CD --value_size 2")
        self.command("First", "addr")
        self.verify_trace(1, "MAC64: 01:02:03:00:00:00:00:01")

    def case(self):
        # Try tests few times because of potential RF failures
        loop = 0
        while loop < 5:
            try:
                self.do_test_iteration()
                break
            except TestStepFail:
                self.logger.info("Warning, iteration failed #"  + str(loop+1))
                loop = loop + 1
                self.delay(5)
        else:
             raise TestStepFail("Too many failed iterations!")

    def tearDown(self):
        self.reset_dut()

