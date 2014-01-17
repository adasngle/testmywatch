#ifndef __L2CAP_ADP_H
#define __L2CAP_ADP_H

/****************************************************************************
 *
 * File:
 *     $Workfile:l2cap.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:84$
 *
 * Description:
 *     Public types, defines, and prototypes for accessing the
 *     upper layer of the Logical Link Control and Adaptation Protocol.
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

#include "bttypes.h"
#include "me_adp.h"
#include "btconfig.h"
#include "xatypes.h"
#include "config.h"
#include "utils.h"

/*---------------------------------------------------------------------------
 * Logical Link Control and Adaptation Protocol (L2CAP) layer
 *
 *     The L2CAP layer handles all Bluetooth data communications that
 *     take place over ACL links. It is primarily a protocol multiplexer,
 *     allowing protocols such as RFCOMM, TCS Binary, and SDP to share
 *     a single device-to-device connection. The L2CAP layer also allows
 *     a higher layer protocol to communicate with several other devices
 *     over separate "channels".
 *     
 *     L2CAP supports segmentation and reassembly, which is the process
 *     of breaking down transmit packets into an appropriate size for
 *     HCI, and reassembling received packets for higher layers. The
 *     Maximum Transmission Unit (MTU) represents the largest possible
 *     reassembled L2CAP packet.
 *
 *     Finally, L2CAP handles channel configuration. This includes settings
 *     for the default L2CAP payload size, timeouts for missing data,
 *     and "quality of service" requirements for data rate, peak bandwidth,
 *     latency, and delay.
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * L2CAP_DEFAULT_MTU constant
 *
 *     Defines the MTU size assumed for devices that do not specify an MTU
 *     size explicitly. This value is defined by the Bluetooth
 *     specification as 672 bytes.
 */
#define L2CAP_DEFAULT_MTU           0x02A0

/*---------------------------------------------------------------------------
 * L2CAP_MINIMUM_MTU constant
 *
 *     Defines the minimum allowed MTU size for all L2CAP implementations.
 *     This value is defined by the Bluetooth specification as 48 bytes.
 */
#define L2CAP_MINIMUM_MTU           0x0030

/*---------------------------------------------------------------------------
 * L2CAP_MAXIMUM_MTU constant
 *
 *     Defines the maximum receive PDU size allowed for this L2CAP
 *     implementation. This is based on the L2CAP_MTU configuration value.
 *     Note that this value may be less than the L2CAP_DEFAULT_MTU value
 *     to save RAM.
 */
#define L2CAP_MAXIMUM_MTU           L2CAP_MTU

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * L2capEventType type
 *
 *     Indicates the type of an L2CAP callback's event during an
 *     L2capCallback function call. These events are delivered in the
 *     L2capCallbackParms structure's "event" field.
 *
 *     Depending on the event, a specific response may be required or
 *     certain fields in the L2capCallbackParms structure may be valid or
 *     invalid. The valid fields and required responses are described
 *     for each event below.
 */
typedef U8 L2capEventType;

/** A remote device has requested a new L2CAP connection (channel).
 * The L2CAP user must call L2CAP_ConnectRsp to confirm or deny the
 * connection request. 
 */
#define L2EVENT_CONNECT_IND      1

/** The channel is connected and ready to exchange data. This event
 * occurs regardless of which device initiated the connection. 
 */
#define L2EVENT_CONNECTED        2

/** The remote device requested that its channel be disconnected. L2CAP
 * will respond with a disconnection response, and send the DISCONNECTED
 * event when the disconnection process is complete.
 */
#define L2EVENT_DISCON_IND       3

/** The channel is disconnected. The callback's "status" field contains the
 * reason for the disconnection.
 */
#define L2EVENT_DISCONNECTED     4

/** Channel data has been received. The callback "ptrs.data" field contains
 * the received data, and "dataLen" indicates the data length.
 */
#define L2EVENT_DATA_IND         5

/** A data packet has been sent. The application may re-use or deallocate
 * the packet in the "ptrs.packet" field. The status of the transmit is 
 * provided in the "status" field of the callback parameters structure.
 */
