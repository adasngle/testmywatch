#ifndef __ME_ADP_H
#define __ME_ADP_H

/****************************************************************************
 *
 * File:
 *     $Workfile:me.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:203$
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
#include "bttypes.h"
#include "xatypes.h"
#include "mesco_adp.h"
#include "utils.h"
#include "bt_common_config.h"
#include "config.h"
#include "eventmgr.h"

/*---------------------------------------------------------------------------
 * Management Entity (ME) layer
 *
 *     The Management Entity allows applications to inquire for
 *     remote devices, establish different types of connections, and
 *     control the access to this device by other devices.
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BT_INQ_TIME_GAP100 constant
 *     Time value specified by Generic Access Profile for the length
 *     of an Inquiry process.
 */
#define BT_INQ_TIME_GAP100 8

/*---------------------------------------------------------------------------
 * BT_MAX_REM_DEV_NAME constant
 *
 *     Maximum length of a Remote Device name (friendly name) allowed by 
 *     Bluetooth. This does not include the null termination at the end.
 */
#define BT_MAX_REM_DEV_NAME 248

/*---------------------------------------------------------------------------
 *
 * The following are used to indicate the use of default values for Window
 * and interval used in inquiry and page scanning. The default values are
 * the default values of the radio module being used. These values should
 * within the range defined by the Generic Access Profile.
 */
#define BT_DEFAULT_SCAN_INTERVAL 0x0000
#define BT_DEFAULT_SCAN_WINDOW   0x0000

/*---------------------------------------------------------------------------
 * BT_INVALID_RSSI constant
 *
 *     Indicates an invalid RSSI value (used when RSSI is not reported
 *     by the controller).
 */
#define BT_INVALID_RSSI   127

#define BT_SSP_IO_CAPABILITY_DISPLAY_ONLY 0x00
#define BT_SSP_IO_CAPABILITY_DISPLAY_YESNO 0x01
#define BT_SSP_IO_CAPABILITY_KEYBORAD_ONLY 0x02
#define BT_SSP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT 0x03

#define BT_SSP_OOB_DATA_PRESENT_NO    0x00
#define BT_SSP_OOB_DATA_PRESENT_YES    0x01

#define BT_SSP_MITM_NOT_REQUIRED_NO_BONDING     (0x00)
#define BT_SSP_MITM_REQUIRED_NO_BONDING         (0x01)
#define BT_SSP_MITM_NOT_REQUIRED_DEDICATED_BONDING     (0x02)
#define BT_SSP_MITM_REQUIRED_DEDICATED_BONDING         (0x03)
#define BT_SSP_MITM_NOT_REQUIRED_GENERAL_BONDING     (0x04)
#define BT_SSP_MITM_REQUIRED_GENERAL_BONDING         (0x05)

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BtEventType type
 *
 *     All indications and confirmations are sent through a callback
 *     function. The event types are defined below.
 */
typedef U8 BtEventType;

/*
 * Group: Global events sent to all handlers. These can be masked out if
 * desired.
 */

/** A remote device is found. "p.inqResult" is valid.  */
#define BTEVENT_INQUIRY_RESULT       1

/** The Inquiry process is completed. "errCode" is valid. */
#define BTEVENT_INQUIRY_COMPLETE     2

/** The Inquiry process is canceled. */
#define BTEVENT_INQUIRY_CANCELED     3

/** An incoming ACL connection. "p.remDev" is valid. */
#define BTEVENT_LINK_CONNECT_IND     4

/** An incoming SCO connection. "p.scoConnect" is valid. */
#define BTEVENT_SCO_CONNECT_IND      5

/** An ACL connection was disconnected. "p.remDev" and
 *  "errCode" are valid. Check "errCode" for completion status */
#define BTEVENT_LINK_DISCONNECT      6

/* An outgoing ACL connection is up. "p.remDev" is valid. */
#define BTEVENT_LINK_CONNECT_CNF     7

/** A restricted connection is up. "p.remDev" is valid. */
#define BTEVENT_LINK_CON_RESTRICT    8

/** Indicates the mode of the link changed. "p.modeChange" is valid */
#define BTEVENT_MODE_CHANGE          9

/** Indicates that an accessibility change is complete. "errCode" and
 *  "p.aMode" are valid. */
#define BTEVENT_ACCESSIBLE_CHANGE   10

/** Indicates that a link has been authenticated. "p.remDev" is valid. */
#define BTEVENT_AUTHENTICATED       11

/** Encryption settings have changed. "p.encrypt" is valid. */
#define BTEVENT_ENCRYPTION_CHANGE   12

/** Security mode has changed. "p.secMode" is valid. */
#define BTEVENT_SECURITY_CHANGE     13

/** Indicates that a role change has occurred */
#define BTEVENT_ROLE_CHANGE         14

/** SCO link has been disconnected. "p.scoConnect" is valid. */
#define BTEVENT_SCO_DISCONNECT      15

/** An outgoing SCO link has been established. "p.scoConnect" is valid. */
#define BTEVENT_SCO_CONNECT_CNF     16

/** Indicates that an ACL connection has received an internal data transmit
 *  request while it is in hold, park or sniff mode. The data will still be 
 *  passed to the radio in park and sniff modes. However, hold mode will
 *  block data transmit. It may be necessary to return the ACL to active
 *  mode to restore normal data transfer. "p.remDev" is valid.
 */
#define BTEVENT_ACL_DATA_NOT_ACTIVE 99

/* Group: The following events are global events but cannot be masked */

/** Indicates that the HCI failed to initialize. This implies that the 
 *  Bluetooth radio is having problems or a radio may not exist. 
 *  Events will be generated with an appropriate error code for any outstanding
 *  operations.  All pending operations that use pointers to memory supplied by 
 *  applications will have a corresponding event generated indicating the 
 *  failure of the operation. Applications should not modify memory passed as 
 *  part of operations until an event signaling the end of the operation
 *  has been received. The HCI and the radio will be reset. If the reset is 
 *  successful then the BTEVENT_HCI_INITIALIZED event will be sent to all 
 *  registered handlers.
 */
#define BTEVENT_HCI_INIT_ERROR      100

/** Indicates that the HCI initialization is successful. The ME will now
 *  accept commands again if an initialization error or fatal error
 *  has occurred. This event is sent whenever HCI is successfully initialized,
 *  including when the stack is first started.
 */
#define BTEVENT_HCI_INITIALIZED     101

/** Indicates that a fatal error has occurred in the radio or the HCI transport.
 *  The HCI and the radio will be reset. If the reset is successful then the 
 *  BTEVENT_HCI_INITIALIZED event will be sent to all registered handlers.
 */
#define BTEVENT_HCI_FATAL_ERROR     102

/** Indicates that the HCI has been deinitialized.  This can happen as the 
 *  result of a call to ME_RadioShutdown, or if some fatal error was reported
 *  by the radio or HCI transport. For any pending operations, events will 
 *  be received with proper error codes prior to receiving this event. 
 *  All pending commands that used memory supplied by the application will 
 *  result in an event, so the application should not modify that memory until 
 *  the event has been received. If the HCI was deinitialized as the result of
 *  a radio or transport error, the HCI will be reset. If the reset is 
 *  successful then the BTEVENT_HCI_INITIALIZED event will be sent to all 
 *  registered handlers.
 */
