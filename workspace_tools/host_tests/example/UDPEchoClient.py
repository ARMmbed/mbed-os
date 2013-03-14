import socket
 
ECHO_SERVER_ADDRESS = '10.2.200.41'
ECHO_PORT = 7

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto("Hello World\n", (ECHO_SERVER_ADDRESS, ECHO_PORT))
response = sock.recv(256)
sock.close()

print response
