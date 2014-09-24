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

import sys
import socket
from sys import stdout
from time import sleep
from host_test import Test
from SocketServer import BaseRequestHandler, TCPServer


SERVER_IP = str(socket.gethostbyname(socket.getfqdn()))
SERVER_PORT = 7


class TCPEchoClientTest(Test):
    def __init__(self):
        Test.__init__(self)
        self.mbed.init_serial()

    def send_server_ip_port(self, ip_address, port_no):
        """ Set up network host. Reset target and and send server IP via serial to Mbed
        """
        print "HOST: Resetting target..."
        self.mbed.reset()

        c = self.mbed.serial_readline() # 'TCPCllient waiting for server IP and port...'
        if c is None:
            self.print_result("ioerr_serial")
            return

        print c.strip()
        print "HOST: Sending server IP Address to target...",
        stdout.flush()
        connection_str = ip_address + ":" + str(port_no) + "\n"
        self.mbed.serial_write(connection_str)
        print connection_str
        stdout.flush()

        # Two more strings about connection should be sent by MBED
        for i in range(0, 2):
            c = self.mbed.serial_readline()
            if c is None:
                self.print_result("ioerr_serial")
                return
            print c.strip()
            stdout.flush()


class TCPEchoClient_Handler(BaseRequestHandler):
    def handle(self):
        """ One handle per connection """
        print "HOST: Connection received...",
        count = 1;
        while True:
            data = self.request.recv(1024)
            if not data: break
            self.request.sendall(data)
            if '{{end}}' in str(data):
                print
                print str(data)
            else:
                if not count % 10:
                    sys.stdout.write('.')
                count += 1
            stdout.flush()


server = TCPServer((SERVER_IP, SERVER_PORT), TCPEchoClient_Handler)
print "HOST: Listening for connections: " + SERVER_IP + ":" + str(SERVER_PORT)

mbed_test = TCPEchoClientTest();
mbed_test.send_server_ip_port(SERVER_IP, SERVER_PORT)

server.serve_forever()
