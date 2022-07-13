# Mbed OS upload method configuration file for target LPC1768.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)
set(MBED_RESET_BAUDRATE 115200)

# Config options for PYOCD
# -------------------------------------------------------------

# Recent PyOCD (>= 0.11) does not appear to be able to talk to LPC1768
# https://github.com/pyocd/pyOCD/issues/745
# https://github.com/pyocd/pyOCD/issues/1124

#set(PYOCD_UPLOAD_ENABLED TRUE)
#set(PYOCD_TARGET_NAME LPC1768)
#set(PYOCD_CLOCK_SPEED 4000k)

# Config options for OPENOCD
# -------------------------------------------------------------

# One note about OpenOCD for LPC1768:
# If you issue a "monitor reset" command, GDB will think that the program is halted, but it actually will have
# resumed.  So, issue a "c" command after "monitor reset" to get things synchronized again.

set(OPENOCD_UPLOAD_ENABLED TRUE)
set(OPENOCD_CHIP_CONFIG_COMMANDS
    -f ${CMAKE_CURRENT_LIST_DIR}/openocd_cfgs/lpc1768.cfg
    -c "gdb_memory_map disable" # prevents OpenOCD crash on GDB connect
	-c "gdb_breakpoint_override hard" # Make sure GDB uses HW breakpoints
    )
