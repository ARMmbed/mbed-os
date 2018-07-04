#   Introduction
##  Overview and Background
NFC offers a simple and secure way of commissioning IoT devices in the field, and we are seeing increasing demand for this from prospective customers. We have a plan to introduce NFC into Mbed OS, this is the first phase to add a reference implementation of card emulation mode.

NFC offers three modes;
1. NFC card emulation
2. NFC reader/writer
3. NFC peer to peer

To support the commissioning of NFC enabled IoT endpoints, Mbed OS should support the NFC card emulation mode. This will allow for commissioning of the device via an NFC-enabled smartphone.

However the architecture should be future-proofed and should also be extendable to support other NFC modes in the future.

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
// TODO

###  NFC Controller

The `NFCController` class offers the following methods:

```cpp
struct nfc_rf_protocols_bitmask_t
{
    uint8_t initiator_t1t : 1;
    uint8_t initiator_t2t : 1;
    uint8_t initiator_t3t : 1;
    uint8_t initiator_iso_dep : 1;
    uint8_t initiator_nfc_dep : 1;

    uint8_t target_t1t : 1;
    uint8_t target_t2t : 1;
    uint8_t target_t3t : 1;
    uint8_t target_iso_dep : 1;
    uint8_t target_nfc_dep : 1;
};

nfc_rf_protocols_bitmask_t get_supported_rf_protocols() const;
```
Retrieve the list of supported RF protocols:
These are mapped against NFC Forum-defined protocols.

T1T is based on ISO4443A-3 and commonly known as Topaz (Innovision). 
T2T is based on ISO4443A-3 and commonly known as Mifare Ultralight/NTAG (NXP).
T3T is based on JIS FIXME, also known as Felica (Sony).
ISO-DEP is also known as ISO4443-4 and is the common interface for contactless smartcards. The underlying radio protocol can either be ISO4443A or ISO4443B.
NFC-DEP is also known as ISO92xxx / FIXME and is the basis for NFC peer-to-peer communication.

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

### NFC Controller Delegate

A `NFCController` instance needs to be configured with a delegate to receive events.

```cpp
enum nfc_discovery_terminated_reason_t {
    nfc_discovery_terminated_completed = 0
    nfc_discovery_terminated_canceled,
    nfc_discovery_terminated_rf_error
};

void on_discovery_terminated(nfc_discovery_terminated_reason_t reason);
```

```cpp
void on_nfc_initiator_discovered(const mbed::SharedPtr<NFCInitiator>& nfc_initiator);

void on_nfc_target_discovered(const mbed::SharedPtr<NFCTarget>& nfc_target);

```

### NFC Endpoint

```cpp
bool is_lost() const;
```

```cpp
nfc_rf_protocols_bitmask_t rf_protocols() const;
```

### NFC Initiator
```cpp
nfc_radio_tech_t radio_tech();

// NDEF Tag API
bool is_ndef_supported();

// 1 to 5 if NDEF is supported
size_t nfc_tag_type();

// Type 4 based on ISO7816
bool is_iso7816_supported();

// Add ISO7816 app
void add_iso7816_application(const ISO7816App& app);

void set_ndef_message(const NDEFMessage& message);
void clear_ndef_message();
NDEFMessage get_ndef_message();
```

### NFC Endpoint Delegate

```cpp
virtual void on_lost();
```

### NFC Initiator Delegate

```cpp
virtual void on_selected();

// Some phones 'park' the target and re-select it later
virtual void on_deselected();

// Initiator can release the target on RF signal can be lost
virtual void on_disconnected(/* code with details */);

virtual void on_before_ndef_read();

virtual void on_after_ndef_write();
```

### NDEF API


### NFC EEPROM API


## HAL APIs

### NFC EEPROM API

### NCI Driver APIs

# Usage Scenarios and Examples
*Show pseudo-code or flowcharts explaining the usage of the feature.*
## Commissioning
###	Identification
###	Transport
##	Other use cases
###	BLE Pairing
###	Smart Poster

# Testing strategy
## NFC Forum Compliance
*Explain which tools needs to be changed and the nature of changes*

## Interoperability
*Explain which tools needs to be changed and the nature of changes*

## HAL testing
*Explain which tools needs to be changed and the nature of changes*

# Dependencies
* Event Queue

					
[phase_1_architecture]: phase_1_architecture.png
[phase_2_architecture]: phase_2_architecture.png