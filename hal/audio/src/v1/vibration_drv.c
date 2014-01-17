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
 *   vibration_drv.c
 *
 * Project:
 * --------
 *   Maui
 *
 * Description:
 * ------------
 *   Vibration speaker controller
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
 * removed!
 * removed!
 * removed!
 * removed!
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include "l1audio.h"

#if defined(__VIBRATION_SPEAKER_SUPPORT__)

#include "audio_enum.h"
#include "l1audio_trace.h"
#include "am.h"
#include "afe.h"
#include "kal_trace.h"
#include "l1sp_trc.h"
#include "ddload.h"
#include "audio_def.h"
#include "PcmSink.h"
#include "vibration_def.h"

#define WAV_ABORT_STATE 0x40
#define VIBR_FILTER_COEF_NUM 5
#define MIN_DIGI_GAIN -128
#define VIBR_PGA_GAIN 60

#define DEFAULT_VS_MOD_FREQ     15

static kal_bool bIsHisrHooked                = KAL_FALSE;
static kal_bool SPK_VRIBATION_ENABLE         = KAL_FALSE;
static kal_bool SPK_VRIBATION_ACTIVATED      = KAL_FALSE;
static kal_bool SPK_VRIBATION_FILTER_INIT    = KAL_FALSE;
static kal_bool SPK_VRIBATION_PLAYDUMMY      = KAL_FALSE;
static kal_uint16 SPK_VRIBATION_GAIN = 0;
static kal_uint16 uDummyAudioID = 0;

#define PCMRUT_MASK       0x0080
#define PCMRUT_ON         0x0081

#if defined(MT6252)
#define VIBR_DSP_STATUS  PPED_PCM_FILLED_ACK
#elif defined(MT6250) || defined(MT6260)
#define VIBR_DSP_STATUS  AUD_VSG_CTRL
#endif

extern const kal_uint16 VIBR_FILTER_COEFF[2][9][VIBR_FILTER_COEF_NUM];

static kal_uint16 VIBR_CF  = DEFAULT_VS_CENTER_FREQ;
static kal_uint16 VIBR_DF  = DEFAULT_VS_DELTA_FREQ;
static kal_uint16 VIBR_MF  = DEFAULT_VS_MOD_FREQ;
static kal_bool   VIBR_CHK = KAL_FALSE;

#if __AFE_CURRENT_SENSOR_ENABLE__  
static kal_uint16 vib_cali_aud_id = 0;
kal_uint16 VIBR_Is_In_Calibration(void);
#endif

VIBR_NVRAM_STRUCT VIBR_NVRAM_DATA;

#if defined(MT6252)

extern const unsigned short VIBR_VS_DIGI_GAIN[2][16];
extern const unsigned short DIGITAL_GAIN_TAB[6];
extern const unsigned short DIGITAL_GAIN_MAX_TAB[16];

static kal_uint16 VIBR_Digi_Gain_Lookup(kal_uint16 spk_amp, kal_uint16 hp_amp)
{
   kal_uint16 vib_digi_gain=0;
   kal_uint16 index1, index2;
    
   index1 = spk_amp & 1;
   index2 = (spk_amp >> 1) + hp_amp + index1;
   if (index2 < 6)
   {
      vib_digi_gain = 0x7FFF;
   }
   else if(index2 < 21)
   {
      index2 -= 6;
      vib_digi_gain = VIBR_VS_DIGI_GAIN[index1][index2];
   }
   else
   {
      ASSERT(0);
   }
   return vib_digi_gain;
}

