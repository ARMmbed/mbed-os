#scan for folders having "Makefile" in them and remove 'this' to prevent loop
DIRS := $(filter-out ./, $(sort $(dir $(shell find . -name 'Makefile'))))

all:	
	for dir in $(DIRS); do \
		cd $$dir; make gcov; cd ..; cd ..;\
	done
	
clean:
	for dir in $(DIRS); do \
		cd $$dir; make clean; cd ..; cd ..;\
	done
	rm -rf ../source/*gcov ../source/*gcda ../source/*o
	rm -rf stubs/*gcov stubs/*gcda stubs/*o
	rm -rf results/*
	rm -rf coverages/*
	rm -rf results
	rm -rf coverages

