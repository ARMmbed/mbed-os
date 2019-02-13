Network Socket test plan
========================

This is a test plan for the Mbed OS Socket API. This describes all test cases and their intended behaviors. When an API document is not clear, use this as a reference for implementing correct behavior.

**NOTE:** Because testing is a moving target, this test plan might define more test cases than Mbed OS implements. Refer to [test case priorities](#test-case-priorities) for a list of test cases that the target must pass to be compliant with the Mbed OS socket API.


Target API
----------

The target for this plan is to test:

-   [Socket](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/Socket.h).
-   [UDPSocket](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/UDPSocket.h).
-   [TCPSocket](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/TCPSocket.h).
-   [TLSSocket](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/TLSSocket.h).
-   [DNS](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/DNS.h).

Please see the [Network Socket Documentation](https://os.mbed.com/docs/mbed-os/latest/apis/network-socket.html) for reference.

Tools to use
----------------

-   Mbed OS.
-   Standard Mbed OS development tools as described in the [Arm Mbed tools overview](https://os.mbed.com/docs/latest/tools/index.html).
-   Test server.

These test cases themselves do not require any special tooling, other than the test server described in "Test environment" chapter.

Test environment
----------------

As a general guideline, network connectivity with public Internet access is required. This satisfies Socket tests, but specific connectivity methods might require some extra configuration or devices within the network.

The test environment consist of DUTs, network connection and the test server. Arm provides a public test server, but it can be installed locally as well, if an isolated test environment is required.

### Public test server

Address: `echo.mbedcloudtesting.com'.

Both IPv4 and IPv6 addresses are available from a public DNS service:

```.sh
$ host echo.mbedcloudtesting.com
echo.mbedcloudtesting.com has address 52.215.34.155
echo.mbedcloudtesting.com has IPv6 address 2a05:d018:21f:3800:8584:60f8:bc9f:e614
```

**Open services in the test server**

-   Echo protocol, [RFC 862](https://tools.ietf.org/html/rfc862) is enabled on both TCP and UDP on port 7. Port 2007 for TLS
-   Discard protocol, [RFC 863](https://tools.ietf.org/html/rfc863) is enabled in both TCP and UDP on port 9. Port 2009 for TLS.
-   Character generator protocol, [RFC 864](https://tools.ietf.org/html/rfc864) is enabled in both TCP and UDP on port 19. Output pattern should follow the proposed example pattern in RFC.
-   Daytime protocol, [RFC 867](https://tools.ietf.org/html/rfc867) in both TCP and UDP on port 13. Port 2013 for TLS.
-   Time protocol, [RFC 868](https://tools.ietf.org/html/rfc868) in both TCP and UDP on port 37.

Configure the firewall to allow this traffic to access the test server.

**Example configuration for Debian/Ubuntu Linux**

These services are available on many operating systems, and installing them is out of scope of this document. Below is an example of how to install these services into a Debian/Ubuntu based Linux distribution using standard Inet Daemon:

```.sh
$ sudo apt install inetutils-inetd
$ nano /etc/inetd.conf
```

Enable following services from /etc/inetd.conf:

```
#:INTERNAL: Internal services
discard     stream  tcp6    nowait  root    internal
discard     dgram   udp6    wait    root    internal
echo        stream  tcp6    nowait  root    internal
echo        dgram   udp6    wait    root    internal
chargen     stream  tcp6    nowait  root    internal
chargen     dgram   udp6    wait    root    internal
daytime     stream  tcp6    nowait  root    internal
time        stream  tcp6    nowait  root    internal
```

**Testing the connectivity**

You can connect to the test server with an NMAP tool like this:

```.sh
$ nmap -sT -p7,9,13,37,2007,2009,2013 echo.mbedcloudtesting.com

Starting Nmap 7.01 ( https://nmap.org ) at 2019-01-07 16:45 CET
Nmap scan report for echo.mbedcloudtesting.com (52.215.34.155)
Host is up (0.027s latency).
Other addresses for echo.mbedcloudtesting.com (not scanned): 2a05:d018:21f:3800:3164:2a5c:75b3:970b
rDNS record for 52.215.34.155: ec2-52-215-34-155.eu-west-1.compute.amazonaws.com
PORT     STATE SERVICE
7/tcp    open  echo
9/tcp    open  discard
13/tcp   open  daytime
37/tcp   open  time
2007/tcp open  dectalk
2009/tcp open  news
2013/tcp open  raid-am

Nmap done: 1 IP address (1 host up) scanned in 0.10 seconds

$ sudo nmap -sU -p7,9,13,37,2007,2009,2013 echo.mbedcloudtesting.com 

Starting Nmap 7.01 ( https://nmap.org ) at 2019-01-08 15:05 CET
Nmap scan report for echo.mbedcloudtesting.com (52.215.34.155)
Host is up (0.044s latency).
Other addresses for echo.mbedcloudtesting.com (not scanned): 2a05:d018:21f:3800:3164:2a5c:75b3:970b
rDNS record for 52.215.34.155: ec2-52-215-34-155.eu-west-1.compute.amazonaws.com
PORT     STATE         SERVICE
7/udp    open          echo
9/udp    open|filtered discard
13/udp   closed        daytime
37/udp   closed        time
2007/udp closed        raid-am
2009/udp closed        whosockami
2013/udp closed        raid-cd

Nmap done: 1 IP address (1 host up) scanned in 1.78 seconds
```

### Ethernet test environment

![Ethernet](eth_environment.png)

The Ethernet test environment consists of devices, an ethernet switch and an optional firewall that allows connecting to the Echo server.

### Wi-Fi test environment

![Wi-Fi](wifi_environment.png)

The Wi-Fi test environment is equivalent to the Ethernet test environment, except that the Wi-Fi test environment has two separate access points or one with dual SSID. Connectivity to echo server is required, but it can be hosted locally, as specified in the Ethernet environment.

Test case priorities
--------------------

Please refer to the following table for priorities of test cases. Priorities are labeled as MUST and SHOULD. MUST means this is a requirement and therefore mandatory to pass the test. SHOULD means it is recommended to pass the test if the driver implements the feature in question.

|     | Test case                               | Priority |
|-----|-----------------------------------------|----------|
| 1   | UDPSOCKET_OPEN_DESTRUCT                 | MUST     |
| 2   | UDPSOCKET_OPEN_LIMIT                    | MUST     |
| 3   | UDPSOCKET_OPEN_TWICE                    | MUST     |
| 4   | UDPSOCKET_OPEN_CLOSE_REPEAT             | MUST     |
| 5   | UDPSOCKET_BIND_PORT                     | SHOULD   |
| 6   | UDPSOCKET_BIND_PORT_FAIL                | SHOULD   |
| 7   | UDPSOCKET_BIND_ADDRESS_PORT             | SHOULD   |
| 8   | UDPSOCKET_BIND_ADDRESS_NULL             | SHOULD   |
| 9   | UDPSOCKET_BIND_ADDRESS_INVALID          | SHOULD   |
| 10  | UDPSOCKET_BIND_WRONG_TYPE               | SHOULD   |
| 11  | UDPSOCKET_BIND_ADDRESS                  | SHOULD   |
| 12  | UDPSOCKET_BIND_UNOPENED                 | SHOULD   |
| 13  | UDPSOCKET_SENDTO_INVALID                | MUST     |
| 14  | UDPSOCKET_SENDTO_REPEAT                 | MUST     |
| 15  | UDPSOCKET_ECHOTEST                      | MUST     |
| 16  | UDPSOCKET_ECHOTEST_NONBLOCK             | MUST     |
| 17  | UDPSOCKET_RECV_TIMEOUT                  | SHOULD   |
| 18  | UDPSOCKET_SENDTO_TIMEOUT                | SHOULD   |
| 19  | TCPSOCKET_OPEN_DESTRUCT                 | MUST     |
| 20  | TCPSOCKET_OPEN_LIMIT                    | MUST     |
| 21  | TCPSOCKET_OPEN_TWICE                    | MUST     |
| 22  | TCPSOCKET_OPEN_CLOSE_REPEAT             | MUST     |
| 23  | TCPSOCKET_BIND_PORT                     | SHOULD   |
| 24  | TCPSOCKET_BIND_PORT_FAIL                | SHOULD   |
| 25  | TCPSOCKET_BIND_ADDRESS_PORT             | SHOULD   |
| 26  | TCPSOCKET_BIND_ADDRESS_NULL             | SHOULD   |
| 27  | TCPSOCKET_BIND_ADDRESS_INVALID          | SHOULD   |
| 28  | TCPSOCKET_BIND_WRONG_TYPE               | SHOULD   |
| 29  | TCPSOCKET_BIND_ADDRESS                  | SHOULD   |
| 30  | TCPSOCKET_BIND_UNOPENED                 | SHOULD   |
| 31  | TCPSOCKET_CONNECT_INVALID               | MUST     |
| 32  | TCPSOCKET_SEND_REPEAT                   | MUST     |
| 33  | TCPSOCKET_ECHOTEST                      | MUST     |
| 34  | TCPSOCKET_ECHOTEST_NONBLOCK             | MUST     |
| 35  | TCPSOCKET_RECV_TIMEOUT                  | SHOULD   |
| 36  | TCPSOCKET_SEND_TIMEOUT                  | SHOULD   |
| 37  | TCPSOCKET_ENDPOINT_CLOSE                | MUST     |
| 38  | UDPSOCKET_ECHOTEST_BURST                | MUST     |
| 39  | UDPSOCKET_ECHOTEST_BURST_NONBLOCK       | MUST     |
| 40  | TCPSOCKET_ECHOTEST_BURST                | MUST     |
| 41  | TCPSOCKET_ECHOTEST_BURST_NONBLOCK       | MUST     |
| 42  | TCPSOCKET_RECV_100K                     | MUST     |
| 43  | TCPSOCKET_RECV_100K_NONBLOCK            | MUST     |
| 44  | TCPSOCKET_THREAD_PER_SOCKET_SAFETY      | MUST     |
| 45  | TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID    | SHOULD   |
| 46  | TLSSOCKET_OPEN_DESTRUCT                 | SHOULD   |
| 47  | TLSSOCKET_OPEN_LIMIT                    | SHOULD   |
| 48  | TLSSOCKET_OPEN_TWICE                    | SHOULD   |
| 49  | TLSSOCKET_CONNECT_INVALID               | SHOULD   |
| 50  | TLSSOCKET_HANDSHAKE_INVALID             | SHOULD   |
| 51  | TLSSOCKET_SEND_CLOSED                   | SHOULD   |
| 52  | TLSSOCKET_SEND_UNCONNECTED              | SHOULD   |
| 53  | TLSSOCKET_SEND_REPEAT                   | SHOULD   |
| 54  | TLSSOCKET_SEND_TIMEOUT                  | SHOULD   |
| 55  | TLSSOCKET_ECHOTEST                      | SHOULD   |
| 56  | TLSSOCKET_ECHOTEST_NONBLOCK             | SHOULD   |
| 57  | TLSSOCKET_ENDPOINT_CLOSE                | SHOULD   |
| 58  | TLSSOCKET_NO_CERT                       | SHOULD   |
| 59  | TLSSOCKET_RECV_TIMEOUT                  | SHOULD   |
| 60  | TLSSOCKET_SIMULTANEOUS_TEST             | SHOULD   |
| 61  | TLSSOCKET_ECHOTEST_BURST                | SHOULD   |
| 62  | TLSSOCKET_ECHOTEST_BURST_NONBLOCK       | SHOULD   |
| 63  | ASYNCHRONOUS_DNS                        | MUST     |
| 64  | ASYNCHRONOUS_DNS_CACHE                  | MUST     |
| 65  | ASYNCHRONOUS_DNS_CANCEL                 | MUST     |
| 66  | ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE   | MUST     |
| 67  | ASYNCHRONOUS_DNS_INVALID_HOST           | MUST     |
| 68  | ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC    | MUST     |
| 69  | ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE     | MUST     |
| 70  | ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT    | MUST     |
| 71  | ASYNCHRONOUS_DNS_SIMULTANEOUS           | MUST     |
| 72  | ASYNCHRONOUS_DNS_TIMEOUTS               | MUST     |
| 73  | SYNCHRONOUS_DNS                         | MUST     |
| 74  | SYNCHRONOUS_DNS_CACHE                   | MUST     |
| 75  | SYNCHRONOUS_DNS_INVALID_HOST            | MUST     |
| 76  | SYNCHRONOUS_DNS_MULTIPLE                | MUST     |



Building test binaries
--------------------------

For testing the board and driver, test against the Mbed OS master branch to get the most recent, up-to-date test cases and drivers.

To create a build environment:

```.sh
mbed new network_test
cd network_test
cd mbed-os
git checkout master
cd ..
```

Also, building socket test cases requires a special macro to enable all tests, so create an `mbed_app.json` file with the following content at minimum:

```
{
    "config": {
        "echo-server-addr" : {
            "help" : "IP address of echo server",
            "value" : "\"echo.mbedcloudtesting.com\""
        },
        "echo-server-port" : {
            "help" : "Port of echo server",
            "value" : "7"
        },
        "echo-server-discard-port" : {
            "help" : "Discard port of echo server",
            "value" : "9"
        },
        "echo-server-port-tls" : {
            "help" : "Echo port of echo server",
            "value" : "2007"
        },
        "echo-server-discard-port-tls" : {
            "help" : "Discard port of echo server",
            "value" : "2009"
        }
    }
}
```

Wi-Fi tests require some more configuration, so for Wi-Fi purposes, the `mbed_app.json` might look like this:

```
{
    "config": {
        "wifi-secure-ssid": {
            "help": "WiFi SSID for WPA2 secured network",
            "value": "\"test-network\""
        },
        "wifi-unsecure-ssid": {
            "help": "WiFi SSID for unsecure netwrok",
            "value": "\"unsecure-test-net\""
        },
        "wifi-password": {
            "help": "WiFi Password",
            "value": "\"password\""
        },
        "wifi-secure-protocol": {
            "help": "WiFi security protocol, valid values are WEP, WPA, WPA2, WPA_WPA2",
            "value": "\"WPA2\""
        },
        "wifi-ch-secure": {
            "help": "Channel number of secure SSID",
            "value": 6
        },
        "wifi-ch-unsecure": {
            "help": "Channel number of unsecure SSID",
            "value": 6
        },
        "ap-mac-secure": {
            "help": "BSSID of secure AP in form of AA:BB:CC:DD:EE:FF",
            "value": "\"58:8b:f3:99:f2:9c\""
        },
        "ap-mac-unsecure": {
            "help": "BSSID of unsecure AP in form of \"AA:BB:CC:DD:EE:FF\"",
            "value": "\"58:8b:f3:99:c2:08\""
        },
        "max-scan-size": {
            "help": "How many networks may appear in Wifi scan result",
            "value": 30
        },
        "echo-server-addr" : {
            "help" : "IP address of echo server",
            "value" : "\"echo.mbedcloudtesting.com\""
        },
        "echo-server-port" : {
            "help" : "Port of echo server",
            "value" : "7"
        }
        "echo-server-port-tls" : {
            "help" : "Port of echo server TLS",
            "value" : "2007"
        }
    },
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "WIFI",
            "nsapi.default-wifi-ssid": "\"WIFI_SSID\"",
            "nsapi.default-wifi-password": "\"WIFI_PASSWORD\"",
            "nsapi.default-wifi-security": "WPA_WPA2"
        }
    }
}
```
Please, see `mbed-os/tools/test_configs` folder for examples.

Now build test binaries:

```.sh
mbed test --compile -t <toolchain> -m <target> -n mbed-os-tests-network-*,mbed-os-tests-netsocket*
```

Running tests
-------------

When device is connected to network, or in case of wireless device near the access point.

```.sh
mbed test -n mbed-os-tests-network-*,mbed-os-tests-netsocket*
```

Test cases for Socket class
---------------------------

These test are equal for UDPSocket and TCPSocket but are described here because of identical API and behaviour. Socket class is abstract so it cannot be instantiated, therefore these test cases are implemented using both TCPSocket and UDPSocket. Some of these tests are also implemented for TLSSocket class. In such case certificate has to be set for the Socket before calling `open()`, unless specified otherwise in the test's
description.

### SOCKET_OPEN_DESTRUCT

**Description:**

Call `Socket::open()` and then destruct the socket.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create an object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Delete the object.
4.  Repeat 100 times.

**Expected result:**

`Socket::open()` should always return `NSAPI_ERROR_OK`.



### SOCKET_OPEN_LIMIT

**Description:**

 Call `Socket::open()` until it runs out of memory or other internal limit
in the stack is reached.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Repeat until `NSAPI_ERROR_NO_MEMORY` or `NSAPI_ERROR_NO_SOCKET` error code is returned.
4.  Call "delete" for all previously allocated sockets.
5.  Repeat.

**Expected result:**

Should be able to reserve at least 4 sockets. After freeing all sockets, should  be able to reserve same number of sockets.



### SOCKET_OPEN_TWICE

**Description:**

Call `Socket::open()` twice

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Call `Socket::open(stack)`.
4.  Delete the socket.

**Expected result:**

`Socket::open()` should first return `NSAPI_ERROR_OK` and second call `NSAPI_ERROR_PARAMETER`.



### SOCKET_OPEN_CLOSE_REPEAT

**Description:**

Call `Socket::open()` followed by `Socket::close()` and then again `Socket::open()`. Should allow you to reuse the same object.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Call `Socket::close(stack)`.
4.  Call `Socket::open(stack)`.
5.  Call `Socket::close(stack)`.
6.  Delete the socket.

**Expected result:**

All `Socket::open()` and `Socket::close()` calls should return `NSAPI_ERROR_OK`.


### SOCKET_BIND_PORT

**Description:**

Call `Socket::bind(port)'.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Call `Socket::bind(<any non-used port number>);`.
4.  Destroy socket.

**Expected result:**

All calls return `NSAPI_ERROR_OK`.



### SOCKET_BIND_PORT_FAIL

**Description:**

Call `Socket::bind(port)` on port number that is already used

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Call `Socket::bind(<any non-used port number>);`.
4.  Repeat 1-3 for a new socket.
5.  Destroy both sockets.

**Expected result:**

Second `Socket::bind()` should return `NSAPI_ERROR_PARAMETER`.



### SOCKET_BIND_ADDRESS_PORT

**Description:**

Call `Socket::bind(addr, port)`.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Get address by calling `NetworkInterface::get_ip_address()`.
4.  Call `Socket::bind(address, <any non-used port number>);`.
5.  Destroy socket.

**Expected result:**

All calls return `NSAPI_ERROR_OK`.



### SOCKET_BIND_ADDRESS_NULL

**Description:**

Call `Socket::bind(NULL, port)`.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Call `Socket::bind(NULL, <any non-used port number>);`.
4.  Destroy socket.

**Expected result:**

`Socket::bind()` should return `NSAPI_ERROR_OK'.



### SOCKET_BIND_ADDRESS_INVALID

**Description:**

Call `Socket::bind(address, port)` with and address that is not assigned
to us.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Check whether device is IPv4 or IPv6 connected.
    1.  For IPv4: Call `Socket::bind("190.2.3.4", <any non-used port number>);`.
    2.  For IPv6: Call `Socket::bind("fe80::ff01", <any non-used port number>);`.

4.  Destroy socket.

**Expected result:**

`Socket::bind()` should return `NSAPI_ERROR_PARAMETER'.



### SOCKET_BIND_ADDRESS_WRONG_TYPE

**Description:**

Call `Socket::bind(SocketAddress)` with and address that is not wrong type for the connection.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Check whether device is IPv4 or IPv6 connected.
    1.  For IPv4: Create `SocketAddress("fe80::ff01", <any non-used port number>);`.
    2.  For IPv6: Create `SocketAddress("190.2.3.4", <any non-used port number>);`.

4.  Call `Socket::bind(address);`.
5.  Destroy socket.

**Expected result:**

`Socket::bind()` should return `NSAPI_ERROR_PARAMETER'.



### SOCKET_BIND_ADDRESS

**Description:**

Call `Socket::bind(SocketAddress)'.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::open(stack)`.
3.  Get address by calling NetworkInterface::get_ip_address();
4.  Create a SocketAddress object using this address and any non-used.
    port number.
5.  Call `Socket::bind(address);`.
6.  Destroy socket.

**Expected result:**

All calls return `NSAPI_ERROR_OK'.



### SOCKET_BIND_UNOPENED

**Description:**

Call `Socket::bind()` on socket that has not been opened.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create a object by calling `new Socket()`.
2.  Call `Socket::bind(<any non-used port number>);`.
3.  Destroy socket.

**Expected result:**

NSAPI_ERROR_NO_SOCKET



Test cases for UDPSocket class
------------------------------

### UDPSOCKET_OPEN_DESTRUCT

**Description:** Run SOCKET_OPEN_DESTRUCT for UDPSocket.

### UDPSOCKET_OPEN_LIMIT

**Description:** Run SOCKET_OPEN_LIMIT for UDPSocket.

### UDPSOCKET_OPEN_TWICE

**Description:** Run SOCKET_OPEN_TWICE for UDPSocket.

### UDPSOCKET_OPEN_CLOSE_REPEAT

**Description:** Run SOCKET_OPEN_CLOSE_REPEAT for UDPSocket.

### UDPSOCKET_BIND_PORT

**Description:** Run SOCKET_BIND_PORT for UDPSocket.

### UDPSOCKET_BIND_PORT_FAIL

**Description:** Run SOCKET_BIND_PORT_FAIL for UDPSocket.

### UDPSOCKET_BIND_ADDRESS_PORT

**Description:** Run SOCKET_BIND_ADDRESS_PORT for UDPSocket.

### UDPSOCKET_BIND_ADDRESS_NULL

**Description:** Run SOCKET_BIND_ADDRESS_NULL for UDPSocket.

### UDPSOCKET_BIND_ADDRESS_INVALID

**Description:** Run SOCKET_BIND_ADDRESS_INVALID for UDPSocket.

### UDPSOCKET_BIND_WRONG_TYPE

**Description:** Run SOCKET_BIND_WRONG_TYPE for UDPSocket.

### UDPSOCKET_BIND_ADDRESS

**Description:** Run SOCKET_BIND_ADDRESS for UDPSocket.

### UDPSOCKET_BIND_UNOPENED

**Description:** Run SOCKET_BIND_UNOPENED for UDPSocket.

### UDPSOCKET_SENDTO_INVALID

**Description:**

Call `UDPSocket::sendto()` with invalid parameters.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket:sendto( NULL, 9, NULL, 0);`.
2.  Call `UDPSocket:sendto( "", 9, NULL, 0);`.
3.  Call `UDPSocket:sendto( "", 0, NULL, 0);`.
4.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 9,NULL, 0);`.
5.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 9, "hello", 5);`.
6.  Destroy the socket.

**Expected result:**

All sendto() calls should return some error code except:

-   step 4 should return 0
-   step 5 should return 5



### UDPSOCKET_SENDTO_REPEAT

**Description:**

Repeatedly send small packets.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 9, "hello", 5);`.
2.  Repeat 100 times.
3.  Fail if `NSAPI_ERROR_NO_MEMORY` is returned two times in a row,.
    Wait 1 second before retry
4.  Destroy the socket.

**Expected result:**

All sendto() calls should return 5.



### UDPSOCKET_ECHOTEST

**Description:**

Repeatedly send packets to echo server and read incoming packets back.
Verify working of different packet sizes.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = loop index>, <loop index>);`.
2.  Wait for incomming packet. If timeout happens, retry.
    sending&receiving, max 3 times.
3.  Verify incomming content was the same that was sent.
4.  Repeat 1200 times.
5.  Destroy the socket.

**Expected result:**

All sendto() calls should return the packet size. All recvfrom() calls
should return the same sized packet that was send with same content.
Calculate packet loss rate, maximum tolerated packet loss rate is 30%



### UDPSOCKET_ECHOTEST_NONBLOCK

**Description:**

Repeatedly send packets to echo server and read incoming packets back.
Verify working of different packet sizes. Use socket in non-blocking
mode

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket::set_blocking(false)`.
2.  Register event handler with `UDPSocket::sigio()`.
3.  Create another thread that constantly waits signal from sigio() handler, when received try `UDPSocket::recvfrom()`.
4.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = loop index>, <loop index>);`.
5.  Wait for incomming packet for one second. If nothing received retry,
    max 3 times.
6.  Verify incomming content was the same that was sent.
7.  Repeat 1200 times.
8.  Destroy the socket.

**Expected result:**

All sendto() calls should return the packet size. All recvfrom() calls
should return the same sized packet that was send with same content or
NSAPI_ERROR_WOULD_BLOCK.

Calculate packet loss rate, maximum tolerated packet loss rate is 30%



### UDPSOCKET_RECV_TIMEOUT

**Description:**

Test whether timeouts are obeyed in UDPSockets.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `UDPSocket::set_timeout(100)`.
2.  Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
3.  Repeat 5 times.
    1.  Record a time in millisecond precission.
    2.  Call `UDPSocket::recvfrom()`.
    3.  Record a time in millisecond precission.

4.  Repeat testcase 10 times.

**Expected result:**

Each `sendto()` calls should return 100.

Within each loop, one `recvfrom()` may return the received packet size
(100). Other calls should return `NSAPI_ERROR_WOULD_BLOCK`.

When `NSAPI_ERROR_WOULD_BLOCK` is received, check that time consumed is
more that 100 milliseconds but less than 200 milliseconds.

After repeating for 10 times, at least 5 packets must have been
received.


### UDPSOCKET_SENDTO_TIMEOUT

**Description:**

Test whether timeouts are obeyed in UDPSockets.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Record time.
2.  Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 9, <random packet, size = 100>, 100);`.
3.  Record time.
4.  Call `UDPSocket::set_timeout(1000)`.
5.  Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 9, <random packet, size = 100>, 100);`.
6.  Record time.

