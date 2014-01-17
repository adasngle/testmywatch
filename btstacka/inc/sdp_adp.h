#ifndef __SDP_ADP_H
#define __SDP_ADP_H

/****************************************************************************
 *
 * File:
 *     $Workfile:sdp.h$ for iAnywhere Blue SDK, Version 2.1.1
 *     $Revision:94$
 *
 * Description:
 *     Public types, defines, and prototypes for accessing the
 *     Service Discovery Protocol component. 
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
#include "utils.h"

#include "btconfig.h"
#include "config.h"
#include "xatypes.h"
/*---------------------------------------------------------------------------
 * Service Discovery Protocol (SDP) layer
 *
 *     The SDP layer is a "yellow pages" of services available on a device.
 *     With the SDP API, you can create and store "service records" for
 *     remote devices to find. After this is done, SDP handles
 *     all queries from remote devices automatically.
 *
 *     You can also query for the service records on connected remote
 *     devices. To do this, you create a query token, fill it with
 *     the parameters for your query, and pass it to SDP. SDP then
 *     handles the interactions with the remote device, returns the 
 *     query token to you, and helps you parse the results.
 *
 *     Note: unlike many other layers of the Bluetooth protocol,
 *     the SDP uses big-endian format, so more significant bytes are stored
 *     and transferred before less significant bytes.
 */

/****************************************************************************
 *
 * Types and Constants
 *
 ****************************************************************************/
#if (SDP_CLIENT_SUPPORT == XA_ENABLED) && (SDP_PARSING_FUNCS == XA_ENABLED)

/*---------------------------------------------------------------------------
 * SdpParsingMode type
 *
 *     Indicates the parsing mode when calling SDP_ParseAttributes.
 */
typedef U8 SdpParsingMode;

#define BSPM_BEGINNING   0x00   /*
                                 * Used to parse the first SDP response 
                                 * packet from the beginning.  This is always
                                 * the first mode used.  This mode uses the 
                                 * SdpQueryToken "results" and "rLen" buffer 
                                 * fields when it start its parsing.
                                 */

#define BSPM_RESUME      0x01   /*
                                 * Used to parse the same SDP response packet
                                 * from where parsing stopped last.  This mode
                                 * can be used to find an attribute returned
                                 * in multiple SDP records.  This mode uses the
                                 * SdpQueryToken "remainLen" and "remainBuff" 
                                 * buffer fields when it starts its parsing.
                                 */

#define BSPM_CONT_STATE  0x02   /*
                                 * Use to parse the beginning of a 
                                 * continuation state SDP response packet.
                                 * This mode is only valid after performing an
                                 * SDP query using the BSQM_CONTINUE query mode.
                                 * This mode uses the SdpQueryToken "results" 
                                 * and "rLen" buffer fields when it start its 
                                 * parsing.
                                 */

/* End of SdpParsingMode */

#endif /* (SDP_CLIENT_SUPPORT == XA_ENABLED) && (SDP_PARSING_FUNCS == XA_ENABLED) */

/*---------------------------------------------------------------------------
 * SdpQueryMode type
 *
 *     Indicates the mode of a query when calling SDP_Query.
 */
typedef U8 SdpQueryMode;

#define BSQM_FIRST     0x00     /*
                                 * Use this mode to issue the first packet in 
                                 * an SDP query. When using this mode, SDP will 
                                 * internally initialize a 1-byte continuation 
                                 * state parameter containing 0x00 and will store 
                                 * this information in the "contState" and 
                                 * "contStateLen" fields in the SdpQueryToken 
                                 * structure.
                                 */

#define BSQM_CONTINUE  0x01     /*
                                 * Use this mode to send the next packet in an 
                                 * SDP query. An SDP query takes multiple packets
                                 * when a response is received containing non-zero 
                                 * continuation state information. When specifying 
                                 * this mode, you must include the continuation 
                                 * state information in the "contState" and 
                                 * "contStateLen" fields in the SdpQueryToken 
                                 * structure.  If SDP_ParseAttributes was used to
                                 * parse the response and BT_STATUS_SDP_CONT_STATE
                                 * was returned, this data will be automatically
                                 * copied into these SdpQueryToken fields.
                                 */

#define BSQM_DONT_CARE 0xFF     /* Used internally. */

/* End of SdpQueryMode */

/*---------------------------------------------------------------------------
 * SdpQueryType type
 *
 *     Indicates the type of a query when calling SDP_Query.
 */
typedef U8 SdpQueryType;

