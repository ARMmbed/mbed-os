"""
Copyright 2018 ARM Limited
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

from icetea_lib.bench import Bench


class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="test_predefined_platforms",
                       title="Test a test case which have support for multiple platforms",
                       status="released",
                       purpose="Just for testing scripts",
                       component=[],
                       type="regression",
                       requirements={
                           "duts": {
                               '*': {
                                   "count": 1,
                                   "type": "hardware",
                                   "allowed_platforms": [
                                       "LPC1768", "KL25Z", "K64F", "K66F", "K22F", "LPC4088", "LPC1549",
                                       "NUCLEO_F072RB", "NUCLEO_F091RC", "NUCLEO_F302R8", "NUCLEO_F303K8",
                                       "NUCLEO_F303RE", "NUCLEO_F207ZG", "NUCLEO_F334R8", "NUCLEO_F303ZE",
                                       "NUCLEO_L053R8", "DISCO_L072CZ_LRWAN1", "NUCLEO_L073RZ", "NUCLEO_L152RE",
                                       "NUCLEO_F410RB", "NUCLEO_F446RE", "NUCLEO_F446ZE", "NUCLEO_F429ZI",
                                       "DISCO_F407VG", "NUCLEO_F746ZG", "NUCLEO_L476RG", "DISCO_L053C8", "DISCO_F334C8",
                                       "DISCO_L475VG_IOT01A", "DISCO_L476VG", "DISCO_F469NI", "DISCO_F429ZI",
                                       "DISCO_F769NI", "ARCH_MAX", "MAX32600MBED", "MOTE_L152RC", "B96B_F446VE"
                                   ],
                                   "application": {
                                       "name": "TEST_APPS-device-exampleapp"
                                   }
                               }
                           }
                       }
                       )

    def setup(self):
        pass

    def case(self):
        pass

    def teardown(self):
        pass
