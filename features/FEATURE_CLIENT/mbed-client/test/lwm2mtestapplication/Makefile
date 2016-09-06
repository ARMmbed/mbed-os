# Makefile for LWM2M Test Application
#
# Define compiler toolchain with PLATFORM variable
# Example
# make PLATFORM=arm-linux-gnueabi-
#

ROOT := ../../../..
LWM2M_ROOT := $(ROOT)/lwm2m
ROUTER_ROOT := $(ROOT)/Applications/NanoRouter

VERSION := $(shell cd $(ROOT) && git describe --tags --long --dirty --always)

# Specify list of sources to build
LWM2M_SRC := \
	main.cpp \
	cmd_commands.cpp \
	cmd_lwm2m.cpp \
	lwm2mtest.cpp \

CXX_SRCS += $(LWM2M_SRC)

HAL_SRCS += \
	linux/arm_hal_interrupt.c \
	linux/arm_hal_timer.c \
	linux/ccm_mutex.c \

SRCS += $(addprefix $(ROOT)/Platforms/,$(HAL_SRCS))

EXE = lwm2mtestapplication

override CFLAGS += -DENDPOINT -DUSE_LINUX
override CFLAGS += -Wall -Wextra
override CFLAGS += -g
override CFLAGS += -pthread
override CFLAGS += -std=gnu99
override CFLAGS += -D'MEM_ALLOC=malloc' -D'MEM_FREE=free'

override CFLAGS += -DVERSION='"$(VERSION)"'

override CXXFLAGS += -I$(ROOT) -I$(NSDL_DIR)/nsdl-c -I$(LIBSERVICE_DIR)/libService -I$(ROOT)/nanostack/nanostack -I$(ROOT)/event-loop/nanostack-event-loop
override CXXFLAGS += -I$(LWM2M_ROOT) -I$(LWM2M_ROOT)/lwm2m-client
override CXXFLAGS += -DTARGET_LIKE_LINUX
override CXXFLAGS += -DHAVE_DEBUG
override CXXFLAGS += -g
# Generate dependency files when building
override CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

# Build with coverage
ifeq ($(COVERAGE), 1)
override CFLAGS += --coverage
override CXXFLAGS += --coverage
endif

#
# Libraries
#
NSDL_DIR := $(ROOT)/nsdl-c
LWM2M_DIR := $(ROOT)/lwm2m/lwm2m-client
MBEDTLS_DIR := $(ROOT)/mbedtls/library
LIBSERVICE_DIR := $(ROOT)/libService
override CFLAGS += -I$(ROOT) -I$(NSDL_DIR)/nsdl-c -I$(LIBSERVICE_DIR)/libService -I$(ROOT)/nanostack/nanostack -I$(ROOT)/event-loop/nanostack-event-loop
override CFLAGS += -I$(ROOT)/nanostack/Core/include -I$(ROOT)/Platforms/linux/include -I$(ROOT)/nanostack/HAL/common
override CFLAGS += -I$(ROUTER_ROOT)/include
override CFLAGS += -I$(ROOT)/Platforms/linux/include -I$(ROOT)/Drivers/linux -I$(ROOT)/Boards/linux
LDFLAGS += -D_REENTRANT
LDFLAGS += -L$(ROOT)/event-loop -leventOS_gcc
LDFLAGS += -L$(LWM2M_DIR) -lmbedclient_gcc
LDFLAGS += -L$(NSDL_DIR) -lnsdl_gcc
LDFLAGS += -L$(LIBSERVICE_DIR) -lCmdline_gcc -lservice_gcc
LDFLAGS += -L$(MBEDTLS_DIR) -lmbedtls -lmbedx509 -lmbedcrypto
LDFLAGS += -lpthread


# Main build goal, specify here before dependecy parsing
.DEFAULT_GOAL: all
.PHONY: all
all: $(EXE)

# Create a list of objects and depencies to build
OBJS := $(SRCS:.c=.o)
CXX_OBJS := $(CXX_SRCS:.cpp=.o)
DEPS += $$(SRCS:.c=.d) $$(CXX_SRCS:.cpp=.d) 

# Include depencies
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

# Linking
$(EXE): $(OBJS) $(CXX_OBJS)
	@echo 'Building target: $@'
	g++ $(CXXFLAGS) $(CFLAGS) -o $@ $(OBJS) $(CXX_OBJS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

.PHONY: clean
clean:
	-$(RM) $(OBJS) $(CXX_OBJS) $(DEPS) $(EXE)
