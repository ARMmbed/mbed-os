import socket

BROADCAST_PORT = 58083

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('0.0.0.0', BROADCAST_PORT))

while True:
    print s.recvfrom(256)
