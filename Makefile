TEST_NR ?= ELMO_SX
ARCH ?=ELMO_F411RE

all:
	python workspace_tools/build.py -m $(ARCH) -t GCC_ARM
	python workspace_tools/make.py -m $(ARCH) -t GCC_ARM -n $(TEST_NR)

clean:
	rm -rvf build

