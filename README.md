# ARM mbed OS with support for ARMv8-M

This is an ARM mbed OS feature branch containing the technology preview of an
ARMv8-M (Cortex-M33) target, with support for uVisor, CMSIS5 and Keil RTX5.

## Repository content

The version of mbed OS contained in this repository provides:

* A new target, called `ARM_MPS2_ARMv8MML`, aimed at ARM Cortex-M33 FastModels.
* The uVisor binaries supporting the `ARM_MPS2_ARMv8MML` target. These binaries
  are compiled from our latest uVisor release, [v0.28.1](https://github.com/ARMmbed/uvisor/releases/tag/v0.28.1).

## Prerequisites

This is a technology preview of the `ARM_MPS2_ARMv8MML` target. There is no
physical MCU yet integrating the ARMv8-M Cortex-M33 core, so you need an ARM
FastModel binary to execute applications built for this target. We currently
support the ARM `FVP_MPS2_Cortex-M33` FastModel version 10.3.

We only support the Launchpad GCC ARM Embedded toolchain, at version
6-2017-q1-update (6.3.1 20170215, release) or above.

This branch is based on ARM mbed OS 5.5.

## The ARMv8-M target

For this technology preview, we only provide a very simplified version of the
ARMv8-M mbed OS target, supporting only a timer, serial output and all NVIC IRQs
(triggered via software).

## uVisor support

uVisor v0.28.1 adds support for the ARMv8-M architecture, and in particular for
the Cortex-M33 core with the ARM TrustZone for Cortex-M security extensions
(SAU, MPU, aliasing).

The execution model of uVisor and mbed OS on this architecture is slightly
different from the one implemented on ARMv7-M devices (Cortex-M3 and Cortex-M4).

On ARMv7-M, the uVisor runs in privileged mode alongside the RTOS,
while the application runs in unprivileged mode. On ARMv8-M, the uVisor controls
the Secure domain, while the Non-Secure domain is completely managed by the OS
and the user application.

The uVisor still provide the possibility to create ACL-controlled sandboxes,
that is, mutually distrustful execution environments with access to only
selected peripherals, interrupts and memories. On ARMv7-M, these boxes run in
unprivileged mode. On ARMv8-M they can run privileged or unprivileged in the
Non-Secure domain. By default all code runs in privileged mode in the Non-Secure
side.

A sandbox executes when scheduled by uVisor from the Secure side. The uVisor
scheduler at the moment implements a simple round-robin scheme, where each box
runs for its own time-slice. More sophisticated scheduling approaches can be
implemented in the future.

As in the previous model, the public box, which is the default box, is visible
and accessible by all other boxes.

Communication between boxes can only happen via message passing. uVisor now
supports new Inter-Process Communication APIs (IPC) that allow asymmetric
asynchronous and synchronous message passing between boxes.

Sandboxes can expose entry points via Remote Procedure Calls (RPC) exactly as
they did on the ARMv7-M architecture, but please note that this feature is still
under development and will soon be released as part of a uVisor patch release
v0.28.x.

Every sandbox can get hold of private memories, peripherals and interrupts.
These capabilities are specified via configuration macros or uVisor ACL types.
See the [Getting started guide for uVisor on mbed OS](https://github.com/ARMmbed/mbed-os/tree/master/features/FEATURE_UVISOR#getting-started-guide-for-uvisor-on-mbed-os)
for more details.

Interrupts are delivered directly to the Non-Secure domain, but only when they
occur while the box that owns them is active. While a box is inactive, its IRQs
are put on hold, waiting to be delivered as soon as the box is switched in
again.

To summarize:

* The uVisor owns the Secure domain.
* mbed OS, and hence Keil RTX5, run in the Non-Secure domain.
* The user application runs in the Non-Secure domain.
* The thread privileged, thread unprivileged and handler modes in the Non-Secure
  domain are independent of uVisor and hence can run out-of-the-box, without
  uVisor intervention.
* All interrupts that are owned by the OS, the user application or any uVisor
  sandbox are delivered directly to the Non-Secure domain.
* Interrupts are only delivered to the box that owns them while that box is
  active. Undelivered IRQs are kept as pending while a box is inactive.
* uVisor only takes care to schedule boxes.
* Boxes communicate via message passing (IPC).
* Boxes expose entry points via functional message passing (RPC).

## How to get started

You can build an app targeting the new `ARM_MPS2_ARMv8MML` target and using
most of the uVisor features as shown in the [Getting started guide for uVisor on mbed OS](https://github.com/ARMmbed/mbed-os/tree/master/features/FEATURE_UVISOR#getting-started-guide-for-uvisor-on-mbed-os),
but please note that the ARMv8-M target currently only supports timers, serial
output and software-triggered IRQs, so some of the features shown in that guide
cannot be directly reproduced on this target.

For convenience, we report here code snippets to build a sample app with the
`ARM_MPS2_ARMv8MML` target.

We will have 2 boxes:

* The public box, which is the default, unprotected box.
* A sandbox, with exclusive ownership of an interrupt slot and a private memory.

As usual, first create a file named `mbed_app.json` containing basic
configurations to enable uVisor:

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

In a file called `public_box.cpp` you can add the code for a simple public box:

```C
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"

static const UvisorBoxAclItem g_public_box_acls[] = {
    {CMSDK_UART0, sizeof(*CMSDK_UART0), UVISOR_TACLDEF_PERIPH},
};

UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_public_box_acls);
UVISOR_SET_PAGE_HEAP(2 * 1024, 5);

Serial shared_pc(USBTX, USBRX);

int main(void)
{
    int count = 0;
    while (1) {
        shared_pc.printf("Public box: count = %d\r\n", count);
        ++count;
        Thread::wait(1000);
    }

    return 0;
}
```

As you can see in the code above, the public box only owns the UART0 peripheral,
which is used to print output to the serial port.

The code for the sandbox can be put in a file called `sandbox.cpp`:

```C
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"

extern Serial shared_pc;

/* Private static memory for the sandbox */
typedef struct {
    uint32_t counter;
} SandboxPrivateMemory;

/* Note: Any slot is OK, as we trigger the IRQ via software anyway. */
#define IRQ_SLOT 42

/* ACLs list for the sandbox */
static const UvisorBoxAclItem g_sandbox_acls[] = {
    {(void *) IRQ_SLOT, 0, UVISOR_TACL_IRQ},
};

static void sandbox_main_thread(const void *);

/* Sandbox configuration */
UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(4096);
UVISOR_BOX_MAIN(sandbox_main_thread, osPriorityNormal, 1024);
UVISOR_BOX_CONFIG(sandbox, g_sandbox_acls, 1024, SandboxPrivateMemory);

#define uvisor_ctx ((SandboxPrivateMemory *) __uvisor_ctx)

static void sandbox_irq_handler(void)
{
    ++(uvisor_ctx->counter);
    NVIC_ClearPendingIRQ((IRQn_Type) IRQ_SLOT);
}

static void sandbox_main_thread(const void *)
{
    /* Configure IRQ.*/
    NVIC_SetVector((IRQn_Type) IRQ_SLOT, (uint32_t) sandbox_irq_handler);
    NVIC_EnableIRQ((IRQn_Type) IRQ_SLOT);

    uvisor_ctx->counter = 0;
    while (1) {
        NVIC_SetPendingIRQ((IRQn_Type) IRQ_SLOT);
        shared_pc.printf("Sandbox: count = %lu\r\n", uvisor_ctx->counter);
        for (volatile size_t i = 0; i < 0x1000000; ++i);
    }
}
```

The sandbox owns the interrupt 42 and a private `uint32_t` counter. The IRQ is
triggered manually via the CMSIS APIs (NVIC) to show how it works.

You can build the app using the following command:

```bash
$ mbed compile -t GCC_ARM -m ARM_MPS2_ARMv8MML
```

And then execute the result ELF file on your FastModel binary:

```bash
$ FVP_MPS2_Cortex-M33 --parameter fvp_mps2.platform_type=0 \
                      --parameter cpu0.SAU=0x8 \
                      --parameter cpu0.semihosting-enable=1 \
                      --parameter fvp_mps2.DISABLE_GATING=1 \
                      --parameter fvp_mps2.UART0.out_file=/dev/stdout \
                      --parameter fvp_mps2.UART0.in_file=/dev/stdin \
                      --parameter fvp_mps2.telnetterminal0.start_telnet=0 \
                      --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
                      --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
                      --parameter fvp_mps2.telnetterminal0.quiet=1 \
                      --parameter fvp_mps2.telnetterminal1.quiet=1 \
                      --parameter fvp_mps2.telnetterminal2.quiet=1 \
                      --application cpu0=app.elf
```

You will see the serial output directly on `/dev/stdout`:

```
Debugging core 0
Public box: count = 0
Sandbox: count = 1
Public box: count = 1
Public box: count = 2
Public box: count = 3
Public box: count = 4
Sandbox: count = 2
Public box: count = 5
Public box: count = 6
Public box: count = 7
Public box: count = 8
Sandbox: count = 3
Public box: count = 9
...
```

You can also compile the application in debug mode:

```bash
$ mbed compile -t GCC_ARM -m ARM_MPS2_ARMv8MML --profile mbed-os/tools/profiles/debug.json
```

The uVisor semihosting output will also be shown on `/dev/stdout`.

## Limitations

At the moment the uVisor support for ARMv8-M comes with the following
limitations:

* RPC is not supported yet (coming soon).
* Sandboxes can only run bare-metal, so they should not rely on OS facilities.
  * This will be fixed with the next release.
* The `ARM_MPS2_ARMv8MML` target HAL only supports timers, serial output and
  software-triggered IRQs.
