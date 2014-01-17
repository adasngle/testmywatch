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
 *   mtk_gps_task.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements mtk_gps component task create function
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/

/*************************************************************************
* Include Statements
 *************************************************************************/
#include <stdio.h>
#include "app_ltlcom.h"
#include "syscomp_config.h"
#include "dcl.h"

#include "mtk_gps.h"
#include "mtk_gps_agps.h"
#include "mtk_gps_task.h"
#include "mtk_gps_type.h"
#include "l1sm_public.h" 
#include "stack_config.h"
#include "string.h"
#include "kal_trace.h"
#include "kal_general_types.h"
#include "kal_public_defs.h"
#include "kal_public_api.h"
#include "kal_internal_api.h"
#include "mnl_trc.h"
#include "task_config.h"  
    
/*************************************************************************
* Function declaration
 *************************************************************************/
static void maui_gps_task(task_entry_struct * task_entry_ptr);
static void mtk_gps_timer_expiry_callback(void *arg);

//FOR  3336 or furture chip, these functions have been moved into MNL  lib 
#ifdef __GPS_MT3326__ 
static void vProc_Agps_Response(void);
static void vProc_Pmtk_Response(void);
#endif
mtk_bool fgAddNmeaChkSum(char *pBuf);
//extern mtk_int32 mtk_gps_sys_nmea_output (char* buffer, mtk_uint32 length);
extern mtk_int32 mtk_gps_get_debug_nmea (char* buffer, mtk_uint16* pfilled_size);

/*************************************************************************
* Global variable declaration
 *************************************************************************/
mtk_gps_task_struct mtk_gps_context;
mtk_gps_task_struct *mtk_gps_context_p = &mtk_gps_context;
mtk_gps_timer_table_struct mtk_gps_timer_table[MAX_NUM_OF_MTK_GPS_TIMER];

task_entry_struct *g_mnl_task_entry_ptr = NULL;

#ifdef __BEE_SUPPORT__
extern void BEE_Main(void);
extern kal_int32 g_bEnableRandomBEE;
extern kal_uint8 g_sv_in_view_list[32];
#endif /* __BEE_SUPPORT__ */
/*************************************************************************
* FUNCTION
*  mnl_create
*
* DESCRIPTION
*
* PARAMETERS
*
* RETURNS
*  None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool
mnl_create(comptask_handler_struct **handle)
{
   static const comptask_handler_struct mtk_gps_handler_info =
   {
      maui_gps_task,  /* task entry function */
      NULL,  /* task initialization function */
      NULL,  /* task configuration function */
      NULL,  /* task reset handler */
      NULL,  /* task termination handler */
   };

   *handle = (comptask_handler_struct *)&mtk_gps_handler_info;
   return KAL_TRUE;
}

#define GPS_ADM_POOL_SIZE (12*1024)


static kal_uint8 gps_adm_pool[GPS_ADM_POOL_SIZE] = {0};
KAL_ADM_ID gps_adm_pool_id = NULL;
kal_mutexid gps_adm_pool_mutex;
kal_eventgrpid gps_init_eventgrpid;
kal_uint8 mtk_gps_sleep_handle;
kal_uint8 gps_frame_sync_sleep_handle;
kal_mutexid gps_restart_cmd_mutex;
kal_uint8 gps_restart_cmd_exist = KAL_FALSE;
kal_mutexid gps_mnl_buffer_mutex;

// init UART/timer
mtk_int32 maui_gps_task_init_system(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    extern void Virtual_UART_Init(void);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Virtual_UART_Init();
    
    //Disable the MAUI sleep feature
    mtk_gps_sleep_handle = L1SM_GetHandle();
    gps_frame_sync_sleep_handle = L1SM_GetHandle();    
    // Create the ADM for MOD_MNL
    gps_adm_pool_id = kal_adm_create(gps_adm_pool, GPS_ADM_POOL_SIZE, NULL, KAL_TRUE);
    gps_adm_pool_mutex = kal_create_mutex("gps_adm_pool_mutex");
    gps_restart_cmd_mutex = kal_create_mutex("gps_restart_cmd_mutex");
    gps_init_eventgrpid = kal_create_event_group( "gps_init_eventgrpid" );
    gps_mnl_buffer_mutex = kal_create_mutex("gps_mnl_buffer_mutex");
    
    mtk_gps_context_p->gps_state = MNL_POWEROFF;

    mtk_gps_context_p->event_scheduler_ptr = evshed_create("MNL base timer", MOD_MNL, 0, 0 );

    return MTK_GPS_SUCCESS;
}

