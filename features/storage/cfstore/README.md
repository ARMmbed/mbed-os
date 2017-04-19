# Secure Key-Value Storage #


## Executive Summary

The Configuration Store (CFSTORE) is a secure, 
associative key-value (KV) store C-Language Hardware Abstraction Layer. 
CFSTORE provides the secure and persistent storage for:
- Storing encryption keys data.
- Storing configuration data.
- Storing firmware, firmware updates and incremental firmware blocks for assembling into a firmware update.

These services are presented to clients with:
- A conceptually simple, file-like interface for storing and managing data using (key, value) pairs in 
  persistent storage media. 
- A simple, hardware-independent API to promote portability across multiple platforms and a low attack surface. 
- A very small code/memory footprint so CFSTORE is capable of running on highly-constrained memory systems (~10kB free memory)
  where typically available SRAM << NV storage. 
- A simple (low complexity) storage capability at the expense of features. For example, CFSTORE only supports the storage of 
  binary blobs rather than a rich set of data types.  

Current support includes:
- NV-backed support. Integration with Flash Abstraction (Flash Journal Strategy Sequential) for persistent storage on the Freescale FRDM K64F target.
- SRAM backed support. 
- More than 60 test cases with >80% test coverage.
- Comprehensive documentation including doxygen generated API and test case documentation. 


# Configuration-Store Software Architecture

```C
    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     
    |  Configuration Store Client   |     
    |  e.g. FOTA                    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Configuration Store          |     |  uvisor                       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     |                               |
                                          |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     |                               |
    |  Flash Abstraction Layer      |     |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     |                               |
                                          |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     |                               |
    |  Flash Driver Layer           |     |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    SW
    -----------------------------------------------------------------------
    HW

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  NV Storage Media e.g. Flash  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    Configuration Store Software Architecture

```

The above figure shows the following entities (from top to bottom):
- A Configuration Store client e.g. FOTA.
- Configuration Store, the associative KV pair store.
- Flash Abstraction Layer, portable across the driver layer.
- Flash Driver layer e.g. CMSIS-Driver.
- NV Storage Media. These are the physical storage media. 


# Providing Feedback

If you would like to make a contribution to CFSTORE, please provide feedback/designs/comments/code in one of the following ways:
- By logging an issue in the CFSTORE repo.
- By submitting a Pull Request to the CFSTORE repo.
- By sending an email to:
-- simon.hughes@arm.com 
-- milosch.meriac@arm.com
  

# Further Reading

* The [CFSTORE Getting Started Guide][CFSTORE_GETSTART] including examples.
* The [CFSTORE Engineering Requirements.][CFSTORE_ENGREQ]
* The [CFSTORE High Level Design Document.][CFSTORE_HLD]
* The [CFSTORE Low Level Design Document.][CFSTORE_LLD]
* The [CFSTORE Test Plan.][CFSTORE_TESTPLAN]
* The [CFSTORE Application Note 0001: NXP Freescale Kinetis FRDM-K64F Flash Memory Usage.][CFSTORE_APPNOTE_0001]
* The [CFSTORE Releases][CFSTORE_RELEASE] provides requirements tracking by listing the requirements supported for a CFSTORE version.


20160714

[CFSTORE_ENGREQ]: doc/design/configuration_store_requirements.md
[CFSTORE_EX1]: ../TESTS/cfstore/example1/example1.cpp
[CFSTORE_EX3]: ../TESTS/cfstore/example3/example3.cpp
[CFSTORE_GETSTART]: doc/design/configuration_store_getting_started.md
[CFSTORE_HLD]: doc/design/configuration_store_hld.md
[CFSTORE_LLD]: doc/design/configuration_store_lld.md
[CFSTORE_TESTPLAN]: doc/design/configuration_store_test_plan.md
[CFSTORE_PROJPLAN]:doc/design/configuration_store_project.md
[CFSTORE_RELEASE]: doc/project/configuration_store_releases.md
[CFSTORE_TERM]: doc/design/configuration_store_terminology.md
[CFSTORE_APPNOTE_0001]: doc/design/configuration_store_app_note_0001.md