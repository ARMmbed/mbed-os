# IoT\Product Requirements\IOTPREQ-334: Design of the mbed OS Configuration Store

The Configuration Store forms a building block of mbed OS. It forms a central repository where configuration for many modules can be placed. This hides the complexity of implementing a similar system from each module owner and it encourages rich configuration interfaces for the developer.

## Background
Configuration is a common operation in embedded systems. It happens at compile time, and run time. Sometimes runtime configuration is persistent across resets, sometimes it is not. Frequently, configuration data is sensitive, such as WiFi passwords, API tokens, encryption keys, and the like. Some configuration is only accessed once in a while. Other configuration must be changed and accessed very frequently and with very little overhead.

## Sample Use Cases
Several sample use cases were used to derive the requirements of the Configuration Store

### Network Configuration
Network configuration varies by network type. As a result, presenting a common interface for network configuration is difficult. Using a pointer to a configuration blob simplifies the Network API and concentrates complexity and knowledge about the network implementation in the network driver and the configuration mechanism in the application.

Networks need hierarchical configuration. A flat model starts to fail when multiple interfaces with similar parameters are used. Most networks need non-volatile, runtime configuration, but Wi-Fi demonstrates this need the best: configuring a Wi-Fi network on a device requires, at minimum, selecting a SSID and entering a password. These must persist past power cycles. Network configuration needs to support overrides. When configuring a network device, it should be possible to recover old configuration until new configuration is committed to non-volatile storage. A network device should ship with sensible default configuration (e.g. DHCP), but this should be overridden when necessary.

Network configuration requires many kinds of value: integer (Channel number), string (SSID), binary blob (hashed password). There is an argument for floating point (transmit power), but this can be done via integer and fixed-point.

### Credential Storage
Storing credentials requires secure access to the storage. As a result, it must be possible to apply permissions to parts or the whole of the tree. Exactly how these permissions work is TBD.

### System initialization
It is conceptually possible to reduce the number of code versions by using more configuration. For example, clock configuration can be done using the config mechanism. If this is the case, then the permanent config store must be accessible early in the boot process, when clocks are configured.

It is necessary to provide a list of modules that explicitly require an init function to be called. To make this possible, those functions could be listed, in order in the configuration store. This has some advantages over conventional approaches, such as linker sections, in that it provides a much more readable way to inspect the modules that are being initialized.

### Resource Management
In the future, a resource manager may be an integral part of mbed OS. In this instance, the resource manager needs in-depth information about how peripherals are connected to clocks and power domains. The Configuration Store should contain this information.

### Peripheral Configuration
If the system interface API (mbed-drivers) were aware of the Configuration Store, then it would be straight-forward to encode defaults via config. This would allow the interface API to extract application-dependent, but still sensible defaults from the config store when they are omitted. This could be combined with Resource Management to automatically correct for clock scaling of buses or the core.

## Design Goals
A number of design goals must be met for the configuration store to be used widely. These design goals are brought out of
The configuration store must:

* hierarchical
    * configuration data must be groupable
    * groups of configuration data should be handled as a unit
* support multiple degrees of persistence
    * permanent
    * non-volatile
    * volatile
* allow config overrides between persistence levels (volatile being the highest priority and permanent the lowest)
* support multiple sources of configuration
    * yotta config
    * application configuration
    * runtime configuration
    * remote configuration (e.g. CBOR blobs returned by a server)
* support multiple kinds of values
    * integer
    * floating point
    * string
    * arbitrary binary blob
* be compatible with [yotta config](http://yottadocs.mbed.com/reference/config.html) output
    * Any format to which JSON is convertible fulfills this requirement
* globally accessible by any module
* able to enforce permissions on entire branches of the config data
* accessible early
* Low memory footprint for parsing
* Low CPU overhead for data accesses

## CBOR
The simplest approach to guaranteeing interoperability with yotta config is to ensure that the format of the Config Store is convertible to JSON. There are several options for this, but the most promising is CBOR. CBOR may not be appropriate for an in-memory representation, but it is likely appropriate for both permanent and non-volatile representations.

Selection of CBOR as an intermediate representation also satisfies the hierarchy requirement and multiple value requirement. CBOR may require a significant CPU/memory tradeoff to be made during parse operations, but there are several mitigation strategies that can be employed.

## Applying permissions
It may be very difficult to apply permissions to a CBOR tree directly. A review of permission management with the uVisor team is necessary.

## Configuration Overrides
To ensure that the correct configuration is always used, permanent configuration must be overriden by non-volatile configuration. Likewise, non-volatile configuration must be overridden by volatile configuration.

The exact method of overrides is TBD.

An initial assessment of this suggests that it would be feasible to accomplish overrides by using only two layers instead of three. On boot, the non-volatile storage is parsed into the volatile storage area. When a node is deleted and its parent is a non-volatile storage element, the key of the deleted node should be re-parsed from non-volatile storage into the location of the deleted node.

## Early Accessibility
Configuration Store initialization must occur after C library init, but before static object initialization, so that static objects can access the Configuration Store. It is possible that a limited version of the Configuration Store could be used prior to C library init, but it would need to be written very carefully.

## Configuration Sources
In some cases, it might be desirable to treat configuration delivered by a server as non-volatile. Changes can be applied locally or requested remotely, but the server is always queried on startup to obtain the configuration data. In general, yotta config will aggregate all configuration data and supply it to the build system in a single JSON file.

Where credentials are provisioned at time of programming, it might be desirable to supply an alternative method of installing a source of configuration. The method for using this alternative source is TBD.

# Low Level Design
The Configuration Store is built in three tiers:
* Permanent Storage
* Non-volatile Storage
* Volatile Storage

## Permanent Storage
Permanent configuration is generated as a CBOR translation of the JSON file created by yotta config. It is linked into the final executable. Parsing permanent storage requires a CPU/memory tradeoff to be made. Indices into the CBOR object can be retained either in RAM or ROM to reduce parsing time at the expense of additional memory. Parsing could be done on-demand or in advance.

## Non-volatile Storage
Non-volatile storage has no specific format requirements, however reusing CBOR parsing from permanent storage may be a good solution. When a change is committed to non-volatile storage, the non-volatile portions of the Configuration store must be re-encoded for storage to flash.

## Volatile Storage
There are two possibilities for implementing volatile storage: either a document object model, or a flat key-value store with recognition of prefixes. The document object model will present a familiar interface for web programmers, and occupy less memory than a flat key-value store. Therefore, a document object model is the preferred solution.

### Objects containing constant data
For most operations, the Configuration Store must copy both keys and values into its own storage. In some cases, it may be advantageous to only retain a reference to external data, such as in cases where external data has been stored in a nonvolatile medium. While this is not an immediate requirement, care should be taken not to prevent this future optimization.


# References

- IoT\Product Requirements\IOTPREQ-334 ![CFSTORE_PROD_REQ_REF_01](http://jira.arm.com/browse/IOTPREQ-334)