/*
 * SDP REQUEST:
 *
 * Use for a ServiceSearchRequest query. This query should result in a
 * response token of type BSQR_SERVICE_SEARCH_RESP or BSQR_ERROR_RESP.
 *
 * The query parameters must include the following:
 *
 * 1) A Data Element Sequence of UUIDs that specify the service(s)
 * desired. Between 1 and 12 UUIDs must be present.
 *
 * 2) The maximum number of service record handles to return (a 2-byte
 * integer).
 *
 * SDP RESPONSE:
 *
 * A successful response (BSQR_SERVICE_SEARCH_RESP) to a 
 * BSQR_SERVICE_SEARCH_REQ query includes the following parameters:
 *
 * 1) The total number of matching service records (a 2-byte integer).
 *
 * 2) The total number of service record handles (a 2-byte integer)
 * expressed in this response. (Additional service records may require
 * a second query containing the ContinuationState parameter.)
 *
 * 3) An array of 32-bit service record handles.
 *
 * 4) The ContinuationState parameter.
 *
 * An unsuccessful query results in an error response (BSQR_ERROR_RESP)
 * which has the following parameters:
 * 
 * 1) A two-byte ErrorCode 
 * 
 * 2) An ErrorInfo field (currently undefined).
 *
 * Error codes are expressed using the SdpErrorCode type.
 */
#define BSQT_SERVICE_SEARCH_REQ         0x02

/*
 * SDP REQUEST:
 *
 * Use for a ServiceAttributeRequest query. This query should result in
 * a response token of type BQSR_ATTRIB_RESP or BSQR_ERROR_RESP.
 *
 * The query parameters must include the following:
 *
 * 1) The ServiceRecordHandle (a 4-byte integer) that identifies the
 * service to query.
 *
 * 2) The maximum size, in bytes, of attribute data to be returned
 * (a 2-byte integer). Between 7 and 0xFFFF bytes can be specified.
 *
 * 3) A Data Element Sequence of ascending attribute IDs and ranges,
 * specifying the attributes of interest on the remote device.
 * IDs are 16-bit identifiers. Ranges are 32-bit identifiers with the
 * higher 16 bits specifying the first ID in the range, and the lower
 * 16 bits specifying the last ID in the range.    
 *
 * SDP RESPONSE:
 * 
 * A successful response (BSQR_ATTRIB_RESP) to a BSQR_ATTRIB_REQ 
 * query includes the following parameters:
 *
 * 1) The number of bytes in the attribute list parameter (next).
 * This is stored as a 2-byte integer and ranges between 2 and 0xFFFF.
 *
 * 2) A Data Element Sequence of attribute ID and value pairs, ascending
 * by ID. Attributes with non-null values are not included.
 *
 * 3) The ContinuationState parameter.
 *
 * An unsuccessful query results in an error response (BSQR_ERROR_RESP)
 * which has the following parameters:
 * 
 * 1) A two-byte ErrorCode 
 * 
 * 2) An ErrorInfo field (currently undefined).
 *
 * Error codes are expressed using the SdpErrorCode type.
 */
#define BSQT_ATTRIB_REQ                 0x04

/*
 * SDP REQUEST:
 * 
 * Use for a ServiceSearchAttributeRequest query, which searches for
 * services and attributes simultaneously. This query should
 * result in a response token of type BSQR_SERVICE_SEARCH_ATTRIB_RESP
 * or BSQR_ERROR_RESP.
 *
 * The query parameters must include the following:
 *
 * 1) A Data Element Sequence of UUIDs that specify the service(s)
 * desired. Between 1 and 12 UUIDs must be present.
 *
 * 2) The maximum size, in bytes, of attribute data to be returned
 * (a 2-byte integer). Between 7 and 0xFFFF bytes can be specified.
 *
 * 3) A Data Element Sequence of ascending attribute IDs and ranges,
 * specifying the attributes of interest on the remote device.
 * IDs are 16-bit identifiers. Ranges are 32-bit identifiers with the
 * higher 16 bits specifying the first ID in the range, and the lower
 * 16 bits specifying the last ID in the range.    
 *
 * SDP RESPONSE:
 *
 * A successful response (BSQR_SERVICE_SEARCH_ATTRIB_RESP) to a 
 * BSQR_SERVICE_SEARCH_ATTRIB_REQ query includes the following 
 * parameters:
 *
 * 1) The number of bytes in the attribute list parameter (next).
 * This is stored as a 2-byte integer and ranges between 2 and 0xFFFF.
 *
 * 2) A Data Element Sequence, where each element corresponds to a
 * particular service record matching the original pattern.
 * Each element of the sequence is itself a Data Element Sequence of
 * attribute ID and value pairs, ascending by ID.
 *
 * 3) The ContinuationState parameter.
 *
 * An unsuccessful query results in an error response (BSQR_ERROR_RESP)
 * which has the following parameters:
 * 
 * 1) A two-byte ErrorCode 
 * 
 * 2) An ErrorInfo field (currently undefined).
 *
 * Error codes are expressed using the SdpErrorCode type.
 */
#define BSQT_SERVICE_SEARCH_ATTRIB_REQ  0x06

/* End of SdpQueryType */

/*---------------------------------------------------------------------------
 * SdpErrorCode type
 *
 *     Indicates the type of error that caused the query to fail
 *     on the remote device. This is expressed as a parameter in
 *     a BSQR_ERROR_RESP response.
 */
typedef U16 SdpErrorCode;

