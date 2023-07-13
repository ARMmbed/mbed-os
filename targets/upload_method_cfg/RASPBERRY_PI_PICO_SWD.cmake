# Mbed OS upload method configuration file for target RASPBERRY_PI_PICO_SWD.
# This config file allows debugging of RPi Pico targets using an external CMSIS-DAP debugger.
# This debugger can be a Picoprobe running the new picoprobe-cmsis firmware, or
# another SWD debugger like a Pitaya-Link.

# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT OPENOCD)

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME rp2040_core0)
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${OpenOCD_SCRIPT_DIR}/interface/cmsis-dap.cfg
	-f ${OpenOCD_SCRIPT_DIR}/target/rp2040.cfg
	-c "set USE_CORE 0" # Don't pause core 1 as that causes weird effects like keeping the TIMER stuck at 0: https://github.com/raspberrypi/picoprobe/issues/45
	-c "adapter speed 4000"
)

# Config options for PICOTOOL
# -------------------------------------------------------------
set(PICOTOOL_UPLOAD_ENABLED TRUE)