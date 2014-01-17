/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
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
 *   vibration_def.h
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Vibration speaker define values
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef __VIBR_DEF_H__
#define __VIBR_DEF_H__

#if defined(__VIBRATION_SPEAKER_SUPPORT__)

#include "audcoeff_default.h"

#if defined(DEFAULT_VS_DELTA_FREQ_5)
#define DEFAULT_VS_DELTA_FREQ 5
#elif defined(DEFAULT_VS_DELTA_FREQ_10)
#define DEFAULT_VS_DELTA_FREQ 10
#else
#error Invalid vibration signal delta frequency
#endif

#define VIBR_FILTER_COEF_NUM 5
#define VIBR_LOUDNESS_FILTER_COEF_NUM 3

#define VIBR_ORDER_SIZE 54
#define VIBR_VALUE_SIZE 53

// Manually sync to "mcu\custom\common\audio_nvram_def.h"
typedef struct {
    kal_bool   VIBR_NVRAM_CALI_DONE;
    kal_uint16 VIBR_NVRAM_CF;
    kal_int16  VIBR_NVRAM_SPH_FILTER_COEF[31];
    kal_uint32 VIBR_NVRAM_LOUD_FILTER_COEF[2][9][3];
} VIBR_NVRAM_STRUCT;

extern VIBR_NVRAM_STRUCT VIBR_NVRAM_DATA;
extern void Vibration_Notch_Filter(int *pSample, int *pOut, int is_stereo, int buffer_size_in_bytes, int *pStack, const int *pCoeff);
extern void Vibration_Notch_Filter_16Bit(short *pSample, short *pOut, int is_stereo, int buffer_size_in_bytes, int *pStack, const int *pCoeff);
extern kal_int32 Vibr_Filter_Stack[2][8];

#if defined(DEFAULT_VS_CENTER_FREQ_153)
#define DEFAULT_VS_CENTER_FREQ 153
#elif defined(DEFAULT_VS_CENTER_FREQ_156)
#define DEFAULT_VS_CENTER_FREQ 156
#elif defined(DEFAULT_VS_CENTER_FREQ_159)
#define DEFAULT_VS_CENTER_FREQ 159
#elif defined(DEFAULT_VS_CENTER_FREQ_162)
#define DEFAULT_VS_CENTER_FREQ 162
#elif defined(DEFAULT_VS_CENTER_FREQ_165)
#define DEFAULT_VS_CENTER_FREQ 165
#elif defined(DEFAULT_VS_CENTER_FREQ_168)
#define DEFAULT_VS_CENTER_FREQ 168
#elif defined(DEFAULT_VS_CENTER_FREQ_171)
#define DEFAULT_VS_CENTER_FREQ 171
#elif defined(DEFAULT_VS_CENTER_FREQ_174)
#define DEFAULT_VS_CENTER_FREQ 174
#elif defined(DEFAULT_VS_CENTER_FREQ_177)
#define DEFAULT_VS_CENTER_FREQ 177
#elif defined(DEFAULT_VS_CENTER_FREQ_180)
#define DEFAULT_VS_CENTER_FREQ 180
#elif defined(DEFAULT_VS_CENTER_FREQ_183)
#define DEFAULT_VS_CENTER_FREQ 183
#elif defined(DEFAULT_VS_CENTER_FREQ_186)
#define DEFAULT_VS_CENTER_FREQ 186
#elif defined(DEFAULT_VS_CENTER_FREQ_189)
#define DEFAULT_VS_CENTER_FREQ 189
#elif defined(DEFAULT_VS_CENTER_FREQ_192)
#define DEFAULT_VS_CENTER_FREQ 192
#elif defined(DEFAULT_VS_CENTER_FREQ_195)
#define DEFAULT_VS_CENTER_FREQ 195
#elif defined(DEFAULT_VS_CENTER_FREQ_198)
#define DEFAULT_VS_CENTER_FREQ 198
#elif defined(DEFAULT_VS_CENTER_FREQ_201)
#define DEFAULT_VS_CENTER_FREQ 201
#elif defined(DEFAULT_VS_CENTER_FREQ_204)
#define DEFAULT_VS_CENTER_FREQ 204
#elif defined(DEFAULT_VS_CENTER_FREQ_207)
#define DEFAULT_VS_CENTER_FREQ 207
#elif defined(DEFAULT_VS_CENTER_FREQ_210)
#define DEFAULT_VS_CENTER_FREQ 210
#else
#error Invalid vibration signal center frequency
#endif

#endif  // __VIBRATION_SPEAKER_SUPPORT__

#endif  // __VIBR_DEF_H__
