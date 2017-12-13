HOST_PROJ_ROOT ?= $(shell pwd)/../..
include $(HOST_PROJ_ROOT)/Makefile.defs

TARGET_LIBS = cc_tztrng

CFLAGS += -DCC_HW_VERSION=$(CC_HW_VERSION)
CFLAGS += -DCC_TZ_TRNG

#TZ-TRNG debug
CFLAGS += -DTZTRNG_DEBUG
#CFLAGS += -DTZTRNG_EHR_DUMP

SOURCES_cc_tztrng = tztrng_driver.c
#platform dependent file
SOURCES_cc_tztrng += tztrng_pal.c
# Random file
SOURCES_cc_tztrng += llf_rnd_common.c

INCDIRS_EXTRA = ./include
INCDIRS_EXTRA += $(SHARED_DIR)/hw/include

# MODE defined in proj.ext.cfg
ifeq ($(CC_CONFIG_TRNG_MODE),0)
    # Slow TRNG
    $(info Slow TRNG: CC_CONFIG_TRNG_MODE=$(CC_CONFIG_TRNG_MODE))
    SOURCES_cc_tztrng += llf_rnd_fetrng.c
    CFLAGS += -DCHECK_VN_AC_ERR
	CFLAGS_EXTRA += -DCC_CONFIG_TRNG_MODE=$(CC_CONFIG_TRNG_MODE)
else ifeq ($(CC_CONFIG_TRNG_MODE),1)
    # TRNG90B
    $(info TRNG90B: CC_CONFIG_TRNG_MODE=$(CC_CONFIG_TRNG_MODE))
    SOURCES_cc_tztrng += llf_rnd_trng90b.c llf_rnd_cont.c
	CFLAGS_EXTRA += -DCC_CONFIG_TRNG_MODE=$(CC_CONFIG_TRNG_MODE)
else
    $(error illegal TRNG: CC_CONFIG_TRNG_MODE=$(CC_CONFIG_TRNG_MODE))
endif

include $(HOST_PROJ_ROOT)/Makefile.rules
