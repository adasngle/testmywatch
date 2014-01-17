#ifndef __ME_H
#define __ME_H

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

#include "me_adp.h"

#include "bttypes.h"
#include "config.h"
#include "xatypes.h"
#include "osapi.h"
/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * ME_RegisterGlobalHandler()
 *
 *     Register a callback handler with the Management Entity to
 *     receive global events. The protocol stack sends a number
 *     of global events to all registered handlers (who have the
 *     proper event mask set). Clients must register a handler 
 *     in order to receive events. The list of global events for 
 *     which the callback will be called are listed in the 
 *     Callback section below. The event mask for the handler is 
 *     initialized to BEM_NO_EVENTS.
 *
 * Parameters:
 *     handler - pointer to a BtHandler structure. The callback
 *         field of the handler must point to a valid callback function.
 *         The handler must be initialized using ME_InitHandler.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - operation completed successfully.
 *
 *     BT_STATUS_FAILED - the operation failed because the handle is
 *         already registered, the handle is not initialized properly.
 */
BtStatus ME_RegisterGlobalHandler(BtHandler *handler);

/*---------------------------------------------------------------------------
 * ME_InitHandler()
 *
 *     Initialize a handler. All global handlers must be initialized
 *     before they can be passed to ME functions. This function
 *     can be used or filling the contents of the handler with 0
 *     will also suffice. A handler should only need to be initialized
 *     once.
 *
 * Parameters:
 *     handler - pointer to a BtHandler structure. 
 */
void ME_InitHandler(BtHandler *handler);

#define ME_InitHandler(h) InitializeListEntry(&((h)->node))

/*---------------------------------------------------------------------------
 * ME_UnregisterGlobalHandler()
 *
 *     Unregister a global handler. Upon successful completion of this 
 *     function the handler is initialized and can be registered again 
 *     without the need for calling ME_InitHandler.
 *
 * Parameters:
 *     handler - pointer to handler to unregister
 *
 * Returns
 *     BT_STATUS_SUCCESS - operation successful
 *
 *     BT_STATUS_FAILED - operations failed because handler
 *         was not registered or 0 (error check only).
 */
BtStatus ME_UnregisterGlobalHandler(BtHandler *handler);

/*---------------------------------------------------------------------------
 * ME_SetEventMask()
 *
 *     Set the event mask for the global handler. The event mask 
 *     is for global events.
 *
 * Parameters:
 *     handler - pointer to handler.
 *
 *     mask - event mask.
 *
 * Returns
 *     BT_STATUS_SUCCESS - operation successful
 *
 *     BT_STATUS_FAILED - operations failed because handler
 *         was not registered or 0 (error check only).
 */
#if XA_ERROR_CHECK == XA_ENABLED
BtStatus ME_SetEventMask(BtHandler *handler, BtEventMask mask);
#else /* XA_ERROR_CHECK */
#define ME_SetEventMask(h,m) ((h)->eMask = (m),BT_STATUS_SUCCESS)
#endif /* XA_ERROR_CHECK */

/*---------------------------------------------------------------------------
 * ME_GetEventMask()
 *
 *     Get the event mask for the global handler. The event mask
 *     is for global events. This function does not check to
 *     see if the handler is valid.
 *
 * Parameters:
 *     handler - pointer to handler.
 *
 * Returns:
 *     The event mask 
 */
BtEventMask ME_GetEventMask(BtHandler *handler);

#define ME_GetEventMask(h) ((h)->eMask)

/*---------------------------------------------------------------------------
 * ME_Inquiry()
 *
 *     Start a Bluetooth Inquiry procedure. If BT_STATUS_PENDING is
 *     returned then the results of the operation will be returned
 *     to all clients with registered global handlers. The following
 *     events will be sent to all registered handlers:
 *
 *     BTEVENT_INQUIRY_RESULT - indicates that a device was found. The
 *     "p.inqResult" field of the BtEvent contains the result.
 *
 *     BTEVENT_INQUIRY_COMPLETE - indicates that the inquiry process is
 *     complete. The "errCode" field of the BtEvent indicates whether 
 *     the operation completed without error.
 *
 *     BTEVENT_INQUIRY_CANCELED - this will be returned if the inquiry
 *     operation is canceled. BTEVENT_INQUIRY_COMPLETE will not be 
 *     returned.
 *
 *     BTEVENT_HCI_FATAL_ERROR - indicates fatal radio or HCI
 *     transport error. Assume all pending operations have failed.
 *     
 * Parameters:
 *     lap - LAP used for the Inquiry (General or Limited)
 *
 *     length - Maximum amount of time before the Inquiry is
 *         halted. Range is 0x01 to 0x30. Time is length * 1.28 sec.
 *         The Generic Access profile specifies using the value
 *         BT_INQ_TIME_GAP100.
 *
 *     maxResp - The maximum number of responses. Specify zero to receive
 *         an unlimited number of responses.
 *
 * Returns:
 *     BT_STATUS_PENDING - The Inquiry process is started results
 *         will be sent via the handler. A BTEVENT_HCI_FATAL_ERROR event
 *         indicates a fatal radio or HCI transport error and that all
 *         pending operations have failed.
 *
 *     BT_STATUS_IN_PROGRESS - An inquiry process is already in 
 *         progress. Only one Inquiry can be in progress at a time.  
 *         Keep track of the general events to get the results from 
 *         the current Inquiry or to see when it ends. If it has just
 *         ended then a cancel is also in progress so wait for
 *         the cancel to complete to start another inquiry.
 *
 *     BT_STATUS_FAILED - The operation failed. 
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_Inquiry(BtIac lap, U8 Length, U8 maxResp);

/*---------------------------------------------------------------------------
 * ME_CancelInquiry()
 *
 *     Cancel the current Inquiry process. When the inquiry
 *     operation is canceled all registered handlers will
 *     receive the BTEVENT_INQUIRY_CANCELED event. The "errCode"
 *     field of the BtEvent indicates the status of the
 *     operation.
 *
 * Returns:
 *     BT_STATUS_PENDING - The cancel operation was started
 *         successfully. The results will be sent to all clients
 *         with registered handlers. A BTEVENT_HCI_FATAL_ERROR event
 *         indicates a fatal radio or HCI transport error and that all
 *         pending operations have failed.
 *
 *     BT_STATUS_SUCCESS - The inquiry process was canceled
 *         immediately. It actually never was started. 
 *         BTEVENT_INQUIRY_CANCELED event will be sent to all handlers 
 *         before this function returns.
 *
 *     BT_STATUS_IN_PROGRESS - A cancel Inquiry is already in
 *         progress. Only one cancel can be in progress at a time.
 *         Keep track of the general events to see when the cancel
 *         is complete. 
 * 
 *     BT_STATUS_FAILED - The operation failed because an inquiry
 *         operation was not in progress.
 */
