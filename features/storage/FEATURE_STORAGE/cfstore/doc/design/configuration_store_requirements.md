# Configuration Store Engineering Requirements

## Definition of Terms

The [CFSTORE Terminology][CFSTORE_TERM] defines terms used in this document. 

## Design Goals

A number of design goals should be met for the configuration store to be used widely. The configuration store should:

* Offer hierarchical storage of data:
    * Configuration data should be groupable.
    * Groups of configuration data should be handled as a unit.
* Support multiple degrees of persistence:
    * Permanent.
    * Non-Volatile.
    * Volatile.
* Allow configuration to be superceded different persistence levels (e.g. volatile being the highest priority and permanent the lowest).
* Support multiple sources of configuration:
    * Build configuration tools.
    * Application configuration.
    * Runtime configuration including remote configuration from a network management entity.
* Support multiple kinds of values:
    * Integer.
    * Floating point.
    * String
    * Arbitrary binary blob.
* Globally accessible by any module with the system.
* Able to enforce permissions on entire branches of the configuration data.
* Accessible early in the system initialisation process.
* Low memory footprint.
* Low CPU overhead for data accesses.


## Secure Key Value Storage\Rationale (REQ-1.xx-SKVS-R) Requirements

#### REQ-1.01-SKVS-R:
The CFSTORE must provide a non-volatile, hardware-independent secure storage
service that can be used by CFSTORE API clients for storing data owned
by a client security context (uvisor box).

#### REQ-1.02-SKVS-R:
Clients of the CFSTORE API must be security contexts (boxes) so that security
policies can be enforced.

#### REQ-1.03-SKVS-R:
The CFSTORE design values simplicity over complexity to promote security by
minimizing the attack surface to secure data.

#### REQ-1.04-SKVS-R:
The CFSTORE must be implemented as a uvisor secure box.

#### REQ-1.05-SKVS-R:
The CFSTORE must support the use of Access Control Lists for policing access to
secure data.

#### REQ-1.06-SKVS-R:
The CFSTORE must support the storage of a {key, value} tuples where
- the key is a zero terminated string of arbitrary length.
- the value is a binary blob of data of arbitrary length

The {key, value} tuple is a CFSTORE object.

##### Discussion
A null key (i.e. "" which includes a terminating null) is not a valid because the null string is not a valid security_prefix_name. 

#### REQ-1.07-SKVS-R: CFSTORE Stores Binary Blobs
The CFSTORE must provide support so that other (client) components can implement
more complex storage types other than a binary blob e.g. by
wrapping the underlying CFSTORE {key, value} tuple with additional
type information.

#### REQ-1.08-SKVS-R: CFSTORE Key Structure as Name Strings
This requirement has been removed as it is a subset of REQ-1.06-SKVS-R [REQ-1.06-SKVS-R]. 

#### REQ-1.09-SKVS-R: Key Creation and Ownership Part 1
CFSTORE keys must be owned by the security context. A security context (box)
is identified by a unique security_name_prefix and this identifier
must be used when the key is created. CFSTORE must support the use of the
security_name_prefix as the leading substring of the CFSTORE key string, in which
case the key_name_prefix is the security_name_prefix and identifies the
owner of the key.

#### REQ-1.10-SKVS-R: Security Context
In order to create objects in the CFSTORE, a security context (box) must have
a security_name_prefix.

#### REQ-1.11-SKVS-R: CFSTORE object
This requirement has been removed as it is a duplicate of REQ-1.10-SKVS-R 

## Secure Key Value Storage High Level Design (REQ-1.2.xx-SKVS-HLD) Requirements

#### REQ-1.2.01-SKVS-HLD:
The CFSTORE must be able to detect the available types of storage media
in the system and report associated storage media attributes.

#### REQ-1.2.02-SKVS-HLD:
The CFSTORE may report the following storage media data retention level attribute,
when available:
- only during device activity
- during sleep
- during deep-sleep
- battery-backed, device can be powered off
- internal non-volatile memory
- external non-volatile memory

