# Introduction

This document describes how to run EMAC tests. The EMAC test cases are made using the Ethernet Configuration Testing Protocol (CTP). To run the tests, you need to configure one device in the Ethernet segment to be a CTP echo server. The devices running the test cases use the echo server to forward the CTP Ethernet frames back.

## Configuring the CTP echo server

To configure a device to be a CTP echo server, you need to enable the `echo-server` setting in the `json` file of the test environment application. When you configure a device to be a CTP echo server, it starts to forward CTP messages automatically when it is on and continues to do so until you switch it off.

## Other configuration options

Targets with connectivity set the `target.network-default-interface-type` configuration variable appropriately, either to their only interface or their most commonly used one. For targets that provide more than one type of connectivity, you may choose the default by overriding the `target.network-default-interface-type` configuration variable.

For Ethernet, if you want to overrride the default, set the `json` configuration to:

```
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "ETHERNET",
            "nsapi.default-stack": "TEST"
        }
    }
```

For Wi-Fi, set the `json` configuration to:

```
    "target_overrides": {
        "*": {
            "target.network-default-interface-type": "WIFI",
            "nsapi.default-wifi-ssid": "\"WIFI_SSID\"",
            "nsapi.default-wifi-password": "\"WIFI_PASSWORD\"",
            "nsapi.default-wifi-security": "WIFI_SECURITY",
            "nsapi.default-stack": "TEST"
        }
    }
```

For Wi-Fi, you also need to configure Wi-Fi SSID and security options to the configuration file. 


Test case priorities
--------------------

Please refer to the following table for priorities of test cases. Priorities are labeled as MUST and SHOULD. MUST means this is a requirement and therefore mandatory to pass the test. SHOULD means it is recommended to pass the test if the driver implements the feature in question.

|     | Test case                               | Priority |
|-----|-----------------------------------------|----------|
| 1   | EMAC initialize                         | MUST     |
| 2   | EMAC broadcast                          | MUST     |
| 3   | EMAC unicast                            | MUST     |
| 4   | EMAC unicast frame length               | MUST     |
| 5   | EMAC unicast burst                      | MUST     |
| 6   | EMAC unicast long                       | MUST     |
| 7   | EMAC multicast filter                   | MUST     |
| 8   | EMAC memory                             | MUST     |

## Example commands

### CTP echo server

You can use the following command to a build CTP echo server:

`mbed test --compile -m TARGET -t GCC_ARM -v -n tests-network-emac --app-config TESTS/network/emac/template_mbed_app_echo_server.txt`

Replace TARGET with the target device. After building, flash the binary to the CTP echo server device.

You can verify that the CTP echo server has started properly by making a terminal connection to the device, resetting it and verifying that `echo server started successfully` prints on the terminal. You can run host tests when the CTP echo server is running on the Ethernet segment.

For Wi-Fi tests, the CTP echo server can also run on the ethernet side as long as the network configuration is such that Ethernet frames are routed between Wi-Fi and Ethernet.

The CTP echo server sends a 100-byte long broadcast CTP Ethernet frame every 60 seconds to inform the network of its presence.

### Running tests

You can use the following command to run tests:

`mbed test --compile --run -m TARGET -t GCC_ARM -v -n tests-network-emac --app-config TESTS/network/emac/template_mbed_app.txt`

Replace TARGET with the target device.

## Traces

Test cases have different trace levels based on how much tracing can be done without affecting the performance of the test case. Configure tracing using `SET\_TRACE\_LEVEL` macro.

For example, the `EMAC broadcast` test enables send, input CTP frame, success and failure traces:

`SET_TRACE_LEVEL(TRACE_SEND | TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);`

This example trace is about a message sent to broadcast address `ff:ff:ff:ff:ff:ff` that an echo server answers:

```

message sent ff:ff:ff:ff:ff:ff

response: receipt number 24 LENGTH OK DATA OK BROADCAST

```

This example trace is about a message sent to broadcast address `ff:ff:ff:ff:ff:ff` that an echo server does not answer:

```

message sent ff:ff:ff:ff:ff:ff

NO RESPONSE: receipt number 25

```

This example is about input message trace. Message hex dump contains the first 32 bytes of the received Ethernet frame:

```

INP> LEN 100

INP> 000000 ff ff ff ff ff ff ba 42 ed 79 11 8a 90 00

INP> 00000e 00 00 02 00 ba 42 ed 79 11 8a 01 00 01 00

```

To verify whether the echo server is receiving CTP Ethernet frames, enable `echo-server-trace` option in the echo server configuration file. You can use this for debugging purposes. Do not run performance tests such as `EMAC unicast burst` with tracing enabled because tracing affects the echo server performance.

## Test cases

### EMAC initialize

**Description:**

The test case initializes and connects the EMAC driver and the test network stack.

**Precondition:**

The device is ready to be connected. (The ethernet cable is plugged in, or a Wi-Fi base station is available).

**Test steps:**

1. Constructs the network interface.
2. Connects the network interface.

**Expected result:**