#define BTEVENT_HCI_DEINITIALIZED   103

/** Indicates that the HCI cannot be initialized. This happens when
 *  the HCI has been reset BT_HCI_NUM_INIT_RETRIES without success or
 *  if some unrecoverable error occurs.
 */
#define BTEVENT_HCI_FAILED          104

/** Indicates that an HCI command has been sent to the radio. This
 *  event is intended for global handlers that would like to be informed
 *  of when certain radio processes are started. "p.hciCmd" is valid.
 */
#define BTEVENT_HCI_COMMAND_SENT    105

/* An outgoing create link has been canceled. "p.remDev" is valid. */
#define BTEVENT_LINK_CREATE_CANCEL  106

/*
 * Group: These events are sent only to the handler that initiated the
 * operation.
 */

/** Indicates the name of a remote device or cancellation of a name request.
 *  "p.meToken" is valid.
 */
#define BTEVENT_NAME_RESULT         17

/** Incoming SCO data has been received. Fields in "p.scoDataInd" are valid
 *  and describe the data received. This data is valid only for the duration
 *  of the callback.
 */
#define BTEVENT_SCO_DATA_IND        18

/** Outgoing SCO data has been sent and the packet is free for re-use by
 *  the application. "p.scoPacketHandled.scoPacket" and
 *  "p.scoPacketHandled.scoCon" are valid.
 */
#define BTEVENT_SCO_DATA_CNF        19

/** Incoming connection request. "p.remDev" is valid. */
#define BTEVENT_LINK_CONNECT_REQ    20

/** Incoming link accept complete. "p.remDev" is valid. */
#define BTEVENT_LINK_ACCEPT_RSP     21

/** Incoming link reject complete. "p.remDev" is valid. */
#define BTEVENT_LINK_REJECT_RSP     22

/** General command (async or sync) has completed. "p.meToken" is valid */
#define BTEVENT_COMMAND_COMPLETE    23

/** Incoming connection request. "p.scoConnect" is valid. The field
 *  "p.scoConnect.scoLinkType" indicates the SCO link type (BLT_SCO or
 *  BLT_ESCO.) When the link is of type BLT_ESCO, the fields
 *  p.scoConnect.scoTxParms and p.scoConnect.scoRxParms point to the
 *  associated eSCO connection parameters.
 */
#define BTEVENT_SCO_CONNECT_REQ     24

/** Set Audio/Voice settings complete.  */
#define BTEVENT_SCO_VSET_COMPLETE   25

/** SCO link connection process started. */
#define BTEVENT_SCO_STARTED         26

/** Select Device operation complete, "p.select" is valid. */
#define BTEVENT_DEVICE_SELECTED     27

/** The eSCO connection has changed. "p.scoConnect" is valid.
 *  The structure "p.scoConnect.scoRxParms" contains the updated
 *  connection parameters.
 */
#define BTEVENT_SCO_CONN_CHNG       28

/* Group: Security-related events. */

/** Indicates access request is successful. "p.secToken" is valid. */
#define BTEVENT_ACCESS_APPROVED     40

/** Indicates access request failed. "p.secToken" is valid. */
#define BTEVENT_ACCESS_DENIED       41

/** Request authorization when "errCode" is BEC_NO_ERROR.
 *  "p.remDev" is valid.
 */
#define BTEVENT_AUTHORIZATION_REQ   42

/** Request a Pin for pairing when "errCode" is BEC_NO_ERROR.
 *  "p.remDev" is valid.
 */
#define BTEVENT_PIN_REQ             43

/** Pairing operation is complete. "p.remDev" is valid. */
#define BTEVENT_PAIRING_COMPLETE    44

/** Authentication operation complete. "p.remDev" is valid. */
#define BTEVENT_AUTHENTICATE_CNF    45

/** Encryption operation complete. "p.remDev" is valid. */
#define BTEVENT_ENCRYPT_COMPLETE    46

/** Security mode 3 operation complete. "p.secMode" is valid. */
#define BTEVENT_SECURITY3_COMPLETE  47

/** A link key is returned. "p.bdLinkKey" is valid.  */
#define BTEVENT_RETURN_LINK_KEYS    48

#define BTEVENT_LOCAL_NAME_CHANGE      201

#define BTEVENT_HCI_EVENTMASK_CHANGE  202

/*
 * Group: SDP Query related events. In all cases, the "p.token" parameter
 * is valid.
 */

/** SDP query completed successfully. */
#define SDEVENT_QUERY_RSP           50

/** SDP query completed with an error */
#define SDEVENT_QUERY_ERR           51

/** SDP query failed */
#define SDEVENT_QUERY_FAILED        52

/*
 * Group: Device Selection monitor events. These events are only sent to
 * the device selection handler. 
 */

/** Request device selection, "p.select" is valid. The device selection
 *  monitor must call DS_SelectDeviceResult() to return the provided
 *  selection request when the selection process is complete.
 */
#define BTEVENT_SELECT_DEVICE_REQ   60

/** Device added to the device database, "p.device" is valid. Note that
 *  during a device inquiry, BTEVENT_DEVICE_ADDED event may not mirror
 *  BTEVENT_INQUIRY_RESULT events. This is because duplicate devices are
 *  not added to the device database and because the device database may
 *  reach capacity, preventing the addition of new devices.
 */
#define BTEVENT_DEVICE_ADDED        61

/** Device removed from the device database, "p.device" is valid. */
#define BTEVENT_DEVICE_DELETED      62

#define BTEVENT_READ_RSSI_COMPLETED      63

#define BTEVENT_IO_CAPABILITY_REQUEST      64
#define BTEVENT_IO_CAPABILITY_RESPONSE      65
#define BTEVENT_USER_CONFIRM_REQUEST          66
#define BTEVENT_PASSKEY_NOTIFICATION          67
#define BTEVENT_KEYPRESS_EVENT          68

#define BTEVENT_SIMPLE_PAIRING_COMPLTETE      69

/* 
 * Me reports this event to bm to check if all link has been disconnected in power off state 
 * Param is invalid. Don't modify the param.
 */

#define BTEVENT_CHECK_ALL_LINK_POWER_OFF      70

/* End of BtEventType */

/*---------------------------------------------------------------------------
 * BtEventMask type
 *
 *     All registered handlers can receive global events. Each handler has
 *     the ability to mask all or some of the events. A handler receives only
 *     events with the mask set. They can be ORed together.
 */
typedef U16 BtEventMask;

#define BEM_NO_EVENTS           0x0000
#define BEM_INQUIRY_RESULT      0x0001
#define BEM_INQUIRY_COMPLETE    0x0002
#define BEM_INQUIRY_CANCELED    0x0004
#define BEM_LINK_CONNECT_IND    0x0008
#define BEM_SCO_CONNECT_IND     0x0010
#define BEM_LINK_DISCONNECT     0x0020
#define BEM_LINK_CONNECT_CNF    0x0040
#define BEM_LINK_CON_RESTRICT   0x0080
#define BEM_MODE_CHANGE         0x0100
#define BEM_ACCESSIBLE_CHANGE   0x0200
#define BEM_AUTHENTICATED       0x0400
#define BEM_ENCRYPTION_CHANGE   0x0800
#define BEM_SECURITY_CHANGE     0x1000
#define BEM_ROLE_CHANGE         0x2000
#define BEM_SCO_DISCONNECT      0x4000
#define BEM_SCO_CONNECT_CNF     0x8000

