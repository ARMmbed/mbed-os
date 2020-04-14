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
        Bench.__init__(self, name = "send_large_payloads",
                        title = "Data transmission test with large packets",
                        status = "released",
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
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "KW41Z"],
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

    def do_test_iteration(self):
        self.command("First", "mlme-reset")
        self.command("Second", "mlme-reset")

        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")

        #104 characters, headers are 2+1+2+8+8+2=23 bytes, resulting in a packet size of 127 (max)
        large_payload = "0123456789abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZZZZZZZZZ0123456789012345678901234567891234"
        # Start PAN coordinator
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        # Start PAN beacon
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        # Set indirect data
        self.command("First", "config-status --data_ind {}".format(large_payload))
        self.command("Second", "config-status --data_ind {}".format(large_payload))

        # Send data to DUT2
        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length {} --msdu {}".format(len(large_payload), large_payload))
        # Wait for transmission to finish
        self.command("Second", "wait --timeout 500")

        # Send data to DUT1
        self.command("Second", "data --dst_addr 01:02:03:00:00:00:00:01 --msdu_length {} --msdu {}".format(len(large_payload), large_payload))
        # Wait for transmission to finish
        self.command("First", "wait --timeout 500")
        # Loop with previous settings
        for i in range(0, 25):
            self.command("First", "data")
            self.command("Second", "wait")
            self.command("Second", "data")
            self.command("First", "wait")

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