The network interface is connected.

### EMAC broadcast

**Description:**

The test case tests basic broadcast functionality and resolves CTP echo server MAC address.

**Precondition:**

The network interface is connected.

**Test steps:**

1.  Sends three CTP broadcast messages (100 bytes each).
2.  Waits for three seconds.
3.  Sends three CTP broadcast messages (60 bytes each).
4.  Listens for the CTP echo server responses.
5.  Stores the addresses of the echo servers if replies are received.

**Expected result:**

The echo server replies to broadcast messages. For each sent broadcast message, a reply waits for three seconds. If the reply is not received, the reply is sent six more times before the test fails.

### EMAC unicast

**Description:**

The test case tests basic unicast functionality, and the CTP echo server replies to unicast messages.

**Precondition:**

The network interface is connected.

**Test steps:**

1.  Sends three CTP unicast messages (100 bytes each) to the CTP echo server.
2.  Verifies all replies.

**Expected result:**

The echo server replies to unicast messages. For each sent unicast message, a reply waits for three seconds. If the reply is not received, it is sent five more times before the test fails.

### EMAC unicast frame length

**Description:**

The test case tests Ethernet frame lengths.

**Preconditions:**

The network interface is connected.

**Test steps:**

1.  Sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50-byte increments.
2.  Verifies all replies.

**Expected result:**

The echo server replies to messages that are sent to it. For each sent unicast message, a reply waits for 500ms. If the reply is not received, it is sendt again five more times before the test fails.

### EMAC unicast burst

**Description:**

The test case tests Ethernet echoing at full speed.

**Precondition:**

The network interface is connected.

**Test steps:**

1.  Sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50-byte increments.
2.  Repeats the sending 10 times.
3.  Verifies all replies.

**Expected result:**

The echo server replies to messages that are sent to it. For each sent unicast message, a reply waits for five seconds. If the reply is not received, it is sent again five more times before the test fails.

### EMAC unicast long

**Description:**

Test case tests Ethernet echoing at full speed for an extended time.

**Preconditions:**

The network interface is connected.

**Test steps:**

1.  Sends CTP unicast messages with random Ethernet message length.
2.  Repeats the sending 50,000 times.
3.  Verifies all replies.

**Expected result:**

The echo server replies to messages that are sent to it. For each sent unicast message, a reply waits for 350ms. If the reply is not received, it is sent five more times before the test fails.

### EMAC multicast filter

**Description:**

The test case tests multicast filtering. Multicast filtering is an optional feature for the EMAC. The test does not fail if filtering is not implemented.

**Precondition:**

The network interface is connected.

**Test steps:**

1.  Using unicast, verify that the echo server responses are received.
2.  Set the IPv6 multicast filter address and the echo server reply (forward) address to different values. Check if the echo response is filtered.
3.  Set the IPv6 multicast filter address and the echo server reply address to same value. Check that the response is not filtered.
4.  Set the IPv4 multicast filter address and the echo server reply address to different values. Check if the response is filtered.
5.  Set the IPv4 multicast filter address and the echo server reply address to same value. Check that the response is not filtered.
6.  Enable the receiving of all multicasts. Check that the response is not filtered.

**Expected result:**

The echo server replies are received as expected by the test step. Supporting of the filtering will affect what messages are received.

### EMAC memory

**Description:**

The test case tests memory manager out-of-memory situations. The test case configures the test memory manager to reject memory buffer allocations made by the EMAC. Memory buffer allocations are divided into output and input memory allocations:

-   The output memory allocations are the ones made by the EMAC in the ``\`link\_out()\`` function called by the network stack (test case).
-   The input memory allocations are other memory allocations made by the EMAC.

Depending on the EMAC implementation, it may or may not allocate memory manager buffers in the link output function. If the memory manager buffers are not allocated, disabling the link output memory allocations in the test does not affect the functionality.

In each test step, the test case sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50-byte increments. Memory buffers sent to the EMAC in the ``\`link\_out()\`` function are forced to be nonaligned in this test case.

**Precondition:**

The network interface is connected.

**Test steps:**

1.  Memory buffer allocations are allowed. Verify that echo server responses are received.
2.  Disable input memory buffer allocations. The echo server responses should not be received.
3.  Allow memory buffer allocations. Verify that the echo server responses are received.
4.  Disable output memory buffer allocations. The echo server responses may or may not be received depending on the EMAC link out implementation.
5.  Allow memory buffer allocations. Verify that the echo server responses are received.
6.  Disable input and output memory buffer allocations. The echo server responses should not be received.
7.  Allow memory buffer allocations. Verify that the echo server responses are received.
8.  Allocate memory buffers that are sent to the EMAC in link out from the heap (contiguous memory). Verify that the echo server responses are received.

**Expected result:**

The echo server replies to messages that are sent to it based on whether the driver can allocate memory for the frame or not. For each sent unicast message, a reply waits for 500ms. If the reply is not received, it is sent another three times. If the test expects that memory should be available and sending fails three times, the test fails.