#define BSEC_BAD_VERSION      0x0001    /* Invalid/unsupported SDP version */
#define BSEC_BAD_HANDLE       0x0002    /* Invalid service record handle */
#define BSEC_BAD_SYNTAX       0x0003    /* Invalid request syntax */
#define BSEC_BAD_PDU_SIZE     0x0004    /* Invalid PDU size */
#define BSEC_BAD_CONTINUATION 0x0005    /* Invalid ContinuationState */
#define BSEC_OUT_OF_RESOURCES 0x0006    /* 
                                         * Insufficient resources to satisfy
                                         * request 
                                         */
/* End of SdpErrorCode */

/*---------------------------------------------------------------------------
 * SdpDataElemType type
 *
 *     Specifies the type of a Data Element.
 *
 *     Data Elements begin with a single byte that contains both type and
 *     size information. To read the type from this byte, use the
 *     SDP_GetElemType macro.
 *
 *     To create the first byte of a Data Element, bitwise OR the
 *     SdpDataElemType and SdpDataElemSize values into a single byte.
 */
typedef U8 SdpDataElemType;

#define DETD_NIL  0x00  /*
                         * Specifies nil, the null type.
                         * Requires a size of DESD_1BYTE, which for this type
                         * means an actual size of 0.
                         */
#define DETD_UINT 0x08  /*
                         * Specifies an unsigned integer. Must use size
                         * DESD_1BYTE, DESD_2BYTES, DESD_4BYTES, DESD_8BYTES,
                         * or DESD_16BYTES.
                         */
#define DETD_SINT 0x10  /*
                         * Specifies a signed integer. May use size
                         * DESD_1BYTE, DESD_2BYTES, DESD_4BYTES, DESD_8BYTES,
                         * or DESD_16BYTES.
                         */
#define DETD_UUID 0x18  /*
                         * Specifies a Universally Unique Identifier (UUID).
                         * Must use size DESD_2BYTES, DESD_4BYTES, or
                         * DESD_16BYTES.
                         */
#define DETD_TEXT 0x20  /*
                         * Specifies a text string. Must use sizes
                         * DESD_ADD_8BITS, DESD_ADD_16BITS, or DESD_ADD32_BITS.
                         */
#define DETD_BOOL 0x28  /*
                         * Specifies a boolean value. Must use size
                         * DESD_1BYTE.
                         */
#define DETD_SEQ  0x30  /*
                         * Specifies a data element sequence. The data contains
                         * a sequence of Data Elements. Must use sizes
                         * DESD_ADD_8BITS, DESD_ADD_16BITS, or DESD_ADD_32BITS.
                         */
#define DETD_ALT  0x38  /*
                         * Specifies a data element alternative. The data contains
                         * a sequence of Data Elements. This type is sometimes
                         * used to distinguish between two possible sequences.
                         * Must use size DESD_ADD_8BITS, DESD_ADD_16BITS,
                         * or DESD_ADD_32BITS.
                         */
#define DETD_URL  0x40  /*
                         * Specifies a Uniform Resource Locator (URL).
                         * Must use size DESD_ADD_8BITS, DESD_ADD_16BITS,
                         * or DESD_ADD_32BITS.
                         */

#define DETD_MASK 0xf8  /*
                         * AND this value with the first byte of a Data
                         * Element to return the element's type.
                         */

/* End of SdpDataElemType */

/*---------------------------------------------------------------------------
 * SdpDataElemSize type
 *
 *     Specifies the size of a Data Element.
 *
 *     Data Elements begin with a single byte that contains both type and
 *     size information. To read the size from this byte, use the
 *     SDP_GetElemSize macro.
 *
 *     To create the first byte of a Data Element, bitwise OR the
 *     SdpDataElemType and SdpDataElemSize values into a single byte.
 *     For example, a standard 16 bit unsigned integer with a value of 0x57
 *     could be encoded as follows:
 * 
 *     U8 val[3] = { DETD_UINT | DESD_2BYTES, 0x00, 0x57 };
 *
 *     The text string "hello" could be encoded as follows:
 *
 *     U8 str[7] = { DETD_TEXT | DESD_ADD_8BITS, 0x05, 'h','e','l','l','o' };
 */
typedef U8 SdpDataElemSize;

#define DESD_1BYTE      0x00    /*
                                 * Specifies a 1-byte element. However, if
                                 * type is DETD_NIL then the size is 0.
                                 */
#define DESD_2BYTES     0x01    /* Specifies a 2-byte element. */
#define DESD_4BYTES     0x02    /* Specifies a 4-byte element. */
#define DESD_8BYTES     0x03    /* Specifies an 8-byte element. */
#define DESD_16BYTES    0x04    /* Specifies a 16-byte element. */
#define DESD_ADD_8BITS  0x05    /*
                                 * The element's actual data size, in bytes,
                                 * is contained in the next 8 bits.
                                 */
#define DESD_ADD_16BITS 0x06    /*
                                 * The element's actual data size, in bytes,
                                 * is contained in the next 16 bits.
                                 */
#define DESD_ADD_32BITS 0x07    /*
                                 * The element's actual data size, in bytes,
                                 * is contained in the next 32 bits.
                                 */

