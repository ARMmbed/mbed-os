# How to contribute

This directory structure is entirely generated or copied from other repositories. Do not send patches against it, they cannot be accepted because all code will be entirely overwritten on next release.

Instead, follow these instructions to send and test your contributions against master repositories.

## Directory structure

This directory consists of following modules

* [mbed-mesh-api](#mbed-mesh-api)
* [mbed-trace](#mbed-trace)
* [nanostack-hal-mbed-cmsis-rtos](#nanostack-hal-mbed-cmsis-rtos)
* [nanostack-libservice](#nanostack-libservice)
* [sal-stack-nanostack-eventloop](#sal-stack-nanostack-eventloop)
* [sal-stack-nanostack-private](#sal-stack-nanostack-private)


## mbed-mesh-api

mbed Mesh API is copied from master repository https://github.com/ARMmbed/mbed-mesh-api

To replace the copied version with the master repository, follow these steps:

* Remove the mbed-mesh-api directory: `rm -rf mbed-mesh-api`
* Clone from the master: `git clone git@github.com:ARMmbed/mbed-mesh-api.git`

Now you have the mbed-mesh-api directory replaced with the Git repository cloned from the original. You can build and test your changes against it and send patches normally to Github as a pull requests.

## mbed-trace

mbed-trace library is copied from master repository https://github.com/ARMmbed/mbed-trace

To replace the copied version with the master repository, follow these steps:

* Remove the mbed-mesh-api directory: `rm -rf mbed-trace`
* Clone from the master: `git clone git@github.com:ARMmbed/mbed-trace.git`

## nanostack-hal-mbed-cmsis-rtos

nanostack-hal-mbed-cmsis-rtos library is copied from master repository https://github.com/ARMmbed/nanostack-hal-mbed-cmsis-rtos

To replace the copied version with the master repository, follow these steps:

* Remove the mbed-mesh-api directory: `rm -rf nanostack-hal-mbed-cmsis-rtos`
* Clone from the master: `git clone git@github.com:ARMmbed/nanostack-hal-mbed-cmsis-rtos.git`

## nanostack-libservice

nanostack-libservice library is copied from master repository https://github.com/ARMmbed/nanostack-libservice

To replace the copied version with the master repository, follow these steps:

* Remove the mbed-mesh-api directory: `rm -rf nanostack-libservice`
* Clone from the master: `git clone git@github.com:ARMmbed/nanostack-libservice.git`

## sal-stack-nanostack-eventloop

sal-stack-nanostack-eventloop library is copied from master repository https://github.com/ARMmbed/sal-stack-nanostack-eventloop

To replace the copied version with the master repository, follow these steps:

* Remove the mbed-mesh-api directory: `rm -rf sal-stack-nanostack-eventloop`
* Clone from the master: `git clone git@github.com:ARMmbed/sal-stack-nanostack-eventloop.git`

## sal-stack-nanostack-private

This directory holds binary libraries generated from the Nanostack networking library.

**Only mbed Partners have access to the source code.**

If you have access, the source directory is available in https://github.com/ARMmbed/sal-stack-nanostack-private

You can replace the binary libraries with the source tree as follows:

* Remove the sal-stack-nanostack directory: `rm -rf sal-stack-nanostack`
* Clone the original source repository: `git@github.com:ARMmbed/sal-stack-nanostack-private.git`

Now you can modify, build and test your changes with the mbed OS build.

### Instructions for generating the binary modules

Check `Releasing.md` from the Nanostack source repository.