void VIBR_Vibration_GetNewGain(unsigned short spk_amp, unsigned short em_idx, unsigned short *hp_amp, unsigned short *digital_gain)
{
    unsigned short index;
    
    switch(em_idx)
    {
        case 0:
            *hp_amp = spk_amp < 2 ? 6 : spk_amp < 4 ? 5 : spk_amp < 7 ? 4 : spk_amp < 11 ? 3 : 2;
            break;
        case 1:
            *hp_amp = spk_amp < 2 ? 6 : spk_amp < 5 ? 5 : spk_amp < 9 ? 4 : spk_amp < 14 ? 3 : 2;
            break;
        case 2:
            *hp_amp = spk_amp < 3 ? 6 : spk_amp < 7 ? 5 : spk_amp < 12 ? 4 : 3;
            break;
        case 3:
            *hp_amp = spk_amp < 1 ? 7 : spk_amp < 5 ? 6 : spk_amp < 10 ? 5 : 4;
            break;
        case 4:
            *hp_amp = spk_amp < 3 ? 7 : spk_amp < 8 ? 6 : 5;
            break;
        case 5:
            *hp_amp = spk_amp < 1 ? 8 : spk_amp < 6 ? 7 : 6;
            break;
        case 6:
            *hp_amp = spk_amp < 4 ? 8 : 7;
            break;
        case 7:
            *hp_amp = spk_amp < 2 ? 9 : 8;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            *hp_amp = em_idx + 1;
            break;
        case 13:
            *hp_amp = em_idx;
            break;
        default:
            break;
    }

    index = (spk_amp >> 1) + em_idx;

    if (em_idx == 13)
    {
        *digital_gain = DIGITAL_GAIN_MAX_TAB[spk_amp];
    }
    else if (spk_amp & 1)
    {
        if (index == 0)
        {
            *digital_gain = DIGITAL_GAIN_TAB[0];
        }
        else if (index == 1)
        {
            *digital_gain = DIGITAL_GAIN_TAB[1];
        }
        else if (index == 2)
        {
            *digital_gain = DIGITAL_GAIN_TAB[2];
        }
        else if (index >= 3 && index <= 4)
        {
            *digital_gain = DIGITAL_GAIN_TAB[3];
        }
        else if (index >= 5 && index <= 7)
        {
            *digital_gain = DIGITAL_GAIN_TAB[4];
        }
        else
        {
            *digital_gain = DIGITAL_GAIN_TAB[5];
        }
    }
    else
    {
        if (index == 0)
        {
            *digital_gain = DIGITAL_GAIN_TAB[0];
        }
        else if (index == 1)
        {
            *digital_gain = DIGITAL_GAIN_TAB[1];
        }
        else if (index >= 2 && index <= 3)
        {
            *digital_gain = DIGITAL_GAIN_TAB[2];
        }
        else if (index >= 4 && index <= 5)
        {
            *digital_gain = DIGITAL_GAIN_TAB[3];
        }
        else if (index >= 6 && index <= 7)
        {
            *digital_gain = DIGITAL_GAIN_TAB[4];
        }
        else
        {
            *digital_gain = DIGITAL_GAIN_TAB[5];
        }
    }
    
    return;
}

#elif defined(MT6250) || defined(MT6260)

extern const unsigned short vib_digi_gain_tab_for_spk_amp_0_2_3[];
extern const unsigned short vib_digi_gain_tab_for_spk_amp_1[];
extern const unsigned short vib_digi_gain_tab_idx;
extern const unsigned short vibr_plus_aud_aud_digi_gain_tab_common[];
extern const unsigned short vibr_plus_aud_voice_amp_tab[4][4];
extern const unsigned short vibr_plus_aud_aud_digi_gain_tab_idx[4][4];
extern const unsigned short vibr_plus_aud_aud_digi_gain_tab_bottom[4];

static kal_uint16 VIBR_Digi_Gain_Lookup(kal_uint16 spk_amp, kal_uint16 voice_amp)
{
    kal_uint16 vib_digi_gain = 0;
    
    if (voice_amp < 16 && spk_amp < 4)
    {
        kal_uint16 shifting;
        kal_uint16 vdg_idx;
        
        shifting  = (3 - spk_amp) << 2;
        
        vdg_idx   = vib_digi_gain_tab_idx;
        vdg_idx >>= shifting;
        vdg_idx  &= 0x000F;
        
        if (voice_amp < vdg_idx)
        {
            vib_digi_gain = 0x7FFF;
        }
        else
        {
            vdg_idx = voice_amp - vdg_idx;
            
            if (spk_amp == 1)
            {
                vib_digi_gain = vib_digi_gain_tab_for_spk_amp_1    [vdg_idx];
            }
            else
            {
                vib_digi_gain = vib_digi_gain_tab_for_spk_amp_0_2_3[vdg_idx];
            }
        }
    }
    else
    {
        ASSERT(0);
    }
    
    return vib_digi_gain;
}