// free system resource
void maui_gps_task_uninit_system(mtk_int32 reason)
{
    // TBD
    return;
}


mtk_int32 maui_gps_task_init(mtk_gps_start_type start_type)
{
    mtk_int32 ret;
    
    // porting: system resource like timer/UART, etc.
    if((ret = maui_gps_task_init_system()) != MTK_GPS_SUCCESS)
    {
        maui_gps_task_uninit_system(ret);
        return ret;
    }
    return MTK_GPS_SUCCESS;

}

static unsigned char
calc_nmea_checksum (const char* sentence)
{
  unsigned char checksum = 0;

  while (*sentence)
  {
    checksum ^= (unsigned char)*sentence++;
  }

  return  checksum;
}

static double
translate_nmea_deg_min_sec (double deg)
{
  int  intpart = (int)deg;
  double fracpart = deg - (double)intpart;

  if (fracpart < .0)
  {
    fracpart = -fracpart;
  }
  if (intpart < 0)
  {
    intpart = -intpart;
  }
  fracpart *= 60.0;

  return  ((double)(intpart) * 100.0) + fracpart;
}


mtk_int32 mtk_gps_sys_callback_func(mtk_gps_notification_type ntype)
{
    kal_int32  i;                         
    mtk_gps_position *pvt;
	#ifndef __GPS_MT3332__
		kal_int32  cnt, slen;
    char *pBuf, *strbuf;
	#endif
    extern void mtk_gps_debug_wrapper(int group);
	#ifdef __GPS_MT3332__
    static kal_bool aic_flag = KAL_TRUE;
     if(aic_flag)//  default enable mode
     {
	  aic_flag  = KAL_FALSE;
	  mtk_gps_set_AIC_mode(MTK_AIC_ON );//用来消除板子上可能对GPS造成干扰的杂讯?
	 }
	#endif
	 
    pvt = (mtk_gps_position *) mtk_gps_sys_mem_alloc(sizeof(mtk_gps_position));
    memset(pvt, NULL, sizeof(mtk_gps_position));
	#ifndef __GPS_MT3332__
    // James modify for dynamic allocation        
    strbuf = mtk_gps_sys_mem_alloc(1024);
    memset(strbuf, NULL, 1024);
    pBuf = mtk_gps_sys_mem_alloc(1024);
    memset(pBuf, NULL, 1024);
	#endif

    switch (ntype)
    {
        case MTK_GPS_MSG_FIX_READY:
        #ifndef __GPS_MT3332__        	  
            mtk_gps_get_position(pvt);
   
            /* GPGGA */
            sprintf(strbuf,
                    "GPGGA,%02d%02d%02d.%03d,%09.4f,%c,%010.4f,%c,%d,%d,",
                    pvt->utc_time.hour, pvt->utc_time.min,
                    pvt->utc_time.sec, pvt->utc_time.msec,
                    translate_nmea_deg_min_sec(pvt->LLH[0]), ((pvt->LLH[0] >= .0) ? 'N' : 'S'),
                    translate_nmea_deg_min_sec(pvt->LLH[1]), ((pvt->LLH[1] >= .0) ? 'E' : 'W'),
                    pvt->fix_quality, pvt->sv_used_num);
            slen = strlen(strbuf);
            if (pvt->fix_type != MTK_GPS_FIX_TYPE_INVALID)
            {
                sprintf(&strbuf[slen], "%.2f", pvt->HDOP);
                slen += strlen(&strbuf[slen]);
            }
            sprintf(&strbuf[slen], ",%.1f,M,%.1f,M",
                    pvt->LLH[3], pvt->LLH[2] - pvt->LLH[3]);
                    slen += strlen(&strbuf[slen]);
            if (pvt->fix_quality == MTK_GPS_FIX_Q_DGPS)
            {
                sprintf(&strbuf[slen], ",%.1f,%d",
                pvt->DGPS_age, pvt->DGPS_station_ID);
            }
            else
            {
                sprintf(&strbuf[slen], ",,");
            }
            sprintf(pBuf,"$%s*%02X\r\n", strbuf, calc_nmea_checksum(strbuf));
            mtk_gps_sys_nmea_output(pBuf, strlen(pBuf));
            
            /* GPGSA */
            sprintf(strbuf,
                    "GPGSA,A,%d", pvt->fix_type);
                    slen = strlen(strbuf);
            for (i = 0; i < 12; i++)
            {
                if (pvt->sv_used_prn_list[i])
                {
                    sprintf(&strbuf[slen], ",%02d", pvt->sv_used_prn_list[i]);
                }
                else
                {
                    sprintf(&strbuf[slen], ",");
                }
                slen += strlen(&strbuf[slen]);
            }
            sprintf(&strbuf[slen], ",%.2f,%.2f,%.2f",
                    pvt->PDOP, pvt->HDOP, pvt->VDOP);
            sprintf(pBuf,"$%s*%02X\r\n", strbuf, calc_nmea_checksum(strbuf));
                    mtk_gps_sys_nmea_output(pBuf, strlen(pBuf));
            
            /* GPGSV */
            cnt = (pvt->sv_in_view_num + 3) / 4;
            if (cnt > 0)
            {
                for (i = 0; i < cnt; i++)
                {
                    kal_int32 svcnt;
                    sprintf(strbuf, "GPGSV,%d,%d,%d", cnt, i+1, pvt->sv_in_view_num);
                    slen = strlen(strbuf);
                    for (svcnt = (i * 4); (svcnt < ((i+1) * 4)) && (svcnt < pvt->sv_in_view_num); svcnt++)
                    {
                        if (pvt->sv_in_view_elev[svcnt] > -5)
                        {
                            sprintf(&strbuf[slen], ",%02d,%02d,%03d,",
                            pvt->sv_in_view_prn_list[svcnt],
                            pvt->sv_in_view_elev[svcnt],
                            pvt->sv_in_view_azim[svcnt]);
                        }
                        else
                        {
                            sprintf(&strbuf[slen], ",%02d,,,",
                            pvt->sv_in_view_prn_list[svcnt]);
                        }
                        slen += strlen(&strbuf[slen]);
                        if (pvt->sv_in_view_snr[svcnt] > 0)
                        {
                            sprintf(&strbuf[slen], "%d", pvt->sv_in_view_snr[svcnt]);
                            slen += strlen(&strbuf[slen]);
                        }
                    }
                    sprintf(pBuf,"$%s*%02X\r\n", strbuf, calc_nmea_checksum(strbuf));
                    mtk_gps_sys_nmea_output(pBuf, strlen(pBuf));
                }
            }
            
            /* GPRMC */
            sprintf(strbuf,
                    "GPRMC,%02d%02d%02d.%03d,%c,%09.4f,%c,%010.4f,%c,%.3f,%.2f,%02d%02d%02d,,,%c",
                    pvt->utc_time.hour, pvt->utc_time.min,
                    pvt->utc_time.sec, pvt->utc_time.msec,
                    (pvt->fix_type >= MTK_GPS_FIX_TYPE_2D ? 'A' : 'V'),
                    translate_nmea_deg_min_sec(pvt->LLH[0]), ((pvt->LLH[0] >= .0) ? 'N' : 'S'),
                    translate_nmea_deg_min_sec(pvt->LLH[1]), ((pvt->LLH[1] >= .0) ? 'E' : 'W'),
                    pvt->gspeed * 1.942795467, pvt->heading,
                    pvt->utc_time.mday, pvt->utc_time.month + 1,
                    pvt->utc_time.year % 100,
                    (pvt->fix_type >= MTK_GPS_FIX_TYPE_2D ?
                    (pvt->fix_quality == MTK_GPS_FIX_Q_DGPS ? 'D' : 'A') :
                    (pvt->fix_quality == MTK_GPS_FIX_Q_EST ? 'E' : 'N')));
            sprintf(pBuf,"$%s*%02X\r\n", strbuf, calc_nmea_checksum(strbuf));
            mtk_gps_sys_nmea_output(pBuf, strlen(pBuf));
             
            /* GPVTG */
            sprintf(strbuf,
                    "GPVTG,%.2f,T,,M,%.3f,N,%.3f,K,%c",
                    pvt->heading,
                    pvt->gspeed * 1.942795467,
                    pvt->gspeed * 3.6,
                    (pvt->fix_type >= MTK_GPS_FIX_TYPE_2D ?
                    (pvt->fix_quality == MTK_GPS_FIX_Q_DGPS ? 'D' : 'A') :
                    (pvt->fix_quality == MTK_GPS_FIX_Q_EST ? 'E' : 'N')));
            sprintf(pBuf,"$%s*%02X\r\n", strbuf, calc_nmea_checksum(strbuf));
            mtk_gps_sys_nmea_output(pBuf, strlen(pBuf));
                
            // Get debug information
            mtk_gps_debug_wrapper(1);
            mtk_gps_debug_wrapper(2);
        #endif
            
#ifdef __BEE_SUPPORT__            
            // Define SUPPORT_RANDOM_BEE_TEST , test BEE random pick
            if (g_bEnableRandomBEE)  
            {
                if (pvt->fix_type > MTK_GPS_FIX_TYPE_2D)
                {
                    memset(g_sv_in_view_list, 0, sizeof(g_sv_in_view_list));
                    for (i = 0; i < pvt->sv_in_view_num; i++)
                    {
                        if (pvt->sv_in_view_prn_list[i] < 32)
                        {
                            if (pvt->sv_in_view_elev[i] > 15)  // select with high elev if needed
                            {
                                g_sv_in_view_list[pvt->sv_in_view_prn_list[i]-1] = 1;
                            }
                        }
                    }                 
                }
            }                            
            BEE_Main();
#endif /* __BEE_SUPPORT__ */           
            break;


#ifdef __GPS_MT3326__ 
        /*FOR  3336 or furture chip, these functions have been moved into MNL  lib */
        case MTK_GPS_MSG_AGPS_RESPONSE:
            vProc_Agps_Response();
            break;

        case MTK_GPS_MSG_PMTK_RESPONSE:
            vProc_Pmtk_Response();
            break;
#endif   


        default:
            break;
    }
	#ifndef __GPS_MT3332__
    mtk_gps_sys_mem_free(strbuf);
    mtk_gps_sys_mem_free(pBuf);
	#endif
    mtk_gps_sys_mem_free(pvt);
    return MTK_GPS_SUCCESS;
}

