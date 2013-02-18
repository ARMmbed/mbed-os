import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 7))
s.listen(1)

while True:
    conn, addr = s.accept()
    print 'Connected by', addr
    while True:
        data = conn.recv(1024)
        if not data: break
        conn.sendall(data)
    conn.close()
