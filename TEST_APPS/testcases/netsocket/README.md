##Netsocket tests

This folder contains netsocket tests for Icetea
The tests located in this folder are dependent of the application [`socket_app`](https://github.com/ARMmbed/mbed-os/blob/master/TEST_APPS/device/socket_app)

The test cases in this folder are similar to [Network Socket test plan](https://github.com/ARMmbed/mbed-os/blob/master/TESTS/netsocket/README.md).

Icetea test cases are processed by passing commands through the `mbed-client-cli` command line. It is possible to manually replicate most test cases by following the instructions below.

In test cases with more than one device under test (DUT) the target device is given in the instructions as DUT1, DUT2 or DUT3.

## Test cases

### `SOCKET_BIND_PORT`

**Description:**

Open and bind port.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `socket new TCPSocket` on device
2.  Call `socket TCPSocket open`
3.  Call `socket TCPSocket bind port <any non-used port number>`
4.  Destroy socket `socket TCPSocket delete`
5.  Create a object by calling `socket new UDPSocket` on device
6.  Call `socket UDPSocket open`
7.  Call `socket UDPSocket bind port <any non-used port number>`
8.  Destroy socket `socket UDPSocket delete`

**Expected result:**

The test exits with status `PASS` without timeouts.

### `TCPSERVER_ACCEPT`

**Description:**

Test that `TCPServer::bind()`, `TCPServer::listen()`
and `TCPServer::accept()` works.

Requires 2 devices.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  DUT1: `socket new TCPServer`
    Command returns server base socket ID
2.  DUT1: `socket <base socket ID> open)`
3.  DUT1: `socket <base socket ID> bind port <port>)`
4.  DUT1: `socket <base socket ID> listen)`
5.  DUT1: Create a new TCPSocket `socket new TCPSocket`
    Command returns server socket ID
6.  DUT1: `socket <server socket ID> open`
7.  DUT2: Create a new TCPSocket `socket new TCPSocket`
    Command returns client socket ID
8.  DUT2: `socket <client socket ID> open`
9.  DUT2: `socket <client socket ID> connect <dut1 IP> <port>`
10. DUT1: `socket <base socket id> accept <server socket id>`
    Command should return new socket ID
11. DUT1: `socket <new socket ID> send hello`
12. DUT2: `socket <client socket ID> recv 5`
13. DUT2: Verify that it received "hello"
14. Destroy all sockets.

**Expected result:**

On DUT1 accept() call blocks until connection is received. Duration
of call verified to be within threshold.

The test exits with status `PASS` without timeouts.

### `TCPSOCKET_ECHOTEST_BURST_SHORT`

**Description:**

Send burst of packets to echo server and read incoming packets back.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `socket <socket ID> open`
2.  Call `socket <socket ID> connect echo.mbedcloudtesting.com 7`
    1.  Create 5 randomized strings
        (100, 200, 300, 120 and 500 characters)
    2.  Send each string `socket <socket ID> send <string>`
    3.  Receive total size of sent data
        `socket <socket ID> recv <total size>`
    4.  Verify that received data is identical to sent data
3.  Repeat 2 times
4.  Destroy the socket

**Expected result:**

All send() calls should return the packet size.

The received data matches the sent data.

The test exits with status `PASS` without timeouts.
