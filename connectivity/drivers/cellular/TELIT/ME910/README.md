# ME910 Cellular Configuration Notes

The ME910 cellular modem driver supports two modes of operation: PPP and AT-based. 

## PPP Mode

PPP (point-to-point protocol) mode is used by default. In this mode, Mbed's IP stack is built into the application binary and the application is responsible for orchestrating data/socket connections over PPP.

The main implication of using this mode is that the primary UART connection to the ME910 modem is *consumed* by the PPP connection (ie: no other traffic, such as AT commands, is allowed on this UART connection while PPP is active).

If the application wants to perform any other functions with the ME910 while a PPP connection is active (eg: GPS/GNSS lookup), it must use the auxiliary UART to do so.

## AT mode (IPEasy)

The ME910 modem hosts an internal AT-command-based IP socket stack called "IPEasy". Using this mode has two advantages: 

1.) It offloads the (relatively significant) RAM and ROM consumption of the IP stack to the modem itself

2.) It allows a single, shared UART connection to be used for all AT-command-based operations on the modem. ie: it enables the use of IPEasy/Socket-related AT commands *and* GPS/GNSS-related AT commands (among other commands) concurrently on the same UART.

One caveat found during testing of this AT driver is that the way in which Mbed performs DNS lookups using a raw socket is not supported properly by the Telit IPEasy implementation on the ME910. The workaround in this case is to use Telit's IPEasy DNS lookup commands so the ME910 performs the DNS lookups itself.

Therefore, to use the AT/IPEasy-based stack on the ME910 it is **very important** that you apply **all** of the following configuration changes:

```
"ppp.enabled": false,
"cellular.offload-dns-queries": true
```

To reiterate: it is **required** to enable offloading of DNS queries to the TELIT ME910 IPEasy stack. Attempting to use the AT-command-based IPEasy stack without this option enabled will likely result in failure to initialize connections.
