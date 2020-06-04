# SPDX-License-Identifier: Apache-2.0

# TODO: @mbed-os-tools This needs to come from tools
# Build system provides "target" list.
# Note: This must be placed before subdirectories
set_property(GLOBAL PROPERTY MBED_OS_TARGET_LABELS Freescale;MCUXpresso_MCUS;KSDK2_MCUS;FRDM;KPSDK_MCUS;KPSDK_CODE;MCU_K64F;Freescale_EMAC;PSA;CORTEX_M;RTOS_M4_M7)

# TODO: @mbed-os-tools This needs to come from tools
set_property(GLOBAL PROPERTY MBED_OS_COMPONENTS "")

# Placeholder, use the same command to overwrite provide target linker script file
set_property(GLOBAL PROPERTY MBED_OS_TARGET_LINKER_FILE "")
