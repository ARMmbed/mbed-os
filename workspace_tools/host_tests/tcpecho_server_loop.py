# Be sure that the tools directory is in the search path
import sys
from os.path import join, abspath, dirname
ROOT = abspath(join(dirname(__file__), "..", ".."))
sys.path.append(ROOT)

from workspace_tools.private_settings import LOCALHOST
from SocketServer import BaseRequestHandler, TCPServer


class TCP_EchoHandler(BaseRequestHandler):
    def handle(self):
        print "\nHandle connection from:", self.client_address
        while True:
            data = self.request.recv(1024)
            if not data: break
            self.request.sendall(data)
        self.request.close()
        print "socket closed"

if __name__ == '__main__':
    server = TCPServer((LOCALHOST, 7), TCP_EchoHandler)
    print "listening for connections on:", (LOCALHOST, 7)
    server.serve_forever()
