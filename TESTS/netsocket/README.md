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

Please see the [Network Socket documentation](https://os.mbed.com/docs/mbed-os/latest/apis/network-socket.html) for reference.

Tools to use
----------------

-   Mbed OS.
-   Standard Mbed OS development tools as described in the [Mbed OS tools overview](https://os.mbed.com/docs/latest/tools/index.html).
-   Test server.

These test cases themselves do not require any special tooling, other than the test server described in "Test environment" chapter.

Test environment
----------------

As a general guideline, network connectivity with public Internet access is required. This satisfies Socket tests, but specific connectivity methods might require some extra configuration or devices within the network.

The test environment consist of DUTs, network connection and the test server. Arm provides a public test server, but it can be installed locally as well, if an isolated test environment is required.

### Public test server

Address: `echo.mbedcloudtesting.com`.

Both IPv4 and IPv6 addresses are available from a public DNS service:

```.sh
$ host echo.mbedcloudtesting.com
echo.mbedcloudtesting.com has address 52.215.34.155
echo.mbedcloudtesting.com has IPv6 address 2a05:d018:21f:3800:8584:60f8:bc9f:e614
```

**Open services in the test server**

-   Echo protocol, [RFC 862](https://tools.ietf.org/html/rfc862) is enabled on both TCP and UDP on port 7. Port 2007 for TLS
-   Discard protocol, [RFC 863](https://tools.ietf.org/html/rfc863) is enabled in both TCP and UDP on port 9. Port 2009 for TLS.
-   Character generator protocol, [RFC 864](https://tools.ietf.org/html/rfc864) is enabled in both TCP and UDP on port 19. Port 2019 for TLS. The output pattern follows the proposed example pattern in RFC.
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

Below is an example of how to install these services in TLS version into a Debian/Ubuntu based Linux distribution using Stunnel4 Daemon:

```.sh
$ sudo apt install stunnel4
$ nano /etc/stunnel/stunnel.conf
```

Enable following services from /etc/inetd.conf:

```
; **************************************************************************
; * Service definitions (remove all services for inetd mode)               *
; **************************************************************************

[echo]
accept  = :::2007
connect = 7
cert = /etc/letsencrypt/live/<test_server_url>/fullchain.pem
key = /etc/letsencrypt/live/<test_server_url>/privkey.pem

[discard]
accept  = :::2009
connect = 9
cert = /etc/letsencrypt/live/<test_server_url>/fullchain.pem
key = /etc/letsencrypt/live/<test_server_url>/privkey.pem

[daytime]
accept  = :::2013
connect = 13
cert =/etc/letsencrypt/live/<test_server_url>/fullchain.pem
key = /etc/letsencrypt/live/<test_server_url>/privkey.pem

[chargen]
accept  = :::2019
connect = 19
cert = /etc/letsencrypt/live/<test_server_url>/fullchain.pem
key = /etc/letsencrypt/live/<test_server_url>/privkey.pem

```

Get, update and install certificate files by certbot (Provided by Let's Encrypt <https://letsencrypt.org/>).

-   Install lighthttpd server:
   
   ```.sh
   $ sudo apt-get install lighttpd
   $ sudo rm -rf /var/www/html/*
   $ sudo echo "<html><body><h1>Empty</h1>" > /var/www/html/index.html
   $ sudo echo "</body></html>" >> /var/www/html/index.html
   $ sudo chown www-data:www-data /var/www/html/index.html
   $ sudo systemctl restart lighttpd.service
   ```

-   Install and set up certbot:
   
   ```.sh
   $ sudo apt-get update
   $ sudo apt-get install software-properties-common
   $ sudo add-apt-repository ppa:certbot/certbot
   $ sudo apt-get update
   $ sudo apt-get install certbot
   $ sudo certbot certonly
   $ sudo certbot certonly --webroot -w /var/www/html -d <test_server_url>
   ```
   
-   Set test server to renew certificate before expiry.
   
   ```.sh
   $ sudo echo "SHELL=/bin/sh" > /etc/cron.d/certbot
   $ sudo echo "PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin" > /etc/cron.d/certbot
   $ sudo echo "0 */12 * * * root test -x /usr/bin/certbot -a \! -d /run/systemd/system && perl -e 'sleep int(rand(43200))' && certbot -q renew" > /etc/cron.d/certbot
   ```

Where <test_server_url> is the test server URL.

**Testing the connectivity**

You can connect to the test server with an NMAP tool:

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

The Ethernet test environment consists of devices, an Ethernet switch and an optional firewall that allows connecting to the Echo server.

### Wi-Fi test environment

![Wi-Fi](wifi_environment.png)

The Wi-Fi test environment is equivalent to the Ethernet test environment, except that the Wi-Fi test environment has two separate access points or one with dual SSID. Connectivity to echo server is required, but it can be hosted locally, as specified in the Ethernet environment.

Test case priorities
--------------------

Please refer to the following table for priorities of test cases. Priorities are labeled as MUST and SHOULD. MUST means this is a requirement and therefore mandatory to pass the test. SHOULD means it is recommended to pass the test if the driver implements the feature in question.

|     | Test case                                     | Priority |
|-----|-----------------------------------------------|----------|
| 1   | UDPSOCKET_OPEN_DESTRUCT                       | MUST     |
| 2   | UDPSOCKET_OPEN_LIMIT                          | MUST     |
| 3   | UDPSOCKET_OPEN_TWICE                          | MUST     |
| 4   | UDPSOCKET_OPEN_CLOSE_REPEAT                   | MUST     |
| 5   | UDPSOCKET_BIND_PORT                           | SHOULD   |
| 6   | UDPSOCKET_BIND_PORT_FAIL                      | SHOULD   |
| 9   | UDPSOCKET_BIND_ADDRESS_INVALID                | SHOULD   |
| 10  | UDPSOCKET_BIND_WRONG_TYPE                     | SHOULD   |
| 11  | UDPSOCKET_BIND_ADDRESS                        | SHOULD   |
| 12  | UDPSOCKET_BIND_UNOPENED                       | SHOULD   |
| 13  | UDPSOCKET_SENDTO_INVALID                      | MUST     |
| 14  | UDPSOCKET_SENDTO_REPEAT                       | MUST     |
| 15  | UDPSOCKET_ECHOTEST                            | MUST     |
| 16  | UDPSOCKET_ECHOTEST_NONBLOCK                   | MUST     |
| 17  | UDPSOCKET_RECV_TIMEOUT                        | SHOULD   |
| 18  | UDPSOCKET_SENDTO_TIMEOUT                      | SHOULD   |
| 19  | TCPSOCKET_OPEN_DESTRUCT                       | MUST     |
| 20  | TCPSOCKET_OPEN_LIMIT                          | MUST     |
| 21  | TCPSOCKET_OPEN_TWICE                          | MUST     |
| 22  | TCPSOCKET_OPEN_CLOSE_REPEAT                   | MUST     |
| 23  | TCPSOCKET_BIND_PORT                           | SHOULD   |
| 24  | TCPSOCKET_BIND_PORT_FAIL                      | SHOULD   |
| 27  | TCPSOCKET_BIND_ADDRESS_INVALID                | SHOULD   |
| 28  | TCPSOCKET_BIND_WRONG_TYPE                     | SHOULD   |
| 29  | TCPSOCKET_BIND_ADDRESS                        | SHOULD   |
| 30  | TCPSOCKET_BIND_UNOPENED                       | SHOULD   |
| 31  | TCPSOCKET_CONNECT_INVALID                     | MUST     |
| 32  | TCPSOCKET_SEND_REPEAT                         | MUST     |
| 33  | TCPSOCKET_ECHOTEST                            | MUST     |
| 34  | TCPSOCKET_ECHOTEST_NONBLOCK                   | MUST     |
| 35  | TCPSOCKET_RECV_TIMEOUT                        | SHOULD   |
| 36  | TCPSOCKET_SEND_TIMEOUT                        | SHOULD   |
| 37  | TCPSOCKET_ENDPOINT_CLOSE                      | MUST     |
| 38  | UDPSOCKET_ECHOTEST_BURST                      | MUST     |
| 39  | UDPSOCKET_ECHOTEST_BURST_NONBLOCK             | MUST     |
| 40  | TCPSOCKET_ECHOTEST_BURST                      | MUST     |
| 41  | TCPSOCKET_ECHOTEST_BURST_NONBLOCK             | MUST     |
| 42  | TCPSOCKET_RECV_100K                           | MUST     |
| 43  | TCPSOCKET_RECV_100K_NONBLOCK                  | MUST     |
| 44  | TCPSOCKET_THREAD_PER_SOCKET_SAFETY            | MUST     |
| 45  | TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID          | SHOULD   |
| 46  | TLSSOCKET_OPEN_DESTRUCT                       | SHOULD   |
| 47  | TLSSOCKET_OPEN_LIMIT                          | SHOULD   |
| 48  | TLSSOCKET_OPEN_TWICE                          | SHOULD   |
| 49  | TLSSOCKET_CONNECT_INVALID                     | SHOULD   |
| 50  | TLSSOCKET_HANDSHAKE_INVALID                   | SHOULD   |
| 51  | TLSSOCKET_SEND_CLOSED                         | SHOULD   |
| 52  | TLSSOCKET_SEND_UNCONNECTED                    | SHOULD   |
| 53  | TLSSOCKET_SEND_REPEAT                         | SHOULD   |
| 54  | TLSSOCKET_SEND_TIMEOUT                        | SHOULD   |
| 55  | TLSSOCKET_ECHOTEST                            | SHOULD   |
| 56  | TLSSOCKET_ECHOTEST_NONBLOCK                   | SHOULD   |
| 57  | TLSSOCKET_ENDPOINT_CLOSE                      | SHOULD   |
| 58  | TLSSOCKET_NO_CERT                             | SHOULD   |
| 59  | TLSSOCKET_RECV_TIMEOUT                        | SHOULD   |
| 60  | TLSSOCKET_SIMULTANEOUS_TEST                   | SHOULD   |
| 61  | TLSSOCKET_ECHOTEST_BURST                      | SHOULD   |
| 62  | TLSSOCKET_ECHOTEST_BURST_NONBLOCK             | SHOULD   |
| 63  | ASYNCHRONOUS_DNS                              | MUST     |
| 64  | ASYNCHRONOUS_DNS_CACHE                        | MUST     |
| 65  | ASYNCHRONOUS_DNS_CANCEL                       | MUST     |
| 66  | ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE         | MUST     |
| 67  | ASYNCHRONOUS_DNS_INVALID_HOST                 | MUST     |
| 68  | ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC          | MUST     |
| 69  | ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE           | MUST     |
| 70  | ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT          | MUST     |
| 71  | ASYNCHRONOUS_DNS_SIMULTANEOUS                 | MUST     |
| 72  | ASYNCHRONOUS_DNS_TIMEOUTS                     | MUST     |
| 73  | SYNCHRONOUS_DNS                               | MUST     |
| 74  | SYNCHRONOUS_DNS_CACHE                         | MUST     |
| 75  | SYNCHRONOUS_DNS_INVALID_HOST                  | MUST     |
| 76  | SYNCHRONOUS_DNS_MULTIPLE                      | MUST     |
| 77  | UDPSOCKET_ECHOTEST_CONNECT_SEND_RECV          | MUST     |
| 78  | UDPSOCKET_ECHOTEST_NONBLOCK_CONNECT_SEND_RECV | MUST     |



Building test binaries
--------------------------

To test the board and driver, test against the Mbed OS master branch to get the most recent, up-to-date test cases and drivers.

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
        },
        "echo-server-discard-port" : {
            "help" : "Discard port of echo server",
            "value" : "9"
        },
        "echo-server-port-tls" : {
            "help" : "Port of echo server for TLS",
            "value" : "2007"
        },
        "echo-server-discard-port-tls" : {
            "help" : "Discard port of echo server for TLS",
            "value" : "2009"
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

Cellular tests require some more configuration, so for cellular purposes, the `mbed_app.json` might look like this:
NOTE! These are just typical settings needed for cellular. Actual configuration values depend on used target HW, SIM, modem and network capabilities.

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
            "help" : "Port of echo server for TLS",
            "value" : "2007"
        },
        "echo-server-discard-port-tls" : {
            "help" : "Discard port of echo server for TLS",
            "value" : "2009"
        }
    },
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "CELLULAR",
            "nsapi.default-cellular-sim-pin": "\"SIM PIN if needed\"",
            "nsapi.default-cellular-plmn": "\"PLMN if needed\"",
            "nsapi.default-cellular-apn": "\"APN if needed\"",
            "nsapi.default-cellular-username": "\"SIM USERNAME if needed\"",
            "nsapi.default-cellular-password": "\"SIM PASSWORD if needed\"",
            "lwip.ipv4-enabled": true,
            "lwip.ipv6-enabled": true,
            "lwip.tcp-enabled": true,
            "lwip.ppp-enabled": true,
            "lwip.ethernet-enabled": false,
            "<YOUR MODEM TARGET>.provide-default": true,
            "cellular.radio-access-technology": <see supported values from cellular mbed_lib.json>
        }
    }
}
```

Please, see `mbed-os/tools/test_configs` folder for examples for different technologies.

Now build the test binaries:

```.sh
mbed test --compile -t <toolchain> -m <target> -n mbed-os-tests-network-*,mbed-os-tests-netsocket*
```

Running tests
-------------

Run this when the device is connected to network, or if the wireless device is near the access point:

```.sh
mbed test -n mbed-os-tests-network-*,mbed-os-tests-netsocket*
```

Test cases for Socket class
---------------------------

These tests are the same as those for UDPSocket and TCPSocket. The Socket class is abstract, so it cannot be instantiated; therefore, these test cases are implemented using both TCPSocket and UDPSocket. Some of these tests are also implemented for the TLSSocket class. In such case, the certificate has to be set for the Socket before calling `open()`, unless specified otherwise in the test's
description.

### SOCKET_OPEN_DESTRUCT

**Description:**

Call `Socket::open()`, and then destruct the socket.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Delete the object.
1. Repeat 100 times.

**Expected result:**

`Socket::open()` always returns `NSAPI_ERROR_OK`.

### SOCKET_OPEN_LIMIT

**Description:**

 Call `Socket::open()` until it runs out of memory or another internal limit in the stack is reached.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Repeat until `NSAPI_ERROR_NO_MEMORY` or `NSAPI_ERROR_NO_SOCKET` error code is returned.
1. Call "delete" for all previously allocated sockets.
1. Repeat.

**Expected result:**

It can reserve at least four sockets. After freeing all sockets, it can reserve the same number of sockets.

### SOCKET_OPEN_TWICE

**Description:**

Call `Socket::open()` twice.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Call `Socket::open(stack)`.
1. Destroy the socket.

**Expected result:**

`Socket::open()` first call (socket was not opened) returns `NSAPI_ERROR_OK` and nexts calls (socet was opened by previous call Socket::open()) return `NSAPI_ERROR_PARAMETER`.

### SOCKET_OPEN_CLOSE_REPEAT

**Description:**

Call `Socket::open()` followed by `Socket::close()` and then again `Socket::open()`. Should allows you to reuse the same object.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Call `Socket::close(stack)`.
1. Call `Socket::open(stack)`.
1. Call `Socket::close(stack)`.
1. Destroy the socket.

**Expected result:**

All `Socket::open()` and `Socket::close()` calls return `NSAPI_ERROR_OK`.

### SOCKET_BIND_PORT

**Description:**

Call `Socket::bind(port)`.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Call `Socket::bind(<any unused port number>);`.
1. Destroy the socket.

**Expected result:**

All calls return `NSAPI_ERROR_OK`.

### SOCKET_BIND_PORT_FAIL

**Description:**

Call `Socket::bind(port)` on a port number that is already used.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Call `Socket::bind(<any unused port number>);`.
1. Repeat steps 1-3 for a new socket.
1. Destroy both sockets.

**Expected result:**

The second `Socket::bind()` returns `NSAPI_ERROR_PARAMETER`.

### SOCKET_BIND_ADDRESS_INVALID

**Description:**

Call `Socket::bind(address, port)` with an address not assigned to you.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Check whether the device is IPv4 or IPv6 connected.
   1. For IPv4: Call `Socket::bind("190.2.3.4", <any unused port number>);`.
   1. For IPv6: Call `Socket::bind("fe80::ff01", <any unused port number>);`.
1. Destroy the socket.

**Expected result:**

`Socket::bind()` returns `NSAPI_ERROR_PARAMETER`.

### SOCKET_BIND_ADDRESS_WRONG_TYPE

**Description:**

Call `Socket::bind(SocketAddress)` with an address that is not the wrong type for the connection.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create a object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Check whether the device is IPv4 or IPv6 connected:
   - For IPv4: Create `SocketAddress("fe80::ff01", <any unused port number>);`.
   - For IPv6: Create `SocketAddress("190.2.3.4", <any unused port number>);`.
1. Call `Socket::bind(address);`.
1. Destroy the socket.

**Expected result:**

`Socket::bind()` returns `NSAPI_ERROR_PARAMETER`.

### SOCKET_BIND_ADDRESS

**Description:**

Call `Socket::bind(SocketAddress)`.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::open(stack)`.
1. Get address by calling `NetworkInterface::get_ip_address();`.
1. Create a SocketAddress object using this address and any unused port number.
1. Call `Socket::bind(address);`.
1. Destroy the socket.

**Expected result:**

All calls return `NSAPI_ERROR_OK`.

### SOCKET_BIND_UNOPENED

**Description:**

Call `Socket::bind()` on a socket that has not been opened.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create an object by calling `new Socket()`.
1. Call `Socket::bind(<any unused port number>);`.
1. Destroy the socket.

**Expected result:**

`NSAPI_ERROR_NO_SOCKET`

Test cases for UDPSocket class
------------------------------

### UDPSOCKET_OPEN_DESTRUCT

**Description:** Run `SOCKET_OPEN_DESTRUCT` for UDPSocket.

### UDPSOCKET_OPEN_LIMIT

**Description:** Run `SOCKET_OPEN_LIMIT` for UDPSocket.

### UDPSOCKET_OPEN_TWICE

**Description:** Run `SOCKET_OPEN_TWICE` for UDPSocket.

### UDPSOCKET_OPEN_CLOSE_REPEAT

**Description:** Run `SOCKET_OPEN_CLOSE_REPEAT` for UDPSocket.

### UDPSOCKET_BIND_PORT

**Description:** Run `SOCKET_BIND_PORT` for UDPSocket.

### UDPSOCKET_BIND_PORT_FAIL

**Description:** Run `SOCKET_BIND_PORT_FAIL` for UDPSocket.

### UDPSOCKET_BIND_ADDRESS_INVALID

**Description:** Run `SOCKET_BIND_ADDRESS_INVALID` for UDPSocket.

### UDPSOCKET_BIND_WRONG_TYPE

**Description:** Run `SOCKET_BIND_WRONG_TYPE` for UDPSocket.

### UDPSOCKET_BIND_ADDRESS

**Description:** Run `SOCKET_BIND_ADDRESS` for UDPSocket.

### UDPSOCKET_BIND_UNOPENED

**Description:** Run `SOCKET_BIND_UNOPENED` for UDPSocket.

### UDPSOCKET_SENDTO_REPEAT

**Description:**

Repeatedly send small packets.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. UDPSocket is open.

**Test steps:**

1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 9, "hello", 5);`.
1. Repeat 100 times.
1. Fail if `NSAPI_ERROR_NO_MEMORY` is returned two times in a row.
1. Wait 1 second before retrying.
1. Destroy the socket.

**Expected result:**

All `sendto()` calls return 5.

### UDPSOCKET_ECHOTEST

**Description:**

Repeatedly send packets to echo server and read incoming packets back. Verify different packet sizes work.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. UDPSocket is open.

**Test steps:**

1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = loop index>, <loop index>);`.
1. Wait for incoming packet.
   - If timeout happens, retry sending and receiving a maximum of three times.
