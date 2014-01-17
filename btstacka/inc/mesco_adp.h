#ifndef __MESCO_ADP_H
#define __MESCO_ADP_H

/****************************************************************************
 *
 * File:
 *     $Workfile:mesco.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:49$
 *
 * Description:
 *     Public types, defines, and prototypes for accessing the
 *     upper layer of the Management Entity.
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

#include "btconfig.h"
#include "xatypes.h"
#include "utils.h"
#include "config.h"

#if NUM_SCO_CONNS > 0

/*---------------------------------------------------------------------------
 * SCO Manager (SCO) layer
 *
 *     This layer is part of the Management Entity. It allows applications
 *     to establish SCO connections and exchange audio data. SCO connections
 *     are derived from existing ACL (data) connections.
 *
 *     SCO-related events may be received by an application in two different
 *     ways: 1) All SCO-related events are sent to global handlers with
 *     appropriate masks (see ME_RegisterGlobalHandler). 2) An application
 *     must register an SCO handler (SCO_RegisterHandler) that
 *     receives only those SCO events that are associated with a certain
 *     ACL link.
 */

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BtScoPacketType type
 *
 *     Bluetooth SCO Packet types are the different packet types allowed on
 *     an SCO link. They can be ORed together.
 */
typedef U16 BtScoPacketType;

#define BSPT_DM1   0x0008
#define BSPT_HV1   0x0020
#define BSPT_HV2   0x0040
#define BSPT_HV3   0x0080
#define BSPT_DV    0x0100

/* End of BtScoPacketType */

/*---------------------------------------------------------------------------
 * BtEscoPktType type
 *
 *     Bluetooth eSCO Packet types are the different packet types allowed on
 *     an eSCO link. They can be ORed together.
 */
typedef U16 BtEscoPktType;

#define BESPT_HV1      0x0001
#define BESPT_HV2      0x0002
#define BESPT_HV3      0x0004
#define BESPT_EV3      0x0008
#define BESPT_EV4      0x0010
#define BESPT_EV5      0x0020
#define BESPT_NO_2_EV3 0x0040
#define BESPT_NO_3_EV3 0x0080
#define BESPT_NO_2_EV5 0x0100
#define BESPT_NO_3_EV5 0x0200

/* End of BtEscoPktType */

/*---------------------------------------------------------------------------
 * BtScoAudioSettings type
 *
 *     Bluetooth SCO Audio Settings describe the input and air formats for
 *     the SCO link. To specify audio settings, OR together one item from
 *     each group.
 */
#ifndef __ME_H
/* Note that this definition must match the one in me.h */
typedef U16 BtScoAudioSettings;
#endif /* __ME_H */ 

/* 
 * The default settings (equivalent to (BSVS_IN_SAMPLE_16BIT |
 * BSVS_IN_DATA_TWOS) 
 */
#define BSAS_DEFAULT            0x0060

/* Group: Input Coding. Select one of the following values. */
#define BSAS_IN_CODING_LINEAR   0x0000  /* Linear */
#define BSAS_IN_CODING_ULAW     0x0100  /* u-law */
#define BSAS_IN_CODING_ALAW     0x0200  /* a-law */

/* Group: Input Data Format. Select one of the following values. */
#define BSAS_IN_DATA_ONES       0x0000  /* 1's complement */
#define BSAS_IN_DATA_TWOS       0x0040  /* 2's complement */
#define BSAS_IN_DATA_SM         0x0080  /* Sign-Magnitude */

/* Group: Input Sample Size (for Linear PCM only). Select one of the
 * following values. 
 */
#define BSAS_IN_SAMPLE_8BIT     0x0000  /* 8 bit */
#define BSAS_IN_SAMPLE_16BIT    0x0020  /* 16 bit */

/* Group: Air Coding Format. Select one of the following values. */
#define BSAS_CVSD               0x0000  /* CVSD */
#define BSAS_ULAW               0x0001  /* u-LAW */
#define BSAS_ALAW               0x0002  /* A-LAW */

/* End of BtScoAudioSettings */

#endif /* NUM_SCO_CONNS > 0 */
#endif /* __MESCO_ADP_H */
