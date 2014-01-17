/****************************************************************************
 *
 * File:
 *     $Workfile:conmgr.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:38$
 *
 * Description: This file contains public definitions for the Connection Manager.
 *             
 * Created:     June 6, 2003
 *
 * Copyright 2003-2005 Extended Systems, Inc.
 * Portions copyright 2005 iAnywhere Solutions, Inc.
 * All rights reserved. All unpublished rights reserved.
 *
 * Unpublished Confidential Information of iAnywhere Solutions, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any means, 
 * or stored in a database or retrieval system, without prior written 
 * permission of iAnywhere Solutions, Inc.
 * 
 * Use of this work is governed by a license granted by iAnywhere Solutions,
 * Inc. This work contains confidential and proprietary information of
 * iAnywhere Solutions, Inc. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/

#ifndef __CONMGR_H_
#define __CONMGR_H_

#include "medev_adp.h"
#include "config.h"
#include "xatypes.h"
#include "bttypes.h"
#include "utils.h"
#include "me_adp.h"
#include "btconfig.h"
#include "eventmgr.h"
#include "osapi.h"
#include "me.h"

#ifndef MTK_SNIFF_TIMER
#define MTK_SNIFF_TIMER 35000
#endif 
#ifndef MTK_SNIFF_MIN_INTERVAL
#define MTK_SNIFF_MIN_INTERVAL 300
#endif 
#ifndef MTK_SNIFF_MAX_INTERVAL
#define MTK_SNIFF_MAX_INTERVAL 800
#endif 
#ifndef MTK_SNIFF_ATTEMPT
#define MTK_SNIFF_ATTEMPT 4
#endif 
#ifndef MTK_SNIFF_TIMEOUT
#define MTK_SNIFF_TIMEOUT 1
#endif 

#if defined (__BT_2_1_SNIFF_SUBRATING__)

#ifndef MTK_SNIFF_SUBRATING_MAX_LATENCY
#define MTK_SNIFF_SUBRATING_MAX_LATENCY 800
#endif 

#ifndef MTK_SNIFF_SUBRATING_MIM_REMOTE_TIMEOUT
#define MTK_SNIFF_SUBRATING_MIM_REMOTE_TIMEOUT 80
#endif 

#ifndef MTK_SNIFF_SUBRATING_MIM_LOCAL_TIMEOUT
#define MTK_SNIFF_SUBRATING_MIM_LOCAL_TIMEOUT 80
#endif 

#define CMGR_SNIFF_SUBRATING_DISABLED 0x00
#define CMGR_SNIFF_SUBRATING_ENABLED 0x01

#endif /* defined (__BT_2_1_SNIFF_SUBRATING__) */ 

/*---------------------------------------------------------------------------
 * Connection Manager API layer
 *
 *     The Connection Manager provides a set of functions for managing ACL and
 *     SCO (and eSCO) links.  Profiles make calls to the Connection Manager when 
 *     they need a resource.  It is up to the Connection Manager to decide how to
 *     best fulfill those requests.  
 *
 *     Adhering to the API described below, it is possible to replace the 
 *     Connection Manager included in any profile.  This may be desirable if
 *     a central connection management layer is required to manage resources
 *     between multiple profiles running on the same device.  This is
 *     highly dependant on system architecture, but the API is provided here
 *     in the event that it proves useful.
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * CMGR_DEFAULT_SNIFF_EXIT_POLICY constant
 *     Defines the default behavior for exiting sniff mode.  The default 
 *     value causes the device to exit sniff mode whenever an audio link
 *     is initiated.
 */
#ifndef CMGR_DEFAULT_SNIFF_EXIT_POLICY
#define CMGR_DEFAULT_SNIFF_EXIT_POLICY CMGR_SNIFF_EXIT_ON_AUDIO
#endif 

/*---------------------------------------------------------------------------
 * CMGR_AUDIO_DEFAULT_PARMS constant
 *     Defines the highest audio connection parameters for an audio link.  See
 *     (CmgrAudioParms) for valid values.  If the defined settings are not 
 *     supported by the radio module, the highest possible settings will 
 *     be used.  For example, Bluetooth 1.1 radios will only support 
 *     CMGR_AUDIO_PARMS_SCO and Bluetooth 1.2 radios will support
 *     CMGR_AUDIO_PARMS_S1 and CMGR_AUDIO_PARMS_SCO.  All parameter types
 *     are supported by Bluetooth 2.0 radios.
 */