**Expected result:**

Each sendto() call should return 100.

All sendto() calls should return faster than 100 milliseconds because
UDP sending should not block that long.


Test cases for TCPSocket class
------------------------------

### TCPSOCKET_OPEN_DESTRUCT

**Description:** Run SOCKET_OPEN_DESTRUCT for TCPSocket.

### TCPSOCKET_OPEN_LIMIT

**Description:** Run SOCKET_OPEN_LIMIT for TCPSocket.

### TCPSOCKET_OPEN_TWICE

**Description:** Run SOCKET_OPEN_TWICE for TCPSocket.

### TCPSOCKET_OPEN_CLOSE_REPEAT

**Description:** Run SOCKET_OPEN_CLOSE_REPEAT for TCPSocket.

### TCPSOCKET_BIND_PORT

**Description:** Run SOCKET_BIND_PORT for TCPSocket.

### TCPSOCKET_BIND_PORT_FAIL

**Description:** Run SOCKET_BIND_PORT_FAIL for TCPSocket.

### TCPSOCKET_BIND_ADDRESS_PORT

**Description:** Run SOCKET_BIND_ADDRESS_PORT for TCPSocket.

### TCPSOCKET_BIND_ADDRESS_NULL

**Description:** Run SOCKET_BIND_ADDRESS_NULL for TCPSocket.