#define BEM_ALL_EVENTS          0xffff

/* End of BtEventMask */

/*---------------------------------------------------------------------------
 * BtErrorCode type
 */
typedef U8 BtErrorCode;

#define BEC_NO_ERROR             0x00   /* No error */
#define BEC_UNKNOWN_HCI_CMD      0x01   /* Unknown HCI Command */
#define BEC_NO_CONNECTION        0x02   /* No connection */
#define BEC_HARDWARE_FAILURE     0x03   /* Hardware Failure */
#define BEC_PAGE_TIMEOUT         0x04   /* Page timeout */
#define BEC_AUTHENTICATE_FAILURE 0x05   /* Authentication failure */
#define BEC_MISSING_KEY          0x06   /* Missing key */
#define BEC_MEMORY_FULL          0x07   /* Memory full */
#define BEC_CONNECTION_TIMEOUT   0x08   /* Connection timeout */
#define BEC_MAX_CONNECTIONS      0x09   /* Max number of connections */
#define BEC_MAX_SCO_CONNECTIONS  0x0a   /* Max number of SCO connections to a device */
#define BEC_ACL_ALREADY_EXISTS   0x0b   /* The ACL connection already exists. */
#define BEC_COMMAND_DISALLOWED   0x0c   /* Command disallowed */
#define BEC_LIMITED_RESOURCE     0x0d   /* Host rejected due to limited resources */
#define BEC_SECURITY_ERROR       0x0e   /* Host rejected due to security reasons */
#define BEC_PERSONAL_DEVICE      0x0f   /* Host rejected (remote is personal device) */
#define BEC_HOST_TIMEOUT         0x10   /* Host timeout */
#define BEC_UNSUPPORTED_FEATURE  0x11   /* Unsupported feature or parameter value */
#define BEC_INVALID_HCI_PARM     0x12   /* Invalid HCI command parameters */
#define BEC_USER_TERMINATED      0x13   /* Other end terminated (user) */
#define BEC_LOW_RESOURCES        0x14   /* Other end terminated (low resources) */
#define BEC_POWER_OFF            0x15   /* Other end terminated (about to power off) */
#define BEC_LOCAL_TERMINATED     0x16   /* Terminated by local host */
#define BEC_REPEATED_ATTEMPTS    0x17   /* Repeated attempts */
#define BEC_PAIRING_NOT_ALLOWED  0x18   /* Pairing not allowed */
#define BEC_UNKNOWN_LMP_PDU      0x19   /* Unknown LMP PDU */
#define BEC_UNSUPPORTED_REMOTE   0x1a   /* Unsupported Remote Feature */
#define BEC_SCO_OFFSET_REJECT    0x1b   /* SCO Offset Rejected */
#define BEC_SCO_INTERVAL_REJECT  0x1c   /* SCO Interval Rejected */
#define BEC_SCO_AIR_MODE_REJECT  0x1d   /* SCO Air Mode Rejected */
#define BEC_INVALID_LMP_PARM     0x1e   /* Invalid LMP Parameters */
#define BEC_UNSPECIFIED_ERR      0x1f   /* Unspecified Error */
#define BEC_UNSUPPORTED_LMP_PARM 0x20   /* Unsupported LMP Parameter Value */
#define BEC_ROLE_CHG_NOT_ALLOWED 0x21   /* Role Change Not Allowed */
#define BEC_LMP_RESPONSE_TIMEOUT 0x22   /* LMP Response Timeout */
#define BEC_LMP_TRANS_COLLISION  0x23   /* LMP Error Transaction Collision */
#define BEC_LMP_PDU_NOT_ALLOWED  0x24   /* LMP PDU Not Allowed */
#define BEC_ENCRYP_MODE_NOT_ACC  0x25   /* Encryption Mode Not Acceptable */
#define BEC_UNIT_KEY_USED        0x26   /* Unit Key Used */
#define BEC_QOS_NOT_SUPPORTED    0x27   /* QoS is Not Supported */
#define BEC_INSTANT_PASSED       0x28   /* Instant Passed */
#define BEC_PAIR_UNITKEY_NO_SUPP 0x29   /* Pairing with Unit Key Not Supported */
#define BEC_NOT_FOUND            0xf1   /* Item not found */
#define BEC_REQUEST_CANCELLED    0xf2   /* Pending request cancelled */

/*
 * Group: The following error codes are used when the
 * SDEVENT_QUERY_FAILED event is sent.
 */
#define BEC_INVALID_SDP_PDU      0x01   /* SDP response PDU is invalid */
#define BEC_SDP_DISCONNECT       0x02   /* The SDP L2CAP channel or link disconnected */
#define BEC_SDP_NO_RESOURCES     0x03   /* Not enough L2CAP resources */
#define BEC_SDP_INTERNAL_ERR     0x04   /* Some type of internal stack error */

/*
 * Group: The following error code is used when the
 * BTEVENT_PAIRING_COMPLETE event is sent.
 */
#define BEC_STORE_LINK_KEY_ERR   0xf0

/* End of BtErrorCode */

/*---------------------------------------------------------------------------
 * BtIac type
 *     Bluetooth Inquiry Access Codes are used to specify which types of
 *     devices should respond to inquiries. Currently there are only two
 *     defined.
 */
typedef U32 BtIac;

#define BT_IAC_GIAC 0x9E8B33    /* General/Unlimited Inquiry Access Code */
#define BT_IAC_LIAC 0x9E8B00    /* Limited Dedicated Inquiry Access Code */

/* End of BtIac */

/*---------------------------------------------------------------------------
 * BtLinkType type
 *
 *     Bluetooth Link type indicates the link type in the
 *     BtScoConnect link structure for SCO commands and operations.
 */
typedef U8 BtLinkType;

#define BLT_SCO   0x00
#define BLT_ACL   0x01
#define BLT_ESCO  0x02

/* End of BtLinkType */

#define BT_LINK_KEY_GAP_TYPE_NO_KEY             0x00
#define BT_LINK_KEY_GAP_TYPE_COMBINATION_NONE16 0x01
#define BT_LINK_KEY_GAP_TYPE_COMBINATION_16     0x02
#define BT_LINK_KEY_GAP_TYPE_UNAUTHENTICATED    0x03
#define BT_LINK_KEY_GAP_TYPE_AUTHENTICATED      0x04

/*---------------------------------------------------------------------------
 * BtAclPacketType type
 *     Bluetooth ACL Packet types are the different packet types allowed on
 *     an ACL link. They can be ORed together.
 */
typedef U16 BtAclPacketType;

#define BAPT_DM1   0x0008
#define BAPT_DH1   0x0010
#define BAPT_AUX1  0x0200
#define BAPT_DM3   0x0400
#define BAPT_DH3   0x0800
#define BAPT_DM5   0x4000
#define BAPT_DH5   0x8000

/*
 * The set of packet types for a reliable link that follows the Serial port
 * profile.
 */
