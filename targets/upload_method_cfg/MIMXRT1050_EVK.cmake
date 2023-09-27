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

# Config options for LINKSERVER
# -------------------------------------------------------------
set(LINKSERVER_UPLOAD_ENABLED TRUE)
# note: might need to change the below to MIMXRT1052xxxxx:EVK-IMXRT1050 if you have an EVK rev A
set(LINKSERVER_DEVICE MIMXRT1052xxxxB:EVKB-IMXRT1050)
