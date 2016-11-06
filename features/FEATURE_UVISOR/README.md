# Quick-Start Guide for uVisor on mbed OS

This guide will help you get started with uVisor on mbed OS by walking you through creating a sample application for the NXP FRDM-K64F board.

The uVisor provides sandboxed environments and resources protection for applications built for ARM Cortex-M3 and Cortex-M4 devices. Here we will show you how to enable the uVisor and configure a secure box to get hold of some exclusive resources (memory, peripherals, interrupts). For more information on the uVisor design philosophy, please check out our the uVisor [introductory document](../../README.md).

## Overview

To get a basic `blinky` application running on mbed OS with uVisor enabled, you will need the following:

* A platform and a toolchain supported by uVisor on mbed OS. You can verify this on [the official list](../../README.md#supported-platforms). Please note that uVisor might support some platform internally, but not on mbed OS. Generally this means that the porting process has only been partially completed. If you want to port your platform to uVisor and enable it on mbed OS, please follow the [uVisor Porting Guide for mbed OS](../core/PORTING.md).
* git. It will be used to download the mbed codebase.
* The mbed command-line tools, mbed-cli. You can run `pip install mbed-cli` to install them.

For the remainder of this guide we will assume the following:

* You are developing on a \*nix machine, in the `~/code` folder.
* You are building the app for the [NXP FRDM-K64F](http://developer.mbed.org/platforms/FRDM-K64F/) target, with the [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded) toolchain.

The instructions provided can be easily generalized to the case of other targets on other host OSs.

## Start with the `blinky` app
[Go to top](#overview)

To create a new mbed application called `uvisor-example` just run the following commands:

```bash
$ cd ~/code
$ mbed new uvisor-example
```

The mbed-cli tools will automatically fetch the mbed codebase for you. By default, git will be used to track your code changes, so your application will be ready to be pushed to a git server, if you want to.

Once the import process is finished, create a `source` folder:
```bash
$ mkdir ~/code/uvisor-example/source
```
and place a new file `main.cpp` in it:

```C
/* ~/code/uvisor-example/source/main.cpp */

#include "mbed.h"
#include "rtos.h"

DigitalOut led(LED1);

int main(void)
{
    while (true) {
        led = !led;
        Thread::wait(500);
    }
}
```

This simple application just blinks an LED from the main thread, which is created by default by the OS.

---

**Checkpoint**

Compile the application:

```bash
$ mbed compile -m K64F -t GCC_ARM
```

The resulting binary will be located at:

```bash
~/code/uvisor-example/.build/K64F/GCC_ARM/uvisor-example.bin
```

Drag-and-drop it onto the USB device mounted on your computer in order to flash the device. When the flashing process is completed, press the reset button on the device. You should see the device LED blinking.

---

In the next sections you will see:

* How to [enable uVisor](#enable-uvisor) on the `uvisor-example` app.
* How to [add a secure box](#add-a-secure-box) to the `uvisor-example` app with exclusive access to a timer, to a push-button interrupt, and to static and dynamic memories.

## Enable uVisor
[Go to top](#overview)

To enable the uVisor on the app, just add the following lines at the beginning of the `main.cpp` file:

```C
/* ~/code/uvisor-example/source/main.cpp */

#include "mbed.h"
#include "rtos.h"
#include "uvisor-lib/uvisor-lib.h"

/* Register privleged system hooks.
 * This is a system-wide configuration and it is independent from the app, but
 * for the moment it needs to be specified in the app. This will change in a
 * later version: The configuration will be provided by the OS. */
extern "C" void SVC_Handler(void);
extern "C" void PendSV_Handler(void);
extern "C" void SysTick_Handler(void);
extern "C" uint32_t rt_suspend(void);
UVISOR_SET_PRIV_SYS_HOOKS(SVC_Handler, PendSV_Handler, SysTick_Handler, rt_suspend);

/* Main box Access Control Lists (ACLs). */
/* Note: These are specific to the NXP FRDM-K64F board. See the section below
 *       for more information. */
static const UvisorBoxAclItem g_main_box_acls[] = {
    /* For the LED */
    {SIM,   sizeof(*SIM),   UVISOR_TACLDEF_PERIPH},
    {PORTB, sizeof(*PORTB), UVISOR_TACLDEF_PERIPH},

    /* For messages printed on the serial port. */
    {OSC,   sizeof(*OSC),   UVISOR_TACLDEF_PERIPH},
    {MCG,   sizeof(*MCG),   UVISOR_TACLDEF_PERIPH},
    {UART0, sizeof(*UART0), UVISOR_TACLDEF_PERIPH},
};

/* Enable uVisor, using the ACLs we just created. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_box_acls);

/* Rest of the existing app code */
...
```

In the code above we specified 3 elements:

1. System-wide uVisor configurations: `UVISOR_SET_PRIV_SYS_HOOKS`. Application authors currently need to specify the privileged system hooks at the application level with this macro, but in the future the operating system will register the privileged system hooks on its own.
1. Main box Access Control Lists (ACLs). Since with uVisor enabled everything runs in unprivileged mode, we need to make sure that peripherals that are accessed by the OS and the main box are allowed. These peripherals are specified using a list like the one in the snippet above. For the purpose of this example we provide you the list of all the ACLs that we know you will need. For other platforms or other applications you need to determine those ACLs following a process that is described in a [section](#the-main-box-acls) below.
1. App-specific uVisor configurations: `UVISOR_SET_MODE_ACL`. This macro sets the uVisor mode (enabled) and associates the list of ACLs we just created with the main box.

Before compiling, we need to override the original `K64F` target to enable the uVisor feature. To do so, add the file `~/code/uvisor-example/mbed_app.json` with the following content:

```JSON
{
    "target_overrides": {
        "K64F": {
            "target.features_add": ["UVISOR"],
            "target.extra_labels_add": ["UVISOR_SUPPORTED"]
        }
    },
    "macros": [
      "FEATURE_UVISOR",
      "TARGET_UVISOR_SUPPORTED"
    ]
}
```

The macros `FEATURE_UVISOR` and `TARGET_UVISOR_SUPPORTED` in the configuration file above are automatically defined for C and C++ files, but not for assembly files. Since the uVisor relies on those symbols in some assembly code, we need to define them manually.

---

**Checkpoint**

Compile the application again. This time the `K64F` target will include the new features and labels we provided in `mbed_app.json`;

```bash
$ mbed compile -m K64F -t GCC_ARM
```

The binary will be located at:

```bash
~/code/uvisor-example/.build/K64F/GCC_ARM/uvisor-example.bin
```

Re-flash the device and press the reset button. The device LED should be blinking as in the previous case.

---

If you enable uVisor in the `blinky` app as it was written above, you will not get any particular security feature. All code and resources share the same security context, which we call the *main box*.

A lot happens under the hood, though. All the user code now runs in unprivileged mode, and the systems services like the `NVIC` APIs or the OS SVCalls are routed through the uVisor.

## Add a secure box
[Go to top](#overview)

Now that uVisor is enabled, we can finally add a *secure box*.

A secure box is a special compartment that is granted exclusive access to peripherals, memories and interrupts. Private resources are only accessible when the *context* of the secure box is active. The uVisor is the only one that can enable a secure box context, for example upon thread switching or interrupt handling.

Code that belongs to a box is not obfuscated by uVisor, so it is still readable and executable from outside of the box. In addition, declaring an object in the same file that configures a secure box does not protect that object automatically.

Instead, we provide specific APIs to instruct the uVisor to protect a private resource. Here we will show how to use these APIs in the `uvisor-example` app.

### Configure the secure box

For this example, we want to create a secure box called `private_timer`. The `private_timer` box will be configured to have exclusive access to the PIT timer and to the GPIO PORT C on the NXP FRDM-K64F board, which means that other boxes will be prevented from accessing these peripherals.

Each secure box must have at least one thread, which we call the box's main thread. In our `private_timer` box we will only use this thread throughout the whole program. The thread will constantly save the current timer value in a private buffer. In addition, we want to print the content of the buffered timer values whenever we press the `SW2` button on the board.

We want the box to have exclusive access to the following resources:

* The timer and push-button peripherals (as specified by a peripheral ACL). Nobody else should be able to read the timer values.
* The push-button interrupt (as specified by an IRQ ACL). We want the button IRQ to be re-routed to our box-specific ISR.
* The buffer that holds the timer samples (as specified by a dynamic memory ACL).
* The static memory that holds information about the timer buffer (as specified by a static memory ACL).

Create a new source file, `~/code/uvisor-example/source/secure_box.cpp`. We will configure the secure box inside this file. The secure box name for this example is `private_timer`.

```C
/* ~/code/uvisor-example/source/secure_box.cpp */

#include "mbed.h"
#include "rtos.h"
#include "uvisor-lib/uvisor-lib.h"

/* Private static memory for the secure box */
typedef struct {
    uint32_t * buffer;
    int index;
} PrivateTimerStaticMemory;

/* ACLs list for the secure box: Timer (PIT). */
static const UvisorBoxAclItem g_private_timer_acls[] = {
    {PIT,   sizeof(*PIT),   UVISOR_TACLDEF_PERIPH}
    {PORTC, sizeof(*PORTC), UVISOR_TACLDEF_PERIPH},
};

static void private_timer_main_thread(const void *);

/* Secure box configuration */
UVISOR_BOX_NAMESPACE(NULL);                   /* We won't specify a box namespace for this example. */
UVISOR_BOX_HEAPSIZE(4096);                    /* Heap size for the secure box */
UVISOR_BOX_MAIN(private_timer_main_thread,    /* Main thread for the secure box */
                osPriorityNormal,             /* Priority of the secure box's main thread */
                1024);                        /* Stack size for the secure box's main thread */
UVISOR_BOX_CONFIG(private_timer,              /* Name of the secure box */
                  g_private_timer_acls,       /* ACLs list for the secure box */
                  1024,                       /* Stack size for the secure box */
                  PrivateTimerStaticMemory);  /* Private static memory for the secure box. */
```

### Create the secure box's main thread function

In general, you can decide what to do in your box's main thread. You can run it once and then kill it, use it to configure memories, peripherals, or to create other threads. In this app, the box's main thread is the only thread for the `private_timer` box, and it will run throughout the whole program.

The `private_timer_main_thread` function configures the PIT timer, allocates the dynamic buffer to hold the timer values and initializes its private static memory, `PrivateTimerStaticMemory`. A spinning loop is used to update the values in the buffer every time the thread is reactivated.

```C
/* Number of timer samples we will use */
#define PRIVATE_TIMER_BUFFER_COUNT 256

/* For debug purposes: print the buffer values when the SW2 button is pressed. */
static void private_timer_button_on_press(void)
{
    for (int i = 0; i < PRIVATE_TIMER_BUFFER_COUNT; i++) {
        printf("buffer[%03d] = %lu\r\n", i, uvisor_ctx->buffer[i]);
    }
}

/* Main thread for the secure box */
static void private_timer_main_thread(const void *)
{
    /* Create the buffer and cache its pointer to the private static memory. */
    uvisor_ctx->buffer = (uint32_t *) malloc(PRIVATE_TIMER_BUFFER_COUNT * sizeof(uint32_t));
    if (uvisor_ctx->buffer == NULL) {
        mbed_die();
    }
    uvisor_ctx->index = 0;

    /* Setup the push-button callback. */
    InterruptIn button(SW2);
    button.mode(PullUp);
    button.fall(&private_timer_button_on_press);

    /* Setup and start the timer. */
    Timer timer;
    timer.start();

    while (1) {
        /* Store the timer value. */
        uvisor_ctx->buffer[uvisor_ctx->index] = timer.read_us();

        /* Update the index. Behave as a circular buffer. */
        if (uvisor_ctx->index < PRIVATE_TIMER_BUFFER_COUNT - 1) {
            uvisor_ctx->index++;
        } else {
            uvisor_ctx->index = 0;
        }
    }
}
```

A few things to note in the code above:

* If code is running in the context of `private_timer`, then any object instantiated inside that code will belong to the `private_timer` heap and stack. This means that in the example above the `InterruptIn` and `Timer` objects are private to the `private_timer` box. The same applies to the dynamically allocated buffer `uvisor_ctx->buffer`.
* The content of the private memory `PrivateTimerStaticMemory` can be accessed using the `PrivateTimerStaticMemory * uvisor_ctx` pointer, which is maintained by uVisor.
* The `InterruptIn` object triggers the registration of an interrupt slot. Since that code is run in the context of the `private_timer` box, then the push-button IRQ belongs to that box. If you want to use the IRQ APIs directly, read the [section](#the-nvic-apis) below.
* Even if the `private_timer_button_on_press` function runs in the context of `private_timer`, we can still use the `printf` function, which accesses the `UART0` peripheral, owned by the main box. This is because all ACLs declared in the main box are by default shared with all the other secure boxes. This also means that the messages we are printing on the serial port are not secure, because other boxes have access to that peripheral.

> **Warning**: Instantiating an object in the `secure_box.cpp` global scope will automatically map it to the main box context, not the `private_timer` one. If you want an object to be private to a box, you need to instantiate it inside the code that will run in the context of that box (like the `InterruptIn` and `Timer` objects), or alternatively statically initialize it in the box private static memory (like the `buffer` and `index` variables in `PrivateTimerStaticMemory`).

---

**Checkpoint**

Compile the application again:

```bash
$ mbed compile -m K64F -t GCC_ARM
```

Re-flash the device, and press the reset button. The device LED should be blinking as in the previous case.

If you don't see the LED blinking, it means that the application halted somewhere, probably because uVisor captured a fault. You can setup the uVisor debug messages to see if there is any problem. Follow the [Debugging uVisor on mbed OS](DEBUGGING.md) document for a step-by-step guide.

If the LED is blinking, it means that the app is running fine. If you now press the `SW2` button on the NXP FRDM-K64F board, the `private_timer_button_on_press` function will be executed, printing the values in the timer buffer. You can observe these values by opening a serial port connection to the device, with a baud rate of 9600. When the print is completed, you should see the LED blinking again.

### Expose public secure entry points to the secure box

Coming soon.

## Wrap-up
[Go to top](#overview)

In this guide we showed you how to:

* Enable uVisor on an existing application.
* Add a secure box to your application.
    * Protect static and dynamic memories in a secure box.
    * Gain exclusive access to a peripheral and an IRQ in a secure box.
    * (Coming soon) Expose public secure entry points to a secure box.

You can now modify the example or create a new one to protect your resources into a secure box. You might find the following resources useful:

* [uVisor API documentation](API.md)
* [Debugging uVisor on mbed OS](DEBUGGING.md)

If you found any bug or inconsistency in this guide, please [raise an issue](https://github.com/ARMmbed/uvisor/issues/new).

## Appendix
[Go to top](#overview)

This section contains additional information that you might find useful when setting up a secure box.

### The NVIC APIs

The ARM CMSIS header files provide APIs to configure, enable and disable IRQs in the NVIC module. These APIs are all prefixed with `NVIC_` and can be found in the `core_cm*.h` files in your CMSIS module.

In addition, the CMSIS header also provide APIs to set and get an interrupt vector at runtime. This requires the interrupt vector table, which is usually located in flash, to be relocated to SRAM.

When the uVisor is enabled, all NVIC APIs are re-routed to the corresponding uVisor vIRQ APIs, which virtualize the interrupt module. The uVisor interrupt model has the following features:

* The uVisor owns the interrupt vector table.
* All ISRs are relocated to SRAM.
* Code in a box can only change the state of an IRQ (enable it, change its priority, etc.) if the box registered that IRQ with uVisor at runtime, using the `NVIC_SetVector` API.
* An IRQ that belongs to a box can only be modified when that box context is active.

Although this behaviour is different from the original NVIC one, it is backwards compatible. This means that legacy code (like a device HAL) will still work after uVisor is enabled. The general use case is the following:

```C
#define MY_IRQ 42

/* Set the ISR for MY_IRQ at runtime.
 * Without uVisor: Relocate the interrupt vector table to SRAM and set my_isr as
                   the ISR for MY_IRQ.
 * With    uVisor: Register MY_IRQ for the current box with my_isr as ISR. */
NVIC_SetVector(MY_IRQ, &my_isr);

/* Change the IRQ state. */
NVIC_SetPriority(MY_IRQ, 3);
NVIC_EnableIRQ(MY_IRQ);
```

> **Note**: In this model a call to `NVIC_SetVector` must always happen before an IRQ state is changed. In platforms that don't relocate the interrupt vector table such a call might be originally absent and must be added to work with uVisor.

For more information on the uVisor APIs, checkout the [uVisor API documentation](API.md) document.

### The *main box* ACLs

The code samples that we provide in this guide give you a ready-made list of ACLs for the main box. The list includes peripherals that we already know will be necessary to make the example app work, and it is specific to the NXP FRDM-K64F target.

This section shows how to discover the needed ACLs for the main box. You might need to follow these instructions in case you want to generate the ACLs list for a different target or a different app.

At the moment the uVisor does not provide a way to detect and list all the faulting ACLs for a given platform automatically. This is a planned feature that will be released in the future.

In order to generate the list of ACLs, use the code provided in the [Enable uVisor](#enable-uvisor) section. In this case, though, start with an empty ACLs list:

```C
static const UvisorBoxAclItem g_main_box_acls[] = {
}
```

You now need to compile your application using uVisor in debug mode. This operation requires some more advanced steps, which are described in detail in the [Debugging uVisor on mbed OS](DEBUGGING.md) document. The main idea is that you compile the application in debug mode:

```bash
$ mbed compile -m K64F -t GCC_ARM -o "debug-info"
```

and then use a GDB-compatible interface to flash the device, enable semihosting, and access the uVisor debug messages. Please read the [Debugging uVisor on mbed OS](DEBUGGING.md) document for the detailed instructions.

Once the uVisor debug messages are enabled, you will see you application fail. The failure is due to the first missing ACL being hit by the main box code. The message will look like:

```
***********************************************************
                    BUS FAULT
***********************************************************

...

* MEMORY MAP
  Address:           0x4004800C
  Region/Peripheral: SIM
    Base address:    0x40047000
    End address:     0x40048060

...
```

Now that you know which peripheral is causing the fault (the `SIM` peripheral, in this example), you can add its entry to the ACLs list:


```C
static const UvisorBoxAclItem g_main_box_acls[] = {
    {SIM, sizeof(*SIM), UVISOR_TACLDEF_PERIPH},
};
```

> **Note**: If the fault debug screen does not show the name of the peripheral, you need to look it up in the target device reference manual.

For readability, do not use the hard-coded addresses of your peripherals, but rather use the symbols provided by the target CMSIS module.

Repeat the process multiple times until all ACLs have been added to the list. When no other ACL is needed any more, the system will run without hitting a uVisor fault.