1. Verify incoming content is the same as the sent content.
1. Repeat 1,200 times.
1. Destroy the socket.

**Expected result:**

- At least one `sendto()` call of every size returns the packet size.
- Errors returned from `recvfrom()` calls are tolerated. 
- Calculate packet loss rate. The maximum tolerated packet loss rate is 30%.

### UDPSOCKET_ECHOTEST_NONBLOCK

**Description:**

Repeatedly send packets to echo server and read incoming packets back. Verify different packet sizes work. Use Socket in nonblocking
mode.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. UDPSocket is open.

**Test steps:**

1. Call `UDPSocket::set_blocking(false)`.
1. Register event handler with `UDPSocket::sigio()`.
1. Create another thread that constantly waits for a signal from the `sigio()` handler.
1. When it's received, try `UDPSocket::recvfrom()`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = loop index>, <loop index>);`.
1. Wait for an incoming packet for one second.
   - If nothing is received, retry a maximum of three times.
1. Verify incoming content is the same as sent content.
1. Repeat 1,200 times.
1. Destroy the socket.

**Expected result:**

- At least one `sendto()` call of every size returns the packet size.
- Errors returned from `recvfrom()` calls are tolerated. 
- Calculate packet loss rate. The maximum tolerated packet loss rate is 30%.

