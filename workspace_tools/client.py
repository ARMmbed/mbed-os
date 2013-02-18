import json
from socket import *
from workspace_tools.settings import *


def get_server_connection():
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect((SERVER_ADDRESS, SERVER_PORT))
    return sock


def get_muts():
    # Get the data structure about the Mbed Under Test provided by this server
    sock = get_server_connection()
    sock.send(json.dumps({"info":"muts"}))
    muts = sock.recv(1024)
    return json.loads(muts)


def request_test(mcu, image_path, test_id, duration=10):
    """
    The test_id needs to be provided only for an automated test the server will
    load the correspondent Checker class and will return a json report at the
    end of the test
    """
    test_spec = {
        "mcu": mcu,
        "image": image_path,
        "duration": duration,
        "test_id": test_id,
    }
    
    sock = get_server_connection()
    sock.send(json.dumps(test_spec))
    
    report_buffer = []
    try:
        while True:
            # Here we do not worry about efficiency the most important feature
            # for the developer in front of the terminal is responsiveness
            c = sock.recv(1)
            if not c: break
            report_buffer.append(c)

    except KeyboardInterrupt, _:
        print "\n[CTRL+c] closing connection"
        sock.send("!")
    
    print '\nTest completed'
    sock.close()
    
    try:
        return json.loads(''.join(report_buffer))
    except Exception, _:
        return {"result": "error"}
