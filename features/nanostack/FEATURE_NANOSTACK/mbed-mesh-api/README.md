# mbed mesh API

ARM mbed mesh API allows the client to use the IPv6 mesh network.

The client can use the `LoWPANNDInterface` or `ThreadInterface` object for connecting to the mesh network and when successfully connected, the client can create a socket by using the [mbed C++ socket API](https://developer.mbed.org/teams/NetworkSocketAPI/code/NetworkSocketAPI/docs/tip/) to start communication with a remote peer.

For ethernet `NanostackEthernetInterface` is provided.

## Supported mesh networking modes

Currently, 6LoWPAN-ND (neighbour discovery) and Thread bootstrap modes are supported.

## Module Configuration

This module supports static configuration via **mbed configuration system** by using the `mbed_app.json` file. The application needs to create the configuration file if it wants to use other than default settings.

An example of the configuration file:

```
{
    "target_overrides": {
        "*": {
            "target.features_add": ["IPV6"],
            "mbed-mesh-api.6lowpan-nd-channel": 12,
            "mbed-mesh-api.6lowpan-nd-channel-mask": "(1<<12)",
            "mbed-mesh-api.heap-size": 10000
        }
    }
}
```

### Configurable parameters in section mbed-mesh-api

| Parameter name  | Value         | Description |
| --------------- | ------------- | ----------- |
| heap-size       | number [0-0xfffe] | Nanostack's internal heap size |
| use-malloc-for-heap | `false` or `true` | Use `malloc()` for reserving the internal heap. Default: `false` |

### Thread related configuration parameters

| Parameter name  | Value         | Description |
| --------------- | ------------- | ----------- |
| thread-pskd     | string [6-255 chars] | Human-scaled commissioning credentials. |
| thread-use-static-link-config | boolean | True: Use the below link config, False: Use commissioning, ignore the below link config. |
| thread-device-type | enum from mesh_device_type_t | Supported device operating modes: MESH_DEVICE_TYPE_THREAD_ROUTER, MESH_DEVICE_TYPE_THREAD_SLEEPY_END_DEVICE, MESH_DEVICE_TYPE_THREAD_MINIMAL_END_DEVICE |
| thread-config-channel-mask | number [0-0x07fff800] | Channel mask, 0x07fff800 scans all channels. |
| thread-config-channel-page | number [0, 2]| Channel page, 0 for 2,4 GHz and 2 for sub-GHz radios. |
| thread-config-channel      | number [0-27] | RF channel to use. |
| thread-config-panid        | number [0-0xFFFF] | Network identifier. |
| thread-config-network-name | string [1-16] | 
| thread-config-commissioning-dataset-timestamp | [0-0xFFFFFFFFFFFFFFFF] | [48 bit timestamp seconds]-[15 bit timestamp ticks]-[U bit] |
|thread-config-extended-panid | byte array [8] | Extended PAN ID |
| thread-master-key      | byte array [16]| Network master key. |
| thread-config-ml-prefix | byte array [8] | Mesh local prefix. |
| thread-config-pskc      | byte array [16] | Pre-Shared Key for the Commissioner. |
| thread-security-policy | number [0-0xFF] | Commissioning security policy bits |

### 6LoWPAN related configuration parameters

| Parameter name  | Type     | Description |
| --------------- | ---------| ----------- |
| 6lowpan-nd-channel-mask    | number [0-0x07fff800] | Channel mask, bit-mask of channels to use |
| 6lowpan-nd-channel-page   | number [0, 2] | 0 for 2,4 GHz and 2 for sub-GHz radios |
| 6lowpan-nd-channel        | number [0-27] | RF channel to use when `channel_mask` is not defined |
| 6lowpan-nd-panid-filter | number [0-0xffff] | Beacon PAN ID filter, 0xffff means no filtering |
| 6lowpan-nd-security-mode | "NONE" or "PSK" | To use either no security, or Pre shared network key |
| 6lowpan-nd-psk-key-id | number | PSK key id when PSK is enabled |
| 6lowpan-nd-psk-key | byte array [16] | Pre shared network key |
| 6lowpan-nd-sec-level | number [1-7] | Network security level. Use default `5` |
| 6lowpan-nd-device-type | "NET_6LOWPAN_ROUTER" or "NET_6LOWPAN_HOST" | Device mode. Router is routing packets from other device, creating a mesh network. |

## Usage notes

This module should not be used directly by the applications. The applications should use the `LoWPANNDInterface`, `ThreadInterface` or `NanostackEthernetInterface` directly.

When using Ethernet interface, there is no configuration options available. It is using dynamic mode to learn the IPv6 prefix from the network. No static configuration is supported.

### Network connection states

After the initialization, the network state is `MESH_DISCONNECTED`. After a successful connection, the state changes to `MESH_CONNECTED` and when disconnected from the network the state is changed back to `MESH_DISCONNECTED`.

In case of connection errors, the state is changed to some of the connection error states. In an error state, there is no need to make a `disconnect` request and the client is allowed to attempt connecting again.

## Getting started

See the example application [mbed-os-example-mesh-minimal](https://github.com/ARMmbed/mbed-os-example-mesh-minimal) for usage.

## Usage example for 6LoWPAN ND mode

Create a network interface and driver objects.

```
LoWPANNDInterface mesh;
NanostackRfPhyNcs36510 rf_phy;
```

Initialize interface with given PHY driver.

```
mesh.initialize(&rf_phy);
```

Then connect to network:

```
    if (mesh.connect()) {
        printf("Connection failed!\r\n");
        return -1;
    }

    printf("connected. IP = %s\r\n", mesh.get_ip_address());
```

## Usage example for 6LoWPAN Thread mode

Basically the same as for ND, but the network interface uses different class:

```
ThreadInterface mesh;
mesh.connect();
```

## Usage example with Ethernet

API is still the same, you just need to provide a driver that implements `NanostackEthernetPhy` API.

```
NanostackEthernetInterface eth;
NanostackEthernetPhyK64F phy;

int main() {
    eth.initialize(&phy);

    if (eth.connect()) {
        printf("Connection failed!\r\n");
        return -1;
    }

    printf("connected. IP = %s\r\n", eth.get_ip_address());
}
```
