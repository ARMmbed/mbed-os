# Description

This document describes how to run EMAC tests. The EMAC test cases are made using Ethernet Configuration Testing Protocol (CTP). To run the tests, one device in the Ethernet segment needs to be configured to be a CTP echo server. The devices running the test cases, use the echo server to forward the CTP Ethernet frames back.

# Configuring CTP echo server

A device can be configured to be a CTP echo server by enabling `echo-server` setting in the test environment's application `json` file. When device is configured to be a CTP echo server, it starts to forward CTP messages automatically after power up and will continue forwarding until power down.

# Test cases

## EMAC interface initialise

Initializes EMAC interface driver.

For WLAN installs test case so that it can intercept incoming Ethernet messages from the WLAN driver. Incoming CTP frames are handed by the test case and other frames are forwarded to the LWIP stack.

## EMAC interface broadcast

Sends three 100 byte CTP broadcast messages, waits for three seconds and sends three 50 byte CTP broadcast messages. Listens for the CTP echo server responses and stores the addresses of the echo servers if replies are received. The test case will pass if there are no responses from echo server, but further test cases will be skipped.

## EMAC interface unicast

Sends three CTP unicast messages to the CTP echo server. Verifies that all are replied.

## EMAC interface unicast frame length
 
Sends CTP unicast messages with Ethernet message length from 100 bytes to maximum. Verifies that all are replied.

