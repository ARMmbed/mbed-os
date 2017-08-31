#ifndef INCLUDED_AR1DE_CR
#define INCLUDED_AR1DE_CR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of General Purpose Registers */
#define AR1DE_NR_GP 4
#define AR1DE_CR_PERIPHERALID4 0x07
#define AR1DE_CR_PERIPHERALID5 0x00
#define AR1DE_CR_PERIPHERALID6 0x00
#define AR1DE_CR_PERIPHERALID7 0x00
#define AR1DE_CR_PERIPHERALID0 0x40
#define AR1DE_CR_PERIPHERALID1 0xDA
#define AR1DE_CR_PERIPHERALID2 0x08
#define AR1DE_CR_PERIPHERALID3 0x00
#define AR1DE_CR_COMPONENTID0 0x0D
#define AR1DE_CR_COMPONENTID1 0xF0
#define AR1DE_CR_COMPONENTID2 0x05
#define AR1DE_CR_COMPONENTID3 0xB1

/** Control Interface.
\par The register map consists of the following regions:
   - Component configuration: These registers enable the identification of system components by software and are standard Primecell Peripheral Component Identification Registers.
   - Event Fault control: These registers control events and exception faults behavior in the AR1DE
   - Task descriptor fifo queue configuration: These registers configure and start tasks on the OptimoDE data engine.
	
\par The control interface register map can be divided into two classes of registers based upon the access methodology provided.
   - Task dependent configuration registers.  These are implemented using a fifo queue of depth 2.  The top head element of the queue is allocated to the AHB-lite control interface, whereas the bottom tail element of the queue is allocated to the AR1DPU (and can only be observed, but not written, from the AHB-lite control interface).  This ensures that the next task to be run on the AR1DE can already be setup when the current task is still executing.
   - Task independent configuration registers.  These are implemented as a single register inside the AR1DE.  Writing to these registers will have an immediate effect.
\par Task Control
Starting a task on the data engine is done by writing a task descriptor into the task buffer. Note that only the PENDENTRY field is mandatory.
   - PENDENTRY is a 32-bit field containing the start address of the task in instruction memory. This offset is a byte offset. However because AR1DE cannot access instruction memory on a byte address resolution, the lower bits are ignored by AR1DE.
   - PENDGP0, PENDGP1, PENDGP2 and PENDGP3 are 32-bit general purpose registers. It is up to the programmer to use them to pass arguments to the task to be run or ignore them. They are typically used to pass stack pointers, frame pointers, constant table pointers etc.

When the AR1DE is idle and a task descriptor is committed to the task buffer by writing the PENDENTRY register:
   - The task will be started
   - The content of the pending task general purpose registers is shifted into the running task general purpose registers to make them available to the AR1DE.

The pending task buffer now becomes empty and thus a next task can be uploaded while the previous task is still running.
\par Starting a task
First a task descriptor has to be loaded into the task buffer. A task descriptor is loaded into the task buffer when a value is written in the PENDENTRY register. Loading a task descriptor sets the task pending flag in the PENDSTATUS register. If the AR1DPU is not busy, the task will be started. This clears the task pending flag of the PENDSTATUS register. The task buffer is again available for loading a next task descriptor. Note that there is no mechanism that prevents from overriding the top element of the queue.
An ARM CPU can use Store Multiple instructions (STM, STMIA)bursts to load a task into the task buffer. It should first write the general purpose fields (if present) and end with the PENDENTRY field - since a write to this register sets the task pending flag in the PENDSTATUS register.
\par Clearing the pending task
It is possible the clear the task buffer by writing to the clear pending task flag in the PENDCTRL register. This clears the task pending flag in the PENDSTATUS register. It is strongly advised to first suspend the queue by writing to the suspend pending task flag in the PENDCTRL register. As such you can first check if the task queue contains a pending task descriptor without the data engine consuming it in the meantime. Please note that you don't have to clear the task valid flag in the PENDSTATUS register when you want to replace the top element of the queue. The fastest way to replace the top element of the queue is as follows:
   - Disable the task buffer by writing the suspend pending task flag in the PENDCTRL register
   - Check if the top task buffer contains an element by reading the task pending flag in the PENDSTATUS register.
   - Load a new task descriptor.
   - Enable the task buffer by clearing the suspend pending task flag in the PENDCTRL register.
\par Aborting a running task
Writing to the running task abort flag in the TASKCTRL register will reset the AR1DE. This might take a variable number of cycles, due to the fact that outstanding transactions (shared memory, AXI fabric) may need to be completed first.  The task abort busy flag in the TASKSTATUS register indicates if the reset has completed.  Note that aborting a task does not implicitly remove the pending task from the queue. If there is a pending task after abort is done, it will be automatically started.

*/
struct ar1de_cr_s {
   /** Pending Task General Purpose Register at address offset 0x000, read-write */
   uint32_t pendgp[AR1DE_NR_GP];
   /** Pending Task Entry Address at address offset 0x010, read-write */
   uint32_t pendentry;
   /** Reserved space */
   uint8_t fill0[236];
   /** Running Task General Purpose Register at address offset 0x100, read-only */
   uint32_t taskgp[AR1DE_NR_GP];
   /** Reserved space */
   uint8_t fill1[240];
   /** Suspend the task descriptor FIFO. Clear TASKVALID flag (abort the pending task). Abort the currently running task at address offset 0x200, write-only */
   uint32_t dectrl;
   /** Flag indicating if there is a pending task. Flag indicating the pending task is suspended. Flag indicating if there is a running task busy. Flag indicating if the running task abort is busy. at address offset 0x204, read-only */
   uint32_t destatus;
   /** Reserved space */
   uint8_t fill2[248];
   /** Clear Fault Status at address offset 0x300, write-only */
   uint32_t fltclear;
   /** Fault Status Register at address offset 0x304, read-only */
   uint32_t fltstatus;
   /** Raw Fault Status Register at address offset 0x308, read-only */
   uint32_t fltrawstatus;
   /** Fault Mask Clear at address offset 0x30C, write-only */
   uint32_t fltmaskclr;
   /** Fault Mask Set at address offset 0x310, write-only */
   uint32_t fltmaskset;
   /** Fault Mask Status at address offset 0x314, read-only */
   uint32_t fltmaskstatus;
   /** Reserved space */
   uint8_t fill3[232];
   /** A 32 bit wide field used for further identification of an OptimoDE data engine at address offset 0x400, read-write */
   uint32_t deinstanceid;
   /** Reserved space */
   uint8_t fill4[252];
   /** AR1DE DTCM size (0x0:32kB,0x1:64kB,0x2:128kB) at address offset 0x500, read-only */
   uint32_t dedtcmsize;
   /** AR1DE ITCM size (0x0:32kB,0x1:64kB,0x2:128kB) at address offset 0x504, read-only */
   uint32_t deitcmsize;
   /** Reserved space */
   uint8_t fill5[2760];
   /** Peripheral ID4 at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 at address offset 0xFE0, read-only constant 0x00000040 */
   uint32_t peripheralid0;
   /** Peripheral ID1 at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID3 at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Component ID0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component ID1 at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Component ID2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Component ID3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for ar1de_cr_s#pendgp */
#define AR1DE_CR_PENDGP_PENDGP_OFFSET 0
#define AR1DE_CR_PENDGP_PENDGP_SIZE 32

/** bit field defines for ar1de_cr_s#pendentry */
#define AR1DE_CR_PENDENTRY_PAGE_SELECT_OFFSET 0
#define AR1DE_CR_PENDENTRY_PAGE_SELECT_SIZE 2
#define AR1DE_CR_PENDENTRY_MEM_SELECT_OFFSET 2
#define AR1DE_CR_PENDENTRY_MEM_SELECT_SIZE 1
#define AR1DE_CR_PENDENTRY_TASK_OFFSET_OFFSET 3
#define AR1DE_CR_PENDENTRY_TASK_OFFSET_SIZE 15
#define AR1DE_CR_PENDENTRY_TASK_ID_OFFSET 18
#define AR1DE_CR_PENDENTRY_TASK_ID_SIZE 14

/** bit field defines for ar1de_cr_s#taskgp */
#define AR1DE_CR_TASKGP_TASKGP_OFFSET 0
#define AR1DE_CR_TASKGP_TASKGP_SIZE 32

/** bit field defines for ar1de_cr_s#dectrl */
#define AR1DE_CR_DECTRL_CLEAR_PENDING_TASK_OFFSET 0
#define AR1DE_CR_DECTRL_CLEAR_PENDING_TASK_SIZE 1
#define AR1DE_CR_DECTRL_SUSPEND_PENDING_TASK_OFFSET 1
#define AR1DE_CR_DECTRL_SUSPEND_PENDING_TASK_SIZE 1
#define AR1DE_CR_DECTRL_ABORT_RUNNING_TASK_OFFSET 16
#define AR1DE_CR_DECTRL_ABORT_RUNNING_TASK_SIZE 1

/** bit field defines for ar1de_cr_s#destatus */
#define AR1DE_CR_DESTATUS_PENDING_TASK_VALID_OFFSET 0
#define AR1DE_CR_DESTATUS_PENDING_TASK_VALID_SIZE 1
#define AR1DE_CR_DESTATUS_PENDING_TASK_SUSPENDED_OFFSET 1
#define AR1DE_CR_DESTATUS_PENDING_TASK_SUSPENDED_SIZE 1
#define AR1DE_CR_DESTATUS_TASK_BUSY_OFFSET 16
#define AR1DE_CR_DESTATUS_TASK_BUSY_SIZE 1
#define AR1DE_CR_DESTATUS_TASK_ABORT_BUSY_OFFSET 18
#define AR1DE_CR_DESTATUS_TASK_ABORT_BUSY_SIZE 1

/** bit field defines for ar1de_cr_s#fltclear */
#define AR1DE_CR_FLTCLEAR_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTCLEAR_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTCLEAR_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTCLEAR_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTCLEAR_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTCLEAR_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTCLEAR_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTCLEAR_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTCLEAR_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTCLEAR_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTCLEAR_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTCLEAR_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTCLEAR_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTCLEAR_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTCLEAR_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTCLEAR_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTCLEAR_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTCLEAR_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTCLEAR_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTCLEAR_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#fltstatus */
#define AR1DE_CR_FLTSTATUS_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTSTATUS_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTSTATUS_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTSTATUS_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTSTATUS_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTSTATUS_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTSTATUS_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTSTATUS_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTSTATUS_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTSTATUS_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTSTATUS_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTSTATUS_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTSTATUS_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTSTATUS_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTSTATUS_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTSTATUS_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#fltrawstatus */
#define AR1DE_CR_FLTRAWSTATUS_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTRAWSTATUS_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTRAWSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTRAWSTATUS_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTRAWSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTRAWSTATUS_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTRAWSTATUS_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTRAWSTATUS_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTRAWSTATUS_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTRAWSTATUS_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTRAWSTATUS_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTRAWSTATUS_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#fltmaskclr */
#define AR1DE_CR_FLTMASKCLR_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTMASKCLR_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTMASKCLR_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTMASKCLR_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTMASKCLR_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTMASKCLR_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTMASKCLR_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKCLR_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTMASKCLR_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKCLR_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTMASKCLR_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKCLR_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTMASKCLR_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKCLR_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTMASKCLR_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#fltmaskset */
#define AR1DE_CR_FLTMASKSET_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTMASKSET_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSET_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTMASKSET_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSET_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTMASKSET_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSET_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTMASKSET_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSET_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTMASKSET_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKSET_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTMASKSET_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKSET_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTMASKSET_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSET_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTMASKSET_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSET_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTMASKSET_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSET_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTMASKSET_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#fltmaskstatus */
#define AR1DE_CR_FLTMASKSTATUS_DTCM_UNALIGNED_READ_ACCESS_OFFSET 0
#define AR1DE_CR_FLTMASKSTATUS_DTCM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_OFFSET 1
#define AR1DE_CR_FLTMASKSTATUS_DTCM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DMEM_UNALIGNED_READ_ACCESS_OFFSET 2
#define AR1DE_CR_FLTMASKSTATUS_DMEM_UNALIGNED_READ_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_OFFSET 3
#define AR1DE_CR_FLTMASKSTATUS_DMEM_UNALIGNED_WRITE_ACCESS_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DMEM_READ_FAULT_OFFSET 4
#define AR1DE_CR_FLTMASKSTATUS_DMEM_READ_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DMEM_WRITE_FAULT_OFFSET 5
#define AR1DE_CR_FLTMASKSTATUS_DMEM_WRITE_FAULT_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_SOFTWARE_EXCEPTION_OFFSET 6
#define AR1DE_CR_FLTMASKSTATUS_SOFTWARE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_AXI_READ_EXCEPTION_OFFSET 7
#define AR1DE_CR_FLTMASKSTATUS_AXI_READ_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_AXI_WRITE_EXCEPTION_OFFSET 8
#define AR1DE_CR_FLTMASKSTATUS_AXI_WRITE_EXCEPTION_SIZE 1
#define AR1DE_CR_FLTMASKSTATUS_DMEM_INSTR_FETCH_READ_FAULT_OFFSET 9
#define AR1DE_CR_FLTMASKSTATUS_DMEM_INSTR_FETCH_READ_FAULT_SIZE 1

/** bit field defines for ar1de_cr_s#deinstanceid */
#define AR1DE_CR_DEINSTANCEID_DEINSTANCEID_OFFSET 0
#define AR1DE_CR_DEINSTANCEID_DEINSTANCEID_SIZE 32

/** bit field defines for ar1de_cr_s#dedtcmsize */
#define AR1DE_CR_DEDTCMSIZE_DEDTCMSIZE_OFFSET 0
#define AR1DE_CR_DEDTCMSIZE_DEDTCMSIZE_SIZE 8

/** bit field defines for ar1de_cr_s#deitcmsize */
#define AR1DE_CR_DEITCMSIZE_DEITCMSIZE_OFFSET 0
#define AR1DE_CR_DEITCMSIZE_DEITCMSIZE_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid4 */
#define AR1DE_CR_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define AR1DE_CR_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid5 */
#define AR1DE_CR_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define AR1DE_CR_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid6 */
#define AR1DE_CR_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define AR1DE_CR_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid7 */
#define AR1DE_CR_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define AR1DE_CR_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid0 */
#define AR1DE_CR_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define AR1DE_CR_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid1 */
#define AR1DE_CR_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define AR1DE_CR_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid2 */
#define AR1DE_CR_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define AR1DE_CR_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for ar1de_cr_s#peripheralid3 */
#define AR1DE_CR_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define AR1DE_CR_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for ar1de_cr_s#componentid0 */
#define AR1DE_CR_COMPONENTID0_COMPONENTID0_OFFSET 0
#define AR1DE_CR_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for ar1de_cr_s#componentid1 */
#define AR1DE_CR_COMPONENTID1_COMPONENTID1_OFFSET 0
#define AR1DE_CR_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for ar1de_cr_s#componentid2 */
#define AR1DE_CR_COMPONENTID2_COMPONENTID2_OFFSET 0
#define AR1DE_CR_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for ar1de_cr_s#componentid3 */
#define AR1DE_CR_COMPONENTID3_COMPONENTID3_OFFSET 0
#define AR1DE_CR_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF ar1de_cr.h */
#endif
