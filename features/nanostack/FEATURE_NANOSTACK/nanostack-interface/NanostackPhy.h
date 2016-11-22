/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#ifndef NANOSTACK_PHY_H_
#define NANOSTACK_PHY_H_

class NanostackPhy {
public:

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t phy_register() = 0;

    /** Read the mac address of this physical interface
     *
     * Note - some devices do not have a mac address
     *        in hardware.
     */
    virtual void get_mac_address(uint8_t *mac) = 0;

    /** Set the mac address of this physical interface
     *
     */
    virtual void set_mac_address(uint8_t *mac) = 0;

protected:
    NanostackPhy() {}
    virtual ~NanostackPhy() {}
};

#endif /* NANOSTACK_INTERFACE_H_ */