BtStatus ME_CancelInquiry(void);

/*---------------------------------------------------------------------------
 * ME_GetRemoteDeviceName()
 *
 *     Get the name of the remote device. If an ACL connection
 *     does not exist then a temporary one will be created to
 *     get the name. If this function returns BT_STATUS_PENDING
 *     then the result will be returned via the callback in the
 *     token with the BTEVENT_NAME_RESULT event. The "errCode" field 
 *     indicates the status of the operation. "p.meToken" points
 *     to the token passed into the function. The output field of
 *     token contains the results if the status indicates success.
 *
 *     If a request to the same remote device is already in progress
 *     only one request will be made. This token will receive the
 *     results of the request in progress.
 *
 * Parameters:
 *     token - pointer to token containing input parameters and
 *         storage for output parameters. The token must be initialized 
 *         before calling this function as follows:
 *
 *         "token.callback" must be set to the callback function.
 *
 *         "token.p.name.bdAddr" must contain the 48-bit address of
 *         the remote device.
 *
 *         "token.p.name.io.in.psi" must contain the page scan information.
 *         If the page scan information is not known then all fields should
 *         be 0.
 *
 * Returns:
 *     BT_STATUS_PENDING - Operation is started and results will
 *         be sent to the callback.
 *
 *     BT_STATUS_FAILED - operation failed because the token was
 *         not properly initialized or already active (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_GetRemoteDeviceName(MeCommandToken *token);

/*---------------------------------------------------------------------------
 * ME_CancelGetRemoteDeviceName()
 *
 *     Request cancellation of a pending ME_GetRemoteDeviceName request.
 *     If this function returns BT_STATUS_SUCCESS, then "token" is
 *     free and no further events will be returned. If this function
 *     returns BT_STATUS_PENDING, then the result will be returned
 *     via the callback in "p.meToken" with the BTEVENT_NAME_RESULT event.
 *     The "errCode" field will indicate status BEC_REQUEST_CANCELLED
 *     if cancelled or BEC_NO_ERROR if the name was returned before the
 *     cancel completed.
 *
 * Parameters:
 *     token - pointer to token passed in to ME_GetRemoteDeviceName.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - the operation has completed successfully.
 *         The "token" is free and no further events will be returned.
 *
 *     BT_STATUS_PENDING - Operation is started and results will
 *         be sent to the callback.
 *
 *     BT_STATUS_FAILED - operation failed because the get remote
 *         device name request is no longer in progress.
 */
BtStatus ME_CancelGetRemoteDeviceName(MeCommandToken *token);

/*---------------------------------------------------------------------------
 * ME_SetLocalDeviceName()
 *
 *     Set the local device name. This is the name that other devices will
 *     get when performing a GetRemoteDeviceName to this device. The name
 *     is a UTF-8 encoded string. The name can be up to 248 bytes in length.
 *     If the name is less than 248 bytes it must be null-terminated.
 *
 * Parameters:
 *     name - pointer to a UTF-8 encoded string. The name will remain in use
 *         until the stack is deinitialized, or until this function is called
 *         with a different name. The name string should not be modified and
 *         the pointer must remain valid while it is in use.
 *
 *     len - indicates the number of bytes in the name including the null
 *         termination byte if the name is less than 248 bytes. 
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The operation is successful.
 *
 *     BT_STATUS_INVALID_PARM - The operation failed because the name or 
 *         length were not properly initialized (XA_ERROR_CHECK only).
 */
BtStatus ME_SetLocalDeviceName(const U8 *name, U8 len);

/*---------------------------------------------------------------------------
 * ME_ReadLocalBdAddr()
 *
 *     Get the 48-bit Bluetooth device address of the local device. This
 *     request cannot be issued until the radio has been initialized. Check
 *     ME_GetStackInitState for BTSS_INITIALIZED. If it's not initialized
 *     wait for the BTEVENT_HCI_INITIALIZED event on a global handler.
 *
 * Parameters:
 *     Addr - Pointer to a BD_ADDR structure to receive the address.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Operation was successful.
 *
 *     BT_STATUS_FAILED - Operation failed because the radio is not
 *         initialized yet. Monitor the global events to be notified
 *         when the radio is initialized (BTEVENT_HCI_INITIALIZED).
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_ReadLocalBdAddr(BD_ADDR *Addr);

/*---------------------------------------------------------------------------
 * ME_SetClassOfDevice()
 *
 *     Set the class of device. The class of device is sent out as part of
 *     the response to inquiry. The actual bit pattern that is sent out is
 *     a composite of the value set using this function and the service
 *     class information found in registered SDP records.
 *
 * Parameters:
 *     classOfDevice - A 32-bit integer where the lower 3 bytes represents
 *         the class of device. The most significant 8 bits are ignored. 
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Operation is successful.
 */
BtStatus ME_SetClassOfDevice(BtClassOfDevice classOfDevice);

#if defined (__BT_2_1_ENTENDED_INQUIRY_RESPONSE__)
BtStatus ME_StartWriteEIR(void);
#endif 

#ifdef __GAP_TESTER_
BtStatus ME_DisableAFH(void);
#endif 
/*---------------------------------------------------------------------------
 * ME_FindRemoteDevice()
 *
 *     Return a pointer to the BtRemoteDevice structure corresponding to
 *     the device with given BD_ADDR. 0 is returned if a match is not found.
 *
 *     When this function returns successfully, the BtRemoteDevice structure
 *     returned always refers to a connected or connecting device. Use
 *     caution, as it will also return successfully if the device is in the
 *     process of disconnecting, but has not yet completed. This typically
 *     occurs if called during the notification of a link failure.
 *
 * Parameters:
 *     bdAddr - pointer to 48-bit address of the device
 *
 * Returns:
 *     Pointer to BtRemoteDevice structure if found. 0 is returned if the
 *     device is not found.
 */
BtRemoteDevice *ME_FindRemoteDevice(BD_ADDR *bdAddr);

/* Prototype for actual ME_FindRemoteDevice implementation */
BtRemoteDevice *ME_FindRemoteDeviceP(U8 *bdAddr);

#define ME_FindRemoteDevice(bd) (ME_FindRemoteDeviceP((bd)->addr))