mtk_int32 maui_gps_task_run()
{
    mtk_int32 ret;
  #ifdef __MTK_TARGET__
    #ifdef __GPS_MT3326__
        ret = mtk_gps_run(mtk_gps_sys_callback_func);
    #else
        ret = mtk_gps_run(mtk_gps_sys_callback_func, NULL);
    #endif
  #endif
    return ret;
}

mtk_int32 maui_gps_task_uninit(mtk_int32 reason)
{
    mtk_gps_uninit();
    maui_gps_task_uninit_system(reason);

    return MTK_GPS_SUCCESS;
}


/*****************************************************************************
 * FUNCTION
 *  maui_gps_task
 * DESCRIPTION
 *  GPS MT3326 task main
 * PARAMETERS
 *  task_entry_ptr      [IN]        task struct pointer 
 * RETURNS
 *  void
 *****************************************************************************/
static void maui_gps_task(task_entry_struct * task_entry_ptr)
{
    mtk_int32 ret;
#ifndef __MTK_TARGET__ 
	ilm_struct current_ilm;
#endif
    g_mnl_task_entry_ptr = task_entry_ptr;
    if((ret = maui_gps_task_init(MTK_GPS_START_DEFAULT)) != MTK_GPS_SUCCESS)
    {
        //mtk_gps_dbg_printf(ret);
        goto MTK_GPS_UNINIT;
    }
    MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_MNL_TASK_INIT_OK));
