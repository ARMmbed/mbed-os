Build time configuration of the stack
=====================================

To minimize the size of the produced stack, we have defined a set of build options.

The suitable build option depends on whether you are building it for mbed OS or for bare metal.

## Build options

Option Name | Features supported | Current binary size v2.1.13
------------| -------------------|-----------------------------
`generic` | Everything. This is only for testing purposes, not to be released. | 537 kB
`lowpan_border_router` | 6LoWPAN-ND border router. | 257 kB
`lowpan_border_router_rf_tunnel` | 6LoWPAN-ND border router using an external microcontroller running the low level MAC functionality. | 313 kB
`lowpan_host` | 6LoWPAN-ND not routing host mode. | 194 kB
`lowpan_router` | 6LoWPAN-ND routing host mode. | 230 kB
`nanostack_full` | Everything but without security features (PANA, ECC, ECJPAKE). | 382 kB
`rf_interface` | RF interface running only low level MAC. To be used with `lowpan_border_router_rf_tunnel`. | 168 kB
`thread_border_router` |  Thread router device with border router capability. | 320 kB
`thread_router` | Thread host with routing capability. | 302 kB

<span class="notes">**Note:** The binary sizes have been estimated using GCC toolchains on Linux by the time of writing this document.
They will differ based on the toolchains or the status of the repository. The final size can only be estimated when linking the final application. The indicated size only gives you a guideline of what kind of changes to expect between different options.</span>

## Using configuration option on mbed OS 5.0

For mbed OS 5.0, there will be only one binary module available. This is a temporary solution for mbed OS 5.0 toolchain and will be updated to support multiple configurations later.

The binary module for mbed OS 5.0 follows the `nanostack_full` configuration.

## Using configuration option on mbed OS 3.0

When using the Nanostack yotta module, you need to select the configuration option through the [yotta configuration system](http://yottadocs.mbed.com/reference/config.html).

Create the `config.json` file as follows:
~~~~
{
	"nanostack": {
		"configuration": "thread_router"
	}
}
~~~~

This file should be placed in your application directory.

The yotta configuration options can only be set by the application or the target definition.
Currently, none of the targets sets Nanostack options, so this needs to be done by the application.

## Using configuration option in Nanomesh/bare-metal

Build Nanostack using the `CONFIG` option for make.

For example:
~~~~
make -C nanostack CONFIG=thread_router
~~~~
