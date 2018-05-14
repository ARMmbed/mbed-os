# Folder origin

components/libraries

# Modifications

Only essential folders have been copied over.

`experimental_log/nrf_log.h` has been removed and reimplemented in `TARGET_NRF5x/nrf_log.h` to 
provide optional logging via mbed stdout. Only the minimum required headers have been left in 
experimental_log folder.

Removed:
 * block_dev/
 * bootloader/
 * bsp/
 * button/
 * cli/
 * crc16/
 * crc32/
 * crypto/
 * csense/
 * csense_drv/
 * ecc/
 * eddystone/
 * experimental_mpu/
 * experimental_ringbuf/
 * experimental_stack_guard/
 * experimental_task_manager/
 * fifo/
 * gfx/
 * gpiote/
 * hardfault/
 * hci/
 * led_softblink/
 * low_power_pwm/
 * mem_manager/
 * mutex/
 * pwm/
 * pwr_mgmt/
 * scheduler/
 * sdcard/
 * sensorsim/
 * serial/
 * sha256/
 * simple_timer/
 * slip/
 * sortlist/
 * spi_mngr/
 * svc/
 * timer/
 * twi/
 * twi_mngr/
 * uart/
 * usbd/
