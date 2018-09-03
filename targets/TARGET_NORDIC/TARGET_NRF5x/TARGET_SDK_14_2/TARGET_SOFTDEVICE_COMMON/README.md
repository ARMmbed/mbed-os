# Folder origin

components/ble
components/softdevice
components/libraries/fstorage/nrf_fstorage_sd.*

# Modifications

The linker definitions for the start of ram in `softdevice/common/nrf_sdh_ble.c` 
have been updated to the start of the mbed nvic section in ram as this 
equals the end of softdevice ram.

Removed:
 * common/nrf_sdh_freertos.c
 * common/nrf_sdh_freertos.h
 * s212/*
 * s332/*

Moved:
 * s112/ to ../TARGET_SOFTDEVICE_S112
 * s132/ to ../TARGET_SOFTDEVICE_s132
 * s140/ to ../TARGET_SOFTDEVICE_s140

Renamed:
 * ble.h to nrf_ble.h