#### REQ-1.2.03-SKVS-HLD:
For a particular storage medium, the CFSTORE may report the following device
data security protection features, when available:
- no security, just safety
- write-once-read-only-memory (WORM)
- against internal software attacks using ACLs
- roll-back protection
- immovable (for internal memory mapping, to stop relocated block to move).
  This attribute must only be set provided:
  - the device memory is mapped into the CPU address space
  - access is only granted to specific CFSTORE API system clients e.g. FOTA,
     DMA.
- hardening against device software (malware running on the device)
- hardening against board level attacks (debug probes, copy protection
  fuses)
- hardening against chip level attacks (tamper-protection)
- hardening against side channel attacks
- tamper-proof memory (will be deleted on tamper-attempts using board level
  or chip level sensors)

#### REQ-1.2.04-SKVS-HLD:
The CFSTORE may be used to implement KV storage protection services
(e.g. flash image roll-back protection, confidentiality) for off-chip
(external) storage media (e.g. SPI/I2C/NAND Flash).

#### REQ-1.2.05-SKVS-HLD:
The device data security protection immovable attribute may only be set


## Secure Key Value Storage\High Level API Description\Key Value Storage (REQ-3.1.xx-SKVS-HLAPID-KVS) Requirements.

#### REQ-3.1.01-SKVS-HLAPID-KVS: CFSTORE Global Key Namespace
The CFSTORE must implement a system global hierarchical tree name-space for
keys. The namespace is shared by all software components operating within
the system. Examples of key names include the following:
 - 'com.arm.mbed.wifi.accesspoint[5].essid'
 - 'com.arm.mbed.wifi.accesspoint[home].essid'
 - 'yotta.your-yotta-registry-module-name.your-value'
 - 'yotta.hello-world.animal{dog}{foot}[3]'
 The key name string forms a path where 'Path Directory Entries' are
 separated by the '.' character.

#### REQ-3.1.02-SKVS-HLAPID-KVS: CFSTORE Key Name String Format Max Length
For CFSTORE keys The maximum length of a CFSTORE key is 220 characters excluding
the terminating null.

#### REQ-3.1.03-SKVS-HLAPID-KVS: CFSTORE Key Name String Allowed Characters
CFSTORE key name strings must only contain the following characters:
- [a-zA-Z0-9.]
- '-'

#### REQ-3.1.04-SKVS-HLAPID-KVS: Key Name Path Directory Entry List {}
Path Directory Entries may have list indicators designated by {}. For
example,
 - 'com.arm.mbed.wifi.accesspoint{5}.essid'
 - 'com.arm.mbed.wifi.accesspoint{home}.essid'
 - 'yotta.hello-world.animal{dog}{foot}{3}'
In the above the list item specifiers are respectively:
 - '5'
 - 'home'
 - 'dog', 'foot, '3'
As list item specifiers are part of the key name string, the list item
substring must be composed of allowable characters.

#### REQ-3.1.05-SKVS-HLAPID-KVS: CFSTORE Global Key Yotta Namespace
The key name prefix 'yotta' is reserved for use by the yotta module. Other
prefixes may be reserved.

#### REQ-3.1.06-SKVS-HLAPID-KVS: CFSTORE Key Names Mapable to Common OS Filesystem Name
The CFSTORE key names must be mappable to common OS Filesystem file names. This requirment 
permits a CFSTORE backend to map individual KVs to files with the filename being the 
key name, for example. Supported OS Filesytems must include DOS, NTFS, EXT3, EXT4 and
JFFS. 


## Secure Key Value Storage\High Level API Description\Access Control Security (REQ-3.2.xx-SKVS-HLAPID-ACS) Requirements.

#### REQ-3.2.01-SKVS-HLAPID-KACS:
The CFSTORE must enforce security policies as defined by Access Control Lists.

