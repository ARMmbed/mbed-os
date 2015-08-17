TEST_NR ?= 27

all:
	python workspace_tools/build.py -m ELMO_F411RE -t GCC_ARM
	python workspace_tools/make.py -m ELMO_F411RE -t GCC_ARM -p $(TEST_NR)

clean:
	rm -rvf build

