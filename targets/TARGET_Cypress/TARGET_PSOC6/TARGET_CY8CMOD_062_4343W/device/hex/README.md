README for pre-compiled PSoC 6 Cortex M0+ core images
=====================================================

This folder contains precompiled program images for the CM0+ core of the
PSoC 6 MCU suitable for use with Mbed OS applications running on CM4 core.

* `psoc6_02_cm0p_sleep.hex`

    This image starts CM4 core at CY_CORTEX_M4_APPL_ADDR=0x10080000
    and puts CM0+ core into a deep sleep.

The images are 'bare metal' code prepared with Cypress ModusToolbox IDE
and are toolchain agnostic, i.e. can be used with CM4 Mbed applications
build with any supported toolchain.

**These images were prepared by Cypress Semiconductor Corporation
and are made available under the conditions of Permissive Binary Licence,
see file LICENSE.txt**