/*---------------------------------------------------------------------------
 * ME_CreateLink()
 *
 *     Create an ACL link to the device specified by "bdAddr". If
 *     this function returns BT_STATUS_PENDING the results will be sent to
 *     the handler. When the operation is complete all registered global
 *     handlers will receive BTEVENT_LINK_CONNECT_CNF as will the handler
 *     passed to this function. The "errCode" field of the BtEvent will 
 *     indicate if the operation was successful or not. If an incoming
 *     connection is in progress to this device then the handler will be
 *     bound and when the incoming connection is complete the handler will
 *     receive the BTEVENT_LINK_CONNECT_CNF event. In this case all global
 *     registered handlers will receive BTEVENT_LINK_CONNECT_IND instead of
 *     BTEVENT_LINK_CONNECT_CNF.
 *
 *     If connections already exist to other devices, then ME will attempt
 *     to put all other connections on hold, then begin a connection
 *     to the specified device.
 *
 *     If a connection already exists to the device indicated by bdAddr,
 *     this function will register the handler with the BtRemoteDevice
 *     and return BT_STATUS_SUCCESS immediately. In this way, many local
 *     handlers can be registered to the same connection. Be aware that
 *     ME_DisconnectLink will not take effect until it is called for
 *     ALL registered handlers of the BtRemoteDevice. To determine whether
 *     a connection already exists without registering a handler,
 *     use ME_FindRemoteDevice instead.
 *
 *     A slave device does not need to use this API unless it needs
 *     to register a local handler or initiate a disconnection of the link
 *     (see ME_DisconnectLink).
 *          
 *     The type of connection created to the remote device uses
 *     the quality of service parameters necessary for the RFCOMM
 *     based profiles or profiles requiring reliable, non
 *     real-time critical links. For other types of connections, use
 *     ME_CreateRestrictedLink.
 *
 * Parameters:
 *     handler - pointer to an unregistered handler to receive 
 *         events. This handler cannot be the global handler though
 *         they both can point to the same callback. The handler 
 *         must be initialized using ME_InitHandler. The handler 
 *         is in use until the link is disconnected and should not
 *         be modified while in use.
 *
 *     bdAddr - pointer to address of remote device.
 *
 *     psi - pointer to page scan information obtained during 
 *         Inquiry. Initial links to a device should set this parameter
 *         to ensure a reliable connection. If the psi is not known, a
 *         default "psi" of 0 on an initial link will use clockOffset = 0,
 *         psMode = 0, and psRepMode = 1. If a client is binding to an
 *         existing remote device, then "psi" can be set to 0.
 *
 *     remDev - pointer to memory to receive remote device pointer.
 *
 * Returns:
 *     BT_STATUS_PENDING - operation has started successfully
 *         result will be sent to handler.
 *
 *     BT_STATUS_SUCCESS - client is successfully bound to the remote device.
 *
 *     BT_STATUS_RESTRICTED - the operation failed because this remote
 *         device already has a restricted link. The handler is free.
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because this remote
 *         device is in the process of being disconnected. The handler
 *         is free.
 *
 *     BT_STATUS_NO_RESOURCES - the operation failed because 
 *         the maximum number of connections already exist. The
 *         handler is free.
 *
 *     BT_STATUS_FAILED - operation failed because the handler is 0, the 
 *         callback is 0 or the handler is not initialized (XA_ERROR_CHECK
 *         only). The handler is free.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_CreateLink(BtHandler *handler, BD_ADDR *bdAddr, BtPageScanInfo *psi, BtRemoteDevice **remDev);

/*---------------------------------------------------------------------------
 * ME_CheckCreatingLink()
 *
 *     Check if the BT Chip is creating ACL link or not.
 */

BOOL ME_CheckCreatingLink(void);

/*---------------------------------------------------------------------------
 * ME_CreateRestrictedLink()
 *
 *     (Not implemented.)
 *
 *     Create a restricted Link to a remote device. A restricted
 *     link is one where only the client specified by the handler
 *     is allowed to bind to the connection. The client also has
 *     the ability to specify the quality of service parameters
 *     so a time bounded link with specific latency and bandwidth
 *     can be created.
 *
 *     If this function returns BT_STATUS_PENDING the results will be sent
 *     to the handler. When the operation is complete all registered global
 *     handlers will receive BTEVENT_LINK_CONNECT_CNF as will the handler
 *     passed to this function. The "errCode" field of the BtEvent will 
 *     indicate if the operation was successful or not. If an incoming
 *     connection is in progress to this device then the handler will be
 *     bound and when the incoming connection is complete the handler will
 *     receive the BTEVENT_LINK_CONNECT_CNF event. In this case all global
 *     registered handlers will receive BTEVENT_LINK_CONNECT_IND instead of
 *     BTEVENT_LINK_CONNECT_CNF.
 *
 *     If connections already exist to another devices then all
 *     existing connections will be put on hold while this operation
 *     takes place.
 *
 * Parameters:
 *     handler - pointer to registered handler to receive events.
 *
 *     bdAddr - address of remote device.
 *
 *     psi - pointer to page scan information obtained during 
 *         Inquiry. If this information is not known then "psi" should be 
 *         set to 0.
 *
 * Returns:
 *     BT_STATUS_PENDING - operation was started successfully and
 *         results will be sent to the handler.
 *
 *     BT_STATUS_RESTRICTED - operation failed because this remote
 *         device already has a restricted link.
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because this
 *         remote device is in the process of being disconnected.
 *
 *     BT_STATUS_NO_RESOURCES - the operation failed because 
 *         the maximum number of connections already exist. The
 *         handler is free.
 *
 *     BT_STATUS_FAILED - operation failed because the handler
 *         is 0, the callback is 0 or the handler is not initialized
 *         (XA_ERROR_CHECK only). The handler is free.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_CreateRestrictedLink(
            BtHandler *handler,
            BD_ADDR *bdAddr,
            BtPageScanInfo *psi,
            BtRestrictedInfo *resInfo);

/*---------------------------------------------------------------------------
 * ME_ForceDisconnectLinkWithReason()
 *
 *     Force disconnection of the link regardless of registered handlers.
 *     See ME_DisconnectLink for additional information.
 *
 * Parameters:
 *     handler - pointer to registered handler (can be 0).
 *
 *     remDev - pointer to the remote device to be disconnected.
 *
 *     reason - disconnect reason. Must be one of: BEC_AUTHENTICATE_FAILURE,
 *          BEC_USER_TERMINATED, BEC_LOW_RESOURCES, BEC_POWER_OFF,
 *          BEC_UNSUPPORTED_REMOTE, or BEC_PAIR_UNITKEY_NO_SUPP.
 *
 *     forceDisconnect - If TRUE, will attempt to disconnect regardless of
 *         whether other handlers are still registered. If FALSE, behaves as
 *         ME_DisconnectLinkWithReason.
 *
 * Returns:
 *     BT_STATUS_INVALID_PARM - the operation failed because the "reason"
 *         parameter is invalid for a disconnect link operation.
 *
 *     See ME_DisconnectLink for additional returns.
 */