void VIBR_Vibration_GetNewGain(kal_uint16 spk_amp, kal_uint16 em_idx, kal_uint16 *voice_amp, kal_uint16 *aud_digi_gain)
{
    if (em_idx < 16 && spk_amp < 4)
    {
        kal_uint16 adg_idx;
        kal_uint16 voice_amp_temp;
        kal_uint16 quotient, remainder, shifting;

        // *AUD_PP_UPPER_BOUND = 0x7FFF;
        
        quotient  = em_idx >> 2;
        remainder = em_idx - (quotient << 2);
        shifting  = (3 - remainder) << 2;
        
        adg_idx   = vibr_plus_aud_aud_digi_gain_tab_idx[spk_amp][quotient];
        adg_idx >>= shifting;
        adg_idx  &= 0x000F;
        
        voice_amp_temp   = vibr_plus_aud_voice_amp_tab[spk_amp][quotient];
        voice_amp_temp >>= shifting;
        voice_amp_temp  &= 0x000F;
        
        *voice_amp = voice_amp_temp;
        
        if (adg_idx < 0xF)
        {
            *aud_digi_gain = vibr_plus_aud_aud_digi_gain_tab_common[adg_idx];
        }
        else
        {
            *aud_digi_gain = vibr_plus_aud_aud_digi_gain_tab_bottom[spk_amp];
        }
    }
    else
    {
        ASSERT(0);
    }
 
    return;
}

#endif


kal_bool VIBR_IsVibrating()
{  
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_IS_VIBRATING,SPK_VRIBATION_ENABLE,0,0,0,0,0);
   return SPK_VRIBATION_ENABLE;
}

kal_bool VIBR_IsPlayingDummy()
{  
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_IS_PLAYINGDUMMY,SPK_VRIBATION_PLAYDUMMY,0,0,0,0,0);
   return SPK_VRIBATION_PLAYDUMMY;
}

static kal_bool VIBR_IsConflict()
{
   kal_bool fResult;
   kal_bool fIsDAI         = KAL_FALSE;
   kal_bool fIsSpeech      = KAL_FALSE; 
   kal_bool fIsVM          = KAL_FALSE; 
   kal_bool fIsLoopBack    = KAL_FALSE;  
   kal_bool fIsBGSND       = KAL_FALSE;  
   kal_bool fIsAvsyn       = KAL_FALSE;
   
#if defined(MT6260)
// MT6260 does not support DAI process
#else   // chip compile option   
   fIsDAI = AM_IsDAIMode();
#endif  // chip compile option
   fIsSpeech = AM_IsAmInSpeechState();
#if BGSND_ENABLE  
   fIsBGSND = AM_IsBgsndOn();
#endif 
#if __VM_CODEC_SUPPORT__
   fIsVM = AM_IsVMPlaybackOn();
#endif
   fIsLoopBack = AM_IsLoopBackTest();
   fIsAvsyn = AM_IsAVsyncOn();
   if(fIsDAI || fIsSpeech || fIsBGSND || fIsVM || fIsLoopBack || fIsAvsyn)
   {
      fResult = KAL_TRUE;
   }
   else
   {
      fResult = KAL_FALSE;
   }
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_IS_CONFLICT,fResult,fIsDAI,fIsSpeech,fIsVM,fIsLoopBack,fIsBGSND);
   return fResult;
}

void VIBR_Filter_SetCoeff(kal_uint32 uSampleRate)
{
#if defined(MT6252)   
   volatile kal_uint16* dsp_ptr;
   kal_uint16 bufAddr=0, pageNum=0;
   kal_uint32 I=0;
   kal_uint32 uTableIndex = 0;

   if(SPK_VRIBATION_FILTER_INIT)
   { 
      return;
   }

   switch (uSampleRate) 
   {
      case 48000:
         uTableIndex = 0;
         break;
      case 44100:
         uTableIndex = 1;
         break;
      case 32000:
         uTableIndex = 2;
         break;
      case 24000:
         uTableIndex = 3;
         break;
      case 22050:
         uTableIndex = 4;
         break;
      case 16000:
         uTableIndex = 5;
         break;
      case 12000:
         uTableIndex = 6;
         break;
      case 11025:
         uTableIndex = 7;
         break;
      case 8000:
         uTableIndex = 8;
         break;
      default:
         ASSERT(0);
   }

   {
      bufAddr = (*PP_IIR_COEF_BASE) & 0x3FFF;
      pageNum = ((*PP_IIR_COEF_BASE) >> 14) & 0x0003;
      dsp_ptr = DSP_DM_ADDR(pageNum, bufAddr);
   }
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_FILTER_SETCOEFF,
                    bufAddr,pageNum,uSampleRate,0,0,0);
   for (I=0; I < VIBR_FILTER_COEF_NUM; I++)
   {
      *dsp_ptr++ = VIBR_FILTER_COEFF[0][uTableIndex][I];
   }
   for (I=0; I < VIBR_FILTER_COEF_NUM; I++)
   {
      *dsp_ptr++ = VIBR_FILTER_COEFF[1][uTableIndex][I];
   }

   SPK_VRIBATION_FILTER_INIT = KAL_TRUE;