### UDPSOCKET_ECHOTEST_CONNECT_SEND_RECV

**Description:**

This test is exactly the same test as UDPSOCKET_ECHOTEST, except instead of using `sendto()` and `recvfrom()` methods, this uses TCPSocket-like `connect()`, `send()` and `recv()` methods.

### UDPSOCKET_ECHOTEST_NONBLOCK_CONNECT_SEND_RECV

**Description:**

This test is exactly the same test as UDPSOCKET_ECHOTEST_NONBLOCK_CONNECT_SEND_RECV, except instead of using `sendto()` and `recvfrom()` methods, this uses TCPSocket-like `connect()`, `send()` and `recv()` methods.

### UDPSOCKET_RECV_TIMEOUT

**Description:**

Test whether timeouts are obeyed in UDPSockets.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Call `UDPSocket::set_timeout(100)`.
1. Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
1. Repeat five times.
   1. Record a time with millisecond precision.
   1. Call `UDPSocket::recvfrom()`.
   1. Record a time with millisecond precision.
1. Repeat the test case 10 times.

**Expected result:**

Each `sendto()` call returns 100.

Within each loop, one `recvfrom()` may return the received packet size (100). Other calls return `NSAPI_ERROR_WOULD_BLOCK`.

When `NSAPI_ERROR_WOULD_BLOCK` is received, check that time consumed is more that 100 milliseconds but less than 200 milliseconds.

