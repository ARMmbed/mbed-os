# Mbed OS upload method configuration file for target NUCLEO_L152RE.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# Notes:
# 1. Using the JLINK upload method with your dev board requires converting its ST-LINK into a J-Link.  See here for details: https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/
# 2. If your target is not natively supported by the pyOCD, then you need install a keil package for family of your target by hands. Type "pyocd pack show" to console and you will see a list of already installed packages.
#   If any package for your family is not on the list, then you need install them via command "pyocd pack install stm32l1".Then just type "pyocd pack find STM32l1" or "pyocd pack find STM32l152" and you will see the part name of your target.
#   Alternqativly you can type command "pyocd list" whne your board is connected. You will see state of your board.
# 3. The STLINK upload method is disabled by default, because it is not usable on this target. ST-link keeps flashing and never stops.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME STM32L152RE)
set(JLINK_CLOCK_SPEED 4000)
set(JLINK_UPLOAD_INTERFACE SWD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME STM32L152RETx)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
-f ${OpenOCD_SCRIPT_DIR}/board/st_nucleo_l1.cfg)

# Config options for STM32Cube
# -------------------------------------------------------------

set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND -c port=SWD reset=HWrst)
set(STM32CUBE_GDBSERVER_ARGS --swd)

# Config options for stlink
# -------------------------------------------------------------

set(STLINK_UPLOAD_ENABLED FALSE)
set(STLINK_LOAD_ADDRESS 0x8000000)
set(STLINK_ARGS --connect-under-reset)
