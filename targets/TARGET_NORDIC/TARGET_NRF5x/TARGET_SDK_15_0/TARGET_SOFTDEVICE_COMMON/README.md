# Folder origin

components/softdevice

# Modifications

Removed:
 * common/nrf_sdh_freertos.c
 * common/nrf_sdh_freertos.h
 * s212/*

Moved:
 * mbr/nrf52832/ to ../TARGET_SOFTDEVICE_S132_MBR
 * mbr/nrf52840/ to ../TARGET_SOFTDEVICE_S140_MBR
 * s112/ to ../TARGET_SOFTDEVICE_S112
 * s132/ to ../TARGET_SOFTDEVICE_S132
 * s140/ to ../TARGET_SOFTDEVICE_S140

Renamed:
 * ble.h to nrf_ble.h