### UDPSOCKET_SENDTO_TIMEOUT

**Description:**

Test whether timeouts are obeyed in UDPSockets.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Record time.
1. Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 9, <random packet, size = 100>, 100);`.
1. Record time.
1. Call `UDPSocket::set_timeout(1000)`.
1. Call `UDPSocket::sendto("echo.mbedcloudtesting.com", 9, <random packet, size = 100>, 100);`.
1. Record time.

**Expected result:**

Each `sendto()` call returns 100.

All `sendto()` calls return faster than 100 milliseconds because UDP sending does not block that long.

Test cases for TCPSocket class
------------------------------

### TCPSOCKET_OPEN_DESTRUCT

**Description:** Run `SOCKET_OPEN_DESTRUCT` for TCPSocket.

### TCPSOCKET_OPEN_LIMIT

**Description:** Run `SOCKET_OPEN_LIMIT` for TCPSocket.

### TCPSOCKET_OPEN_TWICE

**Description:** Run `SOCKET_OPEN_TWICE` for TCPSocket.

### TCPSOCKET_OPEN_CLOSE_REPEAT

**Description:** Run `SOCKET_OPEN_CLOSE_REPEAT` for TCPSocket.

### TCPSOCKET_BIND_PORT

**Description:** Run `SOCKET_BIND_PORT` for TCPSocket.

### TCPSOCKET_BIND_PORT_FAIL

**Description:** Run `SOCKET_BIND_PORT_FAIL` for TCPSocket.

### TCPSOCKET_BIND_ADDRESS_INVALID

**Description:** Run `SOCKET_BIND_ADDRESS_INVALID` for TCPSocket.

### TCPSOCKET_BIND_WRONG_TYPE

**Description:** Run `SOCKET_BIND_WRONG_TYPE` for TCPSocket.

### TCPSOCKET_BIND_ADDRESS

**Description:** Run `SOCKET_BIND_ADDRESS` for TCPSocket.

### TCPSOCKET_BIND_UNOPENED

**Description:** Run `SOCKET_BIND_UNOPENED` for TCPSocket.

### TCPSOCKET_CONNECT_INVALID

**Description:**

Call `TCPSocket::connect()` with invalid parameters.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket:connect( NULL, 9);`.
1. Call `TCPSocket:connect( "", 9);`.
1. Call `TCPSocket:connect( "", 0);`.
1. Call `TCPSocket:connect( "echo.mbedcloudtesting.com", 9);`.
1. Destroy the socket.

**Expected result:**

All `connect()` calls return an error code except the number 4, which returns `NSAPI_ERROR_OK`.

### TCPSOCKET_SEND_REPEAT

**Description:**