#ifndef CMGR_AUDIO_DEFAULT_PARMS
#define CMGR_AUDIO_DEFAULT_PARMS CMGR_AUDIO_PARMS_SCO
#endif 

/*---------------------------------------------------------------------------
 * CMGR_MEMORY_EXTERNAL constant
 *     Memory is allocated externally.  When this constant is defined as
 *     XA_ENABLED, global context memory is not allocated by the Connection
 *     Manager code, and therefore must be allocated external to this module.  
 *     If memory is allocated external to this module, if must be allocated 
 *     using the context variable type defined in conmgr.h, and the entire 
 *     structure must be initialized to 0. In addition, the macro CMGR() must 
 *     be defined so that the context memory can be accessed local to this module.
 */
#ifndef CMGR_MEMORY_EXTERNAL
#define CMGR_MEMORY_EXTERNAL XA_DISABLED
#endif 

/* Link State Flags */

/* === Internal use only === */
#define CMGR_FLAG_AUDIO_UP        0x01
#define CMGR_FLAG_SCO_REGISTERED  0x02
#define CMGR_FLAG_STARTING_SCO    0x04
#define CMGR_FLAG_HANDLE_SCO      0x10
#define CMGR_FLAG_SELECTING       0x20

/* Sniff time values */

/* === Internal use only === */
#define CMGR_SNIFF_DISABLED  (0xFFFFFFFF)
#define CMGR_SNIFF_DONT_CARE  (0)

/* Audio flags */

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * CmgrEvent type
 *
 *     All indications and confirmations are sent through a callback function.
 *     The profile registers with the Connection Manager at initialization.  Calls
 *     are made to request ACL and SCO resources.  The Connection Manager calls
 *     the profile's callback function to indicate the results of those requests.
 */
typedef U8 CmgrEvent;

/** An outgoing ACl connection request has been confirmed.  The 'Status' 
 *  parameter of the callback indicates whether it is successful or unsuccessful.  
 *  The 'CmgrHandler.bdc' field contains a pointer to the device context.
 */
#define CMEVENT_DATA_LINK_CON_CNF     1

/** An incoming ACl connection request has been connected.  The 'Status' 
 *  parameter of the callback indicates whether it is successful or unsuccessful.  
 *  The 'CmgrHandler.remDev' field contains a pointer to the remote device.
 *  All registered handlers receive this event.
 */
#define CMEVENT_DATA_LINK_CON_IND     2

/** The ACL link has been disconnected.  The 'Status' field should indicate
 *  success.  The 'CmgrHandler.bdc' field contains a pointer to the device
 *  context.  The 'CmgrHandler.error' field contains the disconnect reason.
 *  All registered handlers receive this event.
 */
#define CMEVENT_DATA_LINK_DIS         3

/** The SCO connection has been requested from the remote device.
 *  The 'CmgrHandler.bdc' field contains a pointer to the device context.
 *  The application must call CMGR_AcceptAudioLink() to accept the connection.
 *  All registered SCO handlers receive this event.
 */
#define CMEVENT_AUDIO_LINK_REQ        4

/** The SCO connection has come up.  The 'Status' parameter indicates whether
 *  the connection was successful or not.  The 'CmgrHandler.bdc' field contains 
 *  a pointer to the device context.
 */
#define CMEVENT_AUDIO_LINK_CON        5

/** The SCO link has been disconnected.  The 'Status' field should indicate
 *  success.  The 'CmgrHandler.bdc' field contains a pointer to the device
 *  context.  The 'CmgrHandler.error' field contains the disconnect reason.
 */
#define CMEVENT_AUDIO_LINK_DIS        6

/** Audio data has been received on the specified SCO link
 *  The 'CmgrHandler.bdc' field contains a pointer to the device and the
 *  'CmgrHandler.audioData' field contains a pointer to the received data.
 *  All registered SCO handlers receive this event.
 */
#define CMEVENT_AUDIO_DATA            7

