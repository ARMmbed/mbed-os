## Questions and answers

### How are devices joined to the networks and what are the options for commissioning modes? 

There are no different commissioning modes for devices joining to the network, there are only different kinds of commissioners (on-mesh, native, external). Thread devices either have the configuration or not. If they don’t have the configuration, they try to find a Thread network that accepts them. Then they do the commissioning using DTLS to authenticate themselves and if successful, they will receive the network credentials and become a part of the network.

### Where are the default configurations of the Nanostack set?

- Thread devices do not have a default configuration. They should always be commissioned to the network and receive the configuration on site. A commissioner can modify the configurations to the entire network and all Thread devices in the network will automatically pick the new configuration and save it to NVM.
- The border router may have a default configuration set, that provides a starting point for setting up the network. 

### Are the Thread configurations easily exposed to the application and are they runtime configurable?

- The Thread management API can be used to read/store/delete the Thread link configuration and to define the device configuration (`thread_management_if.h`). 
- The Thread commissioner can change the network configuration of the entire Thread network. By using the `thread_commissioning_api.h` and `thread_management_if.h`, you can build an on-mesh or native commissioner that can update the network configuration.
- For example, the border router can be set up with the default configuration. The commissioner can then update the whole Thread network configuration (including border router).

### Who selects the channel and PAN ID? Will different manufacturers pick different channels and PAN IDs?

Channel and PAN ID are part of the Thread configuration that is received when a device is commissioned to the Thread network. The commissioner can modify the configuration at any time. The manufacturer does not decide the Thread link configuration.

### How can ad-hoc mesh networks be easily formed with different manufacturers?

Ad-hoc networks with devices from different manufacturers can be easily created by commissioning.

### Does the border router listen on all channels or just one?

- The border router is a Thread router device and uses the same configuration as other devices in the Thread network. So it uses a single channel.
- Devices that have not yet joined the Thread network scan all channels and PAN IDs to find a Thread network that they want to be part of.

### How to commission with a mobile device? Is there an example app?

An example application is [downloadable from the Android store](https://play.google.com/store/apps/details?id=org.threadgroup.commissioner).

### Are there any application examples? What are the Thread features supported/not supported by them?

- Application examples supporting SED and REED types:
  - [mbed OS Client example](https://github.com/ARMmbed/mbed-os-example-client) 
  - [mesh minimal example](https://github.com/ARMmbed/mbed-os-example-mesh-minimal)
- [Thread border router](https://github.com/ARMmbed/nanostack-border-router)

### How to debug and visualize the network?

- You can download a Wireshark version that supports the Thread protocol from the [Wireshark web page](https://www.wireshark.org/download.html)
- A commissioner can do network diagnostics and topology images using Thread-specific diagnostics commands. Our software supports this but there is no C/C++ API support.

### How to check for errors or check the link quality?

Link quality and errors are easiest to debug using Wireshark.

### How to send/receive with UDP and TCP sockets. How to use multicast groups?

The mesh minimal uses socket communication and multicasting. See more details in the [mesh minimal example](https://github.com/ARMmbed/mbed-os-example-mesh-minimal).

### What if my ISP does not provide IPv6 service? Why do I need IPv6 internet service? Can I use a tunneling service?

Thread is based on the IPv6 protocol. If the communication must go over IPV4 network, there are two solutions available:
- NAT64
- IPv6 tunneling.

### What happens if a packet cannot reach its destination (for example, no internet connection to border router)? What is the responce back to the device?

There are no special Thread codes available, you will have the normal internet error codes. An application must handle the temporary network connection problems with proper retry logic.

### How are network errors sent back to the device? Is there a description of what went wrong or simply no response?

There are no special error codes in Thread. The application can receive ICMPv6 error codes.

### How to message all nodes in the mesh? Can I use a multicast address?

IPv6 multicast messages are supported. link-local and realm-local (mesh-local) multicast scopes work within the Thread network, as well the site-local multicast messages.

### How can I use link-local addresses versus global addresses?

- Usually, global addresses are used to communicate with the remote services. When a device has the destination address, the network stack selects the proper source address and route to that destination. 
- An application developer may implement a protocol (using multicast) for querying the link-local or mesh-local addresses from other devices in the Thread network. Link-local addresses work only over one radio hop. Mesh-local addresses work over multiple hops in the Thread network.
- The mbed OS mesh API provides functions to query the device's own addresses (link-local or global).

### What are the configurable parameters in the Thread devices? What do they mean and where are they documented?

- The Thread configurable parameters are the Active Operational Dataset parameters. The parameter explanations can be found in the [mbed Mesh API](https://github.com/ARMmbed/mbed-os/blob/master/features/nanostack/FEATURE_NANOSTACK/mbed-mesh-api/README.md). 
More information can be found in the Thread specification. 
- The Thread device configuration includes PSKd and EUI64 (usually the MAC address of the radio chip), that are used to commission the device.

### What are PSKc and PSKd? Where can find these?

- PSKc is the authentication key for the native or external commissioner. This value is generated from the passphrase of the network combined with the network name and XPANID that are received when scanning the networks. The commissioner application asks for this passphrase after scanning and selecting an available Thread network, exactly the same way as in WiFi.
- PSKd is the authentication key used to authenticate a device to the Thread network in combination with EUI64. 
- These values are printed to a sticker on the device or the device packaging in the form of a QR code that is then scanned by the commissioner application. This will allow the device to be commissioned to the Thread network. After the authentication, the Thread configuration is given to the new device. 

