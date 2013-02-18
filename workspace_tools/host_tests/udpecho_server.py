from SocketServer import BaseRequestHandler, UDPServer
from private_settings import SERVER_ADDRESS

class UDP_EchoHandler(BaseRequestHandler):
    def handle(self):
        data, socket = self.request
        print "client:", self.client_address
        print "data:", data
        socket.sendto(data, self.client_address)

server = UDPServer((SERVER_ADDRESS, 7195), UDP_EchoHandler)
print "listening for connections"
server.serve_forever()
