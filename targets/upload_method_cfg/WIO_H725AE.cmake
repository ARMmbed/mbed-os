# Mbed OS upload method configuration file for target WIO_H725AE
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. Keep in mind the WIO_H725AE does not contain any on-board debugger, so all upload methods counts with external debugger, usually ST-Link.
# 2. Using the JLINK upload method with your dev board requires converting its ST-LINK (versions lower than 3) into a J-Link.  See here for details: https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/
# 3. PyOCD reports error "No cores were discovered!" 
# 4. OpenOCD tested with ST-Link V2-1

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT STM32CUBE)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED FALSE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME STM32H725AE)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED FALSE)
set(PYOCD_TARGET_NAME STM32H725AEIx)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/wio_h725.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd)

# Config options for stlink
# -------------------------------------------------------------

set(STLINK_UPLOAD_ENABLED TRUE)
set(STLINK_LOAD_ADDRESS 0x8000000)
set(STLINK_ARGS --connect-under-reset)
