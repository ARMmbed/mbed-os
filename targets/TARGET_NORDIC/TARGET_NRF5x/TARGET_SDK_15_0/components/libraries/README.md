# Folder origin

components/libraries

# Modifications

 * Modified util/app_util_platform.c to retarget critical section enter/exit calls to Mbed's HAL API so they both share the same nested critical regions counter. This only applies in builds without `SOFTDEVICE_PRESENT` defined.

Only essential folders have been copied over.

Removed:
 * atomic_flags /
 * block_dev /
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
 * experimental_libuarte/
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
 * stack_info
 * svc/
 * timer/
 * twi_sensor/
 * uart/
 * usbd/
