Please note that `Driver_Storage.h` contains the flash abstraction as used in mbed OS.
It is a superset of and based on the corresponding
[CMSIS Flash Driver](https://www.keil.com/pack/doc/CMSIS/Driver/html/group__flash__interface__gr.html#struct_a_r_m___d_r_i_v_e_r___f_l_a_s_h).

The file `Driver_Common.h` has been taken verbatim from CMSIS; it contains a
subset of the core data-types and return codes upon which the storage driver is
built. This file should not need a review.
