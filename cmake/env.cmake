# SPDX-License-Identifier: Apache-2.0

# TODO: @mbed-os-tools This needs to come from tools
# Build system provides "target" list.
# Note: This must be placed before subdirectories
set(MBED_OS_TARGET_LABELS Freescale;MCUXpresso_MCUS;KSDK2_MCUS;FRDM;KPSDK_MCUS;KPSDK_CODE;MCU_K64F;Freescale_EMAC;PSA;CORTEX_M CACHE INTERNAL "")

# TODO: Follow what is for targets above, get somehow from tools what componenets/modules are enabled
set(MBED_OS_COMPONENTS CACHE INTERNAL "")