### TCPSOCKET_BIND_ADDRESS_INVALID

**Description:** Run SOCKET_BIND_ADDRESS_INVALID for TCPSocket.

### TCPSOCKET_BIND_WRONG_TYPE

**Description:** Run SOCKET_BIND_WRONG_TYPE for TCPSocket.

### TCPSOCKET_BIND_ADDRESS

**Description:** Run SOCKET_BIND_ADDRESS for TCPSocket.

### TCPSOCKET_BIND_UNOPENED

**Description:** Run SOCKET_BIND_UNOPENED for TCPSocket.

### TCPSOCKET_CONNECT_INVALID

**Description:**

Call `TCPSocket::connect()` with invalid parameters.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket:connect( NULL, 9);`.
2.  Call `TCPSocket:connect( "", 9);`.
3.  Call `TCPSocket:connect( "", 0);`.
4.  Call `TCPSocket:connect( "echo.mbedcloudtesting.com", 9);`.
5.  Destroy the socket.

**Expected result:**

All connect() calls should return some error code except the number 4
should return `NSAPI_ERROR_OK`.



### TCPSOCKET_SEND_REPEAT

**Description:**

Repeatedly send small packets.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 9);`.
2.  Call `TCPSocket::send("hello", 5);`.
3.  Repeat 100 times.
4.  Destroy the socket.

