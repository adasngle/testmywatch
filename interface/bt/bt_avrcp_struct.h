
#ifndef __BT_AVRCP_STRUCT_H_
#define __BT_AVRCP_STRUCT_H_

#include "kal_general_types.h"
#include "app_ltlcom.h"

/* AVRCP device roles */
#define BT_AVRCP_TG		0
#define BT_AVRCP_CT		1
#define BT_AVRCP_MAX_CHANNEL   2 /* One Target and one Controller */
#define BT_AVRCP_ROLE_UNDEF		255

/* command/response types */
#define BT_AVRCP_CR_CONTROL				(0x00)
#define BT_AVRCP_CR_STATUS					(0x01)
#define BT_AVRCP_CR_SPECIFIC_INUIRY		(0x02)
#define BT_AVRCP_CR_NOTIFY					(0x03)
#define BT_AVRCP_CR_GENERAL_INQUIRY		(0x04)
#define BT_AVRCP_CR_ACCEPT					(0x09)
#define BT_AVRCP_CR_REJECT					(0x0A)
#define BT_AVRCP_CR_NOT_IMPLEMENT		(0x08)
#define BT_AVRCP_CR_STABLE					(0x0c)

/* sub_unit types */
#define BT_AVRCP_SUBUNIT_TYPE_UNIT_INFO		(0x1f)
#define BT_AVRCP_SUBUNIT_TYPE_SUBUNIT_INFO	(0x1f)
#define BT_AVRCP_SUBUNIT_TYPE_PASS_THROUGH	(0x09) /* Panel subunit */

/* sub_unit ID */
#define BT_AVRCP_SUBUNIT_ID_UNIT_INFO		(0x07)
#define BT_AVRCP_SUBUNIT_ID_SUBUNIT_INFO		(0x07)
#define BT_AVRCP_SUBUNIT_ID_PASS_THROUGH	(0x00)

/* opcodes */
#define BT_AVRCP_OP_UNIT_INFO			(0x30)
#define BT_AVRCP_OP_SUBUNIT_INFO			(0x31)
#define BT_AVRCP_OP_VENDOR				(0x00)
#define BT_AVRCP_OP_PASS_THROUGH			(0x7c)

#ifdef __BT_AVRCP_V13__

#define BT_AVRCP_MAX_CMD_FRAME_SIZE          64//200     /* AV13 use large cmd */ Taicong modi
#define BT_AVRCP_MAX_RSP_FRAME_SIZE          1024//2048    /* AV13 as a controller, target's response could be a large data*/////taicong Modi
#define BT_AVRCP_MAX_IND_CMD_NUM             10      /* Maximum indication send to MMI if confirm not send back yet */
#define BT_AVRCP_MAX_AV_FRAME_SIZE           512     /* Defined by AV/C spec *///@@@@@

/* AVRCP Error code - Message for CT */
#define BT_AVRCP_RESULT_SUCCESS            0x1000
#define BT_AVRCP_RESULT_ERROR              0x1001
#define BT_AVRCP_RESULT_INVALID_ROLE       0x1002
#define BT_AVRCP_RESULT_INTERNAL_ERROR     0x1003
#define BT_AVRCP_RESULT_CHANNEL_NOT_FOUND  0x1004


#define BT_AVRCP_RESULT_TIMEOUT         0x1003
#define BT_AVRCP_RESULT_BAD_STATE       0x1004
#define BT_AVRCP_RESULT_BUSY            0x1005
/* AVRCP Error/Status code AVRCP spec 6.15.3 status and error codes */
#define BT_AVRCP_ERRCODE_INVALID_CMD            0x00 // All cmds
#define BT_AVRCP_ERRCODE_INVALID_PARAM          0x01 // All cmds
#define BT_AVRCP_ERRCODE_NOT_FOUND              0x02 // All cmds
#define BT_AVRCP_ERRCODE_INTERNAL_ERROR         0x03 // All cmds
#define BT_AVRCP_ERRCODE_OPERATION_COMPLETE     0x04 // All cmds except CType is reject

#define BT_AVRCP_ERRCODE_NO_AVAILABLE_PLAYER    0x15 // All cmds
#define BT_AVRCP_ERRCODE_PLAYER_CHANGED         0x16 // register notification