BtStatus ME_ForceDisconnectLinkWithReason(
            BtHandler *handler,
            BtRemoteDevice *remDev,
            BtErrorCode reason,
            BOOL forceDisconnect);

/*---------------------------------------------------------------------------
 * ME_DisconnectLinkWithReason()
 *
 *     Request disconnection of the link with a non-default reason code.
 *     See ME_DisconnectLink for additional information.
 *
 * Parameters:
 *     handler - pointer to registered handler.
 *
 *     remDev - pointer to the remote device to be disconnected.
 *
 *     reason - disconnect reason. If multiple handlers call this
 *         function or ME_DisconnectLink, the last handler's reason code
 *         will be used. If reason is BEC_POWER_OFF, disconnect is
 *         more forceful in that all the handlers need not have already
 *         been freed.
 *
 * Returns:
 *     BT_STATUS_INVALID_PARM - the operation failed because the "reason"
 *         parameter is invalid for a disconnect link operation.
 *
 *     See ME_DisconnectLink for additional returns.
 */
BtStatus ME_DisconnectLinkWithReason(BtHandler *handler, BtRemoteDevice *remDev, BtErrorCode reason);

#define ME_DisconnectLinkWithReason(ha, rd, rs) \
    (ME_ForceDisconnectLinkWithReason(ha, rd, rs, FALSE))

/*---------------------------------------------------------------------------
 * ME_DisconnectLink()
 *
 *     Request disconnection of the link, using the default reason code
 *     (BEC_USER_TERMINATED). This function also deregisters the
 *     specified handler, which was previously registered with ME_CreateLink
 *     or ME_CreateRestrictedLink.
 *
 *     The link is actually closed only when all registered handlers have
 *     requested disconnection and all L2CAP connections are closed. If
 *     a device needs to know exactly when the disconnection occurs,
 *     it should use a global handler to monitor connection state. The
 *     handler specified in this API is freed by this function, so it
 *     will not be notified of any disconnection events.
 *
 *     If the remote device initiated the connection, the local
 *     device may have no registered handler for the connection. In this
 *     case, the local device must call ME_CreateLink or
 *     ME_CreateRestrictedLink to register a handler, then call this function
 *     to request disconnection.
 *     
 * Parameters:
 *     handler - pointer to registered handler.
 *
 *     remDev - pointer to the remote device to be disconnected.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - the operation has completed successfully. 
 *         The caller's handler is unbound from the link. When the link 
 *         is actually disconnected the BTEVENT_LINK_DISCONNECT event will 
 *         be sent to all registered global handlers, unless the link had 
 *         not actually come up yet, in which case no event will be 
 *         generated. The handler is free upon return from this function.
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because the
 *         link is the process of coming up. The connect request has 
 *         been made by a remote device. Wait until the 
 *         BTEVENT_LINK_CONNECT_CNF event has been received. The handler 
 *         is not free.
 *
 *     BT_STATUS_FAILED - the operation failed. The link is
 *         already disconnected, disconnect has already been
 *         issued for this handler, or the handler is not
 *         bound to the link (XA_ERROR_CHECK only).
 */
BtStatus ME_DisconnectLink(BtHandler *handler, BtRemoteDevice *remDev);

#define ME_DisconnectLink(ha, rd) \
    (ME_ForceDisconnectLinkWithReason(ha, rd, BEC_USER_TERMINATED, FALSE))

/*---------------------------------------------------------------------------
 * ME_CancelCreateLink()
 *
 *     Request cancellation of the pending ME_CreateLink. This function
 *     also deregisters the specified handler, which was previously
 *     registered when the ME_CreateLink was initiated.
 *
 * Parameters:
 *     handler - pointer to registered handler.
 *
 *     remDev - pointer to the remote device associated with the
 *          ME_CreateLink to be canceled.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - the operation has completed successfully. 
 *         The caller's handler is unbound from the link. If more than one
 *         handler is bound to the link, no further events will occur.
 *         Otherwise, the BTEVENT_LINK_CREATE_CANCEL event will be sent to
 *         all registered global handlers when the cancel completes. The
 *         global handler must still check the BtEvent "errCode" field
 *         in the event structure, as the cancel may fail and the link may
 *         require further maintenance. The handler is free upon return
 *         from this function.
 *
 *     BT_STATUS_FAILED - the operation failed. The link is already
 *         connected or disconnected, or the handler is not bound to
 *         the link (XA_ERROR_CHECK only).
 */
BtStatus ME_CancelCreateLink(BtHandler *handler, BtRemoteDevice *remDev);

/*---------------------------------------------------------------------------
 * ME_RegisterAcceptHandler()
 *
 *     Register the handler that will accept incoming connections.
 *     If no handler is registered then the Management Entity will
 *     accept all incoming connections automatically. If the
 *     handler is set it will be responsible for accepting 
 *     connections by calling ME_AcceptIncomingLink. The handler
 *     will be notified with the BTEVENT_LINK_CONNECT_REQ event. The
 *     p.remDev field of the btEvent will contain a pointer to the
 *     remote device.
 *
 * Parameters:
 *     handler - pointer to handler. This can be a registered global
 *         handler but does not have to be.
 *
 * Returns:
 *     Pointer to old handler. 
 */
BtHandler *ME_RegisterAcceptHandler(BtHandler *handler);