#define L2EVENT_PACKET_HANDLED   6

/** A request is complete. Different L2CAP operations may generate
 * a COMPLETE event, depending on configuration options. (See L2CAP_Ping,
 * L2CAP_GetInfo, and L2CAP_ConnectRsp for more information.)
 *
 * During this event, the "status" field contains the results of the
 * request.
 */
#define L2EVENT_COMPLETE         7

/** A Quality-of-Service violation was detected. It is up to the
 * application to take any action based on this event.
 */
#define L2EVENT_QOS_VIOLATION    8

/** The Channel configuration parameters are being negotiated. This event
 * is generated when the channel has auto-configuration disabled and
 * enters the configuration state. It is also generated when a connected
 * channel enters renegotiation as initiated by the remote device. No
 * data can be sent while a channel is negotiating is configuration.
 * Channels which have auto-configuration disabled should send a config
 * request upon receiving this event.
 */
#define L2EVENT_CONFIGURING    9

/** If a channel undergoes configuration renegotiation (as indicated
 * by the L2EVENT_CONFIGURING event) this event will be indicated
 * once the configuration negotiation is complete. This event is roughly
 * equivalent to L2EVENT_CONNECTED, and indicates that data transmission
 * can now be resumed.
 */
#define L2EVENT_RECONFIGURED    10

/*
 * Group: The following events do not occur unless L2CAP_FLEXIBLE_CONFIG
 * is enabled and channel auto-configuration is disabled.
 */
#if L2CAP_FLEXIBLE_CONFIG == XA_ENABLED

/** A configuration request has been received from the remote device.
 * The options and configuration flags are available in the L2capCallbackParms
 * "ConfigInd" structure. 
 *
 * This event is only received for CID's with "auto config" disabled (via
 * L2CAP_SetAutoConfigMode). After this event is received,
 * L2CAP_SendConfigRsp must be called to indicate a configuration response.
 *
 * If a configure request is received with the more
 * (continue) flag set, the response must also specify this flag. The remote
 * side will then send additional request(s) with additional options.
 */
#define L2EVENT_CONFIG_IND      11

/** A configuration response has been received from the remote device.
 * The options and configuration flags are available in the L2capCallbackParms
 * "ConfigCnf" structure. If a configure response is received with the more
 * (continue) flag set, the local side must issue a Configure Request to
 * solicit another response from the remote device. This process continues
 * until a response without the more flag is received.
 */
#define L2EVENT_CONFIG_CNF      12

#endif /* L2CAP_FLEXIBLE_CONFIG == XA_ENABLED */

#define L2EVENT_PING_COMPLETE         13

#define L2EVENT_INFO_COMPLETE         14

#define L2EVENT_CONNECT_AUTORIZED    15

#define L2EVENT_FLOW_DATA_IND       16

/* End of L2capEventType */

/*---------------------------------------------------------------------------
 * L2capDiscReason type
 *
 *     Indicates the reason for a disconnection. The reason is delivered
 *     during an L2EVENT_DISCONNECTED callback, and can be found in the
 *     L2capCallbackParms "status" field.
 */
typedef U16 L2capDiscReason;

/* Disconnection occurred for an unknown reason. */
#define L2DISC_REASON_UNKNOWN      0x0000

/* The local or remote device requested the disconnection. */
#define L2DISC_USER_REQUEST        0x0001

/* An L2CAP request timed out. */
#define L2DISC_REQUEST_TIMEDOUT    0x0002

/* The underlying ACL Link was disconnected. */
#define L2DISC_LINK_DISCON         0x0003

/* The connection was terminated due to a QOS violation. */
#define L2DISC_QOS_VIOLATION       0x0004

/* The local security manager refused the connection attempt. */
#define L2DISC_SECURITY_BLOCK      0x0005

/* The remote device does not support the requested protocol service (PSM). */
#define L2DISC_CONN_PSM_UNSUPP     0x0042

/* The remote device's security architecture denied the connection. */
#define L2DISC_CONN_SECURITY_BLOCK 0x0043