#define BAPT_STANDARD1 (BAPT_DM1 | BAPT_DH1 | BAPT_DH3 | BAPT_DH5)

#define BAPT_STANDARD (BAPT_DM1 | BAPT_DH1 | BAPT_DM3 | BAPT_DH3 |BAPT_DM5 |   BAPT_DH5)

#define BT_LINK_SUPERVISION_TIMEOUT (0x6400)

#define BT_AUTOMATICALLY_FLUSHED_TIMEOUT (0x640)

#define BT_LINK_RETRANSMISSION_TXPACKET_NO (0x08)

/* End of BtAclPacketType */

/*---------------------------------------------------------------------------
 * BtQosServiceType type
 */
typedef U8 BtQosServiceType;

#define BQST_NO_TRAFFIC  0x00
#define BQST_BEST_EFFORT 0x01
#define BQST_GUARANTEED  0x02

/* End of BtQosServiceType */

/*---------------------------------------------------------------------------
 * BtLinkPolicy type
 * 
 *     Bluetooth Link Policy specifies which Link modes the Link Manager
 *     can use for a given link. Multiple policies can be enabled by
 *     performing a bit wise OR operation.
 */
typedef U16 BtLinkPolicy;

#define BLP_DISABLE_ALL         0x0000
#define BLP_MASTER_SLAVE_SWITCH 0x0001
#define BLP_HOLD_MODE           0x0002
#define BLP_SNIFF_MODE          0x0004
#define BLP_PARK_MODE           0x0008
#define BLP_SCATTER_MODE        0x0010

typedef enum
{
    BT_OP_DISABLE,
    BT_OP_ENABLE,
    BT_OP_DONT_CARE    
} BtEnableOption;

/* Mask must be updated if new policy values are added */
#define BLP_MASK                0xfff0  /* Disables ScatterNet bit */
#define BLP_SCATTER_MASK        0xffe0  /* Enables ScatterNet bit */

/* End of BtLinkPolicy */

/*---------------------------------------------------------------------------
 * BtAccessibleMode type
 *
 *     Bluetooth Accessibility mode includes Discoverable and connectable
 *     modes.
 */
typedef U8 BtAccessibleMode;

#define BAM_NOT_ACCESSIBLE     0x00     /* Non-discoverable or connectable */
#define BAM_GENERAL_ACCESSIBLE 0x03     /* General discoverable and connectable */
#define BAM_LIMITED_ACCESSIBLE 0x13     /* Limited discoverable and connectable */
#define BAM_CONNECTABLE_ONLY   0x02     /* Connectable but not discoverable */
#define BAM_DISCOVERABLE_ONLY  0x01     /* Discoverable but not connectable */

/* End of BtAccessibleMode */

/*---------------------------------------------------------------------------
 * BtLinkMode type
 */
typedef U8 BtLinkMode;

#define BLM_ACTIVE_MODE     0x00
#define BLM_HOLD_MODE       0x01
#define BLM_SNIFF_MODE      0x02
#define BLM_PARK_MODE       0x03
#define BLM_SCATTER_MODE    0x04

/* End of BtLinkMode */

/*---------------------------------------------------------------------------
 * BtConnectionRole type
 *
 *     Specifies the role of a ACL connection
 */
typedef U8 BtConnectionRole;

#define BCR_MASTER   0x00
#define BCR_SLAVE    0x01
#define BCR_UNKNOWN  0x03

/* === Internal use only === */
#define BCR_PSLAVE   0x04
#define BCR_PMASTER  0x05

/* End of BtConnectionRole */

/*---------------------------------------------------------------------------
 * BtStackState type
 *
 *     State of the Bluetooth protocol stack and radio.
 */
typedef U8 BtStackState;

/* The stack has completed initialization of the radio hardware. */
#define BTSS_NOT_INITIALIZED 0

/* The stack is initialized. */
#define BTSS_INITIALIZED     1

/* The stack has encountered an error while initializing the radio hardware. */
#define BTSS_INITIALIZE_ERR  2

/* The stack is deinitializing. */
#define BTSS_DEINITIALIZE    3

/* End of BtStackState */

/* Forward declaration of the callback parameters */
typedef struct _BtEvent BtEvent;

/* Forward declaration of the SCO connect structures */
typedef struct _BtScoConnect BtScoConnect;
typedef struct _BtScoTxParms BtScoTxParms;
typedef struct _BtScoRxParms BtScoRxParms;

/*---------------------------------------------------------------------------
 * BtCallBack type
 *
 *     A function of this type is called to indicate general Bluetooth and
 *     ME events.
 */
typedef void (*BtCallBack) (const BtEvent *);

/* End of BtCallBack */

/*---------------------------------------------------------------------------
 * BtProtocolId type
 *
 *     The protocol ID is a pointer to the protocol's security callback
 *     function. The user of the protocol ID should not use the protocol ID
 *     as a callback function since in the future it may be something else.
 *     The callback is a unique way to identify a protocol layer.   
 */
typedef BtCallBack BtProtocolId;

/* End of BtProtocolId */

/*---------------------------------------------------------------------------
 * BtClassOfDevice type
 *
 *     Bit pattern representing the class of device along with the 
 *     supported services. There can be more than one supported service.
 *     Service classes can be ORed together. The Device Class is composed
 *     of a major device class plus a minor device class. ORing together
 *     each service class plus one major device class plus one minor device
 *     class creates the class of device value. The minor device class is
 *     interpreted in the context of the major device class.
 */

typedef U32 BtClassOfDevice;

/* Group: Major Service Classes. Can be ORed together */
#define COD_LIMITED_DISCOVERABLE_MODE 0x00002000
#define COD_POSITIONING               0x00010000
#define COD_NETWORKING                0x00020000
#define COD_RENDERING                 0x00040000
#define COD_CAPTURING                 0x00080000
#define COD_OBJECT_TRANSFER           0x00100000
#define COD_AUDIO                     0x00200000
#define COD_TELEPHONY                 0x00400000
#define COD_INFORMATION               0x00800000

/* Group: Major Device Classes (Select one) */
#define COD_MAJOR_MISCELLANEOUS       0x00000000
#define COD_MAJOR_COMPUTER            0x00000100
#define COD_MAJOR_PHONE               0x00000200
#define COD_MAJOR_LAN_ACCESS_POINT    0x00000300
#define COD_MAJOR_AUDIO               0x00000400
#define COD_MAJOR_PERIPHERAL          0x00000500
#define COD_MAJOR_IMAGING             0x00000600
#define COD_MAJOR_UNCLASSIFIED        0x00001F00

/* Group: Minor Device Class - Computer Major class */
#define COD_MINOR_COMP_UNCLASSIFIED   0x00000000
#define COD_MINOR_COMP_DESKTOP        0x00000004
#define COD_MINOR_COMP_SERVER         0x00000008
#define COD_MINOR_COMP_LAPTOP         0x0000000C
#define COD_MINOR_COMP_HANDHELD       0x00000010
#define COD_MINOR_COMP_PALM           0x00000014
#define COD_MINOR_COMP_WEARABLE       0x00000018