Repeatedly send small packets.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 9);`.
1. Call `TCPSocket::send("hello", 5);`.
1. Repeat 100 times.
1. Destroy the socket.

**Expected result:**

`TCPSocket::connect()` returns `NSAPI_ERROR_OK`.

All `send()` calls return 5.

### TCPSOCKET_ECHOTEST

**Description:**

Repeatedly send packets to echo server and read incoming packets back. Verify different packet sizes work.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
1. Call `TCPSocket::send(<random packet, size = loop index>, <size>);`.
   1. If less than <loop index> was returned, size = sent bytes.
1. Call `TCPSocket::recv(buffer, <size>);`.
1. Verify incoming content is the same as sent content.
1. Repeat 1,200 times.
1. Destroy the socket.

**Expected result:**

All `send()` calls return the packet size or less. All `recv()` calls return the same sized packet that was sent with same content.

NOTE: This is stream, so `recv()` might return less data than requested. Keep calling `recv()` until all data you have sent is returned.

### TCPSOCKET_ECHOTEST_NONBLOCK

**Description:**

Repeatedly send packets to echo server and read incoming packets back. Verify different packet sizes work. Use Socket in nonblocking
mode.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
1. Call `TCPSocket::set_blocking(false)`.
1. Register event handler with `TCPSocket::sigio()`.
1. Create another thread that constantly waits for a signal from `sigio()` handler.
1. When it's received, try `TCPSocket::recv(buf+index, <loop index> - index)`, where index is the amount of data already received.
1. Call `TCPSocket:send(<random packet, size = loop index>, <loop index>);`.
   1. If less than <loop index> is returned, try immediately sending the remaining bytes.
   1. If `NSAPI_ERROR_WOULD_BLOCK` is returned, wait for `sigio()` call to happen.
1. Wait for incoming packet for one second.
1. Verify incoming content is the same as sent content.
1. Set index for receiving thread to zero.
1. Repeat 1,200 times.
1. Destroy the socket.

**Expected result:**

All `send()` calls return the packet size or less. All `recv()` calls return `NSAPI_ERROR_WOULD_BLOCK` or a packet size equal to or
less than what has been sent.

### TCPSOCKET_RECV_TIMEOUT

**Description:**

Test whether timeouts are obeyed in TCPSockets.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
1. Call `TCPSocket::set_timeout(100);`.
1. Call `TCPSocket::send(<random packet, size = 100>;, 100);`.
1. Repeat 5 times.
   1. Record a time with millisecond precision.
   1. Call `TCPSocket::recv()`.
   1. Record a time with millisecond precision.
1. Repeat the test case 10 times.

**Expected result:**

Each `send()` call returns 100.

Within each loop, one `recv()` may return the received packet size (100). Other calls return `NSAPI_ERROR_WOULD_BLOCK`.

When `NSAPI_ERROR_WOULD_BLOCK` is received, check the time consumed is more that 100 milliseconds but less than 200 milliseconds.

### TCPSOCKET_SEND_TIMEOUT

**Description:**

Repeatedly send small packets in a given time limit.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket:connect("echo.mbedcloudtesting.com", 9);`.
1. Call `TCPSocket:set_blocking(false);`.
1. Call `TCPSocket:send("hello", 5);`.
1. Repeat 10 times.
1. Destroy the socket.

**Expected result:**

`TCPSocket::connect()` return `NSAPI_ERROR_OK`.

All `send()` calls return in less than 800 milliseconds.

### TCPSOCKET_ENDPOINT_CLOSE

**Description:**

Test whether you tolerate an endpoint closing the connection.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 13);`.
1. Call `TCPSocket::recv(<buffer>, 30);`.
1. Repeat until `recv()` returns 0.
1. Call `TCPSocket::close();`.
1. Destroy the socket.

**Expected result:**

Connect returns `NSAPI_ERROR_OK`.

The first `recv()` returns datetime string length (It is between 10 and 30 bytes).

The second `recv()` returns zero because the endpoint closed the connection. `close()` returns `NSAPI_ERROR_OK`.

### TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID

**Description:**

Test you can request setting valid TCP keepalive values.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Call `TCPSocket::setsockopt(keepalive, [0,1 or 7200]);`.
1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 9);`.
1. Call `TCPSocket::getsockopt(keepalive);`.

**Postconditions:**

1. Call `TCPSocket::close();`.
1. Destroy the socket.

**Expected result:**

`TCPSocket::getsockopt(keepalive)` returns the same value as that set with `TCPSocket::setsockopt()` or `NSAPI_ERROR_UNSUPPORTED`.

Test cases for TLSSocket class
------------------------------

### TLSSOCKET_OPEN_DESTRUCT

**Description:** Run `SOCKET_OPEN_DESTRUCT` for TLSSocket.

### TLSSOCKET_OPEN_LIMIT

**Description:** Run `SOCKET_OPEN_LIMIT` for TLSSocket.

### TLSSOCKET_OPEN_TWICE

**Description:** Run `SOCKET_OPEN_TWICE` for TLSSocket.

### TLSSOCKET_CONNECT_INVALID

**Description:** Run `SOCKET_CONNECT_INVALID` for TLSSocket.

### TLSSOCKET_HANDSHAKE_INVALID

**Description:**

Execute the TLS handshake by calling `TLSSocket::connect()`. The server must not match the certificate used by os.mbed.com.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create TLSSocket.
1. Call `TLSSocket::open()`.
1. Call `TLSSocket::connect("os.mbed.com", 2009)`.
1. Call `TLSSocket::close()`.

**Expected result:**

`TLSSocket::connect` must return an error.

### TLSSOCKET_SEND_CLOSED

**Description:**

Make an HTTP request to a closed socket.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create TLSSocket.
1. Call `TLSSocket::open()`.
1. Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007)`.
1. Call `TLSSocket::close()`.
1. Call `TLSSocket::send("12345", 5)`.

**Expected result:**

`TLSSocket::send` must return an error.

### TLSSOCKET_SEND_REPEAT

**Description:** Run `SOCKET_SEND_REPEAT` for TLSSOCKET by using port number 2009.

### TLSSOCKET_SEND_TIMEOUT

**Description:** Run `SOCKET_SEND_TIMEOUT` for TLSSOCKET by using port number 2009.

### TLSSOCKET_SEND_UNCONNECTED

**Description:** 

Make an HTTP request to an unconnected socket.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create TLSSocket.
1. Call `TLSSocket::open()`.
1. Call `TLSSocket::send("12345", 5)`.
1. Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007)`.

