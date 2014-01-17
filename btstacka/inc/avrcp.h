/****************************************************************************
 *
 * File:
 *     $Workfile:avrcp.h$ for XTNDAccess Blue SDK, Version 2.0
 *     $Revision:37$
 *
 * Description: This file contains public definitions for the Audio/Video
 *     Remote Control Profile (AVRCP).
 *             
 * Created:     Nov 19, 2003
 *
 * Copyright 2003 - 2005 Extended Systems, Inc.  ALL RIGHTS RESERVED.
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

#ifndef __AVRCP_H_
#define __AVRCP_H_

#include "avctp.h"
#include "conmgr.h"

#include "config.h"
#include "xatypes.h"
#include "sdp_adp.h"
#include "me_adp.h"
#include "eventmgr.h"
#include "bttypes.h"
#include "bt_avrcp_struct.h"
/*---------------------------------------------------------------------------
 * AVRCP API layer
 *
 *     The Audio/Video Remote Control Profile (AVRCP) defines procedures for
 *     exchanging 1394 Trade Association AV/C commands between Bluetooth
 *     enabled Audio/Video devices. Internally, AVRCP uses the Audio/Video
 *     Control Transport Protocol (AVCTP) to exchange messages.
 *
 *     This API is designed to support AV remote control applications using
 *     the XTNDAccess Blue SDK core protocol stack.  It provides an API
 *     for connection management and message handling.
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AVRCP_PANEL_SUBUNIT constant
 *
 *     When set to XA_ENABLED, the AVRCP profile adds handling for
 *     panel subunit commands. Most AVRCP implementations use this subset
 *     of commands.
 *
 *     For targets, the application will receive AVRCP_EVENT_PANEL_PRESS,
 *     _HOLD, and _RELEASE events instead of the AVRCP_EVENT_COMMAND event
 *     for all panel subunit AVRCP_OPCODE_PASS_THROUGH commands
 *     except AVRCP_POP_VENDOR_UNIQUE. In addition, UNIT_INFO and
 *     SUBUNIT_INFO commands will be handled automatically, should they
 *     occur. All other commands will appear in ordinary AVRCP_EVENT_COMMAND
 *     events; responses for these commands MUST be sent.
 *
 *     For controllers, the application may invoke the AVRCP_SetPanelKey()
 *     API, and will receive AVRCP_EVENT_PANEL_CNF notifications to indicate
 *     the target's responses.
 */
#ifndef AVRCP_PANEL_SUBUNIT
#define AVRCP_PANEL_SUBUNIT XA_DISABLED
#endif /* AVRCP_PANEL_SUBUNIT */

/*---------------------------------------------------------------------------
 * AVRCP_PANEL_COMPANY_ID constant
 *
 *     Defines the company ID used in UNIT INFO commands received by a
 *     target when AVRCP_PANEL_SUBUNIT is set to XA_ENABLED. This value
 *     may be left at the default or replaced with a value assigned
 *     by the IEEE Registration Authority Committee as defined in the 1394
 *     AV/C Digital Interface Command Set, version 1.1, section 9.12.
 *
 *     This 24-bit value is expressed in a string as a three-byte value with
 *     most significant byte first.
 */
#define AVRCP_PANEL_COMPANY_ID "\xFF\xFF\xFF"


/*---------------------------------------------------------------------------
 * AVRCP_SUBUNIT_OP_QUEUE_MAX constant
 *
 *     Defines the length of the internal operation queue used on the
 *     controller side by AVRCP_SetPanelKey. To buffer n keystrokes,
 *     set this constant to (n*2)+1. Attempts to add keystrokes beyond
 *     the queue size with AVRCP_SetPanelKey() will fail.
 */
#ifndef AVRCP_SUBUNIT_OP_QUEUE_MAX
#define AVRCP_SUBUNIT_OP_QUEUE_MAX 15

#if AVRCP_SUBUNIT_OP_QUEUE_MAX > 255
#error AVRCP_SUBUNIT_OP_QUEUE_MAX must be no greater than 255.
#endif /* AVRCP_SUBUNIT_OP_QUEUE_MAX > 255 */

#endif /* AVRCP_SUBUNIT_OP_QUEUE_MAX */


/* Other internal constants */

#define AVRCP_NUM_SEARCH_ATTRIBUTES 21
#define AVRCP_NUM_ATTRIBUTES 4

/* Number of milliseconds to wait before deciding that a pressed key is being
 * held (target-side)
 */
#define AVRCP_PANEL_PRESSHOLD_TIME 300

/* How long to wait before assuming that the currently held key has been
 * released (target-side)
 */
#define AVRCP_PANEL_AUTORELEASE_TIME 2000

/* How long to wait between updates for a held-down key (controller-side)
 */
#define AVRCP_PANEL_HOLDUPDATE_TIME 1000

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvrcpEvent type
 *      
 */
typedef AvctpEvent AvrcpEvent;