#elif defined(MT6250) || defined(MT6260)
    return;
#endif
}

void VIBR_Filter_Activate(kal_bool b_bypass)
{
#if defined(MT6252)   
   SPK_VRIBATION_FILTER_INIT = KAL_FALSE;
   *AUD_PP_UPPER_BOUND = 0x7FFF;
   *PPED_PCM_FILLED_ACK |= b_bypass ? 0x8000 : 0x8002;
   *DP2_AUDIO_PP_CONTROL |= 0x0081;
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_FILTER_ACTIVATE,
                    SPK_VRIBATION_FILTER_INIT,*DP2_AUDIO_PP_CONTROL,*VIBR_DSP_STATUS,0,0,0);
#elif defined(MT6250) || defined(MT6260)
    return;
#endif
}

void VIBR_Filter_Deactivate()
{
#if defined(MT6250) || defined(MT6260)
    return;
#elif defined(MT6252)
   *DP2_AUDIO_PP_CONTROL &= ~0x0081;
   *PPED_PCM_FILLED_ACK &= ~0xC003;
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_FILTER_DEACTIVATE,
                    SPK_VRIBATION_FILTER_INIT,*DP2_AUDIO_PP_CONTROL,*VIBR_DSP_STATUS,0,0,0);
#endif
}

void VIBR_PlayDummyAudio()
{
   kal_uint16 I, uSampleRate=0;
   kal_uint8 uChannelNumber = 1;
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_PLAY_DUMMY, SPK_VRIBATION_PLAYDUMMY,0,*DSP_PCM_CTRL,0,0,0);
   if(SPK_VRIBATION_PLAYDUMMY)
   {
      return;
   }
   APM_Deactivate();
   //AudioPP_DisableBesSound();
   SPK_VRIBATION_PLAYDUMMY = KAL_TRUE;
   uDummyAudioID = L1Audio_GetAudioID();
   L1Audio_SetFlag( uDummyAudioID );
   KT_StopAndWait();
   ktLock();
   Media_SetOutputDevice(L1SP_LOUD_SPEAKER);
   Media_MuteSpeaker(KAL_FALSE);
   Media_SetOutputVolume(VIBR_PGA_GAIN, MIN_DIGI_GAIN);

#if defined(MT6252)
   DSP_DynamicDownload( DDID_WAVETABLE );
   AM_AudioPlaybackOn( ASP_TYPE_PCM_HI, ASP_FS_16K );
   *DSP_PCM_CTRL = 1;
   for( I = 0; ; I++ ) 
   {
      kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_PLAY_DUMMY, SPK_VRIBATION_PLAYDUMMY,1,*DSP_PCM_CTRL,0,0,0);
      if( *DSP_PCM_CTRL == 2 )
      { 
         *DSP_PCM_W = 0;
         *DSP_PCM_R = 0;
         *DSP_PCM_CTRL = 4;
      }
      if( *DSP_PCM_CTRL == 8  )
      {
         break;
      }
      ASSERT_REBOOT( I < 80 );
      kal_sleep_task( 2 );
   }
#elif defined(MT6250) || defined(MT6260)
   PcmSink_GetCurrentPcmInfo(&uSampleRate, &uChannelNumber);
   if (uSampleRate == 0) 
   {
   	  uSampleRate = 32000;
   }
   PcmSink_StartSound(PCM_FUNC_VIB,uSampleRate,2);
   if( AM_IsAudioPlaybackOn() == -1)
   {
   	  SPK_VRIBATION_PLAYDUMMY = KAL_FALSE;
   	  L1Audio_ClearFlag( uDummyAudioID );
      L1Audio_FreeAudioID(uDummyAudioID);
      ktUnlock(); 
   	  
   }
