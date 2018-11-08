/*
 * flashconfig.c
 *
 * Created: 24.08.2017 20:24:13
 *  Author: Guenter.Prossliner
 */

#include "flashconfig.h"

#include <stdint.h>
#include <string.h>
#include <alloca.h>

#include "blsrv.h"

typedef union {
    uint32_t *w;
    char *c;
    const char *cc;
    intptr_t val;
} cfg_ptr;


// linker-script imports
extern int __bank2_start;
extern int __flashconfig_end;

// we need to use the address of the symbols, because the C compiler dereferences the extern vars automatically
#define start_intptr ((intptr_t)(&__bank2_start))
#define end_intptr ((intptr_t)(&__flashconfig_end))

static cfg_ptr m_endptr;

static void m_checkinit()
{
    // m_endptr needs to be initialized once
    if (m_endptr.val) {
        return;
    }

    // first let's find the first word != 0xFFFFFFFF
    cfg_ptr p_last;
    p_last.val = end_intptr;
    while (p_last.val >= start_intptr && *(--p_last.w) == 0xFFFFFFFF);

    // then let's find the first word 0x00000000
    cfg_ptr p_zero = p_last;
    while (p_zero.val >= start_intptr && *p_zero.w != 0x00000000) {
        p_zero.w--;
    }

    // we may need to recover from uncompleted writes.
    // if this is not true, we zero out the complete block once, so the get state machine in not affected
    size_t incomplete_size = p_last.val - p_zero.val;
    if (incomplete_size) {

        // let's use a stack-buffer
        uint32_t zerob = alloca(incomplete_size);
        memset(zerob, 0, incomplete_size);

        // and write to flash
        p_zero.w++; // don't need to zero out word already zero
        struct blsrv_desc d;
        d.operation = blsrv_write_config_data;
        d.args.write_config_data.address = p_zero.val;
        d.args.write_config_data.buffer = (intptr_t)zerob;
        d.args.write_config_data.buffer_size = incomplete_size;
        blsrv_call(&d);
    }


    // now we are on the first 0xFFFFFFFF again
    m_endptr.w = ++p_last.w;
}

flashconfig_result flashconfig_get_value(const char *name, const char **value)
{
    if (!name) {
        return flashconfig_args_error;
    }
    if (!value) {
        return flashconfig_args_error;
    }

    size_t sname = strlen(name);
    if (!sname) {
        return flashconfig_args_error;
    }

    m_checkinit();

    if (m_endptr.val == start_intptr) {
        return flashconfig_not_found;
    }

    enum {
        s_aftrval,  // after end of a value, incl. termination, so the next word is the endptr, or the first word of the prev. setting
        s_inval,    // we are in the value-string
        s_aftrkey,  // we are in the terminator of a key
        s_inkey,        // we are running the compare for the key
        s_nomatch,  // we have proved the current kvp not to match
        s_match     // we found a match
    } state = s_aftrval;

    const char *valueptr;
    const char *keyptr;

    cfg_ptr p = m_endptr;
    p.w--;  // skip 0xFF byte

    for (; state != s_match && p.val > start_intptr; p.c--) {
        char c = *p.c;

        // please note that the "missing" breaks here are intentional, and part of the state-machine!
        switch (state)   {
            case s_aftrval:
                if (!c) {
                    continue;    // still in \0
                }
                state = s_inval;    // we have anything other than \0

            case s_inval:
                if (c) {
                    continue;    // still in key
                }

                state = s_aftrkey;
                valueptr = p.cc + 1;

            case s_aftrkey:
                if (!c) {
                    continue;    // we are still in \0
                }

                state = s_inkey;        // we are in the key.
                keyptr = name + sname;  // initialize the pointer to compare the arg


            case s_inkey:
                if (!c) {
                    state = s_match;    // done compare
                } else if (c != *--keyptr) {
                    state = s_nomatch;    // no (longer) matching
                }

                continue;

            case s_nomatch:
                if (!c) {
                    state = s_aftrval;
                }
                continue;
        }

    }

    if (state == s_match || state == s_inkey) {
        // found a match!
        *value = valueptr;
        return flashconfig_success;
    }


    return flashconfig_not_found;
}

flashconfig_result flashconfig_set_value(const char *name, char *value)
{
    // let's start with some validation
    if (!name) {
        return flashconfig_args_error;
    }
    if (!value) {
        return flashconfig_args_error;
    }

    size_t sname = strlen(name) + 1;
    size_t svalue = strlen(value) + 1;

    if (!sname || sname >= FLASHCONFIG_NAME_MAX) {
        return flashconfig_args_error;
    }
    if (!svalue || svalue >= FLASHCONFIG_VALUE_MAX) {
        return flashconfig_args_error;
    }

    m_checkinit();

    // build stack buffer with the new setting
    uint32_t buffer[(FLASHCONFIG_NAME_MAX + FLASHCONFIG_VALUE_MAX + 8) / 4];    // alloc as i32 for alignment
    cfg_ptr p;
    p.w = buffer;

    // copy key incl \0
    memcpy(p.val, name, sname);
    p.c += sname;

    // copy value incl \0
    memcpy(p.val, value, svalue);
    p.c += svalue;

    // align to word
    while (p.val % 4) {
        *p.c++ = '\0';
    }

    // add termination word
    *p.w++ = 0;

    size_t memsize = p.val - (intptr_t)buffer;

    if (m_endptr.val + memsize > end_intptr) {
        return flashconfig_overrun;
    }

    // and write to flash
    struct blsrv_desc d;
    d.operation = blsrv_write_config_data;
    d.args.write_config_data.address = m_endptr.val;    /* due a bootloader bug, this is no offset, but an absolute address */
    d.args.write_config_data.buffer = (intptr_t)buffer;
    d.args.write_config_data.buffer_size = memsize;
    blsrv_call(&d);

    m_endptr.c += memsize;

    return flashconfig_success;

}