/* The remote device is out of resources. */
#define L2DISC_CONN_NO_RESOURCES   0x0044

/* Configuration failed due to unacceptable parameters. */
#define L2DISC_CONFIG_UNACCEPT     0x0081

/* Configuration was rejected (unknown reason). */
#define L2DISC_CONFIG_REJECT       0x0082

/* Configuration failed due to an unrecognized configuration option. */
#define L2DISC_CONFIG_OPTIONS      0x0083

/* The maximum number of retransmissions has been exceeded (BT1.2 mode only). */
#define L2DISC_MAX_RETRANS         0x0084

/* The connection was terminated due to a protocol violation. */
#define L2DISC_PROTOCOL_ERROR      0x0085

/* End of L2capDiscReason */

/*---------------------------------------------------------------------------
 * L2capConnStatus type
 *
 *     Indicates the status of a requested connection. When the L2CAP
 *     user receives the L2EVENT_CONNECT_IND event, it must call
 *     L2CAP_ConnectRsp to provide one of the status codes below.
 */
typedef U16 L2capConnStatus;

/* The connection request is accepted. */
#define L2CONN_ACCEPTED                     0x0000

/*
 * The connection request is pending. This might be necessary if some
 * user action is required to approve the connection.
 *
 * After the pending indication is delivered to the remote device, the local
 * protocol will receive an L2EVENT_COMPLETE event. After this event occurs
 * the protocol may re-issue the L2CAP_ConnectRsp call with one of the
 * other connection status values.
 */
#define L2CONN_PENDING                      0x0001

/*
 * Rejects the connection request because no such protocol service exists.
 * Generally, this rejection status should only be originated by L2CAP,
 * not by protocol services.
 *
 * The L2EVENT_DISCONNECTED event will be generated after the rejection
 * has been sent.
 */
#define L2CONN_REJECT_PSM_NOT_SUPPORTED     0x0002

/*
 * Rejects the connection request due to a security failure. The
 * L2EVENT_DISCONNECTED event will be generated after the rejection has
 * been sent.
 */
#define L2CONN_REJECT_SECURITY_BLOCK        0x0003

/*
 * Rejects the connection request due to a resource shortage. The
 * L2EVENT_DISCONNECTED event will be generated after the rejection has
 * been sent.
 */
#define L2CONN_REJECT_NO_RESOURCES          0x0004

/* End of L2capConnStatus */

/*---------------------------------------------------------------------------
 * L2capPsmValue type
 *
 *     Defines well-known protocol service values. These values identify
 *     protocol services that use L2CAP. When creating an L2capPsm structure
 *     for registration with L2CAP_RegisterPsm, the "psm" field must be
 *     set to one of these values.
 *
 *     Dynamically assigned PSM values are also permitted; however, they
 *     must be odd, within the range of 0x1001 to 0xFFFF, and have
 *     the 9th bit (0x0100) set to zero.
 */
typedef U16 L2capPsmValue;

/*
 * PSMs of this type cannot receive a connection. Only clients establishing
 * outbound L2CAP connections can use it.
 */
#define BT_CLIENT_ONLY_PSM      0x0000

/* Value for a Service Discovery Protocol server */
#define BT_PSM_SDP              0x0001

/* Value for an RFCOMM server */
#define BT_PSM_RFCOMM           0x0003

/* Value for a TCS Binary server */
#define BT_PSM_TCS              0x0005

/* Value for a TCS Binary group */
#define BT_PSM_TCS_CORDLESS     0x0007

/* Value for the BNEP service */
#define BT_PSM_BNEP             0x000F

/* Value for the HID Control Channel */
#define BT_PSM_HID_CTRL         0x0011

/* Value for the HID Interrupt Channel */
#define BT_PSM_HID_INTR         0x0013

/* Value for the UPnP/ESDP service */
#define BT_PSM_UPNP             0x0015

/* Value for the A/V Control Transport protocol */
#define BT_PSM_AVCTP            0x0017

/* Value for the A/V Distribution Transport protocol */
#define BT_PSM_AVDTP            0x0019

