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
 *   mtk_gps_uart.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements porting layer UART functions
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/

/*************************************************************************
* Include Statements
 *************************************************************************/
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"       /* Task message communiction */
#include "dcl.h"
#include "mtk_gps.h"
#include "mtk_gps_task.h"
#include "mtk_gps_type.h"
#include "stack_config.h"
#include "kal_trace.h"
#include "stack_ltlcom.h"
#include "kal_general_types.h"
#include "kal_public_defs.h"
#include "kal_public_api.h"
#include "mnl_trc.h"
#include "..\..\inc\Gps_uart.h"

extern kal_uint8 gps_uart_port;
extern mtk_gps_task_struct *mtk_gps_context_p;
extern kal_mutexid gps_mnl_buffer_mutex;

kal_uint8 uart_buf_temp[30];
void mtk_gps_uart_rx_cb(DCL_DEV port);
//mtk_int32 mtk_sys_nmea_input (const char* buffer, mtk_uint32 length);
extern void Host_Based_Debug_Output(unsigned char transmit, char *data, ...);

#define MNL_VIRTUAL_UART_BUFFER_SIZE 10240

typedef struct Ring_buffer     // Ring buffer
{
    kal_char *next_write;     // next position to write to
    kal_char *next_read;      // next position to read from
    kal_char *start_buffer;   // start of buffer
    kal_char *end_buffer;     // end of buffer + 1
} Ring_buffer;

Ring_buffer mnl_ring_buf;
Ring_buffer *g_mnl_ring_buf;
kal_char g_mnl_virtual_uart_buffer[MNL_VIRTUAL_UART_BUFFER_SIZE];
kal_uint8 mnl_ring_buf_overflow_flag = KAL_FALSE;/*add to indicate ring buff overflow  20110809*/


kal_uint8 send_RXilm;
extern kal_mutexid gps_restart_cmd_mutex;
extern kal_uint8 gps_restart_cmd_exist;
#ifdef __BEE_SUPPORT__
extern kal_int32 g_bEnableRandomBEE;
extern kal_int32 g_bJustRestart;
#endif /* __BEE_SUPPORT__ */
extern kal_bool fgVirtualUART_OK;
/*************************************************************************
* Function declaration
 *************************************************************************/
//static mtk_bool Check_Mtk_Nmea_Pkt_Token(const char* command, mtk_r8 *argument);
//static mtk_int32 Mtk_Nmea_Process_Buff(mtk_r8 *argument);

