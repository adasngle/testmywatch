/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2006
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
 *
 * Filename:
 * ---------
 *	bt_adp_avrcp.h
 *
 * Project:
 * --------
 *   MAUI ESI Bluetooth
 *
 * Description:
 * ------------
 *   This file contains functions which provide the service to be the adaption layer 
 *   between Bluetooth and MMI
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
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
 * removed!
 * removed!
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
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *==============================================================================
 *******************************************************************************/
#include "avrcp.h"
#include "bt_avrcp_struct.h"
#include "kal_public_api.h"
#include "kal_general_types.h"

/****************************************************************************
 *
 * Types and Constants
 *
 ****************************************************************************/
#define AVRCP_CONVERT_BDADDR2ARRAY(dest, lap, uap, nap) \
        ((kal_uint8 *) dest)[0] = ((kal_uint8)(((kal_uint32)lap) & 0x000000FF));\
        ((kal_uint8 *) dest)[1] = ((kal_uint8)((((kal_uint32)lap) & 0x0000FF00) >> 8));\
        ((kal_uint8 *) dest)[2] = ((kal_uint8)((((kal_uint32)lap) & 0x00FF0000) >> 16));\
        ((kal_uint8 *) dest)[3] = ((kal_uint8)uap);\
        ((kal_uint8 *) dest)[4] = ((kal_uint8)(((kal_uint16)nap) & 0x00FF));\
        ((kal_uint8 *) dest)[5] = ((kal_uint8)((((kal_uint16)nap) & 0xFF00)>>8)); 

#define AVRCP_CONVERT_ARRAY2BDADDR(dest, src) \
     ((bt_avrcp_dev_addr_struct *)dest)->nap = (((kal_uint16)((kal_uint8 *) src)[4])) | ((kal_uint16)((kal_uint8 *) src)[5]<<8);\
    ((bt_avrcp_dev_addr_struct *)dest)->uap = (kal_uint8)((kal_uint8 *) src)[3];\
    ((bt_avrcp_dev_addr_struct *)dest)->lap =  (((kal_uint32)((kal_uint8 *) src)[2]) << 16) | ((kal_uint32)((kal_uint8 *) src)[1] <<8)|((kal_uint32)((kal_uint8 *) src)[0]); 

#define AVRCP_CONVERT_UUID_FROM_ROLE(_adp_role_ , _uuid_role_) \
    do {                                    \
        if (_adp_role_ == BT_AVRCP_TG)      \
        {                                   \
            _uuid_role_ = AVRCP_TG;         \
        }                                   \
        else if (_adp_role_ == BT_AVRCP_CT) \
        {                                   \
            _uuid_role_ = AVRCP_CT;         \
        }                                   \
        else                                \
        {                                   \
            ASSERT(0);                      \
        }                                   \
    } while(0);                              \

typedef enum 
{
    BT_AVRCP_STATE_IDLE,
    BT_AVRCP_STATE_ACTIVATED,
    BT_AVRCP_STATE_CONNECTING,
    BT_AVRCP_STATE_ACCEPTING_CONNECT,
    BT_AVRCP_STATE_CONNECTED,
    BT_AVRCP_STATE_DISCONNECTING
} avrcp_state;

typedef enum 
{
    AVRCP_NO_DATA,
    AVRCP_CMD_MORE,
    AVRCP_RSP_MORE
} avrcp_more_data;

#ifdef __BT_AVRCP_V13__

typedef kal_uint8 BTAVRCPEvent;

