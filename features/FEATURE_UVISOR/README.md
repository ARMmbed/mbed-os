# Getting started guide for uVisor on mbed OS

This guide will help you start uVisor on mbed OS by showing you how to create a sample application for the NXP FRDM-K64F board.

The uVisor provides sandboxed environments and resources protection for applications built for ARM Cortex-M3 and Cortex-M4 devices. This guide will show you how to enable the uVisor and configure a secure box to access some exclusive resources (memory, peripherals, interrupts). For more information about the uVisor design philosophy, please see the uVisor [introductory document](../../README.md).

## Requirements

To run the `blinky` application on mbed OS with uVisor enabled, you need:

- A platform and a toolchain that uVisor on mbed OS supports. You can verify this on [the official list](../README.md#supported-platforms). If uVisor supports your platform internally but not on mbed OS, the porting process is incomplete. To port your platform to uVisor and enable it on mbed OS, please follow the [uVisor porting guide for mbed OS](../core/PORTING.md).
- Git.
- mbed CLI. Run `pip install mbed-cli` to install it.

The remainder of this guide assumes:

- You are developing on a \*nix machine in the `~/code` folder.
- You are building the app for the [NXP FRDM-K64F](http://developer.mbed.org/platforms/FRDM-K64F/) target with the [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded).

You can use these instructions as guidelines in the case of other targets on other host OSs.

## Start with the `blinky` app
[Go to top](#overview)

Create a new mbed application called `uvisor-example` by running the following commands:

```bash
$ cd ~/code
$ mbed new uvisor-example
$ cd uvisor-example
```

The mbed CLI tools automatically fetch the mbed codebase. By default, Git tracks your code changes, so you can push your application to a Git server if you want to.

Once the import process finishes, create a `source` folder:
```bash
$ mkdir ~/code/uvisor-example/source
```
Place a new file `main.cpp` in it:

```C
/* ~/code/uvisor-example/source/main.cpp */

#include "mbed.h"

DigitalOut led(LED1);

int main(void)
{
    while (true) {
        led = !led;
        wait(0.5);
    }
}
```

This application blinks an LED from the main thread, which the OS creates by default.

---

**Checkpoint**

Compile the application:

```bash
$ mbed compile -m K64F -t GCC_ARM
```

The resulting binary is located at:

```bash
~/code/uvisor-example/BUILD/K64F/GCC_ARM/uvisor-example.bin
```

Drag and drop it onto the USB device mounted on your computer to flash the device. When the flashing process is complete, press the reset button on the device. The device's LED blinks.

## Enable uVisor
[Go to top](#overview)

To enable the uVisor on the app, add these lines to the beginning of the `main.cpp` file:

```C
/* ~/code/uvisor-example/source/main.cpp */

#include "mbed.h"
#include "uvisor-lib/uvisor-lib.h"

/* Public box Access Control Lists (ACLs). */
/* Note: These are specific to the NXP FRDM-K64F board. See the section below
 *       for more information. */
static const UvisorBoxAclItem g_public_box_acls[] = {
    /* For the LED */
    {SIM,   sizeof(*SIM),   UVISOR_TACLDEF_PERIPH},
    {PORTB, sizeof(*PORTB), UVISOR_TACLDEF_PERIPH},

    /* For messages printed on the serial port */
    {OSC,   sizeof(*OSC),   UVISOR_TACLDEF_PERIPH},
    {MCG,   sizeof(*MCG),   UVISOR_TACLDEF_PERIPH},
    {UART0, sizeof(*UART0), UVISOR_TACLDEF_PERIPH},
    {PIT,   sizeof(*PIT),   UVISOR_TACLDEF_PERIPH},
};

/* Enable uVisor, using the ACLs we just created. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_public_box_acls);

/* Rest of the existing code */
...
```

In the code above, we specified two elements:

1. Public box Access Control Lists (ACLs). With uVisor enabled, everything runs in unprivileged mode, so make sure the public box and peripherals the OS accesses are allowed. These peripherals are specified using a list like the one in the snippet above. This example provides the list of all the ACLs you need. For other platforms or other applications, you need to determine those ACLs following the process in [The main box ACLs](#the-main-box-acls).
1. App-specific uVisor configurations: `UVISOR_SET_MODE_ACL`. This macro sets the uVisor mode (enabled) and associates the list of ACLs you just created with the public box.

Before compiling, you need to override the original `K64F` target to enable the uVisor feature. To do so, add the file `~/code/uvisor-example/mbed_app.json` with the following content:

```JSON
{
    "target_overrides": {
        "*": {
            "target.features_add": ["UVISOR"],
            "target.extra_labels_add": ["UVISOR_SUPPORTED"]
        }
    },
    "macros": [
        "FEATURE_UVISOR=1",
        "TARGET_UVISOR_SUPPORTED=1"
    ]
}
```

The macros `FEATURE_UVISOR` and `TARGET_UVISOR_SUPPORTED` in the configuration file above are automatically defined for C and C++ files but not for assembly files. Because the uVisor relies on those symbols in some assembly code, you need to define them manually.

---

**Checkpoint**

Compile the application again. This time, the `K64F` target includes the new features and labels you provided in `mbed_app.json`;

```bash
$ mbed compile -m K64F -t GCC_ARM
```

The binary is located at:

```bash
~/code/uvisor-example/BUILD/K64F/GCC_ARM/uvisor-example.bin
```

Reflash the device, and press the reset button. The device LED blinks as in the previous case.

---

If you enable uVisor in the `blinky` app as it was written above, you do not get any particular security feature. All code and resources share the same security context, which we call the *public box*.

A lot happens unseen, though. All the user code now runs in unprivileged mode, and the systems services, such as the `NVIC` APIs and the OS SVCalls, are routed through the uVisor.

## Add a secure box
[Go to top](#overview)

Now that uVisor is enabled, you can finally add a *secure box*.

A secure box is a special compartment with exclusive access to peripherals, memories and interrupts. Private resources are only accessible when the *context* of the secure box is active. The uVisor is the only one that can enable a secure box context, for example upon thread switching or interrupt handling.

uVisor does not obfuscate code that belongs to a box, so it is still readable and executable from outside of the box. In addition, declaring an object in the same file that configures a secure box does not protect that object automatically.

Instead, we provide specific APIs to instruct the uVisor to protect a private resource. The `uvisor-example` app will show how to use these APIs.

### Configure the secure box

For this example, we want to create a secure box called `private_button`. The `private_button` box has exclusive access to the push-button on the NXP FRDM-K64F board, which means that other boxes cannot access its corresponding peripheral.

Each secure box must have at least one thread, which we call the box's main thread. In our `private_button` box, we only use this thread throughout the whole program. The thread runs every second and counts the number of times it has been called between two button presses. The thread count is saved in a variable private to the box. Whenever we press the `SW2` button on the board, the current thread count is stored into a private buffer and restarts. For debug purposes, the program prints the content of the buffer every time it fills up.

You want the box to have exclusive access to the following resources:

- The push-button peripheral (as specified by a peripheral ACL). Nobody else should be able to access the push-button port.
- The push-button interrupt (as specified by an IRQ ACL). You want the button IRQ to reroute to our box-specific ISR.
- The private dynamically allocated buffer (as specified by a dynamic memory ACL).
- The private variables (as specified by a static memory ACL).

Create a new source file, `~/code/uvisor-example/source/secure_box.cpp`. You will configure the secure box inside this file. The secure box name for this example is `private_button`.

```C
/* ~/code/uvisor-example/source/secure_box.cpp */

#include "mbed.h"
#include "uvisor-lib/uvisor-lib.h"

/* Private static memory for the secure box */
typedef struct {
    uint32_t * buffer;          /* Static private memory, pointing to dynamically allocated private memory */
    uint32_t counter;           /* Static private memory */
    int index;                  /* Static private memory */
    RawSerial * pc;             /* Static private memory, pointing to dynamically allocated private memory */
} PrivateButtonStaticMemory;

/* ACLs list for the secure box: Timer (PIT). */
static const UvisorBoxAclItem g_private_button_acls[] = {
    {PORTC,               sizeof(*PORTC), UVISOR_TACLDEF_PERIPH},     /* Private peripheral */
    {(void *) PORTC_IRQn,              0, UVISOR_TACL_IRQ},           /* Private IRQ */
};

static void private_button_main_thread(const void *);

/* Secure box configuration */
UVISOR_BOX_NAMESPACE(NULL);                   /* We won't specify a box namespace for this example. */
UVISOR_BOX_HEAPSIZE(4096);                    /* Heap size for the secure box */
UVISOR_BOX_MAIN(private_button_main_thread,   /* Main thread for the secure box */
                osPriorityNormal,             /* Priority of the secure box's main thread */
                1024);                        /* Stack size for the secure box's main thread */
UVISOR_BOX_CONFIG(private_button,             /* Name of the secure box */
                  g_private_button_acls,      /* ACLs list for the secure box */
                  1024,                       /* Stack size for the secure box */
                  PrivateButtonStaticMemory); /* Private static memory for the secure box. */
```

### Create the secure box's main thread function

In general, you can decide what to do in your box's main thread. You can run it once and then stop it or use it to configure memories or peripherals or to create other threads. In this app, the box's main thread is the only thread for the `private_button` box, and it runs throughout the program.

The `private_button_main_thread` function configures the push-button to trigger an interrupt when pressed, allocates the dynamic buffer to hold the thread count values and initializes its private static memory, `PrivateButtonStaticMemory`. A spinning loop updates the counter value every second.

```C
/* ~/code/uvisor-example/source/secure_box.cpp */

/* The previous code goes here. */
...

#define uvisor_ctx ((PrivateButtonStaticMemory *) __uvisor_ctx)

#define PRIVATE_BUTTON_BUFFER_COUNT 8

static void private_button_on_press(void)
{
    /* Store the thread count into the buffer and reset it. */
    uvisor_ctx->buffer[uvisor_ctx->index] = uvisor_ctx->counter;
    uvisor_ctx->counter = 0;

    /* Update the index. Behave as a circular buffer. */
    if (uvisor_ctx->index < PRIVATE_BUTTON_BUFFER_COUNT - 1) {
        uvisor_ctx->index++;
    } else {
        uvisor_ctx->index = 0;

        /* For debug purposes: Print the content of the buffer. */
        uvisor_ctx->pc->printf("Thread count between button presses: ");
        for (int i = 0; i < PRIVATE_BUTTON_BUFFER_COUNT; ++i) {
            uvisor_ctx->pc->printf("%lu ", uvisor_ctx->buffer[i]);
        }
        uvisor_ctx->pc->printf("\n");
    }

}

/* Main thread for the secure box */
static void private_button_main_thread(const void *)
{
    /* Allocate serial port to ensure that code in this secure box
     * won't touch handle in the default security context when printing */
    if (!(uvisor_ctx->pc = new RawSerial(USBTX, USBRX))) {
        return;
    }

    /* Create the buffer and cache its pointer to the private static memory. */
    uvisor_ctx->buffer = (uint32_t *) malloc(PRIVATE_BUTTON_BUFFER_COUNT * sizeof(uint32_t));
    if (uvisor_ctx->buffer == NULL) {
        uvisor_ctx->pc->printf("ERROR: Failed to allocate memory for the button buffer\n");
        mbed_die();
    }
    uvisor_ctx->index = 0;
    uvisor_ctx->counter = 0;

    /* Setup the push-button callback. */
    InterruptIn button(SW2);                        /* Private IRQ */
    button.mode(PullUp);
    button.fall(&private_button_on_press);

    /* Increment the private counter every second. */
    while (1) {
        uvisor_ctx->counter++;
        wait(1.0);
    }
}
```

A few things to note in the code above:

- If code runs in the context of `private_button`, then any object instantiated inside that code belongs to the `private_button` heap and stack. This means that in the example above, the `InterruptIn` object is private to the `private_button` box. The same applies to the dynamically allocated buffer `uvisor_ctx->buffer`.
- You can access the content of the private memory `PrivateButtonStaticMemory` using the `void * const __uvisor_ctx` pointer, which uVisor maintains. You need to cast this pointer to your own context type. In this example we used a pre-processor symbol to improve readability.
- The `InterruptIn` object triggers the registration of an interrupt slot using the NVIC APIs. If you want to use the IRQ APIs directly, read the [NVIC APIs section](#the-nvic-apis) below. We registered the push-button IRQ to the `private_button` box through an IRQ ACL, and hence only code from this box can access it. Changing the push-button IRQ state from the public box causes a uVisor fault.
- Even if the `private_button_on_press` function runs in the context of `private_button`, you can still use the `printf` function, which accesses the `UART0` peripheral, owned by the public box. This is because all ACLs declared in the public box are by default shared with all the other secure boxes. This also means that the messages we are printing on the serial port are not secure because other boxes have access to that peripheral.

> **Warning**: Instantiating an object in the `secure_box.cpp` global scope automatically maps it to the public box context, not the `private_button` one. If you want an object to be private to a box, you need to instantiate it inside the code that runs in the context of that box (such as the `InterruptIn` object), or alternatively statically initialize it in the box private static memory (such as the `buffer`, `index` and `counter` variables in `PrivateButtonStaticMemory`).

---

**Checkpoint**

Compile the application again:

```bash
$ mbed compile -m K64F -t GCC_ARM
```

Reflash the device, and press the reset button. The device LED blinks.

If the LED doens't blink, it means the application halted somewhere, probably because uVisor captured a fault. You can set up the uVisor debug messages to see if there is a problem. See [Debugging uVisor on mbed OS](DEBUGGING.md) for a step-by-step guide.

If the LED is blinking, the app is running correctly. If you press the `SW2` button on the NXP FRDM-K64F board, the `private_button_on_press` function executes, printing the values in the timer buffer after `PRIVATE_BUTTON_BUFFER_COUNT` presses. You can observe these values by opening a serial port connection to the device, with a baud rate of 9600.

## Expose public secure entry points to the secure box
[Go to top](#overview)

So far, the code in the secure box cannot communicate to other boxes. To let other boxes call functions in our secure box, you can define public secure entry points. These entry points can map to private functions within the context of a secure box, and an RPC protocol automatically serializes the arguments and return values to ensure no private memory can leak to external boxes.

You can define a public secure entry point to retrieve the index value from the secure box. This index value increases every time you press the `SW2` button.

### Defining a secure entry point

Create a new source file, `~/code/uvisor-example/source/secure_box.h`, where you will define the functions that you can call through RPC.

```cpp
/* ~/code/uvisor-example/source/secure_box.h */

#ifndef SECURE_BOX_H_
#define SECURE_BOX_H_

#include "uvisor-lib/uvisor-lib.h"

UVISOR_EXTERN int (*secure_get_index)(void);

#endif
```

### Implementing a secure entry point

Now that you have defined the secure entry point, you can map the entry point to a function running in the secure box. You can do this through the `UVISOR_BOX_RPC_GATEWAY_SYNC` macro. Open `~/code/uvisor-example/source/secure_box.cpp`, and replace the line with `#define PRIVATE_BUTTON_BUFFER_COUNT 8` by:

```cpp
/* ~/code/uvisor-example/source/secure_box.cpp */

/* Function called through RPC */
static int get_index() {
    /* Access to private memory here */
    return uvisor_ctx->index;
}

UVISOR_BOX_RPC_GATEWAY_SYNC (private_button, secure_get_index, get_index, int, void);

  #define PRIVATE_BUTTON_BUFFER_COUNT 8
```

### Listening for RPC messages

To receive RPC messages, you need to spin up a new thread, running in the secure box context. You can do this in the main thread of the secure box. In `~/code/uvisor-example/source/secure_box.cpp`, replace the first five lines of `private_button_main_thread` with:

```cpp
/* ~/code/uvisor-example/source/secure_box.cpp */

static void listen_for_rpc() {
    /* List of functions to wait for */
    static const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) get_index
    };

    while (1) {
        int caller_id;
        int status = rpc_fncall_waitfor(my_fn_array, 1, &caller_id, UVISOR_WAIT_FOREVER);

        if (status) {
            uvisor_error(USER_NOT_ALLOWED);
        }
    }
}

/* Main thread for the secure box */
static void private_button_main_thread(const void *)
{
    /* allocate serial port to ensure that code in this secure box
     * won't touch handle in the default security context when printing */
    if (!(uvisor_ctx->pc = new RawSerial(USBTX, USBRX)))
        return;

    /* Start listening for RPC messages in a separate thread */
    Thread rpc_thread(osPriorityNormal, 1024);
    rpc_thread.start(&listen_for_rpc);

    /* ... Rest of the private_button_main_thread function ... */
```

### Calling the public secure entry point

To call the public secure entry point from any other box, you can use the `secure_get_index` function. It will automatically do an RPC call into the secure box and serialize the return value. You can try this out from the public box. In `~/code/uvisor-example/source/main.cpp`, first include the header file for the secure box:

```cpp
/* ~/code/uvisor-example/source/main.cpp */

#include "secure_box.h"
```

Then replace the `main` function with:

```cpp
/* ~/code/uvisor-example/source/main.cpp */

int main(void)
{
    while (true) {
        led = !led;
        printf("Secure index is %d\n", secure_get_index());
        Thread::wait(500);
    }
}
```

You can observe the secure index by opening a serial port connection to the device with a baud rate of 9600. When you press the `SW2` button, the index will increase.

## The NVIC APIs

The ARM CMSIS header files provide APIs to configure, enable and disable IRQs in the NVIC module. These APIs all begin with `NVIC_`, and you can find them in the `core_cm*.h` files in your CMSIS module. The CMSIS header files also provide APIs to set and get an interrupt vector at runtime. This requires the relocation of the interrupt vector table, which is usually located in flash, to SRAM.

When the uVisor is enabled, all NVIC APIs are rerouted to the corresponding uVisor vIRQ APIs, which virtualize the interrupt module. The uVisor interrupt model has the following features:

- The uVisor owns the interrupt vector table.
- All ISRs are relocated to SRAM.
- Code in a box can only change the state of an IRQ (enable it, change its priority and so on) if the box registered that IRQ with uVisor through an IRQ ACL.
- An IRQ that belongs to a box can only be modified when that box context is active.

Although this behavior is different from that of the original NVIC, it is backward compatible. Legacy code (such as a device HAL) still works after uVisor is enabled.

All IRQ slots that are not listed in any box ACL list are considered unclaimed. Boxes can gain exclusive ownership of unclaimed IRQs on a first-come first-served basis through the use of the NVIC APIs.

## The *public box* ACLs

The code samples in this guide provide a list of ACLs for the public box. The list includes peripherals necessary to make the example app work, and they are specific to the NXP FRDM-K64F target.

To generate the ACLs list for a different target or a different app, use the code provided in the [Enable uVisor](#enable-uvisor) section, but start with an empty ACLs list:

```C
static const UvisorBoxAclItem g_public_box_acls[] = {
}
```

Compile your application using uVisor in debug mode. This operation requires some more advanced steps. Please read [Debugging uVisor on mbed OS](DEBUGGING.md) for the detailed instructions.

Once the uVisor debug messages are enabled, your application fails. The failure is due to the first missing ACL being hit by the public box code. The message will look like:

```
***********************************************************
                    BUS FAULT
***********************************************************

* Active Box ID: 0
* FAULT SYNDROME REGISTERS

  CFSR: 0x00008200
  BFAR: 0x40048044
  --> PRECISERR: precise data access.

...
```

Look up the faulty address (the value of BFAR) in the target device reference manual.

Once you know which peripheral is causing the fault (the `SIM` peripheral, in this example), add its entry to the ACLs list:

```C
static const UvisorBoxAclItem g_public_box_acls[] = {
    {SIM, sizeof(*SIM), UVISOR_TACLDEF_PERIPH},
};
```

> **Note**: If the fault debug screen does not show the name of the peripheral, look it up in the target device reference manual.

For readability, do not use the hard-coded addresses of your peripherals. Instead, use the symbols that the target CMSIS module provides.

Repeat the process multiple times until all ACLs have been added to the list. When no other ACL is needed, the system runs without hitting a uVisor fault.

## Additional resources
[Go to top](#overview)

- [uVisor API documentation](API.md).
- [Debugging uVisor on mbed OS](DEBUGGING.md).
- [Using nonvolatile storage from uVisor on mbed OS](manual/Flash.md).

If you found any bug or inconsistency in this guide, please [raise an issue](https://github.com/ARMmbed/uvisor/issues/new).