#ifdef __MTK_TARGET__    
    if((ret = maui_gps_task_run()) != MTK_GPS_SUCCESS)
    {
        //mtk_gps_dbg_printf(ret);
        goto MTK_GPS_UNINIT;
    }
#else //for modis :no action
	while (1)
	{
		receive_msg_ext_q(task_info_g[task_entry_ptr->task_indx].task_ext_qid, &current_ilm);
		free_ilm(&current_ilm);
	}

#endif

MTK_GPS_UNINIT:
    maui_gps_task_uninit(ret);

    return;
}


/*****************************************************************************
 * FUNCTION
 *  mtk_gps_timer_expiry
 * DESCRIPTION
 * 
 * PARAMETERS
 *  ilm_ptr                
 * RETURNS
 *  void
 *****************************************************************************/
void mtk_gps_timer_expiry(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    evshed_timer_handler(mtk_gps_context_p->event_scheduler_ptr);

}
/*****************************************************************************
 * FUNCTION
 *  mtk_gps_timer_expiry_callback
 * DESCRIPTION
 *  timer exiry callback
 * PARAMETERS
 *  arg     [IN]   callback function parameter   
 * RETURNS
 *  void
 *****************************************************************************/
static void mtk_gps_timer_expiry_callback(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 *timer_id = (kal_uint8*) arg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((kal_uint32) timer_id < (kal_uint32) MAX_NUM_OF_MTK_GPS_TIMER)
    {
        MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_MTK_GPS_TIMER_ID));
        mtk_gps_timer_table[(kal_uint32) (timer_id)].event_id = NULL;
        (mtk_gps_timer_table[(kal_uint32) (timer_id)].callback_func) (mtk_gps_timer_table[(kal_uint32) (timer_id)].arg);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mtk_gps_shutdown
 * DESCRIPTION
 *  GPS shutdown function
 * PARAMETERS
 *  
 * RETURNS
 *  void
 *****************************************************************************/
