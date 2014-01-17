#ifndef __OSAPI_H
#define __OSAPI_H

/****************************************************************************
 *
 * File:
 *     $Workfile:osapi.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:64$
 *
 * Description:
 *     Defines the APIs used by the stack to access system services.
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
/***************************************************************************** 
* Include
*****************************************************************************/
/* basic system service headers */
#include "xatypes.h"
#include "btdrv_types.h"
/*---------------------------------------------------------------------------
 * Operating System API layer
 *
 *     The OS API is a set of functions used by the stack to access
 *     system services. If you are porting to a new operating system,
 *     you must provide the functions described here.
 *
 *     The functions in this API are intended for exclusive use by the
 *     stack. Applications should use the native operating system interface
 *     and avoid making OS_ calls themselves.
 */

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
#endif
/* typedef unsigned long DWORD; */


typedef void (*OsTimerNotify) (void);


/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * OS_Init()
 *
 *     Initializes the stack and operating system layer. This function is
 *     not called by the stack, but must be called by your operating system
 *     or application to set up the stack and its operating system services.
 * 
 *     The OS_Init function must perform the following tasks in this order:
 * 
 *     1) Seed the random number generator with a random value such as
 *     the current system time.
 * 
 *     2) Any other operating-system specific functions, such as timer
 *     initialization or the creation of semaphores.
 * 
 *     3) Call EVM_Init to initialize the Event Manager. This also
 *     initializes all stack layers. Note that this function may
 *     return FALSE if stack initialization failed for some reason.
 * 
 *     4) Create and start the stack task. This task calls EVM_Process
 *     at least once for every call to OS_NotifyEvm. See OS_NotifyEvm
 *     for more details.
 * 
 * Returns:
 *     TRUE - Success.
 *
 *     FALSE - Initialization failed for some reason.
 */
BOOL OS_Init(void);

/*---------------------------------------------------------------------------
 * OS_Deinit()
 *
 *     Deinitializes the stack and operating system layer. This function
 *     is not called by the stack, but may be called by your operating
 *     system or application to shut down the stack and free its resources.
 */
void OS_Deinit(void);

/*---------------------------------------------------------------------------
 * OS_GetSystemTime()
 *
 *     Called by the stack to get the current system time in ticks.
 *
 *     The system time provided by this function can start at any value;
 *     it does not to start at 0. However, the time must "roll over" only
 *     when reaching the maximum value allowed by TimeT. For instance, a
 *     16-bit TimeT must roll over at 0xFFFF. A 32-bit TimeT must roll
 *     over at 0xFFFFFFFF.
 *
 *     System ticks may or may not be equivalent to milliseconds. See the
 *     MS_TO_TICKS macro in config.h (General Configuration Constants)
 *     for more information.
 *
 * Returns:
 *     The current time in ticks.
 */
TimeT OS_GetSystemTime(void);

/*---------------------------------------------------------------------------
 * OS_Rand()
 *
 *     Called by the stack to generate a random number between
 *     0x0000 and 0xFFFF.
 *
 * Returns:
 *     A 16-bit random number.
 */
U16 OS_Rand(void);


/*---------------------------------------------------------------------------
 * OS_MemCopy()
 *
 *     Called by the stack to copy memory from one buffer to another.
 *     
 *     This function's implementation could use the ANSI memcpy function.
 *
 * Parameters:
 *     dest - Destination buffer for data.
 *
 *     source - Source buffer for data. "dest" and "source" must not
 *         overlap.
 *
 *     numBytes - Number of bytes to copy from "source" to "dest".
 */
void OS_MemCopy(U8 *dest, const U8 *source, U32 numBytes);

BOOL OS_MemCmp(const U8 *buffer1, U16 len1, const U8 *buffer2, U16 len2);

/*---------------------------------------------------------------------------
 * OS_MemSet()
 *
 *     Fills the destination buffer with the specified byte.
 *
 *     This function's implementation could use the ANSI memset
 *     function.
 *
 * Parameters:
 *     dest - Buffer to fill.
 *
 *     byte - Byte to fill with.
 *
 *     len - Length of the destination buffer.
 */
void OS_MemSet(U8 *dest, U8 byte, U32 len);

