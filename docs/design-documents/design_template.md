# Mbed-OS Design Document
Write your feature title above, in this case its "Mbed-OS Design Document"

# Table of Contents
- [Mbed-OS Design Document](#mbed-os-design-document)
- [Table of Contents](#table-of-contents)
    - [Revision history](#revision-history)
- [Introduction](#introduction)
    - [Overview and Background](#overview-and-background)
    - [Requirements and Assumptions](#requirements-and-assumptions)
- [System Architecture and High-Level Design](#system-architecture-and-high-level-design)
    - [System Architecture and Component Interaction](#system-architecture-and-component-interaction)
- [Detailed Design](#detailed-design)
- [Usage Scenarios and Examples](#usage-scenarios-and-examples)
- [Tools and Configuration Changes](#tools-and-configuration-changes)
- [Other Information](#other-information)
    - [Reusability](#reusability)
    - [Deprecations](#deprecations)
    - [References](#references)
    - [Custom Headings Here](#custom-headings-here)

### Revision history
1.0 - A brief description for this version, e.g. Initial revision - Author name - Date
1.1 - Added new section - Author name - Date

# Introduction
### Overview and Background
Provide a background of this feature. Briefly explain why this feature is needed, what is it accomplishing, what problem its solving. If you already have a requirement written for this feature you may also use the contents of the requirement to provide user with overall context of why this feature is needed.
For example:- This document provides a template for writing software design documents for Mbed-OS features.

### Requirements and Assumptions
Capture the requirements for this feature to work and other assumptions made. For example, if you are assuming specific 
hw capabilities, memory requirements, security assets like presence of RoT infrastructure, then capture those here. 
For example:- This feature requires QuadSPI interface on the target as it implements block device driver over QuadSPI 
interface. It also assumes the system can provide 16K of memory for buffering.

# System Architecture and High-Level Design
This section provides high level information on areas or components needing changes or new development to implement this 
feature. The idea here is to capture the high level design goals of this feature for the target reader, the focus here should
be what functionality it provides and not the actual implementation. For example, if you are implementing a new device driver 
for a communication peripheral/device then the high level design goals could be as below. Add more descrption to each high level design goal if required.

* `Configuring the device` - The driver should provide a specific interface to configure the communication paramaters for the device.
* `Starting and stopping the device` - The driver should provide interfaces to start and stop all the communications.
* `Reading from and Writing to the device` - Read and Write interfaces should be implemented to support sending single/multiple bytes. 
* `Resetting the device` - Functionality to reset the device should be provided.

**NOTE: If you already have supporting architecture documentation in the form of technology documents or other existing documents
feel free to link them here and use that, you don't have to replicate the same documentation here.**

For each high level design goal, provide detailed software design under [Detailed Design](#detailed-design) section providing more details on implementation.

### System Architecture and Component Interaction
Description and diagrams showing overall architecture of how the above-mentioned components/resources interface with each other.
For, example if the new feature is to implement a driver and if it provides interfaces with application and uses memory subsystem in OS
to talk to the device the diagram may look something like below. The goal here is to capture high level components and their interaction and not the 
minute details.

![System Architecture and Component Interaction](./diagram_examples/system_arch_example.jpg)

# Detailed Design
This section provides detailed design on implementation of each of the high level design goal mentioned above.
Each components/modules needing changes are also captured in details. The target audience here is a developer
who can read this section and start the real implemention. You can capture things like signature of interfaces,
flow charts showing how the APIs work, data flow diagrams etc. The headings for each detailed design section
should match the headings in high level design goals. For example, based on the example provided above,
the headings for details design sections should be `Configuring the device`, `Starting and stopping the device`, 
`Reading from and Writing to the device`, `Resetting the device`.

###	Detailed Design 1 - e.g, `Configuring the device`

**API Description**
Detailed API Description like signature of interface, explanation of arguments, return codes etc.
For example, if you are trying to define an interface for configuring the device, you may describe
the API as below.

    `Configure API should have following signature:
    mbed_error_status_t configure(int speed, enum flow_control_t flow_control, int parity_bits);`
    where:`
        speed - is an integer value representing the target communication speed
        flow_control - is the enum value representing the flow control to be used
        parity_bits - is an integer value indicating the parity_bits to be used
       
    And the function should return mbed_error_status_t value indicating the result of the call as below:
        MBED_SUCCESS if the call is successful.
        MBED_ERROR_INVALID_ARGUMENT if input values are invalid.`
        MBED_ERROR_CONFIG_UNSUPPORTED if the device doesnt support the requested configuration.`

**Configuration Sequence Diagram**  

Sequence Diagrams, Data flow diagrams etc. For example, the flow chart for configure device API above may
be something like below.

![Configure device operation](./diagram_examples/flow_chart_example.jpg)

###	Detailed Design 2 - e.g, `Starting and stopping the device`

**API Description**
Detailed API Description like signature of interface, explanation of arguments, return codes etc
For example, for start and stop functions exported by the driver.

**Sequence Diagrams for `Starting and stopping the device`**
Sequence Diagrams, Data flow diagrams etc. For example, the sequence digram for stopping the device may look
something like below.

![Stopping the device](./diagram_examples/sequence_diagram_example.jpg)

###	Detailed Design 3 - e.g, `Reading from and Writing to the device`

**API Description**
Detailed API Description like signature of interface, explanation of arguments, return codes etc

**Sequence Diagram for `Reading from and Writing to the device`**
Sequence Diagrams, Data flow diagrams etc.

###	Detailed Design N - e.g, `Resetting the device`

**API Description**
Detailed API Description like signature of interface, explanation of arguments, return codes etc

**Sequence Diagram for `Resetting the device`**
Sequence Diagrams, Data flow diagrams etc.

# Usage Scenarios and Examples
Show pseudo-code or flowcharts explaining the usage of the feature. For example you may want to include 
some pseudo code to demonstrate how the read functionality is used with the new driver design you are
proposing.

### Scenario 1 - e.g, `Reading from the device`
Mention the specific usage scenarios here. For example, the below examples shows how to read data from
device using the new APIs.

**Scenario 1 Example 1. - e.g, `Reading a byte from device`**
Mention the specific usage here. For example, the below example shows pseudo-code for how to read a single byte data from
the device using the new APIs.

```C
char in_byte = 0;
int num_bytes_read = read(&in_byte, 1);
if(num_bytes_read == 1) {
    //do something
    ...
}
```
**Scenario 1 Example 2. - e.g, `Reading multiple bytes from device`**
In this example, the pseudo-code below shows how to read multiple bytes of data from the device using the new APIs.

```C
char in_bytes[NUM_BYTES];
int num_bytes_read = read(in_bytes, NUM_BYTES);
if(num_bytes_read == NUM_BYTES) {
    //process bytes read
    ...
}
```
### Scenario 2 - e.g, `Writing to the device`
Mention another usage scenarios here. For example, you can demostrate how to write to the device
and few examples of usage.

**Scenario 2 Example 1. - e.g, `Writing a byte to device`**
Demonstrate the specific example here. For example, how to write a byte of data to device.

**Scenario 2 Example 2. - e.g, `Writing multiple bytes to device`**
Demonstrate another specific example here. For example, how to write mutiple bytes of data to device.

# Tools and Configuration Changes
Explain which tools needs to be changed and the nature of changes.
For example, if the feature requires adding a new sub-command to mbed-cli capture
the details of changes and usage here. Each tool change can be captured under its own
sub-heading as below. You can also capture new configuration values which needs to be
added to mbed json based configuration system. For each, config value added explan
the name of the config value and how to use it and its relation to any pre-processor
defines in the implementation.

### Tool Change 1 - e.g: - Command line additions/changes

### Configuration Changes e.g: - Configuration changes in Json files etc

# Other Information
Add other relevant information you would like to capture. Add custom headings if required.

### Reusability
List the components or pieces of implementation which can be re-used for specific 
design patterns/other implementations.

### Deprecations
List the APIs which may be deprecated as part of this feature.

### References
Capture information such as specifications, other design docs, other implementations Urls etc.

### Custom Headings Here
Add your custom headings here, for example security impacts of this feature can be put
under the heading `Security Impacts`.