#define DESD_MASK       0x07    /*
                                 * AND this value with the first byte of a Data
                                 * Element to return the element's size.
                                 */

/* End of SdpDataElemSize */

/*---------------------------------------------------------------------------
 * SdpAttributeId type
 *
 *     Represents an attribute ID.
 *
 *     Attribute are identified by attribute ID. This type includes
 *     many (but not all) of the possible attributes available on
 *     a Bluetooth device.
 *
 *     Higher layer services may use these attributes, but may also
 *     need to define their own. In this case, the service must define
 *     attributes with IDs between 0x0200 through 0xFFFF.
 */
typedef U16 SdpAttributeId;

/*
 * Group: The following attributes are required to be present in all
 * service records on all Bluetooth devices.
 */

/*
 * A 32-bit UINT that uniquely identifies the service record for a
 * particular SDP server.
 */
#define AID_SERVICE_RECORD_HANDLE               0x0000

/*
 * A Data Element Sequence of UUIDs. Each UUID represents a service
 * class supported by the service record. At least one UUID must
 * be present.
 *
 * The SdpServiceClassUuid type represents these UUIDs.
 */
#define AID_SERVICE_CLASS_ID_LIST               0x0001

/*
 * Group: The following attributes are "universal" to all service records,
 * meaning that the same attribute IDs are always used. However, attributes
 * may or may not be present within a service record.
 *
 * See the Bluetooth Core specification, Service Discovery Protocol (SDP)
 * chapter, section 5.1 for more detailed explanations of these attributes.
 */

#define AID_SERVICE_RECORD_STATE                0x0002
#define AID_SERVICE_ID                          0x0003
#define AID_PROTOCOL_DESC_LIST                  0x0004
#define AID_BROWSE_GROUP_LIST                   0x0005
#define AID_LANG_BASE_ID_LIST                   0x0006
#define AID_SERVICE_INFO_TIME_TO_LIVE           0x0007
#define AID_SERVICE_AVAILABILITY                0x0008
#define AID_BT_PROFILE_DESC_LIST                0x0009
#define AID_DOC_URL                             0x000a
#define AID_CLIENT_EXEC_URL                     0x000b
#define AID_ICON_URL                            0x000c
#define AID_ADDITIONAL_PROT_DESC_LISTS          0x000d

/*
 * Group: The following "universal" attribute IDs must be added to
 * the appropriate value from the AID_LANG_BASE_ID_LIST attribute (usually 
 * 0x0100).
 */
#define AID_SERVICE_NAME                        0x0000
#define AID_SERVICE_DESCRIPTION                 0x0001
#define AID_PROVIDER_NAME                       0x0002

/* Personal Area Networking Profile */
#define AID_IP_SUBNET                           0x0200

/*
 * Group: The following attribute applies only to a service record that
 * corresponds to a BrowseGroupDescriptor service.
 */

/* A UUID used to locate services that are part of the browse group. */
#define AID_GROUP_ID                            0x0200

/*
 * Group: The following attributes apply only to the service record that
 * corresponds to the Service Discovery Server itself. Therefore, they
 * are valid only when the AID_SERVICE_CLASS_ID_LIST contains
 * a UUID of SC_SERVICE_DISCOVERY_SERVER.
 */
#define AID_VERSION_NUMBER_LIST                 0x0200
#define AID_SERVICE_DATABASE_STATE              0x0201

/*
 * Group: The following attributes are for use by specific profiles as
 * defined in the profile specification.
 */
#define AID_SERVICE_VERSION                     0x0300

/* Cordless Telephony Profile */
#define AID_EXTERNAL_NETWORK                    0x0301

/* Synchronization Profile */
#define AID_SUPPORTED_DATA_STORES_LIST          0x0301

/* Fax Class 1 */
#define AID_FAX_CLASS_1_SUPPORT                 0x0302

/* GAP Profile */
#define AID_REMOTE_AUDIO_VOL_CONTROL            0x0302

/* Fax Class 2.0 */
#define AID_FAX_CLASS_20_SUPPORT                0x0303

/* Object Push Profile */
#define AID_SUPPORTED_FORMATS_LIST              0x0303

/* Fax Service Class 2 - Manufacturer specific */
#define AID_FAX_CLASS_2_SUPPORT                 0x0304
#define AID_AUDIO_FEEDBACK_SUPPORT              0x0305

/* Bluetooth as WAP requirements */
#define AID_NETWORK_ADDRESS                     0x0306
#define AID_WAP_GATEWAY                         0x0307
#define AID_HOME_PAGE_URL                       0x0308
#define AID_WAP_STACK_TYPE                      0x0309

/* Personal Area Networking Profile */
#define AID_SECURITY_DESC                       0x030A
#define AID_NET_ACCESS_TYPE                     0x030B
#define AID_MAX_NET_ACCESS_RATE                 0x030C
#define AID_IPV4_SUBNET                         0x030D
#define AID_IPV6_SUBNET                         0x030E

