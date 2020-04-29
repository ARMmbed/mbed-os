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
from icetea_lib.tools import test_case


class MultipleTestcase(Bench):
    def __init__(self, **kwargs):
        testcase_args = {
            'status': "released",
            'component': ["mbed-os", "netsocket"],
            'type': "smoke",
            'subtype': "socket",
            'requirements': {
                "duts": {
                    "*": {
                        "count": 1,
                        "type": "hardware",
                        "application": {"name": "TEST_APPS-device-socket_app"}
                    },
                    "1": {"nick": "dut1"},
                }
            }
        }

        testcase_args.update(kwargs)
        Bench.__init__(self, **testcase_args)

    def setup(self):
        self.command("dut1", "ifup")

    def socket_bind_port(self, socket_type):
        response = self.command("dut1", "socket new " + socket_type)
        self.socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(self.socket_id) + " open")

        self.command("dut1", "socket " + str(self.socket_id) + " bind port 1024")

    def teardown(self):
        self.command("dut1", "socket " + str(self.socket_id) + " delete")
        
        self.command("dut1", "ifdown")


@test_case(MultipleTestcase,
           name="TCPSOCKET_BIND_PORT",
           title="tcpsocket open and bind port",
           purpose="Verify TCPSocket can be created, opened and port binded")
def test1(self):
    self.socket_bind_port("TCPSocket")


@test_case(MultipleTestcase,
           name="UDPSOCKET_BIND_PORT",
           title="udpsocket open and bind port",
           purpose="Verify UDPSocket can be created, opened and port binded")
def test2(self):
    self.socket_bind_port("UDPSocket")
