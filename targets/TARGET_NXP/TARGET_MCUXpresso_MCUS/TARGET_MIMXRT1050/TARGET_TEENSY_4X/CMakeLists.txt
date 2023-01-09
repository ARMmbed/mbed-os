
# Target for Teensy 4.x common files
add_library(mbed-teensy-4x INTERFACE)
target_sources(mbed-teensy-4x INTERFACE
	bootdata.c)
target_include_directories(mbed-teensy-4x INTERFACE .)
target_link_libraries(mbed-teensy-4x INTERFACE
	mbed-mimxrt105x)

# Target for Teensy 4.0
add_library(mbed-teensy-40 INTERFACE)
target_link_libraries(mbed-teensy-40 INTERFACE
	mbed-teensy-4x)
target_include_directories(mbed-teensy-40 INTERFACE TARGET_TEENSY_40)

# Target for Teensy 4.1
add_library(mbed-teensy-41 INTERFACE)
target_link_libraries(mbed-teensy-41 INTERFACE
	mbed-teensy-4x)