/* Imaging Profile */
#define AID_SUPPORTED_CAPABILITIES              0x0310
#define AID_SUPPORTED_FEATURES                  0x0311
#define AID_SUPPORTED_FUNCTIONS                 0x0312
#define AID_TOTAL_IMAGE_DATA_CAPACITY           0x0313

/* Phonebook Access Profile */
#define AID_SUPPORTED_REPOSITORIES              0x0314  /* Awaiting for an official value from CAR WG */

/* Basic Printing Profile */
#define AID_SUPPORTED_DOC_FORMATS               0x0350
#define AID_SUPPORTED_CHAR_REPERTOIRES          0x0352
#define AID_SUPPORTED_XHTML_IMAGE_FORMATS       0x0354
#define AID_COLOR_SUPPORTED                     0x0356
#define AID_PRINTER_1284ID                      0x0358
#define AID_DUPLEX_SUPPORTED                    0x035E
#define AID_SUPPORTED_MEDIA_TYPES               0x0360
#define AID_MAX_MEDIA_WIDTH                     0x0362
#define AID_MAX_MEDIA_LENGTH                    0x0364

/****************************************************************************
 * HID Specific Attribute IDs
 ****************************************************************************/

#define AID_HID_DEVICE_RELEASE        0x0200
#define AID_HID_PARSER_VERSION        0x0201
#define AID_HID_DEVICE_SUBCLASS       0x0202
#define AID_HID_COUNTRY_CODE          0x0203
#define AID_HID_VIRTUAL_CABLE         0x0204
#define AID_HID_RECONNECT_INIT        0x0205
#define AID_HID_DESCRIPTOR_LIST       0x0206
#define AID_HID_LANG_ID_BASE_LIST     0x0207
#define AID_HID_SDP_DISABLE           0x0208
#define AID_HID_BATTERY_POWER         0x0209
#define AID_HID_REMOTE_WAKE           0x020A
#define AID_HID_PROFILE_VERSION       0x020B
#define AID_HID_SUPERV_TIMEOUT        0x020C
#define AID_HID_NORM_CONNECTABLE      0x020D
#define AID_HID_BOOT_DEVICE           0x020E

/* End of SdpAttributeId */

/*---------------------------------------------------------------------------
 * SdpServiceClassUuid type
 *
 *     Represents the UUID associated with a specific service and
 *     profile.
 *
 *     Any number of these UUIDs may be present in the
 *     AID_SERVICE_CLASS_ID_LIST attribute of a service record, and may
 *     appear in the AID_BT_PROFILE_DESC_LIST.
 */
typedef U16 SdpServiceClassUuid;

/* Service Discovery Server service. */
#define SC_SERVICE_DISCOVERY_SERVER             0x1000

/* Browse Group Descriptor service. */
#define SC_BROWSE_GROUP_DESC                    0x1001

/* Public Browse Group service. */
#define SC_PUBLIC_BROWSE_GROUP                  0x1002

/* Serial Port service and profile. */
#define SC_SERIAL_PORT                          0x1101

/* LAN Access over PPP service. */
#define SC_LAN_ACCESS_PPP                       0x1102

/* Dial-up networking service and profile. */
#define SC_DIALUP_NETWORKING                    0x1103

/* IrMC Sync service and Synchronization profile. */
#define SC_IRMC_SYNC                            0x1104

/* OBEX Object Push service and Object Push profile. */
#define SC_OBEX_OBJECT_PUSH                     0x1105

/* OBEX File Transfer service and File Transfer profile. */
#define SC_OBEX_FILE_TRANSFER                   0x1106

/*
 * IrMC Sync service and Synchronization profile
 * (Sync Command Scenario).
 */
#define SC_IRMC_SYNC_COMMAND                    0x1107

/* Headset service and profile. */
#define SC_HEADSET                              0x1108

/* Cordless telephony service and profile. */
#define SC_CORDLESS_TELEPHONY                   0x1109

/* Audio Source */
#define SC_AUDIO_SOURCE                         0x110A

/* Audio Sink */
#define SC_AUDIO_SINK                           0x110B

/* Audio/Video Remote Control Target */
#define SC_AV_REMOTE_CONTROL_TARGET             0x110C

/* Advanced Audio Distribution Profile */
#define SC_AUDIO_DISTRIBUTION                   0x110D

/* Audio/Video Remote Control */
#define SC_AV_REMOTE_CONTROL                    0x110E

/* Audio/Video Remote Control - 20100304 BT SIG UUID */
#define SC_AV_REMOTE_CONTROL_CONTROL            0x110F

/* Video Conferencing Profile */
#define SC_VIDEO_CONFERENCING                   0x110F

/* Intercom service and profile. */
#define SC_INTERCOM                             0x1110

/* Fax service and profile. */
#define SC_FAX                                  0x1111

/* Headset Audio Gateway */
#define SC_HEADSET_AUDIO_GATEWAY                0x1112

/* WAP service */
#define SC_WAP                                  0x1113

/* WAP client service */
#define SC_WAP_CLIENT                           0x1114