void mtk_gps_shutdown(void)
{
    mtk_gps_set_param(MTK_PARAM_CMD_RESET_DSP, NULL);
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_start_timer
 * DESCRIPTION
 *  start timer
 * PARAMETERS
 *  timer_id            [IN]      timer id  
 *  period              [IN]       period 
 *  timer_expiry        [IN]    delay time in ticks    
 *  arg                 [IN]      callback function parameter      
 * RETURNS
 *  void
 *****************************************************************************/
void mtk_gps_start_timer(kal_uint8 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint32 temp = period * 13 / 60 + 1;    /* ~= x 1/4.615 */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mtk_gps_timer_table[timer_id].callback_func = timer_expiry;
    mtk_gps_timer_table[timer_id].arg = arg;
    mtk_gps_timer_table[timer_id].event_id = evshed_set_event(
                                            mtk_gps_context_p->event_scheduler_ptr,
                                            (kal_timer_func_ptr) mtk_gps_timer_expiry_callback,
                                            (void*)timer_id,
                                            temp /** KAL_TICKS_10_MSEC */ );
}


/*****************************************************************************
 * FUNCTION
 *  gps_stop_timer
 * DESCRIPTION
 *  stop timer
 * PARAMETERS
 *  timer_id        [IN]    timer id    
 * RETURNS
 *  void
 *****************************************************************************/
void mtk_gps_stop_timer(kal_uint8 timer_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mtk_gps_timer_table[timer_id].event_id != NULL)
    {
        evshed_cancel_event(mtk_gps_context_p->event_scheduler_ptr, &(mtk_gps_timer_table[timer_id].event_id));
        mtk_gps_timer_table[timer_id].event_id = NULL;
    }
}