/*****************************************************************************
 * FUNCTION
 *  vPorting_UART_Init
 * DESCRIPTION
 *  GPS MT3326 virtual UART initialize
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Virtual_UART_Init(void)
{

    #if 1
    // Initialize the ring buffer for virtual uart port usage
    g_mnl_ring_buf = &mnl_ring_buf;
    g_mnl_ring_buf->start_buffer = g_mnl_virtual_uart_buffer;
    g_mnl_ring_buf->end_buffer = g_mnl_ring_buf->start_buffer + MNL_VIRTUAL_UART_BUFFER_SIZE;
    g_mnl_ring_buf->next_write = g_mnl_ring_buf->start_buffer;
    g_mnl_ring_buf->next_read = g_mnl_ring_buf->start_buffer;
    fgVirtualUART_OK = KAL_TRUE;
    MNL_TRACE((TRACE_INFO, MNL_TRC_INF_VIRTUAL_UART_INIT_OK));
    #endif
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_data_output
 * DESCRIPTION
 *  Transmit data to the GPS chip
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  msg         [IN]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_data_output (char* buffer, mtk_uint32 length)
{
    mtk_uint32 index;
    DCL_HANDLE handle;
	UART_CTRL_PUT_UART_BYTE_T data;
	
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        if (length > 0)
        {
            for (index = 0; index < length; index++)
            {
	            data.uData = buffer[index];
	            handle = DclSerialPort_Open(gps_uart_port, 0);
	            DclSerialPort_Control(handle, SIO_CMD_PUT_UART_BYTE, (DCL_CTRL_DATA_T*)&data);               
            }
            return  MTK_GPS_SUCCESS;
        }
        return  MTK_GPS_ERROR;
    }
    return  MTK_GPS_SUCCESS;
    
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_nmea_output
 * DESCRIPTION
 *  Transmit NMEA data output, may also contain debug information
 *  (The function body needs to be implemented)
 * PARAMETERS
 *  msg         [IN]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_nmea_output (char* buffer, mtk_uint32 length)
{
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
#else
    kal_uint16  index;
    module_type module_id;      //mtk54501 add for avoid fatal err when allocate [MAUI_03330630]
    
    kal_take_mutex(gps_mnl_buffer_mutex);
    
    index = 0;
    //Steve test start
    //if (index==0)
    //    return MTK_GPS_SUCCESS;
    //Steve test end
    
    while (length--)
    {

        // Read the new character to the virtual uart ring buffer.
        *(g_mnl_ring_buf->next_write++) = buffer[index++];

        // Wrap check the input circular buffer
        if ( g_mnl_ring_buf->next_write >= g_mnl_ring_buf->end_buffer )
        {
            g_mnl_ring_buf->next_write = g_mnl_ring_buf->start_buffer;
        }
        
        // Handle the overflow condition of write pointer ring over read pointer
        if (g_mnl_ring_buf->next_write == g_mnl_ring_buf->next_read)
        {
            if (g_mnl_ring_buf->next_write == g_mnl_ring_buf->start_buffer)
            {
                g_mnl_ring_buf->next_write = g_mnl_ring_buf->end_buffer;
            }
            else
            {
                g_mnl_ring_buf->next_write--; 
            }

            mnl_ring_buf_overflow_flag = KAL_TRUE; /*change 20110809*/
            g_mnl_ring_buf->next_write = g_mnl_ring_buf->start_buffer; /*change 20110809 when over flow reset g_mnl_ring_buf */
            g_mnl_ring_buf->next_read = g_mnl_ring_buf->start_buffer;/*change 20110809*/

            MNL_TRACE((TRACE_WARNING,GPS_MNL_TRC_MNL_RING_BUFF_OVER_FLOW));
            break;
        }
    }

    // Send message to GIS AP
    {
		if(0 == send_RXilm || (mnl_ring_buf_overflow_flag == KAL_TRUE))/*change 20110809*/

        {
            ilm_struct  *ilm_ptr;
            uart_ready_to_read_ind_struct *ind_p;
            
            ind_p = (uart_ready_to_read_ind_struct *)
                        construct_local_para( sizeof( uart_ready_to_read_ind_struct ), TD_CTRL );
            ind_p->port = 0;
#if 1          //mtk54501 add for avoid fatal err when allocate [MAUI_03330630]
            module_id= stack_get_active_module_id();
            ilm_ptr = allocate_ilm(module_id);
            ilm_ptr->src_mod_id = module_id;
#else
/* under construction !*/
/* under construction !*/
#endif
            ilm_ptr->dest_mod_id = MOD_GPS;
            ilm_ptr->sap_id = GPS_MNL_SAP;
            ilm_ptr->msg_id = MSG_ID_UART_READY_TO_READ_IND;
            ilm_ptr->local_para_ptr =  (local_para_struct*) ind_p;
            ilm_ptr->peer_buff_ptr = (peer_buff_struct*) NULL;
            msg_send_ext_queue(ilm_ptr);
            send_RXilm = 1;
            mnl_ring_buf_overflow_flag = KAL_FALSE;/*change 20110809*/

        }
    }
    kal_give_mutex(gps_mnl_buffer_mutex);
    
    return MTK_GPS_SUCCESS;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_uart_poll
 * DESCRIPTION
 *  GPS RX polling function 
 * PARAMETERS
 *  void
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_uart_poll(void)   // Port i/o interface object
{
    kal_uint32 result_count, actual_written_len;
    kal_uint8 status;
    mtk_int32 uart_recv_ok = MTK_GPS_ERROR;
    DCL_HANDLE handle;
	UART_CTRL_GET_BYTES_T data;
 
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        do
        {
            data.u4OwenrId = MOD_MNL;
	        data.u2Length = MTK_GPS_UART_READ_SIZE;
	        data.puBuffaddr = &uart_buf_temp[0];
            data.pustatus = &status;	
	        handle = DclSerialPort_Open(gps_uart_port, 0);
	        DclSerialPort_Control(handle, SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
	        result_count = data.u2RetSize;	
            //result_count = UART_GetBytes(gps_uart_port, &uart_buf_temp[0], MTK_GPS_UART_READ_SIZE, &status, MOD_MNL);
            if (result_count)
            {
                uart_recv_ok = mtk_gps_data_input((const char*) &uart_buf_temp[0], result_count, &actual_written_len); //Send to GPS kernel
                if ((uart_recv_ok == MTK_GPS_SUCCESS) &&
                    (actual_written_len != result_count))  //Steve test and we should take care of this condition further
                {
                    MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_DSP_IN_BUFFER_FULL));
                }
            }
            else
            {
                uart_recv_ok = MTK_GPS_SUCCESS;
            }           
        
            if (uart_recv_ok == MTK_GPS_ERROR)
            {
                break;
            }
        } while (result_count == MTK_GPS_UART_READ_SIZE);        
    }
    else
    {
        uart_recv_ok = MTK_GPS_SUCCESS;
    }
    return (uart_recv_ok);
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_nmea_poll
 * DESCRIPTION
 *  GPS NMEA polling function 
 * PARAMETERS
 *  void
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_nmea_poll(void)   // Port i/o interface object
{
    kal_uint16 result_count;
    kal_uint8 status;
    mtk_int32 uart_recv_ok = MTK_GPS_ERROR;
    DCL_HANDLE handle;
	UART_CTRL_GET_BYTES_T data; 

    MNL_TRACE((TRACE_INFO, MNL_TRC_INF_UART_NMEA_POLL));
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        do
        {
            data.u4OwenrId = MOD_MNL;
	        data.u2Length = MTK_GPS_UART_READ_SIZE;
	        data.puBuffaddr = &uart_buf_temp[0];
            data.pustatus = &status;            
	        handle = DclSerialPort_Open(gps_uart_port, 0);
	        DclSerialPort_Control(handle, SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
	        result_count = data.u2RetSize;	
            if (result_count)
            {
                uart_recv_ok = mtk_gps_nmea_input((const char*) &uart_buf_temp[0], result_count); //Send to porting layer
            }
            else
            {
                uart_recv_ok = MTK_GPS_SUCCESS;
            }
        
            if (uart_recv_ok == MTK_GPS_ERROR)
            {
                break;
            }
        } while (result_count == MTK_GPS_UART_READ_SIZE);
    }
    else
    {
        uart_recv_ok = MTK_GPS_SUCCESS;
    }
    return (uart_recv_ok);
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_rtcm_poll
 * DESCRIPTION
 *  GPS RTCM polling function 
 * PARAMETERS
 *  void
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32 mtk_gps_sys_rtcm_poll(void)   // Port i/o interface object
{
    kal_uint16 result_count;
    kal_uint8 status;
    mtk_int32 uart_recv_ok = MTK_GPS_ERROR;
    DCL_HANDLE handle;
	UART_CTRL_GET_BYTES_T data; 

    MNL_TRACE((TRACE_INFO, MNL_TRC_INF_UART_RTCM_POLL));
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {
        do
        {
            data.u4OwenrId = MOD_MNL;
	        data.u2Length = MTK_GPS_UART_READ_SIZE;
	        data.puBuffaddr = &uart_buf_temp[0];
            data.pustatus = &status;
	        handle = DclSerialPort_Open(gps_uart_port, 0);
	        DclSerialPort_Control(handle, SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
	        result_count = data.u2RetSize;	
	        if (result_count)
            {
                uart_recv_ok = mtk_gps_rtcm_input((const char*) &uart_buf_temp[0], result_count); //Send to GPS kernel
            }
            else
            {
                uart_recv_ok = MTK_GPS_SUCCESS;
            }
            
            if (uart_recv_ok == MTK_GPS_ERROR)
            {
                break;
            }
        } while (result_count == MTK_GPS_UART_READ_SIZE);
    }
    else
    {
        uart_recv_ok = MTK_GPS_SUCCESS;
    }
    return (uart_recv_ok);
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

void
mtk_gps_sys_pmtk_cmd_cb(mtk_uint16 u2CmdNum)
{
    extern kal_uint8 enable_mnl_debug;

    switch (u2CmdNum)
    {
        case 101:
        case 102:
        case 103:
        case 104:
#ifdef __BEE_SUPPORT__
            //def SUPPORT_RANDOM_BEE_TEST , test BEE random pick
            if (g_bEnableRandomBEE)
            {
                g_bJustRestart = 1;
            }
#endif /* __BEE_SUPPORT__ */            
            if (gps_restart_cmd_exist == KAL_TRUE)  // discard the continuous restart command
            {
                MNL_TRACE((TRACE_FUNC,GPS_MNL_TRC_DISCARD_RESTART_CMD));
                GPS_DEBUG_OUTPUT(GPS_DBG_WRN, "[Restart]discard continuous cmd %d", u2CmdNum);
                break;
            }

            MNL_TRACE((TRACE_INFO, MNL_TRC_INF_RESTART_BEGIN, u2CmdNum));
            GPS_DEBUG_OUTPUT(GPS_DBG_INF, "[Restart]begin,cmd=%d", u2CmdNum);
        
            kal_take_mutex(gps_restart_cmd_mutex);
            gps_restart_cmd_exist = KAL_TRUE;
            kal_give_mutex(gps_restart_cmd_mutex);
            break;
        case 299:
            enable_mnl_debug = 1;// Enable logging MNL debug message
            mtk_gps_debug_config(MDBG_ALL , MTK_DBG_INFO);
            break;
        default:
            break;
      }
}


/*****************************************************************************
 * FUNCTION
 *  mtk_mnl_uart_read
 * DESCRIPTION
 *  GPS virtual NMEA port reading function for other GIS software
 * PARAMETERS
 *  void
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_uint16 mtk_mnl_uart_read(mtk_uint8 *buffer, mtk_uint16 length)  // Porting layer implementation
{
    kal_uint16 readLen = 0;
    if ( g_mnl_ring_buf->next_read == g_mnl_ring_buf->next_write )
    {
       send_RXilm = 0;
       return 0;
    }
    
    while ( g_mnl_ring_buf->next_read != g_mnl_ring_buf->next_write )
    {
        // Add the next byte to the TX FIFO
        buffer[readLen++] = *(g_mnl_ring_buf->next_read++);

  	    // Wrap check output circular buffer
        if ( g_mnl_ring_buf->next_read >= g_mnl_ring_buf->end_buffer )
        {
            g_mnl_ring_buf->next_read = g_mnl_ring_buf->start_buffer;
        }
        if (readLen >= length)
        {
            break;
        }
    }
    if ( g_mnl_ring_buf->next_read == g_mnl_ring_buf->next_write )
    {
       send_RXilm = 0;
    }           
    return readLen;
}


mtk_int32
mtk_gps_sys_if_set_spd (mtk_uint32 baudrate, mtk_uint8 hw_fc)
{
    DCL_HANDLE handle;
    UART_CTRL_DCB_T data;
   	
	data.u4OwenrId = MOD_MNL;
	data.rUARTConfig.u4Baud = baudrate;
	data.rUARTConfig.u1DataBits = LEN_8;
	data.rUARTConfig.u1StopBits = SB_1;
	data.rUARTConfig.u1Parity = PA_NONE;
	data.rUARTConfig.u1FlowControl = FC_NONE;
	data.rUARTConfig.ucXonChar = 0x11;
	data.rUARTConfig.ucXoffChar = 0x13;
	data.rUARTConfig.fgDSRCheck = DCL_FALSE;//chang KAL_FALSE -> DCL_FALSE
	
    MNL_TRACE((TRACE_INFO, MNL_TRC_INF_UART_SET_BAUDRATE, baudrate));
    GPS_DEBUG_OUTPUT(GPS_DBG_INF, "[MNL Set]baudrate=%d", baudrate);
    if (hw_fc)
    {
        data.rUARTConfig.u1FlowControl = FC_HW;
    }
    
    if (mtk_gps_context_p->gps_state != MNL_POWEROFF)
    {

    	handle = DclSerialPort_Open(gps_uart_port, 0);
	    DclSerialPort_Control(handle, SIO_CMD_SET_DCB_CONFIG, (DCL_CTRL_DATA_T*)&data);
	    return MTK_GPS_SUCCESS;
    }
    return MTK_GPS_ERROR;
}
