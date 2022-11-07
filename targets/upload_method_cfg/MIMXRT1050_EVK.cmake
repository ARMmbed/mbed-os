# Mbed OS upload method configuration file for target MIMXRT1050_EVK.
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
set(PYOCD_UPLOAD_ENABLED TRUE)
set(PYOCD_TARGET_NAME mimxrt1050_hyperflash) # Note: change to "mimxrt1050_quadspi" if onboard QSPI flash is used
set(PYOCD_CLOCK_SPEED 4000k)

# Config options for REDLINK
# -------------------------------------------------------------
set(REDLINK_UPLOAD_ENABLED TRUE)
set(REDLINK_PART_NUMBER MIMXRT1052xxxxB)
set(REDLINK_PART_XML_DIR ${CMAKE_CURRENT_LIST_DIR}/redlink_cfgs)
set(REDLINK_CLOCK_SPEED 4000)
set(REDLINK_CONNECT_ARGS
	--connectscript=RT1050_connect.scp
	--reset=
	--coreindex 0
	--cache disable
	--no-packed)