/* PASS THROUGH PDU_ID - AV/C  PANEL subunit spec */
#define BT_AVRCP_OPERATION_ID_PLAY              0x44
#define BT_AVRCP_OPERATION_ID_STOP              0x45
#define BT_AVRCP_OPERATION_ID_PAUSE             0x46
#define BT_AVRCP_OPERATION_ID_FORWARD           0x4B
#define BT_AVRCP_OPERATION_ID_BACKWARD          0x4C
#define BT_AVRCP_OPERATION_ID_FASTFORWARD       0x49
#define BT_AVRCP_OPERATION_ID_REWIND            0x48
#define BT_AVRCP_OPERATION_ID_VOLUME_UP         0x41
#define BT_AVRCP_OPERATION_ID_VOLUME_DOWN       0x42
#define BT_AVRCP_OPERATION_ID_VENDOR_UNIQUE     0x7E
#define BT_AVRCP_OPERATION_ID_VENDOR_NEXT_GROUP       0x0000
#define BT_AVRCP_OPERATION_ID_VENDOR_PREVIOUS_GROUP   0x0001
/* AVRCP V1.3 */
#define BT_AVRCP_MAX_ATTRIBUTE_NUM			    4			// Max number of player application setting attributes
#define BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM	    4			// Max number of player application setting attibute value
#define BT_AVRCP_MAX_EVENT_NUM			        8//20			// Max number of event supported (TG)

#define BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE      80          // Max string leng for text
#define BT_AVRCP_MAX_VALUE_STRING_SIZE          80          // Max string leng for text
#define BT_AVRCP_MAX_GET_ELEMENT_ATTR_NUM       8//10          // Max number of media elements which be querying in once

#define BT_AVRCP_MAX_ELEMENET_BUFFER_SIZE       512

#define AVRCP_MAX_EVENT_NUM                                       0x08
#define AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED                0x01
#define AVRCP_NOTIFY_EVENT_TRACK_CHANGED                          0x02
#define AVRCP_NOTIFY_EVENT_TRACK_REACHED_END                      0x03
#define AVRCP_NOTIFY_EVENT_TRACK_REACHED_START                    0x04
#define AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED                   0x05
#define AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED                    0x06
#define AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED                  0x07
#define AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED     0x08

#else

#define BT_AVRCP_MAX_CMD_FRAME_SIZE     20
#define BT_AVRCP_MAX_IND_CMD_NUM        10      /* Maximum indication send to MMI if confirm not send back yet */

#endif
typedef struct
{
	kal_uint32 lap; 
	kal_uint8  uap; 
	kal_uint16 nap; // non-significant 32..47
} bt_avrcp_dev_addr_struct;


/* ===================================================== */

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 local_role;
} bt_avrcp_activate_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;	
    kal_uint16 result;
} bt_avrcp_activate_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
} bt_avrcp_deactivate_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint16 result;
} bt_avrcp_deactivate_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 local_role;    
    bt_avrcp_dev_addr_struct device_addr;
} bt_avrcp_connect_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 connect_id;
    kal_uint16 result;
    bt_avrcp_dev_addr_struct device_addr;
} bt_avrcp_connect_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 connect_id;
    bt_avrcp_dev_addr_struct device_addr;
} bt_avrcp_connect_ind_struct;

#ifdef __BT_AVRCP_V13__
typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
	kal_uint16 result;
    bt_avrcp_dev_addr_struct device_addr;

	kal_uint16 version;
    kal_uint16 remote_support_feature; 
} bt_avrcp_version_ind_struct;
#endif

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;    
    kal_bool accept_conn;
} bt_avrcp_connect_ind_res_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
} bt_avrcp_disconnect_req_struct;


typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 connect_id;    
    kal_uint16 result;
} bt_avrcp_disconnect_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 chnl_num;
    kal_uint8 connect_id;    
} bt_avrcp_disconnect_ind_struct;


typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   c_type;
    kal_uint8   subunit_type;
    kal_uint8   subunit_id;
    kal_uint16  data_len;
    kal_uint8   frame_data[BT_AVRCP_MAX_CMD_FRAME_SIZE];
} bt_avrcp_cmd_frame_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;	
    kal_uint8   seq_id;
    kal_uint8   c_type;
    kal_uint8   subunit_type;
    kal_uint8   subunit_id;
    kal_uint16  data_len;
    kal_uint8   frame_data[BT_AVRCP_MAX_CMD_FRAME_SIZE];
} bt_avrcp_cmd_frame_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8   chnl_num;	
    kal_uint8   seq_id;
    kal_uint8   c_type;
    kal_uint8   subunit_type;
    kal_uint8   subunit_id;
    kal_uint16  data_len;
    kal_uint16  profile_id;
    kal_uint8   frame_data[BT_AVRCP_MAX_CMD_FRAME_SIZE];
} bt_avrcp_cmd_frame_ind_struct;

typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;	
    kal_uint8   seq_id;
    kal_uint8   c_type;
    kal_uint8   subunit_type;
    kal_uint8   subunit_id;
    kal_uint16  data_len;
    kal_uint16  profile_id;	
    kal_uint8   frame_data[BT_AVRCP_MAX_CMD_FRAME_SIZE];
} bt_avrcp_cmd_frame_ind_res_struct;


#ifdef __BT_AVRCP_V13__

typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
} bt_avrcp_list_playerapp_attribute_ind_struct;

#ifdef __TG_V13__
/***************** AVRCP 1.3 TG ******************/
/* reject: AVCTP header. error_code: AVRCP spec 6.15.3 */
#define LOCAL_PARA_AVRCP_REJECT \
   kal_uint8	reject; \
   kal_uint8	error_code;

/* MSG_ID_BT_AVRCP_GET_CAPABILITIES_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   capability_id; /* 0x03 avrcp_event_suppport */
} bt_avrcp_get_capabilities_ind_struct;

/* MSG_ID_BT_AVRCP_GET_CAPABILITIES_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint8   capability_id;
	kal_uint8   count; // capability count 
	kal_uint8   capability[BT_AVRCP_MAX_EVENT_NUM]; // 2..n spec 6.4.1 
} bt_avrcp_get_capabilities_rsp_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_ATTRIBUTE_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
} bt_avrcp_list_playerapp_attribute_ind_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_ATTRIBUTE_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	LOCAL_PARA_AVRCP_REJECT
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];
} bt_avrcp_list_playerapp_attribute_rsp_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_VALUE_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   attribute_id; // query the possible values of this attribute id
} bt_avrcp_list_playerapp_value_ind_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_VALUE_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
	kal_uint8   attribute_id;
	kal_uint8   count; //availabe values[]
	kal_uint8   attribute_values[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; //possiblel setting values
} bt_avrcp_list_playerapp_value_rsp_struct;

/* MSG_ID_BT_AVRCP_GET_CURPLAYERAPP_VALUE_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
} bt_avrcp_get_curplayerapp_value_ind_struct;

/* MSG_ID_BT_AVRCP_GET_CURPLAYERAPP_VALUE_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
	kal_uint8   attribute_value[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // value of index of attribute id 
} bt_avrcp_get_curplayerapp_value_rsp_struct;

/* MSG_ID_BT_AVRCP_SET_PLAYERAPP_VALUE_IND */
typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
	kal_uint8   values[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // value of index of attribute id 
} bt_avrcp_set_playerapp_value_ind_struct;

