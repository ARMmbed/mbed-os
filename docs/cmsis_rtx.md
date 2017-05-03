# CMSIS & RTX

CMSIS/RTX code is imported from the original CMSIS repository which can be found: https://github.com/ARM-software/CMSIS_5/

## Configuration

mbed OS changes to RTX configuration are all maintained in a single file: `mbed-os/rtos/rtx2/mbed_rtx_conf.h`

## Code structure

Due to differences in how mbed OS and CMSIS directory structures look like, we can't import the original code directly, some directory changes are necessary:

CMSIS5 | mbed OS | Explanation |
-------|---------|-------------|
CMSIS_5/CMSIS/Core/Include/core_*.h | mbed-os/cmsis/ | Core specific code |
CMSIS_5/CMSIS/Core/Include/tz_context.h | mbed-os/cmsis/ | TrustZone code |
CMSIS_5/CMSIS/Core/Include/cmsis_compiler.h | mbed-os/cmsis/ | Toolchain generic code |
CMSIS_5/CMSIS/Core/Include/cmsis_{armcc,armclang,gcc}.h | mbed-os/cmsis/TOOLCHAIN_{ARM,GCC}/ | Toolchain specific code |
CMSIS_5/CMSIS/RTOS2/Include/cmsis_os2.h | mbed-os/rtos/rtx2/TARGET_CORTEX_M/ | RTX main header |
CMSIS_5/CMSIS/RTOS2/RTX/Config/ | mbed-os/rtos/rtx2/TARGET_CORTEX_M/ | RTX configuration files |
CMSIS_5/CMSIS/RTOS2/RTX/Include1/ | mbed-os/rtos/rtx/ | RTOS1 compatibility layer |
CMSIS_5/CMSIS/RTOS2/RTX/Include/ | mbed-os/rtos/rtx2/TARGET_CORTEX_M/ | RTX definitions |
CMSIS_5/CMSIS/RTOS2/RTX/Source/rtx_* | mbed-os/rtos/rtx2/TARGET_CORTEX_M/ | RTX sources |
CMSIS_5/CMSIS/RTOS2/RTX/Source/svc_user.c | mbed-os/rtos/rtx2/TARGET_CORTEX_M/ | RTX SVC user table |
CMSIS_5/CMSIS/RTOS2/RTX/Source/{ARM,GCC,IAR}/| mbed-os/rtos/rtx2/TARGET_CORTEX_M/TARGET_{M0,M0P,M3,RTOS_M4_M7}/TOOLCHAIN_{ARM,GCC,IAR} | Toolchain and core specific exception handlers |

## Modification

Due to different use cases between mbed OS and CMSIS, we had to do some modifications to the source code. We've tried to upstream our changes to CMSIS repository, but in cases where they weren't compatible with CMSIS requirements we were forced to maintain small set of changes.

### CMSIS


Filename | Description |
---------|-------------|
cmsis_compiler.h | Added IAR missing __ALIGNED attribute for earlier (less than 7.8.4) versions |
arm_math.h | Rename `PI` to `_PI` to avoid name clash with a register name in mcr20a-rf-driver |

### RTX

Filename | Description |
---------|-------------|
cmsis_os2.h | Doxygen added; added per-thread uVisor context |
cmsis_os1.h | Change `osThreadDef` to accept 3 parameters, rather than 4, and be not static as expected by mbed OS |
core_cm.h | Doxygen added; included headers changed to match mbed OS core selection; deferred priority setting of SVCall to uVisor, when uVisor is enabled |
RTX_Config.h | Doxygen added, mbed OS RTX config included |
rtx_evr.c | CMSIS component definition include removed |
rtx_evr.h | Doxygen added |
rtx_thread.c | Added per-thread uVisor context; notify uVisor of OS events  |
rtx_kernel.c | Added per-thread uVisor context; notify uVisor of OS events |
rtx_lib.h | Doxygen added; added per-thread uVisor context |
rtx_os.h | Doxygen added; added per-thread uVisor context |
irq_cm4.s | For all toolchains: added case for Cortex M4 cores without VFP|
svc_user.c | Removed as it's template file and should not be in our code base|
rt_OsEventObserver.{c,h} | Added an interface for uVisor to get notified about certain events from privileged code |

#### Other

* irq_cm0.s is used for both M0 and M0P cores in mbed OS for all toolchains