/** Audio data has been sent on the specified SCO link
 *  The 'CmgrHandler.bdc' field contains a pointer to the device and the
 *  'CmgrHandler.audioData' field contains a pointer to the packet.
 *  All registered SCO handlers receive this event.
 */
#define CMEVENT_AUDIO_DATA_SENT       8

/** Sniff mode entered.
 */
#define CMEVENT_ENTER_SNIFF_MODE      9

/** Sniff or Park mode exited.
 */
#define CMEVENT_EXIT_SNIFF_PARK_MODE  10

#define CMEVENT_READ_RSSI_COMPLETED  11

/* End of CmgrEvent */

/*---------------------------------------------------------------------------
 * CmgrSniffExitPolicy type
 *
 * Defines the policy for exiting sniff mode.  While it is possible to both
 * send data a create SCO connections in sniff mode, it may be desired to
 * exit sniff mode for performances or compatibility reasons.
 */
typedef U8 CmgrSniffExitPolicy;

/** Exit sniff mode whenever there is data to send.
 */
#define CMGR_SNIFF_EXIT_ON_SEND     0x01

/** Exit sniff mode whenever an audio link is being created.
 */
#define CMGR_SNIFF_EXIT_ON_AUDIO    0x02

/* End of CmgrSniffExitPolicy */

/*---------------------------------------------------------------------------
 * CmgrAudioParms type
 *
 * Defines the parameters for an audio connection.  These values are based
 * on the audio parameters defined by the Hands-free v1.5 specification for
 * eSCO connections.  If defined as CMGR_AUDIO_PARMS_CUSTOM, the default
 * eSCO settings will be used first.  See CMGR_SetAudioVoiceSettings(), 
 * CMGR_SetAudioLatency(), CMGR_SetAudioPackTypes(), and 
 * CMGR_SetAudioRetransmitEffort().
 */
typedef U8 CmgrAudioParms;

#define CMGR_AUDIO_PARMS_SCO      1
#define CMGR_AUDIO_PARMS_ESCO      2

/* End of CmgrAudioParms */

/* Forward reference */
typedef struct _CmgrHandler CmgrHandler;

/*---------------------------------------------------------------------------
 * CmgrCallback type
 *
 * A function of this type is called to indicate events to the profile.
 */
typedef void (*CmgrCallback) (CmgrHandler *Handler, CmgrEvent Event, BtStatus Status);

/* End of CmgrCallback */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * CmgrAudioData structure
 *
 *     Contains audio data.
 */
typedef struct _CmgrAudioData
{
    U16 len;
    U8 *data;
} CmgrAudioData;

/*---------------------------------------------------------------------------
 * CmgrHandler structure
 *
 *     Used to identify the handler for Connection Manager events.
 */
struct _CmgrHandler
{
    ListEntry node; /* Internal use only  */

    CmgrCallback callback;      /* Callback function to receive events */
    BtDeviceContext *bdc;       /* Pointer to the device context       */
    BtErrorCode errCode;        /* Error code - valid during callback  */
    BtRemoteDevice *remDev;     /* Remote device on connect indication */
    U8 sniffSubratingEnabled;
#if NUM_SCO_CONNS > 0
#if BT_SCO_HCI_DATA == XA_ENABLED
    CmgrAudioData audioData;    /* Audio data when enabled of HCI      */
    BtPacket *audioPacket;
#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 
#endif /* NUM_SCO_CONNS > 0 */ 

    /* === Internal use only === */
    BtHandler btHandler;

#if NUM_SCO_CONNS > 0
    BtScoConnect *scoConnect;
#endif 

    BtSelectDeviceToken devQuery;
    U8 flags;  /* CMGR_FLAG_XXXX */

    /* Sniff Mode Variables */
    CmgrSniffExitPolicy sniffPolicy;
    EvmTimer sniffTimer;
    U8 sniffTimerActivated;
    TimeT sniffTimeout;
    BtSniffInfo sniffInfo;
};

/*---------------------------------------------------------------------------
 * CMGR_Init()
 *
 *     Initialize the connection manager.  This function should only be called
 *     once, normally at initialization time.  The calling of this function
 *     can be specified in overide.h using the XA_LOAD_LIST macro
 *     (i.e. #define XA_LOAD_LIST XA_MODULE(CMGR)).
 *
 * Returns:
 *     TRUE - Initialization was successful
 *
 *     FALSE - Initialization failed.
 */
