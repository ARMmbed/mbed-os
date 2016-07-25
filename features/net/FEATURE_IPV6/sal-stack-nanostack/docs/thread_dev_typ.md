Thread network device types and roles
============

## Device types

The different device types within a Thread network are described in the subsequent sections.

### Border routers

A border router is a specific type of a router that provides connectivity from the 802.15.4 network to adjacent networks on other physical layers (for example, WiFi and Ethernet). Border routers provide services for devices within the 802.15.4 network, including routing services for off-network operations. There may be one or more border routers in a Thread network.

### Routers

Routers provide routing services to network devices. Routers also provide joining and security services for devices trying to join the network. They are not designed to sleep and they can downgrade their functionality and become Router-eligible End Devices (REED).

### Router-eligible end devices (REED)

REEDs have the capability to become Routers but due to the network topology or conditions these devices are not acting as Routers. These devices do not generally forward messages or provide joining or security services for other devices in the Thread network. The Thread network manages REEDs becoming Routers if necessary without user interaction.

### Sleepy end devices

Sleepy end devices are host devices. They communicate only through their Parent Router and cannot forward messages for other devices.

## Roles

Devices participating in a Thread network can take up various roles depending upon their type and configuration of the Thread network join process. A Thread network join process is called as Thread Commissioning. The different roles Thread devices are described in the subsequent sections.

### Commissioner 

The currently elected authentication server for new Thread devices and the authority for providing the network credentials required to join the network. A device capable of being elected as a commissioner is called a commissioner candidate. Devices without Thread interfaces may perform this role, but the ones with a Thread interface may combine this role with all other roles except the Joiner. For example, a Border Router can perform the role of commissioner. This device may be, for example, a cell phone or a server in the cloud, and typically provides the interface by which a human administrator manages the joining of a new device to the Thread network.

### Joiner

Joiner is a device to be added by a human administrator or generally a device needed to be commissioned for the Thread network. This role requires a Thread interface and cannot be combined with any other role in a single device. A joiner does not have network credentials.

### Joiner Router

Joiner Router is an existing Thread Router or a REED on the secure Thread network that is one radio hop away from the joiner. A Joiner Router requires a Thread interface to operate, and may be combined in any device with other roles except the joiner role.

### Thread Leader

Thread Leader is a device responsible for managing router ID assignment. It is a single distinguished device in any Thread network that currently acts as a central arbiter for the network configuration. A Leader requires a Thread interface and may be combined in any device with other roles except the joiner.

**Figure 2-2 General Thread Network**

![nw-arc](img/Thread_General.png)