/* Value for Unrestricted Digital Information Control Plane protocol */
#define BT_PSM_UDI_C            0x001D

/*
 * Servers that use dynamically allocated PSMs should register using
 * this value. The L2CAP_RegisterPsm function will assign a dynamic
 * PSM value and return it in the "psm" field.
 */
#define BT_DYNAMIC_PSM          0x1101

#define BT_PSM_TESTING           0x1003

/* End of L2capPsmValue */

/*---------------------------------------------------------------------------
 * L2capInfoType type
 *
 *     Defines the types of information that can be queried from a remote
 *     L2CAP entity with L2CAP_GetInfo.
 */
typedef U16 L2capInfoType;

#define L2INFO_CONNECTIONLESS_MTU   0x0001

#define L2INFO_EXTENDED_FEATURES    0x0002

#define L2INFO_FIXED_CHANNEL_SUPPORTED 0x0003

/* End of L2capInfoType */

/*---------------------------------------------------------------------------
 * L2capConfigFlags type
 * 
 */
typedef U16 L2capConfigFlags;

/* No flags. */
#define L2CFG_FLAG_NONE     0x0000

/* More/Continuation bit in configuration packets. */
#define L2CFG_FLAG_MORE     0x0001

/* End of L2capConfigFlags */

/*---------------------------------------------------------------------------
 * L2capConfigResult type
 * 
 *     Describes the result of configuration negotiation. This type is
 *     indicated during L2EVENT_CONFIG_CNF events. It is also used when
 *     sending a L2CAP Configure Response.
 */
typedef U16 L2capConfigResult;

/* Requested parameters are acceptable */
#define L2CFG_RESULT_ACCEPTED      0x0000

/* Requested parameters are unacceptable */
#define L2CFG_RESULT_UNACCEPTABLE  0x0001

/* Request rejected - unspecified reason */
#define L2CFG_RESULT_REJECTED      0x0002

/* Unknown option(s) were present */
#define L2CFG_RESULT_UNKN_OPTIONS  0x0003

/* End of L2capConfigResult */

/*---------------------------------------------------------------------------
 * L2capConfigOption type
 *
 *     Describes the type of a configuration parameter option. Configuration
 *     parameters are accessible during a L2EVENT_READ_CONFIG_OPTION callback
 *     by calling the L2CAP_ReadOption function.
 *
 *     The buffer returned by L2CAP_ReadOption contains a set of
 *     configuration parameters. Each parameter begins with an
 *     L2capConfigOption type, a one-byte length, and option-specific data.
 *
 */
typedef U8 L2capConfigOption;

/*
 * Option type for the remote device's maximum payload size (maximum
 * transmission unit). The length field will indicate 2 bytes, and be
 * followed by the 16-bit MTU size.
 */
#define L2CFG_OPTION_MTU            0x01

/*
 * Option type for the remote device's flush timeout option. This is the
 * amount of time the remote device will attempt to retransmit before
 * giving up and flushing the data. The length field will indicate 2 bytes,
 * and be followed by the flush timeout value.
 */
#define L2CFG_OPTION_FLUSH_TIMEOUT  0x02

/*
 * Option type for the remote device's quality of service parameters.
 * This option corresponds with the guarantees that the remote device
 * will provide. See the Bluetooth L2CAP specification for more information.
 */
#define L2CFG_OPTION_QOS            0x03

/*
 * Option type for the remote device's retransmission and flow control
 * modes. This option specifies the link modes supported by L2CAP such
 * as flow control or retransmission mode plus related configuration
 * parameters.
 */
#define L2CFG_OPTION_FLOW_CONTROL   0x04

/* End of L2capConfigOption */

/*---------------------------------------------------------------------------
 * L2capQosServiceType type
 *
 *      Indicates the quality of service (QOS) guaranteed by a device.
 *      This type is used when reading a L2CFG_OPTION_QOS configuration
 *      option parameter (see L2capConfigOption).
 */
typedef U8 L2capQosServiceType;

/* No outgoing data will be sent so QOS parameters are unnecessary */
#define L2QOS_OPTION_NO_TRAFFIC       0x00