#endif
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_PLAY_DUMMY, SPK_VRIBATION_PLAYDUMMY,1,*DSP_PCM_CTRL,0,0,0);
}

void VIBR_StopDummyAudio()
{  
   kal_uint16 I, ctl;
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_STOP_DUMMY, SPK_VRIBATION_PLAYDUMMY,0,0,0,0,0);
   if(SPK_VRIBATION_PLAYDUMMY == KAL_FALSE)
   {
      return;
   }
#if defined(MT6252)   
   for( I = 0; ; I++ ) 
   {
      ctl = *DSP_PCM_CTRL;
      if( ctl == 0 )               
      {
         break;
      }
      if( ctl == 8 )
      {
         *DSP_PCM_CTRL = WAV_ABORT_STATE;
      }
      ASSERT_REBOOT( I < 40 );
      kal_sleep_task( 2 );
   }
   AM_AudioPlaybackOff( true );
#elif defined(MT6250) || defined(MT6260)
   PcmSink_StopSound(PCM_FUNC_VIB); 
#endif        
   L1Audio_ClearFlag( uDummyAudioID );
   L1Audio_FreeAudioID(uDummyAudioID);
   ktUnlock(); 
      
   SPK_VRIBATION_PLAYDUMMY = KAL_FALSE;
   //AudioPP_EnableBesSound();
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_STOP_DUMMY, SPK_VRIBATION_PLAYDUMMY,1,0,0,0,0);
}

static void VIBR_Vibration_WriteGain2DSP()
{  
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_WRITE_GAIN2DSP, 
                    SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VS_DIGI_GAIN,SPK_VRIBATION_GAIN,0,0);
   *VS_DIGI_GAIN = SPK_VRIBATION_GAIN;
}

void VIBR_Vibration_UpdateGain(kal_uint16 uAudBufGain, kal_uint16 uAmpBuf)
{  
   SPK_VRIBATION_GAIN = VIBR_Digi_Gain_Lookup(uAmpBuf, uAudBufGain);
   kal_brief_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_UPDATE_GAIN, 
                    SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,uAudBufGain,uAmpBuf,SPK_VRIBATION_GAIN,0);

   if( AM_IsAudioPlaybackOn() != -1 )
   {
      VIBR_Vibration_WriteGain2DSP();
   }
}

#if defined( __I2S_INPUT_MODE_SUPPORT__ )
extern kal_bool I2S_is_On();
#endif

void VIBR_Vibration_Activate()
{
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_VIBRATION_ACTIVATE, 
              SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VIBR_DSP_STATUS,0,0,0);
   if(!SPK_VRIBATION_ENABLE || SPK_VRIBATION_ACTIVATED)
   {
      return;   
   }
#if defined( __I2S_INPUT_MODE_SUPPORT__ )
    // In AFE_TurnOnEDI(), DSP workaround for I2S HW issue, audio clock 8K->16K
    if(I2S_is_On())
    {
        return;
    }
#endif
   if(!PcmSink_IsAudioRuning() && !Media_IsDSPDec())
   {
      if(VIBR_IsConflict())
      {
         return;
      }
      if(AM_IsAudioPlaybackOn() == -1 )
      {
         VIBR_PlayDummyAudio();
         if(!SPK_VRIBATION_PLAYDUMMY)
         {
      	    return;
         }
      }
   }
   if(AFE_IsAudioLoudSpk())
   {
      kal_uint8 uVol;
      kal_int8  iDigiGain;
      Media_MuteSpeaker(KAL_FALSE);
      AFE_GetOutputVolume( L1SP_AUDIO, &uVol, &iDigiGain );
      if(uVol==0 || iDigiGain==MIN_DIGI_GAIN)
      {
         // to prevent loudspeaker is mute
         Media_SetOutputVolume( VIBR_PGA_GAIN, MIN_DIGI_GAIN);
      }
      VIBR_Vibration_WriteGain2DSP();
#if defined(MT6252)   
      {
         *VS_CENTER_FREQ = DEFAULT_VS_CENTER_FREQ;
         *VS_MOD_FREQ    = DEFAULT_VS_MOD_FREQ;
         *VS_DELTA_FREQ  = DEFAULT_VS_DELTA_FREQ;
         *PPED_PCM_FILLED_ACK |= 0x2000;
      }
#elif defined(MT6250) || defined(MT6260)
      {
         *VS_MOD_FREQ    = VIBR_MF;                      
         *VS_CENTER_FREQ = VIBR_CF;
         *AUD_VSG_CTRL   = 0x8000 | (VIBR_DF & 0x3FFF);

      }
#endif
      SPK_VRIBATION_ACTIVATED = KAL_TRUE;
      kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_VIBRATION_ACTIVATE, 
      SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VIBR_DSP_STATUS,1,0,0);
   }
}

void VIBR_Vibration_Deactivate()
{  
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_VIBRATION_DEACTIVATE, 
              SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VIBR_DSP_STATUS,0,0,0);
   if(SPK_VRIBATION_ACTIVATED)
   {
#if defined(MT6252) 
      *PPED_PCM_FILLED_ACK &= ~0x3000;
      {
        kal_uint16 I;
        for( I = 0; ; I++ ) 
        {
            if((*PPED_PCM_FILLED_ACK & 0x0800) == 0)
            {
                break;
            }
            ASSERT_REBOOT( I < 80 );
            kal_sleep_task( 2 );
        }
      }
#elif defined(MT6250) || defined(MT6260)
      if (VIBR_CHK)
      {
          *AUD_VSG_CTRL &= ~0xC000;
      }
      else
      {
          *AUD_VSG_CTRL &= ~0x8000;
          {
              kal_uint16 I;
              for( I = 0; ; I++ ) 
              {
                  if((*AUD_VSG_CTRL & 0xC000) == 0)
                  {
                      break;
                  }
                  ASSERT_REBOOT( I < 80 );
                  kal_sleep_task( 2 );
              }
          }
      }
#endif
      VIBR_StopDummyAudio();
      SPK_VRIBATION_ACTIVATED = KAL_FALSE;
      kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_VIBRATION_DEACTIVATE, 
                 SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VIBR_DSP_STATUS,1,0,0);
   }
}

void VIBR_SetInitCoef(kal_uint16 vs_cf, kal_uint16 vs_df, kal_uint16 vs_mf, kal_bool restore)
{
    if(restore)
    {
        VIBR_CF  = VIBR_NVRAM_DATA.VIBR_NVRAM_CF;
        VIBR_DF  = DEFAULT_VS_DELTA_FREQ;
        VIBR_MF  = DEFAULT_VS_MOD_FREQ;
        VIBR_CHK = KAL_FALSE;
    }
    else
    {
        VIBR_CF  = vs_cf;
        VIBR_DF  = vs_df;
        VIBR_MF  = vs_mf;
        VIBR_CHK = KAL_TRUE;
    }
}


Media_Status Media_SpkVibration_Enable( kal_bool fEnable )
{
   SPK_VRIBATION_ENABLE = fEnable;
   kal_trace( TRACE_GROUP_AUD_PLAYBACK, L1AUDIO_GENERAL_INFO, AUDIO_TRACE_VIBR_ENABLE, 
              SPK_VRIBATION_ENABLE,SPK_VRIBATION_ACTIVATED,*VIBR_DSP_STATUS,0,0,0);
#if __AFE_CURRENT_SENSOR_ENABLE__
   if(VIBR_Is_In_Calibration()) //avoid re-entry while calibration
   {
   	  return;
   }
#endif      
   if(SPK_VRIBATION_ENABLE)
   {
      VIBR_Vibration_Activate();
   }
   else
   {
      VIBR_Vibration_Deactivate();
   }
   return MEDIA_SUCCESS;
}

void (*pVIB_Calibration_CallBack)(void *cali_data) = NULL;
void Media_VIB_Calibration_Write(void(*VIB_Calibration_CallBack)(void *cali_data))
{
   pVIB_Calibration_CallBack = VIB_Calibration_CallBack;
}

#if __AFE_CURRENT_SENSOR_ENABLE__
extern kal_uint16 AFE_Vibrate_Calibration(void);
kal_uint32 VIBR_Loudness_Filter_Gen(kal_int32 freq, kal_uint32 *p_loud_coef, kal_int16 *p_sph_coef);

Media_Status Media_SpkVibration_Enable_Cal( kal_bool fEnable )
{
   SPK_VRIBATION_ENABLE = fEnable;
   if(SPK_VRIBATION_ENABLE)
   {
      VIBR_Vibration_Activate();
   }
   else
   {
      VIBR_Vibration_Deactivate();
   }
   return MEDIA_SUCCESS;
}
void VIBR_Calibration_In_Audio_Task() 
{
   kal_uint16 CF = AFE_Vibrate_Calibration();
   PcmSink_StopSound(PCM_FUNC_VIB);
   
   if(CF!=VIBR_NVRAM_DATA.VIBR_NVRAM_CF)
   {
      VIBR_NVRAM_DATA.VIBR_NVRAM_CF = CF;
      VIBR_CF = CF;
      VIBR_Loudness_Filter_Gen((kal_int32)CF, (kal_uint32 *)VIBR_NVRAM_DATA.VIBR_NVRAM_LOUD_FILTER_COEF, (kal_int16 *)VIBR_NVRAM_DATA.VIBR_NVRAM_SPH_FILTER_COEF);
   }
   //VIBR_NVRAM_DATA.VIBR_NVRAM_CALI_DONE = KAL_TRUE;
   pVIB_Calibration_CallBack(&VIBR_NVRAM_DATA);
   audio_vib_spk_is_calib();
   L1Audio_FreeAudioID(vib_cali_aud_id);
   vib_cali_aud_id = 0;
}

void VIBR_Calibration_Switch_Audio_Task()
{
   vib_cali_aud_id = L1Audio_GetAudioID();
   L1Audio_SetEventHandler( vib_cali_aud_id, VIBR_Calibration_In_Audio_Task );
   L1Audio_SetEvent( vib_cali_aud_id, 0 );
}


kal_uint16 VIBR_Is_In_Calibration() 
{
   return vib_cali_aud_id;
}
#endif


kal_bool Media_Is_Vibrator_Calibration(void)
{
#if __AFE_CURRENT_SENSOR_ENABLE__ 	
	  return AFE_Is_VIBR_Calibration();
#else
    return KAL_TRUE;
#endif	  
}

void Media_Vibrate_CF_Check()
{
#if __AFE_CURRENT_SENSOR_ENABLE__ 
   if(AFE_Is_VIBR_Calibration() || VIBR_Is_In_Calibration())
   {
   	  return;
   }	
   PcmSink_TerminateSound();
   PcmSink_StartSound(PCM_FUNC_VIB,32000,2);
	 kal_sleep_task(5);	
	 VIBR_Calibration_Switch_Audio_Task(); 
#else
   audio_vib_spk_is_calib();	 
#endif
}

void Media_SetVibrInitParam(void *pBuf, kal_uint32 uLen)
{
    memcpy(&VIBR_NVRAM_DATA, pBuf, sizeof(VIBR_NVRAM_STRUCT));
}

#if __AFE_CURRENT_SENSOR_ENABLE__ 
extern const kal_uint8  VIBR_LOUD_COEF_IDX[6];
extern const kal_uint8  VIBR_LOUD_DATA_HI_IDX[6];
extern const kal_uint8  VIBR_LOUD_DATA_LO_IDX[4];
extern const kal_uint16 VIBR_LOUD_FLT_ROM[70];
extern const kal_int16  VIBR_SPH_FLT_ROM[9];
extern const kal_uint32 VIBR_ORDER_ARRAY[VIBR_ORDER_SIZE];
extern const kal_uint32 VIBR_VALUE_ARRAY[VIBR_VALUE_SIZE];

static kal_int32 Vibr_Decode_Array(kal_bool is_order_dec, kal_int32 dec_order, kal_int32 *p_dec_idx, kal_int32 *p_dec_bit)
{
    kal_int32 dec_idx    = *p_dec_idx;
    kal_int32 dec_bit    = *p_dec_bit;
    kal_int32 dec_len    = is_order_dec ? 3 : dec_order << 2;
    kal_int32 dec_code   = 0;
    kal_int32 dec_shift  = 32 - dec_len;
    
    while (dec_len-- > 0)
    {
        kal_int32 dec_temp = is_order_dec ? VIBR_ORDER_ARRAY[dec_idx] : VIBR_VALUE_ARRAY[dec_idx];
        kal_int32 dec_mask = 1 << dec_bit;
        
        dec_code <<= 1;
        if (dec_temp & dec_mask)
        {
            dec_code++;
        }
        else
        {
            dec_len = is_order_dec ? 0 : dec_len;   // to leave the while loop
        }
        
        if (dec_bit == 0)
        {
            dec_bit = 31;
            dec_idx++;
        }
        else
        {
            dec_bit--;
        }
    }
    
    if (is_order_dec)
    {
        dec_code = dec_code == 0 ? 0 : dec_code == 2 ? 1 : dec_code == 6 ? 2 : 3;
    }
    else
    {
        dec_code = dec_code << dec_shift >> dec_shift;
    }
    
    *p_dec_idx = dec_idx;
    *p_dec_bit = dec_bit;
    
    return dec_code;
}

kal_uint32 VIBR_Loudness_Filter_Gen(kal_int32 freq, kal_uint32 *p_loud_coef, kal_int16 *p_sph_coef)
{
    kal_int32 idx = (freq - 152) / 3;
    
    // Clear buffer to zeros
    memset(p_loud_coef,  0, 2*9*VIBR_LOUDNESS_FILTER_COEF_NUM*sizeof(kal_uint32));
    memset(p_sph_coef,   0, 31*sizeof(kal_int16));
    
    if (idx < 0)
    {
    	 idx = 0;
    }
    else if(idx >= 20)
    {
    	 idx = 19;
    }
    
    // if (freq >= 153 && freq <= 210)
    if (idx >= 0 && idx < 20)
    {
        kal_uint16 vibr_flt_tmp[70];
        
        memcpy(vibr_flt_tmp, VIBR_LOUD_FLT_ROM, 70*sizeof(kal_uint16));
        
        {
            kal_int32 i, j;
            kal_int32 dec_idx_order = 0;
            kal_int32 dec_bit_order = 31;
            kal_int32 dec_idx_value = 0;
            kal_int32 dec_bit_value = 31;
            
            for (i = 0; i < idx; i++)
            {
                for (j = 0; j < 70; j++)
                {
                    kal_int32 order = Vibr_Decode_Array(KAL_TRUE ,     0, &dec_idx_order, &dec_bit_order);
                    kal_int32 value = Vibr_Decode_Array(KAL_FALSE, order, &dec_idx_value, &dec_bit_value);
                    vibr_flt_tmp[j] += value;
                }
            }
        }
        
        {
            kal_int32 i, j;
            kal_uint16 *p_data = vibr_flt_tmp;
            
            for (i = 0; i < 9; i++)
            {
                for (j = 0; j < 6; j++)
                {
                    kal_uint32 *ptr = p_loud_coef + VIBR_LOUD_COEF_IDX[j];
                    
                    {
                        *ptr = (*ptr & 0x0000FFFF) | ((kal_uint32)*(p_data + VIBR_LOUD_DATA_HI_IDX[j]) << 16);
                    }
                    
                    if (j < 4)
                    {
                        *ptr = (*ptr & 0xFFFF0000) | (kal_uint32)*(p_data + VIBR_LOUD_DATA_LO_IDX[j]);
                    }
                }
                p_data      += 6;
                p_loud_coef += 3;
            }
            
            memcpy(p_sph_coef + 0, VIBR_SPH_FLT_ROM,  9*sizeof(kal_int16));
            memcpy(p_sph_coef + 9,           p_data, 16*sizeof(kal_int16));
        }
    }
    
    return 0;
}

void Set_Vibr_Center_Freq(kal_uint16 vibr_center_freq)
{
    kal_uint16 my0 = 0x20C4;    // default 32000Hz sampling rate

/*
    switch (uSampleRate) // my0: Q5.11
    {
        case 8000:  my0 = 0x8312; break;
        case 11025: my0 = 0x5F1B; break;
        case 12000: my0 = 0x5761; break;
        case 16000: my0 = 0x4189; break;
        case 22050: my0 = 0x2F8D; break;
        case 24000: my0 = 0x2BB0; break;
        case 32000: my0 = 0x20C4; break;
        case 44100: my0 = 0x17C6; break;
        case 48000: my0 = 0x15D8; break;
    }
*/
    
    // limitation: the generated tone is Fs/4 at most
    {
        volatile kal_uint16 *dsp_ptr = DSP_DM_ADDR(5, 0x37E9);
        *dsp_ptr = (kal_uint16)(((kal_uint32)vibr_center_freq * my0 << 1) >> 12);
    }
    
    return;
}
#endif //__AFE_CURRENT_SENSOR_ENABLE__

#else

void VIBR_Vibration_Deactivate(){}
void VIBR_Filter_SetCoeff(kal_uint32 uSampleRate){}

#endif//__VIBRATION_SPEAKER_SUPPORT__

