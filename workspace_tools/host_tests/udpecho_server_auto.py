"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

from socket import socket, AF_INET, SOCK_DGRAM
import re
from host_test import DefaultTest
from time import time
from sys import stdout

class UDPEchoServerTest(DefaultTest):
    ECHO_SERVER_ADDRESS = ""
    ECHO_PORT = 0
    s = None # Socket

    PATTERN_SERVER_IP = "^Server IP Address is (\d+).(\d+).(\d+).(\d+):(\d+)"
    re_detect_server_ip = re.compile(PATTERN_SERVER_IP)

    def run(self):
        ip_msg_timeout = self.mbed.options.timeout
        serial_ip_msg = ""
        start_serial_pool = time();
        while (time() - start_serial_pool) < ip_msg_timeout:
            c = self.mbed.serial_read(512)
            if c is None:
                self.print_result("ioerr_serial")
                return
            stdout.write(c)
            stdout.flush()
            serial_ip_msg += c
            # Searching for IP address and port prompted by server
            m = self.re_detect_server_ip.search(serial_ip_msg)
            if m and len(m.groups()):
                self.ECHO_SERVER_ADDRESS = ".".join(m.groups()[:4])
                self.ECHO_PORT = int(m.groups()[4]) # must be integer for socket.connect method
                duration = time() - start_serial_pool
                print "UDP Server found at: " + self.ECHO_SERVER_ADDRESS + ":" + str(self.ECHO_PORT) + " after " + "%.2f" % duration  + " sec"
                stdout.flush()
                break
        else:
            print "Error: No IP and port information sent from server"
            self.print_result('error')
            exit(-2)

        # We assume this test fails so can't send 'error' message to server
        try:
            self.s = socket(AF_INET, SOCK_DGRAM)
        except Exception, e:
            print "Error: %s" % e
            self.print_result('error')
            exit(-1)

        TEST_STRING = 'Hello, world !!!'
        self.s.sendto(TEST_STRING, (self.ECHO_SERVER_ADDRESS, self.ECHO_PORT))

        data = self.s.recv(len(TEST_STRING))
        received_str = repr(data)[1:-1]

        if TEST_STRING == received_str: # We need to cut not needed single quotes from the string
            print 'Received data: ' + received_str
            self.print_result('success')
        else:
            self.print_result('failure')
        self.s.close()

        # Receiving
        try:
            while True:
                c = self.mbed.serial_read(512)
                if c is None:
                    self.print_result("ioerr_serial")
                    break
                stdout.write(c)
                stdout.flush()
        except KeyboardInterrupt, _:
            print "\n[CTRL+c] exit"

if __name__ == '__main__':
    UDPEchoServerTest().run()
