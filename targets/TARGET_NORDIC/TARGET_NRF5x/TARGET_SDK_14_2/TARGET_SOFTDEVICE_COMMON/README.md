# Folder origin

components/ble
components/softdevice
components/libraries/fstorage/nrf_fstorage_sd.*

# Modifications

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
