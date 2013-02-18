import socket

ECHO_SERVER_ADDRESS = "10.2.131.195"
ECHO_PORT = 7

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((ECHO_SERVER_ADDRESS, ECHO_PORT))

s.sendall('Hello, world')
data = s.recv(1024)
s.close()
print 'Received', repr(data)