/* Group: Minor Device Class - Phone Major class */
#define COD_MINOR_PHONE_UNCLASSIFIED  0x00000000
#define COD_MINOR_PHONE_CELLULAR      0x00000004
#define COD_MINOR_PHONE_CORDLESS      0x00000008
#define COD_MINOR_PHONE_SMART         0x0000000C
#define COD_MINOR_PHONE_MODEM         0x00000010
#define COD_MINOR_PHONE_ISDN          0x00000014

/* Group: Minor Device Class - LAN Access Point Major class */
#define COD_MINOR_LAN_0               0x00000000        /* fully available */
#define COD_MINOR_LAN_17              0x00000020        /* 1-17% utilized */
#define COD_MINOR_LAN_33              0x00000040        /* 17-33% utilized */
#define COD_MINOR_LAN_50              0x00000060        /* 33-50% utilized */
#define COD_MINOR_LAN_67              0x00000080        /* 50-67% utilized */
#define COD_MINOR_LAN_83              0x000000A0        /* 67-83% utilized */
#define COD_MINOR_LAN_99              0x000000C0        /* 83-99% utilized */
#define COD_MINOR_LAN_NO_SERVICE      0x000000E0        /* 100% utilized */

/* Group: Minor Device Class - Audio Major class */
#define COD_MINOR_AUDIO_UNCLASSIFIED  0x00000000
#define COD_MINOR_AUDIO_HEADSET       0x00000004
#define COD_MINOR_AUDIO_HANDSFREE     0x00000008
#define COD_MINOR_AUDIO_MICROPHONE    0x00000010
#define COD_MINOR_AUDIO_LOUDSPEAKER   0x00000014
#define COD_MINOR_AUDIO_HEADPHONES    0x00000018
#define COD_MINOR_AUDIO_PORTABLEAUDIO 0x0000001C
#define COD_MINOR_AUDIO_CARAUDIO      0x00000020
#define COD_MINOR_AUDIO_SETTOPBOX     0x00000024
#define COD_MINOR_AUDIO_HIFIAUDIO     0x00000028
#define COD_MINOR_AUDIO_VCR           0x0000002C
#define COD_MINOR_AUDIO_VIDEOCAMERA   0x00000030
#define COD_MINOR_AUDIO_CAMCORDER     0x00000034
#define COD_MINOR_AUDIO_VIDEOMONITOR  0x00000038
#define COD_MINOR_AUDIO_VIDEOSPEAKER  0x0000003C
#define COD_MINOR_AUDIO_CONFERENCING  0x00000040
#define COD_MINOR_AUDIO_GAMING        0x00000048

/* Group: Minor Device Class - Peripheral Major class */
#define COD_MINOR_PERIPH_KEYBOARD     0x00000040
#define COD_MINOR_PERIPH_POINTING     0x00000080
#define COD_MINOR_PERIPH_COMBOKEY     0x000000C0
/* Group: Minor Device Class - ORed with Peripheral Minor Device class */
#define COD_MINOR_PERIPH_UNCLASSIFIED 0x00000000
#define COD_MINOR_PERIPH_JOYSTICK     0x00000004
#define COD_MINOR_PERIPH_GAMEPAD      0x00000008
#define COD_MINOR_PERIPH_REMOTECTRL   0x0000000C
#define COD_MINOR_PERIPH_SENSING      0x00000010
#define COD_MINOR_PERIPH_DIGITIZER    0x00000014
#define COD_MINOR_PERIPH_CARD_RDR     0x00000018

/* Group: Minor Device Class - Imaging Major class */
#define COD_MINOR_IMAGE_UNCLASSIFIED  0x00000000
#define COD_MINOR_IMAGE_DISPLAY       0x00000010
#define COD_MINOR_IMAGE_CAMERA        0x00000020
#define COD_MINOR_IMAGE_SCANNER       0x00000040
#define COD_MINOR_IMAGE_PRINTER       0x00000080

/* Group: Masks used to isolate the class of device components */
#define COD_SERVICE_MASK              0x00ffC000        /* Less LIAC bit */
#define COD_MAJOR_MASK                0x00001F00
#define COD_MINOR_MASK                0x000000FC
#define COD_LIMITED_DISC_MASK         0x00002000        /* LIAC bit */

/* End of BtClassOfDevice */

/*---------------------------------------------------------------------------
 * BtLinkKeyType type
 */

typedef U8 BtLinkKeyType;

#define COMBINATION_KEY 0x00
#define LOCAL_UNIT_KEY  0x01
#define REMOTE_UNIT_KEY 0x02

/* End of BtLinkKeyType */

#if defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__)
#define MAX_EIR_SDP_SIZE 30
#endif 

/*---------------------------------------------------------------------------
 * BtRemDevState type
 *
 *     Indicates the current state of a Remote Device.
 */
typedef U8 BtRemDevState;

#define BDS_DISCONNECTED  0x00
#define BDS_OUT_CON       0x01  /* Starting an out going connection */
#define BDS_IN_CON        0x02  /* In process of incoming connection */
#define BDS_CONNECTED     0x03  /* Connected */
#define BDS_OUT_DISC      0x04  /* Starting an out going disconnect */
#define BDS_OUT_DISC2     0x05  /* Disconnect status received */
#define BDS_OUT_CON2      0x06  /* In SCO, used when connection request has
                                   been sent */

/* End of BtRemDevState */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BtPageScanInfo structure
 */
typedef struct _BtPageScanInfo
{
    U8 psRepMode;
    U8 psMode;
    U16 clockOffset;
} BtPageScanInfo;

/*---------------------------------------------------------------------------
 * BtSniffInfo structure
 *
 *     Identifies the sniff requirements during a ME_StartSniff request.
 */

typedef struct _BtSniffInfo
{
    /*
     * Maximum acceptable interval between each consecutive sniff period.
     * May be any even number between 0x0002 and 0xFFFE, but the mandatory
     * sniff interval range for controllers is between 0x0006 and 0x0540.
     * The value is expressed in 0.625 ms increments (0x0006 = 3.75 ms).
     *
     * The actual interval selected by the radio will be returned in
     * a BTEVENT_MODE_CHANGE event.
     */
    U16 maxInterval;

    /*
     * Minimum acceptable interval between each consecutive sniff period.
     * Must an even number between 0x0002 and 0xFFFE and be less than
     * "maxInterval". Like maxInterval this value is expressed in
     * 0.625 ms increments.
     */
    U16 minInterval;

    /*
     * The number of master-to-slave transmission slots during which
     * a device should listen for traffic (sniff attempt).
     * Expressed in 0.625 ms increments. May be between 0x0001 and 0x7FFF.
     */
    U16 attempt;

    /*
     * The amount of time before a sniff timeout occurs. Expressed in
     * 1.25 ms increments. May be between 0x0000 and 0x7FFF, but the mandatory
     * range for controllers is 0x0000 to 0x0028.
     */
    U16 timeout;

} BtSniffInfo;

typedef enum
{
    BT_QOS_NO_TRAFFIC,
    BT_QOS_BEST_EFFORT,
    BT_QOS_GUARANTEED
} BtQoSServiceType;

/*---------------------------------------------------------------------------
 * BtQoSInfo structure
 *
 *     Identifies the QoS requirements during a ME_SetUpQoS request.
 */