**Expected result:**

`TCPSocket::connect()` should return `NSAPI_ERROR_OK'.

All send() calls should return 5.



### TCPSOCKET_ECHOTEST

**Description:**

Repeatedly send packets to echo server and read incoming packets back.
Verify working of different packet sizes.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
2.  Call `TCPSocket::send(<random packet, size = loop index>, <size>);`.
    1.  If less than <loop index> was returned, size = sent bytes.

3.  Call `TCPSocket::recv(buffer, <size>);`.
4.  Verify incomming content was the same that was sent.
5.  Repeat 1200 times.
6.  Destroy the socket.

**Expected result:**

All send() calls should return the packet size or less. All recv() calls
should return the same sized packet that was send with same content.

NOTE: This is stream so recv() might return less data than what was
requested. In this case you need to keep calling recv() until all data
that you have sent is returned.


### TCPSOCKET_ECHOTEST_NONBLOCK

**Description:**

Repeatedly send packets to echo server and read incoming packets back.
Verify working of different packet sizes. Use socket in non-blocking
mode

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
2.  Call `TCPSocket::set_blocking(false)`.
3.  Register event handler with `TCPSocket::sigio()`.
4.  Create another thread that constantly waits signal from `sigio()` handler, when received try `TCPSocket::recv(buf+index, <loop index> - index)`, where index is the amount of data already received.
5.  Call `TCPSocket:send(<random packet, size = loop index>, <loop index>);`.
    1.  If less than <loop index> was returned, try immeadiately sending remaining bytes.
    2.  If `NSAPI_ERROR_WOULD_BLOCK` is returned, wait for sigio() call to happen.

6.  Wait for incomming packet for one second.
7.  Verify incomming content was the same that was sent, set index for receiving thread to zero.
8.  Repeat 1200 times.
9.  Destroy the socket.

**Expected result:**

All send() calls should return the packet size or less. All recv() calls
should return `NSAPI_ERROR_WOULD_BLOCK` or packet size that is equal or
less than what has been sent.

### TCPSOCKET_RECV_TIMEOUT

**Description:**

Test whether timeouts are obeyed in TCPSockets.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
2.  Call `TCPSocket::set_timeout(100);`.
3.  Call `TCPSocket::send(<random packet, size = 100>;, 100);`.
4.  Repeat 5 times.
    1.  Record a time in millisecond precission.
    2.  Call `TCPSocket::recv()`.
    3.  Record a time in millisecond precission.

5.  Repeat testcase 10 times.

**Expected result:**

Each send() call should return 100.

Within each loop, one recv() may return the received packet size (100).
Other calls should return `NSAPI_ERROR_WOULD_BLOCK`.

When `NSAPI_ERROR_WOULD_BLOCK` is received, check that time consumed is
more that 100 milliseconds but less than 200 milliseconds.


### TCPSOCKET_SEND_TIMEOUT

**Description:**

Repeatedly send small packets in a given time limit

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket:connect("echo.mbedcloudtesting.com", 9);`.
3.  Call `TCPSocket:set_blocking(false);`.
3.  Call `TCPSocket:send("hello", 5);`.
4.  Repeat 10 times.
5.  Destroy the socket.

