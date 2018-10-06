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
        Bench.__init__(self, name = "send_data_indirect",
                        title = "Indirect data transmission test",
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
                        purpose = "Tests sending data indirectly, i.e polling the coordinator for data",
                        feature = ["MCPS-DATA", "MLME-POLL"],
                        component = ["MAC"],
                        requirements = {
                            "duts": {
                                '*': {
                                    "count":3,
                                    "type": "hardware",
                                    "allowed_platforms": ["K64F", "K66F", "NUCLEO_F429ZI", "KW24D", "UBLOX_EVK_ODIN_W2"],
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
        self.command("First", "addr --64-bit 01:02:03:00:00:00:00:01")
        self.command("Second", "addr --64-bit 01:02:03:00:00:00:00:02")
        self.command("Third", "addr --64-bit 01:02:03:00:00:00:00:03")

    def case(self):
        self.command("First", "start --pan_coordinator true --logical_channel {}".format(self.channel))
        self.command("Second", "start --pan_coordinator false --logical_channel {}".format(self.channel))
        self.command("Third", "start --pan_coordinator false --logical_channel {}".format(self.channel))

        #macRxOnWhenIdle
        self.command("Second", "mlme-set --attr 0x52 --value_uint8 0 --value_size 1")
        self.command("Third", "mlme-set --attr 0x52 --value_uint8 0 --value_size 1")

        self.command("First", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:02 --pan_id 0x1234 --index 0")
        self.command("First", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:03 --pan_id 0x1234 --index 1")
        self.command("Second", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:01 --pan_id 0x1234 --index 0")
        self.command("Third", "add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:01 --pan_id 0x1234 --index 0")

        self.command("Second", "config-status --data_ind abcde")
        self.command("Third", "config-status --data_ind 12345")

        #Runs into timing issues if extensive printing is enabled
        self.command("*", "silent-mode on")
        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length 5 --msdu abcde --indirect_tx true --wait_for_confirm false")
        self.command("Second", "poll --coord_address 01:02:03:00:00:00:00:01")

        self.command("First", "data")
        self.command("First", "data")
        self.command("Second", "poll")
        self.command("Second", "poll")
        self.command("Second", "config-status --poll 235")
        self.command("Second", "poll")

        self.command("Second", "config-status --poll 235")
        self.command("First", "data --dst_addr 01:02:03:00:00:00:00:03 --msdu 12345")
        self.command("Second", "poll")
        self.command("Third", "poll --coord_address 01:02:03:00:00:00:00:01")
        self.command("*", "silent-mode off")

    def tearDown(self):
        self.reset_dut()
