/*
 * Copyright (c) 2018 Nuvoton Technology Corp.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NuMaker EMAC Config header file
 */
 
#ifndef NUMAKER_EMAC_CONFIG_H__
#define NUMAKER_EMAC_CONFIG_H__

#define NU_RX_RING_LEN              (8)
#define NU_TX_RING_LEN              (4)

#define NU_ETH_MAX_FLEN             (1520) 

#define NU_HWADDR_SIZE              (6)

#define NU_ETH_MTU_SIZE              1500
#define NU_ETH_IF_NAME               "nu"

#define THREAD_STACKSIZE             512

#endif // #define NUMAKER_EMAC_CONFIG_H__