/*
 * The device cannot guarantee the QOS parameters, but will make its
 * "best effort" to honor them.
 */
#define L2QOS_OPTION_BEST_EFFORT      0x01

/*
 * The device guarantees that it will operate within the QOS parameters
 * provided.
 */
#define L2QOS_OPTION_GUARANTEED       0x02

/* End of L2capQosServiceType */

/*---------------------------------------------------------------------------
 * L2capLinkMode type
 *
 *     Indicates the L2CAP Link modes supported by the channel. This type
 *     is used when reading a L2CFG_OPTION_FLOW_CONTROL configuration option
 *     flags parameter.
 */
typedef U8 L2capLinkMode;

/* Bluetooth v1.1 mode, no L2CAP flow control or retransmissions. */
#define L2MODE_BASIC                0x01

/*
 * Retransmission of L2CAP frames is enabled. In retransmission mode, flow
 * control is utilized on the connection and frames may be retransmitted
 * as part of an error recovery procedure.
 */
#define L2MODE_RETRANSMISSION       0x02

/*
 * Flow control only of L2CAP frames is enabled. In this mode flow control is
 * used, but unacknowledged frames are not retransmitted.
 */
#define L2MODE_FLOW_CONTROL         0x04

/* End of L2capLinkMode */

/*---------------------------------------------------------------------------
 * L2capChannelId type
 *
 *     Identifies a particular channel. Channel ID values are assigned
 *     by L2CAP to uniquely identify a connection to a remote device,
 *     using a specific higher-layer protocol. Channel IDs may also
 *     identify a connectionless group of devices.
 */
typedef U16 L2capChannelId;

/* Used to specify an invalid L2CAP channel ID. */
#define L2CID_INVALID        0x0000

/* End of L2capChannelId */

/*---------------------------------------------------------------------------
 * L2ExtendedFeatures type
 *
 *     Defines the L2CAP Extended Feature Mask values found in an L2CAP 
 *     Information Response. The response value is sent in Little Endian and
 *     may need conversion before being compared with the mask values. If a
 *     bit is set in the response, the corresponding feature is present.
 */
typedef U32 L2ExtendedFeatures;

/* L2CAP Flow Control is supported */
#define L2EXTENDED_FEATURE_FLOW_CONTROL         0x00000001

/* L2CAP Retransmission is supported */
#define L2EXTENDED_FEATURE_RETRANSMISSIONS      0x00000002

/* L2CAP bi-directional QoS is supported */
#define L2EXTENDED_FEATURE_BIDIRECTIONAL_QOS    0x00000004

/* L2CAP UniCast is supported */
#define L2EXTENDED_FEATURE_UNICAST                0x00000200


/* End of L2ExtendedFeatures */



/*---------------------------------------------------------------------------
 * L2FixedChannelSupported type
 *
 *     Defines the L2CAP fix channel supported status.
 */
typedef U32 L2FixChannelSupportedLSB;
typedef U32 L2FixChannelSupportedMSB;

#define L2FIX_CHANNEL_SUPPORTING_SIGNALING          0x00000001
#define L2FIX_CHANNEL_SUPPORTING_CONNECTIONLESS     0x00000002

/* End of L2FixChannelSupported */


/* Forward reference for the L2capCallback type */
typedef struct _L2capCallbackParms L2capCallbackParms;

/*---------------------------------------------------------------------------
 * L2capCallback type
 *
 *     A function of this type is called to indicate L2CAP events to
 *     a protocol service. When L2CAP calls this function, it provides
 *     the channel ID that received the event and additional information
 *     in the "info" parameter.
 *
 *     Protocol services provide the callback function's address to
 *     L2CAP by calling the L2CAP_RegisterPsm function.
 */
typedef void (*L2capCallback) (L2capChannelId cid, L2capCallbackParms *Info);

/* End of L2capCallback */

/*---------------------------------------------------------------------------
 * L2CAP Security Id
 *
 * Security ID for L2CAP access.
 */
