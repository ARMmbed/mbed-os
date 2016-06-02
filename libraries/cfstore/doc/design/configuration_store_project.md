# Configuration Store Project Documentation With Project Milestones


# Planning Estimates

Please see the following: 

* The [CFSTORE Project Plan Excel Spreadsheet with estimates and milestones][CFSTORE_PROJXLS]


# Milestone 1 (MS1): SRAM-backed CFSTORE

## Summary
This milestone is intended to give developers:
- CFSTORE API implementation backed by SRAM (non-persistent) in area_0.
- Synchronous implementation.
- Supported on the K64F target.
- Some mbed greentea test demonstrating how to use the API.
- High Level Design Document.
- Alpha grade code quality (testing giving ~50% code coverage).
- area_0 SRAM initialisation using target.json generated CFSTORE_SRAM_START_ADDR & CFSTORE_SRAM_SIZE #defines  rather than malloc(). 

Estimated Delivery Date: 28 Feb 2016
Status: Delivered

## Features Included in this Milestone

See above.

## Features Not Included in this Milestone

This milestone does not including the following:
- Integration with the Flash Abstraction Layer.
- Integration with uVisor.


# Milestone 2 (MS2): CFSTORE Flash Ping-Pong Alpha (Feature Complete)
- CFSTORE API implementation backed:
  -- area_0 SRAM (non-persistent) e.g. version i+1 of KVs.
  -- area_1 NV storage (persistent e.g. version i of area_0 KVs).
  -- area_2 NV storage (persistent) e.g. version i-1 of area_0 KVs).
- Asynchronous implementation
- Supported on the K64F target.
- High Level Design Document.
- Alpha grade code quality (testing giving ~50% code coverage).

Estimated Delivery Date: 31 Mar 2016
Status: Delivered


# Milestone 2.1 (MS2.1): CFSTORE Flash Ping-Pong Beta
- CFSTORE extend testing to ~90% code coverage
 
Estimated Delivery Date: 30 Apr 2016
Status: Delivered


# Milestone 3.0 (MS3): CFSTORE(SRAM)+Morpheus
- CFSTORE v0.2.4 SRAM version ported to Morpheus

Estimated Delivery Date: 31 May 2016
Status: In-progress
 

# Milestone 3.1 (MS3.1): CFSTORE(Flash-Journal)+Morpheus
- CFSTORE v0.2.4 with Flash-Journal integration ported to Morpheus

Estimated Delivery Date: 15 June 2016
Status: In-progress
 

# Milestone 4 (MS4): CFSTORE+uvisor+Morpheus
- CFSTORE v0.2.4 version ported to Morpheus
- SRAM version working
- Flash integration working
- uvisor integration working

Estimated Delivery Date: 30 June 2016
Status: In-progress
 

# Release Branching and Labels
 
<p align="center">
  <img src="https://github.com/ARMmbed/configuration-store/blob/master/doc/project/pics/cfstore_release_branch_tags.jpg"/>
</p>

This section describes the release policy of the CFSTORE project.

- The above picture shows the CFSTORE release branches and the associated labels.
- The master branch contains the lastest development version of CFSTORE. 
- The MBED_V_0_1_0 branch contains the Milestone 1 (MS1): SRAM-backed CFSTORE. This version is published in the yotta
  private registry as version 0.1.0.
- The MBED_V_0_2_0 branch contains the Milestone 2 (MS2): : CFSTORE Flash Ping-Pong Alpha (Feature Complete). This version is published in the yotta
  private registry as version 0.2.0.
   
## General Notes on Branch Software

- CFSTORE policy is that master always builds correctly without warnings for the supported targets:
  - frdm-k64f-gcc
  - x86-linux-native.
- CFSTORE policy is that master tests always run successfully for the supported targets.

# Guidance for Submitting Issues

Here are the guidelines for submitting issues to the configuration-store repository:
- If you have an issue with configuration-store then please file an issue at github.com/armmbed/configuration-store.git
- The issue should be filed with a stand-alone testcase or sample application that only depends on configuration-store and shows the fault on the target platform. 
- The issue should include the following information under separate SUMMARY and DETAILS headings:

SUMMARY
=======
APPLCIATION/Library NAME: 	<e.g. configuration_store, example3, myappname> 
OS VERSION: 				<e.g. MBEDOSv3>
TARGET:						<e.g. k64f>
TOOLCHAIN:					<e.g. GCC_ARM, ARM>
DESCRIPTION:				<e.g. a 1-line summary description of the problem >
MODULE DEPENDENCIES:		<e.g. the version of the modules used to build the application, output from yotta ls, etc>


DETAILS
=======
<step by step guide about how to build and test the provided resources so as to reproduce the problem. 


CODE
====
<details of supplied code sample showing problem>



# Unplanned Features

There are currently no unplanned features

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


[CFSTORE_PROJXLS]:https://github.com/ARMmbed/configuration-store/blob/master/doc/project/ARM_MBED_TN_0020_cfstore_project_plan.xlsx
