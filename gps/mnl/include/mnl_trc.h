/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2007
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*  bee_trc.h
*
* Project:
* --------
*  MAUI
*
* Description:
* ------------
*  BEE trace goes here
*
* Author:
* -------
 * -------
*
*============================================================================
*             HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
 * removed!
*
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!

*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*============================================================================
*******************************************************************************/
#ifndef __MNL_TRC_H__
#define __MNL_TRC_H__
#if defined(__MNL_SUPPORT__) 

#include "stack_config.h"
#include "kal_trace.h"

#ifndef __GNUC__
#define MNL_TRACE(args) kal_trace args
#else
#define MNL_TRACE(args)  NULL    
#endif


#if !defined(GEN_FOR_PC)
#if defined(__TST_MODULE__) || defined(__CUSTOM_RELEASE__)
    #include "mnl_trc_gen.h"
#endif /* TST Trace Defintion */
#endif
BEGIN_TRACE_MAP(MOD_MNL)

   /***************TRC MSG FOR GPS BEGIN ********************/
   /* --------------------------------------------------------------- */
   /* -----------------------   COMMON USE ID    -------------------- */
   /* --------------------------------------------------------------- */

    /* --------------------------------------------------------------- */
    /* ----------------------- MODULE NAME:GPS_MNL------------------- */
    /* --------------------------------------------------------------- */
    TRC_MSG(GPS_MNL_TRC_MNL_TASK_INIT_OK, "[GPS_MNL] MNL Task Init OK !!!")    
    TRC_MSG(GPS_MNL_TRC_MTK_GPS_TIMER_ID, "[GPS_MNL] timer_id=%d")
    TRC_MSG(GPS_MNL_TRC_HOST_OWN_MSG, "[GPS_MNL] HOST OWN TYPE = %d")
    TRC_MSG(GPS_MNL_TRC_FRAME_SYNC_MEASURE_FAIL, "[GPS_MNL] Fail to complete frame sync measurement !!!")
    TRC_MSG(GPS_MNL_TRC_DISCARD_RESTART_CMD, "[GPS_MNL] Discard the continuous restart command !!!")
    TRC_MSG(GPS_MNL_TRC_DSP_IN_BUFFER_FULL, "[GPS_MNL] Warning.DSP in-buffer is full !!!")    
    TRC_MSG(GPS_MNL_TRC_MNL_RING_BUFF_OVER_FLOW, "[GPS_MNL]g_mnl_ring_buf overflow !!!")

    /* --------------------------------------------------------------- */
    /* ----------------------- INF & WRN & ERR Log ------------------- */
    /* --------------------------------------------------------------- */

    TRC_MSG(MNL_TRC_INF_VIRTUAL_UART_INIT_OK, "[INF]virtual uart init OK!")
    TRC_MSG(MNL_TRC_INF_UART_SET_BAUDRATE, "[INF][UART]set baudrate=%d")
    TRC_MSG(MNL_TRC_INF_UART_NMEA_POLL,    "[INF][UART]nmea poll")
    TRC_MSG(MNL_TRC_INF_UART_RTCM_POLL,    "[INF][UART]rtcm poll")

    TRC_MSG(MNL_TRC_INF_RESTART_BEGIN, "[INF][Restart]begin")
    TRC_MSG(MNL_TRC_INF_RESTART_OK,    "[INF][Restart]OK")
    TRC_MSG(MNL_TRC_WRN_RESTART_ERR,   "[WRN][Restart]ERR!")

    TRC_MSG(MNL_TRC_WRN_FILE_GET_DIRVE_FAIL,   "[WRN][File]get drive failed!")
    TRC_MSG(MNL_TRC_WRN_FILE_WRITE_NVFILE_FAIL,"[WRN][File]write NVFile failed!")
    TRC_MSG(MNL_TRC_INF_FILE_OPEN_NVFILE,      "[INF][File]open NVFile %c:\\MTKGPS.dat, handle=%d, mode=%x")
    TRC_MSG(MNL_TRC_INF_FILE_CLOSE_NVFILE,     "[INF][File]close NVFile handle=%d")
    TRC_MSG(MNL_TRC_INF_FILE_DEL_NVFILE,       "[INF][File]del NVFile %c:\\MTKGPS.dat")
  
   /***************TRC MSG FOR GPS END ********************/
END_TRACE_MAP(MOD_MNL)
#endif /* __MNL_SUPPORT__*/
#endif /* __MNL_TRC_H__ */
