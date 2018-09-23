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
        Bench.__init__(self, name = "send_data",
                        title = "Simple data transmission test",
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
                        purpose = "Tests that sending data works",
                        feature = ["MCPS-DATA"],
                        component = ["MAC"],
                        requirements = {
                            "duts": {
                                '*': {
                                    "count":2,
                                    "type": "hardware",
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "UBLOX_EVK_ODIN_W2"],
                                    "application": {
                                       "name": "TEST_APPS-device-nanostack_mac_tester"
                                    }
                                },
                                "1":{"nick": "First"},
                                "2":{"nick": "Second"}
                        }}
        )

    def setUp(self):
        self.channel = 11
        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")

    def case(self):
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length 5 --msdu abcde")
        self.command("Second", "data --dst_addr 01:02:03:00:00:00:00:01 --msdu_length 5 --msdu 12345")

    def tearDown(self):
        self.reset_dut()
