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
 *   mtk_gps_task.h
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements gps_mt3326 component task type
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/

#ifndef _MTK_GPS_TASK_
#define _MTK_GPS_TASK_

#include "kal_public_api.h"
#include "kal_general_types.h"
    
/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
typedef enum 
{
  PORTING_FALSE,
  PORTING_TRUE
} proting_bool;


/*******************************************************************************
 * Task related Definitions
 *******************************************************************************/

typedef enum
{
    MTK_GPS_SLEEP_MODE_TIMER = 0,
//    MTK_GPS_RTC_INIT_TIMER,
    MAX_NUM_OF_MTK_GPS_TIMER
} mtk_gps_timer_enum;


typedef struct
{
    eventid event_id;
    kal_timer_func_ptr callback_func;
    void *arg;
} mtk_gps_timer_table_struct;


typedef enum
{
    MNL_POWERON = 0,
    MNL_POWEROFF,
    MNL_RUNNING,
    MNL_RESTART,
    MNL_TERMINATE,
    MNL_STATE_TOTAL
} mtk_gps_state_enum;


typedef struct
{
    mtk_gps_state_enum gps_state;
    event_scheduler *event_scheduler_ptr;
} mtk_gps_task_struct;

extern void mtk_gps_start_timer(kal_uint8 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg);
extern void mtk_gps_stop_timer(kal_uint8 timer_id);
/*******************************************************************************
 * Constant definition
 *******************************************************************************/
#ifndef NULL
#define NULL               0
#endif

#define MTK_GPS_UART_READ_SIZE 30


/*************************************************************************
* GPS Definition
 *************************************************************************/
#define PMTK_MAX_FIELD_NUM 32
#define PMTK_MAX_FIELD_LENGTH 32
#define PMTK_MAX_PKT_LENGTH 256

#define PMTK_ACK                     1
#define PMTK_AGPS_MODE               290
#define PMTK_SET_AGPS_ACCEPT_MAP     292
#define PMTK_API_GET_AGPS_FTIME      484
#define PMTK_API_GET_AGPS_LOC        485
#define PMTK_API_GET_AGPS_MEAS       486
#define PMTK_API_GET_AGPS_ASS_BIT    487
#define PMTK_API_GET_AGPS_LAST_LOC   488
#define PMTK_DT_EPH                  710
#define PMTK_DT_ALM                  711
#define PMTK_DT_TIME                 712
#define PMTK_DT_LOC                  713
#define PMTK_DT_CLK                  714
#define PMTK_DT_KLB                  715
#define PMTK_DT_UCP                  716
#define PMTK_DT_BSV                  717
#define PMTK_DT_ACQ                  718
#define PMTK_DT_FTIME                719
#define PMTK_DT_RTCM                 720
#define PMTK_DT_AGPS_REQ_ASSIST      730
#define PMTK_DT_AGPS_LOC_EST         731
#define PMTK_DT_AGPS_GPS_MEAS        732
#define PMTK_DT_AGPS_LOC_ERR         733
#define PMTK_DT_AGPS_FTIME           734
#define PMTK_DT_AGPS_FTIME_ERR       735


#define GPS_INIT_FINISH_EVENT           (0x0001)
#define GPS_SHUTDOWN_FINISH_EVENT       (0x0002)

#endif  /* _MTK_GPS_TASK_ */
