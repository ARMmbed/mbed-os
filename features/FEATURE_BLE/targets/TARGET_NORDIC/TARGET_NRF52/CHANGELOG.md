# Change Log

## [v2.5.3](https://github.com/ARMmbed/ble-nrf51822/tree/v2.5.3) (2016-02-16)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.5.2...v2.5.3)

**Merged pull requests:**

- Fix for compilation errors with S110 softdevice in btle.cpp [\#109](https://github.com/ARMmbed/ble-nrf51822/pull/109) ([ddavidebor](https://github.com/ddavidebor))

## [v2.5.2](https://github.com/ARMmbed/ble-nrf51822/tree/v2.5.2) (2016-02-16)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.5.1...v2.5.2)

**Merged pull requests:**

- Sync develop against master [\#113](https://github.com/ARMmbed/ble-nrf51822/pull/113) ([pan-](https://github.com/pan-))
- Fix incorrect handles of characteristics descriptors. [\#112](https://github.com/ARMmbed/ble-nrf51822/pull/112) ([pan-](https://github.com/pan-))

## [v2.5.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.5.1) (2016-01-27)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.5.0...v2.5.1)

**Merged pull requests:**

- Remove Gap::state updates from this module [\#108](https://github.com/ARMmbed/ble-nrf51822/pull/108) ([andresag01](https://github.com/andresag01))
- merge version  [\#106](https://github.com/ARMmbed/ble-nrf51822/pull/106) ([pan-](https://github.com/pan-))

## [v2.5.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.5.0) (2016-01-12)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.4.1...v2.5.0)

**Merged pull requests:**

- Fix access to enum member [\#105](https://github.com/ARMmbed/ble-nrf51822/pull/105) ([pan-](https://github.com/pan-))
- Hotfix dependency [\#104](https://github.com/ARMmbed/ble-nrf51822/pull/104) ([pan-](https://github.com/pan-))
- Finish implementation of getAddressesFromBondTable [\#103](https://github.com/ARMmbed/ble-nrf51822/pull/103) ([andresag01](https://github.com/andresag01))

## [v2.4.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.4.1) (2016-01-11)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.4.0...v2.4.1)

**Merged pull requests:**

- merge branch develop \(v2.4.0\) [\#100](https://github.com/ARMmbed/ble-nrf51822/pull/100) ([pan-](https://github.com/pan-))

## [v2.4.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.4.0) (2016-01-10)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.3.1...v2.4.0)

**Merged pull requests:**

- Add implementation of experimental whitelisting API [\#99](https://github.com/ARMmbed/ble-nrf51822/pull/99) ([andresag01](https://github.com/andresag01))

## [v2.3.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.3.1) (2016-01-07)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.3.0...v2.3.1)

**Merged pull requests:**

- Update yotta module dependencies [\#98](https://github.com/ARMmbed/ble-nrf51822/pull/98) ([pan-](https://github.com/pan-))

## [v2.3.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.3.0) (2015-12-23)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.10...v2.3.0)

**Merged pull requests:**

- Implementation of Characteristic descriptor discovery [\#74](https://github.com/ARMmbed/ble-nrf51822/pull/74) ([pan-](https://github.com/pan-))

## [v2.2.10](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.10) (2015-12-23)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.9...v2.2.10)

**Fixed bugs:**

- nRF5xn::init don't verify if errors have occurred during btle\_init [\#59](https://github.com/ARMmbed/ble-nrf51822/issues/59)

**Closed issues:**

- A call to shutdown does not clear the state of some components of BLE API [\#85](https://github.com/ARMmbed/ble-nrf51822/issues/85)
- Memory allocation issue on the NRF51DK board. [\#76](https://github.com/ARMmbed/ble-nrf51822/issues/76)
- Terrible handling of initLen / minLen and variable length characteristics. [\#56](https://github.com/ARMmbed/ble-nrf51822/issues/56)

**Merged pull requests:**

- Fix shutdown of Gap instance to avoid NULL refs [\#96](https://github.com/ARMmbed/ble-nrf51822/pull/96) ([andresag01](https://github.com/andresag01))
- Add check for return code of ble\_init [\#95](https://github.com/ARMmbed/ble-nrf51822/pull/95) ([andresag01](https://github.com/andresag01))

## [v2.2.9](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.9) (2015-12-18)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.8...v2.2.9)

**Closed issues:**

- Cannot open source input file "system\_nrf51.h" [\#52](https://github.com/ARMmbed/ble-nrf51822/issues/52)

**Merged pull requests:**

- Remove occurrence of deprecated appearance enum [\#92](https://github.com/ARMmbed/ble-nrf51822/pull/92) ([andresag01](https://github.com/andresag01))

## [v2.2.8](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.8) (2015-12-16)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.7...v2.2.8)

## [v2.2.7](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.7) (2015-12-15)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.6...v2.2.7)

**Merged pull requests:**

- Replace deprecated inclusions of mbed.h [\#89](https://github.com/ARMmbed/ble-nrf51822/pull/89) ([andresag01](https://github.com/andresag01))
- Improve shutdown to clear BLE API and not just SD [\#87](https://github.com/ARMmbed/ble-nrf51822/pull/87) ([andresag01](https://github.com/andresag01))

## [v2.2.6](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.6) (2015-12-15)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.5...v2.2.6)

**Merged pull requests:**

- follow the extraction of address related types from Gap.h into BLEProtocol.h [\#88](https://github.com/ARMmbed/ble-nrf51822/pull/88) ([rgrover](https://github.com/rgrover))

## [v2.2.5](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.5) (2015-12-11)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.3...v2.2.5)

**Merged pull requests:**

- Added SecurityManager::setLinkSecurity call for elevating security settings on a particular connection. [\#86](https://github.com/ARMmbed/ble-nrf51822/pull/86) ([marcuschangarm](https://github.com/marcuschangarm))

## [v2.2.3](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.3) (2015-12-10)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.2...v2.2.3)

## [v2.2.2](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.2) (2015-12-08)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.1...v2.2.2)

**Merged pull requests:**

- Add -Wno-unused-function to supress-warnings.cmake [\#83](https://github.com/ARMmbed/ble-nrf51822/pull/83) ([andresag01](https://github.com/andresag01))

## [v2.2.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.1) (2015-12-08)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.2.0...v2.2.1)

**Merged pull requests:**

- WIP: UUID endian change [\#82](https://github.com/ARMmbed/ble-nrf51822/pull/82) ([marcuschangarm](https://github.com/marcuschangarm))

## [v2.2.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.2.0) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.1.4...v2.2.0)

## [v2.1.4](https://github.com/ARMmbed/ble-nrf51822/tree/v2.1.4) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.1.3...v2.1.4)

## [v2.1.3](https://github.com/ARMmbed/ble-nrf51822/tree/v2.1.3) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.1.2...v2.1.3)

## [v2.1.2](https://github.com/ARMmbed/ble-nrf51822/tree/v2.1.2) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.1.1...v2.1.2)

**Merged pull requests:**

- Allow GattAttributes to have variable length [\#81](https://github.com/ARMmbed/ble-nrf51822/pull/81) ([andresag01](https://github.com/andresag01))

## [v2.1.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.1.1) (2015-12-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.1.0...v2.1.1)

**Merged pull requests:**

- Fixed endianness bug in nRF5xServiceDiscovery::processDiscoverUUIDResponse so it is consistent with BLE API. [\#80](https://github.com/ARMmbed/ble-nrf51822/pull/80) ([marcuschangarm](https://github.com/marcuschangarm))
- Fixed bug in nRF5xGap.setAddress where random adresses where not set properly. [\#79](https://github.com/ARMmbed/ble-nrf51822/pull/79) ([marcuschangarm](https://github.com/marcuschangarm))
- Separate concept of minlen and len for BLE chars [\#78](https://github.com/ARMmbed/ble-nrf51822/pull/78) ([andresag01](https://github.com/andresag01))
- Split nordic sdk into its own module [\#75](https://github.com/ARMmbed/ble-nrf51822/pull/75) ([LiyouZhou](https://github.com/LiyouZhou))

## [v2.1.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.1.0) (2015-11-27)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.8...v2.1.0)

**Merged pull requests:**

- Update to sdk 8.1 [\#77](https://github.com/ARMmbed/ble-nrf51822/pull/77) ([LiyouZhou](https://github.com/LiyouZhou))

## [v2.0.8](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.8) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.7...v2.0.8)

## [v2.0.7](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.7) (2015-11-26)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.6...v2.0.7)

**Closed issues:**

- test2000 [\#72](https://github.com/ARMmbed/ble-nrf51822/issues/72)
- test1000000 [\#71](https://github.com/ARMmbed/ble-nrf51822/issues/71)
- test4 [\#70](https://github.com/ARMmbed/ble-nrf51822/issues/70)
- test3 [\#69](https://github.com/ARMmbed/ble-nrf51822/issues/69)
- test2 [\#68](https://github.com/ARMmbed/ble-nrf51822/issues/68)

**Merged pull requests:**

- use Extern c around \#include to use nordic sdk headers implemented in C [\#73](https://github.com/ARMmbed/ble-nrf51822/pull/73) ([LiyouZhou](https://github.com/LiyouZhou))

## [v2.0.6](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.6) (2015-11-17)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.5...v2.0.6)

**Closed issues:**

- test [\#66](https://github.com/ARMmbed/ble-nrf51822/issues/66)

**Merged pull requests:**

- add Nordic's license agreement. [\#67](https://github.com/ARMmbed/ble-nrf51822/pull/67) ([rgrover](https://github.com/rgrover))

## [v2.0.5](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.5) (2015-11-16)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.4...v2.0.5)

**Merged pull requests:**

- Post radio notification callback through minar [\#65](https://github.com/ARMmbed/ble-nrf51822/pull/65) ([andresag01](https://github.com/andresag01))

## [v2.0.4](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.4) (2015-11-13)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.3...v2.0.4)

**Merged pull requests:**

- Fix assembly sequence to start bootloader in GCC [\#64](https://github.com/ARMmbed/ble-nrf51822/pull/64) ([andresag01](https://github.com/andresag01))

## [v2.0.3](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.3) (2015-11-09)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.2...v2.0.3)

**Merged pull requests:**

- Added watchdog header file from Nordic SDK 8.1 [\#62](https://github.com/ARMmbed/ble-nrf51822/pull/62) ([marcuschangarm](https://github.com/marcuschangarm))

## [v2.0.2](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.2) (2015-11-03)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/mbedos-release-15-11...v2.0.2)

## [mbedos-release-15-11](https://github.com/ARMmbed/ble-nrf51822/tree/mbedos-release-15-11) (2015-11-03)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.1...mbedos-release-15-11)

## [v2.0.1](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.1) (2015-11-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v2.0.0...v2.0.1)

**Merged pull requests:**

- Ensure that the initialization flags is set to false if the BLE stack is shutdown properly. [\#58](https://github.com/ARMmbed/ble-nrf51822/pull/58) ([pan-](https://github.com/pan-))

## [v2.0.0](https://github.com/ARMmbed/ble-nrf51822/tree/v2.0.0) (2015-11-02)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v1.1.0...v2.0.0)

**Closed issues:**

- Nordic SDK and SoftDevice [\#57](https://github.com/ARMmbed/ble-nrf51822/issues/57)
- shouldn't eab6631cb be merged into master? [\#54](https://github.com/ARMmbed/ble-nrf51822/issues/54)

**Merged pull requests:**

- Introduced changes for memory savings [\#55](https://github.com/ARMmbed/ble-nrf51822/pull/55) ([andresag01](https://github.com/andresag01))

## [v1.1.0](https://github.com/ARMmbed/ble-nrf51822/tree/v1.1.0) (2015-10-28)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v1.0.0...v1.1.0)

**Closed issues:**

- target dependencies in module.json [\#50](https://github.com/ARMmbed/ble-nrf51822/issues/50)

**Merged pull requests:**

- When connecting, if no scanning parameters are passed, use values from Gap parent. [\#53](https://github.com/ARMmbed/ble-nrf51822/pull/53) ([marcuschangarm](https://github.com/marcuschangarm))

## [v1.0.0](https://github.com/ARMmbed/ble-nrf51822/tree/v1.0.0) (2015-10-19)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/mbedos-techcon-oob2...v1.0.0)

## [mbedos-techcon-oob2](https://github.com/ARMmbed/ble-nrf51822/tree/mbedos-techcon-oob2) (2015-10-19)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.8...mbedos-techcon-oob2)

**Closed issues:**

- rename the bootloader files with \_fota in the name? [\#51](https://github.com/ARMmbed/ble-nrf51822/issues/51)

**Merged pull requests:**

- Update S110 detection macros, again [\#49](https://github.com/ARMmbed/ble-nrf51822/pull/49) ([jpbrucker](https://github.com/jpbrucker))
- Error check number of characteristics [\#48](https://github.com/ARMmbed/ble-nrf51822/pull/48) ([Timmmm](https://github.com/Timmmm))

## [v0.4.8](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.8) (2015-09-25)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.7...v0.4.8)

**Closed issues:**

- Error real cause loss in nRF5xGattServer.cpp [\#44](https://github.com/ARMmbed/ble-nrf51822/issues/44)

**Merged pull requests:**

- rgrover patch fixed [\#47](https://github.com/ARMmbed/ble-nrf51822/pull/47) ([fabiencomte](https://github.com/fabiencomte))
- Update S110 detection macros [\#43](https://github.com/ARMmbed/ble-nrf51822/pull/43) ([jpbrucker](https://github.com/jpbrucker))
- remove some unnecessary include paths [\#42](https://github.com/ARMmbed/ble-nrf51822/pull/42) ([autopulated](https://github.com/autopulated))
- Add FOTA bootloader image [\#41](https://github.com/ARMmbed/ble-nrf51822/pull/41) ([jpbrucker](https://github.com/jpbrucker))

## [v0.4.7](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.7) (2015-08-13)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.6...v0.4.7)

## [v0.4.6](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.6) (2015-08-11)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.5...v0.4.6)

**Closed issues:**

- remove duplication of global static variable BLE\_EVT\_BUFFER [\#39](https://github.com/ARMmbed/ble-nrf51822/issues/39)
- clearScanResponse\(\)  [\#30](https://github.com/ARMmbed/ble-nrf51822/issues/30)
- Debug builds fail due to missing bsp.h [\#11](https://github.com/ARMmbed/ble-nrf51822/issues/11)

**Merged pull requests:**

- Disable GattClient features when using S110 SoftDevice [\#38](https://github.com/ARMmbed/ble-nrf51822/pull/38) ([jpbrucker](https://github.com/jpbrucker))

## [v0.4.5](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.5) (2015-08-10)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.4...v0.4.5)

## [v0.4.4](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.4) (2015-08-07)
[Full Changelog](https://github.com/ARMmbed/ble-nrf51822/compare/v0.4.3...v0.4.4)

**Closed issues:**

- nrf51822 hangs after calling sd\_flash\_page\_erase\(\) [\#35](https://github.com/ARMmbed/ble-nrf51822/issues/35)
- nRF5xn::getVersion return \(Unknown\) with version 8 soft device [\#29](https://github.com/ARMmbed/ble-nrf51822/issues/29)

**Merged pull requests:**

- Changed Gap:: to GapAdvertisingParams:: because of change in BLE [\#34](https://github.com/ARMmbed/ble-nrf51822/pull/34) ([jslater8](https://github.com/jslater8))
- Select the clock source dynamically on SoftDevice initialisation [\#32](https://github.com/ARMmbed/ble-nrf51822/pull/32) ([jpbrucker](https://github.com/jpbrucker))
- Add S110 SoftDevice compatibility [\#28](https://github.com/ARMmbed/ble-nrf51822/pull/28) ([jpbrucker](https://github.com/jpbrucker))

## [v0.4.3](https://github.com/ARMmbed/ble-nrf51822/tree/v0.4.3) (2015-07-22)
**Closed issues:**

- Target polling failed [\#24](https://github.com/ARMmbed/ble-nrf51822/issues/24)
- support  handling of HVX Events \(notifications and indications\). [\#22](https://github.com/ARMmbed/ble-nrf51822/issues/22)
- provide an implementation for GattServer::areUpdatesEnabled\(\) [\#21](https://github.com/ARMmbed/ble-nrf51822/issues/21)
- getValueHandle\(\) returns characteristicIndex instead of attribute-handle [\#20](https://github.com/ARMmbed/ble-nrf51822/issues/20)
- Clash With Definition And Enum Naming [\#16](https://github.com/ARMmbed/ble-nrf51822/issues/16)
- Errors in GCC build [\#14](https://github.com/ARMmbed/ble-nrf51822/issues/14)
- bring s110 support back [\#10](https://github.com/ARMmbed/ble-nrf51822/issues/10)
- Allow adding a User Description descriptor to a GattCharacteristic. [\#9](https://github.com/ARMmbed/ble-nrf51822/issues/9)
- device\_manager\_peripheral.c includes app\_trace.h [\#7](https://github.com/ARMmbed/ble-nrf51822/issues/7)
- linking esb\_gcc.a \(nrf51822 enhanced shock burst\) with mbed [\#5](https://github.com/ARMmbed/ble-nrf51822/issues/5)
- The app\_timer usage may conflict [\#2](https://github.com/ARMmbed/ble-nrf51822/issues/2)
- Nordic License [\#1](https://github.com/ARMmbed/ble-nrf51822/issues/1)

**Merged pull requests:**

- Develop [\#25](https://github.com/ARMmbed/ble-nrf51822/pull/25) ([zoujixing](https://github.com/zoujixing))
- Remove unnecessary 'compiler\_abstraction.h' to get rid of duplicate '… [\#23](https://github.com/ARMmbed/ble-nrf51822/pull/23) ([adfernandes](https://github.com/adfernandes))
- restructure for minimal yotta compatibility [\#15](https://github.com/ARMmbed/ble-nrf51822/pull/15) ([autopulated](https://github.com/autopulated))
- Fix various GCC compilation issues. [\#12](https://github.com/ARMmbed/ble-nrf51822/pull/12) ([adfernandes](https://github.com/adfernandes))
- Fix for GCC lost in SDK v8.0 update [\#8](https://github.com/ARMmbed/ble-nrf51822/pull/8) ([rosterloh](https://github.com/rosterloh))
- new target DELTA\_DFCM\_NNN40 with nrf51822 chip, config internal RC crystal. [\#6](https://github.com/ARMmbed/ble-nrf51822/pull/6) ([Marcomissyou](https://github.com/Marcomissyou))
- Updated return value for nRF51GattServer::updateValue. Will now report w... [\#4](https://github.com/ARMmbed/ble-nrf51822/pull/4) ([marcuschangarm](https://github.com/marcuschangarm))
- Added optional data and length fields to the return struct for authorize... [\#3](https://github.com/ARMmbed/ble-nrf51822/pull/3) ([marcuschangarm](https://github.com/marcuschangarm))



\* *This Change Log was automatically generated by [github_changelog_generator](https://github.com/skywinder/Github-Changelog-Generator)*