/*---------------------------------------------------------------------------
 * OS_StartTimer()
 *
 *     Called by the stack to start the event timer. When the time
 *     expires, it is the system's responsibility to call the notify
 *     function provided. The notify function may be called in either
 *     interrupt or task modes as required by the operating system.
 *
 *     If the timer is already active, OS_StartTimer automatically
 *     cancels the previous timer as if OS_CancelTimer was called.
 *
 *     In non-multitasking systems (where XA_MULTITASKING is disabled), this
 *     function is not used. EVM_Process will call OS_GetSystemTime
 *     frequently to determine if its internal timers have elapsed.
 *
 * Requires:
 *     XA_MULTITASKING enabled.
 *
 * Parameters:
 *     time - number of ticks until the timer fires
 * 
 *     func - The function to call when the timer expires.
 *
 */
void OS_StartTimer(TimeT time, OsTimerNotify func);

/*---------------------------------------------------------------------------
 * OS_CancelTimer()
 *
 *     Called by the stack to stops the event timer. This must prevent
 *     the operating system from calling the timer notification
 *     function provided by OS_StartTimer.
 *
 *     If function is called when the event timer is not running, do
 *     nothing.
 *
 *     In non-multitasking systems (where XA_MULTITASKING is disabled), this
 *     function is not used.
 *
 * Requires:
 *     XA_MULTITASKING enabled.
 */
void OS_CancelTimer(void);
void OS_ResetTimerId(void);
void OS_ResetInd(void);

/*---------------------------------------------------------------------------
 * OS_NotifyEvm()
 *
 *     Called by the stack to indicate that EVM_Process should be called.
 *     Depending on your operating system, this call may be made in
 *     interrupt mode or task mode.
 *
 *     In multitasking systems, the stack task alternately calls EVM_Process
 *     and blocks to save CPU time. The block may be implemented by an
 *     event semaphore or similar object. OS_NotifyEvm should post the
 *     semaphore, or otherwise cause the stack task to unblock and
 *     call EVM_Process.
 *
 *     In non-multitasking systems (where XA_MULTITASKING is disabled),
 *     tasks are usually simulated using a round-robin execution scheme. In
 *     this case, EVM_Process is called continuously, and no task ever
 *     blocks. Therefore, this function is not used.
 *
 *     If OS_NotifyEvm is called during EVM_Process execution,
 *     EVM_Process must be executed again before the stack task blocks.
 *
 * Requires:
 *     XA_MULTITASKING enabled.
 */
void OS_NotifyEvm(void);

#define OS_LockStack()   (void)0
#define OS_UnlockStack() (void)0
#define OS_StopHardware() (void)0
#define OS_ResumeHardware() (void)0



/*---------------------------------------------------------------------------
 * OS_StrCmp()
 *
 *     Compares two strings for equality.
 *
 * Parameters:
 *     Str1 - String to compare.
 *     Str2 - String to compare.
 *
 * Returns:
 *     Zero - If strings match.
 *     Non-Zero - If strings do not match.
 */
U8 OS_StrCmp(const char *Str1, const char *Str2);

/*---------------------------------------------------------------------------
 * OS_StrLen()
 *
 *     Calculate the length of the string.
 *
 * Parameters:
 *     Str - String to count length.
 *
 * Returns:
 *     Returns length of string.
 */
U16 OS_StrLen(const char *Str);

void MTK_BT_Assert(void);

//Move to \mcu\interface\hal\bluetooth\btdrv_types.h
#if 0
#ifndef __BT_SHRINK_SIZE__ 
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(DEBUG_KAL)
/* under construction !*/
#else 
/* under construction !*/
#endif 
/* under construction !*/
/* under construction !*/
#else /* __BT_SHRINK_SIZE__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* __BT_SHRINK_SIZE__ */
/* under construction !*/
#endif

/*---------------------------------------------------------------------------
 * OS_Report()
 *
 *     Called by the stack to report debugging information.
 *
 * Requires:
 *     XA_DEBUG_PRINT enabled.
 *
 * Parameters:
 *     format - A string containing the failed expression.
 *
 *     ... - printf style arguments.
 */
void OS_Report(const char *format, ...);

#define DebugPrint(s) OS_Report s
#define Report(s) OS_Report s

#endif /* __OSAPI_H */ 

