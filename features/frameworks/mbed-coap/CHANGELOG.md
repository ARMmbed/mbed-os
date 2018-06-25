# Change Log

## [v4.5.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.5.0) 
**Closed issues:**
 - IIOTCLT-2769 - mbed-coap: extra response received after registration

Added own flag to enable blockwise support, without setting default blockwise
payload size. This allows to receive blockwise messages while still sending
without blockwise.

Fix CoAP request blockwise response handling
When request is sent, response can have blockwise option set. All requests must
be stored to the linked list.

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.4.4...v4.5.0)

## [v4.4.4](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.4.4) 
**Closed issues:**
 - IOTCLT-2638 [GitHub] hardfault during reconnection retry with Thread

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.4.3...v4.4.4)

## [v4.4.3](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.4.3) 
**Closed issues:**
 - IOTCLT-2506 [GitHub] Cannot set registration time if server does not use max age option

Extend blockwise message transfer status to have states for sending as well.

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.4.2...v4.4.3)

## [v4.4.2](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.4.2) 
**Closed issues:**
 - IOTCLT-2469 CoAP UDP retransmission does not work for blocks after first one for requests (Eg. registration POST)

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.4.1...v4.4.2)

## [v4.4.1](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.4.1) 
**Closed issues:**
 - IOTCLT-2539 Block wise messaging call-backs not working logically

 Improve TCP+TLS transport layer to allow send larger messages without blockwising.

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.4.0...v4.4.1)

## [v4.4.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.4.0) 
**New feature:**
- Make sn_coap_protocol_send_rst as public needed for CoAP ping sending
- Allow disabling resendings by defining SN_COAP_DISABLE_RESENDINGS

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.3.0...v4.4.0)

## [v4.3.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.3.0) 
**New feature:**
-  Add new API which clears the whole sent blockwise message list

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.2.0...v4.3.0)

## [v4.2.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.2.0) 
**New feature:**
-  Add new API to remove sent blockwise message from the linked list

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.1.1...v4.2.0)

## [v4.1.1](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.1.1) 
**Closed issues:**
-  IOTCLT-2203 mbed-coap does not handle PUT or POST if they indicate a smaller block size preference (fixed regression) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.1.0...v4.1.1)

## [v4.1.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.1.0) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.10...v4.1.0)

 **New feature:**
  - New API to disable automatic GET(BLOCK2) request sending.

 **Closed issues:**
  -  IOTCLT-2203 mbed-coap does not handle PUT or POST if they indicate a smaller block size preference 

## [v4.0.10](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.10) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.9...v4.0.10)

 **Closed issues:**
  -  IOTMAC-615 Node mDS registration failure during OTA transfer

## [v4.0.9](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.9) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.8...v4.0.9)

 **Closed issues:**
  -  IOTCLT-1899 Maximum COAP message resending buffer size limited to 255 bytes
  -  IOTCLT-1888 Problem with blockwise transfers that are even increments of block_size

## [v4.0.8](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.8) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.4...v4.0.8)

 **Closed issues:**
  -  IOTCLT-1880 Lwm2m blockwise data transfer (using Block1 option) with Leshan not working
  -  IOTCLT-1885 Return 4.08 Request Entity Incomplete on Block transfer errors
  -  IOTCLT-1883 Detected message duplications stop mbed-client

## [v4.0.4](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.4) 

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.3...v4.0.4)

 **Closed issues:**
	- https://github.com/ARMmbed/mbed-client/issues/481 - Obs Con blockwise fails to transmit 2nd block

## [v4.0.3](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.3)

-[Full Changelog](https://github.com/ARMmbed/mbed-coap/compare/v4.0.2...v4.0.3)

 **New feature**
 		
 	- CoAP Protocol Confirmable resend fix and minor memory optimization (IOTMAC-328)
 		
 **Closed issues:**
 		
 	- IOTCLT-1439 - stuck in while loop

## [v4.0.0](https://github.com/ARMmbed/mbed-coap/releases/tag/v4.0.2)

**New feature**

 - Initial release of mbed-coap separated from mbed-client-c