**Expected result:**

`TCPSocket::connect()` should return `NSAPI_ERROR_OK'.

All send() calls should return in less than 800 milliseconds


### TCPSOCKET_ENDPOINT_CLOSE

**Description:**

Test whether we tolerate endpoint closing the connection.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 13);`.
2.  Call `TCPSocket::recv(<buffer>, 30);`.
3.  Repeat until recv() returns 0.
4.  Call `TCPSocket::close();`.
5.  Delete socket.

**Expected result:**
Connect should return `NSAPI_ERROR_OK`.

First recv() should return more that zero. Something between 10 and 30 bytes (datetime string)

Second recv() should return zero because endpoint closed the connection.
close() should return `NSAPI_ERROR_OK'.

### TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID

**Description:**

Test we are able to request setting valid TCP keepalive values

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `TCPSocket::setsockopt(keepalive, [0,1 or 7200]);`.
2.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 9);`.
3.  Call `TCPSocket::getsockopt(keepalive);`.

**Postconditions:**

1.  Call `TCPSocket::close();`.
2.  Delete socket.

**Expected result:**

`TCPSocket::getsockopt(keepalive)` returns same value as was set with `TCPSocket::setsockopt()` or `NSAPI_ERROR_UNSUPPORTED'.

Test cases for TLSSocket class
------------------------------

### TLSSOCKET_OPEN_DESTRUCT

**Description:** Run SOCKET_OPEN_DESTRUCT for TLSSocket.

### TLSSOCKET_OPEN_LIMIT

**Description:** Run SOCKET_OPEN_LIMIT for TLSSocket.

### TLSSOCKET_OPEN_TWICE

**Description:** Run SOCKET_OPEN_TWICE for TLSSocket.

### TLSSOCKET_CONNECT_INVALID

**Description:** Run SOCKET_CONNECT_INVALID for TLSSocket.

### TLSSOCKET_HANDSHAKE_INVALID

**Description:**

Execute TLS handshake by calling `TLSSocket::connect()` - server must not match to the certificate used by to os.mbed.com

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create TLSSocket.
2.  Call `TLSSocket::open()`.
3.  Call `TLSSocket::connect("os.mbed.com", 2009)`.
4.  Call `TLSSocket::close()`.

**Expected result:**

TLSSocket::connect must return an error

### TLSSOCKET_SEND_CLOSED

**Description:**

Make a HTTP request to a closed socket.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create TLSSocket.
2.  Call `TLSSocket::open()`.
3.  Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007)`.
4.  Call `TLSSocket::close()`.
5.  Call `TLSSocket::send("12345", 5)`.

