# Mbed OS upload method configuration file for target RASPBERRY_PI_PICO (without SWD debugger)
# Currently, only picotool is supported for uploading code, which requires manually putting the board
# into bootloader mode whenever you wish to program it.
# Note: If you have a debugger connected to your board, you should be using the RASPBERRY_PI_PICO_SWD target instead.
# To change any of these parameters from their default values, set them in your build script between where you
# include app.cmake and where you add mbed os as a subdirectory.

# General config parameters
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT PICOTOOL)

# Config options for PICOTOOL
# -------------------------------------------------------------
set(PICOTOOL_UPLOAD_ENABLED TRUE)