**Expected result:**

`TLSSocket::send` must return an error.

### TLSSOCKET_ECHOTEST

**Description:** Run `SOCKET_ECHOTEST` for TLSSOCKET by using port number 2007.

### TLSSOCKET_ECHOTEST_NONBLOCK

**Description:** Run `SOCKET_ECHOTEST_NONBLOCK` for TLSSOCKET by using port number 2007.

### TLSSOCKET_ENDPOINT_CLOSE

**Description:** Run `SOCKET_ENDPOINT_CLOSE` for TLSSOCKET by using port number 2013.

### TLSSOCKET_NO_CERT

**Description:**

Verify TLS Socket fails to connect without a certificate.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.

**Test steps:**

1. Create TLSSocket, without adding a default certificate.
1. Call `TLSSocket::open()`.
1. Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2009)`.

**Expected result:**

`TLSSocket::connect` must return an error if the certificate is not present.

### TLSSOCKET_RECV_TIMEOUT

**Description:**

Run `TCPSOCKET_RECV_TIMEOUT` for TLSSOCKET by using port number 2007.

### TLSSOCKET_SIMULTANEOUS_TEST

**Description:**

Simultaneously send packets to echo server on two opened sockets and read incoming packets back. Verify TLS sockets open and operate simultaneously.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TLSSockets are open and one additional thread has been created.
1. Both threads get their own socket instance.

**Test steps:**

1. (Simultaneously with step 2), on the main thread:
   1. Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007);`.
   1. Call `TLSSocket::send(<random packet, size = loop index>, <loop index>);`.
      - If less than <loop index> is returned, size = sent bytes.
   1. Call `TLSSocket::recv(buffer, <size>);`.
   1. Verify incoming content is the same as sent content.
   1. Repeat 100 times.
1. (Simultaneously with step 1), on the additional thread:
   1. Call `TLSSocket::connect("echo.mbedcloudtesting.com", 2007);`.
   1. Call `TLSSocket::send(<random packet, size = loop index>, <loop index>);'.
      - If less than <loop index> is returned, size = sent bytes.
   1. Call `TLSSocket::recv(buffer, <size>);`.
   1. Verify incoming content is the same as sent content.
   1. Repeat 100 times.
1. Wait for the additional thread to end.
1. Close and destroy the sockets.

**Expected result:**

All `send()` calls return the packet size or less. All `recv()` calls on the main thread return the same sized packet that was sent with the same content. All `recv()` calls on the additional thread return the valid daytime string.

Performance tests
-----------------

### UDPSOCKET_ECHOTEST_BURST

**Description:**

Send a burst of packets to the echo server and read incoming packets back.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. UDPSocket is open.

**Test steps:**

1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 200>, 200);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 300>, 300);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 120>, 120);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 500>, 500);`.
1. Wait for incoming packets for five seconds.
1. Verify incoming content is the same as sent content.
1. Allow packet reordering.
1. Repeat 100 times.
1. Destroy the socket.

**Expected result:**

All `sendto()` calls return the packet size.

All `recvfrom()` calls return the same sized packet that was sent with same content. Allow packet reordering.

Calculate packet loss rate. The maximum tolerated packet loss rate is 30%.

The number of succesful rounds is higher than 70.

### UDPSOCKET_ECHOTEST_BURST_NONBLOCK

**Description:**

Send a burst of packets to the echo server and read incoming packets back. Use Socket in nonblocking mode.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. UDPSocket is open.

**Test steps:**

1. Call `UDPSocket::set_blocking(false)`.
1. Register event handler with `UDPSocket::sigio()`.
1. Create another thread that constantly waits signal from sigio() handler.
1. When received, try `UDPSocket::recvfrom()`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 100>, 100);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 200>, 200);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 300>, 300);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 120>, 120);`.
1. Call `UDPSocket:sendto("echo.mbedcloudtesting.com", 7, <random packet, size = 500>, 500);`.
1. Wait for incoming packets for five seconds.
1. Verify incoming content is the same as sent content.
1. Allow packet reordering.
1. Repeat 100 times.
1. Destroy the socket.

**Expected result:**

All `sendto()` calls return the packet size.

All `recvfrom()` calls return the same sized packet that was sent with the same content. Allow packet reordering.

Calculate packet loss rate. The maximum tolerated packet loss rate is 30%.

The number of succesful rounds is higher than 70.

### TCPSOCKET_ECHOTEST_BURST

**Description:**

Send a burst of packets to the echo server and read incoming packets back.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
1. Call `TCPSocket::send(<random packet, size = 100>, 100);`.
1. Call `TCPSocket::send(<random packet, size = 200>, 200);`.
1. Call `TCPSocket::send(<random packet, size = 300>, 300);`.
1. Call `TCPSocket::send(<random packet, size = 120>, 120);`.
1. Call `TCPSocket::send(<random packet, size = 500>, 500);`.
1. Call `TCPSocket::recv(buf, 1220)`.
1. Verify incoming content is the same as sent content.
1. Repeat 100 times.
1. Destroy the socket.

**Expected result:**

All `send()` calls return the packet size.

NOTE: This is stream, so `recv()` might return less data than requested. Keep calling `recv()` with the remaining size until all data you have sent is returned.

Consecutive calls to `recv()` return all the data that has been sent. The total amount of returned data must match 1220.

### TCPSOCKET_ECHOTEST_BURST_NONBLOCK

**Description:**

Send a burst of packets to the echo server and read incoming packets back. Use Socket in nonblocking mode.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Register event handler with `TCPSocket::sigio()`.
1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7);`.
1. Call `TCPSocket::set_blocking(false)`.
1. Create another thread that constantly waits for a signal from `sigio()` handler.
1. When it's received, try `TCPSocket::recv()`.
1. For randomly generated packets sized 100, 200, 300, 120 and 500:
   1. Call `TCPSocket::send(packet, size);`.
   1. If less than size is sent, repeat with remaining.
   1. If `NSAPI_ERROR_WOULD_BLOCK` returned, wait for next `sigio()`.
1. Wait for incoming packets for five seconds.
1. Verify incoming content is the same as sent content.
1. Allow `recv()` to return smaller pieces.
1. Repeat 100 times.
1. Destroy the socket.

**Expected result:**

All `send()` calls return `NSAPI_ERROR_WOULD_BLOCK` or a size less than or equal to what has been requested.

All `recv()` calls return a value less than or equal to what has been sent. With consecutive calls, the sizes should match.