#ifdef __GPS_MT3326__ 
/*FOR  3336 or furture chip, vProc_Agps_Response() has been moved into MNL  lib */
static void vProc_Agps_Response(void)
{
    MTK_AGPS_RESPONSE_T rRsp;
    char szBuf[PMTK_MAX_PKT_LENGTH];


    if (mtk_gps_get_agps_response(&rRsp) != MTK_GPS_SUCCESS)
    {
        return;
    }


    switch (rRsp.u2Cmd)
    {
        case PMTK_ACK:
            sprintf(szBuf, "$PMTK%03d,%d,%d",
                           PMTK_ACK,
                           rRsp.uData.rAck.u2AckCmd, 
                           rRsp.uData.rAck.u1Flag);
            fgAddNmeaChkSum(szBuf);
            mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
        break;

        case PMTK_AGPS_MODE:
            sprintf(szBuf, "$PMTK%03d,%d,%d",
                           PMTK_AGPS_MODE,
                           rRsp.uData.rAgpsMode.u1Arg1,
                           rRsp.uData.rAgpsMode.u1Arg2);
            fgAddNmeaChkSum(szBuf);
            mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
        break;

      case PMTK_DT_AGPS_REQ_ASSIST:
          sprintf(szBuf, "$PMTK%03d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                         PMTK_DT_AGPS_REQ_ASSIST,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0001) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0002) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0004) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0008) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0010) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0020) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0040) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0080) ? 1 : 0,
                         (rRsp.uData.rReqAssist.u2BitMap & 0x0100) ? 1 : 0);
          fgAddNmeaChkSum(szBuf);
          mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
          break;

      case PMTK_DT_AGPS_LOC_EST:
          sprintf(szBuf, "$PMTK%03d,%d,%d,%d,%d,%.6f,%.6f,%d,%d,%d,%d,%d,%d,%d,%d", 
                         PMTK_DT_AGPS_LOC_EST,
                         rRsp.uData.rLoc.u4Frame,
                         rRsp.uData.rLoc.u2WeekNo,
                         rRsp.uData.rLoc.u4TowMS,
                         rRsp.uData.rLoc.u1FixType,
                         rRsp.uData.rLoc.dfLat,
                         rRsp.uData.rLoc.dfLon,
                         rRsp.uData.rLoc.i2Alt,
                         (mtk_uint32)rRsp.uData.rLoc.fUnc_SMaj,
                         (mtk_uint32)rRsp.uData.rLoc.fUnc_SMin,
                         rRsp.uData.rLoc.u2Maj_brg,
                         (mtk_uint32)rRsp.uData.rLoc.fUnc_Vert,
                         rRsp.uData.rLoc.u1Conf,
                         rRsp.uData.rLoc.u2HSpeed,
                         rRsp.uData.rLoc.u2Bearing);
          fgAddNmeaChkSum(szBuf);
          mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
          break;

      case PMTK_DT_AGPS_GPS_MEAS:
      {
          mtk_int32 i;

          for (i = 0; i < rRsp.uData.rPRM.u1NumValidMeas; i++)
          {
              sprintf(szBuf, "$PMTK%03d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                      PMTK_DT_AGPS_GPS_MEAS,
                      rRsp.uData.rPRM.u4Frame,
                      rRsp.uData.rPRM.u4GpsTow,
                      rRsp.uData.rPRM.u1NumValidMeas,
                      i,
                      rRsp.uData.rPRM.SV_Data[i].u1PRN_num,
                      rRsp.uData.rPRM.SV_Data[i].u1SNR,
                      rRsp.uData.rPRM.SV_Data[i].i2Dopp,
                      rRsp.uData.rPRM.SV_Data[i].u2Code_whole,
                      rRsp.uData.rPRM.SV_Data[i].u2Code_fract,
                      rRsp.uData.rPRM.SV_Data[i].u1Mul_Path_Ind,
                      rRsp.uData.rPRM.SV_Data[i].u1Range_RMS_Man,
                      rRsp.uData.rPRM.SV_Data[i].u1Range_RMS_Exp);
              fgAddNmeaChkSum(szBuf);
              mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
          }
      }
          break;

      case PMTK_DT_AGPS_LOC_ERR:
          sprintf(szBuf, "$PMTK%03d,%d", 
                         PMTK_DT_AGPS_LOC_ERR,
                         rRsp.uData.rLocErr.u1Type);
          fgAddNmeaChkSum(szBuf);
          mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
        break;

    #ifdef FRAME_SYNC_NETWORK_FINE_TIME_AIDING
      case PMTK_DT_AGPS_FTIME:
          sprintf(szBuf, "$PMTK%03d,%d,%.9lf,%.9lf,%.9lf,%.9lf",            
                         PMTK_DT_AGPS_FTIME,
                         rRsp.uData.rFTime.u2WeekNo,
                         rRsp.uData.rFTime.dfTow,
                         rRsp.uData.rFTime.dfTowRms,
                         rRsp.uData.rFTime.dfFTime,
                         rRsp.uData.rFTime.dfFTimeRms);        
          fgAddNmeaChkSum(szBuf);
          mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
          break;

      case PMTK_DT_AGPS_FTIME_ERR:
          sprintf(szBuf, "$PMTK%03d,%d", 
                         PMTK_DT_AGPS_FTIME_ERR,
                         rRsp.uData.rFTimeErr.u1Type);
          fgAddNmeaChkSum(szBuf);
          mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
          break;
    #endif

      default:
        break;
    }

}



