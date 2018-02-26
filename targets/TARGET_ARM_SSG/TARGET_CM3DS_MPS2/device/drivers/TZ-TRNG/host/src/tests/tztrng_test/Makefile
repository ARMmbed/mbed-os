HOST_PROJ_ROOT ?= $(shell pwd)/../../..

# this section is normaly defined by hosr/src/config. Since this makefile is the only one published we need to define these varainbles here
ifeq ($(CROSS_COMPILE),$(filter $(CROSS_COMPILE),arm-ds5 arm-none-eabi-))
TEE_OS = freertos
TEST_BOARD = mps2+
TEST_PRODUCT = cc3x
else 
# TEE_OS = linux // Do not overwrite TEE_OS in case of linux values
TEST_BOARD = zynq
TEST_PRODUCT = cc3x
endif

ifeq ($(NO_PAL),)
NO_PAL=1
endif

include $(HOST_PROJ_ROOT)/Makefile.defs

ifeq ($(TEE_OS),$(filter $(TEE_OS),linux cc_linux))
TARGET_EXES = tztrng_test
DEPLIBS = cc_tztrng
TEE_OS=linux
else
TARGET_LIBS = tztrng_test
TEST_INCLUDES += $(CURDIR)/tztrng_test.h
endif

# This specific test should not run with linux pal layer.
ifeq ($(TEE_OS),$(filter $(TEE_OS),freertos))
include $(HOST_PROJ_ROOT)/Makefile.freertos

ifneq ($(NO_PAL),1)
include $(HOST_PROJ_ROOT)/Makefile.pal
SOURCES_tztrng_test += tztrng_test_arm.c  
# do not compile baord_configs since it is using a defile that is missong from the hw headers.
SOURCES_tztrng_test += $(filter-out board_configs.c, $(PAL_SOURCES))
endif # pal
endif # freertos

# Sources
SOURCES_tztrng_test += tztrng_test.c 
SOURCES_tztrng_test += tztrng_test_pal.c  

# Includes
INCDIRS_EXTRA += $(SHARED_DIR)/hw/include
INCDIRS_EXTRA += $(HOST_PROJ_ROOT)/src/tztrng_lib/include
INCDIRS_EXTRA += $(CURDIR)/pal
INCDIRS_EXTRA += $(CURDIR)/pal/$(TEE_OS)
ifneq ($(NO_PAL),1)
INCDIRS_EXTRA += $(HOST_LIBDIR) 
INCDIRS_EXTRA += $(HOST_SRCDIR)/hal
INCDIRS_EXTRA += $(HOST_SRCDIR)/hal/$(TEST_PRODUCT)
INCDIRS_EXTRA +=  $(HOST_SRCDIR)/tests/common
INCDIRS_EXTRA +=  $(HOST_SRCDIR)/tests/pal/include
INCDIRS_EXTRA +=  $(HOST_SRCDIR)/tests/hal/$(TEE_OS)
INCDIRS_EXTRA +=  $(HOST_SRCDIR)/tests/pal/include/$(TEE_OS)
INCDIRS_EXTRA +=  $(HOST_SRCDIR)/tests/hal/$(TEST_BOARD)/$(TEST_PRODUCT)
endif

# External use PAL.. These files are going to be given to client
VPATH += $(CURDIR)/pal
VPATH += $(CURDIR)/pal/$(TEE_OS)
# Internal use PAL
VPATH += $(HOST_SRCDIR)/tests/hal/$(TEST_BOARD)/$(TEST_PRODUCT)
VPATH += $(HOST_SRCDIR)/tests/pal/$(TEE_OS)
VPATH += $(HOST_SRCDIR)/tests/common

include $(HOST_PROJ_ROOT)/Makefile.rules