/** The transport layer is connected and commands/responses can now
 * be exchanged.
 * 
 *  During this callback, the 'p.remDev' parameter is valid.
 */
#define AVRCP_EVENT_CONNECT             AVCTP_EVENT_CONNECT
#define AVRCP_EVENT_CONNECT_IND         AVCTP_EVENT_CONNECT_IND

/** The application will receive this event when a lower layer connection
 *  (L2CAP) has been disconnected.  Both the target and controller of the
 *  connection are notified.
 * 
 */
#define AVRCP_EVENT_DISCONNECT          AVCTP_EVENT_DISCONNECT

/** A command was received from the remote AVCTP device (controller).
 * 
 *  During this callback, the 'p.cmdFrame' parameter is valid. It contains the
 *  the AVRCP command header information, including operands. If the "more"
 *  value is TRUE then this event only signals the first part of the operands.
 *  Subsequent AVRCP_EVENT_OPERANDS events will follow this event with
 *  additional operand data.
 *
 *  Note that the AVRCP specification requires that target devices respond to
 *  commands within 100ms after receiving the command.
 */
#define AVRCP_EVENT_COMMAND             AVCTP_EVENT_COMMAND

/** A response was received from the remote AVCTP device (target).
 * 
 * During this callback, the 'p.rspFrame' parameter is valid. It contains the
 * the AVRCP response header information, including operands. If the "more"
 * value is TRUE then this event only signals the first part of the operands.
 * Subsequent AVRCP_EVENT_OPERANDS events will follow this event with
 * additional operand data.
 */
#define AVRCP_EVENT_RESPONSE            AVCTP_EVENT_RESPONSE

/** The remote device (target) rejected the AVCTP command.
 * 
 * During this callback, the 'p.rspFrame' parameter is valid. It contains the
 * the AVRCP reject header information, including operands. If the "more"
 * value is TRUE then this event only signals the first part of the operands.
 * Subsequent AVCTP_EVENT_OPERANDS events will follow this event with
 * additional operand data.
 */
#define AVRCP_EVENT_REJECT              AVCTP_EVENT_REJECT

/** A command (see AVRCP_SendCommand) or response (see AVRCP_SendResponse)
 * has been sent. Memory allocated for the operation can be freed or reused
 * after receiving this event.
 * 
 * During this callback, the 'p.cmdFrame' or 'p.rspFrame' parameter associated
 * with the sent command or response is valid. In addition, "status" will be
 * set to indicate "BT_STATUS_SUCCESS" or "BT_STATUS_FAILED" to indicate
 * whether the event was properly delivered.
 */
#define AVRCP_EVENT_TX_DONE             AVCTP_EVENT_TX_DONE

/** Additional operand data has been received for the previous
 * AVRCP_EVENT_COMMAND or AVRCP_EVENT_RESPONSE.
 * 
 * During this callback, the 'p.cmdFrame' or 'p.rspFrame' parameter associated
 * with the received command or response is valid. The "operands" and
 * "operandLen" fields indicate the chunk of operands being received for
 * the command or response. If the "more" field is set to TRUE, the full
 * operand buffer will be received in multiple _OPERANDS events and the
 * last operand buffer indicated with the "more" field set to FALSE.
 */
#define AVRCP_EVENT_OPERANDS            AVCTP_EVENT_OPERANDS

#if AVRCP_PANEL_SUBUNIT == XA_ENABLED

/** The key corresponding to a panel operation has been pressed on the
 * remote controller device. See p.panelInd for information about the
 * operation.
 *
 * AVRCP_RejectPanelOperation may be used to reject an unsupported or
 * reserved command. If the operation is not rejected during the callback
 * it is implicitly accepted. If accepted, the next panel-related event
 * for the operation will be AVRCP_EVENT_PANEL_HOLD (if the controller key
 * is held down for at least AVRCP_PANEL_PRESSHOLD_TIME) or
 * AVRCP_EVENT_PANEL_RELEASE (if the controller key is released more quickly).
 *
 * This event is only generated when AVRCP_PANEL_SUBUNIT is set to XA_ENABLED.
 */
#define AVRCP_EVENT_PANEL_PRESS        (AVCTP_EVENT_LAST + 1)

/** The key corresponding to a panel operation has been held down on the
 * remote controller device for at least AVRCP_PANEL_PRESSHOLD_TIME.
 * See p.panelInd for information about the operation.
 *
 * A target receiving this event should act as if the key was held down until
 * the AVRCP_EVENT_PANEL_RELEASE event is received.
 *
 * This event is only generated when AVRCP_PANEL_SUBUNIT is set to XA_ENABLED. 
 */
#define AVRCP_EVENT_PANEL_HOLD         (AVCTP_EVENT_LAST + 2)

/** The key corresponding to a panel operation has been released on the
 * remote controller device.  See p.panelInd for information about the
 * operation.
 *
 * This event is only generated when AVRCP_PANEL_SUBUNIT is set to XA_ENABLED.
 */