/* Personal Area Networking Profile */
#define SC_PANU                                 0x1115

/* Personal Area Networking Profile */
#define SC_NAP                                  0x1116

/* Personal Area Networking Profile */
#define SC_GN                                   0x1117

/* Basic Printing Profile */
#define SC_DIRECT_PRINTING                      0x1118

/* Basic Printing Profile */
#define SC_REFERENCE_PRINTING                   0x1119

/* Imaging Profile */
#define SC_IMAGING                              0x111A

/* Imaging Profile */
#define SC_IMAGING_RESPONDER                    0x111B

/* Imaging Profile */
#define SC_IMAGING_AUTOMATIC_ARCHIVE            0x111C

/* Imaging Profile */
#define SC_IMAGING_REFERENCED_OBJECTS           0x111D

/* Handsfree Profile */
#define SC_HANDSFREE                            0x111E

/* Handsfree Audio Gateway */
#define SC_HANDSFREE_AUDIO_GATEWAY              0x111F

/* Basic Printing Profile */
#define SC_DIRECT_PRINTING_REF_OBJECTS          0x1120

/* Basic Printing Profile */
#define SC_REFLECTED_UI                         0x1121

/* Basic Printing Profile */
#define SC_BASIC_PRINTING                       0x1122

/* Basic Printing Profile */
#define SC_PRINTING_STATUS                      0x1123

/* Human Interface Device Profile */
#define SC_HUMAN_INTERFACE_DEVICE               0x1124

/* Hardcopy Cable Replacement Profile */
#define SC_HCR                                  0x1125

/* Hardcopy Cable Replacement Profile */
#define SC_HCR_PRINT                            0x1126

/* Hardcopy Cable Replacement Profile */
#define SC_HCR_SCAN                             0x1127

/* Common ISDN Access / CAPI Message Transport Protocol */
#define SC_ISDN                                 0x1128

/* Video Conferencing Gateway */
#define SC_VIDEO_CONFERENCING_GW                0x1129

/* Unrestricted Digital Information Mobile Termination */
#define SC_UDI_MT                               0x112A

/* Unrestricted Digital Information Terminal Adapter */
#define SC_UDI_TA                               0x112B

/* Audio Video service */
#define SC_AUDIO_VIDEO                          0x112C

/* SIM Access Profile */
#define SC_SIM_ACCESS                           0x112D

/* Phonebook Access Client */
#define SC_PBAP_CLIENT                          0x112E

/* Phonebook Access Server */
#define SC_PBAP_SERVER                          0x112F

/* Phonebook Access */
#define SC_PBAP                                 0x1130

/* Message Access Server */
#define SC_MAP_SERVER                           0x1132

/* Message Notification Server */
#define SC_MAP_MNS_SERVER                       0x1133

/* Message Access Profile */
#define SC_MAP                                  0x1134

/* Plug-n-Play service */
#define SC_PNP_INFO                             0x1200

/* Generic Networking service. */
#define SC_GENERIC_NETWORKING                   0x1201

/* Generic File Transfer service. */
#define SC_GENERIC_FILE_TRANSFER                0x1202

/* Generic Audio service. */
#define SC_GENERIC_AUDIO                        0x1203

/* Generic Telephony service. */
#define SC_GENERIC_TELEPHONY                    0x1204

/* UPnP L2CAP based profile. */
#define SC_UPNP_SERVICE                         0x1205

/* UPnP IP based profile. */
#define SC_UPNP_IP_SERVICE                      0x1206

/* UPnP IP based solution using PAN */
#define SC_ESDP_UPNP_IP_PAN                     0x1300

/* UPnP IP based solution using LAP */
#define SC_ESDP_UPNP_IP_LAP                     0x1301

/* UPnP L2CAP based solution */
#define SC_ESDP_UPNP_L2CAP                      0x1302

#ifdef __BT_SUPPORT_SYNCML__
/* SyncML uses OBEX over Bluetooth */
/* Using 0xFF01 to represent real SyncML_Server UUID 00000001-0000-1000-8000-0002EE000002, because token->uuid only 16-bits */
#define SC_SYNCML_BT_SERVER 0xFF01 
#define SC_SYNCML_BT_SERVER_128 0x00,0x00,0x00,0x01,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x02,0xEE,0x00,0x00,0x02
/* Using 0xFF02 to represent real SyncML_Client UUID 00000002-0000-1000-8000-0002EE000002, because token->uuid only 16-bits */
#define SC_SYNCML_BT_CLIENT 0xFF02
#define SC_SYNCML_BT_CLIENT_128 0x00,0x00,0x00,0x02,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x02,0xEE,0x00,0x00,0x02
#endif /* __BT_SUPPORT_SYNCML__ */

/* SPP Server uses the range from SC_SERIAL_PORT0 to (SC_SERIAL_PORT0+NUM_SPP_SRVS-1) */
#define SC_SERIAL_PORT0                         0x9001
#if SPP_SERVER == XA_ENABLED
#define SC_SPP_SCO0                      SC_SERIAL_PORT0 + NUM_SPP_SRVS
#else 
#define SC_SPP_SCO0                      SC_SERIAL_PORT0 + 1
#endif 