#### REQ-3.2.02-SKVS-HLAPID-KACS: CFSTORE Key Creation Part 2
The CFSTORE objects must be created with an ACL. The ACL is attached to the object
so that access permissions to the KV data can be enforced.

#### REQ-3.2.03-SKVS-HLAPID-KACS:
The CFSTORE Access Control Lists must support the groups for 'owner' and 'other',
with optional permissions read, write and executable. The owner group
permissions describe the access permissions of the owner of the object. The
other group permissions describe the access permissions for entities other
than the owner. The writable and executable permissions are mutually
exclusive.

#### REQ-3.2.04-SKVS-HLAPID-KACS:
A CFSTORE API client must be able to query the CFSTORE for a list of KV pairs provided
the KV pair ACL permissions allow the client access. The query result must
contain all client owner KVs. The query results may include non-client
owned KVs provided the other group permissions grant access.


## Secure Key Value Storage\API Logic\Finding Keys (REQ-5.1.xx-SKVS-APIL-FK) Requirements.

#### REQ-5.1.01-SKVS-APIL-FK: Key Searching/Finding Scoped by ACL
The CFSTORE must provide an interface to query for active keys in the global
storage. The query must:
- return results based on the ACL provided by the client
- support wild card searches using the '*' character. The '*' character
  can occur at most once any point in the search string. For example:
  - com.arm.mbed.wifi.accesspoint*.essid
  - yotta.your-yotta-registry-module-name.*
  - yotta.hello-world.animal{dog}{foot}{*}
  - yotta.hello-world.animal{dog}{foot}*
  - yotta.hello-world.animal{dog*3}

#### REQ-5.1.02-SKVS-APIL-FK: CFSTORE Global Key Namespace Reserves Character '*'
The character '*' is reserved in the CFSTORE global key namespace. The current
functions of this character as follows:
- a wild card character in searches.
- a wild card character used in the delete operation.


#### REQ-5.1.03-SKVS-APIL-FK: Key Searching/Finding Resume
In order to support the return of a large list of key query results (perhaps
exceeding the ability of the caller to consume in a single operation), the
query interface must support the ability to restart/resume the query to
retrieve a subsequent set of records to those already received.

#### REQ-5.1.04-SKVS-APIL-FK: Key Searching/Finding Internals (key versions)
The CFSTORE must be robust against incomplete, corrupted or aborted write 
operations to NV store caused for example, by loss of power during the 
write. 


## Secure Key Value Storage\API Logic\Get Storage Information (REQ-5.2.xx-SKVS-APIL-GSI) Requirements.

#### REQ-5.2.01-SKVS-APIL-GSI: storage_detect
The CFSTORE must provide an API so that clients can discover the CFSTORE storage
capabilities. Storage capabilities may include:
- write-block sizes for O_BLOCK_WRITE mode (sequence of writes into the
  same value)
- supported Data Retention Levels
- supported Device Data Security Protection Features

#### REQ-5.2.02-SKVS-APIL-GSI: Minimal Storage Support
The CFSTORE must provide minimal storage media services including the following:
- SRAM/SDRAM memory with no security guarantees.


## Secure Key Value Storage\API Logic\Creating & Opening Keys for Writing (REQ-5.3.xx-SKVS-APIL-COKFW) Requirements.

#### REQ-5.3.01-SKVS-APIL-COKFW: storage_key_create with O_CREATE
CFSTORE keys must be explicitly created using 'storage_key_create' with the following
parameters:
- security ACLs (owner & others)
- the intended retention levels (bit mask to allow caching if needed)
- indicating the expected Device Data Security Protection Features
- the key pointer (zero-terminated)
- the value size
- alignment bits of the value hardware address (only for O_CONTINUOUS).
  The structure start is aligned accordingly to ensure that the value
  blob is aligned on a multiple of the 2^alignment_bits