#define AVRCP_EVENT_PANEL_RELEASE      (AVCTP_EVENT_LAST + 3)

/** A panel response has been received from the remote target device.
 * All fields of "p.panelCnf" are valid.
 *
 * This event is only generated when AVRCP_PANEL_SUBUNIT is set to XA_ENABLED. 
 */
#define AVRCP_EVENT_PANEL_CNF          (AVCTP_EVENT_LAST + 4)

#endif /* AVRCP_PANEL_SUBUNIT == XA_ENABLED */
/**  Fail to send a Tx packet
 * 
 */

#ifdef __BT_AVRCP_V13__
#define AVRCP_EVENT_TX_ERROR           AVCTP_EVENT_LAST + 5
#define AVRCP_EVENT_SDP_DONE          AVCTP_EVENT_LAST + 6
#endif
/* End of AvrcpEvent */

/*---------------------------------------------------------------------------
 * AvrcpRole type
 *      
 */
typedef U16 AvrcpRole;

/* AVRCP Controller role */
#define AVRCP_CT    SC_AV_REMOTE_CONTROL

/* AVRCP Target role */
#define AVRCP_TG    SC_AV_REMOTE_CONTROL_TARGET
/* End of AvrcpRole */

/*---------------------------------------------------------------------------
 * AvrcpCtype type
 *
 * This type defines the AV/C ctype (command type) codes.
 */
typedef AvctpCtype AvrcpCtype;

#define AVRCP_CTYPE_CONTROL               AVCTP_CTYPE_CONTROL
#define AVRCP_CTYPE_STATUS                AVCTP_CTYPE_STATUS
#define AVRCP_CTYPE_SPECIFIC_INQUIRY      AVCTP_CTYPE_SPECIFIC_INQUIRY
#define AVRCP_CTYPE_NOTIFY                AVCTP_CTYPE_NOTIFY
#define AVRCP_CTYPE_GENERAL_INQUIRY       AVCTP_CTYPE_GENERAL_INQUIRY
/* End of AvrcpCtype */

/*---------------------------------------------------------------------------
 * AvrcpResponse type
 *
 * This type defines the AV/C response codes.
 */
typedef AvctpResponse AvrcpResponse;

#define AVRCP_RESPONSE_NOT_IMPLEMENTED    AVCTP_RESPONSE_NOT_IMPLEMENTED   
#define AVRCP_RESPONSE_ACCEPTED           AVCTP_RESPONSE_ACCEPTED          
#define AVRCP_RESPONSE_REJECTED           AVCTP_RESPONSE_REJECTED          
#define AVRCP_RESPONSE_IN_TRANSITION      AVCTP_RESPONSE_IN_TRANSITION     
#define AVRCP_RESPONSE_IMPLEMENTED_STABLE AVCTP_RESPONSE_IMPLEMENTED_STABLE
#define AVRCP_RESPONSE_CHANGED            AVCTP_RESPONSE_CHANGED           
#define AVRCP_RESPONSE_INTERIM            AVCTP_RESPONSE_INTERIM

/* This code, when received in an AVRCP_EVENT_PANEL_CNF event, indicates
 * that a "release" command was not actually delvered to the target because
 * the original "press" command was rejected.
 *
 * This value is NOT legal for use in functions that accept AvrcpResponse
 * as a parameter.
 */
#define AVRCP_RESPONSE_SKIPPED            0xF0

/* This code, when received in an AVRCP_EVENT_PANEL_CNF event, indicates
 * that the expected response message from the target was not received
 * within the expected time frame. The application may proceed normally
 * as if the command was accepted, or take some other action.
 *
 * This value is NOT legal for use in functions that accept AvrcpResponse
 * as a parameter.
 *
 * Note: The TIMEOUT response is currently not implemented. AVRCP
 * expects, as per specifications, that all commands will receive a response.
 * Commands that timeout therefore cause a service link disconnection.
 */
#define AVRCP_RESPONSE_TIMEOUT            0xF1
/* End of AvrcpResponse */

/*---------------------------------------------------------------------------
 * AvrcpOpcode type
 *
 * This type defines the AV/C Opcodes.
 */
typedef AvctpOpcode AvrcpOpcode;

#define AVRCP_OPCODE_VENDOR_DEPENDENT     AVCTP_OPCODE_VENDOR_DEPENDENT
#define AVRCP_OPCODE_UNIT_INFO            AVCTP_OPCODE_UNIT_INFO       
#define AVRCP_OPCODE_SUBUNIT_INFO         AVCTP_OPCODE_SUBUNIT_INFO    
#define AVRCP_OPCODE_PASS_THROUGH         AVCTP_OPCODE_PASS_THROUGH    
/* End of AvrcpOpcode */

/*---------------------------------------------------------------------------
 * AvrcpCmdFrame type
 *
 * Defines the parameters required for an RCP command.
 */
typedef AvctpCmdFrame AvrcpCmdFrame;