**Expected result:**

TLSSocket::send must return an error

### TLSSOCKET_SEND_REPEAT

**Description:** Run SOCKET_SEND_REPEATÂ for TLSSOCKET by using port number 2009.

### TLSSOCKET_SEND_TIMEOUT

**Description:** Run SOCKET_SEND_TIMEOUTÂ for TLSSOCKET by using port number 2009.

### TLSSOCKET_SEND_UNCONNECTED

**Description:** 

Make a HTTP request to an unconnected socket.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create TLSSocket.
2.  Call `TLSSocket::open()`.
3.  Call `TLSSocket::send("12345", 5)`.
4.  Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007)`.

**Expected result:**

TLSSocket::send must return an error

### TLSSOCKET_ECHOTEST

**Description:** Run SOCKET_ECHOTESTÂ for TLSSOCKET by using port number 2007.

### TLSSOCKET_ECHOTEST_NONBLOCK

**Description:** Run SOCKET_ECHOTEST_NONBLOCKÂ for TLSSOCKET by using port number 2007.

### TLSSOCKET_ENDPOINT_CLOSE

**Description:** Run SOCKET_ENDPOINT_CLOSE for TLSSOCKET by using port number 2013.

### TLSSOCKET_NO_CERT

**Description:**

Verify that TLS Socket fails to connect without certificate.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.

**Test steps:**

1.  Create TLSSocket, without adding a default certificate.
2.  Call `TLSSocket::open()`.
3.  Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2009)`.

**Expected result:**

TLSSocket::connect must return an error if the certificate is not present.

### TLSSOCKET_RECV_TIMEOUT

**Description:**

Run TCPSOCKET_RECV_TIMEOUTÂ for TLSSOCKET by using port number 2007.

### TLSSOCKET_SIMULTANEOUS_TEST

**Description:**

SimultaneouslyÂ send packets to echo server on two opened sockets and read incoming packets back. Verify working of two TLS sockets open and operate simultaneously.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TLSSockets are open and one additional thread has been created.
4.  Both threads get their own socket instance.

**Test steps:**

1.  On main thread:
    1.  Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007);`.
    2.  Call `TLSSocket::send(<random packet, size = loop index>, <loop index>);`.
        1.  If less than <loop index> was returned, size = sent bytes.

    3.  `Call TLSSocket::recv(buffer, <size>);`.
    4.  Verify incomming content was the same that was sent.
    5.  Repeat 100 times.

2.  Simultaneously with the earlier step do on the additional thread:
    1.  Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007);`.
    2.  Call `TLSSocket::send(<random packet, size = loop index>, <loop index>);'.
        1.  If less than <loop index> was returned, size = sent bytes.

    3.  Call `TLSSocket::recv(buffer, <size>);`.
    4.  Verify incomming content was the same that was sent.
    5.  Repeat 100 times.

3.  Wait for end additional thread.
4.  Close and destroy the sockets.

**Expected result:**

All send() calls should return the packet size or less. All recv() calls on main thread should return the same sized packet that was send with same content.Â All recv() calls on additionalÂ thread should return the valid daytime string .Â 

Performance tests
-----------------


### UDPSOCKET_ECHOTEST_BURST

**Description:**

Send burst of packets to echo server and read incoming packets back.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
2.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 200>, 200);`.
3.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 300>, 300);`.
4.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 120>, 120);`.
5.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 500>, 500);`.
6.  Wait for incomming packets for five second.
7.  Verify incomming content was the same that was sent. Allow packet reordering.
8.  Repeat 100 times.
9.  Destroy the socket.

**Expected result:**

All sendto() calls should return the packet size.

All recvfrom() calls should return the same sized packet that was send with same content. Allow packet reordering.

Calculate packet loss rate, maximum tolerated packet loss rate is 30%.

Calculate number of succesfull rounds, it should be higher than 70.


### UDPSOCKET_ECHOTEST_BURST_NONBLOCK

**Description:**

Send burst of packets to echo server and read incoming packets back. Use socket in non-blocking mode.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  UDPSocket is open.

**Test steps:**

1.  Call `UDPSocket::set_blocking(false)`.
2.  Register event handler with `UDPSocket::sigio()`.
3.  Create another thread that constantly waits signal from sigio() handler, when received try `UDPSocket::recvfrom()`.
4.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
5.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 200>, 200);`.
6.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 300>, 300);`.
7.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 120>, 120);`.
8.  Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 500>, 500);`.
9.  Wait for incomming packets for five second.
10. Verify incomming content was the same that was sent. Allow packet reordering.
11. Repeat 100 times.
12. Destroy the socket.

**Expected result:**

All sendto() calls should return the packet size.

All recvfrom() calls should return the same sized packet that was send with same content. Allow packet reordering.

Calculate packet loss rate, maximum tolerated packet loss rate is 30%.

Calculate number of succesfull rounds, it should be higher than 70.



### TCPSOCKET_ECHOTEST_BURST

**Description:**

Send burst of packets to echo server and read incoming packets back.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
2.  Call `TCPSocket::send(<random packet, size = 100>, 100);`.
3.  Call `TCPSocket::send(<random packet, size = 200>, 200);`.
4.  Call `TCPSocket::send(<random packet, size = 300>, 300);`.
5.  Call `TCPSocket::send(<random packet, size = 120>, 120);`.
6.  Call `TCPSocket::send(<random packet, size = 500>, 500);`.
7.  Call `TCPSocket::recv(buf, 1220)`.
8.  Verify incomming content was the same that was sent.
9.  Repeat 100 times.
10. Destroy the socket.

**Expected result:**

All send() calls should return the packet size.

NOTE: This is stream so recv() might return less data than what was requested. In this case you need to keep calling recv() with remaining size until all data that you have sent is returned.

Consecutive calls to recv() should return all the data that has been send. Total amount of returned must match 1220.


### TCPSOCKET_ECHOTEST_BURST_NONBLOCK

**Description:**

Send burst of packets to echo server and read incoming packets back. Use socket in non-blocking mode.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Register event handler with `TCPSocket::sigio()`.
2.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
3.  Call `TCPSocket::set_blocking(false)`.
4.  Create another thread that constantly waits signal from sigio() handler, when received try `TCPSocket::recv()`.
5.  For randomly generated packets, sized 100, 200, 300, 120 and 500 do
    1.  Call `TCPSocket::send(packet, size);`.
    2.  If less than size is sent, repeat with remaining.
    3.  If `NSAPI_ERROR_WOULD_BLOCK` returned, wait for next sigio().

6.  Wait for incomming packets for five second.
7.  Verify incomming content was the same that was sent. Allow recv() to return smaller piezes.
8.  Repeat 100 times.
9.  Destroy the socket.

**Expected result:**

All send() calls should return `NSAPI_ERROR_WOULD_BLOCK` or size which is equal or less than requested.

All recv() calls should return value that is less or equal to what have been sent. With consecutive calls, size should match.

When recv() returns `NSAPI_ERROR_WOULD_BLOCK` wait for next sigio() event. No other error codes allowed.


### TCPSOCKET_RECV_100K

**Description:**

Download 100kB of data

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 19);`.
2.  Call `TCPSocket::recv(buffer, 100);`.
3.  Verify input according to known pattern.
4.  Loop until 100kB of data received.
5.  Close socket.

