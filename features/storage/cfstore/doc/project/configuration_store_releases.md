# Configuration Store Releases
Author: Simon Hughes

# Overview

This page documents the supported requirements in each release. See the [Configuration Store Requirements][CFSTORE_ENGREQ] 
for the definitions of the REQ-xxx reference numbers used in this document.


# Releases

## Release 0.3.2

The requirements status (supported versus not supported) in this release are as follows:

- REQ-1.01-SKVS-R: Not supported. Currently only SRAM back CFSTORE is supported.
- REQ-1.02-SKVS-R: Not supported. uvisor integration is still outstanding.
- REQ-1.03-SKVS-R: Supported.
- REQ-1.04-SKVS-R: Supported.
- REQ-1.05-SKVS-R: Supported.
- REQ-1.06-SKVS-R: Supported.
- REQ-1.07-SKVS-R: CS Stores Binary Blobs:  Supported.
- REQ-1.08-SKVS-R: CS Key Structure as Name Strings: Supported.
- REQ-1.09-SKVS-R: Key Creation and Ownership Part 1: Not supported. uvisor integration is still outstanding.
- REQ-1.10-SKVS-R: Security Context: Not supported. uvisor integration is still outstanding.
- REQ-1.2.01-SKVS-HLD: Not supported. Currently only SRAM back CFSTORE is supported.
- REQ-1.2.02-SKVS-HLD: "only during device activity" supported but other options not supported.
- REQ-1.2.03-SKVS-HLD: Not supported.
- REQ-1.2.04-SKVS-HLD: Not supported.
- REQ-1.2.05-SKVS-HLD: Supported.
- REQ-3.1.01-SKVS-HLAPID-KVS: CS Global Key Namespace: Supported.
- REQ-3.1.02-SKVS-HLAPID-KVS: CS Key Name String Format Max Length: Supported.
- REQ-3.1.03-SKVS-HLAPID-KVS: CS Key Name String Allowed Characters: Supported.
- REQ-3.1.04-SKVS-HLAPID-KVS: Key Name Path Directory Entry List []:  Supported.
- REQ-3.1.05-SKVS-HLAPID-KVS: CS Global Key Yotta Namespace:  Supported.
- REQ-3.2.01-SKVS-HLAPID-KACS: Not supported as requires uvisor integration.
- REQ-3.2.02-SKVS-HLAPID-KACS: CS Key Creation Part 2: Supported.
- REQ-3.2.03-SKVS-HLAPID-KACS: Partially supported as requires uvisor integration.
- REQ-3.2.04-SKVS-HLAPID-KACS: Supported.
- REQ-5.1.01-SKVS-APIL-FK: Key Searching/Finding Scoped by ACL: Supported.
- REQ-5.1.02-SKVS-APIL-FK: CS Global Key Namespace Reserves Character '*': Supported.
- REQ-5.1.03-SKVS-APIL-FK: Key Searching/Finding Resume:  Supported.
- REQ-5.1.04-SKVS-APIL-FK: Key Searching/Finding Internals (key versions): Not supported.
- REQ-5.2.01-SKVS-APIL-GSI: storage_detect: Not supported.
- REQ-5.2.02-SKVS-APIL-GSI: Minimal Storage Support: Supported.
- REQ-5.3.01-SKVS-APIL-COKFW: storage_key_create with O_CREATE: Partially suppported.
- REQ-5.3.02-SKVS-APIL-COKFW: storage_key_create without O_CREATE: Not supported.
- REQ-5.3.03-SKVS-APIL-COKFW: O_CONTINUOUS for executable objects: Not supported.
- REQ-5.3.04-SKVS-APIL-COKFW: O_CONTINUOUS for non-executable objects: Not supported.
- REQ-6.1.01-SKVS-USP: Supported.
- REQ-6.2.01-SKVS-USP-DK: Supported.
- REQ-6.2.02-SKVS-USP-DK: Supported.
- REQ-6.3.01-SKVS-USP-OKFR storage_key_open_read:  Supported.
- REQ-6.4.01-SKVS-USP-SIKV storage_value_rseek: Supported.
- REQ-6.4.02-SKVS-USP-SIKV storage_value_write has no write location: Supported.
- REQ-6.4.03-SKVS-USP-SIKV storage_value_write sequential-access: Supported.
- REQ-6.5.01-SKVS-USP-WK: Partially supported.
- REQ-6.5.01-SKVS-USP-WK: Not supported.
- REQ-6.6.01-SKVS-USP-EKFU: Not supported.
- REQ-6.6.02-SKVS-USP-EKFU: Not supported.
- REQ-7.1.01-SKVS-M: Supported.
- REQ-7.1.02-SKVS-M: Supported.
- REQ-7.1.03-SKVS-M KV Value Data May Not Fit into Available (SRAM) Memory. Not supported. 




[CFSTORE_ENGREQ]: doc/design/configuration_store_requirements.md
