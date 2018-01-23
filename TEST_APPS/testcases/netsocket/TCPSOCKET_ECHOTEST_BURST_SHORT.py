#
# Copyright (c) 2018, Arm Limited and affiliates.
#
from icetea_lib.bench import Bench, TestStepFail
from icetea_lib.tools import test_case
import random
import string


class Testcase(Bench):
    def __init__(self):
        Bench.__init__(self,
                       name="TCPSOCKET_ECHOTEST_BURST_SHORT",
                       title="TCPSOCKET_ECHOTEST_BURST_SHORT",
                       purpose="Verify that TCPSocket can send burst of packets to echo server and read incoming packets",
                       status="released",
                       component=["mbed-os", "netsocket"],
                       author="Juha Ylinen <juha.ylinen@arm.com>",
                       type="smoke",
                       subtype="socket",
                       requirements={
                           "duts": {
                               '*': {  # requirements for all nodes
                                    "count": 1,
                                    "type": "hardware",
                                    "application": {"name": "TEST_APPS-device-socket_app"}
                               },
                               "1": {"nick": "dut1"},
                           }
                       }
                       )

    def setup(self):
        self.command("dut1", "ifup")

    def randomString(self, length):
        return ''.join(random.choice(string.ascii_uppercase) for i in range(length))

    def case(self):
        response = self.command("dut1", "socket new TCPSocket")
        socket_id = int(response.parsed['socket_id'])

        self.command("dut1", "socket " + str(socket_id) + " open")
        self.command("dut1", "socket " + str(socket_id) + " connect echo.mbedcloudtesting.com 7")

        for i in range(2):
            sentData = ""
            for size in (100, 200, 300, 120, 500):
                packet = self.randomString(size)
                sentData += packet
                response = self.command("dut1", "socket " + str(socket_id) + " send " + str(packet))
                response.verify_trace("TCPSocket::send() returned: " + str(size))

            received = 0
            data = ""
            totalSize = 1220
            while received < totalSize:
                response = self.command("dut1", "socket " + str(socket_id) + " recv " + str(totalSize))
                data += response.parsed['data'].replace(":", "")
                received += int(response.parsed['received_bytes'])

            if data != sentData:
                raise TestStepFail("Received data doesn't match the sent data")

        self.command("dut1", "socket " + str(socket_id) + " delete")

    def teardown(self):
        self.command("dut1", "ifdown")