/*---------------------------------------------------------------------------
 * ME_AcceptIncomingLink()
 *
 *     Accept an incoming link. The incoming link is signaled by
 *     a call to the accept handler with the event 
 *     BTEVENT_LINK_CONNECT_REQ. 
 *
 * Parameters:
 *     remDev - pointer to the remote device structure representing
 *         the incoming connection. This was passed in the 
 *         "btEvent.p.remDev" field of the BTEVENT_LINK_CONNECT_REQ event.
 *
 *     role - desired role. BCR_MASTER cause a master/slave switch
 *         with the local device becoming the master. BCR_SLAVE will 
 *         cause the local device to remain the slave.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully
 *         and the result will be sent to the accept handler via
 *         the BTEVENT_LINK_ACCEPT_RSP event. When complete all registered 
 *         global handlers will receive the BTEVENT_LINK_CONNECT_IND event.
 *         The "errCode" field of the BtEvent will indicate the success.
 *
 *     BT_STATUS_FAILED - the operation failed because the remote
 *         device is not in a state to accept an incoming connection.
 *
 *     BT_STATUS_INVALID_PARM - the operation failed because the
 *         "remDev" parameter is invalid or 0 or the role parameter
 *         is invalid (XA_ERROR_CHECK only).
 *     
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_AcceptIncomingLink(BtRemoteDevice *remDev, BtConnectionRole role);

/*---------------------------------------------------------------------------
 * ME_RejectIncomingLink()
 *
 *     Reject an incoming link. The incoming link is signaled by
 *     a call to the accept handler with the event 
 *     BTEVENT_LINK_CONNECT_REQ. 
 *
 * Parameters:
 *     remDev - pointer to the remote device structure representing
 *         the incoming connection. This was passed in the 
 *         "btEvent.p.remDev" field of the BTEVENT_LINK_CONNECT_REQ event.
 *
 *     reason - reason for the rejection. The only reasons allowed
 *         are as follows:
 *
 *     BEC_LIMITED_RESOURCE - Host rejected due to limited resources
 *
 *     BEC_SECURITY_ERROR   - Host rejected due to security reasons
 *
 *     BEC_PERSONAL_DEVICE  - Host rejected (remote is personal device)
 *
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully
 *         and the result will be sent to the accept handler via
 *         the BTEVENT_LINK_REJECT_RSP event. 
 *
 *     BT_STATUS_FAILED - the operation failed because the remote
 *         device is not in a state to reject an incoming connection.
 *
 *     BT_STATUS_INVALID_PARM - the operation failed because the
 *         "remDev" parameter is invalid or 0 or the reason is not
 *         valid (XA_ERROR_CHECK only).
 *     
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_RejectIncomingLink(BtRemoteDevice *remDev, BtErrorCode reason);

/*---------------------------------------------------------------------------
 * ME_GetCurrentMode()
 *
 *     Get the current mode of the link to the remote device. The
 *     value is meaningless if a connection does not exist to
 *     the remote device. This function does not check for a valid
 *     remDev pointer.
 *
 * Parameters:
 *     rm - pointer to remote device.
 *
 * Returns:
 *     The current mode if a link exists otherwise the value
 *     is meaningless.
 */
BtLinkMode ME_GetCurrentMode(BtRemoteDevice *rm);

#define ME_GetCurrentMode(r) ((r)->mode)

/*---------------------------------------------------------------------------
 * ME_GetCurrentRole()
 *
 *     Get the current role played by the local device. The
 *     value is meaningless if a connection does not exist to
 *     the remote device. This function does not check for a valid
 *     remDev pointer. When the role is currently being discovered,
 *     the role BCR_UNKNOWN will be returned. When the role discovery
 *     completes, the BTEVENT_ROLE_CHANGE event will be indicated.
 *
 * Parameters:
 *     rm - pointer to remote device.
 *
 * Returns:
 *     The current role if a link exists otherwise the value
 *     is meaningless.
 */
BtConnectionRole ME_GetCurrentRole(BtRemoteDevice *rm);

#define ME_GetCurrentRole(r) ((r)->role > BCR_UNKNOWN ? BCR_UNKNOWN : (r)->role)

/*---------------------------------------------------------------------------
 * ME_Hold()
 *
 *     Put the ACL link associated with remDev in hold mode. When the
 *     link is actually placed in hold all registered and bound handlers
 *     will receive a BTEVENT_MODE_CHANGE event signaling the change.
 *
 * Parameters:
 *     remDev - pointer to remote device
 *
 *     max - maximum length of the hold interval. The hold time
 *         is calculated as max * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *
 *     min - minimum length of the hold interval. The hold time
 *         is calculated as min * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *         The parameter min should not be greater than max.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - Invalid parameter (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed because ACL link is
 *         in wrong state.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_Hold(BtRemoteDevice *remDev, U16 max, U16 min);

/*---------------------------------------------------------------------------
 * ME_StartSniff()
 *
 *     Start sniff mode for the ACL link specified by "remDev".
 *
 * Parameters:
 *     remDev - pointer to remote device
 *
 *     info - pointer to the sniff mode parameters. This structure may
 *            be freed after ME_StartSniff returns.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - Invalid parameter (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StartSniff(BtRemoteDevice *remDev, const BtSniffInfo *info);

/*---------------------------------------------------------------------------
 * ME_StopSniff()
 *
 *     Stop sniff mode and enter active mode for the ACL link
 *     specified by remDev. 
 *          
 * Parameters:
 *     remDev - pointer to remote device.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed. Device is not in
 *         sniff mode.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StopSniff(BtRemoteDevice *remDev);

/*---------------------------------------------------------------------------
 * ME_StartSniffSubrating()
 *
 *     Start to make the sniff subrating for the ACL link specified by "remDev".
 *
 * Parameters:
 *     remDev - pointer to remote device
 *
 *     maxLatency - It is used to calculate the max sniff subrate that the 
 *                  remote device may use.
 *     minRemoteTimeout - The min base sniff subrate timeout the remote device 
 *                  may use.
 *     minLocalTimeout - The min base sniff subrate timeout the local device 
 *                  may use.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *     BT_STATUS_FAILED - the operation failed.
 *
 */

