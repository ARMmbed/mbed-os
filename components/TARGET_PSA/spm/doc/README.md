# Mbed Secure Partition Manager (SPM)

The Platform Security Architecture (PSA) firmware framework specifications contain a logic component called the Secure Partition Manager (SPM). PSA defines a Secure Processing Environment (SPE) for:

* Sensitive data, such as keys, credentials and firmware.
* The code that manages it.
* Its trusted hardware resources.

The PSA SPM interfaces decouple components, allowing reuse of components in other device platform and helps to reduce an integration effort.

Mbed SPM is an implementation of PSA SPM, which:

* Secures low cost IoT devices, where a full Trusted Execution Environment (TEE) would not be appropriate.
* Protects sensitive assets (keys, credentials and firmware) by separating these from the application firmware and hardware.
* Is architecture agnostic and can be implemented on different Arm Cortex&reg;-M architectures, offering variable level of protection, based on platform resources.

![diagram](png/PSA-standardized-Interfaces-diagram.png)

## Further reading

* The [introduction to PSA SPM](INTRO.md) provides more details about PSA SPM.
* Visit the official Arm Platform Security Architecture web page https://pages.arm.com/psa-resources
* Trusted firmware presentation during the Linaro Connect event by James King on IOT http://connect.linaro.org/resource/hkg18/hkg18-212/
