PIT_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/pit
SOURCES += $(wildcard $(PIT_DRIVER_DIR)/src/*.c)
INCLUDES += $(PIT_DRIVER_DIR)
