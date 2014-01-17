/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mtk_gps_misc.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements porting layer misc functions
 *
 * Author:
 * -------
 * 
 *
 ****************************************************************************/
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"
#include "task_config.h"
#include "stack_ltlcom.h"
#include "intrCtrl.h"
#include "dcl.h"
#include "kal_trace.h"

#include "mtk_gps.h" //add by jiawang 20110824 mtk_gps_sys_time_read()
#include "mtk_gps_task.h"
#include "mtk_gps_misc.h"


#include "mtk_gps_type.h"
#include "stack_config.h"
#include "drv_comm.h"
#include "l1sm_public.h"   
#include "kal_public_defs.h"
#include "kal_general_types.h"
#include "kal_public_api.h"
#include "l1sm_public.h"
#include "mnl_trc.h"
#include "..\..\inc\Gps_uart.h"

extern KAL_ADM_ID gps_adm_pool_id;
extern kal_mutexid gps_adm_pool_mutex;
extern mtk_gps_task_struct *mtk_gps_context_p;
extern kal_uint8 gps_uart_port;
extern kal_uint8 gps_frame_sync_sleep_handle;

#if defined(__L1_GPS_AUTO_TIMING_SYNC_SUPPORT__) || defined(__L1_GPS_REF_TIME_SUPPORT__)  
extern void gps_frame_sync_meas_req(mtk_gps_frame_sync_type_enum application_type); 
#endif

extern void gps_uart_rx_sleep_handler(void);
extern void gps_host_wake_up_ind_handler(void);
extern void mtk_gps_timer_expiry(ilm_struct *ilm_ptr);
extern void GPS_MNL_Init(void);
extern void GPS_MNL_Shutdown(void);
extern mtk_int32 mtk_gps_sys_nmea_poll(void);
extern mtk_int32 mtk_gps_sys_uart_poll(void);