/* End of AvrcpCmdFrame */

/*---------------------------------------------------------------------------
 * AvrcpRspFrame type
 *
 * Defines the parameters required for an RCP response.
 */
typedef AvctpRspFrame AvrcpRspFrame;

/* End of AvrcpRspFrame */

/*---------------------------------------------------------------------------
 * AvrcpSubunitType type
 *
 *     Defines recognized AVRCP subunit types 
 */
typedef U8 AvrcpSubunitType;

/* Panel subunit type, as specified by the 1394 AV/C Panel Subunit
 * Specification
 */
#define AVRCP_SUBUNIT_PANEL        0x09

/* Unit type, used when requesting Unit Info or Subunit Info */
#define AVRCP_SUBUNIT_UNIT         0x1F

/* End of AvrcpSubunitType */

/*---------------------------------------------------------------------------
 * AvrcpPanelOperation type
 *
 *     Panel subunit operations that may be sent (by a controller) or
 *     received (by a target). These codes are defined by the 1394
 *     AV/C Panel Subunit Specification (version 1.1).
 */
typedef U8 AvrcpPanelOperation;

#define AVRCP_POP_SELECT            0x00
#define AVRCP_POP_UP                0x01
#define AVRCP_POP_DOWN              0x02
#define AVRCP_POP_LEFT              0x03
#define AVRCP_POP_RIGHT             0x04
#define AVRCP_POP_RIGHT_UP          0x05
#define AVRCP_POP_RIGHT_DOWN        0x06
#define AVRCP_POP_LEFT_UP           0x07
#define AVRCP_POP_LEFT_DOWN         0x08
#define AVRCP_POP_ROOT_MENU         0x09
#define AVRCP_POP_SETUP_MENU        0x0A
#define AVRCP_POP_CONTENTS_MENU     0x0B
#define AVRCP_POP_FAVORITE_MENU     0x0C
#define AVRCP_POP_EXIT              0x0D

#define AVRCP_POP_0                 0x20
#define AVRCP_POP_1                 0x21
#define AVRCP_POP_2                 0x22
#define AVRCP_POP_3                 0x23
#define AVRCP_POP_4                 0x24
#define AVRCP_POP_5                 0x25
#define AVRCP_POP_6                 0x26
#define AVRCP_POP_7                 0x27
#define AVRCP_POP_8                 0x28
#define AVRCP_POP_9                 0x29
#define AVRCP_POP_DOT               0x2A
#define AVRCP_POP_ENTER             0x2B
#define AVRCP_POP_CLEAR             0x2C

#define AVRCP_POP_CHANNEL_UP        0x30
#define AVRCP_POP_CHANNEL_DOWN      0x31
#define AVRCP_POP_PREVIOUS_CHANNEL  0x32
#define AVRCP_POP_SOUND_SELECT      0x33
#define AVRCP_POP_INPUT_SELECT      0x34
#define AVRCP_POP_DISPLAY_INFO      0x35
#define AVRCP_POP_HELP              0x36
#define AVRCP_POP_PAGE_UP           0x37
#define AVRCP_POP_PAGE_DOWN         0x38

#define AVRCP_POP_POWER             0x40
#define AVRCP_POP_VOLUME_UP         0x41
#define AVRCP_POP_VOLUME_DOWN       0x42
#define AVRCP_POP_MUTE              0x43
#define AVRCP_POP_PLAY              0x44
#define AVRCP_POP_STOP              0x45
#define AVRCP_POP_PAUSE             0x46
#define AVRCP_POP_RECORD            0x47
#define AVRCP_POP_REWIND            0x48
#define AVRCP_POP_FAST_FORWARD      0x49
#define AVRCP_POP_EJECT             0x4A
#define AVRCP_POP_FORWARD           0x4B
#define AVRCP_POP_BACKWARD          0x4C

#define AVRCP_POP_ANGLE             0x50
#define AVRCP_POP_SUBPICTURE        0x51

#define AVRCP_POP_F1                0x71
#define AVRCP_POP_F2                0x72
#define AVRCP_POP_F3                0x73
#define AVRCP_POP_F4                0x74
#define AVRCP_POP_F5                0x75

#define AVRCP_POP_VENDOR_UNIQUE     0x7E

#define AVRCP_POP_RESERVED          0x7F

/* End of AvrcpPanelOperation */

#ifdef __BT_AVRCP_V13__

/* Packet Types */
#define AVRCP_PACKET_TYPE_SINGLE     0
#define AVRCP_PACKET_TYPE_START      1
#define AVRCP_PACKET_TYPE_CONTINUE   2
#define AVRCP_PACKET_TYPE_END        3

/* AV13 AVRCP cmd format type
 * Pass throught:
 *   AVCTP(3B) + A/V header (3B) + key and parameter(2B)
 * AVRCP specific:
 *   AVCTP(3B) + A/V header (3B) + BT SIG company ID(3B) + PDUID (1B) + packtype(1B) + Len(2B)
 * AVRCP browse:
 *   AVCTP(3B) + PDUID (1B) + Len(2B)
 * AVC:
 *   AVCTP(3B) + A/V header (3B) + payload
 */
