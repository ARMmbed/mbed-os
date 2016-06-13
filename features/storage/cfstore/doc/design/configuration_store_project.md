# Configuration Store Project Documentation



# Feature 1 (FT1): SRAM-backed CFSTORE (Complete)
This feature includes the following:

- CFSTORE API implementation backed by SRAM (non-persistent).
- Synchronous mode implementation.
- Supported on the K64F target.
- mbed greentea tests demonstrating how to use the API.
- High Level Design Document present.
- Alpha grade code quality (testing giving ~50% code coverage).
- area_0 SRAM initialisation using target.json generated CFSTORE_SRAM_START_ADDR & CFSTORE_SRAM_SIZE #defines  rather than malloc(). 


## Features Not Included in this Feature (Complete)

This feature does not including the following:
- Integration with the Flash Abstraction Layer.
- Integration with uVisor.


# Feature 2: CFSTORE Flash Ping-Pong Alpha (Complete)
- CFSTORE API implementation backed by SRAM and Non-Volatile (NV) storage.
- Flash-jounral asynchronous mode support.
- Flash-jounral synchronous mode support.
- Flash-jounral supported on the K64F target.
- High Level Design Document present.


# Feature 3.0: CFSTORE+SRAM with mbedOS Q2 Release (Complete)
- CFSTORE SRAM version ported to mbedOS Q2 Release.


# Feature 3.1: CFSTORE + Flash-Journal + mbedOS Q2 Release.
- CFSTORE with Flash-Journal integration ported to mbedOS Q2 Release.

# Feature 4: CFSTORE + uvisor + mbedOS Q2 Release
- CFSTORE with Flash-Journal integration ported to mbedOS Q2 Release.
- uvisor integration working


# Guidance for Submitting Issues

Here are the guidelines for submitting issues to the configuration-store repository:

- If you have an issue with configuration-store then please file an issue at github.com/armmbed/configuration-store.git
- The issue should be filed with a stand-alone testcase or sample application that only depends on configuration-store and shows the fault on the target platform. 
- The issue should include the following information under separate SUMMARY and DETAILS headings:

```
    
    SUMMARY
    =======
    APPLCIATION/Library NAME:     <e.g. configuration_store, example3, myappname> 
    OS VERSION:                 <e.g. MBEDOSv3>
    TARGET:                        <e.g. k64f>
    TOOLCHAIN:                    <e.g. GCC_ARM, ARM>
    DESCRIPTION:                <e.g. a 1-line summary description of the problem >
    MODULE DEPENDENCIES:        <e.g. the version of the modules used to build the application, output from yotta ls, etc>
    
    DETAILS
    =======
    <step by step guide about how to build and test the provided resources so as to reproduce the problem. 
    
    CODE
    ====
    <details of supplied code sample showing problem>
```



# References 

## Useful Documents

- [KeyValueStorage.md](https://github.com/ARMmbed/device-security/blob/master/KeyValueStorage.md)
- [uVisor README.md](https://github.com/ARMmbed/uvisor/blob/master/README.md)
- [FlashAbstraction](https://github.com/ARMmbed/device-security/blob/master/FlashAbstraction.md)


## Useful GitHub Repositories

- [device-security](https://github.com/ARMmbed/device-security)


## Useful References

- [mbed Coding Standard](https://developer.mbed.org/teams/SDK-Development/wiki/mbed-sdk-coding-style)
- [Security Ref](http://resources.sei.cmu.edu/asset_files/BookChapter/2005_009_001_52692.pdf)
- [Security Ref](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1255.pdf)
 

## Useful Jira Tickets

- [Configuration Store for mbed OS IOT Product Requirements IOTPREQ-334](http://jira.arm.com/browse/IOTPREQ-334)