typedef struct _BtQoSInfo
{
    BtConnectionRole role;
    U8 flags; /* RFU */
    BtQoSServiceType serviceType;
    U32 tokenRate;
    U32 peakBandwidth;
    U32 latency;
    U32 delayVar;
} BtQoSInfo;


/*---------------------------------------------------------------------------
 * BtAccessModeInfo structure
 */
typedef struct _BtAccessModeInfo
{
    U16 inqInterval;    /* Inquiry scan interval */
    U16 inqWindow;      /* Inquiry scan Window */
    U16 pageInterval;   /* Page scan interval */
    U16 pageWindow;     /* Page scan window */
} BtAccessModeInfo;

/*---------------------------------------------------------------------------
 * BtInquiryResult structure
 *
 *     Represents information about a single device found during an
 *     Inquiry process.
 */
typedef struct _BtInquiryResult
{
    BD_ADDR bdAddr;     /* Device Address */
    BtPageScanInfo psi; /* Page scan info used for connecting */
    U8 psPeriodMode;
    BtClassOfDevice classOfDevice;
    S8 rssi;

    /*
     * RSSI in dBm (-127 to +20). Only valid when controller reports RSSI with
     * in inquiry results. Otherwise it will be set to BT_INVALID_RSSI.
     */
#if defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__)
    U8 supported_eir;
    U8 *eirResponse;
#endif /* defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__) */ 
} BtInquiryResult;

/*---------------------------------------------------------------------------
 * BtChannelClass structure
 *
 *     Indicates channel classification for AFH with 79 1-bit fields in
 *     a 10-byte array. Each channel may be marked bad or unknown. The nth
 *     field (0 - 78) indicates the value for channel n. Bit 79 is unused.
 */
typedef struct _BtChannelClass
{
    U8 map[10];
} BtChannelClass;

/*---------------------------------------------------------------------------
 * BtHandler structure
 */
typedef struct _BtHandler
{
    ListEntry node;         /* For internal stack use only. */
    BtCallBack callback;    /* Pointer to callback function */
    BtEventMask eMask;      /* For internal stack use only. */

} BtHandler;

/* Internal types required for BtRemoteDevice structure */
typedef U8 BtAuthState;
typedef U8 BtEncryptState;
typedef U8 BtAuthorizeState;
typedef U8 BtSecAccessState;
typedef U8 BtLinkRxState;
typedef U8 BtOpType;
typedef U8 BtIoCapabilityState;

typedef struct _BtOperation
{
    ListEntry node;
    BtOpType opType;
} BtOperation;

/*---------------------------------------------------------------------------
 * BtRemoteDevice structure
 *
 *     Represents a remote device. This structure is used to manage the ACL
 *     and SCO links to a remote device.
 */
typedef struct _BtRemoteDevice
{
    BtOperation op;         /* Used internally by the stack */
    BtOperation policyOp;   /* Used internally by the stack */
    BtOperation cancelOp;   /* Used internally by the stack */
    BtOperation roleOp;     /* Used internally by the stack */
    BtOperation packetTypeOp;
    BtOperation remoteFeatureOp;
    BtOperation remoteVersionOp;
    BtOperation supervisionOp;
#if defined (__BT_2_1_AUTO_FLUSH__)
    BtOperation automaticallyFlushedOp;
#endif 
#if defined (__BT_2_1_SNIFF_SUBRATING__)
    BtOperation sniffSubrateOp;
#endif 

    
#if (SPP_SCO_SUPPORT == XA_ENABLED) || (BT_READ_RSSI == XA_ENABLED)
    BtOperation readRSSIOp;
#endif 
    BtOperation readWIFIRSSIOp;
    BtOperation qosOp;
    /*
     * Group: Access functions are provided for accessing these values. They
     * should not be accessed directly by an application. 
     */
    BD_ADDR bdAddr;             /* 48-bit address of the remote device */
    BtConnectionRole role;      /* The local device's role in this connection */
    BtLinkMode mode;            /* Current mode of the device */
    BtClassOfDevice cod;        /* COD of the device */
    void *secRec;               /* Current Security Record being processed */
/* [MAUI_02323312] mtk01705: Remove this compile option __BT_2_1_SIMPLE_PAIRING__ to allow SIMAP 16-digits check. */
//#if !defined (__BT_2_1_SIMPLE_PAIRING__)
    U8 pin_code_len;
    BOOL simap_issue_authentication_procedure;
//#endif /* !defined (__BT_2_1_SIMPLE_PAIRING__) */ 
    /* Group: ME uses the following fields internally. */
    ListEntry handlers;         /* List of bound handlers */
    ListEntry scoList;          /* Head of list of SCO connections */
    ListEntry secTokens;        /* List of security tokens */
    BtHandler *scoHandler;      /* Handler for SCO event (1 per remDev) */
    BtHandler *authHandler;     /* Handler for authentication */
    BtHandler *encryptHandler;  /* Handler for encryption */
    U16 hciHandle;              /* Connection handle for HCI */
    U16 LinkPolicySetting;
    U16 LinkPolicyParms;
    BtRemDevState state;        /* State of the ACL link */
    U8 remoteFeature[8];
    U8 remoteVersion[5];
    U8 restricted;              /* TRUE indicates restricted link */
    U8 discFlag;                /* TRUE indicates disconnect request */
    U8 discReason;              /* Non-zero = disconnect reason */
    U8 discTimeoutLevel;
#if defined (__BT_2_1_SNIFF_SUBRATING__)
    U8 sniffSubrateState;
    U16 sniffSubrateMaxLatency;
    U16 sniffSubrateMinRemoteTimeout;
    U16 sniffSubrateMinLocalTimeout;
#endif /* defined (__BT_2_1_SNIFF_SUBRATING__) */ 

#if defined (__BT_2_1_SUPERVISION_TIMEOUT_EVENT__)
    U16 supervisionTimeout;
#endif 
    EvmTimer connectTimer;
    EvmTimer discTimer;
    EvmTimer readyToSendTimer;
    EvmTimer sdapTimer;
    EvmTimer changePacketTimer;
    EvmTimer disconnectACLCheckCreatACLTimer;
#ifdef __ENABLE_NUM_COMPLETED_PKT_EVENT_TIMER__    
    /* Add monitorNumCompletedPktEventTimer for MAUI_01482962 error handling */
    EvmTimer monitorNumCompletedPktEventTimer;
#endif
    /* MAUI_01498320 by mtk01411 */
    EvmTimer monitorPairResultTimer;
    U8 disconnectACLCheckCreatACLCount;
    U8 disconnectACLCheckCreatACLAlwaysForce;
    /* Group: The following are used for security */
    BtSecAccessState secAccessState;
    BtAuthState authState;
    BtAuthorizeState authorizeState;
    BtEncryptState encryptState;
    U8 pairingType; /* Uses the BtPairingType type */
    /* Special use for MT6601 */
    U8 switchState;
    U8 oldEncryptionMode;
    U8 roleSwitchCounter;
    U8 sniffCounter;
    U8 connectCounter;
    /* Group: The following are parameters to me operations */
    union
    {
        /* Create connection operation */
        struct
        {
            BtPageScanInfo psi;
            U8 allowRoleChange;
        } con;

        /* Park & Hold mode commands */
        struct
        {
            U16 max;
            U16 min;
        } mode;

        BtSniffInfo sniff;              /* Sniff command */
        BtConnectionRole acceptRole;    /* Connection accept */

        U8 rejectReason;        /* Connection reject */
        BtLinkPolicy policy;    /* Write link policy */
    } parms;

    /* Group: L2CAP uses the following fields internally. */
    BOOL okToSend;
    ListEntry txHighPriorityQueue;
    ListEntry txLowPriorityQueue;
    U32 lastHighPriorityTxTime;
    U32 lastLowPriorityTxTime;
    U32 lastTxRxTime;
    U8 refCount;
    BtLinkRxState rxState;
    U16 rxBufferLen;
    U8 *rxBuffer;
#if L2CAP_FLOW_CONTROL == XA_ENABLED
    ListEntry txPacketList;
    BtPacket txPackets[BT_LINK_RETRANSMISSION_TXPACKET_NO];
    U8 linkModes;   /* L2capLinkMode defined in l2cap.h */
    ListEntry txNonAckQueue;
#endif /* L2CAP_FLOW_CONTROL == XA_ENABLED */ 

    /* Group: The SDP client uses the following fields internally. */

    U16 sdpClientChannel;       /* L2CAP channel for SDP client */
    ListEntry queryList;        /* List of queries */
    BtPacket sdpClientPacket;   /* Packet for sending queries */
    U16 transId;                /* Current transaction ID */
    U8 sdpClientState;          /* State of the client */
    U16 flushTimeOut;
#if defined (__BT_2_1_SIMPLE_PAIRING__)
    BtIoCapabilityState ioCapabilitiyState;
    U8 gapKeyType;
    U8 gapSecurityLevel;
#endif /* defined (__BT_2_1_SIMPLE_PAIRING__) */ 
#if (SPP_SCO_SUPPORT == XA_ENABLED) || (BT_READ_RSSI == XA_ENABLED)
    U8 rssi;
#endif 

    struct _SdpQueryToken *curToken;    /* The token currently being processed. */

    /* Group: The SDP server uses the following field internally. */
    struct _SdpServerInfo *sdpServInfo;
    U8 scOutoConnectState;
    U8 scoParms;
    U8 supportESCO;
    BtQoSInfo qos;
    U32 linkEstablishTime;
} BtRemoteDevice;

