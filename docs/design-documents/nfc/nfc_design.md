# Near-Field Communication in Mbed OS

## Table of Contents

- [Near-Field Communication in Mbed OS](#near-field-communication-in-mbed-os)
    - [Table of Contents](#table-of-contents)
    - [Revision history](#revision-history)
- [Introduction](#introduction)
    - [Overview and Background](#overview-and-background)
    - [Use cases](#use-cases)
        - [Commissioning](#commissioning)
            - [Identification](#identification)
            - [Transport](#transport)
        - [BLE Pairing](#ble-pairing)
- [System Architecture and High-Level Design](#system-architecture-and-high-level-design)
    - [Compliance with NFC Forum Specifications](#compliance-with-nfc-forum-specifications)
    - [User-facing API](#user-facing-api)
    - [Phase 1: MicroNFC stack integration](#phase-1-micronfc-stack-integration)
    - [Phase 2: NFC Host/Controller split, NCI and NFC HAL API](#phase-2-nfc-hostcontroller-split-nci-and-nfc-hal-api)
- [Detailed Design](#detailed-design)
    - [User-facing APIs](#user-facing-apis)
        - [NFC Controller](#nfc-controller)
        - [Endpoints](#endpoints)
            - [NFC Remote Endpoint](#nfc-remote-endpoint)
            - [NFC NDEF Capable](#nfc-ndef-capable)
            - [NFC Remote Initiator](#nfc-remote-initiator)
            - [NFC Target](#nfc-target)
            - [NFC EEPROM](#nfc-eeprom)
            - [NFC Remote Target](#nfc-remote-target)
    - [NDEF API](#ndef-api)
        - [Common objects](#common-objects)
        - [Parsing](#parsing)
            - [ndef::MessageParser](#ndefmessageparser)
                - [ndef::MessageParser::Delegate](#ndefmessageparserdelegate)
            - [NDEF Record parsing](#ndef-record-parsing)
                - [ndef::RecordParser](#ndefrecordparser)
                - [ndef::RecordParserChain](#ndefrecordparserchain)
                - [ndef::GenericRecordParser<ParserImplementation, ParsingResult>](#ndefgenericrecordparserparserimplementation-parsingresult)
                    - [ndef::GenericRecordParser<ParserImplementation, ParsingResult>::Delegate](#ndefgenericrecordparserparserimplementation-parsingresultdelegate)
            - [Common parsers](#common-parsers)
            - [Simple parser](#simple-parser)
                - [Delegate](#delegate)
        - [Serialization](#serialization)
    - [HAL APIs](#hal-apis)
        - [NFC EEPROM API](#nfc-eeprom-api)
        - [NCI Driver APIs](#nci-driver-apis)
- [Testing strategy](#testing-strategy)
    - [NFC Forum Compliance](#nfc-forum-compliance)
    - [Interoperability](#interoperability)
    - [HAL testing](#hal-testing)
- [Dependencies](#dependencies)
  
## Revision history
| Revision 	| Date           	| Authors                                                	| Mbed OS Version 	| Comments         	|
|----------	|----------------	|--------------------------------------------------------	|-----------------	|------------------	|
| 1.0      	| 24 August 2018 	| Donatien Garnier ([@donatieng](https://github.com/donatieng/)); Vincent Coubard ([@pan-](https://github.com/pan-/)) 	| 5.10+           	| Initial revision 	|

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
For now, the one controller we support is the PN512 which implements the `NFCControllerDriver` class which is specific to the current MicroNFC release.

It offers the following methods:

```cpp
void set_delegate(Delegate* delegate);
```
Set the instance's delegate.

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
nfc_err_t initialize();
```
Initialize the NFC controller.

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

#### NFC Remote Endpoint

A remote endpoint is a generic NFC-enabled device with which the controller is communicating over the air interface.

```cpp
nfc_err_t connect();
```

Establish a connection with the remote endpoint.

```cpp
nfc_err_t disconnect();
```

Drop the connection with the remote endpoint.


```cpp
bool is_connected() const;
```

Set to true when the connection to this endpoint has been activated and is currently selected by the controller.

```cpp
bool is_disconnected() const;
```

Set to true when this endpoint has been lost and the reference to the shared pointer has been released by the controller instance.

```cpp
nfc_rf_protocols_bitmask_t rf_protocols() const;
```

List the RF protocols which have been activated to communicate with that endpoint.

**Delegate**

```cpp
virtual void on_connected();
```

This is called when a connection to this endpoint has been succesfully established.


```cpp
virtual void on_disconnected();
```

This is called when this endpoint has been lost and the reference to the shared pointer is about to be released by the controller instance.

#### NFC NDEF Capable

This class is inherited by all endpoints that have the capability of handling NDEF data.

User-facing API:
```cpp
NFCNDEFCapable(uint8_t* buffer, size_t buffer_size);
```
The instance needs to be constructed using a stratch buffer which will be used to encode and/or decode NDEF messages.

```cpp
bool is_ndef_supported() const;

void set_ndef_delegate(Delegate* delegate);
```

API used by descendant classes:
```cpp
void parse_ndef_message(const ac_buffer_t& buffer);
void build_ndef_message(ac_buffer_builder_t& buffer_builder);
ndef_msg_t* ndef_message();
```

**Delegate**

The instance receives requests to encode and decode NDEF messages and the user can choose how to handle them using the relevant builders and parsers.

```cpp
void parse_ndef_message(const uint8_t* buffer, size_t size);
```
The encoded NDEF message is passed to the user for processing.

```cpp
size_t build_ndef_message(uint8_t* buffer, size_t capacity);
```
The user can encode a NDEF message in the buffer provided and return its size (or 0).

#### NFC Remote Initiator

This class derives from the base `NFCRemoteEndpoint` and `NFCNDEFCapable` classes.

```cpp
enum nfc_tag_type_t {
    nfc_tag_type_1,
    nfc_tag_type_2,
    nfc_tag_type_3,
    nfc_tag_type_4a,
    nfc_tag_type_4b,
    nfc_tag_type_5
};
```

```cpp
nfc_tag_type_t nfc_tag_type();
```

Additionally the type of NFC tag (1 to 5) which is being emulated can be recovered.
Type 4 can be implemented on top of two technologies, therefore it is separated into type 4a and type 4b.

```cpp
bool is_iso7816_supported();
void add_iso7816_application(ISO7816App* app);
```

If supported by the underlying technology (ISO-DEP), a contactless smartcard can be emulated and ISO7816-4 applications can be registered using this API.

**Delegate**

The delegate derives from delegates of `NFCRemoteEndpoint` and `NFCNDEFCapable`.

#### NFC Target

This is the base class for NFC targets that can be of two types:
* NFC EEPROMs (Dual-interface wired devices)
* Remote NFC Targets (NFC devices over NFC RF interface)

Apart from the actual transport (Wired or NFC), the usage is very similar which explains why these methods are shared across these devices types.

This class derives from `NFCNDEFCapable`.

```cpp
void write_ndef_message();
void erase_ndef_message();
void read_ndef_message();
```

Calling these functions will trigger the appropriate NDEF parsing/building process if handlers are registered in the `NFCNDEFCapable` instance.

**Delegate**

```cpp
void on_ndef_message_erased(nfc_err_t result);
void on_ndef_message_written(nfc_err_t result);
void on_ndef_message_read(nfc_err_t result);
```

#### NFC EEPROM

The `NFCEEPROM` class derives from `NFCTarget` and shares the same API.
A pointer to a `NFCEEPROMDriver` instance (see below) must be passed in the constructor.

#### NFC Remote Target

*Note: This is initially out of scope for the initial release*

The `NFCRemoteTarget` class derives from `NFCTarget` and additionally from `NFCRemoteEndpoint`.

## NDEF API

The NDEF API is constructed with these requirements in mind:
* Minimizing memory allocation/copies
* NFC Forum compliance
* Ease of use
* Extensibility

### Common objects

We will provide multiple helpers to make it easy to create/parse common record types:
* URI
* Text
* Smart Poster
* MIME data

For instance, the `URI`'s class API is as follows:
```cpp
uri_prefix_t uri_prefix() const
void set_uri_prefix(uri_prefix_t prefix)

bool get_uri(char* uri, size_t max_sz) const
size_t uri_size() const
void set_uri(const char* uri)

bool get_full_uri(char* uri, size_t max_sz) const
size_t full_uri_size() const
void set_full_uri(const char* uri)
```

**Note:** These types can be replaced by user defined ones if parsing and serialization logic is provided.

### Parsing

#### ndef::MessageParser

![ndef_message_parser_diagram]

Messages incoming from the peer are parsed by a `MessageParser` which produce
`Record` instances to its client. The parsing operation is event-driven: a
message parser client registers a delegate inside the message parser. This delegate
gets notified whenever an interesting event happens during the parsing.

```cpp
void set_delegate(Delegate* delegate);
void parse(const ac_buffer_t& data_buffer);
```

It is important to note that the data_buffer in entry of the parse function must
contain the entire NDEF message.

##### ndef::MessageParser::Delegate

```cpp
virtual void on_parsing_started() { }
virtual void on_record_parsed(const Record& record) { }
virtual void on_parsing_terminated() { }
virtual void on_parsing_error(error_t error) { }
```

The delegate is notified by the parser when the parsing start or end; when an error
is encountered or when an ndef `Record` has been parsed.

To reduce memory consumption `Record` instances generated by the parser are short
lived. They are only valid during the callback invocation. If a client is interested
by the content of a message parsed and wants to use it after the parsing callback
then it must make a copy of the record object.

#### NDEF Record parsing

![ndef_record_parser_diagram]

NDEF records can contain any type of content. Therefore parsing of records is
specific to the application. To help the developer; an optional ndef record
parsing framework is included. It follows the _chain-of-responsibility_ design
pattern that facilitate the integration of record parsers defined by client code.

##### ndef::RecordParser

Is is the base building block of the record parsing frame working. It parses a
record then return true if the record has been parsed or false otherwise.

```cpp
virtual bool parse(const Record&);
```

##### ndef::RecordParserChain

It aggregate `RecordParser` instances and defer parsing to the instances it contains.

```cpp
bool parse(const Record& record);
void set_next_parser(RecordParser* parser);
```

##### ndef::GenericRecordParser<ParserImplementation, ParsingResult>

This is a partial implementation of the `RecordParser` interface. It exposes a
delegate type that can be implemented and registered by clients of this parser.
This delegate expects objects of the parsing result type.

```cpp
bool parse(const Record&)
void set_delegate(Delegate* delegate)
```

Implementation of this class must expose the following non virtual function:

```c++
bool do_parse(const Record& record, ParsingResult& parsing_result);
```

If the parsing is successful then it should return true and fill `parsing_result`
otherwise it should return false and leave `parsing_result` untouched.

**Note:** The Curiously recurring template pattern (CRTP) is used to implement
the delegation mechanism in a type-safe fashion. This is not achievable with
_regular_ polymorphism.

###### ndef::GenericRecordParser<ParserImplementation, ParsingResult>::Delegate

This delegate must be implemented by clients of this class. It receives the objects
parsed.

```cpp
virtual void on_record_parsed(const ParsingResult& record, const RecordID* id);
```

**Note:** Usually clients are client of an implementation of an
ndef::GenericRecordParser<ParserImplementation, ParsingResult> . They can refer
to the delegate as `ImplementationName::Delegate`.

#### Common parsers

![ndef_common_parsers_diagram]

Parsers for each common record type exists. They inherit from the
`GenericRecordParser` to exposes a common delegate interface:

```cpp
virtual void on_record_parsed(const <ParsedType>& result, const ndef::RecordID* id)
```

#### Simple parser

The API provide a class named `SimpleMessageParser` that glues together a
`MessageParser` and a chain `RecordParser`'s containing the parsers for the common
types.

![ndef_simple_parser_diagram]

Clients of the class can register a delegate, parse a message or add a new
`RecordParser` in the parsing chain.

```cpp
void set_delegate(Delegate* delegate);
void parse(const ac_buffer_t& data_buffer);
void add_record_parser(ndef::RecordParser* parser);
```

##### Delegate

This delegate must be implemented by clients of this class. It receives events
from the parsing process:

```cpp
virtual void on_parsing_error(ndef::MessageParser::error_t error);
virtual void on_parsing_started();
virtual void on_text_parsed(const Text& text, const ndef::RecordID* id);
virtual void on_mime_parsed(const Mime& text, const ndef::RecordID* id);
virtual void on_uri_parsed(const URI& uri, const ndef::RecordID* id);
virtual void on_unknown_record_parsed(const ndef::Record& record);
virtual void on_parsing_terminated();
```

### Serialization

The class `MessageBuilder` is used to map a record into an NDEF message. It
includes a data buffer that contains the _raw_ message. Client code use the
functions `append_record` to append a new record into the message being built.

![ndef_message_builder_diagram]

For convenience, serialization functions for common types are provided as well as
a specialized `MessageBuilder` named `SimpleMessageBuilder` that exposes them
in an object oriented fashion.

## HAL APIs

### NFC EEPROM API

The one HAL API that will have to be implemented by vendors to implement a `NFCEEPROMDriver` driver are the following virtual methods.

From the upper layer's point of view, the EEPROM is a byte array that can be read from/written to. Long operations (reads, writes, erasures) must happen asynchronously. Booleans indicate whether a particular operation was succesful. Encoding is handled by the upper layer.

Address 0 means the start of the NDEF buffer (not necessarily at address 0 in the EEPROM).

When a buffer is passed to the backend, the reference remains valid till the corresponding event is called.

The `set_size()` command is called to change the size of the buffer (within the limits set by `get_max_size()`). Inversely that buffer size can be read using `get_size()`.

`start_session()` and `end_session()` are used before a series of memory operations to allow the driver to lock/un-lock the RF interface during these operations to avoid having concurrent access to the memory.

```cpp
void reset();
size_t get_max_size();
void start_session();
void end_session();
void read_bytes(uint32_t address, size_t count);
void write_bytes(uint32_t address, const uint8_t* bytes, size_t count);
void read_size(size_t count);
void write_size();
void erase_bytes(uint32_t address, size_t size)
```

The following events must be called to signal completion of long operations:
```cpp
void on_session_started(bool success);
void on_session_ended(bool success);
void on_bytes_read(size_t count);
void on_bytes_written(size_t count);
void on_size_read(bool success, size_t size);
void on_size_written(bool success);
void on_bytes_erased(size_t count);
```

The implementation also has access to an event queue in case asynchronous operations need to be run:
```cpp
Delegate *delegate();
events::EventQueue *event_queue();
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
[interop_test_rig]: interop_test_rig.png
[ndef_message_parser_diagram]: uml_diagram_ndef_message_parser.png
[ndef_record_parser_diagram]: uml_diagram_ndef_record_parser.png
[ndef_common_parsers_diagram]: uml_diagram_ndef_common_parsers.png
[ndef_simple_parser_diagram]: uml_diagram_ndef_simple_parser.png
[ndef_message_builder_diagram]: uml_diagram_ndef_message_builder_diagram.png