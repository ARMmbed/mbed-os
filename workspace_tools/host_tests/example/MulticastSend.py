import socket
from time import sleep, time

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)

while True:
    print "Multicast to group: %s\n" % MCAST_GRP
    data = 'Hello World: ' + repr(time()) + '\n'
    sock.sendto(data, (MCAST_GRP, MCAST_PORT))
    sleep(1)
