# Folder origin

components/softdevice

# Modifications

Removed:
 * common/nrf_sdh_freertos.c
 * common/nrf_sdh_freertos.h
 * s112/toolchain
 * s132/toolchain
 * s212/

Moved:
 * common/ to ../modules/softdevice/common
 * mbr/nrf52832/ to ../TARGET_SOFTDEVICE_S132_MBR
 * mbr/nrf52840/ to ../TARGET_SOFTDEVICE_S140_MBR
 * s112/ to ../TARGET_SOFTDEVICE_S112
 * s132/ to ../TARGET_SOFTDEVICE_S132_FULL
 * s140/ to ../TARGET_SOFTDEVICE_S140_FULL

Renamed:
 * ble.h to nrf_ble.h
