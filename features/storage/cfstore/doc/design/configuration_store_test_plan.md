# Configuration Store Test Plan

# Overview

This document describes the test plan and test cases for the configuration-store project.


# Overview of Test Binaries

The test cases are divided into a number of test case binaries:
- Access Control List (ACL) test cases implemented by acl.cpp. The ACL test binary verifies 
  the ACL related functionality is working correctly. 
- Add/Delete test cases implemented by add_del.cpp. The ADD_DEL test binary verifies
  that key-value attributes can be added/deleted to/from the store as expected using
  the Create()/Delete() API functions.
- Close test cases implemented by close.cpp. The CLOSE test binary verifies
  that the key-value Close() API function works correctly.
- Create test cases implemented by create.cpp. The CREATE test binary verifies 
  the Create() API function works correctly. 
- Find test cases implemented by find.cpp. The FIND test binary verifies 
  the Find() API function works correctly. 
- Find2 test cases implemented by find2.cpp. The FIND test binary verifies 
  the Find()/Create() API functions works correctly. 
- Flash test cases implemented by flash.cpp. The FLASH test binary verifies 
  the Flash-Journal API functions works correctly from the CFSTORE perspective. 
- Flush test cases implemented by flush.cpp. The FLUSH test binary verifies 
  the Flush() API functions works correctly. The test case uses a finite state
  machine to track asynchronous completion of operations. 
- Flush2 test cases implemented by flush2.cpp. The FLUSH2 test binary verifies 
  the Flush() API functions works correctly.  
- Flush3 test cases implemented by flush3.cpp. The FLUSH3 test binary verifies 
  the Flush() API functions works correctly.  
- Initialize/Uninitialize test cases implemented by init.cpp. The INIT test binary verifies 
  the Initialize()/Uninitialize() API functions works correctly.  
- Misc test cases implemented by misc.cpp. The MISC test binary verifies 
  the GetCapabilities()/PowerControl() API functions works correctly, for example.  
- Read test cases implemented by read.cpp. The READ test binary verifies 
  the Read()/Rseek() API functions works correctly.  
- Write test cases implemented by write.cpp. The WRITE test binary verifies 
  the Write() API function works correctly.  

There are a number of test cases that are also examples:
- Example1 test case implemented by example1.cpp. 
    - The EXAMPLE1 test binary demonstrates each API function works correctly. 
    - The example code shows how to write application code that works for both
      asynchronous and synchronous modes of operation.
    - example1.cpp can be compiled as mbed greentea test case or a stand-alone test
      application. 
- Example2 test case implemented by example2.cpp. 
    - The EXAMPLE2 test binary demonstrates a subset of the API functions each work correctly. 
    - The example code shows how to write application code that works for both
      asynchronous and synchronous modes of operation.
    - example2.cpp can be compiled only as mbed greentea test case 
- Example3 test case implemented by example3.cpp. 
    - The EXAMPLE3 test binary demonstrates each API function works correctly. 
    - The example code shows how to write application code that works for synchronous mode only.
    - example3.cpp can be compiled as mbed greentea test case or a stand-alone test
      application. 
- Example4 test case implemented by example4.cpp. 
    - The EXAMPLE2 test binary demonstrates a subset of the API functions each work correctly. 
    - The example code shows how to write application code that works for synchronous modes only.
    - example2.cpp can be compiled only as mbed greentea test case 
- Example5 test case implemented by example5.cpp. 
    - The EXAMPLE5 test binary demonstrates each API function works correctly. 
    - The example code shows how to write application code that works for synchronous mode only.
    - example5.cpp can be compiled as mbed greentea test case or a stand-alone test
      application. 

# Test Case Documentation

The individual test cases implemented in each test binary are described in the doxygen test case function
documentation. For example, for the test case documentation for ADD_DEL,
see the doxygen generated documentation for each test case function generated from the add_del.cpp file. 
Alternatively, see the comments in the add_del.cpp file.   