#if defined (__BT_2_1_SNIFF_SUBRATING__)
BtStatus ME_StartSniffSubrating(BtRemoteDevice *remDev);
#endif 
/*---------------------------------------------------------------------------
 * ME_StartPark()
 *
 *     Start park mode for the ACL link specified by "remDev".
 *
 * Parameters:
 *     rm - pointer to remote device
 *
 *     max - acceptable longest period between beacons. The time
 *         is calculated as max * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *
 *     min - acceptable shortest period between beacons. The time
 *         is calculated as min * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *         The parameter min should not be greater than max,
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed. 
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StartPark(BtRemoteDevice *remDev, U16 max, U16 min);

/*---------------------------------------------------------------------------
 * ME_StopPark()
 *
 *     Stop park mode and enter active mode for the ACL link
 *     specified by "remDev". 
 *          
 * Parameters:
 *     remDev - pointer to remote device.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed because the link is
 *         not in park.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StopPark(BtRemoteDevice *remDev);

/*---------------------------------------------------------------------------
 * ME_StartParkAll()
 *
 *     Start park mode for all ACL links.
 *
 * Parameters:
 *     max - acceptable longest period between beacons. The time
 *         is calculated as max * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *
 *     min - acceptable shortest period between beacons. The time
 *         is calculated as min * 0.625 ms. Range is 0.625ms to 40.9 sec.
 *         The parameter min should not be greater than max,
 *
 *     num - pointer to an integer that receives the number of devices
 *         that were requested to enter park mode. This count can be
 *         used in conjunction with BTEVENT_MODE_CHANGE events to determine
 *         when all devices are parked.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote devices 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed. 
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StartParkAll(U16 max, U16 min, U8 *num);

/*---------------------------------------------------------------------------
 * ME_StopParkAll()
 *
 *     Return all parked ACL links to active mode.
 *          
 * Parameters:
 *     num - pointer to an integer that receives the number of devices
 *         that were requested to exit park mode. This count can be
 *         used in conjunction with BTEVENT_MODE_CHANGE events to determine
 *         when all devices are unparked.
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_MODE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the mode 
 *         change event. The "p.modeChange" field indicates for which remote 
 *         Device the change has occurred along with the new mode and 
 *         interval. It is possible that link is disconnected before the
 *         mode change has occurred. In that case the handlers will not
 *         receive BTEVENT_MODE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed because the link is
 *         not in park.
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_StopParkAll(U8 *num);

/*---------------------------------------------------------------------------
 * ME_SwitchRole()
 *
 *     Switch the current role the device is performing for the ACL link
 *     specified by remDev. If the current role is slave then switch to
 *     master. If the current role is master then switch to slave. The
 *     current role can be found via remDev.role. The result of the 
 *     operation will be returned via the BTEVENT_ROLE_CHANGE event.
 *
 * Parameters:
 *     remDev - pointer to remote device
 *
 * Returns:
 *     BT_STATUS_PENDING - the operation was started successfully.
 *         All registered handlers and handlers bound to the remote device 
 *         will receive the BTEVENT_ROLE_CHANGE event. The "errCode" field 
 *         of the BtEvent will indicate the success or failure of the role 
 *         change event. The "p.roleChange" field indicates for which remote 
 *         Device the change has occurred along with the new role.  It is 
 *         possible that link is disconnected before the role change has 
 *         occurred. In that case the handlers will not receive 
 *         BTEVENT_ROLE_CHANGE but instead will receive
 *         BTEVENT_LINK_DISCONNECT.
 *
 *     BT_STATUS_INVALID_PARM - the parameters are not valid
 *         (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_IN_PROGRESS - the operation failed because a mode
 *         change or disconnect operation is already in progress.
 *
 *     BT_STATUS_FAILED - the operation failed. 
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_SwitchRole(BtRemoteDevice *remDev);

/*---------------------------------------------------------------------------
 * ME_SetAccessibleModeNC()
 *
 *     Set the accessibility mode when the device is not 
 *     connected. If the mode is set to a value other than 
 *     BAM_NOT_ACCESSIBLE and there are no connections then the 
 *     Bluetooth radio will enter inquiry and/or page scan mode 
 *     using the information passed in info. If info is 0 or the
 *     values in info are set to defaults (BT_DEFAULT_SCAN_INTERVAL
 *     and BT_DEFAULT_SCAN_WINDOW) the radio module default values
 *     are used. It is assumed that the macro defaults
 *     match the radio defaults (see BT_DEFAULT_PAGE_SCAN_WINDOW
 *     documentation.) So, the first call to ME_SetAccessibleModeNC
 *     with info set to 0 will not change the settings as the radio has
 *     already been initialized to its default settings. If there is
 *     a connection or a connection is in the process  of being created
 *     then mode and info are saved and applied when all connections are 
 *     disconnected.  
 *
 *     To keep other devices from finding and connecting to this 
 *     device set the mode to BAM_NOT_ACCESSIBLE. The default mode when
 *     the stack is first loaded and initialized is controlled by
 *     BT_DEFAULT_ACCESS_MODE_NC.
 *
 *     Any time the scan interval or window is different from 
 *     the current settings in the radio, the radio will be 
 *     instructed to change to the new settings. This means that 
 *     if there are different settings for the connected state
 *     versus the non-connected state, the radio module will be 
 *     instructed to change the settings when the first connection 
 *     comes up and when the last connection goes down
 *     (automatically). This also means that if different values
 *     for window and interval are set when going from any setting
 *     of accessible to non-accessible then the radio will be 
 *     instructed to change. In most cases it is best to use
 *     the radio defaults. In this way the radio is never told
 *     to change the scan interval or window.
 *
 * Parameters:
 *     mode - desired accessibility mode
 *
 *
 * Returns:
 *     BT_STATUS_PENDING - the mode is being set. All registered 
 *         global handlers with the BEM_ACCESSIBLE_CHANGE mask set will 
 *         receive BTEVENT_ACCESSIBLE_CHANGE event when the mode change 
 *         actually takes affect or an error has occurred. The "errCode"
 *         field of the BtEvent indicates the status of the operation. 
 *         If the operation is successful the "aMode" field of BtEvent
 *         indicates the new mode.  A BTEVENT_HCI_FATAL_ERROR event
 *         indicates a fatal radio or HCI transport error and that all
 *         pending operations have failed.
 *
 *     BT_STATUS_SUCCESS - Accessible mode is set. No event
 *         is sent out. This is returned if a connection exists and 
 *         the values are only saved or info already matches the current
 *         setting.
 *
 *     BT_STATUS_IN_PROGRESS - operation failed because a change
 *         is already in progress. Monitor the global events to 
 *         determine when the change has taken place.
 *
 *     BT_STATUS_INVALID_PARM - operation failed. The mode or info 
 *         parameter contains bad values (XA_ERROR_CHECK only)
 *
 *     BT_STATUS_HCI_INIT_ERR - operation failed because the HCI has
 *         an initialization error. Monitor the global events to
 *         be notified when the error has been corrected.
 */
BtStatus ME_SetAccessibleModeNC(BtAccessibleMode mode);

/*---------------------------------------------------------------------------
 * ME_GetAccessibleModeNC()
 *
 *     Return the current accessibility mode that is used when no
 *     connections exist. This is not necessarily the current mode.
 *
 * Parameters:
 *     mode - pointer to memory to receive accessibility mode. If
 *         mode is 0 then mode is not returned. If the accessible mode
 *         has not yet been set, and therefore unknown, 0xff will be returned.
 *
 *
 * Returns:
 *     BT_STATUS_SUCCESS - operation was successful
 *
 *     BT_STATUS_IN_PROGRESS - operation failed because a change
 *         is in progress. Monitor the global events to determine when
 *         the change is finished.
 *
 *     BT_STATUS_FAILED - operation failed.
 */
