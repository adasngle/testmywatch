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
 *   mtk_gps_rtc.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements porting layer RTC functions
 *
 * Author:
 * -------
 * 
 *
 ****************************************************************************/
 
 #include "mtk_gps.h"
#include <time.h>
#include "dcl.h"

#include "mtk_gps_type.h"
    
static mtk_uint32 first_time_modification_flag = 0;
static mtk_uint32 time_modification_flag = 0;
static mtk_int32 rtctime_diff;
/*****************************************************************************
 * FUNCTION
 *  mtk_gps_time_modification
 * DESCRIPTION
 *
 * PARAMETERS
 *  utctime     [OUT]
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_time_modification (DCL_RTC_DATA_T *new_tSysTime, DCL_RTC_DATA_T *pre_tSysTime, mtk_uint32 first_time_flag)
{
    struct tm tt;
    time_t time_new, time_prev, time_diff;

    tt.tm_isdst = 0;
    tt.tm_yday = 0;
    tt.tm_wday = 0;
    tt.tm_year = new_tSysTime->rtc_year + 100;
    tt.tm_mon = new_tSysTime->rtc_mon - 1;
    tt.tm_mday = new_tSysTime->rtc_day;
    tt.tm_hour = new_tSysTime->rtc_hour;
    tt.tm_min = new_tSysTime->rtc_min;
    tt.tm_sec = new_tSysTime->rtc_sec;
    time_new = mktime(&tt);

    tt.tm_year = pre_tSysTime->rtc_year + 100;
    tt.tm_mon = pre_tSysTime->rtc_mon - 1;
    tt.tm_mday = pre_tSysTime->rtc_day;
    tt.tm_hour = pre_tSysTime->rtc_hour;
    tt.tm_min = pre_tSysTime->rtc_min;
    tt.tm_sec = pre_tSysTime->rtc_sec;
    time_prev = mktime(&tt);

    time_diff = time_prev - time_new;
    
    if (1 == time_modification_flag)  // Previous time difference was not retrieved by MNL yet
    {
        rtctime_diff += (mtk_int32) time_diff;
    }
    else
    {
        rtctime_diff = (mtk_int32) time_diff;
    }

    time_modification_flag = 1;
    first_time_modification_flag = first_time_flag;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_time_read
 * DESCRIPTION
 *
 * PARAMETERS
 *  utctime     [OUT]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_time_read (mtk_time* utctime)
{
   
    if ((utctime) &&
        (first_time_modification_flag == 0))
    {
        RTC_CTRL_GET_TIME_T tSysTime;
        /* handler for RTC Module */
        DCL_HANDLE rtc_handler; // It could be a global variable or a local variable

        memset(&tSysTime, 0, sizeof(tSysTime));

        /* Open a RTC handler to perform further operations */
	    rtc_handler = DclRTC_Open(DCL_RTC, FLAGS_NONE);
        //RTC_GetTime(&tSysTime);
        DclRTC_Control(rtc_handler, RTC_CMD_GET_TIME, (DCL_CTRL_DATA_T *)&tSysTime);
        
        utctime->year = tSysTime.u1Year + 100;   
        utctime->month = tSysTime.u1Mon - 1;       
        utctime->mday = tSysTime.u1Day;          
        utctime->hour = tSysTime.u1Hour;         
        utctime->min = tSysTime.u1Min;           
        utctime->sec = tSysTime.u1Sec;
        utctime->msec = 0;  // TODO: system time tick?? do not use timers
        if (1 == time_modification_flag)
        {
            time_modification_flag = 0;
            mtk_gps_time_change_notify(rtctime_diff);

            return MTK_GPS_ERROR_TIME_CHANGED;
        }
        return  MTK_GPS_SUCCESS;
    }
    first_time_modification_flag = 0;
    return  MTK_GPS_ERROR;
}