**Expected result:**

Each recv() call should return equal or less than 100 bytes of data. No
errors should be returned.

Measure time taken for receiving, report speed

### TCPSOCKET_RECV_100K_NONBLOCK

**Description:**

Download 100kB of data

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  TCPSocket is open.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 19);`.
2.  Call `TCPSocket::set_blocking(false)`.
3.  Create another thread that constantly waits signal from sigio() handler, when received try `TCPSocket::recv()`.
    1.  Call `TCPSocket::recv(buffer, 100);`.
    2.  Verify input according to known pattern.

4.  Wait until 100kB of data received.
5.  Close socket.

**Expected result:**

Each recv() call should return equal or less than 100 bytes of data or `NSAPI_ERROR_WOULD_BLOCK` in which case thread should wait for another sigio(). No errors should be returned.

Measure time taken for receiving, report speed.

### TCPSOCKET_THREAD_PER_SOCKET_SAFETY

**Description:**

Run two threads which both exercise the underlying stack and driver through a dedicated socket.

**Preconditions:**

1.  Network interface and stack are initialized.
2.  Network connection is up.
3.  2 TCPSockets are open and one additional thread has been created.
4.  Both threads get their own socket instance.

**Test steps:**

1.  Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7)` in both threads - in the main thread executing the test case and on the additional one.
2.  On main thread
    1.  For randomly generated packets, sized 1001, 901, 801,...,101,1 do
        1.  Call `TCPSocket::send(packet, size);`.
        2.  Verify incoming content was the same that was sent.
            Allow recv() to return smaller pieces.

3.  Simultaneously with the earlier step do on the additional thread
    1.  For randomly generated packets, sized 10 do
        1.  Call `TCPSocket::send(packet, size);`.
        2.  Verify incomming content was the same that was sent.
            Allow recv() to return smaller piezes.
        3.  Stop the thread if inconsistensies were found and report it to main thread.

4.  Kill the additional thread.
5.  Close and destroy the sockets.

**Expected result:**

Echo server returns data to both threads and received data matches to send data. The additional thread isn't stopped prematurely.

Test cases for DNS class
---------------------------

### ASYNCHRONOUS_DNS

**Description:**

Verify the basic functionality of asynchronous DNS. Call `NetworkInterface::gethostbyname_async()` with a valid host name, and verify the result.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` with a valid host name and a callback.
2.  Verify that callback is called with correct parameters.

**Expected result:**

Callback is called with `NSAPI_ERROR_OK` and IP address.

### ASYNCHRONOUS_DNS_SIMULTANEOUS

**Description:**

Verify that simultaneous asynchronous DNS queries work correctly. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names. Wait for all requests to complete, and verify the result. Cache should not contain host names used in asynchronous request.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` in a row 6 times with a different host names. Host names shall not be found from cache.
2.  Verify that last `gethostbyname_async()` operation is rejected because there is room only for 5 simultaneous operations.
3.  Verify that callback is called with correct parameters 5 times.

**Expected result:**

Sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_OK` and IP address 5 times.

### ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE

**Description:**

Verify that the caching of DNS results works correctly with simultaneous asynchronous DNS queries. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names. Wait for all requests to complete and verify result. Cache shall contain at least one host name used in asynchronous request. You can achieve this, for example, by running test "Asynchronous DNS simultaneous" before this test and using same host names in this run.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` in a row 6 times with a different host names. At least one host name shall be found from cache.
2.  Verify that callback is called with correct parameters 6 times.

**Expected result:**

Callback is called with `NSAPI_ERROR_OK` and IP address 6 times.

### ASYNCHRONOUS_DNS_CACHE

**Description:**

Verify that the caching of DNS results works correctly. Call `NetworkInterface::gethostbyname_async()` 5 times with the same host name and verify result after each request. For first request, cache shall not contain the host name. Verify that first request completes slower than the requests made after it (where the response is found from cache).

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` with a host name. For first request, host name shall not be found from cache.
2.  Verify that callback is called with correct parameters.
3.  Repeat the sequence 4 times using the same host name.
4.  For each request, calculate how long time it takes for DNS query to complete.

**Expected result:**

Callback is called with `NSAPI_ERROR_OK` and IP address 5 times. First request shall complete before the requests made after it (where the response is found from cache).

### ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC

**Description:**

Verify synchronous (in other words, blocking) DNS queries and asynchronous (in other words, non-blocking) queries work at the same time. Call `NetworkInterface::gethostbyname_async()`. Right after that, make 6 synchronous `NetworkInterface::gethostbyname()` calls with different host names.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` with a host name. Host name shall not be found from cache.
2.  Call `gethostbyname()` 6 times with a different host names (none of the names shall be same as in step 1).
3.  Verify that each `gethostbyname()` returns success.
4.  Verify that the asynchronous callback is called with correct parameters.

**Expected result:**

All operations shall return `NSAPI_ERROR_OK` and IP address.

### ASYNCHRONOUS_DNS_CANCEL

**Description:**