BtStatus ME_GetAccessibleModeNC(BtAccessibleMode *mode);

/*---------------------------------------------------------------------------
 * ME_GetCurAccessibleMode()
 *
 *     Return the current accessibility mode. This is the mode
 *     at the very instant that call is made. It may be about
 *     to change so it is important to check the global events.
 *
 * Parameters:
 *     mode - pointer to memory to receive accessibility mode. If
 *         mode is 0 then mode is not returned. If the accessible mode
 *         has not yet been set, and therefore unknown, 0xff will be returned.
 *
 *
 * Returns:
 *     BT_STATUS_SUCCESS - operation was successful
 *
 *     BT_STATUS_FAILED - operation failed.
 */
BtStatus ME_GetCurAccessibleMode(BtAccessibleMode *mode);

/*---------------------------------------------------------------------------
 * ME_SetDefaultLinkPolicy()
 *
 *     Set the default link policy used on outgoing and incoming ACL 
 *     connections. The link policy determines the behavior of the local
 *     link manager when it receives a request from a remote device or it
 *     determines itself to change the master-slave role or to enter hold,
 *     sniff, or park mode. The default if this function is not called 
 *     is to disable all modes.
 *
 *     Policy for incoming ACL connections is set independent of policy
 *     for outgoing connections. The default policy is applied immediately 
 *     after the link comes up. An higher level Management Entity should
 *     use this function, not applications. It should be called before any
 *     ACL connections exist and does not effect existing
 *     ACL connections.
 *
 * Parameters:
 *     inACL - default link policy applied to all incoming ACL connections.
 *         Incoming ACL connections are those initiated by a remote device.
 *
 *     outACL - default link policy applied to all outgoing ACL connections.
 *         Outgoing ACL connections are those initiated by the local device.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - operation was successful.
 *
 *     BT_STATUS_FAILED - operation failed because the policy settings are
 *         not valid (error checking only).
 */
BtStatus ME_SetDefaultLinkPolicy(BtLinkPolicy inACL, BtLinkPolicy outACL);

void MeEnabledLocalLoopback(void);

/*---------------------------------------------------------------------------
 * ME_GetHciConnectionHandle()
 *
 *     Get the HCI connection handle for the link to the remote device. The
 *     value is meaningless if a connection does not exist to the remote 
 *     device.
 *
 * Parameters:
 *     rm - pointer to remote device.
 *
 * Returns:
 *     The HCI connection handle a link exists otherwise the value
 *     is meaningless.
 */
U16 ME_GetHciConnectionHandle(BtRemoteDevice *rm);

/*---------------------------------------------------------------------------
 * ME_GetBdAddr()
 *
 *     Get the 48-bit address of the remote device. The value is meaningless 
 *     if a connection does not exist to the remote  device.
 *
 * Parameters:
 *     rm - pointer to remote device.
 *
 *     bdAddr - pointer to a BD_ADDR structure to receive the result.
 */
void ME_GetBdAddr(BtRemoteDevice *rm, BD_ADDR *bdAddr);

/*---------------------------------------------------------------------------
 * ME_GetRemDevState()
 *
 *     Get the connection state of the remote device. This function
 *     does not check for a valid remDev pointer.
 *
 * Parameters:
 *     remDev - pointer to remote device.
 *
 * Returns:
 *     BtRemDevState
 */
BtRemDevState ME_GetRemDevState(BtRemoteDevice *remDev);

#define ME_GetRemDevState(r) ((r)->state)

/*---------------------------------------------------------------------------
 * ME_GetRemDevCOD()
 *
 *     Get the Class of Device for the remote device. This function
 *     does not check for a valid remDev pointer.
 *
 * Parameters:
 *     remDev - pointer to remDev.
 *
 * Returns:
 *     BtClassOfDevice
 */
BtClassOfDevice ME_GetRemDevCOD(BtRemoteDevice *remDev);

#define ME_GetRemDevCOD(r) ((r)->cod)

/*---------------------------------------------------------------------------
 * ME_GetRemDevEncryptState()
 *
 *     Get the encryption state of the remote device. This function
 *     does not check for a valid remDev pointer.
 *
 * Parameters:
 *     remDev - pointer to remDev.
 *
 * Returns:
 *     Zero for no encryption, non-zero for encryption.
 */
BtEncryptState ME_GetRemDevEncryptState(BtRemoteDevice *remDev);

#define ME_GetRemDevEncryptState(r) ((r)->encryptState)

/*---------------------------------------------------------------------------
 * ME_GetStackInitState()
 *
 *     Reads the initialization state of the stack.
 *
 * Returns:
 *     BtStackState - If the value is BTSS_INITIALIZED the stack is 
 *         initialized and ready.
 */
BtStackState ME_GetStackInitState(void);

/*---------------------------------------------------------------------------
 * ME_GetBtVersion()
 *
 *     Gets the Bluetooth version of the local radio.
 *
 * Returns:
 *     0 for BT v1.0.
 *     1 for BT v1.1.
 *     2 for BT v1.2.
 *     3 for BT v2.0.
 */
U8 ME_GetBtVersion(void);

/*---------------------------------------------------------------------------
 * ME_GetBtFeatures()
 *
 *     Gets the value of the specified byte of the Bluetooth features
 *     bitmask from the local radio. See the Bluetooth specification for
 *     a description of the features bitmask.
 *
 * Parameters:
 *     byte - byte to retrieve. Must be between 0 and 7.
 *
 * Returns:
 *     Value of the specified byte of the BT features bitmask.
 */
U8 ME_GetBtFeatures(U8 byte);

/*---------------------------------------------------------------------------
 * ME_HasPendingTask()
 *
 *     Checks for pending ME tasks. The ME task is set when an HCI command
 *     is sent to the controller. ME_HasPendingTask will return the event
 *     expected to complete the pending HCI command.
 *
 * Returns:
 *     Pending HCI event associated with the completion of the current
 *     task or 0 for no pending tasks. See HciEventType in hci.h for possible
 *     return values. If not enough information is available to determine the
 *     completion event, ME_HasPendingTask returns 0xFF00.
 */
U16 ME_HasPendingTask(void);

/*---------------------------------------------------------------------------
 * ME_GetChannelClassification()
 *
 *     Gets the channel classification currently stored in ME for Adaptive
 *     Frequency Hopping (AFH.) See ME_SetChannelClassification.
 *
 * Parameters:
 *     channelClass - BtChannelClass structure containing buffer to receive
 *         the stored channel classification.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The current channel classification has been
 *         copied to the BtChannelClass structure.
 */