#if SPP_SCO_SERVER == XA_ENABLED
#define SC_RFCOMM_TESTER                        SC_SPP_SCO0 + NUM_SPP_SCO_SRVS
#else 
#define SC_RFCOMM_TESTER                        SC_SPP_SCO0 + 1
#endif 

/* End of SdpServiceClassUuid */

/*---------------------------------------------------------------------------
 * SdpProtocolUuid type
 *
 *     Represents the UUID associated with a protocol.
 *
 *     Any number of these UUIDs may be present in the
 *     AID_SERVICE_CLASS_ID_LIST attribute of a service record, and may
 *     appear in the AID_BT_PROFILE_DESC_LIST.
 */
typedef U16 SdpProtocolUuid;

/* Service Discovery Protocol */
#define PROT_SDP                     0x0001

/* UDP Protocol */
#define PROT_UDP                     0x0002

/* RFCOMM Protocol */
#define PROT_RFCOMM                  0x0003

/* TCP Protocol */
#define PROT_TCP                     0x0004

/* TCS Binary Protocol */
#define PROT_TCS_BIN                 0x0005

/* TCS-AT Protocol */
#define PROT_TCS_AT                  0x0006

/* OBEX Protocol */
#define PROT_OBEX                    0x0008

/* IP Protocol */
#define PROT_IP                      0x0009

/* FTP Protocol */
#define PROT_FTP                     0x000A

/* HTTP Protocol */
#define PROT_HTTP                    0x000C

/* WSP Protocol */
#define PROT_WSP                     0x000E

/* BNEP Protocol */
#define PROT_BNEP                    0x000F

/* Universal Plug and Play */
#define PROT_UPNP                    0x0010

/* Human Interface Device Profile */
#define PROT_HIDP                    0x0011

/* Hardcopy Cable Replacement Control Channel */
#define PROT_HCR_CONTROL_CHANNEL     0x0012

/* Hardcopy Cable Replacement Data Channel */
#define PROT_HCR_DATA_CHANNEL        0x0014

/* Hardcopy Cable Replacement Notification */
#define PROT_HCR_NOTIFICATION        0x0016

/* Audio/Video Control Transport Protocol */
#define PROT_AVCTP                   0x0017

/* Audio/Video Distribution Transport Protocol */
#define PROT_AVDTP                   0x0019

/* CAPI Message Transport Protocol */
#define PROT_CMPT                    0x001B

/* Unrestricted Digital Information Control Plane */
#define PROT_UDI_C                   0x001D

/* L2CAP Protocol */
#define PROT_L2CAP                   0x0100

/* End of SdpProtocolUuid */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

#if SDP_CLIENT_SUPPORT == XA_ENABLED
#if SDP_PARSING_FUNCS == XA_ENABLED
/*---------------------------------------------------------------------------
 * SdpClientParsingStateInfo structure
 *
 *     Contains all the state information stored by the SDP Client when 
 *     parsing an SDP response.  This state information is mainly necessary 
 *     in the case of a fragmented response (continuation state).
 */
typedef struct _SdpClientParsingStateInfo
{
    U8 state;   /* Parsing state */

    U16 attribByteCount;    /* Attribute Byte Count length */
    U32 attribListLen;      /* Attribute List length */
    U16 attribId;           /* Attribute ID being processed */
    U32 attribIdLen;        /* Attribute Id List length */
    U32 attribSkipLen;      /* Attribute length being skipped */
    U32 resultValueLen;     /* Result length */

    U8 storedBuff[17];  /* Stored buffer */
    U8 storedLen;       /* Stored buffer length */
    U8 curStoredOff;    /* Offset into the stored buffer */

    U8 flags;   /* For error checking */
} SdpClientParsingStateInfo;
#endif /* SDP_PARSING_FUNCS == XA_ENABLED */

/*---------------------------------------------------------------------------
 * SdpQueryToken structure
 *
 *     Defines a query and expresses its results.
 *
 *     Pass this structure to SDP_Query to initiate a query and 
 *     SDP_ParseAttributes to parse the query results. The query results
 *     are returned during the callback function with an event of
 *     SDEVENT_QUERY_RSP, SDEVENT_QUERY_ERR, or SDEVENT_QUERY_FAILED.
 */