#define AVRCP_CMD_FORMAT_PASS_THROUGH     0x00
#define AVRCP_CMD_FORMAT_AVRCP_SPECIFIC   0x01
#define AVRCP_CMD_FORMAT_AVC              0x03
/* End of AVRCP cmd format */

#define AVRCP_NUM_TX_PACKETS 5 

#ifdef __TG_V13__
#define AVRCP_MAX_PACKET_LEN 1024
#else
#define AVRCP_MAX_PACKET_LEN 100
#endif

#define AVRCP_TX_IDLE        0
#define AVRCP_TX_SENDING     1

/* Wait for CT to send AVRCP continue packet */
#define AVRCP_TX_CONTINUE_TIMEOUT    10

/* How long to wait before receive a continue/abort packet
 */
#define BT_AVRCP_CONTINUE_WAIT_TIMEOUT 10000


/* AV13 AvrcpSpecific cmd opcode */
#define AVRCP_SPECIFIC_OPCODE_GET_CAPABILITIES                    0x10
#define AVRCP_SPECIFIC_OPCODE_LIST_PLAYERAPP_SETTINGATTRIBUTE     0x11
#define AVRCP_SPECIFIC_OPCODE_LIST_PLAYERAPP_SETTINGVALUE         0x12
#define AVRCP_SPECIFIC_OPCODE_GET_CURPLAYERAPP_SETTINGVALUE       0x13
#define AVRCP_SPECIFIC_OPCODE_SET_PLAYERAPP_SETTINGVALUE          0x14
#define AVRCP_SPECIFIC_OPCODE_GET_PLAYERAPP_SETTINGATTRIBUTETEXT  0x15
#define AVRCP_SPECIFIC_OPCODE_GET_PLAYERAPP_SETTINGVALUETEXT      0x16
#define AVRCP_SPECIFIC_OPCODE_INFORM_DISPLAY_CHARSET              0x17
#define AVRCP_SPECIFIC_OPCODE_INFORM_BATTERY_STATUSOFCT           0x18
#define AVRCP_SPECIFIC_OPCODE_GET_ELEMENT_ATTRIBUTES              0x20
#define AVRCP_SPECIFIC_OPCODE_GET_PLAYERAPP_STATUS                0x30
#define AVRCP_SPECIFIC_OPCODE_REGISTER_NOFITICATION               0x31      
#define AVRCP_SPECIFIC_OPCODE_REQUEST_CONTINUING                  0x40
#define AVRCP_SPECIFIC_OPCODE_ABORT_CONTINUING                    0x41
/* End of AvrcpSpecific cmd opcode */


typedef struct _AvrcpQueuePacketFrame AvrcpQueuePacketFrame;

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/* Basic Channel info for send back */
typedef struct _AvrcpChnlInfo {
    U8 chnl_num;
    U8 transId;
} AvrcpChnlInfo; // AVRCP channel info


/*---------------------------------------------------------------------------
 * AV13: AvrcpSpecific Cmd structure
 * Contains data for a AV panel pass through cmd or AVRCP specific cmd or AVRCP browse cmd.
 */
struct _AvrcpQueuePacketFrame{
    ListEntry node;
   
    /* cmd type AVRCP_CMD_TYPE ex. AVRCP_CMD_FORMAT_PASS_THROUGH */
    U8 cmd_type; 

    /* AVCTP header */
    U8 is_cmd; /* cmd (0) or response (1) */
    U8 transId; /* AVCTP Transaction ID Note: Ignore this.*/
    U8 av_ctype; /* avctp control / status type ex. AVCTP_RESPONSE_ACCEPTED/AVCTP_RESPONSE_IMPLEMENTED_STABLE */
    U8 av_addr; /* subunit type (5 bits) & subunit Id (3 bits)*/
    
    /* AV/C header */
    U8 av_opcode; /* opcode value. 0 for vendor dependent 0x7c for pass through */
    
    /* AVRCP specific cmd header */
    U8 pdu_id; /* AVRCP specific PDU ID */

    /* Abort the packet's transmission */
    U8 abort;
    
    /* pointer to payload data */
    U8 *data;
    
    /* the total size of payload data. Note! No opcode No pdu_id*/
    U16 data_len;
    /* abort flag */
    U8 is_abort;
};
#endif

/* Forward reference */
typedef struct _AvrcpChannel AvrcpChannel;
typedef struct _AvrcpCallbackParms AvrcpCallbackParms;

/*---------------------------------------------------------------------------
 * AvrcpCallback type
 *
 * A function of this type is called to indicate events to the application.
 */
typedef void (*AvrcpCallback)(AvrcpChannel *chnl, AvrcpCallbackParms *Parms);

