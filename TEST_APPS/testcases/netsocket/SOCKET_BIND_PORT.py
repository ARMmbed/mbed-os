#
# Copyright (c) 2017-2018, Arm Limited and affiliates.
#
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
        socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(socket_id) + " open")

        self.command("dut1", "socket " + str(socket_id) + " bind port 1024")

        self.command("dut1", "socket " + str(socket_id) + " delete")

    def teardown(self):
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