static void vProc_Pmtk_Response(void)
{
    MTK_PMTK_RESPONSE_T rRsp;
    char szBuf[PMTK_MAX_PKT_LENGTH];


    if (mtk_gps_get_pmtk_response(&rRsp) != MTK_GPS_SUCCESS)
    {
        return;
    }


    switch (rRsp.u2Cmd)
    {
        case PMTK_ACK:
            sprintf(szBuf, "$PMTK%03d,%d,%d",
                           PMTK_ACK,
                           rRsp.uData.rAck.u2AckCmd, 
                           rRsp.uData.rAck.u1Flag);
            fgAddNmeaChkSum(szBuf);
            mtk_gps_sys_nmea_output(szBuf, strlen(szBuf));
        break;

      default:
        break;
    }
}
#endif

mtk_bool fgAddNmeaChkSum(char *pBuf)
{
  mtk_int16 i;
  mtk_uint8 chksum;
  char ch;
  mtk_uint16 Size;


  Size = strlen(pBuf);

  if (Size == 0)
  {
    return MTK_GPS_FALSE;
  }


  // will add six characters: '*', CHK1, CHK2, <CR>, <LF>, <ZERO>
  if (Size > (PMTK_MAX_PKT_LENGTH - 6))
  {
    Size = PMTK_MAX_PKT_LENGTH - 6;
  }

  chksum = 0;
  for (i = 1; i < Size; i++)
  {
    if (pBuf[i] != '*')
    {
      chksum ^= pBuf[i];
    }
    else
    {
      Size = i;
      break;
    }
  }


  // Add symbol '*'
  pBuf[Size] = '*';


  // Add CHK1
  ch = (chksum & 0xF0) >> 4;
  if (ch >= 10)
  {
    ch = 'A' + ch - 10;
  }
  else
  {
    ch = '0' + ch;
  }
  pBuf[Size + 1] = ch;


  // Add CHK2
  ch = chksum & 0x0F;
  if (ch >= 10)
  {
    ch = 'A' + ch - 10;
  }
  else
  {
    ch = '0' + ch;
  }
  pBuf[Size + 2] = ch;



  pBuf[Size + 3] = '\r';
  pBuf[Size + 4] = '\n';
  pBuf[Size + 5] = '\0';

  return MTK_GPS_TRUE;
}


