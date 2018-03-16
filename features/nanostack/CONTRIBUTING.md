# How to contribute

This directory structure contains some repositories that are copied from external sources.

Please follow these instructions to send contributions to master repositories.

## Directory structure

This directory consists of following modules

* [coap-service](#coap-service)
* [mbed-mesh-api](#mbed-mesh-api)
* [nanostack-interface](#nanostack-interface)
* [sal-stack-nanostack](#sal-stack-nanostack)

## coap-service

Master repository is located in the https://github.com/ARMmbed/coap-service

Please send contributions against that repository.

To test changes, remove the `coap-service` repository and replace with Git clone
of the master repository.

```
rm -rf coap-service
git clone git@github.com:ARMmbed/coap-service.git
```

## mbed-mesh-api

This is the master source of mbed-mesh-api.
Send contributions directly to this repository.

## nanostack-interface

This is the master source of nanostack-interface.
Send contributions directly to this repository.

## sal-stack-nanostack

This directory holds source code snapshot of the Nanostack networking library. Master repository is available for **mbed Partners** in https://github.com/ARMmbed/sal-stack-nanostack-private. 

If you have access to the master repository, please send contributions against that repository.

To test changes with access to master repository:
 * Remove the sal-stack-nanostack directory: `rm -rf sal-stack-nanostack`
 * Clone the master repository `git clone git@github.com:ARMmbed/sal-stack-nanostack-private.git`as sal-stack-nanostack

Now you can modify, build and test your changes with the mbed OS build.

**NOTE:** You do not need to clone the Nanostack master repository to exactly same location in the build tree. 

If you don't have access to the master repository then you can send contributions directly to this repository.