/* MSG_ID_BT_AVRCP_SET_PLAYERAPP_VALUE_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
} bt_avrcp_set_playerapp_value_rsp_struct; //Ack the set-value ( not take effect yet by spec 6.5.4)

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_ATTRIBUTETEXT_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
} bt_avrcp_get_playerapp_attributetext_ind_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_ATTRIBUTETEXT_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint16  charset[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint8   attribute_string_len[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint8   attribute_string[BT_AVRCP_MAX_ATTRIBUTE_NUM][BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE]; // 4*40=160 Byte. Note! 
} bt_avrcp_get_playerapp_attributetext_rsp_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_VALUETEXT_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   attribute_id; // query all text of attribute values of a specific attribute id
    kal_uint8   count;        // available number of attribute values
	kal_uint8   value_ids[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM];
} bt_avrcp_get_playerapp_valuetext_ind_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_VALUETEXT_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint8   attribute_id; 
    kal_uint8   count; 
	kal_uint8   value_ids[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM];
    kal_uint16  charset[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; // results
    kal_uint8   value_string_len[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; // query results
    kal_uint8   value_string[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM][BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE]; // 4*40=160 Byte. Note! 
} bt_avrcp_get_playerapp_valuetext_rsp_struct;

/* MSG_ID_BT_AVRCP_INFORM_DISPLAY_CHARSET_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   count; // useless? in spec. figure 26.2. the counter is always 1 
    kal_uint16  charset_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // in spec. 6.5.7. the charset_id is not a array.
} bt_avrcp_inform_display_charset_ind_struct;

/* MSG_ID_BT_AVRCP_INFORM_DISPLAY_CHARSET_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
} bt_avrcp_inform_display_charset_rsp_struct;

/* MSG_ID_BT_AVRCP_INFORM_BATTERY_STATUSOFCT_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   battery_status; // spec. 6.5.8
} bt_avrcp_inform_battery_statusofct_ind_struct;

/* MSG_ID_BT_AVRCP_INFORM_BATTERY_STATUSOFCT_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
} bt_avrcp_inform_battery_statusofct_rsp_struct;

/* MSG_ID_BT_AVRCP_GET_ELEMENT_ATTRIBUTES_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   identifier[8];
    kal_uint8   count;
    kal_uint32  attributes[BT_AVRCP_MAX_GET_ELEMENT_ATTR_NUM];
} bt_avrcp_get_element_attributes_ind_struct;

/* MSG_ID_BT_AVRCP_GET_ELEMENT_ATTRIBUTES_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint8   count; // spec 6.6.1
    bt_avrcp_element_attributes_struct elements[BT_AVRCP_MAX_GET_ELEMENT_ATTR_NUM];
    kal_uint8   element_data[BT_AVRCP_MAX_ELEMENET_BUFFER_SIZE];
} bt_avrcp_get_element_attributes_rsp_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERSTATUS_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
} bt_avrcp_get_playerstatus_ind_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERSTATUS_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint32  song_length; // spec 6.7.1
    kal_uint32  song_position; // spec 6.7.1
    kal_uint8   play_status; // spec 6.7.1
} bt_avrcp_get_playerstatus_rsp_struct;

/* MSG_ID_BT_AVRCP_REGISTER_NOTIFICATION_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   event_id; // spec appendix H
    kal_uint32  playback_interval; // spec 6.7.1 only available when event_id is EVENT_PLAYBACK_POS_CHANGED
} bt_avrcp_register_notification_ind_struct;

/* MSG_ID_BT_AVRCP_REGISTER_NOTIFICATION_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint8   is_interim; // figure 26.3 interim message(0x01) rsp (0x00)
    kal_uint8   event_id; // spec appendix H. below fields are defined by event_id
    kal_uint8   status;   // all EVENT except belows
    kal_uint8   identifier[8]; // EVENT_TRACK_CHANGED
	kal_uint8   count;  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint8   attribute_value[BT_AVRCP_MAX_ATTRIBUTE_NUM];  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint16  id; // EVENT_ADDRESSEDPLAYER_CHANGED
	kal_uint16  uid_counter; // EVENT_ADDRESSEDPLAYER_CHANGED or EVENT_UID_CHANGED
	kal_uint8   volume; // EVENT_VOLUME_CHANGED
	kal_uint32  pos; // EVENT_POS_CHANGED
} bt_avrcp_register_notification_rsp_struct;

/* MSG_ID_BT_AVRCP_ABORT_CONTINUERESPONSE_IND */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   pdu_id; // AVRCP specific AV/C command's PDU ID. AVRCP spec. table 4.5
} bt_avrcp_abort_continueresponse_ind_struct;

/* MSG_ID_BT_AVRCP_ABORT_CONTINUERESPONSE_RSP */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_REJECT
    kal_uint8   pdu_id; // AVRCP specific AV/C command's PDU ID. AVRCP spec. table 4.5
} bt_avrcp_abort_continueresponse_rsp_struct;
#endif

/***************** AVRCP 1.3 CT ******************/
/* reject: AVCTP header. error_code: AVRCP spec 6.15.3 */
#define LOCAL_PARA_AVRCP_RESULT \
   kal_uint16	result; \
   kal_uint8	error_code;