/* End of AvrcpCallback */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvrcpCallbackParms structure
 *
 * Contains information for the application callback event.
 */
struct _AvrcpCallbackParms {
    /* AVRCP event */
    AvrcpEvent      event;

    /* AVRCP channel associated with the event */
    AvrcpChannel   *channel;
#ifdef __BT_AVRCP_V13__
    /* AVCTP's translabel */
    U8 transId;
#endif
    /* Callback parameter object */
    union {
        /* Remote device associated with the event */
        BtRemoteDevice *remDev;

        /* Command frame associated with the event */
        AvrcpCmdFrame  *cmdFrame;

        /* Response frame associated with the event */
        AvrcpRspFrame  *rspFrame;
#ifdef __BT_AVRCP_V13__
        /* queueFrame for TX_ERROR/TX_DONE - Tx */
        AvrcpQueuePacketFrame *queueFrame;
 #endif
#if AVRCP_PANEL_SUBUNIT == XA_ENABLED

        /* Panel indication received during AVRCP_EVENT_PANEL_CNF */
        struct {
            /* Operation to which the remote target responded */
            AvrcpPanelOperation operation;
            
            /* The press state of the key in the command to which
             * the target responded.
             */
            BOOL                press;

            /* Response from the target. May indicate an "extended" response
             * code such as AVRCP_RESPONSE_SKIPPED or AVRCP_RESPONSE_TIMEOUT.
             */ 
            AvrcpResponse       response;
        } panelCnf;

        /* Panel indication received during AVRCP_EVENT_PANEL_PRESS,
         * AVRCP_EVENT_PANEL_HOLD, or AVRCP_EVENT_PANEL_RELEASE
         */
        struct {
            /* Operation corresponding to the key pressed, held, or
             * released. AVRCP will only indicate a new operation
             * when the previous one has been _RELEASE'd.
             */
            AvrcpPanelOperation operation;
        } panelInd;
#endif /* AVRCP_PANEL_SUBUNIT == XA_ENABLED */
    } p;
};

/*---------------------------------------------------------------------------
 * AvrcpChannel structure
 *
 * Defines the RCP channel.
 */
struct _AvrcpChannel {
    /* === Internal use only === */

    /* AVRCP Role */
    AvrcpRole       role;

    /* Application callback function */
    AvrcpCallback   callback;

    /* AVCTP Channel */
    AvctpChannel    chnl;

    /* Connection Manager Handler */
    CmgrHandler     cmgrHandler;

    /* SDP Variables */
    SdpQueryToken   sdpQueryToken;
    EvmTimer          conntimer;  

#ifdef __BT_AVRCP_V13__

	AvrcpQueuePacketFrame *curQueueFrame;

    /* AVCTP channel */
    /* AV13 wait for transmiting Packets */
    ListEntry txPacketList;

    union {
        AvrcpCmdFrame avrcpCmd;
        AvrcpRspFrame avrcpRsp;
    }curTx;

	U8 curTxState; 
    /* AV13 offset of current fragmeneted QueuePacket */
    U16 txOffset;

    /* Command Timeout */
    TimeT txTimeout;
    EvmTimer txTimer;
    BOOL startTimer;
    BOOL txCmdTimeout;
	
	U8 txBuffer[BT_AVRCP_MAX_AV_FRAME_SIZE];      // AV/C packet limitation for sending

	U8 rxBuffer[BT_AVRCP_MAX_AV_FRAME_SIZE]; // av/c max size. Used for assemble avctp packets.///@@@@@
	    /* AV13 sdp search result */
    struct {
        U8 tgValid;               // Has searched remote device's TG sdp record
        U8 tgAvailable;           // TG: remote device has TG sdp record
        U16 tgVersion;             // TG: remote device's TG AVRCP version
        U16 tgFeatureFlag;         // TG: remote device's TG feature bitmask in sdp record
        U16 tgSupportBrowse;       // TG: remote device support browse channel or not
        U16 tgDoneConnect;         // After search remote TG record, try to connect
    }remoteTGSdp;
    struct{
        U8 ctValid;               // Has searched remote device's TG sdp record
        U8 ctAvailable;           // CT: remote device has CT sdp record
        U16 ctVersion;             // CT: remote device's CT AVRCP version
        U16 ctFeatureFlag;         // CT: remote device's CT feature bitmask in sdp record
        U16 ctSupportBrowse;       // CT: remote device support browse channel or not
    }remoteCTSdp;
	
#endif
	
#if AVRCP_PANEL_SUBUNIT == XA_ENABLED
    U8 panelState;
    EvmTimer panelTimer;

    union {
        struct {        
            AvrcpPanelOperation opQueue[AVRCP_SUBUNIT_OP_QUEUE_MAX];
            U8 opRead, opWrite;
            AvrcpCmdFrame cmd;
            U8 data[2];
        } ct;
        
