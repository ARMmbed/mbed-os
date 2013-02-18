import socket
 
ECHO_PORT = 7

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', ECHO_PORT))

while True:
    data, address = sock.recvfrom(256)
    print "datagram from", address
    sock.sendto(data, address)
