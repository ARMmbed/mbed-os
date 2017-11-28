include ../../makefile_defines.txt

COMPONENT_NAME = 6lowpan_mesh_unit
SRC_FILES = \
	../../../../../source/6LoWPAN/Mesh/mesh.c \

TEST_SRC_FILES = \
	main.cpp \
	test_mesh.cpp \
	mesh_test.cpp \
	../../stub/6lowpan_mesh_stub.c \
	../../stub/mbed_trace_stub.c \
	../../stub/mac_helper_stub.c \
	../../stub/buffer_dyn_stub.c \
	../../stub/socket_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT
