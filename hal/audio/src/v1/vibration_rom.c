/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
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
 *   vibration_rom.c
 *
 * Project:
 * --------
 *   Maui
 *
 * Description:
 * ------------
 *   Vibration speaker coefficient
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
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#if defined(__VIBRATION_SPEAKER_SUPPORT__)

#include "vibration_def.h"
#include "afe_def.h"

#if defined(MT6252)

const kal_uint16 DIGITAL_GAIN_TAB[6] = {0x101D, 0x16C2, 0x2026, 0x2D6A, 0x4026, 0x5A9D};
const kal_uint16 DIGITAL_GAIN_MAX_TAB[16] = { 0x779F, 0x78F3, 0x7A11, 0x7B02, 0x7BCC, 0x7C77, 0x7D06, 0x7D7F, 0x7DE5, 0x7E3A, 0x7E82, 0x7EBF, 0x7EF1, 0x7F1C, 0x7F40, 0x7F5F };
const kal_uint16 VIBR_VS_DIGI_GAIN[2][16] = 
{
   0x42C8, 0x2F47, 0x2178, 0x17B2, 0x10C6, 0x0BE0, 0x0867, 0x05F3, 0x0436, 0x02FB, 0x021C, 0x017E, 0x010E, 0x00BF, 0x0087, 0x0000, 
   0x4F5F, 0x3830, 0x27C7, 0x1C29, 0x13EF, 0x0E1D, 0x09FD, 0x0712, 0x0501, 0x038B, 0x0281, 0x01C6, 0x0142, 0x00E3, 0x00A1, 0x0072
};

#elif defined(MT6250) || defined(MT6260)

const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_common[15] = {
    0x040C, // 0x0
    0x0518, // 0x1
    0x066A, // 0x2
    0x0813, // 0x3
    0x0A2A, // 0x4
    0x0CCC, // 0x5
    0x101D, // 0x6
    0x1449, // 0x7
    0x1989, // 0x8
    0x2026, // 0x9
    0x2879, // 0xA
    0x32F4, // 0xB
    0x4026, // 0xC
    0x50C2, // 0xD
    0x65AB, // 0xE
};

#if defined(VIBR_AMP_560MVRMS)

const kal_uint16 vib_digi_gain_tab_for_spk_amp_0_2_3[13] = {0x79A5, 0x60A0, 0x4CC0, 0x3CF7, 0x306D, 0x2677, 0x1E8E, 0x1845, 0x1347, 0x0F50, 0x0C2A, 0x09A9, 0x07AC};
const kal_uint16 vib_digi_gain_tab_for_spk_amp_1[5] = {0x6C6A, 0x561E, 0x4468, 0x3656, 0x2B29};
const kal_uint16 vib_digi_gain_tab_idx = 0xCB63;
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_bottom[4] = {0x4308, 0x54D6, 0x70AF, 0x7853};
const kal_uint16 vibr_plus_aud_voice_amp_tab[4][4] = {
    0xDDDD, 0xDDDE, 0xEEEF, 0xFFFF, // spk 0
    0xBBBC, 0xCCCC, 0xDDEE, 0xFFFF, // spk 1
    0x8888, 0x899A, 0xBBCD, 0xDEFF, // spk 2
    0x8888, 0x8889, 0xAABC, 0xDEFF, // spk 3
};
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_idx[4][4] = {
    0x2345, 0x6788, 0x9ABB, 0xCFFF, // spk 0
    0x4566, 0x789A, 0xABBC, 0xCDFF, // spk 1
    0x789A, 0xBBCC, 0xCDDD, 0xEEEF, // spk 2
    0x789A, 0xBCDD, 0xDEEE, 0xEEEF, // spk 3
};

#elif defined(VIBR_AMP_490MVRMS)

const kal_uint16 vib_digi_gain_tab_for_spk_amp_0_2_3[13] = {0x6A70, 0x548C, 0x4328, 0x3558, 0x2A5F, 0x21A8, 0x1ABC, 0x153C, 0x10DE, 0x0D66, 0x0AA4, 0x0874, 0x06B7};
const kal_uint16 vib_digi_gain_tab_for_spk_amp_1[6] = {0x776D, 0x5EDD, 0x4B5A, 0x3BDB, 0x2F8B, 0x25C4};
const kal_uint16 vib_digi_gain_tab_idx = 0xCA63;
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_bottom[4] = {0x4AA7, 0x5A3B, 0x7299, 0x7948};
const kal_uint16 vibr_plus_aud_voice_amp_tab[4][4] = {
    0xCCCC, 0xCDDD, 0xDEEF, 0xFFFF, // spk 0
    0xBBBB, 0xBBCC, 0xCDDE, 0xFFFF, // spk 1
    0x8888, 0x899A, 0xABCD, 0xDEFF, // spk 2
    0x8888, 0x8889, 0xAABC, 0xDEFF, // spk 3
};
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_idx[4][4] = {
    0x3456, 0x7789, 0xAABB, 0xCFFF, // spk 0
    0x4567, 0x899A, 0xBBCC, 0xCDFF, // spk 1
    0x789A, 0xBBCC, 0xDDDD, 0xEEEF, // spk 2
    0x789A, 0xBCDD, 0xDEEE, 0xEEEF, // spk 3
};

#elif defined(VIBR_AMP_420MVRMS)

const kal_uint16 vib_digi_gain_tab_for_spk_amp_0_2_3[14] = {0x72DB, 0x5B3C, 0x4878, 0x3990, 0x2DB9, 0x2452, 0x1CD9, 0x16EA, 0x1234, 0x0E75, 0x0B7C, 0x091F, 0x073F, 0x05C1};
const kal_uint16 vib_digi_gain_tab_for_spk_amp_1[6] = {0x665D, 0x5150, 0x4096, 0x334E, 0x28C0, 0x205F};
const kal_uint16 vib_digi_gain_tab_idx = 0xBA52;
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_bottom[4] = {0x5246, 0x5FA0, 0x7483, 0x7A3E};
const kal_uint16 vibr_plus_aud_voice_amp_tab[4][4] = {
    0xBBCC, 0xCCCD, 0xDDEE, 0xFFFF, // spk 0
    0xAAAA, 0xBBBC, 0xCDDE, 0xEFFF, // spk 1
    0x8888, 0x8899, 0xABCC, 0xDEFF, // spk 2
    0x8888, 0x8889, 0x9ABC, 0xDEFF, // spk 3
};
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_idx[4][4] = {
    0x4556, 0x7899, 0xABBC, 0xCDFF, // spk 0
    0x5678, 0x89AA, 0xBBCC, 0xDDFF, // spk 1
    0x789A, 0xBCCD, 0xDDDE, 0xEEEF, // spk 2
    0x789A, 0xBCDD, 0xEEEE, 0xEEEF, // spk 3
};

#elif defined(VIBR_AMP_350MVRMS)

const kal_uint16 vib_digi_gain_tab_for_spk_amp_0_2_3[15] = {0x787F, 0x5FB6, 0x4C07, 0x3C64, 0x2FF8, 0x261A, 0x1E44, 0x180A, 0x1318, 0x0F2B, 0x0C0C, 0x0992, 0x079A, 0x060A, 0x04CC};
const kal_uint16 vib_digi_gain_tab_for_spk_amp_1[7] = {0x6B64, 0x554E, 0x43C2, 0x35D3, 0x2AC1, 0x21F5, 0x1AF9};
const kal_uint16 vib_digi_gain_tab_idx = 0xA941;
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_bottom[4] = {0x59E5, 0x6506, 0x766D, 0x7B33};
const kal_uint16 vibr_plus_aud_voice_amp_tab[4][4] = {
    0xBBBB, 0xBBCC, 0xCDDE, 0xFFFF, // spk 0
    0x99AA, 0xAABB, 0xCCDD, 0xEFFF, // spk 1
    0x8888, 0x8899, 0xABBC, 0xDEFF, // spk 2
    0x8888, 0x8888, 0x9ABC, 0xDEFF, // spk 3
};
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_idx[4][4] = {
    0x4567, 0x899A, 0xBBCC, 0xCDFF, // spk 0
    0x6778, 0x9AAB, 0xBCCD, 0xDDFF, // spk 1
    0x789A, 0xBCCD, 0xDDEE, 0xEEEF, // spk 2
    0x789A, 0xBCDE, 0xEEEE, 0xEEEF, // spk 3
};

#elif defined(VIBR_AMP_280MVRMS)

const kal_uint16 vib_digi_gain_tab_for_spk_amp_0_2_3[16] = {0x795B, 0x6065, 0x4C92, 0x3CD2, 0x3050, 0x2660, 0x1E7B, 0x1836, 0x133B, 0x0F47, 0x0C22, 0x09A3, 0x07A8, 0x0615, 0x04D4, 0x03D6};
const kal_uint16 vib_digi_gain_tab_for_spk_amp_1[8] = {0x6C29, 0x55EA, 0x443E, 0x3635, 0x2B0F, 0x2234, 0x1B2B, 0x1594};
const kal_uint16 vib_digi_gain_tab_idx = 0x9830;
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_bottom[4] = {0x6184, 0x6A6B, 0x7857, 0x7C29};
const kal_uint16 vibr_plus_aud_voice_amp_tab[4][4] = {
    0xAAAA, 0xBBBB, 0xCCDE, 0xEFFF, // spk 0
    0x9999, 0x9AAB, 0xBCCD, 0xEFFF, // spk 1
    0x8888, 0x8889, 0xAABC, 0xDEFF, // spk 2
    0x8888, 0x8888, 0x9ABC, 0xDEFF, // spk 3
};
const kal_uint16 vibr_plus_aud_aud_digi_gain_tab_idx[4][4] = {
    0x5678, 0x89AB, 0xBCCC, 0xDDFF, // spk 0
    0x6789, 0xAABB, 0xCCDD, 0xDDEF, // spk 1
    0x789A, 0xBCDD, 0xDEEE, 0xEEEF, // spk 2
    0x789A, 0xBCDE, 0xEEEE, 0xEEEF, // spk 3
};

#else
#error Invalid vibration signal amplitude
#endif

#else
#error Chip does not support vibration speaker
#endif

#if defined(MT6260)
#if __AFE_CURRENT_SENSOR_ENABLE__ 
const kal_uint8 VIBR_LOUD_COEF_IDX[6] = 
{
    0, 1, 27, 28, 2, 29
};

const kal_uint8 VIBR_LOUD_DATA_HI_IDX[6] = 
{
    0, 0, 0, 0, 3, 5
};

const kal_uint8 VIBR_LOUD_DATA_LO_IDX[4] = 
{
    1, 2, 1, 4
};

const kal_uint16 VIBR_LOUD_FLT_ROM[70] = 
{
    0x3DEA, 0x8433, 0x7D5C, 0xC288, 0x7A1A, 0xC5A1,
    0x3DC6, 0x847C, 0x7CB5, 0xC328, 0x7A14, 0xC5A1,
    0x3CF0, 0x862E, 0x7BFC, 0xC3C8, 0x7761, 0xC80C,
    0x3B81, 0x8917, 0x7A92, 0xC504, 0x72B0, 0xCC28,
    0x3B34, 0x89B4, 0x793D, 0xC63D, 0x7298, 0xCC28,
    0x3992, 0x8D10, 0x765F, 0xC8A5, 0x6E3F, 0xCF8E,
    0x37EB, 0x9084, 0x7351, 0xCB00, 0x699D, 0xD2D7,
    0x36F6, 0x927D, 0x71D6, 0xCC28, 0x66C2, 0xD4F7,
    0x349C, 0x9789, 0x6F7A, 0xCE70, 0x5DBC, 0xDA0D,
    
    0x0B71, 0xE1E4, 0x3DEF, 0x1F2C, 0xC1C5, 
    0x1F2C, 0xE194, 0x3E43, 0x1FF4, 0xC034,
    0x1FF4, 0xE0F4, 0x3EE8, 0x1FFE, 0xC024,
    0x1FFE
};

const kal_int16 VIBR_SPH_FLT_ROM[9] = 
{
    -4678, 11792, 407, 204, 407, -6978, 12257, 2929, -2945
};

const kal_uint32 VIBR_ORDER_ARRAY[VIBR_ORDER_SIZE] = 
{
    0x00000081, 0x02040804, 0x89040008, 0x10204081, 0x02012244, 
    0x80000000, 0x01020402, 0xA954AA00, 0x00204081, 0x020402A9, 
    0x55AA8206, 0x06000408, 0x10200A8A, 0xB6AA0408, 0x10204081, 
    0x02012244, 0x80000400, 0x20408100, 0xAA552AAA, 0xB12C4695, 
    0x2A8A1509, 0x28A88B7C, 0xB6DB5400, 0x00000408, 0x10200413, 
    0x64800408, 0x10204081, 0xB7704104, 0x00000081, 0x0204080D, 
    0x904D9000, 0x00810204, 0x08101B20, 0x9B26C7FB, 0x1FEC7FF9, 
    0xFFE7FF9F, 0xFE7FF9FF, 0xE7E10410, 0x00000204, 0x0810203F, 
    0x41040000, 0x20408102, 0x04008000, 0x00408102, 0x0408101F, 
    0xA09B2010, 0x00000810, 0x204009B2, 0x08001020, 0x40810204, 
    0x00820900, 0x00008102, 0x040B03F4, 0x10400000, 
};

const kal_uint32 VIBR_VALUE_ARRAY[VIBR_VALUE_SIZE] = 
{
    0x12234F1F, 0x11111122, 0x4F1F1F12, 0x24FF3FFF, 0x3FFF3F11, 
    0x1224FF3F, 0xFF3F507F, 0x3F8F6EE2, 0x2242E22E, 0x2B0F72E2, 
    0x11111123, 0x4F1F1F11, 0x1225F1F1, 0xF1F1F1F1, 0xF1110911, 
    0x0A113111, 0x11111121, 0x12131141, 0x11645F7B, 0x4550BEF0, 
    0xBCF35C7C, 0x123511C0, 0x42111111, 0x32F51EEA, 0x30EA1111, 
    0x123520DF, 0x2220DF21, 0x12224E02, 0x111E0211, 0x25B70A3F, 
    0x641CC90A, 0x4F6425B8, 0x0A3F660C, 0x3E7C330D, 0x14095ED6, 
    0x28CDAC09, 0x2EDF289D, 0xBF08FEE8, 0x283DD30D, 0x7E593C3C, 
    0xCC08AEFA, 0x275E0122, 0x21133513, 0xBEB92221, 0x12235111, 
    0x1123509B, 0xF5D22EF1, 0x02113351, 0xD0311111, 0x12225222, 
    0x11133E0F, 0x099F5D22, 0x20000000, 
};
#endif
#endif  // chip compile option

#endif 
