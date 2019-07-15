# PSoC 6 Cortex M0+ prebuilt images

### Overview

Precompiled application images executed on Cortex M0+ core of the PSoC 6 dual-core MCU.
The images are provided as C arrays ready to be compiled as part of the Cortex M4 application.
The Cortex M0+ application code is placed to internal flash by the Cortex M4 linker script.

### Images

* CM0P_SLEEP

    This image starts CM4 core at CY_CORTEX_M4_APPL_ADDR=0x10008000 and puts CM0+ core into a deep sleep.

### More information
Use the following links for more information, as needed:
* [Cypress](http://www.cypress.com)
* [ModusToolbox](https://www.cypress.com/products/modustoolbox-software-environment)

---
Copyright (c) Cypress Semiconductor Corporation, 2019.