#define SEC_L2CAP_ID    (BtProtocolId)L2Cap_SecAccessRsp

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * L2capCallbackParms structure
 *
 *     Provides information to the L2CAP user about an L2capCallback
 *     event. Different fields are valid depending on the type of
 *     the event, indicated by the "event" field. See L2capEventType for
 *     more information about specific callback events.
 *     
 *     The L2CAP user should not modify elements in this structure.
 *     Unless otherwise noted, the callback structure and any data pointers
 *     it provides should be considered invalid after the callback returns.
 */
struct _L2capCallbackParms
{
    /* L2CAP upper layer event. Always valid. */
    L2capEventType event;

    /* 
     * Completion status, based on the event type. Possible types are
     * L2capDiscReason for L2EVENT_DISCONNECTED events or BtStatus for
     * most others. 
     */
    U16 status;

    /* Device that originated the event or data. Always valid. */
    BtRemoteDevice *aclLink;

    union
    {
        /* Pointer to the PSM structure passed in to L2CAP_RegisterPsm */
        const struct _L2capPsm *psm;

        /* Pointer to the group structure passed in to L2CAP_RegisterGroup */
        const struct _L2capGroup *group;
    } owner;

    /* Length of data in "ptrs.data", during L2EVENT_DATA_IND events. */
    U16 dataLen;

    union
    {
        /* 
         * Packet data, valid only during L2EVENT_PACKET_HANDLED callback
         * events. 
         */
        BtPacket *packet;

        /*
         * During a L2EVENT_DATA_IND callback event this points to the data received
         * by the channel or group. 
         * During a L2EVENT_COMPLETE callback event for an L2CAP_GetInfo() call,
         * this points to the requested information.
         * During L2EVENT_COMPLETE events for an L2CAP_Ping() call, this points
         * to any data provided in the echo response.
         */
        U8 *data;

    #if L2CAP_FLEXIBLE_CONFIG == XA_ENABLED
        struct _ConfigInd
        {
            U8 ident;
            L2capConfigFlags flags;
            U8 *options;
            U16 optionsLen;
        } configInd;

        struct _ConfigCnf
        {
            U8 ident;
            L2capConfigFlags flags;
            U16 result;
            U8 *options;
            U16 optionsLen;
        } configCnf;
    #endif /* L2CAP_FLEXIBLE_CONFIG == XA_ENABLED */ 
    } ptrs;
};

/*---------------------------------------------------------------------------
 * L2capPsm structure
 *
 *     Represents a protocol service that uses L2CAP. Any higher-layer
 *     protocol or application that will use L2CAP's connection-oriented
 *     services must allocate and completely fill out this structure.
 *     The structure is then provided to L2CAP by calling L2CAP_RegisterPsm.
 *
 *     For an L2CAP user who will only call L2CAP_Ping and L2CAP_GetInfo,
 *     only the "callback" field is required to be filled out. Connection
 *     oriented services may use L2CAP_Ping and L2CAP_GetInfo as well.
 */
