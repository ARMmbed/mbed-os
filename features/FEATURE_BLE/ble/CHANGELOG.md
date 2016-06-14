# Change Log

## [v2.5.2](https://github.com/ARMmbed/ble/tree/v2.5.2) (2016-03-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.5.1...v2.5.2)

**Closed issues:**

- Infinite busy [\#69](https://github.com/ARMmbed/ble/issues/69)

## [v2.5.1](https://github.com/ARMmbed/ble/tree/v2.5.1) (2016-01-27)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.5.0...v2.5.1)

**Closed issues:**

- Gap state is not updated when the advertising stop after the timeout [\#80](https://github.com/ARMmbed/ble/issues/80)
- crash on memory allocation [\#26](https://github.com/ARMmbed/ble/issues/26)

**Merged pull requests:**

- Change onShutdown\(obj, func\) definition [\#174](https://github.com/ARMmbed/ble/pull/174) ([andresag01](https://github.com/andresag01))
- Invalid adv payloads are not committed before checking [\#171](https://github.com/ARMmbed/ble/pull/171) ([andresag01](https://github.com/andresag01))
- Update Gap state in startAdvertising\(\) [\#170](https://github.com/ARMmbed/ble/pull/170) ([andresag01](https://github.com/andresag01))
- Improve doxygen documentation: [\#169](https://github.com/ARMmbed/ble/pull/169) ([pan-](https://github.com/pan-))
- Update Gap state after advertising times out [\#168](https://github.com/ARMmbed/ble/pull/168) ([andresag01](https://github.com/andresag01))
- merge version 2.5.0 into master [\#163](https://github.com/ARMmbed/ble/pull/163) ([pan-](https://github.com/pan-))
- Fix doxygen warnings for BLE API [\#162](https://github.com/ARMmbed/ble/pull/162) ([andresag01](https://github.com/andresag01))

## [v2.5.0](https://github.com/ARMmbed/ble/tree/v2.5.0) (2016-01-12)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.4.0...v2.5.0)

**Merged pull requests:**

- merge branch develop \(v2.4.0\) [\#161](https://github.com/ARMmbed/ble/pull/161) ([pan-](https://github.com/pan-))
- Add API to get addresses of peers in bond table [\#160](https://github.com/ARMmbed/ble/pull/160) ([andresag01](https://github.com/andresag01))

## [v2.4.0](https://github.com/ARMmbed/ble/tree/v2.4.0) (2016-01-07)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.3.0...v2.4.0)

**Merged pull requests:**

- Whitelisting experimental API [\#159](https://github.com/ARMmbed/ble/pull/159) ([andresag01](https://github.com/andresag01))

## [v2.3.0](https://github.com/ARMmbed/ble/tree/v2.3.0) (2015-12-23)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.2.3...v2.3.0)

**Merged pull requests:**

- Characteristic descriptor discovery [\#105](https://github.com/ARMmbed/ble/pull/105) ([pan-](https://github.com/pan-))

## [v2.2.3](https://github.com/ARMmbed/ble/tree/v2.2.3) (2015-12-23)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.2.2...v2.2.3)

**Closed issues:**

- Two different enum list of BLE appearances [\#136](https://github.com/ARMmbed/ble/issues/136)
- Gap::updateAdvertisingPayload should work for different length of data [\#84](https://github.com/ARMmbed/ble/issues/84)

**Merged pull requests:**

- Hotfix for backward compatibility [\#158](https://github.com/ARMmbed/ble/pull/158) ([LiyouZhou](https://github.com/LiyouZhou))
- Clean up code in DiscoveredCharacteristic.cpp [\#154](https://github.com/ARMmbed/ble/pull/154) ([andresag01](https://github.com/andresag01))
- Modify functions that manipulate adv payload [\#153](https://github.com/ARMmbed/ble/pull/153) ([andresag01](https://github.com/andresag01))

## [v2.2.2](https://github.com/ARMmbed/ble/tree/v2.2.2) (2015-12-21)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.2.1...v2.2.2)

**Merged pull requests:**

- Add BLE\_ERROR\_INTERNAL\_STACK\_FAILURE error code [\#155](https://github.com/ARMmbed/ble/pull/155) ([andresag01](https://github.com/andresag01))

## [v2.2.1](https://github.com/ARMmbed/ble/tree/v2.2.1) (2015-12-18)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.2.0...v2.2.1)

**Merged pull requests:**

- Remove deprecated appearance enum from blecommon.h [\#150](https://github.com/ARMmbed/ble/pull/150) ([andresag01](https://github.com/andresag01))

## [v2.2.0](https://github.com/ARMmbed/ble/tree/v2.2.0) (2015-12-17)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.16...v2.2.0)

**Merged pull requests:**

- Add onShutdown to register callbacks [\#146](https://github.com/ARMmbed/ble/pull/146) ([andresag01](https://github.com/andresag01))
- transparenly support existing applications which may have used Gap::ADDR\_TYPE\_\* [\#145](https://github.com/ARMmbed/ble/pull/145) ([rgrover](https://github.com/rgrover))

## [v2.1.16](https://github.com/ARMmbed/ble/tree/v2.1.16) (2015-12-16)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.15...v2.1.16)

**Merged pull requests:**

- Improve API to facilitate full shutdown procedure [\#141](https://github.com/ARMmbed/ble/pull/141) ([andresag01](https://github.com/andresag01))

## [v2.1.15](https://github.com/ARMmbed/ble/tree/v2.1.15) (2015-12-15)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.14...v2.1.15)

**Merged pull requests:**

- Extract Adress related types from Gap.h into BLEProtocol.h [\#142](https://github.com/ARMmbed/ble/pull/142) ([rgrover](https://github.com/rgrover))

## [v2.1.14](https://github.com/ARMmbed/ble/tree/v2.1.14) (2015-12-11)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.13...v2.1.14)

## [v2.1.13](https://github.com/ARMmbed/ble/tree/v2.1.13) (2015-12-11)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.11...v2.1.13)

**Merged pull requests:**

- Added SecurityManager::setLinkSecurity call for elevating security settings on a particular connection. [\#140](https://github.com/ARMmbed/ble/pull/140) ([marcuschangarm](https://github.com/marcuschangarm))

## [v2.1.11](https://github.com/ARMmbed/ble/tree/v2.1.11) (2015-12-10)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.10...v2.1.11)

## [v2.1.10](https://github.com/ARMmbed/ble/tree/v2.1.10) (2015-12-07)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.9...v2.1.10)

**Merged pull requests:**

- WIP: UUID endian change [\#128](https://github.com/ARMmbed/ble/pull/128) ([marcuschangarm](https://github.com/marcuschangarm))

## [v2.1.9](https://github.com/ARMmbed/ble/tree/v2.1.9) (2015-12-04)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.8...v2.1.9)

**Merged pull requests:**

- Fix documentation for advertising interval getters [\#134](https://github.com/ARMmbed/ble/pull/134) ([andresag01](https://github.com/andresag01))
- I change the service description [\#132](https://github.com/ARMmbed/ble/pull/132) ([iriark01](https://github.com/iriark01))
- Add a doxyfile that warns for undocumented elements [\#131](https://github.com/ARMmbed/ble/pull/131) ([bremoran](https://github.com/bremoran))

## [v2.1.8](https://github.com/ARMmbed/ble/tree/v2.1.8) (2015-12-03)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.7...v2.1.8)

**Merged pull requests:**

- Edit [\#130](https://github.com/ARMmbed/ble/pull/130) ([iriark01](https://github.com/iriark01))
- Edit [\#129](https://github.com/ARMmbed/ble/pull/129) ([iriark01](https://github.com/iriark01))

## [v2.1.7](https://github.com/ARMmbed/ble/tree/v2.1.7) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.6...v2.1.7)

## [v2.1.6](https://github.com/ARMmbed/ble/tree/v2.1.6) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.5...v2.1.6)

**Closed issues:**

- Terrible handling of initLen / minLen and variable length characteristics. [\#93](https://github.com/ARMmbed/ble/issues/93)

**Merged pull requests:**

- Allow GattAttributes to have variable length [\#127](https://github.com/ARMmbed/ble/pull/127) ([andresag01](https://github.com/andresag01))

## [v2.1.5](https://github.com/ARMmbed/ble/tree/v2.1.5) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.4...v2.1.5)

**Merged pull requests:**

- Edit [\#126](https://github.com/ARMmbed/ble/pull/126) ([iriark01](https://github.com/iriark01))
- Edit [\#125](https://github.com/ARMmbed/ble/pull/125) ([iriark01](https://github.com/iriark01))
- Edit [\#124](https://github.com/ARMmbed/ble/pull/124) ([iriark01](https://github.com/iriark01))
- Edit [\#123](https://github.com/ARMmbed/ble/pull/123) ([iriark01](https://github.com/iriark01))
- Separate the concept of minlen and len in GattAttr [\#122](https://github.com/ARMmbed/ble/pull/122) ([andresag01](https://github.com/andresag01))
- Doxygen fixes [\#120](https://github.com/ARMmbed/ble/pull/120) ([metc](https://github.com/metc))
- Minor edits [\#114](https://github.com/ARMmbed/ble/pull/114) ([iriark01](https://github.com/iriark01))

## [v2.1.4](https://github.com/ARMmbed/ble/tree/v2.1.4) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.3...v2.1.4)

**Merged pull requests:**

- Guard nordic specific implementation with \#ifdef TARGET\_NRF51822 [\#119](https://github.com/ARMmbed/ble/pull/119) ([LiyouZhou](https://github.com/LiyouZhou))

## [v2.1.3](https://github.com/ARMmbed/ble/tree/v2.1.3) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.2...v2.1.3)

**Merged pull requests:**

- Call bootloader\_start implicitly trough a event handler call [\#118](https://github.com/ARMmbed/ble/pull/118) ([LiyouZhou](https://github.com/LiyouZhou))

## [v2.1.2](https://github.com/ARMmbed/ble/tree/v2.1.2) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.1...v2.1.2)

**Merged pull requests:**

- Reintroduce old Eddystone with deprecated warnings [\#117](https://github.com/ARMmbed/ble/pull/117) ([andresag01](https://github.com/andresag01))

## [v2.1.1](https://github.com/ARMmbed/ble/tree/v2.1.1) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.1.0...v2.1.1)

## [v2.1.0](https://github.com/ARMmbed/ble/tree/v2.1.0) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.5...v2.1.0)

**Closed issues:**

- All BLE event handling should happen in thread mode [\#89](https://github.com/ARMmbed/ble/issues/89)

**Merged pull requests:**

- First Doxygen output [\#116](https://github.com/ARMmbed/ble/pull/116) ([metc](https://github.com/metc))
- Minor doc changes [\#113](https://github.com/ARMmbed/ble/pull/113) ([metc](https://github.com/metc))
- Minor edits [\#112](https://github.com/ARMmbed/ble/pull/112) ([iriark01](https://github.com/iriark01))
- Minor edits [\#111](https://github.com/ARMmbed/ble/pull/111) ([iriark01](https://github.com/iriark01))
- Minor edits [\#110](https://github.com/ARMmbed/ble/pull/110) ([iriark01](https://github.com/iriark01))
- Remove persistant callbacks [\#109](https://github.com/ARMmbed/ble/pull/109) ([pan-](https://github.com/pan-))
- Changed GapAdvertisingData.addData to take current fields into account when adding data. [\#108](https://github.com/ARMmbed/ble/pull/108) ([marcuschangarm](https://github.com/marcuschangarm))
- Remove Eddystone implementation in ble/services [\#107](https://github.com/ARMmbed/ble/pull/107) ([andresag01](https://github.com/andresag01))

## [v2.0.5](https://github.com/ARMmbed/ble/tree/v2.0.5) (2015-11-16)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.4...v2.0.5)

**Merged pull requests:**

- Edits - with a couple of comments [\#104](https://github.com/ARMmbed/ble/pull/104) ([iriark01](https://github.com/iriark01))

## [v2.0.4](https://github.com/ARMmbed/ble/tree/v2.0.4) (2015-11-13)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.3...v2.0.4)

**Merged pull requests:**

- Edits [\#102](https://github.com/ARMmbed/ble/pull/102) ([iriark01](https://github.com/iriark01))

## [v2.0.3](https://github.com/ARMmbed/ble/tree/v2.0.3) (2015-11-03)
[Full Changelog](https://github.com/ARMmbed/ble/compare/mbedos-release-15-11...v2.0.3)

## [mbedos-release-15-11](https://github.com/ARMmbed/ble/tree/mbedos-release-15-11) (2015-11-03)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.2...mbedos-release-15-11)

## [v2.0.2](https://github.com/ARMmbed/ble/tree/v2.0.2) (2015-11-03)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.1...v2.0.2)

**Merged pull requests:**

- BLE::init\(\) should also be able to take an \<object, member\> tuple for its callback [\#97](https://github.com/ARMmbed/ble/pull/97) ([rgrover](https://github.com/rgrover))

## [v2.0.1](https://github.com/ARMmbed/ble/tree/v2.0.1) (2015-11-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v2.0.0...v2.0.1)

**Merged pull requests:**

- Fix beaconPeriod and defaults for voltage/temp [\#94](https://github.com/ARMmbed/ble/pull/94) ([andresag01](https://github.com/andresag01))

## [v2.0.0](https://github.com/ARMmbed/ble/tree/v2.0.0) (2015-11-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/mbedos-techcon-oob2...v2.0.0)

**Closed issues:**

- BLE::init\(\) should be non-blocking [\#90](https://github.com/ARMmbed/ble/issues/90)
- Wrong include file in HealthTermometerService.h:20:17 [\#86](https://github.com/ARMmbed/ble/issues/86)
- Return error on write request when locked [\#19](https://github.com/ARMmbed/ble/issues/19)

**Merged pull requests:**

- Introduced fixes to Eddystone implementation [\#92](https://github.com/ARMmbed/ble/pull/92) ([andresag01](https://github.com/andresag01))
- BLE::init\(\) now takes in a callback paramter [\#91](https://github.com/ARMmbed/ble/pull/91) ([rgrover](https://github.com/rgrover))
- Fixed include problem in HealthThermometer header [\#87](https://github.com/ARMmbed/ble/pull/87) ([andresag01](https://github.com/andresag01))
- use the github url public url in module.json [\#82](https://github.com/ARMmbed/ble/pull/82) ([jrobeson](https://github.com/jrobeson))
- Reduce the memory consumed by FunctionPointerWithContext instances [\#81](https://github.com/ARMmbed/ble/pull/81) ([pan-](https://github.com/pan-))
- Introduced fix for defect IOTSFW-1058 [\#79](https://github.com/ARMmbed/ble/pull/79) ([andresag01](https://github.com/andresag01))

## [mbedos-techcon-oob2](https://github.com/ARMmbed/ble/tree/mbedos-techcon-oob2) (2015-10-19)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v1.0.0...mbedos-techcon-oob2)

## [v1.0.0](https://github.com/ARMmbed/ble/tree/v1.0.0) (2015-10-19)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.8...v1.0.0)

**Closed issues:**

- add support for Eddystone [\#57](https://github.com/ARMmbed/ble/issues/57)

**Merged pull requests:**

- Add st-ble-shield as a possible target dependency. [\#76](https://github.com/ARMmbed/ble/pull/76) ([pan-](https://github.com/pan-))
- Support for Environmental Service \(temperature, pressure and humidity\) [\#75](https://github.com/ARMmbed/ble/pull/75) ([crespum](https://github.com/crespum))

## [v0.4.8](https://github.com/ARMmbed/ble/tree/v0.4.8) (2015-09-29)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.7...v0.4.8)

**Closed issues:**

- ERROR: undefined reference to `createBLEInstance\(\)' [\#68](https://github.com/ARMmbed/ble/issues/68)

**Merged pull requests:**

- Adding Eddystone Support [\#74](https://github.com/ARMmbed/ble/pull/74) ([BlackstoneEngineering](https://github.com/BlackstoneEngineering))
- Changed onConnection and onDisconnection to accept object/methods and… [\#72](https://github.com/ARMmbed/ble/pull/72) ([marcuschangarm](https://github.com/marcuschangarm))
- adding an initial prototype for a yotta-config based initialization f… [\#71](https://github.com/ARMmbed/ble/pull/71) ([rgrover](https://github.com/rgrover))
- Gap.h - Added onConnection callback chain simarly to the currently pr… [\#70](https://github.com/ARMmbed/ble/pull/70) ([marcuschangarm](https://github.com/marcuschangarm))

## [v0.4.7](https://github.com/ARMmbed/ble/tree/v0.4.7) (2015-08-13)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.6...v0.4.7)

## [v0.4.6](https://github.com/ARMmbed/ble/tree/v0.4.6) (2015-08-11)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.5...v0.4.6)

**Merged pull requests:**

- Eddystone Service Working [\#66](https://github.com/ARMmbed/ble/pull/66) ([BlackstoneEngineering](https://github.com/BlackstoneEngineering))

## [v0.4.5](https://github.com/ARMmbed/ble/tree/v0.4.5) (2015-08-10)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.4...v0.4.5)

## [v0.4.4](https://github.com/ARMmbed/ble/tree/v0.4.4) (2015-08-07)
[Full Changelog](https://github.com/ARMmbed/ble/compare/v0.4.3...v0.4.4)

**Closed issues:**

- GapAdvertisingData::getPayload\(\) should not return NULL if payloadLen == 0 [\#64](https://github.com/ARMmbed/ble/issues/64)
- enableActiveScanning\(\) [\#60](https://github.com/ARMmbed/ble/issues/60)
- add central role for s130 [\#58](https://github.com/ARMmbed/ble/issues/58)
- Clash With Definition and Enum Naming [\#46](https://github.com/ARMmbed/ble/issues/46)
- URIBeacon2Service Needs initial AdvertisedTxPowerLevels [\#17](https://github.com/ARMmbed/ble/issues/17)

**Merged pull requests:**

- Conversion from advertisement duration units to ms moved from Gap to … [\#63](https://github.com/ARMmbed/ble/pull/63) ([jslater8](https://github.com/jslater8))
- Add a new function updateAdvertisingPayload\(\) [\#61](https://github.com/ARMmbed/ble/pull/61) ([sunsmilearm](https://github.com/sunsmilearm))
- minor corrections to the comments [\#59](https://github.com/ARMmbed/ble/pull/59) ([ddavidebor](https://github.com/ddavidebor))

## [v0.4.3](https://github.com/ARMmbed/ble/tree/v0.4.3) (2015-07-22)
[Full Changelog](https://github.com/ARMmbed/ble/compare/0.4.1...v0.4.3)

**Closed issues:**

- add GattClient::onHVX [\#53](https://github.com/ARMmbed/ble/issues/53)
- Need some help regarding the development of BLE network simulator [\#52](https://github.com/ARMmbed/ble/issues/52)

**Merged pull requests:**

- expose descriptors optional arg in derived GattChar. constructors [\#56](https://github.com/ARMmbed/ble/pull/56) ([OJFord](https://github.com/OJFord))
- Change DiscoveredCharacteristic API to return long or short UUIDs. [\#55](https://github.com/ARMmbed/ble/pull/55) ([adfernandes](https://github.com/adfernandes))
- Refactor using new include paths [\#51](https://github.com/ARMmbed/ble/pull/51) ([rosterloh](https://github.com/rosterloh))
- Revert "use mbed-classic as mbed dependency" [\#50](https://github.com/ARMmbed/ble/pull/50) ([rgrover](https://github.com/rgrover))
- use mbed-classic as mbed dependency [\#45](https://github.com/ARMmbed/ble/pull/45) ([autopulated](https://github.com/autopulated))

## [0.4.1](https://github.com/ARMmbed/ble/tree/0.4.1) (2015-07-02)
[Full Changelog](https://github.com/ARMmbed/ble/compare/workingOldBootloader...0.4.1)

**Closed issues:**

- DiscoveredCharacteristic::setupOnDataRead\(\) should be GattClient::onDataRead\(\) instead [\#49](https://github.com/ARMmbed/ble/issues/49)
- CONNECTION\_TIMEOUT should be added to DisconnectionReason\_t [\#43](https://github.com/ARMmbed/ble/issues/43)
- Typo in definition of UNIT\_0\_625\_MS in Gap.h [\#40](https://github.com/ARMmbed/ble/issues/40)
- Initial value for \_requiredSecurity member of GattCharacteristic class breaks existing code [\#39](https://github.com/ARMmbed/ble/issues/39)
- Allow adding a User Description descriptor to a GattCharacteristic. [\#38](https://github.com/ARMmbed/ble/issues/38)
- The example code in GattAttribute.h does not refer to GattAttribute [\#37](https://github.com/ARMmbed/ble/issues/37)
- BLEDevice::clearAdvertisingPayload doesn't clear the scan response PDU [\#36](https://github.com/ARMmbed/ble/issues/36)

**Merged pull requests:**

- Added get/set methods for advertisement payload and parameters. [\#42](https://github.com/ARMmbed/ble/pull/42) ([marcuschangarm](https://github.com/marcuschangarm))

## [workingOldBootloader](https://github.com/ARMmbed/ble/tree/workingOldBootloader) (2015-03-02)
**Implemented enhancements:**

- Which are the allowed values for txPower? \> BLEDevice::setTxPower\(int8\_t txPower\) [\#30](https://github.com/ARMmbed/ble/issues/30)
- Modes: UriBeacon and UriBeaconConfig [\#15](https://github.com/ARMmbed/ble/issues/15)
- URIBeacon2Service.h should be called UriBeaconConfigService.h [\#11](https://github.com/ARMmbed/ble/issues/11)
- Lock and Unlock missing from URIBeacon2Service.h [\#10](https://github.com/ARMmbed/ble/issues/10)
- Service Characteristics [\#9](https://github.com/ARMmbed/ble/issues/9)
- URIBeacon2Service.h missing TX Power Mode [\#8](https://github.com/ARMmbed/ble/issues/8)

**Fixed bugs:**

- URIBeacon2Service configure\(\) [\#14](https://github.com/ARMmbed/ble/issues/14)
- URIBeacon2Service.h does not reset to defaults [\#7](https://github.com/ARMmbed/ble/issues/7)
- Uri Data Length and semantics [\#6](https://github.com/ARMmbed/ble/issues/6)

**Closed issues:**

- Inconsistent use of uint\_16\_t and Handle\_t [\#34](https://github.com/ARMmbed/ble/issues/34)
- Incorrect documentation on void setAdvertisingType\(\)? [\#29](https://github.com/ARMmbed/ble/issues/29)
- URIBeacon2Service setTxPowerLevel should follow specification [\#16](https://github.com/ARMmbed/ble/issues/16)
- URIBeacon2Service does not advertise UriBeacon [\#13](https://github.com/ARMmbed/ble/issues/13)
- Roadmap [\#3](https://github.com/ARMmbed/ble/issues/3)
- nRF51822 advertising interval problem [\#2](https://github.com/ARMmbed/ble/issues/2)
- nRF51822 low-power operation [\#1](https://github.com/ARMmbed/ble/issues/1)

**Merged pull requests:**

- Added callback on characteristic reads [\#33](https://github.com/ARMmbed/ble/pull/33) ([jeremybrodt](https://github.com/jeremybrodt))
- Updated UUID class to get length and pointer. Added UUID comparison. [\#32](https://github.com/ARMmbed/ble/pull/32) ([jeremybrodt](https://github.com/jeremybrodt))
- Extended attribute length handling to support dynamic length. [\#31](https://github.com/ARMmbed/ble/pull/31) ([jeremybrodt](https://github.com/jeremybrodt))
- added API for creating iBeacons [\#28](https://github.com/ARMmbed/ble/pull/28) ([BlackstoneEngineering](https://github.com/BlackstoneEngineering))
- Uribeacon update [\#25](https://github.com/ARMmbed/ble/pull/25) ([schilit](https://github.com/schilit))
- Fix README links [\#23](https://github.com/ARMmbed/ble/pull/23) ([shirishb](https://github.com/shirishb))
- Added optional data and length fields to the return struct for authorize... [\#22](https://github.com/ARMmbed/ble/pull/22) ([marcuschangarm](https://github.com/marcuschangarm))
- Chained callbacks for onDataSent [\#21](https://github.com/ARMmbed/ble/pull/21) ([marcuschangarm](https://github.com/marcuschangarm))
- Updated Readme [\#20](https://github.com/ARMmbed/ble/pull/20) ([BlackstoneEngineering](https://github.com/BlackstoneEngineering))
- Add getHandle for readability [\#18](https://github.com/ARMmbed/ble/pull/18) ([schilit](https://github.com/schilit))
- make the library less dependent on the mbed.h header [\#5](https://github.com/ARMmbed/ble/pull/5) ([xiongyihui](https://github.com/xiongyihui))
- adding initial readme with references to developer.mbed.org [\#4](https://github.com/ARMmbed/ble/pull/4) ([BlackstoneEngineering](https://github.com/BlackstoneEngineering))



\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*