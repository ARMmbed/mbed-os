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
        Bench.__init__(self, name = "send_large_payloads",
                        title = "Data transmission test with large packets",
                        status = "development",
                        type = "reliability",
                        subtype = "",
                        execution = {
                            "skip": {
                                "value": False,
                                "reason": ""
                            }
                        },
                        author = "Valtteri Erkkila",
                        purpose = "Repeatedly sends long packets, checking that the payload is correct in each one",
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
        #104 characters, headers are 2+1+2+8+8+2=23 bytes, resulting in a packet size of 127 (max)
        large_payload = "0123456789abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZZZZZZZZZ0123456789012345678901234567891234"
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        self.command("First", "config-status --data_ind {}".format(large_payload))
        self.command("Second", "config-status --data_ind {}".format(large_payload))

        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length {} --msdu {}".format(len(large_payload), large_payload))
        self.command("Second", "wait --timeout 500")

        self.command("Second", "data --dst_addr 01:02:03:00:00:00:00:01 --msdu_length {} --msdu {}".format(len(large_payload), large_payload))
        self.command("First", "wait --timeout 500")
        for i in range(0, 25):
            self.command("First", "data")
            self.command("Second", "wait")
            self.command("Second", "data")
            self.command("First", "wait")

    def tearDown(self):
        self.reset_dut()