BOOL CMGR_Init(void);

/*---------------------------------------------------------------------------
 * CMGR_RegisterHandler()
 *
 *     Register and Initialize the connection manager handler.  A function 
 *     callback must be specified.
 *
 * Parameters:
 *     Handler - Handler structure to use.  This memory is allocated by the
 *         application.
 *
 *     Callback - Function that is called to receive events from the connection
 *         manager.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Initialization was successful.
 *
 *     BT_STATUS_IN_USE - Could not register Handler, because it is
 *         already in use.
 *
 *     BT_STATUS_INVALID_PARM - Callback function or handler are not valid.
 */
BtStatus CMGR_RegisterHandler(CmgrHandler *Handler, CmgrCallback Callback);

/*---------------------------------------------------------------------------
 * CMGR_DeregisterHandler()
 *
 *     Deregister a connection manager handler.
 *
 * Parameters:
 *     Handler - Handler structure to use.  This memory is allocated by the
 *         application.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Deinitialization was successful.
 *
 *     BT_STATUS_NOT_FOUND - Could not deregister Handler, because it is
 *         not registered.
 *
 *     BT_STATUS_INVALID_PARM - Handler is not valid.
 */
BtStatus CMGR_DeregisterHandler(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_CreateDataLink()
 *
 *     Create the ACL link.  If no ACL link exists, an outgoing link is 
 *     established.  If an ACL link exists, the Handler is registered to
 *     receive events for that link.  On an incoming link, this function
 *     must be called to receive Connection Manager events on that ACL
 *     link.
 *
 * Parameters:
 *     Handler - Handler structure to use for the duration of the link.
 *         Handler->bdc must be initialized to 0. Handler->callback must
 *         be initialized with the callback function.
 * 
 *     Addr - Address of target device.  
 * 
 *            If set to 0, the call will attempt to get a device through 
 *            the Device Selection entity by calling DS_SelectDevice().
 *            The parameters used to select the device must be provided
 *            by the caller by using the devQuery member of the CmgrHandler
 *            structure.
 *         
 *
 * Returns:
 *     BT_STATUS_PENDING - Connection started.  The CMEVENT_DATA_LINK_CON_CNF
 *         event will indicate if the connection was successful.
 *
 *     BT_STATUS_NOT_FOUND - Could not create data link, because  
 *         Handler is not registered.
 *
 *     BT_STATUS_SUCCESS - A connection already exists.
 *
 *     BT_STATUS_FAILED - Could not create connection
 *
 *     (Also see ME_CreateLink)
 */
BtStatus CMGR_CreateDataLink(CmgrHandler *Handler, BD_ADDR *Addr);

/*---------------------------------------------------------------------------
 * CMGR_RemoveDataLink()
 *
 *     Disassociates the handler from the link. If no other data links are
 *     present, the management entity will automatically drop the ACL link.
 *     No further events will be sent to the handler.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The link is already down
 *
 *     BT_STATUS_NOT_FOUND - Could not remove data link, because  
 *         Handler is not registered.
 *
 *     BT_STATUS_PENDING - The link will be closed (but regardless, no further
 *         events will be sent to the handler)
 * 
 *
 *     (Also see ME_DisconnectLink)
 */
BtStatus CMGR_RemoveDataLink(CmgrHandler *Handler);
void CMGR_RemoveCmgrWithDevice(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_IsLinkUp()
 *
 *     Returns TRUE if the data link is active.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     TRUE - Link is up.
 *
 *     FALSE - No link exists.
 */
BOOL CMGR_IsLinkUp(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_GetLinkState()
 *
 *     Returns the state of the data link.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BtRemDevState
 */
BtRemDevState CMGR_GetLinkState(CmgrHandler *Handler);

#define CMGR_GetLinkState(h) (ME_GetRemDevState((h)->bdc->link))

/*---------------------------------------------------------------------------
 * CMGR_GetRemoteDevice()
 *
 *     Returnst the BtRemoteDevice structure associated with the specified
 *     Connection Manager handler.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BtRemoteDevice if one exsits, otherwise 0.
 */
BtRemoteDevice *CMGR_GetRemoteDevice(CmgrHandler *Handler);

#define CMGR_GetRemoteDevice(h) (CMGR_IsLinkUp(h) ? (h)->bdc->link : (BtRemoteDevice *)0)
BtRemoteDevice *CMGR_GetRemoteLink(CmgrHandler *Handler);

#define CMGR_GetRemoteLink(h) ( (h)->bdc->link)

#if NUM_SCO_CONNS > 0

/* Used below */
BtStatus CmgrRegisterAudioHandler(CmgrHandler *Handler, BOOL flag);
BtStatus CmgrDeregisterAudioHandler(CmgrHandler *Handler);

/* Deprecated */
BtStatus CMGR_RegisterScoHandler(CmgrHandler *Handler);

#define CMGR_RegisterScoHandler(h) CmgrRegisterAudioHandler(h, FALSE)
BtStatus CMGR_RegisterScoRequestHandler(CmgrHandler *Handler);

#define CMGR_RegisterScoRequestHandler(h) CmgrRegisterAudioHandler(h, TRUE)
BtStatus CMGR_DeregisterScoHandler(CmgrHandler *Handler);

#define CMGR_DeregisterScoHandler(h) CmgrDeregisterAudioHandler(h)

/*---------------------------------------------------------------------------
 * CMGR_RegisterAudioHandler()
 *
 *     Register a SCO/eSCO handler.  When the handler is successfully 
 *     registered, requests to bring up the audio link by the remote device are 
 *     automatically accepted.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_NOT_FOUND - Could not register the audio handler, because  
 *         Handler is not registered.
 *
 *     (see SCO_RegisterHandler)
 */
BtStatus CMGR_RegisterAudioHandler(CmgrHandler *Handler);

#define CMGR_RegisterAudioHandler(h) CmgrRegisterAudioHandler(h, FALSE)

/*---------------------------------------------------------------------------
 * CMGR_RegisterAudioRequestHandler()
 *
 *     Register a SCO/eSCO handler.  When a the handler is successfully 
 *     registered, requests to bring up the audio link by the remote device will
 *     be indicated to the Connection Manager Handler.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_NOT_FOUND - Could not register the audio handler, because  
 *         Handler is not registered.
 *
 *     (see SCO_RegisterHandler)
 */
BtStatus CMGR_RegisterAudioRequestHandler(CmgrHandler *Handler);

#define CMGR_RegisterAudioRequestHandler(h) CmgrRegisterAudioHandler(h, TRUE)

/*---------------------------------------------------------------------------
 * CMGR_DeregisterAudioHandler()
 *
 *     Deregister a SCO/eSCO handler.  Audio link requests will no longer be 
 *     accepted.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_NOT_FOUND - Could not deregister the audio handler, because
 *         Handler is not registered.
 *
 *     (see SCO_UnregisterHandler)
 */
BtStatus CMGR_DeregisterAudioHandler(CmgrHandler *Handler);

#define CMGR_DeregisterAudioHandler(h) CmgrDeregisterAudioHandler(h)

/*---------------------------------------------------------------------------
 * CMGR_CreateAudioLink()
 *
 *     Create the SCO or eSCO connection.  The CMEVENT_AUDIO_LINK_CON event will 
 *     indicate if the connection was successful.  The Connection manager will
 *     automatically try to connect using the best possible parameters.  At the
 *     very least, a SCO connection will be established.  See 
 *     CMGR_SetAudioDefaultParms() and CMGR_SetAudioCustomParms().
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_NO_CONNECTION - No data connection exists.
 *
 *     BT_STATUS_NOT_FOUND - Could not create audio link, because Handler is
 *         not registered.
 *
 *     (Also see SCO_CreateLink)
 */
BtStatus CMGR_CreateAudioLink(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_RemoveAudioLink()
 *
 *     Bring down the SCO connection.  The CMEVENT_AUDIO_LINK_DIS event indicates 
 *     when the audio connection is down.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:    
 *     BT_STATUS_SUCCESS - The audio connection is down.
 *
 *     BT_STATUS_NOT_FOUND - Could not remove audio link, because Handler is
 *         not registered.
 *
 *     (see also SCO_DisconnectLink)
 */
BtStatus CMGR_RemoveAudioLink(CmgrHandler *Handler);

#if BT_SCO_HCI_DATA == XA_ENABLED
/*---------------------------------------------------------------------------
 * CMGR_SendAudioData()
 *
 *     Send data on the audio link.
 *
 * Requires:
 *     BT_SCO_HCI_DATA enabled
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 *     BtPacket - Packet initialized with audio data.
 *
 * Returns:    
 *     (see SCO_SendData)
 */
BtStatus CMGR_SendAudioData(CmgrHandler *Handler, BtPacket *Packet);

#define CMGR_SendAudioData(h, p) SCO_SendData((h)->scoConnect, p);
#endif /* BT_SCO_HCI_DATA == XA_ENABLED */

/*---------------------------------------------------------------------------
 * CMGR_IsAudioUp()
 *
 *     Returns the state of the audio link.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     TRUE - Audio link is up.
 *
 *     FALSE - No audio link exists.
 */
BOOL CMGR_IsAudioUp(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_GetAudioDefaultParms()
 *
 *     Gets the default parameters for SCO or eSCO connections.  These settings
 *     are based on the settings described in the Hands-free 1.5 specification.
 *     This is a global setting that directly affects all handlers.
 *
 * Returns:  
 *     (See CmgrAudioParms)
 */
CmgrAudioParms CMGR_GetAudioDefaultParms(void);

/*---------------------------------------------------------------------------
 * CMGR_SetAudioVoiceSettings()
 *
 *     Sets the voice settings for audio connections.
 *
 * Returns:
 *     void
 */
void CMGR_SetAudioVoiceSettings(BtScoAudioSettings settings);

/*---------------------------------------------------------------------------
 * CMGR_SetAudioVoiceSettings()
 *
 *     Gets the current voice settings for audio connections.
 *
 * Returns:
 *     Current voice settings (See BtScoAudioSettings)
 */
BtScoAudioSettings CMGR_GetAudioVoiceSettings(void);

/*---------------------------------------------------------------------------
 * CMGR_SetAudioCustomParms()
 *
 *     Sets the custom audio parameters.  This is a global setting that directly
 *     affects all handlers.  These parameters will be used to create an eSCO 
 *     connection if possible.  If the connection fails, then the 
 *     CMGR_AUDIO_PARMS_S1 settings will be used.  If that fails, then a SCO 
 *     connection will be attempted.
 *
 * Parameters:
 *     customParms - a pointer to the SCO transmit parameters.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - parameters were set successfully.
 *     BT_STATUS_INVALID_PARMS - invalid parameter (Error check only)
 */
BtStatus CMGR_SetAudioCustomParms(BtScoTxParms *customParms);

U8 CMGR_GetProfileSupportAudioType(void);

#endif /* NUM_SCO_CONNS > 0 */

/*---------------------------------------------------------------------------
 * CMGR_SetSniffTimer()
 *
 *     Sets the timeout value used to put the link into sniff mode.  This
 *     timer starts after a link connection is complete.  It is disabled
 *     when a SCO (or eSCO) connection is established and the sniff mode is 
 *     exited.  When the SCO (or eSCO) connection is terminated the timer
 *     is started again.  If CMGR_IndicateLinkActivity is called, the timer
 *     is restarted.  If multiple handlers set sniff mode timers, then
 *     the longest timer is used.
 *
 *     The sniff timeout can be set to any value, however, it is
 *     important to note that a value of 0 means that the handler
 *     doesn't care if sniff mode is used and a value of -1 means
 *     that sniff mode is disabled for the specified handler (which 
 *     also disables sniff mode for all handlers).
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 *     SniffInfo - Sniff mode parameters.
 *
 *     Time - Amount of time to wait before starting sniff mode.  Any value
 *         greater than 0 will set the timer.  CMGR_SNIFF_DISABLED disables
 *         sniff mode for the specified handler, and consequently, for all
 *         handlers with connections to the same device (see 
 *         CMGR_DisableSniffTimer()).  CMGR_SNIFF_DONT_CARE means that this 
 *         handler doesn't care if sniff mode is used (see 
 *         CMGR_ClearSniffTimer()).
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Sniff timer set.
 *
 *     BT_STATUS_NOT_FOUND - Could not set the sniff timer, because Handler
 *         is not registered.
 */
BtStatus CMGR_SetSniffTimer(CmgrHandler *Handler, BtSniffInfo *SniffInfo, TimeT Time);
BtStatus CMGR_ActivateSniffTimer(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_GetSniffTimer()
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     TimeT - Amount of time to wait before starting sniff mode.
 */
TimeT CMGR_GetSniffTimer(CmgrHandler *Handler);

#define CMGR_GetSniffTimer(h) ((h)->sniffTimeout)

/*---------------------------------------------------------------------------
 * CMGR_ClearSniffTimer()
 *
 *     Clears the sniff mode timeout value.  This is equivalent to saying that 
 *     the handler no longer cares if the link is put into sniff mode or not.
 *     Clears the timeout value before the link is put into sniff mode. 
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Sniff timer cleared.
 *
 *     BT_STATUS_NOT_FOUND - Could not clear the sniff timer, because  
 *         Handler is not registered.
 */
BtStatus CMGR_ClearSniffTimer(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_IndicateLinkActivity()
 *
 *     Indicates that activity has occured on the link, which restarts
 *     the sniff timeout.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 */
void CMGR_IndicateLinkActivity(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_DisableSniffTimer()
 *
 *     Disables sniff mode for the specified handler.  This also disables 
 *     sniff mode for all handlers.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Sniff mode disabled.
 *
 *     BT_STATUS_NOT_FOUND - Could not disable the sniff timer, because  
 *         Handler is not registered.
 */
BtStatus CMGR_DisableSniffTimer(CmgrHandler *Handler);

#if defined (__BT_2_1_SNIFF_SUBRATING__)

/*---------------------------------------------------------------------------
 * CMGR_EnableSniffSubratingMode()
 *
 *     To set the sniff subrating mode ON.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     N/A
 */

BtStatus CMGR_EnableSniffSubratingMode(CmgrHandler *Handler);

/*---------------------------------------------------------------------------
 * CMGR_DisableSniffSubratingMode()
 *
 *     To set the sniff subrating mode OFF.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 * Returns:
 *     N/A
 */
void CMGR_DisableSniffSubratingMode(CmgrHandler *Handler);
#endif /* defined (__BT_2_1_SNIFF_SUBRATING__) */ 

/*---------------------------------------------------------------------------
 * CMGR_SetSniffExitPolicy()
 *
 *     Sets the policy for exiting sniff mode.  The policy can be set to 
 *     CMGR_SNIFF_EXIT_ON_SEND or CMGR_SNIFF_EXIT_ON_AUDIO_LINK.  These 
 *     values can also be OR'd together to enable both (See 
 *     CmgrSniffExitPolicy).
 *
 * Parameters:
 *     Handler - Handler structure to use.
 * 
 *     Policy - Bitmask that defines the policy for exiting sniff mode.
 *
 * Returns:
 *
 *     BT_STATUS_SUCCESS - The policy was set successfully.
 *
 *     BT_STATUS_NOT_FOUND - Could not set the sniff policy, because  
 *         Handler is not registered.
 */
BtStatus CMGR_SetSniffExitPolicy(CmgrHandler *Handler, CmgrSniffExitPolicy Policy);

/*---------------------------------------------------------------------------
 * CMGR_GetSniffExitPolicy()
 *
 *     Gets the policy for exiting sniff mode on the specified channel.
 *
 * Parameters:
 *     Handler - Handler structure to use.
 * 
 * Returns:
 *
 *     CmgrSniffExitPolicy
 */
CmgrSniffExitPolicy CMGR_GetSniffExitPolicy(CmgrHandler *Handler);
BtStatus CMGR_SetDeviceAsMaster(CmgrHandler *Handler);
BtStatus CMGR_SetRemoteAsMaster(BtRemoteDevice *link);
void CMGR_HidNoRoleSwitch(U8 on_off);

BtStatus CMGR_ReadRSSI(BtRemoteDevice *remDev);
BOOL ConnMgrIsLinkConnected(CmgrHandler *Handler);
//BOOL ConnMgrCheckLinkStatus(CmgrHandler *Handler, U8 link_state);

#endif /* __CONMGR_H_ */ 