- mode flags (O_CREATE, O_CONTINUOUS, O_LAZY_FLUSH, O_BLOCK_WRITE,
  O_ALLOCATE_AT_OFFEST).
  - O_CREATE. The call will create the KV pair. If a
     pre-existing KV with the same name is present in CFSTORE then the
     storage_key_create will fail with FILE_EXISTS.
  - O_CONTINUOUS. The KV value will be stored in a continuous range
     of hardware addresses.
  - O_LAZY_FLUSH 
  - O_BLOCK_WRITE
  - O_ALLOCATE_AT_OFFEST 

##### Discussion
The following should not be included in the API (e.g. because the principle of encapsulation is broken, or specific to a particular clients requirements):
- optionally the offset address (restricted feature, ideally only granted
  to the FOTA security context)

#### REQ-5.3.02-SKVS-APIL-COKFW: storage_key_create without O_CREATE
Pre-existing CFSTORE objects can be updated by calling the storage_key_create
API with the O_CREATE not set.
In case a pre-existing key is updated (O_CREATE not set) and the previous
ACL allows writing to the caller:
- all key-value fragments of the previous key are set as inactive
- the new key is allocated (in fragments if permitted)
- all permissions and settings are copied from the previous key
- the version number is incremented compared to the previous key (see 
  REQ-5.3.05-SKVS-APIL-COKFW). 

#### REQ-5.3.03-SKVS-APIL-COKFW: O_CONTINUOUS for executable objects
CFSTORE will manage an executable KV as though the mode O_CONTINUOUS flag
is set.

#### REQ-5.3.04-SKVS-APIL-COKFW: O_CONTINUOUS for non-executable objects
A CFSTORE client may specify the mode O_CONTINUOUS flag for non-executable
objects.

#### REQ-5.3.05-SKVS-APIL-COKFW: Versioning of KVs
KVs in NV storage should have version numbers. When writing an updated 
KV (with changed value data for example), a new copy of the KV data with
updated version number should be written to NV store. The earlier version is 
left as a fallback copy of data that may be deleted at some point in time,
when the NV storage is required, or more than a certain number of 
versions is exceeded. 

## Secure Key Value Storage\Updating and Settings and Permissions (REQ-6.1.xx-SKVS-USP) Requirements.

#### REQ-6.1.01-SKVS-USP:
CFSTORE does not permit the updating of KV pair permissions or settings. This is to promote security.


## Secure Key Value Storage\Updating and Settings and Permissions\Deleting Keys (REQ-6.2.xx-SKVS-USP-DK) Requirements.

#### REQ-6.2.01-SKVS-USP-DK:
Only the owner of the CFSTORE KV pair can delete the object. The wildcard
'*' character can be specified to delete an owned subtree of the CFSTORE
global key namespace.

#### REQ-6.2.02-SKVS-USP-DK:
This requirement has been removed.

## Secure Key Value Storage\Updating and Settings and Permissions\Opening Keys for Reading (REQ-6.2.xx-SKVS-USP-OKFR) Requirements.

#### REQ-6.3.xx-SKVS-USP-OKFR storage_key_open_read
CFSTORE objects must be explicitly opened with storage_key_open_read(key_name)
before operations on the KV pair can be performed. The KV must
pre-exist in the store before it can be opened.


## Secure Key Value Storage\Updating and Settings and Permissions\Seeking in Key Values (REQ-6.3.xx-SKVS-USP-SIKV) Requirements.

#### REQ-6.4.01-SKVS-USP-SIKV storage_value_rseek
The function storage_value_rseek can be used on a opaque reference to a KV
to change the read position inside a value i.e. the storage_value_read
method supports random-access.  

#### REQ-6.4.02-SKVS-USP-SIKV storage_value_write has no write location
storage_value_write does not support the concept of a write location that can be modified i.e. random access write support is not supported. 

#### REQ-6.4.03-SKVS-USP-SIKV storage_value_write sequential-access
storage_value_write supports sequential-access. Random-access to the value data is not supported.


