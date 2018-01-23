[![Mbed OS][mbed-os-logo]][mbed-os-link]

[![Build status release][mbed-travis-release-svg]][mbed-travis-release] 
[![Build status master][mbed-travis-master-svg]][mbed-travis-master] 
[![Tools coverage status][mbed-coveralls-tools-svg]][mbed-coveralls-tools] 
[![PR progress][mbed-waffle-svg]][mbed-waffle] 

[mbed-os-logo]: logo.png
[mbed-os-link]: https://www.mbed.com/en/platform/mbed-os/
[mbed-travis-master]: https://travis-ci.org/ARMmbed/mbed-os
[mbed-travis-master-svg]: https://travis-ci.org/ARMmbed/mbed-os.svg?branch=master
[mbed-travis-release]: https://travis-ci.org/ARMmbed/mbed-os/branches
[mbed-travis-release-svg]: https://travis-ci.org/ARMmbed/mbed-os.svg?branch=latest
[mbed-coveralls-tools]: https://coveralls.io/github/ARMmbed/mbed-os?branch=master
[mbed-coveralls-tools-svg]: https://coveralls.io/repos/github/ARMmbed/mbed-os/badge.svg?branch=master
[mbed-waffle]: https://waffle.io/ARMmbed/mbed-os
[mbed-waffle-svg]: https://badge.waffle.io/ARMmbed/mbed-os.svg?columns=all

Arm Mbed OS is an open source embedded operating system designed specifically for the "things" in the Internet of Things. It includes all the features you need to develop a connected product based on an Arm Cortex-M microcontroller, including security, connectivity, an RTOS and drivers for sensors and I/O devices.

Mbed OS provides a platform that includes:
* Security foundations.
* Cloud management services.
* Drivers for sensors, I/O devices and connectivity. 

## Release notes
The [release notes](https://os.mbed.com/releases) detail the current release. You can also find information about previous versions.

## Getting started for developers
 
We have a [developer website](https://os.mbed.com) for asking questions, engaging with others, finding information on boards and components, using an online IDE and compiler, reading the documentation and learning about what's new and what's coming next in Mbed OS.

## Getting started for contributors
 
We also have a [contributing and publishing guide](https://os.mbed.com/contributing/) that covers licensing, contributor agreements and style guidelines.
=======
# mbed-client-cli

This is the Command Line Library for a CLI application. This library provides methods for:

* Adding commands to the interpreter.
* Deleting commands from the interpreter.
* Executing commands.
* Adding command aliases to the interpreter.
* Searching command arguments.

## API

Command Line Library API is described in the snipplet below: 

```c++
// if thread safety for CLI terminal output is needed
// configure output mutex wait cb before initialization so it's available immediately
cmd_set_mutex_wait_func( (func)(void) );
// configure output mutex release cb before initialization so it's available immediately
cmd_set_mutex_wait_func( (func)(void) );
// initialize cmdline with print function
cmd_init( (func)(const char* fmt, va_list ap) );
// configure ready cb
cmd_set_ready_cb( (func)(int retcode)  );
// register command for library
cmd_add( <command>, (int func)(int argc, char *argv[]), <help>, <man>); 
//execute some existing commands
cmd_exe( <command> );
```

## Tracing

Command Line Library has trace messages, which are disabled by default.
"MBED_CLIENT_CLI_TRACE_ENABLE" flag if defined, enables all the trace prints for debugging.

## Usage example

Adding new commands to the Command Line Library and executing the commands:

```c++
//example print function
void myprint(const char* fmt, va_list ap){ vprintf(fmt, ap); }

// ready cb, calls next command to be executed
void cmd_ready_cb(int retcode) { cmd_next( retcode ); }

// dummy command with some option
int cmd_dummy(int argc, char *argv[]){
  if( cmd_has_option(argc, argv, "o") ) {
    cmd_printf("This is o option");
  } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
  }
  return CMDLINE_RETCODE_SUCCESS;
}

// timer cb (pseudo-timer-code)
void timer_ready_cb(void) {
   cmd_ready(CMDLINE_RETCODE_SUCCESS);
}

// long command, that needs for example some events to complete the command execution
int cmd_long(int argc, char *argv[] ) {
   timer_start( 5000, timer_ready_cb ); //pseudo timer code
   return CMDLINE_RETCODE_EXCUTING_CONTINUE;
}
void main(void) {
   cmd_init( &myprint );              // initialize cmdline with print function
   cmd_set_ready_cb( cmd_ready_cb );  // configure ready cb
   cmd_add("dummy", cmd_dummy, 0, 0); // add one dummy command
   cmd_add("long", cmd_long, 0, 0);   // add one dummy command
   //execute dummy and long commands
   cmd_exe( "dummy;long" );
}
```

## Thread safety
The CLI library is not thread safe, but the CLI terminal output can be locked against other 
output streams, for example if both traces and CLI output are using serial out.

```c++
static Mutex MyMutex;

// mutex wait cb, acquires the mutex, waiting if necessary
static void mutex_wait(void)
{
    MyMutex.lock();
}

// mutex release cb, releases the mutex
static void my_mutex_release(void)
{
    MyMutex.unlock();
}

void main(void) {
   cmd_mutex_wait_func( my_mutex_wait ); // configure mutex wait function before initializing
   cmd_mutex_release_func( my_mutex_release ); // configure mutex release function before initializing
   cmd_init( &myprint );              // initialize cmdline with print function
   cmd_set_ready_cb( cmd_ready_cb );  // configure ready cb.
   //CLI terminal output now locks against MyMutex
}

```
