## The Secure Partition Manager

The **Secure Partition Manager (SPM)** is a Platform Security Architecture (PSA) compliant software hypervisor that creates and manages independent Secure Partitions on Arm Cortex&reg;-M microcontrollers. It increases resilience against malware and protects secrets from leaking between different modules in the same application. The SPM complements other important security features, such as safe firmware updates and secure crypto libraries.

The SPM provides hardware-enforced partitions for individual code blocks by limiting access to memories and peripherals using the existing hardware security features of the Cortex&reg;-M microcontrollers. It isolates software in partitions, managing the execution of software within those partitions and providing Inter Process Communication (IPC) between the partitions. Correct use of SPM prevents malware from becoming resident on the device and enables protection of device secrets, such as cryptographic keys.

### Isolating partitions in the Secure Processing Environment

The SPM and the secure partitions are located in the Secure Processing Environment (SPE), isolating them from the Non-Secure Processing Environment (NSPE), which contains the application firmware, OS kernel and libraries, and other nonsecure hardware resources.

A secure partition is a container for one or more root of trust services, and a platform may have multiple secure partitions. Secure partitions provide the execution environment for security functionality.

Platform hardware, such as the Security Attribution Unit (SAU) and Memory Protection Unit (MPU) in the new ARMv8-M platforms, enforces the separation of partitions. Other platforms may use different mechanisms to provide equivalent isolation for the partitions.

#### PSA levels of isolation

If you are prototyping software or using platforms without SAU or MPU, you can choose to have no isolation between the SPE and NSPE (sometimes referred to as Level 0), though the PSA does not specify this. However, for production software, consider implementing one of the following levels of isolation:

* **Level 1 - SPE isolation** In this level, the SPE is fully isolated from the nonsecure application firmware and hardware.
* **Level 2 - Root of Trust isolation** In this level, the SPE is fully isolated from the nonsecure application firmware and hardware and the trusted partitions (secure partitions that implement Root of Trust services) are isolated from other secure partitions.
* **Level 3 - Maximum firmware isolation** In this level, the SPE is fully isolated from the nonsecure application firmware and hardware, and all secure and trusted partitions are individually sandboxed from one another and from the SPM.

### Using secure partitions

Secure partitions are located within the SPE, and must contain at least one set of related security operations (known as a root of trust service) or at least one Interrupt Request (IRQ). You can have multiple root of trust services in a single secure partition.

For a secure partition, you need:

* The **secure partition code**, which must:
  * Be single threaded.
  * Be structured as a loop that waits for inputs.
  * Never exit its loop (considered as a programming error).
  * Be written in C or 'extern "C"' to avoid C++ name mangling.
  * Follow PSA IPC rules. Secure partitions communicate with one another using the IPC API defined in [IPC API](https://github.com/ARMmbed/PSA-IPC-doc/blob/master/IPC_revision.md). All IPC messages must eventually be completed [`call psa_end()`]. Note that the SPM does not schedule IPC messages fairly.
* A **manifest file** in JSON format, that describes the Secure Partition characteristics. The specifications in the manifest file are validated during the build process and at run time.

### Manifest file example

The secure partition manifest file describes the properties of the secure partitions. In this file:

* **entry_point** is the function name of the partition's thread.
* **source_files** is the list of source files containing the partition's code.
* **heap_size** sets the heap size for platforms that have an isolation level of 2 and higher.
* **services** is the list of the partition's root of trust services with their properties.
* **extern_sids** defines a dependency to other Root of Trust Service (referenced by SID). If the manifest does not specify the access between a partition (acting as client) and a root of trust service (acting as server), then the client is not able to send any messages to the root of trust service.

For example:

```json
{
  "name": "BOX_MAIN",
  "type": "APPLICATION-ROT",
  "priority": "NORMAL",
  "id": "0x7BADD00D",
  "entry_point": "main",
  "stack_size": 10,
  "heap_size": "0x0400",
  "mmio_regions": [
    {
      "name": "CMU",
      "permission": "READ-WRITE"
    },
    {
      "name": "MSC",
      "permission": "READ-WRITE"
    },
    {
      "name": "GPIO",
      "permission": "READ-WRITE"
    },
    {
      "name": "TIMER0",
      "permission": "READ-WRITE"
    },
    {
      "name": "UART0",
      "permission": "READ-WRITE"
    },
    {
      "base": "0x10000000",
      "size": "0x1000",
      "permission": "READ-ONLY"
    },
    {
      "base": "0x42000000",
      "size": "0x02000000",
      "permission": "READ-ONLY"
    }
  ],
  "services": [
    {
      "sid": "PSA_TRUSTED_UPDATE",
      "signal": "PSA_TRUSTED_UPDATE",
      "non_secure_clients": true,
      "minor_version": 1,
      "minor_policy": "STRICT"
    }
  ],
  "extern_sids": [
    "PSA_CRYPTO_RSA",
    "PSA_CRYPTO_AES"
  ],
  "source_files": [
    "../source/main.cpp"
  ],
  "irqs": [
    {
      "signal": "MY_IRQ",
      "line_num": 4
    }
  ]
}
```

#### Code example

[Mbed SPM example on GitHub](https://github.com/ARMmbed/mbed-os-example-spm)