extern void L1D_Raise_FrameSync(void);
kal_uint8 enable_mnl_debug = 1;
mtk_bool gps_frame_sync_flag = MTK_GPS_TRUE;
mtk_bool gps_first_power_on_flag = MTK_GPS_TRUE;
mtk_bool g_gps_frame_sync_meas_req_by_ftime;
mtk_bool g_gps_frame_sync_meas_req_by_utran;
kal_bool g_gps_frame_sync_maintain_phase;
/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_task_sleep
 * DESCRIPTION
 *
 * PARAMETERS
 *  milliseconds [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_sys_task_sleep (mtk_uint32 milliseconds)
{
  // 4.615ms per tick
  kal_sleep_task(milliseconds * 13 / 60 + 1);
 }

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_msg_recv
 * DESCRIPTION
 *
 * PARAMETERS
 *  msg         [OUT]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_msg_recv (mtk_gps_msg **msg)
{
#define MSG_SYSTEM  0
#define MSG_OWN     1
    ilm_struct current_ilm;
    extern task_entry_struct *g_mnl_task_entry_ptr;
    kal_uint32 my_index;
    kal_int32 type = MSG_SYSTEM;  //default type; we break the do-while loop until MSG_OWN
    
    kal_get_my_task_index(&my_index);
    do
    {
        receive_msg_ext_q(task_info_g[g_mnl_task_entry_ptr->task_indx].task_ext_qid, &current_ilm);
        stack_set_active_module_id(my_index, current_ilm.dest_mod_id);
        switch (current_ilm.msg_id)
        {
            /* MSG from system */
            case MSG_ID_UART_READY_TO_READ_IND:            	 
                gps_uart_rx_sleep_handler();
                if (MNL_RUNNING == mtk_gps_context_p->gps_state)
                {
                    mtk_gps_sys_uart_poll();  // Read the UART data into GPS lib
                }
                break;

            case MSG_ID_TIMER_EXPIRY:
            	mtk_gps_timer_expiry(&current_ilm);
                break;

            case MSG_ID_MNL_HOST_WAKE_UP_IND:
                gps_host_wake_up_ind_handler();
                break;

            case MSG_ID_GPS_MNL_INIT_REQ:                 
                GPS_MNL_Init();
                break;

            case MSG_ID_GPS_MNL_SHUTDOWN_REQ:                
                GPS_MNL_Shutdown();
                break;

            case MSG_ID_MNL_HOST_OWN: //MSG from ourself
            	if(((*msg) = ((mtk_gps_msg_struct *)(current_ilm.local_para_ptr))->msg) != NULL)
                {
                    type = MSG_OWN;
                    MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_HOST_OWN_MSG,(*msg)->type));
                }                
                break;
                
            default:
            	break;
        }
        free_ilm(&current_ilm);
    }while(type == MSG_SYSTEM);
    
    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_msg_send
 * DESCRIPTION
 *
 * PARAMETERS
 *  msg         [IN]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_msg_send (const mtk_gps_msg* msg)
{
    ilm_struct  *mtk_gps_ilm;
    mtk_gps_msg_struct  *local_param;   
    module_type module_id;   

    if (KAL_TRUE == kal_if_hisr())
    {
        module_id = MOD_DRV_HISR;
    }
    else
    {
        module_id = stack_get_active_module_id();
    }

    /* [klocwork]If module_id > RPS_TOTAL_STACK_MODULES, 
     * Out-of-bound will occur in allocate_ilm which will be called in DRV_BuildPrimitive */
    if(module_id > RPS_TOTAL_STACK_MODULES) return MTK_GPS_SUCCESS;

    local_param = (mtk_gps_msg_struct *)construct_local_para(sizeof(mtk_gps_msg_struct),TD_UL);
    local_param->msg = (mtk_gps_msg*) msg;
    DRV_BuildPrimitive(mtk_gps_ilm, module_id, MOD_MNL, MSG_ID_MNL_HOST_OWN, local_param);
    msg_send_ext_queue(mtk_gps_ilm);

    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_msg_alloc
 * DESCRIPTION
 *
 * RETURNS
 *
 *****************************************************************************/
mtk_gps_msg *
mtk_gps_sys_msg_alloc (mtk_uint16 size)
{
    void *pAdm = NULL;
    
    kal_take_mutex(gps_adm_pool_mutex);
    pAdm = kal_adm_alloc(gps_adm_pool_id, size);
    kal_give_mutex(gps_adm_pool_mutex);

    ASSERT(pAdm != NULL);

    return (mtk_gps_msg *)pAdm;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_msg_free
 * DESCRIPTION
 *
 * PARAMETERS
 *  msg         [IN]
 * RETURNS
 *  success(0)
 *****************************************************************************/
void
mtk_gps_sys_msg_free (mtk_gps_msg* msg)
{
    kal_take_mutex(gps_adm_pool_mutex);
    kal_adm_free(gps_adm_pool_id, msg);        
    kal_give_mutex(gps_adm_pool_mutex);
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_mem_alloc
 * DESCRIPTION
 *  Allocate a block of memory
 * PARAMETERS
 *  size        [IN]   the length of the whole memory to be allocated
 * RETURNS
 *  On success, return the pointer to the allocated memory
 *  NULL (0) if failed
 *****************************************************************************/
void*
mtk_gps_sys_mem_alloc (mtk_uint32 size)
{
    void *pAdm = NULL;
    
    kal_take_mutex(gps_adm_pool_mutex);
    pAdm = kal_adm_alloc(gps_adm_pool_id, size);
    kal_give_mutex(gps_adm_pool_mutex);

    ASSERT(pAdm != NULL);

    return pAdm;
    //return (void *)kal_adm_alloc(gps_adm_pool_id, size);
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_mem_free
 * DESCRIPTION
 *  Release unused memory
 * PARAMETERS
 *  pmem         [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_sys_mem_free (void* pmem)
{
    kal_take_mutex(gps_adm_pool_mutex);
    kal_adm_free(gps_adm_pool_id, pmem);        
    kal_give_mutex(gps_adm_pool_mutex);
}

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
#if __MTK_TARGET__    
/* under construction !*/
#else    
/* under construction !*/
#endif /* __MTK_TARGET__ */    
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


mtk_int32 mtk_gps_sys_data_output_dbg (char* buffer, mtk_uint32 length)
{
    
    if (enable_mnl_debug == 0)
    {
        return length;
    }
    
#if 1 //Hiki, output debug message to catcher log
    kal_print_string_trace(MOD_MNL, TRACE_FUNC, buffer);    
#endif    

    return (mtk_gps_sys_nmea_output(buffer,length));
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_start_result_handler
 * DESCRIPTION
 *  Handler routine for the result of restart command
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  result         [IN]  the result of restart
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32 
mtk_gps_sys_start_result_handler(mtk_gps_start_result result)
{
    extern void GPS_MNL_Shutdown(void);
    extern kal_mutexid gps_restart_cmd_mutex;
    extern kal_uint8 gps_restart_cmd_exist;

    if (result == MTK_GPS_START_RESULT_ERROR)
    {
        MNL_TRACE((TRACE_WARNING, MNL_TRC_WRN_RESTART_ERR));
        GPS_DEBUG_OUTPUT(GPS_DBG_ERR, "[Restart]Failed!");
        GPS_MNL_Shutdown();
    }
    else
    {
        MNL_TRACE((TRACE_INFO, MNL_TRC_INF_RESTART_OK));
        GPS_DEBUG_OUTPUT(GPS_DBG_INF, "[Restart]OK!");
    }
    kal_take_mutex(gps_restart_cmd_mutex);
    gps_restart_cmd_exist = KAL_FALSE;
    kal_give_mutex(gps_restart_cmd_mutex);

    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_dsp_boot_rx_cb_handler
 * DESCRIPTION
 *  Handler routine for porting layer implementation right before GPS DSP boot up
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  port
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
void 
mtk_gps_sys_dsp_boot_rx_cb_handler(DCL_UINT32 u4Port)
{     
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_dsp_boot_begin_handler
 * DESCRIPTION
 *  Handler routine for porting layer implementation right before GPS DSP boot up
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32 
mtk_gps_sys_dsp_boot_begin_handler(void)
{
#ifdef __MTK_TARGET__
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        DCL_HANDLE handle;
	    UART_CTRL_REG_RX_CB_T data;
	    
	    data.u4OwenrId = MOD_MNL;
	    data.func = mtk_gps_sys_dsp_boot_rx_cb_handler;

	    handle = DclSerialPort_Open(gps_uart_port, 0);
	    DclSerialPort_Control(handle, SIO_CMD_REG_RX_CB, (DCL_CTRL_DATA_T*)&data);
    }
#endif /* __MTK_TARGET__ */
    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_dsp_boot_end_handler
 * DESCRIPTION
 *  Handler routine for porting layer implementation right after GPS DSP boot up
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32 
mtk_gps_sys_dsp_boot_end_handler(void)
{
#ifdef __MTK_TARGET__
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        DCL_HANDLE handle;
	    UART_CTRL_REG_RX_CB_T data;
	    
	    data.u4OwenrId = MOD_MNL;
	    data.func = NULL;

	    handle = DclSerialPort_Open(gps_uart_port, 0);
	    DclSerialPort_Control(handle, SIO_CMD_REG_RX_CB, (DCL_CTRL_DATA_T*)&data);
    }
#endif /* __MTK_TARGET__ */
    return MTK_GPS_SUCCESS;
}


/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_set_frame_sync_flag
 * DESCRIPTION
 * PARAMETERS
 *  frame_sync_flag [IN] frame sync flag
 * RETURNS
 *  void
 *****************************************************************************/
void mtk_gps_sys_set_frame_sync_flag(mtk_bool frame_sync_flag)
{
    gps_frame_sync_flag = frame_sync_flag;
}


/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_meas
 * DESCRIPTION
 * PARAMETERS
 *  pdfFrameTime [OUT] frame time of the issued frame pulse (seconds)
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_frame_sync_meas(mtk_r8 *pdfFrameTime)
{
    mtk_uint32 _savedMask;        
    mtk_uint32 u4FN;
    mtk_uint32 u4Ebit;

    // Get Frame Number and E-bit information
    
    /* To prevent preempted by higher priority ISR */
    _savedMask = SaveAndSetIRQMask();
    u4FN = L1I_GetTimeStamp();
#ifdef __MTK_TARGET__    
    u4Ebit = L1I_GetTimeStampEbits();
#else
    u4Ebit = 0;
#endif /* __MTK_TARGET__ */   
    //L1D_Raise_FrameSync();
    RestoreIRQMask(_savedMask);
    *pdfFrameTime = u4FN * (mtk_r8)(60 / 13.0) * 1E-3 + u4Ebit * (mtk_r8)(60 / 13.0) * 1E-7;
        
    if ((MTK_GPS_TRUE == gps_frame_sync_flag) &&
        (MTK_GPS_TRUE == gps_first_power_on_flag))
    {
        return MTK_GPS_SUCCESS;
    }
    else
    {
        gps_first_power_on_flag = MTK_GPS_TRUE;
        MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_FRAME_SYNC_MEASURE_FAIL));
        return MTK_GPS_ERROR;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_enable_sleep_mode
 * DESCRIPTION
 * PARAMETERS
 *  pdfFrameTime [OUT] frame time of the issued frame pulse (seconds)
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_frame_sync_enable_sleep_mode(mtk_bool mode)
{


    if (mode == MTK_GPS_TRUE)
    {
        L1SM_SleepEnable(gps_frame_sync_sleep_handle);   
    }
    else
    {
        L1SM_SleepDisable(gps_frame_sync_sleep_handle);    
    }

    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  gps_frame_sync_meas_req_by_utran
 * DESCRIPTION
 *  issue a frame sync measurement request
 * PARAMETERS
 *  none
 * RETURNS
 *  none
 *****************************************************************************/
void gps_frame_sync_meas_req_by_utran(mtk_bool mode)
{
#if defined(__L1_GPS_AUTO_TIMING_SYNC_SUPPORT__) || defined(__L1_GPS_REF_TIME_SUPPORT__)     
    g_gps_frame_sync_meas_req_by_utran = mode;
#endif    
}

/*****************************************************************************
 * FUNCTION
 * gps_frame_sync_meas_req_by_ftime
 * DESCRIPTION
 *  issue a frame sync measurement request
 * PARAMETERS
 *  none
 * RETURNS
 *  none
 *****************************************************************************/
void gps_frame_sync_meas_req_by_ftime(mtk_bool mode)
{
#if defined(__L1_GPS_AUTO_TIMING_SYNC_SUPPORT__) || defined(__L1_GPS_REF_TIME_SUPPORT__)     
    g_gps_frame_sync_meas_req_by_ftime = mode;  
#endif    
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_meas_req_by_network
 * DESCRIPTION
 *  issue a frame sync measurement request
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *  fail(MTK_GPS_ERROR)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_frame_sync_meas_req_by_network(void)
{
#if defined(__L1_GPS_AUTO_TIMING_SYNC_SUPPORT__) || defined(__L1_GPS_REF_TIME_SUPPORT__)  

    
    if (g_gps_frame_sync_meas_req_by_ftime == KAL_TRUE)
    {
        gps_frame_sync_meas_req(GPS_FRAME_SYNC_TYPE1);
    }
    else
    {
        gps_frame_sync_meas_req(GPS_FRAME_SYNC_TYPE2);
    }
#endif
    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_meas_req
 * DESCRIPTION
 *  send a frame sync meas request to issue a frame pulse
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *  fail(MTK_GPS_ERROR)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_frame_sync_meas_req(mtk_gps_fs_work_mode mode)
{
#if defined(__L1_GPS_AUTO_TIMING_SYNC_SUPPORT__) || defined(__L1_GPS_REF_TIME_SUPPORT__)  
    if (mode == MTK_GPS_FS_WORK_MODE_AIDING)
    {
        g_gps_frame_sync_maintain_phase = KAL_FALSE;
    }
    else
    {
        g_gps_frame_sync_maintain_phase = KAL_TRUE;
    }
    gps_frame_sync_meas_req(GPS_FRAME_SYNC_TYPE3);
#endif    
    return MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_ftime_diff_by_network
 * DESCRIPTION
 *  calculate ftime difference between two frame sync request
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *  fail(MTK_GPS_ERROR)
 *****************************************************************************/
mtk_r8 mtk_gps_sys_frame_sync_ftime_diff_by_network(mtk_r8 dfFTime_New, mtk_r8 dfFTime_Old)
{
    mtk_r8 r8FTime_Diff, r8FTime_Max;

    if (g_gps_frame_sync_meas_req_by_utran == KAL_TRUE)
    {
        // 3G frame boundary: 40.96 sec
        r8FTime_Max = 40.96f;
    }
    else
    {
        // 2G frame boundary: 12533.76 sec       
        r8FTime_Max = 12533.76f;        
    }
            
    if (dfFTime_New >= dfFTime_Old)
    {
        r8FTime_Diff = dfFTime_New - dfFTime_Old;
    }
    else
    {
        r8FTime_Diff = dfFTime_New + (r8FTime_Max - dfFTime_Old);
    }
    
    return r8FTime_Diff;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_frame_sync_ftime_diff
 * DESCRIPTION
 *  calculate ftime difference between two frame sync request
 * PARAMETERS
 *  none
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *  fail(MTK_GPS_ERROR)
 *****************************************************************************/
mtk_r8 mtk_gps_sys_frame_sync_ftime_diff(mtk_r8 dfFTime_New, mtk_r8 dfFTime_Old)
{
    mtk_r8 r8FTime_Diff, r8FTime_Max;

    r8FTime_Max = 44343134792.571038f;        
            
    if (dfFTime_New >= dfFTime_Old)
    {
        r8FTime_Diff = dfFTime_New - dfFTime_Old;
    }
    else
    {
        r8FTime_Diff = dfFTime_New + (r8FTime_Max - dfFTime_Old);
    }
    
    return r8FTime_Diff;
}

/***************************************************************************** 
 * FUNCTION 
 * mtk_gps_sys_time_tick_get 
 * DESCRIPTION 
 * get the current system tick of target platform (msec) 
 * PARAMETERS 
 * none 
 * RETURNS 
 * system time tick 
 *****************************************************************************/ 
mtk_uint32 mtk_gps_sys_time_tick_get(void)
{
    return (60 / 13 * L1I_GetTimeStamp());
}

/***************************************************************************** 
 * FUNCTION 
 * mtk_gps_sys_time_tick_get_max 
 * DESCRIPTION 
 * get the maximum system tick of target platform (msec) 
 * PARAMETERS 
 * none 
 * RETURNS 
 * system time tick 
 *****************************************************************************/ 
mtk_uint32 mtk_gps_sys_time_tick_get_max(void)
{
    return 0xFFFFFFFF;
}

#if 1//add by Hiki
mtk_int32 mtk_gps_sys_initialize_mutex(void)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_create_mutex(mtk_mutex_enum mutex_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_take_mutex(mtk_mutex_enum mutex_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_give_mutex(mtk_mutex_enum mutex_idx) 
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_destroy_mutex(mtk_mutex_enum mutex_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_event_create(mtk_gps_event_enum mutex_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_event_set(mtk_gps_event_enum event_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_event_wait(mtk_gps_event_enum event_idx)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_event_delete(mtk_gps_event_enum mutex_idx) 
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_uart_init(char* portname, mtk_uint32 baudrate, mtk_uint32 txbufsize, mtk_uint32 rxbufsize)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_uart_write(mtk_int32 UARTHandle, const char* buffer, mtk_uint32 bufsize, mtk_int32* wrotenlength)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_uart_read(mtk_int32 UARTHandle, char* buffer, mtk_uint32 bufsize, mtk_int32* length)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_uart_uninit(mtk_int32 UARTHandle)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_set_spi_mode(mtk_uint8 enable_int, mtk_uint8 enable_burst)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_spi_poll (void)
{
    return MTK_GPS_SUCCESS;
}

mtk_int32 mtk_gps_sys_nmea_output_to_app(const char * buffer, mtk_uint32 length)
{
    return MTK_GPS_SUCCESS;
}
#endif 

#if 0//Hiki, temp for HS using (need to modify HS.lib)
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

