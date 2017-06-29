Thread network device types and roles
============

There are different device types and roles in the Thread network. mbed OS offers own builds or configurations to different Thread device types and roles.

## End devices

Usually, end devices are the most constrained devices in the Thread network. They serve as a last hop in the mesh topology. Devices such as plain sensors, controls and switches can be implemented as end devices only. The different types of end devices are described in the following sections.

### Full end device (FED)

A Thread host device that communicates through the parent device, but has the least constraints to the parent. A FED makes address queries and listens to multicast messages.

### Minimal end device (MED)

A Thread host device type that registers its addresses to the parent leaving all routing and address queries to be done by the parent.

### Sleepy end device (SED)

A Thread host device that is similar to the minimal end device, but does not keep the radio on all the time. It uses the polling mechanism to receive packets from the parent. Requires parents to store the packets.

## Routers

Routers provide routing services to the network devices. Routers also provide joining and security services for devices trying to join the network.

A router always attaches to the Thread network as a Router Eligible End Device (REED). REEDs do not forward messages or provide any joining or security services for other devices in the Thread network. The Thread network manages REEDs becoming routers (or vice versa) if necessary without user interaction.

## Roles

Devices participating in a Thread network can take up various roles depending upon their device type and the Thread network join process configuration. The Thread network join process is called Thread Commissioning. The different roles of Thread devices are described in the subsequent sections.

### Border router

A border router is a specific type of a router that provides connectivity from the Thread network to adjacent networks on other physical layers (for example, WiFi and Ethernet). Border routers provide services for devices within the 802.15.4 network, including routing services for off-network operations. There may be one or more border routers in a Thread network.

### Commissioner 

The currently elected authentication server for new Thread devices and the authority for providing the network credentials required to join the network. A device capable of being elected as a commissioner is called a commissioner candidate. Devices without Thread interfaces may perform this role, but the ones with a Thread interface may combine this role with all other roles except the Joiner. For example, a border router can perform the role of commissioner. This device may be, for example, a cell phone or a server in the cloud, and typically provides the interface by which a human administrator manages the joining of a new device to the Thread network.

### Joiner

A joiner is a device to be added by a human administrator or generally a device needed to be commissioned for the Thread network. This role requires a Thread interface and cannot be combined with any other role in a single device. A joiner does not have network credentials.

### Joiner router

A joiner router is an existing Thread router or a REED on the secure Thread network that is one radio hop away from the joiner. A Joiner router requires a Thread interface to operate, and may be combined in any device with other roles except the joiner role.

### Thread leader

A Thread leader is one of the routers that has formed the network and takes the following additional responsibilities on top of the router responsibilities. The first router on a network becomes a leader. 

* Managing router ID assignment.
* Thread network data registration and distribution.
* Commissioner petition.
* Accepting and distributing Thread configurations to the rest of the network.

Thread leader is a single distinguished device in any Thread network, acting as a central arbiter for the network configuration. 

**Figure 2-2 General Thread Network**

![nw-arc](img/Thread_General.png)
