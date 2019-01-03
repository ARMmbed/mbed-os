# NFC test SDK comparison

A comparison of the SDKs exposed to manage NFC tags on Android IOs and Python module PyNFC.

<!-- TOC -->

- [NFC test SDK comparison](#nfc-test-sdk-comparison)
- [Overview](#overview)
- [Comparison](#comparison)
    - [IOS (objective C)](#ios-objective-c)
    - [Android (Java)](#android-java)
    - [pynfc (python)](#pynfc-python)
- [Observe](#observe)

<!-- /TOC -->

# Overview
A comparison which analyses NFC use cases on mobile, as background to the test case design/implementation in the comissioning workflow :NFC-Bluetooth-pairing application. 
- Analyse the [Apple API](https://developer.apple.com/documentation/corenfc)
- Analyse the [Android API](https://developer.android.com/guide/topics/connectivity/nfc/advanced-nfc#java)
- Python test [pynfc modules](https://nfcpy.readthedocs.io/en/latest/modules/index.html)


# Comparison
From the lowest level, each programmer interface has a definition for Errors, Link, NDEF, Tags and Sessions.
Note: Comparisons are high level and use past experience and old docs.


## IOS (objective C)
1. Errors: 
   are a struct with 3 members:<BR>
   int:value, <BR>struct:string:usererror<BR>struct:string:localized
1. isodep:

   not supported
1. NDEF support parameters with

   payload<BR>and Typename, 
4. Tags:
 
   Are an Object representing tag
5. Sessions are managed using 

   Delegate &callbacks

## Android (Java)
1. Errors:
 
   thrown as IOException with cause and message, there are no returned error datas
2. isodep:

   get/set communication parameters and RAW. 
3. NDEF: 

   Includes SNEP
4. Tags :

   (3)A,(3)B,(4)F,V and isoDep layer
5. Sessions 
 
   Intents and Actions, runtime registration of PendingIntent() allows hooking using tag filters

## pynfc (python) 
1. Errors :

   raises Exceptions nfc.clf.Error and others per class
2. isodep:

   get/set communication parameters and RAW.
3. NDEF: 

   full implementation and types. Includes SNEP
4. Tags :

   1,2,(3)A,(3)B,(4)F isoDep layer
5. Sessions :

   using delegate class callbacks

# Observe
Negative test cases would be better designed around the user cases, than around the implementations, base error conditions at the API layer look more like
- UnsupportedTarget
- Communication
- Protocol
- (w) Transmission
- (w) Timeout
- (w) BrokenLink
- ValueError

Valuable test data cases shall be for valid and boundary cases for the smartposter NDEF record:
- uri – URI string ASCII only
- title – Smart poster title(s), (additional internationalizations with IANA codes not tested)
- icons – omitted
- action – The recommended action , a string
