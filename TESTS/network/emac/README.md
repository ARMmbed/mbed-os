# Introduction

This document describes how to run EMAC tests. The EMAC test cases are made using the Ethernet Configuration Testing Protocol (CTP). To run the tests, one device in the Ethernet segment needs to be configured to be a CTP echo server. The devices running the test cases, use the echo server to forward the CTP Ethernet frames back.

## Configuring the CTP echo server

To configure a device to be a CTP echo server, you need to enable the `echo-server` setting in the `json` file of the test environment application. When a device is configured to be a CTP echo server, it starts to forward CTP messages automatically when it is switched on and continues to do so until it is switched off.

## Test cases

### EMAC initialize

The test case initializes the EMAC driver and the test network stack.

The EMAC test environment uses the test network stack as the default stack. To enable the stack, set the `nsapi.default-stack` option in the `json` file of the test environment application to value `TEST`.

The test network stack is a bare minimum implementation and has the functionality needed to set up the network interface. The test network stack is defined in the `emac_TestNetworkStack.h` and `emac_TestNetworkStack.cpp` files. The stack uses the test memory manager for the EMAC. The test memory manager is defined in the `emac_TestMemoryManager.h` and `emac_TestMemoryManager.cpp` files. Message buffers sent to the EMAC in `link_out()` are allocated from the buffer pool of the test memory manager. The test memory manager pool allocation unit (buffer size) is 610 bytes.

The initialization test constructs the network interface and connects to it. The test network stack and the EMAC are bound to the network interface using `get_default_instance()` calls to the stack and to the EMAC.

After the construction, the network interface is connected. A connect call triggers a set up call to the test network stack. The set up call triggers a call to `emac_if_init()` function in the EMAC initialization test case. 

The `emac_if_init()` function of the test case configures and powers up the EMAC.

The configuration steps are:

* Setting the test memory manager for the EMAC.
* Setting the EMAC link input and state callbacks to call the test environment input and state callback handlers.
* Reading and setting the Ethernet MAC address.

### EMAC broadcast

1. Sends three CTP broadcast messages (100 bytes each)
2. Waits for three seconds
3. Sends three CTP broadcast messages (60 bytes each). 
4. Listens for the CTP echo server responses. 
5. Stores the addresses of the echo servers if replies are received. 

The test case passes if there are no responses from the echo server, but further test cases are skipped.

### EMAC unicast

1. Sends three CTP unicast messages (100 bytes each) to the CTP echo server. 
2. Verifies that all are replied. 

### EMAC unicast frame length
 
1. Sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50 bytes increments. 
2. Verifies that all are replied. 

### EMAC unicast burst
 
1. Sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50 bytes increments. 
2. Repeats the sending 10 times. 
3. Verifies that all are replied. 

### EMAC multicast filter
 
Tests multicast filtering. Multicast filtering is an optional feature for the EMAC. The test does not fail if filtering is not implemented.

The multicast testing requests the CTP echo server to forward the CTP messages to a specified multicast address as the destination address.

Test steps:

1. Using unicast, verify that the echo server responses are received.
2. Set the IPv6 multicast filter address and the echo server reply (forward) address to different values. Check if the echo response is filtered.
3. Set the IPv6 multicast filter address and the echo server reply address to same value. Check that the response is not filtered.
4. Set the IPv4 multicast filter address and the echo server reply address to different values. Check if the response is filtered.
5. Set the IPv4 multicast filter address and the echo server reply address to same value. Check that the response is not filtered.
6. Enable the receiving of all multicasts. Check that the response is not filtered.

### EMAC memory

Tests memory manager out-of-memory situations. The test case configures the test memory manager to reject memory buffer allocations made by the EMAC. Memory buffer allocations are divided into output and input memory allocations:

* The output memory allocations are the ones made by the EMAC in the `link_out()` function called by the network stack (test case). 
* The input memory allocations are other memory allocations made by the EMAC. 

Depending on the EMAC implementation, it may or may not allocate memory manager buffers in the link output function. If the memory manager buffers are not allocated, disabling the link output memory allocations in the test does not affect the functionality.

In each test step, the test case sends CTP unicast messages with Ethernet message length from 100 bytes to the maximum defined by the MTU of the EMAC with 50 bytes increments. Memory buffers sent to the EMAC in the `link_out()` function are forced to be non-aligned in this test case.

Test steps:

1. Memory buffer allocations are allowed. Verify that echo server responses are received.
2. Disable input memory buffer allocations. The echo server responses should not be received.
3. Allow memory buffer allocations. Verify that the echo server responses are received.
4. Disable output memory buffer allocations. The echo server responses may or may not be received depending on the EMAC link out implementation.
5. Allow memory buffer allocations. Verify that the echo server responses are received.
6. Disable input and output memory buffer allocations. The echo server responses should not be received.
7. Allow memory buffer allocations. Verify that the echo server responses are received.
8. Allocate memory buffers that are sent to the EMAC in link out from the heap (contiguous memory). Verify that the echo server responses are received.