Verify that asynchronous DNS query cancel works correctly. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names. Cache shall contain 3 host names used in requests. This can be achieved e.g. by running test "Asynchronous DNS synchronous and asynchronous" before this test and using same host names in this run. For each request that was given an unique ID, call cancel. Verify that callback is not called for canceled requests.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` in a row 6 times with a different host names. Cache shall contain in maximum 3 host names used in requests.
2.  Call `gethostbyname_async_cancel()` for each request that was given an unique ID.
3.  Verify that for canceled requests, callback is not called.
4.  Verify that for other requests, callback is called.

**Expected result:**

Callback shall be called only for requests that were not canceled.

### ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE

**Description:**

Verify that providing an external event queue works correctly. Define a thread and an event queue running on it. Define a DNS call in callback function that uses the event queue (call_in_callback_cb_t). Enable external event queue. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Define a thread and an event queue running on it.
2.  Define a DNS call in callback function that uses the event queue (call_in_callback_cb_t).
3.  Start thread and event queue.
4.  Set DNS callback function using the `nsapi_dns_call_in_set()` call.
5.  Call `gethostbyname_async()` in a row 6 times with a different host names. Host names shall not be found from cache.
6.  Verify that last `gethostbyname_async()` operation is rejected since there is room only for 5 simultaneous operations.
7.  Verify that callback is called with correct parameters 5 times.
8.  Restore default DNS callback function using the `nsapi_dns_call_in_set()` call.

**Expected result:**

Sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_OK` and IP address 5 times.

### ASYNCHRONOUS_DNS_INVALID_HOST

**Description:**

Verify that DNS failure error is provided for invalid hosts. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names. First, third and fifth host name shall be invalid.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` in a row 6 times with a different host names. Host names shall not be found from cache. First, third and fifth host name shall be invalid.
2.  Verify that last `gethostbyname_async()` operation is rejected since there is room only for 5 simultaneous operations.
3.  Verify that callback is called with correct parameters 5 times.

**Expected result:**

Sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_DNS_FAILURE` for first, third and fifth host name. Callback is called with `NSAPI_ERROR_OK` and IP address for second and fourth host name.

### ASYNCHRONOUS_DNS_TIMEOUTS

**Description:**

Test DNS timeouts using an external event queue that is modified to timeout the events faster that standard event queue. In this test event queue shall not delay events, instead it handles those immediately. Call `NetworkInterface::gethostbyname_async()` in a row 6 times with a different host names. All or some of the request shall timeout and timeout return value is returned.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Define a thread and an event queue running on it.
2.  Define a DNS call in callback function that uses the event queue (call_in_callback_cb_t). Callback function shall not delay callbacks; instead it shall handle those immediately.
3.  Start thread and event queue.
4.  Set DNS callback function using the `nsapi_dns_call_in_set()` call.
5.  Call `gethostbyname_async()` in a row 6 times with a different host names. Host names shall not be found from cache.
6.  Verify that last `gethostbyname_async()` operation is rejected since there is room only for 5 simultaneous operations.
7.  Verify that callback is called with correct parameters 5 times.

**Expected result:**

Sixth `gethostbyname_async()` is rejected. At least for one operation, callback is called with `NSAPI_ERROR_TIMEOUT` value.

### ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT

**Description:**

Verify that simultaneous asynchronous DNS queries work correctly when repeated in sequence. Call `NetworkInterface::gethostbyname_async()` in a row 5 times with a different host names. Wait for all requests to complete and verify result. Repeat the procedure 100 times.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname_async()` in a row 5 times with a different host names, providing a callback to be called when the operation completes.
2.  Verify that callback is called with correct parameters 5 times for the first operation.
3.  Repeat 100 times steps 1 to 2.

**Expected result:**

Callback, registered for `gethostbyname_async()`, is called with `NSAPI_ERROR_OK` and an IP address 5 times for every one of a hundred repetitions of the test.

### SYNCHRONOUS_DNS

**Description:**

Verify the basic functionality of synchronous DNS. Call `NetworkInterface::gethostbyname()` with a valid host name, and verify the result.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname()` with a valid host name.
2.  Verify the address was resolved and the return value was valid.

**Expected result:**

Return value is `NSAPI_ERROR_OK` and IP address is obtained from the function call.

### SYNCHRONOUS_DNS_MULTIPLE

**Description:**

Verify the basic functionality of synchronous DNS. Call `NetworkInterface::gethostbyname()` with a list of 6 host names, and verify the result.

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname()` with a list of 6 host names.
2.  Verify that each of the addresses was resolved and the return value was valid.

**Expected result:**

Return value is `NSAPI_ERROR_OK` and IP addresses are obtained from the function call.

### SYNCHRONOUS_DNS_CACHE

**Description:**

Verify that the caching of DNS results works correctly. Call `NetworkInterface::gethostbyname()` 5 times with the same host name, and verify the result after each request. For first request, cache shall not contain the host name. Verify that first request completes before the requests made after it (where the response is found from cache).

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname()` with a host name. For the first request, host name should not be found in cache and an error should be returned.
2.  Verify that address was resolved and the return value was valid.
3.  Repeat the sequence 4 times using the same host name.
4.  For each request, calculate how long time it takes for DNS query to complete.

**Expected result:**

Return value is `NSAPI_ERROR_OK` and IP address is obtained from the function call 5 times. First request shall complete before the requests made after it (where the response is found from cache).

### SYNCHRONOUS_DNS_INVALID_HOST

**Description:**

Verify that DNS failure error is provided for invalid hosts. Call `NetworkInterface::gethostbyname()` in a row 6 times with different host names. First, third and fifth of the host names used in this test shall be invalid (for example by adding an incorrect suffic, like so: "google.com_invalid").

**Preconditions:**

1.  Network interface is initialized.
2.  Network connection is up.

**Test steps:**

1.  Call `gethostbyname()` in a row 6 times with a different host names. Host names shall not be found from cache. First, third and fifth host name shall be invalid.
2.  Verify that return value was valid and for valid hostnames the address was resolved 6 times.

**Expected result:**

Return value is `NSAPI_ERROR_DNS_FAILURE` for first, third and fifth host name, which were invalidated at the beginning of the test. Return value is `NSAPI_ERROR_OK` and IP address is obtained for second and fourth host name, which were valid.

Subset for driver test
----------------------

### For physical layer driver (emac, PPP):

-   TCPSOCKET_ECHOTEST
-   TCPSOCKET_ECHOTEST_BURST
-   TCPSOCKET_ECHOTEST_BURST_NONBLOCK
-   TCPSOCKET_ECHOTEST_NONBLOCK
-   TCPSOCKET_RECV_100K
-   TCPSOCKET_RECV_100K_NONBLOCK
-   TCPSOCKET_RECV_TIMEOUT
-   TCPSOCKET_SEND_REPEAT
-   UDPSOCKET_BIND_SENDTO
-   UDPSOCKET_ECHOTEST
-   UDPSOCKET_ECHOTEST_NONBLOCK
-   UDPSOCKET_RECV_TIMEOUT
-   UDPSOCKET_SENDTO_INVALID
-   UDPSOCKET_SENDTO_REPEAT
-   UDPSOCKET_SENDTO_TIMEOUT

### For socket layer driver (AT-driven, external IP stack):

All Socket, UDPSocket, TCPSocket and TLSSocket testcases.