typedef struct _SdpQueryToken
{
    ListEntry node; /* For internal use only. */

    /*
     * Group: The following fields must be filled in before calling
     * SDP_Query.
     */
    BtRemoteDevice *rm; /* 
                         * The remote device to query. An ACL
                         * connection must exist with this device
                         * before the query is started. 
                         */

    SdpQueryType type;  /* Type of query to send. */

    const U8 *parms;    /*
                         * Parameters to be sent with the query.
                         * The caller of SDP_Query must make sure
                         * that this buffer is composed correctly.
                         */

    U16 plen;   /* Number of bytes in the "parms" buffer. */

    BtCallBack callback;    /* Function to call with results */

    /*
     * Group: The following fields must be filled in when calling
     * SDP_Query with the BSQM_CONTINUE query mode.  SDP_ParseAttributes
     * will automatically fill in these fields if BT_STATUS_SDP_CONT_STATE 
     * was returned.  However, if SDP_ParseAttributes is not used, these 
     * fields must be modified manually.
     */

    U8 *contState;  /* Continuation State information */

    U8 contStateLen;    /*
                         * Continuation State length. This value 
                         * must range from 1-17 bytes.
                         */

    /*
     * Group: The following fields are filled in by SDP after completion
     * of the query. These fields should not be modified and are only
     * valid during the SDEVENT_QUERY_RSP or SDEVENT_QUERY_ERR
     * callbacks. After the callback returns, the fields are invalid.
     */

    SdpErrorCode errorCode;     /*
                                 * Error code if query failed. Invalid if
                                 * the query succeeded.
                                 */

    /*
     * SDP_ParseAttributes uses these values when the BSPM_BEGINNING or
     * BSPM_CONT_STATE parsing modes are used to parse current SDP 
     * response packet.
     */
    U8 *results;    /* Pointer to query result parameters. */

    U16 rLen;   /* Number of bytes in results buffer. */

#if SDP_PARSING_FUNCS == XA_ENABLED
    /*
     * Group: The following fields must be filled in before calling
     * SDP_ParseAttributes.
     */
    SdpAttributeId attribId;    /* Attribute ID to search for */
    U16 uuid;                   /* UUID to search for (use 0 if unused) */
    SdpParsingMode mode;        /* SDP parsing mode (see SdpParsingMode) */

    /*
     * Group: The following fields are filled in by SDP after
     * completion of SDP_ParseAttributes.
     */

    /*
     * Valid when BT_STATUS_SUCCESS and possibly BT_STATUS_SDP_CONT_STATE are
     * returned from SDP_ParseAttributes.  Partial results may be returned,
     * and therefore should not be evaluated until the entire result is 
     * received.
     */
    U32 totalValueLen;  /* Total length of the SDP result value */
    U16 availValueLen;  /* Available length of the SDP result value */
    U8 *valueBuff;      /* Pointer to the available SDP result value */

    /*
     * Valid when BT_STATUS_SUCCESS is returned from SDP_ParseAttributes.
     * SDP_ParseAttributes uses these values when the BSPM_RESUME parsing mode 
     * is used to parse the remaining data in the current SDP response packet.
     */
    U8 *remainBuff; /* Pointer to the remaining results buffer */
    U16 remainLen;  /* Length of the remaining results buffer */

    /* Group: The following field is for internal use only. */
    SdpClientParsingStateInfo parseData;
#endif /* SDP_PARSING_FUNCS == XA_ENABLED */

    /* Group: The following field is for internal use only. */
    U16 transId;

} SdpQueryToken;
#endif /* SDP_CLIENT_SUPPORT == XA_ENABLED */

#if SDP_SERVER_SUPPORT == XA_ENABLED
/*---------------------------------------------------------------------------
 * SdpAttribute structure
 *
 *     Defines an attribute's ID and value. SdpAttribute structures
 *     are stored in a SdpRecord prior to calling the SDP_AddRecord
 *     function. 
 */
typedef struct _SdpAttribute
{
    SdpAttributeId id;  /* Attribute ID. */

    U16 len;    /* Length of the value buffer */

    const U8 *value;    /*
                         * An array of bytes that contains the value
                         * of the attribute. The buffer is in
                         * Data Element form (see SdpDataElemType
                         * and SdpDataElemSize).
                         */

    /* Group: The following field is for internal use only */
    U16 flags;

} SdpAttribute;

/*---------------------------------------------------------------------------
 * SdpRecord structure
 *
 *     Defines the contents of a service record. Service records are
 *     initialized and provided to SDP_AddRecord.
 */
typedef struct _SdpRecord
{
    ListEntry node; /* For internal use only. */

    U8 num; /* Total number of attributes in "attribs". */

    /*
     * Pointer to an array of attributes.
     *
     * Store the attributes in ascending order by attribute ID, and
     * do not add two attributes with the same ID.
     *
     * Do not include an AID_SERVICE_RECORD_HANDLE attribute. This
     * attribute is handled automatically by SDP.
     */
    SdpAttribute *attribs;

    /*
     * The service class fields of the class of device. Use the values defined
     * in me.h. The device class portion is ignored.
     */
    BtClassOfDevice classOfDevice;

    /* Group: The following fields are for internal use only. */
    U32 handle;         /* Service Record Handle */
    U32 recordState;    /* Service Record State */
    U16 flags;          /* Flag to keep track of marked attributes */
    U16 handleFlag;     /* Flag to keep track of service record handle */
    U16 stateFlag;      /* Flag to keep track of service record state */

} SdpRecord;
#endif /* SDP_SERVER_SUPPORT == XA_ENABLED */
#endif /* __SDP_ADP_H */ 

