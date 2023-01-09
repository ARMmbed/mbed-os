# Mbed OS upload method configuration file for target MIMXRT1060_EVK.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.
#
# Notes:
# 1. Using the JLINK upload method requires either converting the board's LPC-Link into a J-Link, or
#      connecting an external J-Link probe.  See here for conversion instructions: https://www.segger.com/products/debug-probes/j-link/models/other-j-links/lpc-link-2/
#      Also, a relatively new version of the J-Link software is needed (7.82 confirmed working), so try updating
#      it if you get any errors.
# 2. pyocd seems glitchy for this device.  Sometimes it works great, sometimes you get 'no ACK received.'
#      Might be related to pyOCD#861?

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT JLINK)

# Config options for JLINK
# -------------------------------------------------------------
set(JLINK_UPLOAD_ENABLED TRUE)
set(JLINK_CPU_NAME MIMXRT1062xxx5B?BankAddr=0x60000000&Loader=QSPI) # Extra parameters needed to select correct flash loader
set(JLINK_UPLOAD_INTERFACE SWD)
set(JLINK_CLOCK_SPEED 4000)

# Config options for PYOCD
# -------------------------------------------------------------
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME mimxrt1060) # Note: This seems to use QSPI.  There does not seem to be a pyocd equivalent for hyperflash.
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for REDLINK
# -------------------------------------------------------------
set(REDLINK_UPLOAD_ENABLED TRUE)
set(REDLINK_PART_NUMBER MIMXRT1062xxxxA)
set(REDLINK_PART_XML_DIR ${CMAKE_CURRENT_LIST_DIR}/redlink_cfgs)
set(REDLINK_CLOCK_SPEED 4000)
set(REDLINK_CONNECT_ARGS
	--connectscript=RT1060_connect.scp
	--reset=
	--coreindex 0
	--cache disable
	--no-packed)