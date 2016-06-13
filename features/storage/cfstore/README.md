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
- Comprehensive documentation.


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

* The [CFSTORE Getting Started Guide.][CFSTORE_GETSTART]
* The [CFSTORE Client Example 3 for flash-journal synchronous mode only (simpler code).][CFSTORE_EX3]
* The [CFSTORE Client Example 1 for both flash-journal modes (asynchronous and synchronous)(more complicated but versatile code).][CFSTORE_EX1]
* The CFSTORE Product Requirements were not written.
* The [CFSTORE Engineering Requirements.][CFSTORE_ENGREQ]
* The [CFSTORE High Level Design Document.][CFSTORE_HLD]
* The [CFSTORE Low Level Design Document.][CFSTORE_LLD]
* The [CFSTORE Project Test Plan describing the test methodologies and test cases.][CFSTORE_TESTPLAN]
* The [CFSTORE Release Notes.][CFSTORE_RELEASES]
* The [CFSTORE Jenkins Build and Test Results.][CFSTORE_JENKINS_BT]
* The [CFSTORE Jenkins Code Coverage Results.][CFSTORE_JENKINS_COV]
* The [CFSTORE Project Plan describing milestones and roadmap.][CFSTORE_PROJPLAN]
* The [CFSTORE Project Plan Excel Spreadsheet with estimates and milestones][CFSTORE_PROJXLS]
* The [Flash Abstraction Layer.][FAL]


[CFSTORE_ENGREQ]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_requirements.md
[CFSTORE_EX1]: https://github.com/ARMmbed/configuration-store-example1
[CFSTORE_EX3]: https://github.com/ARMmbed/configuration-store-example3
[CFSTORE_GETSTART]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_getting_started.md
[CFSTORE_HLD]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_hld.md
[CFSTORE_JENKINS_BT]: http://e108747.cambridge.arm.com:8080/job/configuration-store-nightly-build-and-test/
[CFSTORE_JENKINS_COV]: http://e108747.cambridge.arm.com:8080/job/configuration-store-test-coverage/
[CFSTORE_LLD]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_lld.md
[CFSTORE_TESTPLAN]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_test_plan.md
[CFSTORE_PRODREQ]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_product_requirements.md
[CFSTORE_PROJPLAN]:https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_project.md
[CFSTORE_PROJXLS]:https://github.com/ARMmbed/configuration-store/blob/master/doc/project/ARM_MBED_TN_0020_cfstore_project_plan.xlsx
[CFSTORE_RELEASES]:https://github.com/ARMmbed/configuration-store/blob/master/doc/project/configuration_store_releases.md
[CFSTORE_TERM]: https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_terminology.md
[FAL]: https://github.com/ARMmbed/flash-abstraction
