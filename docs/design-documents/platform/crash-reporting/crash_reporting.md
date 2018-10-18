# Crash Reporting

# Table of contents

1. [Crash Reporting](#crash-reporting).
1. [Table of contents](#table-of-contents).
        1. [Revision history](#revision-history).
1. [Introduction](#introduction).
        1. [Overview and background](#overview-and-background).
        1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
        1. [System architecture and component interaction](#system-architecture-and-component-interaction).
1. [Detailed design](#detailed-design).
1. [Usage scenarios and examples](#usage-scenarios-and-examples).
1. [Tools and configuration changes](#tools-and-configuration-changes).
1. [Other information](#other-information).
        1. [Reusability](#reusability).
        1. [Deprecations](#deprecations).
        1. [References](#references).

### Revision history

1.0 - Initial version - Senthil Ramakrishnan - 10/10/2018

# Introduction

### Overview and background

Mbed-OS currently implements error and exception handlers which gets invoked when the system encounters a fatal error/exception. The error handler capture information such as register context/thread info etc and these are valuable information required to debug the issue later. This information is currently printed over the serial port, but in many cases the serial port is not accessible and the serial terminal log is not captured, particularly in the case of field deployed devices. We cannot send this information using mechanisms like Network because the state of the system might be unstable after the fatal error. And thus a different mechanism is needed to record and report this data. So, if we can auto-reboot the system after a fatal error has occured, without losing the RAM contents where we have the error information collected, we can send this information over network or other interfaces to be logged externally(E.g:- ARM Pelion cloud) or can even be written to file system if required. 

### Requirements and assumptions

This feature requires 256 bytes of dedicated RAM allocated for storing the error and fault context information.

# System architecture and high-level design

Below are the high-level design goals for "Crash Reporting" feature.

**Error information collection including exception context**

The current error handling implementation in Mbed OS already collects error and exception context. With this feature the above mentioned data structures should be placed in an uninitialized 
RAM region so that the data is retained after a auto-reboot(warm-reset).

**Mechanism to auto reboot(also called warm-reset) the system without losing RAM contents where error info is stored**

Implement auto-reboot functionality to reboot the system automatically after a fatal error. Note that the auto-reboot feature should be configurable.

**Mechanism to retrieve the error data after reboot**

Provide platform APIs to read the saved error information after reboot.

**System should implement mechanism to track number of times the system is auto-rebooted and be able to stop auto-reboot when a configurable limit is reached**

Implementation should provide a mechanism to prevent constant reboot loop by limiting the number aut-reboots. The number of times auto-reboot happens should be configurable.

**Implementation should provide following configuration options**

1. Configuration option to enable or disable this feature
1. Configuration option to enable or disable auto-reboot when the system enters a fatal error scenario
1. Configuration option to limit the number of auto-reboots
1. Configuration option to print the saved error report on reboot

### System architecture and component interaction

The below diagram shows overall architecture of crash-reporting implementation.

![System architecture and component interaction](./diagrams/crash-report1.jpg)

As depicted in the above diagram, when the system gets into fatal error state the information collected by error and fault handlers are saved into RAM space allocated for Crash-Report. This is followed by a
auto-reboot triggered from error handler. On reboot the the initialization routine validates the contents of Crash-Report RAM. This validation is for two purposes - to validate the captured content itself and also it tells the
system if the previous reboot was caused by a fatal error. It then reads this information and calls an application defined callback function passing the crash-report information. The callback is invoked just before the entry to main()
and thus the callback implementation may access libraries and other resources as other parts of the system have already initialized(like SDK, HAL etc).

# Detailed design

### Error information collection including exception context**

Current error and exception handling implementation in Mbed OS already collects error and exception context.
But currently these data structures are implemented as statically allocated memory locations. With this feature these data structures should be placed in an uninitialized RAM region 
so that the data is retained after a auto-reboot(warm-reset). So, this should be allocated as a dedicated region using linker command file(or in scatter file) for the corresponding target for each
toolchain. Also note that this region should be marked as uninitialized region using the right toolchain attributes. For example, for ARM compiler  

### Mechanism to auto reboot(also called warm-reset) the system without losing RAM contents where error info is stored**

Implement auto-reboot functionality to reboot the system automatically after a fatal error. Note that the auto-reboot feature should be configurable.

### Mechanism to retrieve the error data after reboot**

Provide platform APIs to read the saved error information after reboot.

### System should implement mechanism to track number of times the system is auto-rebooted and be able to stop auto-reboot when a configurable limit is reached**

Implementation should provide a mechanism to prevent constant reboot loop by limiting the number aut-reboots. The number of times auto-reboot happens should be configurable.

### Implementation should provide following configuration options**

1. Configuration option to enable or disable this feature
1. Configuration option to enable or disable auto-reboot when the system enters a fatal error scenario
1. Configuration option to limit the number of auto-reboots
1. Configuration option to print the saved error report on reboot


# Usage scenarios and examples

Below (pseudocode) are some common usage scenarios using the error handling APIs.

### Defining error codes

**Defining a nonfatal out-of-memory error.**

As shown in the example below, you can fabricate new error codes using MAKE_ERROR macro.

```
    char *dataPtr = malloc( datasize );
	If( dataPtr == NULL )
	{
		// Make a new error code
		uint32 errorCode = MAKE_ERROR(
        ERROR_TYPE_SYSTEM, 
        ENTITY_DRIVER,  
        ERROR_OUT_OF_MEMORY);
		set_error( errorCode, NULL, NULL );
    }
```

Below (pseudocode) are some common usage scenarios using the error handling APIs.

### Defining error codes

**Defining a nonfatal out-of-memory error.**

As shown in the example below, you can fabricate new error codes using MAKE_ERROR macro.

```
    char *dataPtr = malloc( datasize );
	If( dataPtr == NULL )
	{
		// Make a new error code
		uint32 errorCode = MAKE_ERROR(
        ERROR_TYPE_SYSTEM, 
        ENTITY_DRIVER,  
        ERROR_OUT_OF_MEMORY);
		set_error( errorCode, NULL, NULL );
    }
```

### Reporting errors

**Reporting a fatal error using set_error_fatal API**

The below sample code shows how to report a fatal error using set_error_fatal API:

```
    char *dataPtr = malloc( datasize );
	If( dataPtr == NULL )
	{
		// Make a new error code
		uint32 errorCode = MAKE_ERROR(
        ERROR_TYPE_SYSTEM, 
        ENTITY_DRIVER,  
        ERROR_OUT_OF_MEMORY);
		set_error_fatal( errorCode, NULL, NULL );
    }
```

### Setting up an error hook function

**Defining and registering an error hook function**

The below shows a definition for error hook function, followed by Error hook registration from main.

```
MbedErrorStatus MyErrorHookFunction(unsigned int error_code)
{
	printf( “New error received” %X”, error_code );
	//Do more processing
	…
}
//Main function registering the Error hook function.
Main()
{
	set_error_hook(MyErrorHookFunction);
	.. Continue
}
```

### Getting error info

**Getting error info using get_error_log_count and get_error calls.**

The below sample code shows how to retrieve all the recorded error codes from the error log.
int totalErrorsInLog = get_error_log_count();
for( index = 0; index < totalErrorsInLog; index++)
{
	printf( “Error %d: Code = 0X%08X”, get_error(index));
}
Reporting errors

**Reporting a fatal error using set_error_fatal API**

The below sample code shows how to report a fatal error using set_error_fatal API.
char *dataPtr = malloc( datasize );
	If( dataPtr == NULL )
	{
		// Make a new error code
		uint32 errorCode = MAKE_ERROR(
ERROR_TYPE_SYSTEM, 
ENTITY_DRIVER,  
ERROR_OUT_OF_MEMORY);
		set_error_fatal( errorCode, NULL, NULL );
}

### Setting up an error hook function

**Defining and registering an error hook function**

The below shows a definition for error hook function, followed by Error hook registration from main.
MbedErrorStatus MyErrorHookFunction(unsigned int error_code)
{
	printf( “New error received” %X”, error_code );
	//Do more processing
	…
}
//Main function registering the Error hook function.
Main()
{
	set_error_hook(MyErrorHookFunction);
	.. Continue
}

### Getting error info

**Getting error info using get_error_log_count and get_error calls.**

The below sample code shows how to retrieve all the recorded error codes from the error log.
int totalErrorsInLog = get_error_log_count();
for( index = 0; index < totalErrorsInLog; index++)
{
	printf( “Error %d: Code = 0X%08X”, get_error(index));
}

# Tools and configuration changes

### Platform configuration options for error handling infrastruture

Below is the list of new configuration options added to configure error handling functionality. All of these options are capture in mbed_lib.json file in platform folder.

**error-all-threads-info**

Enabling this reports all the threads in the system as part of error report.

**error-filename-capture-enabled**

Enables capture of filename and line number as part of error context capture, this works only for debug and develop builds. On release builds, filename capture is always disabled

**error-hist-enabled**

Enable this option for error history tracking.

**error-hist-size**

This options sets the number of most recent errors the system keeps in its history, needs error-hist-enabled set to true for this to work.

**max-error-filename-len**

Sets the maximum length of buffer used for capturing the filename in error context. This needs error-filename-capture-enabled feature.

# Other information

### Reusability

The error handling implementation is very generic that other components should no longer need to implement their own error codes or handling. For example, fault exception handling implements part of error handling (like halting the system) which is no longer needed and can be switched to use common error handling which handles system behavior on a fatal error.

### Deprecations

### References



