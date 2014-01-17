#ifndef __BTTYPES_H
#define __BTTYPES_H

/****************************************************************************
 *
 * File:
 *     $Workfile:bttypes.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:46$
 *
 * Description:
 *     General types, defines, and prototypes for Blue SDK components.
 *
 * Copyright 1999-2005 Extended Systems, Inc.
 * Portions copyright 2005 iAnywhere Solutions, Inc.
 * All rights reserved. All unpublished rights reserved.
 *
 * Unpublished Confidential Information of iAnywhere Solutions, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any 
 * means, or stored in a database or retrieval system, without prior written 
 * permission of iAnywhere Solutions, Inc.
 * 
 * Use of this work is governed by a license granted by iAnywhere Solutions,
 * Inc.  This work contains confidential and proprietary information of
 * iAnywhere Solutions, Inc. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/

#include "xatypes.h"
#include "btconfig.h"
#include "xastatus.h"
#include "utils.h"
#include "config.h"
#include "btdrv_types.h"
/*---------------------------------------------------------------------------
 * Global Types layer
 *
 *     General types and conversion functions used by all components of
 *     the stack and applications are defined here.
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BDADDR_NTOA_SIZE constant
 *
 *     Defines the minimum size for the string when using the bdaddr_ntoa
 *     function.
 */
#define BDADDR_NTOA_SIZE        18

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

//Move to \mcu\interface\hal\bluetooth\btdrv_types.h
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

/*---------------------------------------------------------------------------
 * BtPacketFlags type
 *
 *     This type is used by L2CAP and protocols that use directly L2CAP
 *     to manage the status of a particular BtPacket.
 */
typedef U8 BtPacketFlags;

#define BTP_FLAG_NONE       0x00        /* No current flags */
#define BTP_FLAG_INUSE      0x01        /* Used only by packet owner */
#define BTP_FLAG_LSYSTEM    0x02        /* Used only by L2CAP */
#define BTP_FLAG_TAIL       0x04        /* Used only by L2CAP Applications */
#define BTP_FLAG_RDEV_FISRT 0x08        /* Used only by L2CAP, retransmission tx first */
#define BTP_FLAG_FCS        0x10        /* FCS field is valid, set only by L2CAP */
#define BTP_FLAG_RDEV_RETX  0x20        /* Used only by L2CAP, retransmission tx packet */





#define BTP_FLAG_HIGH_PRIPORTY       0x20
#define BTP_FLAG_LOW_PRIPORTY       0x40
#define BTP_FLAG_NONE_PRIPORTY       0x80

/* End of BtPacketFlags */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

//Move to \mcu\interface\hal\bluetooth\btdrv_types.h
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined (__BT_2_1_AUTO_FLUSH__)
/* under construction !*/
#endif 
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if XA_STATISTICS == XA_ENABLED
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* XA_STATISTICS == XA_ENABLED */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if L2CAP_FLOW_CONTROL == XA_ENABLED
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* L2CAP_FLOW_CONTROL == XA_ENABLED */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

/* Indicates size of BD_ADDR array */
#define BD_ADDR_SIZE    6

/*---------------------------------------------------------------------------
 * BD_ADDR structure
 *
 *     Represents a 48-bit Bluetooth device address.
 */
typedef struct _BD_ADDR
{
    U8 addr[BD_ADDR_SIZE];
} BD_ADDR;

/* Internal ROM constant defined in btrom.c to represent an invalid address */
extern const BD_ADDR INVALID_BDADDR;

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * bdaddr_aton()
 *
 *     Converts an ASCII-formatted Bluetooth device address into a BD_ADDR
 *     structure.
 *
 * Parameters:
 *     addr - Address in the format "xx:xx:xx:xx:xx", where x is a
 *         hexadecimal digit. The most significant byte appears first.
 *
 * Returns:
 *     BD_ADDR - A structure that contains the binary version of the
 *         address.
 */
BD_ADDR bdaddr_aton(const char *addr);

/*---------------------------------------------------------------------------
 * bdaddr_ntoa()
 *
 *     Converts a BD_ADDR structure into an ASCII-formatted device address.
 *
 * Parameters:
 *     addr - Binary device address to convert.
 *
 *     addrString - Destination string space for formatted device address.
 *         The string must contain at least BDADDR_NTOA_SIZE bytes of space.
 *
 * Returns:
 *     char * - A copy of the pointer passed in as "addrString".
 */
char *bdaddr_ntoa(const BD_ADDR *addr, char *addrString);

/*---------------------------------------------------------------------------
 * AreBdAddrsEqual()
 *
 *     Compares two Bluetooth device addresses for equality.
 *
 * Parameters:
 *     addrA - First address to compare.
 *
 *     addrB - Second address to compare.
 *
 * Returns:
 *     TRUE when the addresses are the same, FALSE when they are different
 */
BOOL AreBdAddrsEqual(const BD_ADDR *addrA, const BD_ADDR *addrB);

#define AreBdAddrsEqual(_ADDRA, _ADDRB)   OS_MemCmp((_ADDRA)->addr, 6, (_ADDRB)->addr, 6)

/*---------------------------------------------------------------------------
 * IsValidBdAddr()
 *
 *     Determines whether a BD_ADDR contains a valid Bluetooth device
 *     address.
 *
 * Parameters:
 *     addr - Address to examine.
 *
 * Returns:
 *     TRUE when the address is valid, FALSE if it is invalid.
 */
BOOL IsValidBdAddr(const BD_ADDR *Addr);

#define IsValidBdAddr(_BDA) (OS_MemCmp((_BDA)->addr, 6, INVALID_BDADDR.addr, 6) == FALSE)

#endif /* __BTTYPES_H */ 

