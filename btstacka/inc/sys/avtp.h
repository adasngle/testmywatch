/****************************************************************************
 *
 * File:
 *     $Workfile:avtp.h$ for XTNDAccess Blue SDK, Version 2.0
 *     $Revision:16$
 *
 * Description: This file contains the API for encoding AVDTP and AVCTP
 *              packets.
 *             
 * Created:     Mar 22, 2004
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

#ifndef __AVTP_H_
#define __AVTP_H_

#include "bttypes.h"
#include "l2cap_adp.h"

#include "utils.h"
#include "xatypes.h"
#include "osapi.h"
#include "eventmgr.h"
/* Maximum length of a packet (Based on the maximum AVDTP/AVCTP packet) */
#define AVTP_MAX_LEN 522

/* Transmission States */
#define AVTP_TRANS_STATE_FIRST      0
#define AVTP_TRANS_STATE_CONTINUE   1

/* Packet Types */
#define AVTP_PACKET_TYPE_SINGLE     0
#define AVTP_PACKET_TYPE_START      1
#define AVTP_PACKET_TYPE_CONTINUE   2
#define AVTP_PACKET_TYPE_END        3

/* Message Types */
#define AVTP_MSG_TYPE_COMMAND       0
#define AVTP_MSG_TYPE_RESERVED        1
#define AVTP_MSG_TYPE_ACCEPT        2
#define AVTP_MSG_TYPE_REJECT        3

/* Events */
#define AVTP_EVENT_TX_DONE            0
#define AVTP_EVENT_TX_TIMEOUT         1
#define AVTP_EVENT_TX_ERROR           2
#define AVTP_EVENT_RX_IND             3
#define AVTP_EVENT_RX_UNKNOWN_PACKET  4
#define AVTP_EVENT_RX_UNKNOWN_MESSAGE 5
#define AVTP_EVENT_RX_BUFFER_UNDERRUN 6
#define AVTP_EVENT_RX_BUFFER_OVERRUN  7
#define AVTP_EVENT_RX_BAD_TRANS_ID    8

/* Status Codes */

/* Forward reference */
typedef struct _AvtpChannel AvtpChannel;
typedef struct _AvtpCallbackParms AvtpCallbackParms;

/* Channel Manager Callback */
typedef void (*AvtpCallback) (AvtpChannel *chnl, AvtpCallbackParms *Parms);

/* Channel Packet */
typedef struct _AvtpPacket
{
    ListEntry node;

    U8 msgType;

    BOOL useMsgHdr;
    U8 msgHdr[3];

    U8 txIdSize;
    U16 txId;
    U8 internal_used;
    U16 txDataLen;
    U8 *txData;
} AvtpPacket;

/* Channel */
struct _AvtpChannel
{

    U32 context;

    /* Identifier */
    U16 rxId;

    /* Transmit Packet */
    ListEntry avPacketList;
    AvtpPacket *curAvPacket;

    /* Transaction ID */
    U8 intTransId;
    U8 acpTransId;

    /* Transmit State */
    U8 txState;
    U16 offset;
    U16 packetSize;
    U8 txPacketsLeft;

    /* Channel Receive State */
    U8 rxState;
    U8 rxPacketsLeft;

    /* Channel Resources */
    BtPacket packet;
    L2capChannelId l2ChannelId;

    /* Command Timeout */
    TimeT txTimeout;
    EvmTimer txTimer;
    BOOL resetTimer;
    BOOL txCmdTimeout;

    /* Channel Callback */
    AvtpCallback callback;
};

/* Channel Callback Parameters */
struct _AvtpCallbackParms
{

    U8 event;           /* Callback event type               */
    BtStatus status;    /* Transport status                  */
    U8 msgType;         /* Message type                      */
    U8 pktType;         /* Packet type                       */
    U16 rxId;           /* RX ID                             */

    U8 packetsLeft;     /* Number of packet still to receive */
    U16 len;            /* Length of the current data        */
    AvtpPacket *packet; /* Pointer to the transmitted packet */
    U8 *data;           /* Pointer to the received data      */
    U8 transid;
};

/* Initialize the channel structure */
void AVTP_InitChannel(AvtpChannel *chnl, L2capChannelId L2ChannelId, AvtpCallback Callback, TimeT Timeout);

/* Deinitializes the channel structure */
void AVTP_DeinitChannel(AvtpChannel *chnl);

/* Start transmission of a packet */
BtStatus AVTP_SendStart(AvtpChannel *chnl, AvtpPacket *packet);

/* Continue and check completion of a packet */
void AVTP_SendContinue(AvtpChannel *chnl);

/* Parse received data */
void AVTP_Receive(AvtpChannel *chnl, U8 *Buffer, U16 Len, U8 rxIdSize);

#endif /* __AVTP_H_ */ 