typedef struct
{
    kal_uint32  attribute_id;
    kal_uint16  charset;
    kal_uint16  value_len;
    kal_uint16  value_text_offset; // NOTE: it should be put in another data field 
}bt_avrcp_element_attributes_struct;

/* MSG_ID_BT_AVRCP_GET_CAPABILITIES_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   capability_id; /* 0x03 avrcp_event_suppport */
} bt_avrcp_get_capabilities_req_struct;

/* MSG_ID_BT_AVRCP_GET_CAPABILITIES_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint8   capability_id;
	kal_uint8   count; // capability count 
	kal_uint8   capability_or_companyID[9]; // max(8,3*3)
	//kal_uint8   capability[BT_AVRCP_MAX_EVENT_NUM]; // 2..n spec 6.4.1 
	//kal_uint8   company_id[BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE];
} bt_avrcp_get_capabilities_cnf_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_ATTRIBUTE_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
} bt_avrcp_list_playerapp_attribute_req_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_ATTRIBUTE_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	LOCAL_PARA_AVRCP_RESULT
	kal_uint8   count;
	kal_uint8   attribute_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM];
} bt_avrcp_list_playerapp_attribute_cnf_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_VALUE_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   attribute_id; // query the possible values of this attribute id
} bt_avrcp_list_playerapp_value_req_struct;

/* MSG_ID_BT_AVRCP_LIST_PLAYERAPP_VALUE_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
	kal_uint8   attribute_id;
	kal_uint8   count; //availabe values[]
	kal_uint8   attribute_values[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; //possiblel setting values
} bt_avrcp_list_playerapp_value_cnf_struct;

/* MSG_ID_BT_AVRCP_GET_CURPLAYERAPP_VALUE_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
} bt_avrcp_get_curplayerapp_value_req_struct;

/* MSG_ID_BT_AVRCP_GET_CURPLAYERAPP_VALUE_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
	kal_uint8   count;
	kal_uint8   attribute_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
	kal_uint8   attribute_values[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // value of reqex of attribute id 
} bt_avrcp_get_curplayerapp_value_cnf_struct;

/* MSG_ID_BT_AVRCP_SET_PLAYERAPP_VALUE_REQ */
typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
	kal_uint8   values[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // value of reqex of attribute id 
} bt_avrcp_set_playerapp_value_req_struct;

