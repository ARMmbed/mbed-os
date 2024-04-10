# Mbed OS upload method configuration file for target B_U585_IOT02A.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. To use this target with PyOCD, you need to install a pack: `pyocd pack install STM32U585AIIxQ`.
#    You might also need to run `pyocd pack update` first.
# 2. PyOCD 0.35 can flash this device but not debug -- it is unable to hit breakpoints in my testing.
# 3. Flashing via OpenOCD seems somewhat unreliable with this processor with OpenOCD 0.12.  It works great with
#   latest Git OpenOCD though (as of Jan 2024)

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME STM32U585AIIxQ)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/st_nucleo_u5x.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd)