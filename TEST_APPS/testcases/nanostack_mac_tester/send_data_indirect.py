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
        Bench.__init__(self, name = "send_data_indirect",
                        title = "Indirect data transmission test",
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
                        purpose = "Tests sending data indirectly, i.e polling the coordinator for data",
                        feature = ["MCPS-DATA", "MLME-POLL"],
                        component = ["MAC"],
                        requirements = {
                            "duts": {
                                '*': {
                                    "count":3,
                                    "type": "hardware",
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "KW41Z"],
                                    "application": {
                                       "name": "TEST_APPS-device-nanostack_mac_tester"
                                    }
                                },
                                "1":{"nick": "First"},
                                "2":{"nick": "Second"},
                                "3":{"nick": "Third"}
                        }}
        )

    def setUp(self):
        self.channel = 11

    def do_test_iteration(self):
        self.channel = 11
        self.command("First", "mlme-reset")
        self.command("Second", "mlme-reset")
        self.command("Third", "mlme-reset")

        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")
        self.command("Third", "addr --64-bit 01:02:03:00:00:00:00:03")

        # Start PAN coordinator
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        # Start PAN beacon
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))
        self.command("Third", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        # Set MAC RX on-while-idle off
        self.command("Second", "mlme-set --attr 0x52 --value_uint8 0 --value_size 1")
        self.command("Third", "mlme-set --attr 0x52 --value_uint8 0 --value_size 1")

        # Add neighbours
        self.command("First", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:02 --pan_id 0x1234 --index 0")
        self.command("First", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:03 --pan_id 0x1234 --index 1")
        self.command("Second", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:01 --pan_id 0x1234 --index 0")
        self.command("Third", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:01 --pan_id 0x1234 --index 0")

        # Configure indirect data
        self.command("Second", "config-status --data_ind abcde")
        self.command("Third", "config-status --data_ind 12345")

        # Runs into timing issues if extensive printing is enabled
        self.command("*", "silent-mode on")

        # Send data indirectly to DUT2
        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length 5 --msdu abcde --indirect_tx true --wait_for_confirm false")
        # Poll DUT1 for data
        self.command("Second", "poll --coord_address 01:02:03:00:00:00:00:01")

        # Send more data
        self.command("First", "data")
        self.command("First", "data")
        # Poll more data
        self.command("Second", "poll")
        self.command("Second", "poll")
        # Set expected poll return status to 0xEB(no data after poll)
        self.command("Second", "config-status --poll 235")
        # No data should remain to be polled for
        self.command("Second", "poll")

        # Set expected poll return status to 0xEB(no data after poll)
        self.command("Second", "config-status --poll 235")
        # Send data to DUT3
        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:03 --msdu 12345")
        # Poll(expected failure)
        self.command("Second", "poll")
        # Poll DUT1 for data(expected success)
        self.command("Third", "poll --coord_address 01:02:03:00:00:00:00:01")
        self.command("*", "silent-mode off")

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
        self.command("*", "silent-mode off")
        self.reset_dut()