/*---------------------------------------------------------------------------
 * BtRestrictedInfo structure
 *
 *     Used to create a restricted link to a remote device. A restricted link
 *     has a specific Quality of service, link policy, and hold policy. 
 */
typedef struct _BtRestrictedInfo
{
    BtAclPacketType aclPacketType;
    BtQosServiceType serviceType;

    /*
     * Range of flush time out is 0x0000 - 0x0EFF. 0xFFFF is used
     * to indicate infinite - no automatic flush. Flush time out is
     * the amount of time to wait until a transmit frame is flushed.
     * Times are specified in 0.625 us units.
     */
    U16 minRemoteFlushTo;
    U16 minLocalFlushTo;

    U32 tokenRate;              /* Token rate in bytes per second */
    U32 peakBandwidth;          /* Peak bandwidth in bytes per second */
    U32 latency;                /* Latency in microseconds */
    U32 delayVariation;         /* Delay variation in microseconds */
    BtLinkPolicy linkPolicy;    /* Link policy */

} BtRestrictedInfo;

/*---------------------------------------------------------------------------
 * BtDeviceRecord structure
 *
 *     Represents a Bluetooth Device Database record as seen by the
 *     Security manager. 
 */
typedef struct _BtDeviceRecord
{
    BD_ADDR bdAddr;
    BOOL trusted;
    U8 linkKey[16];
/* [MAUI_02323312] mtk01705: Remove this compile option __BT_2_1_SIMPLE_PAIRING__ to allow SIMAP 16-digits check. */
//#if !defined (__BT_2_1_SIMPLE_PAIRING__)
    U8 pin_code_len;
    BOOL paired_with_simap;
//#endif /* !defined (__BT_2_1_SIMPLE_PAIRING__) */ 
    BtLinkKeyType keyType;
#if defined (__BT_2_1_SIMPLE_PAIRING__)
    U8 gapKeyType;
#endif 
} BtDeviceRecord;

typedef struct _BtCodRecord
{
    BD_ADDR bdAddr;
    U32 cod;
} BtCodRecord;

/*---------------------------------------------------------------------------
 * BtDeviceLinkKey structure
 *
 *     Represents a Bluetooth Device link key returned from an HCI
 *     ReadStoredLinkKey command.
 */
typedef struct _BtDeviceLinkKey
{
    BD_ADDR bdAddr; /* Device Address */
    U8 linkKey[16];
} BtDeviceLinkKey;

/*---------------------------------------------------------------------------
 * MeRemoteNameRequest structure
 *
 *     Used in an MeCommandToken when handling a "Remote Name Request"
 *     operation.
 */
typedef struct _MeRemoteNameRequest
{
    /* Common for both input and output */
    BD_ADDR bdAddr;

    /*
     * Input/Output parameters for the operation. Either the "in"
     * or "out" parameters are valid, depending on when this structure
     * is accessed.
     */
    union
    {

        /* Fields to fill before starting the operation. */
        struct
        {
            BtPageScanInfo psi; /* Page scan information */
        } in;

        /* Fields to read when the operation is complete. */
        struct
        {

            /* Length of string stored in "name". */
            U8 len;

            /* Remote device name in UTF-8 format */
            U8 *name;
        } out;

    } io;

} MeRemoteNameRequest;

/*---------------------------------------------------------------------------
 * MeGeneralHciCommand structure
 *
 *     Used in an MeCommandToken when handling general and user defined
 *     HCI commands. 
 */
typedef struct _MeGeneralHciCommand
{
    /* Fields to be filled before starting the operation */
    struct
    {

        /*
         * The hciCommand is composed of the OGF and the OCF. The OGF
         * is the upper 6 bits while the OCF is the remaining 12 bits.
         */
        U16 hciCommand; /* HCI command (see hci.h for commands) */
        U8 parmLen;     /* Length of the command parameters */
        U8 *parms;      /* Pointer to command parameters */
        U8 event;       /* Main event which signals end of operation */
    } in;

    /* Fields to read when the operation is complete */
    struct
    {
        /*
         * The status must be checked to see if the remaining out parameters
         * valid. If the status is BT_STATUS_SUCCESS then the rest of the
         * out parameters is valid.
         */
        BtStatus status;

        U8 event;   /* Event ending operation (see hci.h for event) */
        U8 parmLen; /* Length of event parameters */
        U8 *parms;  /* Pointer to event parameters */
    } out;
} MeGeneralHciCommand;

typedef U16 BtScoAudioSettings; /* This must match definition in mesco.h */

/*---------------------------------------------------------------------------
 * MeCommandToken structure
 *
 *     A token that represents a special HCI command, including
 *     user-defined HCI commands.
 */