When `recv()` returns `NSAPI_ERROR_WOULD_BLOCK`, wait for the next `sigio()` event. No other error codes are allowed.

### TCPSOCKET_RECV_100K

**Description:**

Download 100kB of data.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 19);`.
1. Call `TCPSocket::recv(buffer, 100);`.
1. Verify input according to a known pattern.
1. Loop until 100kB of data received.
1. Close socket.

**Expected result:**

Each `recv()` call returns 100 bytes of data or fewer. No errors are returned.

Measure time taken for receiving and report speed.

### TCPSOCKET_RECV_100K_NONBLOCK

**Description:**

Download 100kB of data.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. TCPSocket is open.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 19);`.
1. Call `TCPSocket::set_blocking(false)`.
1. Create another thread that constantly waits for a signal from `sigio()` handler.
1. When it's received, try `TCPSocket::recv()`.
   1. Call `TCPSocket::recv(buffer, 100);`.
   1. Verify input according to a known pattern.
1. Wait until 100kB of data received.
1. Close socket.

**Expected result:**

Each `recv()` call returns 100 bytes of data or less or `NSAPI_ERROR_WOULD_BLOCK`, in which case thread waits for another `sigio()`. No errors are returned.

Measure time taken for receiving and report speed.

### TCPSOCKET_THREAD_PER_SOCKET_SAFETY

**Description:**

Run two threads that both exercise the underlying stack and driver through a dedicated socket.

**Preconditions:**

1. Network interface and stack are initialized.
1. Network connection is up.
1. Two TCPSockets are open, and one additional thread has been created.
1. Both threads get their own socket instance.

**Test steps:**

1. Call `TCPSocket::connect("echo.mbedcloudtesting.com", 7)` in both threads - in the main thread executing the test case and on the additional one.
   - (Simultaneously with the next step) on the main thread:
      1. For randomly generated packets, sized 1001, 901, 801,...,101,1 do
         1. Call `TCPSocket::send(packet, size);`.
         1. Verify incoming content was the same that was sent.
         1. Allow `recv()` to return smaller pieces.
   - (Simultaneously with the earlier step) on the additional thread:
      1. For randomly generated packets, sized 10:
         1. Call `TCPSocket::send(packet, size);`.
         1. Verify incoming content is the same as sent content.
         1. Allow `recv()` to return smaller pieces.
         1. Stop the thread if inconsistencies are found, and report it to main thread.
1. Kill the additional thread.
1. Close and destroy the sockets.

**Expected result:**

The echo server returns data to both threads and received data matches to send data. The additional thread isn't stopped prematurely.

Test cases for DNS class
---------------------------

### ASYNCHRONOUS_DNS

**Description:**

Verify the basic functionality of asynchronous DNS. Call `NetworkInterface::gethostbyname_async()` with a valid host name, and verify the result.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` with a valid host name and a callback.
1. Verify the callback is called with correct parameters.

**Expected result:**

Callback is called with `NSAPI_ERROR_OK` and IP address.

### ASYNCHRONOUS_DNS_SIMULTANEOUS

**Description:**

Verify simultaneous asynchronous DNS queries work correctly. Call `NetworkInterface::gethostbyname_async()` 6 times with different host names. Wait for all requests to complete, and verify the result. The cache does not contain host names used in asynchronous request.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` 6 times with different host names. Host names are not found from the cache.
1. Verify the last `gethostbyname_async()` operation is rejected because there is room only for five simultaneous operations.
1. Verify the callback is called with correct parameters five times.

**Expected result:**

The sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_OK` and IP address five times.

### ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE

**Description:**

Verify the caching of DNS results works correctly with simultaneous asynchronous DNS queries. Call `NetworkInterface::gethostbyname_async()` six times with different host names. Wait for all requests to complete, and verify the result. The cache contains at least one host name used in asynchronous request. You can achieve this, for example, by running the "Asynchronous DNS simultaneous" test before this test and using the same host names in this run.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` six times with different host names. At least one host name is found from cache.
1. Verify that callback is called with correct parameters six times.

**Expected result:**

Callback is called with `NSAPI_ERROR_OK` and IP address six times.

### ASYNCHRONOUS_DNS_CACHE

**Description:**

Verify the caching of DNS results works correctly. Call `NetworkInterface::gethostbyname_async()` five times with the same host name and verify the result after each request. For the first request, the cache does not contain the host name. Verify the first request completes more slowly than the requests made after it (where the response is found from cache).

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` with a host name. For the first request, the host name is not found from cache.
1. Verify the callback is called with correct parameters.
1. Repeat the sequence four times using the same host name.
1. For each request, calculate how long it takes for the DNS query to complete.

**Expected result:**

The callback is called with `NSAPI_ERROR_OK` and IP address five times. The first request is complete before the requests made after it (where the response is found from cache).

### ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC

**Description:**

Verify synchronous (in other words, blocking) DNS queries and asynchronous (in other words, nonblocking) queries work at the same time. Call `NetworkInterface::gethostbyname_async()`. Right after that, make six synchronous `NetworkInterface::gethostbyname()` calls with different host names.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` with a host name. The host name is not found from cache.
1. Call `gethostbyname()` six times with different host names (none of the names are the same as that in step 1).
1. Verify each `gethostbyname()` returns success.
1. Verify the asynchronous callback is called with correct parameters.

**Expected result:**

All operations return `NSAPI_ERROR_OK` and IP address.

### ASYNCHRONOUS_DNS_CANCEL

**Description:**

Verify the asynchronous DNS query cancel works correctly. Call `NetworkInterface::gethostbyname_async()` six times with different host names. The cache contains three host names used in requests. You can achieve this, for example, by running the "Asynchronous DNS synchronous and asynchronous" test before this test and using the same host names in this run. For each request that was given an unique ID, call cancel. Verify the callback is not called for canceled requests.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` six times with different host names. The cache contains a maximum of three host names used in requests.
1. Call `gethostbyname_async_cancel()` for each request that was given an unique ID.
1. Verify that for canceled requests, callback is not called.
1. Verify that for other requests, callback is called.

**Expected result:**

Callback is called only for requests that were not canceled.

### ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE

**Description:**

Verify that providing an external event queue works correctly. Define a thread and an event queue running on it. Define a DNS call in callback function that uses the event queue (call_in_callback_cb_t). Enable external event queue. Call `NetworkInterface::gethostbyname_async()` six times with different host names.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Define a thread and an event queue running on it.
1. Define a DNS call in a callback function that uses the event queue (call_in_callback_cb_t).
1. Start thread and event queue.
1. Set DNS callback function using the `nsapi_dns_call_in_set()` call.
1. Call `gethostbyname_async()` six times with different host names. The host names are not found from cache.
1. Verify the last `gethostbyname_async()` operation is rejected because there is room only for five simultaneous operations.
1. Verify the callback is called with correct parameters five times.
1. Restore the default DNS callback function using the `nsapi_dns_call_in_set()` call.

**Expected result:**

The sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_OK` and IP address five times.

