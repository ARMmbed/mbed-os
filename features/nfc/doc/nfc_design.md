#   Introduction
##  Overview and Background
NFC offers a simple and secure way of commissioning IoT devices in the field, and we are seeing increasing demand for this from prospective customers. We have a plan to introduce NFC into Mbed OS, this is the first phase to add a reference implementation of card emulation mode.

NFC offers three modes;
1. NFC card emulation
2. NFC reader/writer
3. NFC peer to peer

To support new use cases such as commissioning, BLE pairing and identification/authentication of NFC enabled IoT endpoints, Mbed OS should support the card emulation mode. 

However the architecture should be future-proofed and should also be extendable to support other NFC modes in the future.

## Use cases
### Commissioning

NFC is a great medium to support commissioning requirements.

####	Identification

An NDEF message can be used to carry a device's unique identifier to ease identification before handing over to another transport medium such as BLE.

####	Transport

If the NFC controller can emulate a smartcard, no handover is necessary and the full commissioning flow can happen over NFC.

###	BLE Pairing

A specifically crafted NDEF message can be used to facilitate out-of-band pairing wth man-in-the-middle protection as specified in the [Bluetooth® Secure Simple Pairing Using NFC](https://members.nfc-forum.org/apps/group_public/download.php/18688/NFCForum-AD-BTSSP_1_1.pdf) document.

#   System Architecture and High-Level Design

##  Compliance with NFC Forum Specifications
The NFC Forum is one the bodies producing NFC standards. Most smartphones supporting NFC today are compliant with the NFC Forum's specifications. In that consideration the NFC component in mbed OS should map with the relevant standards from the NFC Forum, and NFC Forum terminology should be used where possible.

##	User-facing API
The NFC API exposed to the user should provide high-level, object-oriented C++ APIs for the following:
* Starting/Stopping a discovery loop
* Listing wired targets (NFC EEPROMs)
* Exchanging NDEF messages with an initiator or a wired target
* Emulate ISO7816-4 applications if supported

##  Phase 1: MicroNFC stack integration
The first step towards integrating NFC in mbed OS is the integration of the MicroNFC stack which has drivers for the PN512 and derivatives.

Architecture:

![phase_1_architecture]

##	Phase 2: NFC Host/Controller split, NCI and NFC HAL API
At the moment the MicroNFC stack is split into two components:
* Applications protocols and upper stack
* Transceiver-specific polling loop an drivers

In order to match more closely with the NFC Forum standard and add a well-defined way for partners to add support for their transceivers, we will amend that split to be compliant with the NFC Forum's NCI (NFC Communication Interface) protocol.

The generic part of the controller stack will be clearly separated so that partners can make use of it if they wish (approach 1).

For NFC controllers natively supporting the NCI protocol, partners would only have to write a transport driver (approach 2).

![phase_2_architecture]

Examples of NCI-compliant controllers:
* ST ST21NFC
* NXP PN7120 & PN7150

Examples of non NCI-compliant transceivers:
* NXP PN512
* NXP PN5180
* AMS AS395x series

# Detailed Design

##	User-facing APIs

The following APIs are designed with the following principles in mind:
*   Abstracting the underlying complexities of NFC from the user
*   Offering a high-level C++ object-oriented API to the user
*   Ensuring compliance with the NFC Forum's standards and terminology
*   Ensuring consistency with the mbed OS codebase

Class Diagram:

###  NFC Controller

![nfc_controller_diagram]

The `NFCController` class is the entrypoint into NFC for the user.

When NCI integration is complete (phase 2), this class will have to be provided with a `NCIDriver` instance.

It offers the following methods:

```cpp
struct nfc_rf_protocols_bitmask_t
{
    uint8_t initiator_t1t : 1;
    uint8_t initiator_t2t : 1;
    uint8_t initiator_t3t : 1;
    uint8_t initiator_iso_dep : 1;
    uint8_t initiator_nfc_dep : 1;
    uint8_t initiator_t5t : 1;

    uint8_t target_t1t : 1;
    uint8_t target_t2t : 1;
    uint8_t target_t3t : 1;
    uint8_t target_iso_dep : 1;
    uint8_t target_nfc_dep : 1;
    uint8_t target_t5t : 1;
};

nfc_rf_protocols_bitmask_t get_supported_rf_protocols() const;
```
Retrieve the list of supported RF protocols.
These are mapped against NFC Forum-defined protocols.

* T1T is based on ISO/IEC 14443A-3 and commonly known as Topaz (Innovision). 
* T2T is based on ISO/IEC 14443A-3 and commonly known as Mifare Ultralight/NTAG (NXP).
* T3T is based on JIS X6319-4, also known as Felica (Sony).
* ISO-DEP is based on ISO/IEC 14443-4 and is the common interface for contactless smartcards. The underlying radio protocol can either be ISO/IEC 14443A or ISO/IEC 14443B.
* NFC-DEP is based on ISO/IEC 18092 / FIXME and is the basis for NFC peer-to-peer communication.
* T5T is also known as ISO/IEC 15963.

```cpp
nfc_err_t configure_rf_protocols(nfc_rf_protocols_bitmask_t rf_protocols);
```
Configure which protocols should be enabled during the discovery process.

```cpp
nfc_err_t start_discovery();
```
Start the discovery process.

```cpp
nfc_err_t cancel_discovery();
```
Cancel the discovery process (if running).

**Delegate**

A `NFCController` instance needs to be configured with a delegate to receive events.

```cpp
enum nfc_discovery_terminated_reason_t {
    nfc_discovery_terminated_completed = 0
    nfc_discovery_terminated_canceled,
    nfc_discovery_terminated_rf_error
};

void on_discovery_terminated(nfc_discovery_terminated_reason_t reason);
```
Let the user know when a discovery loop has been terminated (either because endpoints have been found, the user canceled it or an error occured).

```cpp
void on_nfc_initiator_discovered(const mbed::SharedPtr<NFCRemoteInitiator>& nfc_initiator);

void on_nfc_target_discovered(const mbed::SharedPtr<NFCRemoteTarget>& nfc_target);
```
These methods called when a remote initiator (the local controller is acting as a target) or a remote target (the local controller is acting as an initiator) is detected.

Shared pointers are used so that the user does not have to maintain the lifetime of these objects. The `NFCController` instance will release its reference when the endpoint is lost (see below).

### Endpoints

![nfc_endpoints_diagram]

#### NFC Endpoint

An endpoint is a generic NFC-enabled device with which the controller is communicating over the air interface.

```cpp
bool is_lost() const;
```

Set to true when this endpoint has been lost and the reference to the shared pointer has been released by the controller instance.

```cpp
nfc_rf_protocols_bitmask_t rf_protocols() const;
```

List the RF protocols which have been activated to communicate with that endpoint.

**Delegate**

```cpp
virtual void on_lost();
```

This is called when this endpoint has been lost and the reference to the shared pointer is about to be released by the controller instance.

#### NFC Remote Initiator

This class derives from the base `NFCEndpoint` class.

```cpp
bool is_ndef_supported();
size_t nfc_tag_type();

void set_ndef_message(const NDEFMessage& message);
void clear_ndef_message();
NDEFMessage get_ndef_message();
```
These methods provide access to the NDEF message that we are exposing to the remote initiator.

Additionally the type of NFC tag (1 to 5) which is being emulated can be recovered.

```cpp
bool is_iso7816_supported();
void add_iso7816_application(const ISO7816App& app);
```

If supported by the underlying technology (ISO-DEP), a contactless smartcard can be emulated and ISO7816-4 applications can be registered using this API.

**Delegate**

```cpp
virtual void on_selected();
virtual void on_deselected();
```

Some phones/readers 'park' a target and re-select it later - these events let the user know if the what state the local target is being put in.

```cpp
virtual void on_before_ndef_read();
virtual void on_after_ndef_write();
```

When emulating a NFC tag, it can be useful to generate a NDEF message dynamically just before a read by the initiator. Conversly if the initiator updates the NDEF message, it can be processed immediately afterwards.

#### NFC Target

This is the base class for NFC targets that can be of two types:
* NFC EEPROMs (Dual-interface wired devices)
* Remote NFC Targets (NFC devices over NFC RF interface)

Apart from the actual transport (Wired or NFC), the usage is very similar which explains why these methods are shared across these devices types.

```cpp
void write_ndef_message(const NDEFMessage& message)
void erase_ndef_message()
void read_ndef_message()
```

**Delegate**

```cpp
void on_ndef_message_written(nfc_err_t result)
void on_ndef_message_erased(nfc_err_t result)
void on_ndef_message_read(nfc_err_t result, NDEFMessage* message)
```

#### NFC EEPROM

The `NFCEEPROM` class derives from `NFCTarget` and shares the same API.

#### NFC Remote Target

*Note: This is initially out of scope for the initial release*

The `NFCRemoteTarget` class derives from `NFCTarget` and additionally from `NFCEndpoint`.

## NDEF API

![ndef_diagram]

The NDEF API is constructed with these requirements in mind:
* Minimizing memory allocation/copies
* NFC Forum compliance
* Ease of use

#### NDEF Message

A NDEF Message is made of multiple NDEF Records which is reflected by the API:

```cpp
bool parse(const uint8_t* buffer, size_t sz)
size_t count()
NDEFRecord operator[](size_t n)
```

The message can be mapped with a byte array and individual records are decoded/populated on the fly.

#### NDEF Message builder

We're using a builder pattern to encode an NDEF message over a byte array.

```cpp
NDEFMessageBuilder(uint8_t* buffer, size_t max_sz)
bool add_record(const NDEFRecord& record)
NDEFMessage build()
```

A reference to the array is provided in the constructor and records can be appended by the user (within memory limits).

Once done a NDEFMessage instance mapped to a subset of the byte array can be generated.

#### NDEF Record

The NDEF Record class is closely mapped with the NFC NDEF specification.

Each record holds:
* A Type Name Format indicator - indicates which namespace the type field belongs to ('Well-known NDEF types', MIME, Absolute URI, etc.)
* A type field
* An optional ID field
* The record's value

All arrays are passed by reference (no copy made).

```cpp
static bool parse(const uint8_t* buffer, size_t max_sz)
ssize_t build(const uint8_t* buffer, size_t max_sz)

uint8_t tnf()
void set_tnf(uint8_t tnf)

const uint8_t* type() const
size_t type_size() const
void set_type(const uint8_t* type, size_t type_size)

const uint8_t* id() const
size_t id_size() const
void set_id(const uint8_t* id, size_t id_size)

const uint8_t* value() const
size_t value_size() const
void set_value(const uint8_t* type, size_t type_size)
```

**Helpers**

We will provide multiple helpers to make it easy to create/parse common record types:
* URI
* Text
* Smart Poster
* MIME data

For instance, the `URIRecord`'s class API is as follows:
```cpp
static bool is_uri_record(const NDEFRecord& record)
static URIRecord as_uri_record(const NDEFRecord& record)

uri_prefix_t uri_prefix() const
void set_uri_prefix(uri_prefix_t prefix)

bool get_uri(char* uri, size_t max_sz) const
size_t uri_size() const
void set_uri(const char* uri)

bool get_full_uri(char* uri, size_t max_sz) const
size_t full_uri_size() const
void set_full_uri(const char* uri)
```

This includes some helper classes to check whether a record is an URI record, and if so to construct an `URIRecord` instance from a `NDEFRecord`.

In this case buffers are copied to account for the NULL-terminator character that is not present in the underlying byte buffer.

## HAL APIs

### NFC EEPROM API

The one HAL API that will have to be implemented by vendors to make use of the `NFCEEPROM` class are the following virtual methods.

From the upper layer's point of view, the EEPROM is a byte array that can be read from/written to. Long operations (reads, writes, erasures) must happen asynchronously. Booleans indicate whether a particular operation was succesful. Encoding is handled by the upper layer.

Address 0 means the start of the NDEF buffer (not necessarily at address 0 in the EEPROM). 

When a buffer is passed to the backend, the reference remains valid till the corresponding event is called.

The `backend_set_size()` command is called to change the size of the buffer (within the limits set by `backend_get_max_size()`). Inversely that buffer size can be read by `backend_get_size()`.

```cpp
void backend_reset()
size_t backend_get_max_size() const
void backend_read_bytes(uint32_t address, size_t count)
void backend_write_bytes(uint32_t address, const uint8_t* bytes, size_t count)
void backend_set_size(size_t count)
void backend_get_size()
void backend_erase_bytes(uint32_t address, size_t size) 
```

The following events must be called to signal completion of long operations:
```cpp
void on_backend_has_read_bytes(bool success, const uint8_t* bytes)
void on_backend_has_written_bytes(bool success)
void on_backend_has_set_size(bool success)
void on_backend_has_gotten_size(bool success, size_t size)
void on_backend_has_erased_bytes(bool success)
```

### NCI Driver APIs

This API will be defined in phase 2.

# Testing strategy
## NFC Forum Compliance

A dongle driven by [PyNFC](https://nfcpy.readthedocs.io/en/latest/index.html) will be used to run GreenTea-based tests to ensure that the implementation behaves correctly for a range of system tests.

Unit tests will cover all internal logic and NFC endpoints can be mocked/emulated where possible.

In the future we could run NFC Forum test suites using approved testing equipment.

## Interoperability

Interoperability is important with a technology such as NFC. Therefore our testing rig will include a selection of smartphones and NFC tags that can be connected using analog switches to the relevant NFC-enabled platform running mbed OS.

![interop_test_rig]

## HAL testing

GreenTea tests will be provided to partners to ensure compliance with the NFC EEPROM backend API.

# Dependencies
* Event Queue

There are currently at least four event queues (Plaftorm, BLE, USB, IP) in mbed OS and NFC will also require an event queing mechanism. We should aim at reusing one of these existing queues with the long term goal of unifying these code bases.
					
[phase_1_architecture]: phase_1_architecture.png
[phase_2_architecture]: phase_2_architecture.png
[nfc_controller_diagram]: uml_diagram_controller.png
[nfc_endpoints_diagram]: uml_diagram_endpoints.png
[ndef_diagram]: uml_diagram_ndef.png
[interop_test_rig]: interop_test_rig.png