## Secure Key Value Storage\Updating and Settings and Permissions\Writing Keys (REQ-6.4.xx-SKVS-USP-WK) Requirements.

#### REQ-6.5.01-SKVS-USP-WK
CFSTORE KV values can be written in one or more operation. For example, if a value blob has size n bytes, then 2 n/2 byte write operations have the following effect:
- the first write sets bytes 0-n/2-1 in the value blob.
- the second write sets bytes n/2 to n-1 in the value blob.
This may require a write position to be maintained, for example. The ability to seek the write location must not be supported.

#### REQ-6.5.02-SKVS-USP-WK
This requirement has been removed.

#### REQ-6.5.03-SKVS-USP-WK
The KV stored in NV store should have an associated CRC so that: 
- the integrity of the data can be protected
- the corruption of the data can be detected e.g. if power is lost during the flush to NV store. 
The CRC should be stored at the end of the KV so it is written last. The CRC write transaction is termed "finalising" the KV.  

#### REQ-6.5.04-SKVS-USP-WK
KVs that have not been flushed to NV storage should be flushed at reboot time. 

#### REQ-6.5.05-SKVS-USP-WK
KVs may be fragmented into several smaller pieces for NV storage. In the case that a KV is fragmented, each fragment should have a an associated CRC as per REQ-6.5.03-SKVS-USP-WK.  

#### REQ-6.5.06-SKVS-USP-WK
A KV value or KV fragment can only be finalised by its owner.  

#### REQ-6.5.07-SKVS-USP-WK
Closing of a written KV in the finalization of all open value-fragment for that KV.  

#### REQ-6.5.08-SKVS-USP-WK
Flushing of the KVs causes finalisation of the store.

#### REQ-6.5.09-SKVS-USP-WK
Non-finalised object must not be readable.


## Secure Key Value Storage\Updating and Settings and Permissions\Executable Keys and Firmware Updates (REQ-6.6.xx-SKVS-USP-EKFU)
Requirements.

#### REQ-6.6.01-SKVS-USP-EKFU
To facilitate modular firmware updates in future, executable keys are supported by the API 

#### REQ-6.6.02-SKVS-USP-EKFU
For immovable & O_CONTINUOUS keys, the absolute hardware address and the relative address of a value key can be queried using the API
This API feature is restricted to boxes that require that function (FOTA, DMA). By keeping most key-value pairs movable the flash can be de-fragmented.


## Secure Key Value Storage\Miscellaneous (REQ-7.1.xx-SKVS-M)Requirements.

#### REQ-7.1.01-SKVS-M
The CFSTORE will implement a C Language interface.

#### REQ-7.1.02-SKVS-M
The CFSTORE does not support hot-pluggable storage devices e.g. SD flash. 

#### REQ-7.1.03-SKVS-M KV Value Data May Not Fit into Available (SRAM) Memory. 
CFSTORE must be capable of writing a KV to NV backing store where the value
data length >> size of available SRAM. This requirement therefore implies 
that CFSTORE must support the case that the whole of the KV value data cannot all be
resident in SRAM memory at one time.  


# Outstanding Issues With This Document

#### REQ-5.3.01-SKVS-APIL-COKFW: storage_key_create with O_CREATE
- How does the offset-address work?
- What is the definition of O_LAZY_FLUSH 
- What is the definition of O_BLOCK_WRITE 
- What is the definition of O_ALLOCATE_AT_OFFEST 

#### REQ-5.3.02-SKVS-APIL-COKFW: storage_key_create without O_CREATE
- To which "previous ACL" does this requirement refer?
- Is an implementation internally creating a new copy of the key?

#### REQ-6.5.01-SKVS-USP-WK
- Are there any additional requirements arising from these statements?


# Contributors

This document was made possible through the contributions of the following people:
- Rohit Grover
- Simon Hughes
- Milosch Meriac


[CFSTORE_TERM]: doc/design/configuration_store_terminology.md