### ASYNCHRONOUS_DNS_INVALID_HOST

**Description:**

Verify the DNS failure error is provided for invalid hosts. Call `NetworkInterface::gethostbyname_async()` six times with different host names. The first, third and fifth host names are invalid.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` six times with different host names. Host names are not found from cache. The first, third and fifth host names are invalid.
1. Verify the last `gethostbyname_async()` operation is rejected because there is room only for five simultaneous operations.
1. Verify the callback is called with correct parameters five times.

**Expected result:**

The sixth `gethostbyname_async()` is rejected. Callback is called with `NSAPI_ERROR_DNS_FAILURE` for the first, third and fifth host names. Callback is called with `NSAPI_ERROR_OK` and IP address for the second and fourth host names.

### ASYNCHRONOUS_DNS_TIMEOUTS

**Description:**

Test DNS timeouts using an external event queue that is modified to time out the events faster than the standard event queue. In this test, the event queue does not delay events; instead, it handles those immediately. Call `NetworkInterface::gethostbyname_async()` six times with different host names. All or some of the request time out, and the timeout return value is returned.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Define a thread and an event queue running on it.
1. Define a DNS call in callback function that uses the event queue (call_in_callback_cb_t). The callback function does not delay callbacks; instead it handles those immediately.
1. Start thread and event queue.
1. Set DNS callback function using the `nsapi_dns_call_in_set()` call.
1. Call `gethostbyname_async()` six times with a different host names. Host names are not found from cache.
1. Verify the last `gethostbyname_async()` operation is rejected because there is room only for five simultaneous operations.
1. Verify the callback is called with correct parameters five times.

**Expected result:**

The sixth `gethostbyname_async()` is rejected. At least for one operation, the callback is called with the `NSAPI_ERROR_TIMEOUT` value.

### ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT

**Description:**

Verify the simultaneous asynchronous DNS queries work correctly when repeated in sequence. Call `NetworkInterface::gethostbyname_async()` five times with different host names. Wait for all requests to complete, and verify the result. Repeat the procedure 100 times.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname_async()` five times with different host names, providing a callback to be called when the operation completes.
1. Verify the callback is called with correct parameters five times for the first operation.
1. Repeat steps 1-2 100 times.

**Expected result:**

A callback, registered for `gethostbyname_async()`, is called with `NSAPI_ERROR_OK` and an IP address five times for every one of 100 repetitions of the test.

### SYNCHRONOUS_DNS

**Description:**

Verify the basic functionality of synchronous DNS. Call `NetworkInterface::gethostbyname()` with a valid host name, and verify the result.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname()` with a valid host name.
1. Verify the address was resolved and the return value was valid.

**Expected result:**

Return value is `NSAPI_ERROR_OK` and IP address is obtained from the function call.

### SYNCHRONOUS_DNS_MULTIPLE

**Description:**

Verify the basic functionality of synchronous DNS. Call `NetworkInterface::gethostbyname()` with a list of six host names, and verify the result.

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname()` with a list of six host names.
1. Verify each of the addresses was resolved and the return value was valid.

**Expected result:**

Return value is `NSAPI_ERROR_OK` and IP addresses are obtained from the function call.

### SYNCHRONOUS_DNS_CACHE

**Description:**

Verify the caching of DNS results works correctly. Call `NetworkInterface::gethostbyname()` five times with the same host name, and verify the result after each request. For the first request, the cache does not contain the host name. Verify the first request completes before the requests made after it (where the response is found from cache).

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname()` with a host name. For the first request, the host name is not found in cache, and an error is returned.
1. Verify the address was resolved and the return value was valid.
1. Repeat the sequence four times using the same host name.
1. For each request, calculate how long it takes the DNS query to complete.

**Expected result:**

The return value is `NSAPI_ERROR_OK`, and the IP address is obtained from the function call five times. The first request completes before the requests made after it (where the response is found from cache).

### SYNCHRONOUS_DNS_INVALID_HOST

**Description:**

Verify the DNS failure error is provided for invalid hosts. Call `NetworkInterface::gethostbyname()` six times with different host names. The first, third and fifth host names are invalid (for example by adding an incorrect suffix, such as "google.com_invalid").

**Preconditions:**

1. Network interface is initialized.
1. Network connection is up.

**Test steps:**

1. Call `gethostbyname()` six times with different host names. Host names are not found from cache. The first, third and fifth host names are invalid.
1. Verify the return value was valid and for valid hostnames the address was resolved six times.

**Expected result:**

The return value is `NSAPI_ERROR_DNS_FAILURE` for the first, third and fifth host names, which were invalidated at the beginning of the test. The return value is `NSAPI_ERROR_OK`, and the IP address is obtained for the second and fourth host names, which were valid.

Subset for driver test
----------------------

### For physical layer driver (emac, PPP):

-   TCPSOCKET_ECHOTEST.
-   TCPSOCKET_ECHOTEST_BURST.
-   TCPSOCKET_ECHOTEST_BURST_NONBLOCK.
-   TCPSOCKET_ECHOTEST_NONBLOCK.
-   TCPSOCKET_RECV_100K.
-   TCPSOCKET_RECV_100K_NONBLOCK.
-   TCPSOCKET_RECV_TIMEOUT.
-   TCPSOCKET_SEND_REPEAT.
-   UDPSOCKET_BIND_SENDTO.
-   UDPSOCKET_ECHOTEST.
-   UDPSOCKET_ECHOTEST_NONBLOCK.
-   UDPSOCKET_RECV_TIMEOUT.
-   UDPSOCKET_SENDTO_INVALID.
-   UDPSOCKET_SENDTO_REPEAT.
-   UDPSOCKET_SENDTO_TIMEOUT.

### For socket layer driver (AT-driven, external IP stack):

All Socket, UDPSocket, TCPSocket and TLSSocket test cases.
