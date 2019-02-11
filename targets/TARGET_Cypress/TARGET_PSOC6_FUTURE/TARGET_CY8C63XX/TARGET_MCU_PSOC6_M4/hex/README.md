README for pre-compiled PSoC 6 Cortex M0+ core images
=====================================================

This folder contains pre-compiled program images for the CM0+ core of the PSoC 6(63xx) MCU suitable for use with MBed OS applications running on CM4 core. Two images are available:

* `psoc63_m0_default_1.02.hex`

    This image contains basic code, that brings up the chip, starts CM4 core and puts CM0+ core into a deep sleep. It is suitable for use with all Mbed applications except those intendif to use BLE feature.

* `psoc63_m0_ble_controller_1.05.hex`

    This image brings up the chip, starts CM4 core and runs BLE HCI controller for the build-in BLE device. This image is suitable for use with Mbed BLE applications.

The images are 'bare metal' code prepared with Cypress PSoC Creator IDE and are toolchain agnostic, i.e. can be used with CM4 Mbed applications build with any supported toolchain.

**These images were prepared by Future Electronics and are made available under the conditions of Permissive Binary Licence, see file LICENSE.txt**