BtStatus ME_GetChannelClassification(BtChannelClass *channelClass);

/*---------------------------------------------------------------------------
 * ME_SetChannelClassification()
 *
 *     Sets the channel classification for Adaptive Frequency Hopping (AFH.)
 *     Each of the 79 1-MHz channels in the Bluetooth 2.4-GHz band may be
 *     marked bad or unknown. The AFH controller will not use channels
 *     marked bad. The AFH controller will evaluate for use any channels
 *     marked unknown (if automatic classification mode is enabled.) Bad
 *     channels are indicated with a 0 value. Unknown channels are
 *     indicated with a 1 value in the bit-mask.
 *
 * Parameters:
 *     channelClass - 79 1-bit fields in a 10-byte array, indicating channels
 *         marked bad or unknown. The nth field (0 - 78) indicates the value
 *         for channel n. Bit 79 is unused and must be 0.
 *
 *     autoMode - Boolean value indicating whether the controller should
 *         automatically determine channel classification for the channels
 *         marked unknown.
 *     
 * Returns:
 *     BT_STATUS_SUCCESS - the channel classification has been sent to the
 *         controller.
 *
 *     BT_STATUS_IN_PROGRESS - another ME_SetChannelClassification operation
 *         is currently in progress. Try again later.
 *
 *     BT_STATUS_INVALID_PARM - The parameters are not valid
 *         (XA_ERROR_CHECK only).
 */
BtStatus ME_SetChannelClassification(BtChannelClass *channelClass, BOOL autoMode);

void MeChangePacketType(BtRemoteDevice *remDev);

void MeReadRemoteSupportedFeature(BtRemoteDevice *remDev);

void MeReadRemoteVersion(BtRemoteDevice *remDev);

/*---------------------------------------------------------------------------
 * ME_MarkAfhChannels()
 *
 *     Sets or clears the bits associated with the indicated range of
 *     channels in the BtChannelClass map. If the "initialize" parameter is
 *     TRUE, also initializes the BtChannelClass map to the "Unknown" state.
 *
 * Parameters:
 *     channelClass - BtChannelClass structure containing buffer to be
 *         modified.
 *
 *     begin - the first channel (0 - 78) in the range to set.
 *
 *     count - the number of channels to set starting at "begin".
 *
 *     state - If TRUE, the Bluetooth channel will be set to the "Unknown"
 *         state. If FALSE, the Bluetooth channel will be set to the "Bad"
 *         state.
 *
 *     initialize - If TRUE, the BtChannelClass structure will be initialized
 *         to the "Unknown" state. If FALSE, the structure will be modified
 *         as is.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The channels have been marked successfully.
 */
BtStatus ME_MarkAfhChannels(BtChannelClass *channelClass, U8 begin, U8 count, BOOL state, BOOL initialize);

/*---------------------------------------------------------------------------
 * ME_MarkWiFiChannel()
 *
 *     Sets or clears the bits associated with the indicated WiFi channel in
 *     the BtChannelClass map. If the "initialize" parameter is TRUE, also
 *     initializes the BtChannelClass map to the "Unknown" state.
 *
 * Parameters:
 *     channelClass - BtChannelClass structure containing buffer to be
 *         modified.
 *
 *     wifiChannel - WiFi channel (1-14) to set.
 *
 *     state - If TRUE, the WiFi channel will be set to the "Unknown"
 *         state. If FALSE, the WiFi channel will be set to the "Bad"
 *         state.
 *
 *     initialize - If TRUE, the BtChannelClass structure will be initialized
 *         to the "Unknown" state and then set as indicated. If FALSE, the
 *         structure will be modified as is.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The channels have been marked successfully.
 */
BtStatus ME_MarkWiFiChannel(BtChannelClass *channelClass, U8 wifiChannel, BOOL state, BOOL initialize);

/*---------------------------------------------------------------------------
 * ME_RadioInit()
 *
 *     Called to initialize the radio module, including the HCI 
 *     transport drivers, the HCI, and the radio module.  This function is
 *     called by the Event Manager in EVM_Init() when the stack initialization 
 *     sequence begins, but can be called by applications afterwards if 
 *     necessary (ME_RadioShutdown() has been called).  When initialization 
 *     is successful, the BTEVENT_HCI_INITIALIZED event will be received. 
 *
 * Parameters:  void
 *
 * Returns:
 *     BT_STATUS_SUCCESS - Initialization was successful.  The application will
 *         be notified during the call that initialization is complete with
 *         BTEVENT_HCI_INITIALIZED.

 *     BT_STATUS_PENDING - Initialization was started successfully.  The
 *         application will be notified when initialization is complete.  If 
 *         initialization is successful, BTEVENT_HCI_INTIALIZED will be 
 *         received.  If initialization fails, BTEVENT_HCI_INIT_ERROR will be 
 *         received.  Initialization will be retried BT_HCI_NUM_INIT_RETRIES
 *         times when an error occurs.  After initialization has been retried 
 *         and has failed the maximum number of times, BTEVENT_HCI_FAILED will
 *         be received.

 *     BT_STATUS_FAILED - Initialization failed to start.
 */
BtStatus ME_RadioInit(void);

#define ME_RadioInit() RMGR_RadioInit()

/*---------------------------------------------------------------------------
 * ME_RadioShutdown()
 *
 *     Gracefully shuts down the radio module, including the transports and 
 *     the HCI.  ME_RadioShutdown will wait 1 second for pending operations
 *     before shutting down.  (Note: Use ME_RadioShutdownTime if you need to
 *     specify the wait time.)  When the shutdown is complete, the global
 *     handler will receive a BTEVENT_HCI_DEINITIALIZED event.
 *
 *     This function must not be called during a callback.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The radio is down. The application will be notified
 *         during the call that the shutdown was complete with 
 *         BTEVENT_HCI_DEINITIALIZED.
 *     BT_STATUS_FAILED - The radio is already down.
 *     BT_STATUS_PENDING - The radio will be shut down. The application will 
 *         be notified when shutdown is complete with BTEVENT_HCI_DEINITIALIZED.
 */
BtStatus ME_RadioShutdown(void);
BtStatus ME_RadioShutdownTime(TimeT time);

#define ME_RadioShutdown() ME_RadioShutdownTime(1000)

void Me_ChangeLinkPolicy(BtRemoteDevice *remDev, BtEnableOption roleSwitch, BtEnableOption sniff);

#endif /* __ME_H */ 