typedef struct _MeCommandToken
{
    BtOperation op;         /* For internal use by the stack. */
    BtOperation cancelOp;   /* For internal use by the stack. */
    BtRemoteDevice *remDev; /* For internal use by the stack. */
    EvmTimer tokenTimer;
    /*
     * The caller must set the callback function. It is called
     * when the command is complete.
     */
    BtCallBack callback;

    /*
     * One of the fields in this union is used to store information
     * about the operation.
     */
    union
    {

        /* Information about a "Remote Name Request" operation */
        MeRemoteNameRequest name;

        /* Information for the SCO Audio settings command */
        BtScoAudioSettings vsettings;

        /* Information used in general and user defined operations */
        MeGeneralHciCommand general;

    } p;

    /* Group: The following field is for internal stack use only */
    BtEventType eType;
    U8 flags;
} MeCommandToken;

/*---------------------------------------------------------------------------
 * BtEvent structure
 *
 *     Used to pass information from the stack to the application. Not all
 *     fields are valid at any given time. The type of event determines which
 *     fields are valid.
 */
struct _BtEvent
{
    BtEventType eType;      /* Event causing callback */
    BtErrorCode errCode;    /* Error code */
    BtHandler *handler;     /* Pointer to handler */

    U8 *raw_data;
    /* Parameters */
    union
    {
        BtInquiryResult inqResult;  /* Inquiry result */
        BtDeviceLinkKey bdLinkKey;  /* Returned Link Key */

        BtAccessibleMode aMode;     /* New access mode */
        BtRemoteDevice *remDev;     /* Pointer to remote device */
        BD_ADDR bdAddr;             /* Device Address */
        MeCommandToken *meToken;    /* Me command token */

        struct _BtSecurityToken *secToken;  /* Security Token */
        struct _SdpQueryToken *token;

        /* Result for BTEVENT_ENCRYPTION_CHANGE event. */
        struct
        {
            BtRemoteDevice *remDev;
            U8 mode;    /* New encryption mode (uses the
                           BtEncryptMode type) */
        } encrypt;

        /* Result for the BTEVENT_SCO_DATA_CNF event */

        struct
        {
            BtRemoteDevice *remDev; /* Pointer to remote device */
            BtScoConnect *scoCon;   /* SCO connection */
            BtPacket *scoPacket;    /* SCO Packet Handled */
        } scoPacketHandled;

        /*
         * Result for the BTEVENT_SCO_CONNECT_CNF && BTEVENT_SCO_CONNECT_IND 
         * events.
         */
        struct
        {
            BtRemoteDevice *remDev;     /* Pointer to remote device */
            U16 scoHandle;              /* SCO Connection handle for HCI */
            BtScoConnect *scoCon;       /* SCO connection */
            BtLinkType scoLinkType;     /* SCO link type */
            BtScoTxParms *scoTxParms;   /* Pointer to eSCO TX parameters */
            BtScoRxParms *scoRxParms;   /* Pointer to eSCO RX parameters */
        } scoConnect;

        /* Result for the BTEVENT_SCO_DATA_IND event */
        struct
        {
            BtRemoteDevice *remDev; /* Pointer to remote device */
            U16 scoHandle;          /* SCO Connection handle for HCI */
            BtScoConnect *scoCon;   /* SCO connection. */
            U8 len;                 /* SCO data len */
            U8 *ptr;                /* SCO data ptr */
        } scoDataInd;

        /*
         * Result for the BTEVENT_SECURITY_CHANGE and
         * BTEVENT_SECURITY3_COMPLETE events
         */
        struct
        {
            U8 mode;    /* New security mode (uses the BtSecurityMode
                           type). */
            BOOL encrypt;   /* Indicate if encryption set or not */
        } secMode;

        /* Results for the BTEVENT_MODE_CHANGE event */
        struct
        {
            BtRemoteDevice *remDev;
            BtLinkMode curMode;
            U16 interval;
        } modeChange;

        /* Results for BTEVENT_ROLE_CHANGE */
        struct
        {
            BtRemoteDevice *remDev;
            BtConnectionRole newRole;   /* New role */
        } roleChange;

        /* Informational BTEVENT_HCI_COMMAND_SENT event. */
        struct
        {
            /* "HciCommandType" values found in sys/hci.h. */
            U16 type;

            /* Length of optional parameters */
            U8 length;

            /*
             * Optional parameters to HCI command. Refer to Bluetooth HCI
             * specification for command parameter descriptions.
             */
            const U8 *parms;
        } hciCmd;
        struct
        {
            BtRemoteDevice *remDev;
            U8 RSSI;    /* New encryption mode (uses the
                           BtEncryptMode type) */
        } rssi;
        /*
         * Results for BTEVENT_DEVICE_SELECTED event. When received by
         * the Device Selection monitor, it represents the selection query.
         */
        struct _BtSelectDeviceToken *select;

        /* BTEVENT_DEVICE_ADDED and BTEVENT_DEVICE_DELETED events. */
        struct _BtDeviceContext *device;
    #if defined (__BT_2_1_SIMPLE_PAIRING__)
        struct
        {
            BtRemoteDevice *remDev;
            U8 ioCapability;
            U8 OOBDataPresent;
            U8 authRequirements;
        } ioCapabilityResponse;
        struct
        {
            BtRemoteDevice *remDev;
            U32 numericValue;
        } userConfirmRequest;
        struct
        {
            BtRemoteDevice *remDev;
            U32 numericValue;
        } passKeyEntry;
        struct
        {
            BtRemoteDevice *remDev;
            U8 passkeyEvent;
        } passKeyPress;

    #endif /* defined (__BT_2_1_SIMPLE_PAIRING__) */ 
    } p;
};

#if defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__)
typedef struct _BtEIRSdpRecord
{
    U8 used;
    U8 counter;
    U16 service_class;
} BtEIRSdpRecord;
#endif /* defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__) */ 

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * BtScoTxParms structure
 */
struct _BtScoTxParms
{
    U32 transmitBandwidth;
    U32 receiveBandwidth;
    U16 maxLatency;
    U16 voiceSetting;
    U8 retransmissionEffort;
    BtEscoPktType eScoPktType;

};

/*---------------------------------------------------------------------------
 * BtScoRxParms structure
 */
struct _BtScoRxParms
{
    U8 scoTransmissionInterval;
    U8 scoRetransmissionWindow;
    U16 scoRxPacketLen;
    U16 scoTxPacketLen;

};

/*---------------------------------------------------------------------------
 * BtScoConnect structure
 *
 *     Represents an SCO connection. An SCO connection must be added to an
 *     exiting ACL connection. 
 */
struct _BtScoConnect
{
    /* Group: The following fields are used internally by ME. */
    BtOperation scoop;      /* This must be the first item in the structure */
    ListEntry node;
    BtRemDevState scostate; /* State of the SCO link */
    U16 scoHciHandle;       /* SCO Connection handle for HCI */
#if BT_SCO_HCI_DATA == XA_ENABLED
    ListEntry scoTxQueue;
#endif     
    BtRemoteDevice *scord;  /* Link to ACL Remote Device structure */

    /* Group: The following are parameters to me operations */
    /* SCO Link type. See BtScoLinkType defines. */
    BtLinkType scoLinkType;
    /* Create SCO connection operation. See BtScoPacketType defines. */
    BtScoPacketType scopt;
    /* SCO Connection reject. */
    U8 scoRejectReason;

    /* Group: The following fields are BT v1.2 SCO parameters. */
    BtScoTxParms scoTxParms;
    BtScoRxParms scoRxParms;
};

#endif /* __ME_ADP_H */