/* MSG_ID_BT_AVRCP_SET_PLAYERAPP_VALUE_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
} bt_avrcp_set_playerapp_value_cnf_struct; //Ack the set-value ( not take effect yet by spec 6.5.4)

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_ATTRIBUTETEXT_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
	kal_uint8   count;
	kal_uint8   attribute_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM];	
} bt_avrcp_get_playerapp_attributetext_req_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_ATTRIBUTETEXT_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
	kal_uint8   count;
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint16  charset[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint8   attribute_string_len[BT_AVRCP_MAX_ATTRIBUTE_NUM];
    kal_uint8   attribute_string[BT_AVRCP_MAX_ATTRIBUTE_NUM][BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE]; // 4*40=160 Byte. Note! 
} bt_avrcp_get_playerapp_attributetext_cnf_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_VALUETEXT_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   attribute_id; // query all text of attribute values of a specific attribute id
    kal_uint8   count;        // available number of attribute values
	kal_uint8   value_ids[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM];
} bt_avrcp_get_playerapp_valuetext_req_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERAPP_VALUETEXT_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint8   attribute_id; 
    kal_uint8   count; 
	kal_uint8   value_ids[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM];
    kal_uint16  charset[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; // results
    kal_uint8   value_string_len[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM]; // query results
    kal_uint8   value_string[BT_AVRCP_MAX_ATTRIBUTE_VALUE_NUM][BT_AVRCP_MAX_ATTRIBUTE_STRING_SIZE]; // 4*40=160 Byte. Note! 
} bt_avrcp_get_playerapp_valuetext_cnf_struct;

/* MSG_ID_BT_AVRCP_INFORM_DISPLAY_CHARSET_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   count; // useless? in spec. figure 26.2. the counter is always 1 
    kal_uint16  charset_ids[BT_AVRCP_MAX_ATTRIBUTE_NUM]; // in spec. 6.5.7. the charset_id is not a array.
} bt_avrcp_inform_display_charset_req_struct;

/* MSG_ID_BT_AVRCP_INFORM_DISPLAY_CHARSET_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
} bt_avrcp_inform_display_charset_cnf_struct;

/* MSG_ID_BT_AVRCP_INFORM_BATTERY_STATUSOFCT_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   battery_status; // spec. 6.5.8
} bt_avrcp_inform_battery_statusofct_req_struct;

/* MSG_ID_BT_AVRCP_INFORM_BATTERY_STATUSOFCT_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
} bt_avrcp_inform_battery_statusofct_cnf_struct;

/* MSG_ID_BT_AVRCP_GET_ELEMENT_ATTRIBUTES_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   identifier[8];
    kal_uint8   count;
    kal_uint32  attributes[BT_AVRCP_MAX_GET_ELEMENT_ATTR_NUM];
} bt_avrcp_get_element_attributes_req_struct;

/* MSG_ID_BT_AVRCP_GET_ELEMENT_ATTRIBUTES_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint8   count; // spec 6.6.1
    bt_avrcp_element_attributes_struct elements[BT_AVRCP_MAX_GET_ELEMENT_ATTR_NUM];
    kal_uint8   element_data[BT_AVRCP_MAX_ELEMENET_BUFFER_SIZE];   
} bt_avrcp_get_element_attributes_cnf_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERSTATUS_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
} bt_avrcp_get_playerstatus_req_struct;

/* MSG_ID_BT_AVRCP_GET_PLAYERSTATUS_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint32  song_length; // spec 6.7.1
    kal_uint32  song_position; // spec 6.7.1
    kal_uint8   play_status; // spec 6.7.1
} bt_avrcp_get_playerstatus_cnf_struct;

/* MSG_ID_BT_AVRCP_REGISTER_NOTIFICATION_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   event_id; // spec appendix H
    kal_uint32  playback_interval; // spec 6.7.1 only available when event_id is EVENT_PLAYBACK_POS_CHANGED
} bt_avrcp_register_notification_req_struct;

/* MSG_ID_BT_AVRCP_REGISTER_NOTIFICATION_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint8   is_interim; // figure 26.3 interim message(0x01) cnf (0x00)
    kal_uint8   event_id; // spec appendix H. below fields are defined by event_id(0x01~0x0d)
    kal_uint8   status;   // all EVENT except belows
    kal_uint8   identifier[8]; // EVENT_TRACK_CHANGED
	kal_uint8   count;  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint8   attribute_id[BT_AVRCP_MAX_ATTRIBUTE_NUM];  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint8   attribute_value[BT_AVRCP_MAX_ATTRIBUTE_NUM];  //EVENT_PLAYER_APPLICATION_SETTING_CHANGED
	kal_uint32  pos; // EVENT_POS_CHANGED
} bt_avrcp_register_notification_cnf_struct;

/* MSG_ID_BT_AVRCP_ABORT_CONTINUERESPONSE_REQ */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    kal_uint8   pdu_id; // AVRCP specific AV/C command's PDU ID. AVRCP spec. table 4.5
} bt_avrcp_abort_continueresponse_req_struct;

/* MSG_ID_BT_AVRCP_ABORT_CONTINUERESPONSE_CNF */
typedef struct
{
	LOCAL_PARA_HDR
    kal_uint8   chnl_num;
    kal_uint8   seq_id;
    LOCAL_PARA_AVRCP_RESULT
    kal_uint8   pdu_id; // AVRCP specific AV/C command's PDU ID. AVRCP spec. table 4.5
} bt_avrcp_abort_continueresponse_cnf_struct;

#else //__BT_AVRCP_V13__
/* AVRCP Error code*/
#define BT_AVRCP_RESULT_SUCCESS       0x1000
#define BT_AVRCP_RESULT_ERROR           0x1001
#define BT_AVRCP_RESULT_INVALID_ROLE   0x1002

#define BT_AVRCP_RESULT_TIMEOUT         0x1003
#define BT_AVRCP_RESULT_BAD_STATE     0x1004
#define BT_AVRCP_RESULT_BUSY               0x1005

#endif  //__BT_AVRCP_V13__

#endif /* #ifndef __BT_AVRCP_STRUCT_H_ */

