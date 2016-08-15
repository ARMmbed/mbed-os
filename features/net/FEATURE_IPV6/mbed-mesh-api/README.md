# mbed mesh API

ARM mbed mesh API allows the client to use the IPv6 mesh network.

The client can use the `LoWPANNDInterface` or `ThreadInterface` object for connecting to the mesh network and when successfully connected, the client can create a socket by using the [mbed C++ socket API](https://developer.mbed.org/teams/NetworkSocketAPI/code/NetworkSocketAPI/docs/tip/) to start communication with a remote peer.

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

**Configurable parameters in section `mbed-mesh-api`:**

| Parameter name  | Value         | Description |
| --------------- | ------------- | ----------- |
| heap-size       | number [0-0xfffe] | Nanostack's internal heap size |

**Thread related configuration parameters:**

| Parameter name  | Value         | Description |
| --------------- | ------------- | ----------- |
| thread-pskd     | string [6-255 chars] | Human-scaled commissioning credentials. |
| hread-device-type | enum from mesh_device_type_t | Set device operating mode. |
| thread-config-channel-mask | number [0-0x07fff800] | Channel mask, 0x07fff800 scans all channels. |
| thread-config-channel-page | number [0, 2]| Channel page, 0 for 2,4 GHz and 2 for sub-GHz radios. |
| thread-config-channel      | number [0-27] | RF channel to use. |
| thread-config-panid        | number [0-0xFFFF] | Network identifier. |
| thread-master-key      | byte array [16]| Network master key. |
| thread-config-ml-prefix | byte array [8] | Mesh local prefix. |
| thread-config-pskc      | byte array [16] | Pre-Shared Key for the Commissioner. |

**6LoWPAN related configuration parameters:**

| Parameter name  | Type     | Description |
| --------------- | ---------| ----------- |
| 6lowpan-nd-channel-mask    | number [0-0x07fff800] | Channel mask, bit-mask of channels to use |
| 6lowpan-nd-channel-page   | number [0, 2] | 0 for 2,4 GHz and 2 for sub-GHz radios |
| 6lowpan-nd-channel        | number [0-27] | RF channel to use when `channel_mask` is not defined |
| 6lowpan-nd-security-mode | "NONE" or "PSK" | To use either no security, or Pre shared network key |
| 6lowpan-nd-psk-key-id | number | PSK key id when PSK is enabled |
| 6lowpan-nd-psk-key | byte array [16] | Pre shared network key |
| 6lowpan-nd-sec-level | number [1-7] | Network security level. Use default `5` |


## Usage notes

This module should not be used directly by the applications. The applications should use the `LoWPANNDInterface` or `ThreadInterface` directly.

### Network connection states

After the initialization, the network state is `MESH_DISCONNECTED`. After a successful connection, the state changes to `MESH_CONNECTED` and when disconnected from the network the state is changed back to `MESH_DISCONNECTED`.

In case of connection errors, the state is changed to some of the connection error states. In an error state, there is no need to make a `disconnect` request and the client is allowed to attempt connecting again.

## Getting started

See the example application [mbed-os-example-mesh-minimal](https://github.com/ARMmbed/mbed-os-example-mesh-minimal) for usage.

## Usage example for 6LoWPAN ND mode

**Create a network interface:**

```
LoWPANNDInterface mesh;
```

**Connect:**

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