        struct {
            AvrcpPanelOperation curOp;
            AvrcpResponse curRsp;
            AvrcpRspFrame rsp;
            U8 data[5];
        } tg;
    } panel;    
#endif
    
};

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/
#ifdef __BT_AVRCP_V13__
#ifdef __TG_V13__
void AvrcpContinueTimeout(EvmTimer *Timer);
void AvrcpCancelContinueTimer(AvrcpChannel *chnl);
void AvrcpStartContinueTimer(AvrcpChannel *chnl);
U16 AvrcpComposeSingleAVCTPRspFrame(AvrcpChannel *chnl, AvrcpQueuePacketFrame *qPacket, AvrcpRspFrame *avctpRsp, U16 u2DataOffset);

#endif
U16 AvrcpComposeSingleAVCTPCmdFrame(AvrcpChannel *chnl, AvrcpQueuePacketFrame *qPacket, AvrcpCmdFrame *avctpCmd, U16 u2DataOffset);
BtStatus AVRCP_SendQueuePacket(AvrcpChannel *chnl, AvrcpQueuePacketFrame *packetFrame);
U16 AvrcpGetSinglePacketMtu(AvctpChannel *chnl, U8 type);

#endif


/*---------------------------------------------------------------------------
 * AVRCP_Init()
 *
 *     Initialize the AVRCP SDK.  This function should only be called
 *     once, normally at initialization time.  The calling of this function
 *     can be specified in overide.h using the XA_LOAD_LIST macro
 *     (i.e. #define XA_LOAD_LIST XA_MODULE(AVRCP) ... ).
 *
 * Returns:
 *     TRUE - Initialization was successful
 *
 *     FALSE - Initialization failed.
 */
BOOL AVRCP_Init(void);

/*---------------------------------------------------------------------------
 * AVRCP_Register()
 *
 *      Registers an application callback to receive AVRCP events. This 
 *      function must be called before any other AVRCP functions.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 *      callback - Identifies the application function that will be called
 *        with AVRCP events.
 *
 *      role - Channel role (CT or TG.)
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVRCP application callback Function was
 *      successfully registered.
 *
 *      BT_STATUS_IN_USE - The specified channel is already in use.
 *
 *      BT_STATUS_INVALID_PARM - The chnl or Callback parameter does not contain 
 *         a valid pointer. (XA_ERROR_CHECK only).
 */
BtStatus AVRCP_Register(AvrcpChannel *chnl, AvrcpCallback callback, AvrcpRole role);

/*---------------------------------------------------------------------------
 * AVRCP_Deregister()
 *
 *      De-registers the AVRCP callback. After making this call
 *      successfully, the callback specified in AVRCP_Register will
 *      receive no further events.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVRCP callback was successfully deregistered.
 * 
 *      BT_STATUS_IN_USE - The specified channel is still in use.
 *
 *      BT_STATUS_NOT_FOUND - An AVRCP callback was not previously registered.
 *     
 *      BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 */
BtStatus AVRCP_Deregister(AvrcpChannel *chnl);

/*---------------------------------------------------------------------------
 * AVRCP_Connect()
 * 
 *     Initiates a connection to a remote AVRCP device.  This function is 
 *     used to establish the lower layer connection (L2CAP), which allows
 *     sending messages.
 *
 *     If the connection attempt is successful, the AVRCP_EVENT_CONNECT event
 *     will be received.  If the connection attempt is unsuccessful, the
 *     AVRCP_EVENT_DISCONNECT event will be received.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 *      addr - The Bluetooth address of the device to which the connection 
 *             should be made. If 0, the connection manager is used
 *             to select an appropriate device.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The connection process has been successfully
 *         started. When the connection process is complete, the
 *         application callback will receive either the AVRCP_EVENT_CONNECT or 
 *         AVRCP_EVENT_DISCONNECT event.
 *
 *     BT_STATUS_IN_USE - This channel is already connected or is in the
 *         process of connecting.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device
 *         selector database.  The device must be discovered, paired, or added
 *         manually using DS_AddDevice();
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).
 */
BtStatus AVRCP_Connect(AvrcpChannel *chnl, BD_ADDR *addr);

/*---------------------------------------------------------------------------
 * AVRCP_Connect()
 * 
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 *      Accept - accept connect indication or not.
 *
 * Returns:
 *
 */
BtStatus AVRCP_ConnectRsp(AvrcpChannel *chnl, BOOL Accept);


/*---------------------------------------------------------------------------
 * AVRCP_Disconnect()
 *
 *     Terminates a connection with a remote AVRCP device.  The lower layer
 *     connection (L2CAP) is disconnected.
 *
 * Parameters:
 *
 *     chnl - A registered and open AVRCP channel.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The disconnect process has been successfully
 *         started. When the disconnect process is complete, the
 *         application callback will receive the AVRCP_EVENT_DISCONNECT event.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on the specified channel.
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device
 *         selector database.  The device must be discovered, paired, or added
 *         manually using DS_AddDevice();
 *
 *     It is possible to receive other error codes, depending on the lower 
 *     layer service in use (L2CAP or Management Entity).
 */
