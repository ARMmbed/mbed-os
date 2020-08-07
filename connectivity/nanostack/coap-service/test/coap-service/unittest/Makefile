# Copyright (c) 2015-2017, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#scan for folders having "Makefile" in them and remove 'this' to prevent loop
ifeq ($(OS),Windows_NT)
all:
clean:
else
DIRS := $(filter-out ./, $(sort $(dir $(shell find . -name 'Makefile'))))

all:	
	for dir in $(DIRS); do \
		cd $$dir; make gcov; cd ..;\
	done
	
clean:
	for dir in $(DIRS); do \
		cd $$dir; make clean; cd ..;\
	done
	rm -rf stub/*gcov stub/*gcda stubs/*o
	rm -rf results/*
	rm -rf coverages/*
	rm -rf results
	rm -rf coverages
endif