#ifdef __TG_V13__
#define BTAVRCP_EVENT_GET_CAPABILITIES_IND                    0x20
#define BTAVRCP_EVENT_LIST_PLAYERAPP_SETTINGATTRIBUTE_IND     0x21
#define BTAVRCP_EVENT_LIST_PLAYERAPP_SETTINGVALUE_IND         0x22
#define BTAVRCP_EVENT_GET_CURPLAYERAPP_SETTINGVALUE_IND       0x23
#define BTAVRCP_EVENT_SET_PLAYERAPP_SETTINGVALUE_IND          0x24
#define BTAVRCP_EVENT_GET_PLAYERAPP_SETTINGATTRIBUTETEXT_IND  0x25
#define BTAVRCP_EVENT_GET_PLAYERAPP_SETTINGVALUETEXT_IND      0x26
#define BTAVRCP_EVENT_INFORM_DISPLAY_CHARSET_IND              0x27
#define BTAVRCP_EVENT_INFORM_BATTERY_STATUSOFCT_IND           0x28
#define BTAVRCP_EVENT_GET_ELEMENT_ATTRIBUTES_IND              0x29
#define BTAVRCP_EVENT_GET_PLAYERAPP_STATUS_IND                0x2a
#define BTAVRCP_EVENT_REGISTER_NOFITICATION_IND               0x2b
#define BTAVRCP_EVENT_REQUEST_CONTINUING_IND                  0x2c
#define BTAVRCP_EVENT_ABORT_CONTINUING_IND                    0x2d
#endif
#define BTAVRCP_EVENT_GET_CAPABILITIES_CNF                    0x50
#define BTAVRCP_EVENT_LIST_PLAYERAPP_SETTINGATTRIBUTE_CNF     0x51
#define BTAVRCP_EVENT_LIST_PLAYERAPP_SETTINGVALUE_CNF         0x52
#define BTAVRCP_EVENT_GET_CURPLAYERAPP_SETTINGVALUE_CNF       0x53
#define BTAVRCP_EVENT_SET_PLAYERAPP_SETTINGVALUE_CNF          0x54
#define BTAVRCP_EVENT_GET_PLAYERAPP_SETTINGATTRIBUTETEXT_CNF  0x55
#define BTAVRCP_EVENT_GET_PLAYERAPP_SETTINGVALUETEXT_CNF      0x56
#define BTAVRCP_EVENT_INFORM_DISPLAY_CHARSET_CNF              0x57
#define BTAVRCP_EVENT_INFORM_BATTERY_STATUSOFCT_CNF           0x58
#define BTAVRCP_EVENT_GET_ELEMENT_ATTRIBUTES_CNF              0x59
#define BTAVRCP_EVENT_GET_PLAYERAPP_STATUS_CNF                0x5a
#define BTAVRCP_EVENT_REGISTER_NOFITICATION_CNF               0x5b
#define BTAVRCP_EVENT_REQUEST_CONTINUING_CNF                  0x5c
#define BTAVRCP_EVENT_ABORT_CONTINUING_CNF                    0x5d
/* command indication */

typedef bt_avrcp_list_playerapp_attribute_ind_struct         BTAVRCPListPlayerappAttributeInd;
#ifdef __TG_V13__
typedef bt_avrcp_get_capabilities_ind_struct                 BTAVRCPGetCapabilitiesInd;
typedef bt_avrcp_list_playerapp_value_ind_struct             BTAVRCPListPlayerappValueInd;
typedef bt_avrcp_get_curplayerapp_value_ind_struct           BTAVRCPGetCURPlayerValueInd;
typedef bt_avrcp_set_playerapp_value_ind_struct              BTAVRCPSetPlayerValueInd; 
typedef bt_avrcp_get_playerapp_attributetext_ind_struct      BTAVRCPGetPlayerAttrTextInd;
typedef bt_avrcp_get_playerapp_valuetext_ind_struct          BTAVRCPGetPlayerValueTextInd;
typedef bt_avrcp_inform_display_charset_ind_struct           BTAVRCPInformCharsetInd;
typedef bt_avrcp_inform_battery_statusofct_ind_struct        BTAVRCPInformBattOfCTInd;
typedef bt_avrcp_get_element_attributes_ind_struct           BTAVRCPGetElementAttrInd;
typedef bt_avrcp_get_playerstatus_ind_struct                 BTAVRCPGetPlayerStatusInd;
typedef bt_avrcp_register_notification_ind_struct             BTAVRCPRegisterNotifyInd;
typedef bt_avrcp_abort_continueresponse_ind_struct           BTAVRCPAbortContinueInd;
#endif