BtStatus AVRCP_Disconnect(AvrcpChannel *chnl);

/*---------------------------------------------------------------------------
 * AVRCP_SendCommand()
 *
 *     Sends an RCP command on the specified channel. The channel must be
 *     registered as a controller (SC_AV_REMOTE_CONTROL), connected, in
 *     the open state, and without another command outstanding.
 *     The "cmdFrame" parameter must be set with valid RCP command parameters.
 *
 * Parameters:
 *     chnl - A registered and open AVRCP channel.
 *
 *     cmdFrame - An AvrcpCmdFrame structure initialized with valid
 *         RCP command parameters.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The send command operation has been started
 *         successfully. When the associated packet has been sent,
 *         the application callback will receive the AVRCP_EVENT_TX_DONE
 *         event.
 *
 *     BT_STATUS_BUSY - A command is already outstanding.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer, or the channel is not registered as a controller
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.
 */
BtStatus AVRCP_SendCommand(AvrcpChannel *chnl, AvrcpCmdFrame *cmdFrame);

/*---------------------------------------------------------------------------
 * AVRCP_SendResponse()
 *
 *     Sends an RCP command on the specified channel. The channel must be
 *     connected and in the open state. The "cmdFrame" parameter must be
 *     set with valid RCP command parameters.
 *
 * Parameters:
 *     chnl - A registered and open AVRCP channel.
 *
 *     rspFrame - An AvrcpRspFrame structure initialized with valid
 *         RCP response parameters.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The send response operation has been started
 *         successfully. When the associated packet has been sent,
 *         the application callback will receive the AVRCP_EVENT_TX_DONE
 *         event.
 *
 *     BT_STATUS_FAILED - There is no outstanding command to respond to.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.
 */
BtStatus AVRCP_SendResponse(AvrcpChannel *chnl, AvrcpRspFrame *rspFrame);

#if AVRCP_PANEL_SUBUNIT == XA_ENABLED
/*---------------------------------------------------------------------------
 * AVRCP_SetPanelKey()
 *
 *     Indicates the state of the key corresponding to the specified
 *     panel subunit operation. Successive calls to this function will
 *     queue up key events to be delivered to the target.
 *
 * Requires:
 *     AVRCP_PANEL_SUBUNIT set to XA_ENABLED
 * 
 * Parameters:
 *     chnl - A registered and open AVRCP channel.
 *
 *     op - Panel operation code to send. If a previous call indicated a
 *         different "op" as pressed, calling this function with a new
 *         "op" will automatically release it.
 *
 *     press - TRUE indicates the key corresponding to operation was pressed,
 *         FALSE indicates the key was released. For FALSE, if the "op"
 *         specified was not already pressed, this call signals a
 *         single press-and-release of the key.
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation was started successfully.
 *         AVRCP_EVENT_PANEL_CNF message(s) will be sent corresponding
 *         to the target's responses to the press, release, or hold messages
 *         brought about by this command.
 *
 *     BT_STATUS_NO_RESOURCES - The internal keystroke buffer is full.
 *
 *     BT_STATUS_FAILED - The channel is not connected or some other error
 *         occured.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 */
BtStatus AVRCP_SetPanelKey(AvrcpChannel *chnl, AvrcpPanelOperation op,
                           BOOL press);

/*---------------------------------------------------------------------------
 * AVRCP_RejectPanelOperation()
 *
 *     Rejects a panel operation received from the connected
 *     controller. This function must be called during
 *     notification of the AVRCP_EVENT_PANEL_IND event. Further notifications
 *     corresponding to the operation (AVRCP_PS_HOLD, AVRCP_PS_RELEASE)
 *     will not be received.
 *
 *     If this function is not called during the AVRCP_EVENT_PANEL_IND
 *     the operation is automatically "ACCEPTED".
 *
 * Requires:
 *     AVRCP_PANEL_SUBUNIT set to XA_ENABLED
 * 
 * Parameters:
 *     chnl - A registered and open AVRCP channel.
 *
 *     rsp - Response code. Must be one of AVRCP_RESPONSE_NOT_IMPLEMENTED
 *         or AVRCP_RESPONSE_REJECTED. Note that REJECTED is only used
 *         when the operation is already reserved by another controller.
 *         This response code does NOT indicate the result of the
 *         operation itself (for example, a PLAY command when no media
 *         is available should be accepted, but simply have no effect).
 *
 * Returns:
 *     None
 */
void AVRCP_RejectPanelOperation(AvrcpChannel *chnl, AvrcpResponse rsp);
#define AVRCP_RejectPanelOperation(c,r) (c)->panel.tg.curRsp = (r)

#endif /* AVRCP_PANEL_SUBUNIT == XA_ENABLED */


BtStatus AvrcpCreateAclLink(BD_ADDR *Addr, CmgrHandler *Handler, CmgrCallback Callback);

#endif /* __AVRCP_H_ */
