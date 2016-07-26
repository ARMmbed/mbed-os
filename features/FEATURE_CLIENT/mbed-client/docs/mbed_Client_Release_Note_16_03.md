
# Release note for mbed Client 16.03
 
mbed Client is an OS-agnostic embedded software library that provides the means to connect and manage constrained embedded devices to web applications through mbed Device Server.
 
## Release summary
 
This is an iterative release of mbed Client, which is available for mbed OS as well as for Ubuntu-like Linux distributions. We’re continuously maturing mbed Client and adding new features to mbed Client.. This release builds on the Technology Preview 15.11 release through the introduction of new APIs, enhanced functionalities and bug fixes.
## Collaboration
We’re building mbed Client as a collaborative project, bringing together industry and open source community contributions. If you’d like to work on mbed Client with us, we’d encourage you [to pitch in](https://github.com/ARMmbed/mbed-os/blob/master/CONTRIBUTING.md). With this technology preview we’re ready to start receiving contributions back from the community.
## Documentation
To get started with mbed Client, please visit our [getting started guide](https://docs.mbed.com/docs/mbed-client-guide/en/latest/). 
# Changes since last release
 
## New APIs and functions

* New APIs:
 * Setting the max-age of a resource value. See [https://tools.ietf.org/html/draft-ietf-core-coap-09#section-5.10.6](https://tools.ietf.org/html/draft-ietf-core-coap-09#section-5.10.6).
 * Omitting registered resources' URI path from the registration message's body (which is sent from the client to the server). 
 * Allowing the client to send a delayed response to POST requests. See [https://tools.ietf.org/html/draft-ietf-core-coap-09#section-5.2.2](https://tools.ietf.org/html/draft-ietf-core-coap-09#section-5.2.2).
 * Getting Object and Object Instance information from Resource Object.
* Added a new class for handling arguments received from POST method for Resource. 
* Enabled CoAP Blockwise payload handling by client. See  [https://tools.ietf.org/html/draft-ietf-core-block-08#section-2](https://tools.ietf.org/html/draft-ietf-core-block-08#section-2).
* Added support for handling observation cancellation through a RESET message from Device Connector Server.
* Disabled Bootstrap API functionality from source code.

## Bug fixes
 
1. Multiple fixes related to Object, Object Instance and Resource Observation.
2. Multiple fixes related to Write Attributes parameter handling for Object, Object Instance and Resource.
3. Multiple fixes for creating and deleting object instances.
4. Fixes related to sending notifications from multiple resource levels.
5. Fixing memory leaks.
6. Fixing mbed Client IPv6 connectivity for Linux.
7. Memory handling in the ``m2mstring`` class.
8. Fix for preventing the client from sending multiple confirmable messages on mbed OS.
 
# Modules versions
 
mbed Client is available as part of mbed OS or as a standalone component for Ubuntu-like Linux distributions.

The yotta modules & versions released for these different product configurations are listed below.

mbed Client as part of mbed OS:

1. mbed-client-quickstart (example)
1. mbed-client - 1.6.x
1. mbed-client-c - 2.2.x
1. nanostack-libservice - 3.1.x
1. mbed-client-mbedtls - 1.0.x
1. mbedtls - 2.2.0
1. mbed-client-mbed-os - 1.1.x
1. mbed-client-examples
1. target-frdm-k64f-gcc - 2.0.0


mbed Client as a standalone distribution for Ubuntu-like Linux distributions:

1. mbed-client-linux-example
1. mbed-client - 1.6.x
1. mbed-client-c - 2.2.x
1. nanostack-libservice - 3.1.x
1. mbed-client-mbedtls - 1.0.x
1. mbedtls - 2.2.0
1. mbed-client-linux - 1.1.x
1. target-x86-linux-native - 1.0.0
1. target-linux-native - 1.0.0
 
 
### Known issues
 
The known issues list for this release is available as [a separate document](Known_Issues.md)

### Other ways of accessing this release


We prefer that you access and collaborate with mbed Client online. However, the release may also be downloaded as a [tar archive file](https://mbed-media.mbed.com/filer_public/7b/75/7b75d9fe-bf15-4818-b959-c54e11e767af/mbed-client-1603.zip).
 