/* response indication */
typedef bt_avrcp_get_capabilities_cnf_struct                 BTAVRCPGetCapabilitiesCnf;
typedef bt_avrcp_list_playerapp_attribute_cnf_struct         BTAVRCPListPlayerappAttributeCnf;
typedef bt_avrcp_list_playerapp_value_cnf_struct             BTAVRCPListPlayerappValueCnf;
typedef bt_avrcp_get_curplayerapp_value_cnf_struct           BTAVRCPGetCURPlayerValueCnf;
typedef bt_avrcp_set_playerapp_value_cnf_struct              BTAVRCPSetPlayerValueCnf; 
typedef bt_avrcp_get_playerapp_attributetext_cnf_struct      BTAVRCPGetPlayerAttrTextCnf;
typedef bt_avrcp_get_playerapp_valuetext_cnf_struct          BTAVRCPGetPlayerValueTextCnf;
typedef bt_avrcp_inform_display_charset_cnf_struct           BTAVRCPInformCharsetCnf;
typedef bt_avrcp_inform_battery_statusofct_cnf_struct        BTAVRCPInformBattOfCTCnf;
typedef bt_avrcp_get_element_attributes_cnf_struct           BTAVRCPGetElementAttrCnf;
typedef bt_avrcp_get_playerstatus_cnf_struct                 BTAVRCPGetPlayerStatusCnf;
typedef bt_avrcp_register_notification_cnf_struct             BTAVRCPRegisterNotifyCnf;
typedef bt_avrcp_abort_continueresponse_cnf_struct           BTAVRCPAbortContinueCnf;
typedef struct _BTAVRCPCallbackParms {
    BTAVRCPEvent    event;   /* Event associated with the callback       */
    kal_uint16            result;
    kal_uint8             error_code;
    union{
		
        BTAVRCPListPlayerappAttributeInd AVRCPListPlayerappAttributeInd;
		#ifdef __TG_V13__
        BTAVRCPGetCapabilitiesInd AVRCPGetCapabilitiesInd;
        BTAVRCPListPlayerappValueInd AVRCPListPlayerappValueInd;
        BTAVRCPGetCURPlayerValueInd ABTAVRCPGetCURPlayerValueInd;
        BTAVRCPSetPlayerValueInd  ABTAVRCPSetPlayerValueInd ;
        BTAVRCPGetPlayerAttrTextInd ABTAVRCPGetPlayerAttrTextInd;
        BTAVRCPGetPlayerValueTextInd ABTAVRCPGetPlayerValueTextInd;
        BTAVRCPInformCharsetInd ABTAVRCPInformCharsetInd;
        BTAVRCPInformBattOfCTInd ABTAVRCPInformBattOfCTInd;
        BTAVRCPGetElementAttrInd ABTAVRCPGetElementAttrInd;
        BTAVRCPGetPlayerStatusInd ABTAVRCPGetPlayerStatusInd;
        BTAVRCPRegisterNotifyInd ABTAVRCPRegisterNotifyInd;
        BTAVRCPAbortContinueInd ABTAVRCPAbortContinueInd;
		#endif
        BTAVRCPGetCapabilitiesCnf ABTAVRCPGetCapabilitiesCnf;
        BTAVRCPListPlayerappAttributeCnf ABTAVRCPListPlayerappAttributeCnf;
        BTAVRCPListPlayerappValueCnf ABTAVRCPListPlayerappValueCnf;
        BTAVRCPGetCURPlayerValueCnf ABTAVRCPGetCURPlayerValueCnf;
        BTAVRCPSetPlayerValueCnf ABTAVRCPSetPlayerValueCnf;
        BTAVRCPGetPlayerAttrTextCnf ABTAVRCPGetPlayerAttrTextCnf;
        BTAVRCPGetPlayerValueTextCnf ABTAVRCPGetPlayerValueTextCnf;
        BTAVRCPInformCharsetCnf ABTAVRCPInformCharsetCnf;
        BTAVRCPInformBattOfCTCnf ABTAVRCPInformBattOfCTCnf;
        BTAVRCPGetElementAttrCnf ABTAVRCPGetElementAttrCnf;
        BTAVRCPGetPlayerStatusCnf ABTAVRCPGetPlayerStatusCnf;
        BTAVRCPRegisterNotifyCnf ABTAVRCPRegisterNotifyCnf;
        BTAVRCPAbortContinueCnf ABTAVRCPAbortContinueCnf;
    }p;

}BTAVRCPCallbackParms;

typedef void (*BTAVRCPCallback)(BTAVRCPCallbackParms *Parms);

typedef struct
{
    avrcp_state     state;
    kal_uint8       rcp_role;
    kal_uint8       connect_id;
    AvrcpChannel    rcp_channel;
    AvrcpCmdFrame   cmdFrame;
    AvrcpRspFrame   rspFrame;
    avrcp_more_data   cmd_more_data;
    avrcp_more_data   rsp_more_data;
    BTAVRCPCallback avrcpCBfunc;

    /* AV13 available Packets */
    ListEntry freePacketList;
    /* AV13 Packet resources */
    AvrcpQueuePacketFrame txPackets[AVRCP_NUM_TX_PACKETS];
    /* AV13 packet array for queue the incoming data */
    kal_uint8 txData[AVRCP_NUM_TX_PACKETS][AVRCP_MAX_PACKET_LEN]; 

    /* AV13 channel type 0 for AVCTP 1 for AVCTP_Browsing*/
} avrcp_adp_context;

#else 

typedef struct
{
    avrcp_state     state;
    kal_uint8       rcp_role;
    kal_uint8       connect_id;
    AvrcpChannel    rcp_channel;
    AvrcpCmdFrame   cmdFrame;
    AvrcpRspFrame   rspFrame;
    avrcp_more_data   more_data;    
} avrcp_adp_context;

#endif