typedef struct _L2capPsm
{
    /* Callback to invoke whenever an event occurs. */
    L2capCallback callback;

    /* Identifier for the protocol service. */
    L2capPsmValue psm;
    BOOL authorizedSetting;

    /*
     * Group: The following parameters specify the connection settings used
     * to configure channels opened on this PSM. L2CAP will automatically
     * negotiate the connection configuration with the remote peer. Channels
     * that require control over their configuration process should use
     * the L2CAP_ConfigReq and L2CAP_ConfigRsp functions to execute their own
     * configuration process.
     */

    /*
     * Maximum receive MTU for this service. Should be equal to or less 
     * than L2CAP_MAXIMUM_MTU. 
     */
    U16 localMtu;

    /*
     * Minimum acceptable value for remote device's receive MTU size. Aka, 
     * local transmit MTU. Should be no less than L2CAP_MINIMUM_MTU. 
     */
    U16 minRemoteMtu;

#if L2CAP_FLOW_CONTROL == XA_ENABLED

    /*
     * Requested operating modes for channels opened for this PSM. For
     * maximum compatiblity, this should include L2MODE_BASIC. If basic is
     * not included, connections to Bluetooth v1.1 devices will fail. An
     * L2CAP Information request may be issued to retrieve the extended
     * features mask to ascertain the remote devices capabilities. If the
     * PSM does not permit use the flow and error control extensions added
     * in Bluetooth v1.2, set this parameter to L2MODE_BASIC.
     *
     * The inbound and outbound link modes can be independently configured.
     * So, for example inbound packets can use retransmission mode while
     * outbound data uses only flow control. However, if the link is
     * configured to use basic mode in one direction, it must use basic
     * mode in the other direction also.
     */

    /* Specify the acceptable link modes for incoming data. */
    L2capLinkMode inLinkMode;

    /* Specify the acceptable link modes for outgoing data. */
    L2capLinkMode outLinkMode;

    /*
     * The maximum number of times the remote L2CAP can transmit a packet
     * before considering the channel lost. The minimum value of 1, indicates
     * no retransmissions shall be sent. Note that if the retransmit timer
     * expires and the transmitMax value has been reached, the channel will
     * be disconnected. This value is not used by basic mode connections.
     */
    U8 inTransmitMax;

    /*
     * The retransmission timeout to be used by the remote device when 
     * retransmission mode is enabled. This timeout should be set fairly
     * high in order to avoid unnecessary retransmissions due to delayed
     * acknowledgements. Suitable values, specified in milliseconds, could
     * be 100’s of milliseconds or higher. This value is not used by basic
     * mode connections.
     */
    U16 inRetransTimeout;

    /*
     * The monitor timeout to be used by the remote device when no activity
     * is present on the channel. It serves to ensure that lost acknowledgements
     * are retransmitted. Suitable values, specified in milliseconds, could
     * be 100’s of milliseconds or higher. This value is not used by basic
     * mode connections.
     */
    U16 inMonitorTimeout;

    /*
     * Number of bytes that can be received from the remote device before
     * an acknowledgement must be sent. This value represents the buffering
     * capability internal to the L2CAP User. The minimum value is Psm.localMtu.
     */
    U32 inBufferSize;

#endif /* L2CAP_FLOW_CONTROL == XA_ENABLED */

} L2capPsm;

/*---------------------------------------------------------------------------
 * L2capConnSettings structure
 *
 *     Provides connection settings to L2CAP during connection establishment.
 */
typedef struct _L2capConnSettings
{
    /*
     * The channel-specific buffer for reassembling received data
     * packets. Channels that utilize either the Reassembly or Flow
     * Control link modes must provide a reassembly buffer. This buffer
     * must be sized to match the localMtu value specified in the PSM
     * structure. L2CAP will bind this buffer to the channel until the
     * connection is terminated. This setting is valid only when
     * L2CAP_FLOW_CONTROL is XA_ENABLED.
     */
    ListEntry *reasmBuffer;
    ListEntry *receivedBuffer;
    U16     rxWindowSize;
    U8      *txMaxTransmitCount;
} L2capConnSettings;

#if L2CAP_NUM_GROUPS > 0
/*---------------------------------------------------------------------------
 * L2capGroup structure
 *
 *     Represents a set of devices in a group. Before calling
 *     L2CAP_RegisterGroup, the L2CAP user must allocate an L2capGroup
 *     structure and fill the "callback", "psm", and "maxMembers" fields.
 *
 *     This structure is only available when the L2CAP_NUM_GROUPS
 *     configuration is greater than 0.
 */
typedef struct _L2capGroup
{
    /* Callback to use for group events. */
    L2capCallback callback;

    /* The PSM value for this group. */
    L2capPsmValue psm;

    /* Maximum number of group members in the "members" field. */
    U8 maxMembers;

    /* 
     * Group Channel ID. This is filled in by L2AP during the 
     * L2CAP_RegisterGroup call. 
     */
    L2capChannelId cid;

    /* === Internal use only === */
    U16 inUse;
    BD_ADDR members[2];

} L2capGroup;

#endif /* L2CAP_NUM_GROUPS > 0 */
#endif /* __L2CAP_ADP_H */