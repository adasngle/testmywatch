/****************************************************************************
 *
 * File:
 *     $Workfile:avctpcon.h$ for XTNDAccess Blue SDK, Version 2.0
 *     $Revision:7$
 *
 * Description: This file contains the header file for the L2CAP/ACL 
 *              connection state machine for AVCTP connections.
 *             
 * Created:     Mar 31, 2004
 *
 * Copyright 2004 - 2005 Extended Systems, Inc.  ALL RIGHTS RESERVED.
 *
 * Unpublished Confidential Information of Extended Systems, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any means, 
 * or stored in a database or retrieval system, without prior written 
 * permission of Extended Systems, Inc.
 * 
 * Use of this work is governed by a license granted by Extended Systems, Inc. 
 * This work contains confidential and proprietary information of Extended 
 * Systems, Inc. which is protected by copyright, trade secret, trademark and 
 * other intellectual property rights.
 *
 ****************************************************************************/

#ifndef __AVCTPCON_H_
#define __AVCTPCON_H_


#include "l2cap_adp.h"

#include "xatypes.h"
#include "me_adp.h"
#include "bttypes.h"
/* Connection Flags */
#define AVCTP_FLAG_LINK_ACL       0x01

/* Conn Events */
#define AVCTP_CONN_EVENT_CONNECT     1
#define AVCTP_CONN_EVENT_CONNECT_IND 2
#define AVCTP_CONN_EVENT_DISCONNECT  3
#define AVCTP_CONN_EVENT_DATA_SENT   4
#define AVCTP_CONN_EVENT_DATA_IND    5

/* Connection States */
#define AVCTP_CONN_NUM_STATES     6

#define AVCTP_STATE_DISCONNECTED   0
#define AVCTP_STATE_CONN_PENDING   1
#define AVCTP_STATE_CONN_INCOMING  2
#define AVCTP_STATE_DISC_PENDING   3
#define AVCTP_STATE_DISC_INCOMING  4
#define AVCTP_STATE_CONNECTED      5

/* Forward references */
typedef struct _AvctpConnCallbackParms AvctpConnCallbackParms;
typedef struct _AvctpConn AvctpConn;

/* State machine function prototype */
typedef void (*AvctpConnState) (AvctpConn *Conn, L2capCallbackParms *Parms);

/* Conn callback function */
typedef void (*AvctpConnCallback) (AvctpConn *Conn, AvctpConnCallbackParms *Parms);

/* Conn calback parameters */
struct _AvctpConnCallbackParms
{
    U8 event;
    U16 dataLen;
    union
    {
        BtRemoteDevice *remDev;
        U8 *data;
    } ptrs;
};

/* Connection State */
struct _AvctpConn
{
    L2capChannelId l2ChannelId;
    U8 state;
    AvctpConnCallback callback;
    BtRemoteDevice *remDev;
};

/* State machine functions */
void AvctpConnInitStateMachine(void);
void AvctpL2Callback(L2capChannelId l2ChannelId, L2capCallbackParms *parms);

/* Function Prototypes */
void AvctpInitConn(AvctpConn *conn, AvctpConnCallback callback);
BtStatus AvctpConnect(AvctpConn *conn, BtRemoteDevice *RemDev);
BtStatus AvctpConnectRsp(AvctpConn *Conn, BOOL Accept);
BtStatus AvctpDisconnect(AvctpConn *conn);
BOOL AvctpIsConnected(AvctpConn *conn);

#endif /* __AVCTPCON_H_ */ 

