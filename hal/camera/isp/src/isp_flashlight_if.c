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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   isp_flashlight_if.c
 *
 * Project:
 * --------
 *   MT6238
 *
 * Description:
 * ------------
 *   flash light control
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "kal_release.h"
#include "isp_hw_feature_def.h"
#if (defined(ISP_SUPPORT)&&defined(RAW_MAIN_CAMERA))
#include "mm_comm_def.h"
#include "image_sensor.h"
#include "aaa_process.h"
#include "aaa_flash.h"
#include "aaa_awb.h"
#include "aaa_ae.h"
#include "isp_device.h"

#include "sensor_comm_def.h"
#include "isp_if.h"
#include "isp_comm_if.h"
#include "cal_comm_def.h"
#include "isp_flashlight_if.h"


/**************   FLASH LIGHT DECLARATION    ***************************************/

FLASHLIGHT_OPERATION_STATE_ENUM FlashlightOperationState=FLASHLIGHT_STANDBY_STATE;
kal_uint16	AwbRgainBak,AwbGRgainBak,AwbBgainBak,AwbGBgainBak;
kal_uint8 AeIndexBackup;
kal_bool MainFlashFrameFlag;


kal_uint32 eShutter;//6238 flash
kal_uint16 sensor_pregain,isp_pregain,pregain;//6238 flash
kal_uint8 FlashHisrCnt;


// Customize Sensor Driver Parameters
FLASHLIGHT_PREVIEW_PARA_STURCT FlashightPreviewPara;
FLASHLIGHT_CAPTURE_PARA_STRUCT FlashlightCapturePara;

//ae 
FLASH_AE_SUBFLASH_CHECK_INFO_STRUCT  FlashSubFlashInput;
FLASH_AE_SUBFLASH_CHECK_RESULT_STRUCT FlashSubFlashResult;
FLASH_AE_MAINFLASH_CAL_INFO_STRUCT FlashMainFlashInput;
FLASH_AE_MAINFLASH_CAL_RESULT_STRUCT FlashMainFlashResult;

FLASH_AE_CAPTURE_SHUTTER_CAL_INFO_STRUCT  FlashCapShutterInput;
FLASH_AE_CAPTURE_SHUTTER_CAL_RESULT_STRUCT  FlashCapShutterResult;


kal_uint8 FlashAfResult;

kal_uint8 CdtFlashFlag = KAL_FALSE;
FLASH_TRIGGER_STRUCT FlashTrigStatus;

FLASHLIGHT_INFO_WITH_SENSOR_STRUCT FlashlightInfo;

////////////////
extern ISP_CAMERA_PREVIEW_STRUCT IspPreviewConfigData;
///////////////
/****************************************************************************
 *          Flash Light	Control 											 *
 ****************************************************************************/
/*************************************************************************
* FUNCTION
*	FlashConfig
*
* DESCRIPTION
*	Config flash light offset and duty
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

FLASH_TRIGGER_STRUCT *FlashGetTrigStatus(void) 
{
    return &FlashTrigStatus;
}

void FlashReset(void) 
{
    SET_FLASH_START_LINE(0); 
    SET_FLASH_LNUNIT(0);//not need to add 1 (0 is one unit)
    SET_FLASH_LNUNIT_COUNT(0);
    SET_FLASH_FRAME_DELAY(0);

}

void FlashConfig(kal_uint16 StartTime,  kal_uint32 DutyTime) {
    kal_uint32 FlashLineUnit, FlashLineUnitNum;

	/* Delay time setting since Vsync occured */
    if( StartTime >= 4096) 
    {
        StartTime = 4095;
     }

    SET_FLASH_START_LINE(StartTime); 

    FlashLineUnit = (DutyTime / MAX_TIME_FLASH_LNUNIT) +1; //confirm if need to add 1
    FlashLineUnitNum = (DutyTime * FLASH_CLK) / FlashLineUnit; 
    
    SET_FLASH_LNUNIT(FlashLineUnit-1);//not need to add 1 (0 is one unit)
    SET_FLASH_LNUNIT_COUNT(FlashLineUnitNum);

}


/*************************************************************************
* FUNCTION
*	InitFlashlight
*
* DESCRIPTION
*	Initial flash light GPIO and state machine
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void InitFlashlight(void) 
{
    FlashlightOperationState = FLASHLIGHT_STANDBY_STATE;

    if(KAL_TRUE==DscSupportInfo.dsccomp.flashlight) 
    {	
        if((FLASHLIGHT_LED_ONOFF==CameraDriverOperParaBuffer.flashlight_mode) ||
        (FLASHLIGHT_LED_CONSTANT==CameraDriverOperParaBuffer.flashlight_mode) ) 
        {
            DISABLE_FLASH_LIGHT;	        
        }          
    }	    	       
}

#if(defined(LED_FLASHLIGHT_SUPPORT))
/**************   FLASH LIGHT DECLARATION    ***************************************/
#define XENON_TRIGGER_PIN (4) ///LED
kal_bool XenonPrestrobeFlag = KAL_FALSE;

/*****************************************************************************/
/*                          Xenon Flash Functions                            */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	FlashlightPreProcess
*
* DESCRIPTION
*	Pre strobe Entry point
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPreProcess(void) 
{
    CdtFlashFlag = KAL_FALSE; 
    FlashTrigStatus.flashTrigFlag = KAL_FALSE;//for exif
    FlashTrigStatus.flashRedeyeTrigFlag = KAL_FALSE;
    if (DscStatus.Ae.Flash ==CAM_FLASH_OFF) 
    {
        return;
    }
    //since flash setting changes TG int time 
    DISABLE_CAMERA_TG_DONE_INT;
    ENABLE_CAMERA_AVSYNC_DONE_INT;
    SET_AVSYNC_LINENO(1);


    /* store AWB setting */
    AwbRgainBak = AwbHwRGain;			
    AwbGRgainBak = AwbHwGrGain;			
    AwbBgainBak = AwbHwBGain;			
    AwbGBgainBak = AwbHwGbGain;	

    AeIndexBackup = AeGetAeApplyCompExpIdx(); 

    FlashlightInfo.enable_flashlight_tansfer = KAL_FALSE;

    MainFlashFrameFlag = KAL_FALSE; //reset
     
    // XXX
    /* FlashLight Shutter Config */	
    FlashAfResult = FlashAfSubFlashCheck();

    //ae
    FlashSubFlashInput.preview_expo_line_no = 1;/* min preview expo line with common expo range */
    FlashSubFlashInput.flash_mode = DscStatus.Ae.Flash;
    FlashAeSubFlashCheck(&FlashSubFlashInput, &FlashSubFlashResult);
             
    //simulate AE change exposure here
        
    if( (KAL_TRUE==DscSupportInfo.dsccomp.flashlight)&&
    (CAM_WB_MANUAL!= IspPreviewConfigData.pCalIspPreviewPara->WbMode)&& 
    (CAM_FLASH_OFF!=DscStatus.Ae.Flash)&&
    ((FLASHLIGHT_NONE!=CameraDriverOperParaBuffer.flashlight_mode) &&
    (FLASHLIGHT_LED_ONOFF!=CameraDriverOperParaBuffer.flashlight_mode) )) //LED: need to define a customied index
    {
        if( (CAM_FLASH_ON==DscStatus.Ae.Flash) ||
        (AeIndexBackup <= device_support_info.autoflash.lutidx))  
        {
            AwbSetFlashModeEn(KAL_TRUE);

            FlashlightPreStrobe(FlashSubFlashResult.min_flash_preview_delay_frame);//LED AE to do

            FlashlightOperationState = FLASHLIGHT_MAIN_STROBE_STATE;//enter to prestrobe

            //after hisr trigger & collection ae info, determine if second pre-strobe is necessary....
        }   
        else 
        {
            FlashlightInfo.enable_flashlight_tansfer = KAL_FALSE;                                           
            FlashlightOperationState = FLASHLIGHT_STANDBY_STATE; 
        }       
    }
    else 
    {
        FlashlightOperationState = FLASHLIGHT_STANDBY_STATE;      
    }
}

/*************************************************************************
* FUNCTION
*	FlashlightRedeyeProcess
*
* DESCRIPTION
*	Flash light anti-redeye control and capture stable loop
*
* PARAMETERS
*	capture_delay_frame
*
* RETURNS
*	redeye strobe or not
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool FlashlightRedeyeProcess(kal_uint32  CaptureDelayFrame) 
{
    // If not strobe state, return
    if((FLASHLIGHT_STANDBY_STATE==FlashlightOperationState) 
        || (CAM_FLASH_OFF==DscStatus.Ae.Flash)) 
    {
        return KAL_FALSE;
    }
    return FlashlightRedeye(CaptureDelayFrame);

}

/*************************************************************************
* FUNCTION
*	FlashlightPostProcess
*
* DESCRIPTION
*	Flash light post control for capture timing switch
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPostProcess(void)  //called after sensor switch to capture mode setting
{
    // If not strobe state, return
    if(FLASHLIGHT_STANDBY_STATE==FlashlightOperationState)
    {
        return;
    }
    FlashlightPostStrobe();
    AwbSetFlashModeEn(KAL_FALSE);        
}

/*************************************************************************
* FUNCTION
*	FlashlightTriggerProcess
*
* DESCRIPTION
*	Flash light trigger process. 
*   This function should right before open image data base.
*   From trigger process to enable capture should < 1 frame time.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightTriggerProcess(void) 
{    
    // If not strobe state, return
    if(FLASHLIGHT_STANDBY_STATE==FlashlightOperationState) 
    {
        return;
    }
    // FlashLight Trigger & Enable Pin Config
    if((KAL_TRUE==DscSupportInfo.dsccomp.flashlight)&&(CAM_FLASH_OFF!=DscStatus.Ae.Flash)) 
    {
        FlashlightTrigger();
    }    
}

/*************************************************************************
* FUNCTION
*	FlashlightStopProcess
*
* DESCRIPTION
*	Flash light stop process. 
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightStopProcess(MULTISHOT_INFO_ENUM MultishotInfo)
{
        IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;

       FlashTrigStatus.flashTrigFlag = KAL_FALSE;//for exif
       FlashTrigStatus.flashRedeyeTrigFlag = KAL_FALSE;
	if (CAM_FLASH_OFF==DscStatus.Ae.Flash) 
	{
		return;
	}

        //restore interrupt status
        ENABLE_CAMERA_TG_DONE_INT;      
        if( (MULTISHOT_FALSE==MultishotInfo) || (MULTISHOT_END==MultishotInfo) )
        {
            DISABLE_CAMERA_AVSYNC_DONE_INT;
            FlashlightOperationState = FLASHLIGHT_STANDBY_STATE;    	
        }
       
        SensorCommCtrl.FeatureEnable=KAL_FALSE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);

  	/* store AE/AWB setting */
        AwbHwRGain = AwbRgainBak;
        AwbHwGrGain = AwbGRgainBak;			
        AwbHwBGain = AwbBgainBak;			
        AwbHwGbGain = AwbGBgainBak;

        AwbSetGain();  

	
}


/*************************************************************************
* FUNCTION
*	FlashlightPreStrobe (Xenon Flash)
*
* DESCRIPTION
*	LED Pre strobe restore AE index
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPreStrobe(kal_uint8 ShutterDelayFrame) 
{  
    // Update Xenon flash info    

    kal_uint32  EventGroup, i;

    if(FLASHLIGHT_LED_CONSTANT==CameraDriverOperParaBuffer.flashlight_mode) /* Set FLASHLIGHT_PRE_STROBE_STATE to trigger flashlight HISR */ 
    {   
          FlashTrigStatus.flashTrigFlag = KAL_TRUE;
	    //test main flash frame flag
        // Wait one more frame for start pre-strobe  /
        for (i=0; i<ShutterDelayFrame; i++) 
        {
            kal_set_eg_events(IspEventId,0,KAL_AND);                           	                                               

            ENABLE_VIEW_FINDER_MODE;                    
                kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME,
                        &EventGroup,KAL_SUSPEND);

            DISABLE_VIEW_FINDER_MODE;         
        }	    
        SET_AVSYNC_LINENO(0);//VSYNC to reduce LED duration frame
        MainFlashFrameFlag = KAL_FALSE; //temp test
        

                
        IRQMask(IRQ_CAMERA_CODE);    	    
        FlashlightOperationState = FLASHLIGHT_PRE_STROBE_STATE;
        XenonPrestrobeFlag = KAL_TRUE; //set flag & trigger in hisr
        IRQUnmask(IRQ_CAMERA_CODE);       

    	ENABLE_CAMERA_IDLE_INT;
        kal_set_eg_events(IspEventId,0,KAL_AND);    	

        ENABLE_VIEW_FINDER_MODE;            

        //after FlashlightHisr for AE/AWB
        kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME,
               &EventGroup,KAL_SUSPEND);

            
        DISABLE_VIEW_FINDER_MODE;      

        // check frame end
        kal_retrieve_eg_events(IspEventId,CAMERA_ISP_IDLE_EVENT,KAL_OR_CONSUME,
               &EventGroup,KAL_SUSPEND);
        DISABLE_CAMERA_IDLE_INT;                            

    }    
    

    if(KAL_FALSE==MainFlashFrameFlag) // fake main strobe, restore AE index	
    {   
           FlashlightInfo.enable_flashlight_tansfer = KAL_FALSE;
    }

}

/*************************************************************************
* FUNCTION
*	FlashlightPostStrobe (Xenon Flash)
*
* DESCRIPTION
*	Xenon Post strobe AE/AWB control
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPostStrobe(void)
{   // Xenon Flash Post Strobe
    IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;

    if((KAL_FALSE==MainFlashFrameFlag) && (CAM_FLASH_ON!=DscStatus.Ae.Flash)) //fake main flash but not forced on
    { 
                SensorCommCtrl.FeatureEnable=KAL_TRUE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);
    }       
}

void FlashlightPostStrobeStop(void) 
{
    kal_uint32  EventGroup;  
    IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
    if((KAL_FALSE==MainFlashFrameFlag) && (CAM_FLASH_ON!=DscStatus.Ae.Flash)) 
    {
        kal_set_eg_events(IspEventId,0,KAL_AND);    	
        //after FlashlightHisr for AE/AWB
        kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME,
           &EventGroup,KAL_SUSPEND);

        SensorCommCtrl.FeatureEnable=KAL_FALSE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);
    }
}

/*************************************************************************
* FUNCTION
*	FlashlightRedeyeTrigger (Xenon Flash)
*
* DESCRIPTION
*	Xenon flash strobe for anti-red-eye
*
* PARAMETERS
*	None
*
* RETURNS
*	anti-red-eye/pre-strobe
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool FlashlightRedeyeTrigger(void)  // IGBT red eye           
{
    kal_uint32  EventGroup;    
    IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;


    ENABLE_CAMERA_IDLE_INT;
    kal_set_eg_events(IspEventId,0,KAL_AND);    	


    //after FlashlightHisr for AE/AWB
    kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME,
    &EventGroup,KAL_SUSPEND);

    SensorCommCtrl.FeatureEnable=KAL_TRUE;
    CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);


    DISABLE_VIEW_FINDER_MODE;      
    kal_set_eg_events(IspEventId,0,KAL_AND);  
    ENABLE_VIEW_FINDER_MODE;   

    // check frame end
    kal_retrieve_eg_events(IspEventId,CAMERA_ISP_IDLE_EVENT,KAL_OR_CONSUME,
    &EventGroup,KAL_SUSPEND);


    SensorCommCtrl.FeatureEnable=KAL_FALSE;
    CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);

    DISABLE_VIEW_FINDER_MODE;  
    DISABLE_CAMERA_IDLE_INT;    

    return KAL_TRUE;     
  
}


kal_bool FlashlightRedeye(kal_uint32  CaptureDelayFrame)
{ 
    kal_uint32  EventGroup;    
    kal_uint32  i;
    //kal_uint32  redeye_wait_frame;
        
    // If not strobe state, return
    if(FLASHLIGHT_STANDBY_STATE==FlashlightOperationState) //not strobe, return
    {
        return KAL_FALSE;
    }
    /* FlashLight Shutter Config */	
    if( ((FLASHLIGHT_LED_CONSTANT==CameraDriverOperParaBuffer.flashlight_mode)
    &&(CAM_FLASH_REDEYE==DscStatus.Ae.Flash)
    &&(KAL_TRUE==MainFlashFrameFlag)))  // wait stable and strobe for anti-red-eye  
    {
        FlashTrigStatus.flashRedeyeTrigFlag = KAL_TRUE;

        FlashlightRedeyeTrigger();   
       		
        for(i=0; i<CameraDriverOperParaBuffer.led_flashlight_redeye_delay_frame; i++) 
        {

            kal_set_eg_events(IspEventId,0,KAL_AND);
            kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME, //flash modify
                    &EventGroup,KAL_SUSPEND);  		
            }
        return KAL_TRUE;
    }
    else 
    {
        FlashlightPostStrobeStop();
    }
    return KAL_FALSE;
}


void FlashlightTriggerStop(void)
{
   IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
       
    SensorCommCtrl.FeatureEnable=KAL_FALSE;
    CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);
}

/*************************************************************************
* FUNCTION
*	FlashlightTrigger (Xenon Flash)
*
* DESCRIPTION
*	LED strobe
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightTrigger(void)
{
    kal_uint32  EventGroup;
    IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
    kal_set_eg_events(IspEventId,0,KAL_AND);    

    kal_retrieve_eg_events(IspEventId,CAMERA_ISP_VD_READY_EVENT,KAL_OR_CONSUME, //flash modify
                &EventGroup,KAL_SUSPEND);  		

    if((KAL_TRUE==MainFlashFrameFlag) || (CAM_FLASH_ON==DscStatus.Ae.Flash)) 
    {
        AwbSetFlashGain();

        SensorCommCtrl.FeatureEnable=KAL_TRUE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);

        FlashlightOperationState = FLASHLIGHT_TRIGGER_DONE_STATE;

    }
    else 
    {
        SensorCommCtrl.FeatureEnable=KAL_FALSE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);           
    }       
           
    FlashlightOperationState = FLASHLIGHT_MAIN_STROBE_STATE;
   
}

/*************************************************************************
* FUNCTION
*	FlashlightHisr (Xenon Flash)
*
* DESCRIPTION
*	Entry function of Flash light HISR
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightHisr(void)
{
    IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;

    if( KAL_TRUE==XenonPrestrobeFlag)  //prestrobe setting & trigger
    {
        FlashHisrCnt = 0;
        XenonPrestrobeFlag = KAL_FALSE;	        

        SensorCommCtrl.FeatureEnable=KAL_TRUE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);

        //Get AE for normal minimum shtter
        AeGetStatistic();

    }                
    else if(KAL_FALSE==XenonPrestrobeFlag) 
    { 
        if (FLASH_READ_PREFLASH_FRAME==FlashHisrCnt) 
        {
            SensorCommCtrl.FeatureEnable=KAL_FALSE;
            CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &SensorFeatureParaOutLen);
            //ae 
            FlashAeGetStatistic();
            // XXX
            FlashMainFlashInput.sub_flash_duty = 3;	// must >=3 (255/90)
            FlashMainFlashInput.max_main_flash_duty = 0xFF;	
            FlashAeMainFlashCal(&FlashMainFlashInput, &FlashMainFlashResult);

            if (0 == FlashMainFlashResult.main_flash_duty_flag)
            {
                MainFlashFrameFlag = KAL_FALSE;
            }
            else
            {
                MainFlashFrameFlag = KAL_TRUE;          		
            }          

            awbFlashProc();//6238 flash
            FlashlightOperationState = FLASHLIGHT_PRE_STROBE_DONE_STATE;   
        }

    }   

    FlashHisrCnt += 1;

}

/*****************************************************************************/
/*                       Customize Sensor Interface                          */
/*****************************************************************************/
void FlashlightSensorPreviewConfig( FLASHLIGHT_PREVIEW_PARA_STURCT *Para,
                                FLASHLIGHT_INFO_WITH_SENSOR_STRUCT *SensorConfigData) 
{

            SensorConfigData->flashlight_offset= Para->lines ;
            SensorConfigData->flashlight_min_shutter = Para->frame_lines;


}


kal_uint32 FlashlightSensorCaptureConfig(   FLASHLIGHT_CAPTURE_PARA_STRUCT *Para,
                                        FLASHLIGHT_INFO_WITH_SENSOR_STRUCT *SensorConfigData)
{

    kal_uint32  Shutter = Para->shutter;    
    IMAGE_SENSOR_COMM_DATA_STRUCT SensorCommData;

    if (CAM_FLASH_OFF!=DscStatus.Ae.Flash)
    {

        /* cal offset: valid line after VD falling edge */      
        SensorConfigData->flashlight_offset = 1;
        SensorConfigData->flashlight_min_shutter = Shutter;
        //flashCapSize = 1;

        FlashCapShutterInput.main_flash_min_capture_shutter = Shutter;
        FlashCapShutterInput.pre_flash_capture_shutter = Shutter;
        FlashCapShutterInput.main_flash_duty = FlashMainFlashResult.main_flash_duty;
        FlashAeCaptureShutterCal(&FlashCapShutterInput, &FlashCapShutterResult);       
        /* update flash duty */
        FlashMainFlashResult.main_flash_duty = FlashCapShutterResult.main_flash_reduce_duty;		        
        SensorConfigData->flashlight_duty = FlashMainFlashResult.main_flash_duty;

        //if hisr predict main flasy duty is zero, even shutter has common region, it should not trigger main flash
        if( 0!=FlashMainFlashResult.main_flash_duty) 
        {
            if (0 == FlashCapShutterResult.flash_capture_shutter_flag)
            {
                MainFlashFrameFlag = KAL_FALSE;
            }
            else
            {
                MainFlashFrameFlag = KAL_TRUE;           	
            }            
        }
        else 
        {
            MainFlashFrameFlag = KAL_FALSE;
        }

        Shutter = FlashCapShutterResult.main_flash_shutter; //return to sensor driver for update 

        SensorCommData.FeatureValue=FlashCapShutterResult.main_flash_sensor_gain;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_GAIN, &SensorCommData, NULL, 0, &SensorFeatureParaOutLen);

        AeSetIspPregain(FlashCapShutterResult.main_flash_isp_gain);


        //*****Should restore white balance setting if main flash is false
        if ( (KAL_FALSE==MainFlashFrameFlag) && (FLASHLIGHT_STANDBY_STATE!=FlashlightOperationState)) 
        {
            AwbHwRGain = AwbRgainBak;
            AwbHwGrGain = AwbGRgainBak;			
            AwbHwBGain = AwbBgainBak;			
            AwbHwGbGain = AwbGBgainBak;
            AwbSetGain();  

        }
    }
    else
    {
    	SensorConfigData->flashlight_duty = 0;
    }    
    return Shutter;

}

#else  // END #if (defined(LED_FLASHLIGHT_SUPPORT))

/**************   FLASH LIGHT DECLARATION    ***************************************/


/*****************************************************************************/
/*                          Xenon Flash Functions                            */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	FlashlightPreProcess
*
* DESCRIPTION
*	Pre strobe Entry point
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPreProcess(void) 
{
}

/*************************************************************************
* FUNCTION
*	FlashlightRedeyeProcess
*
* DESCRIPTION
*	Flash light anti-redeye control and capture stable loop
*
* PARAMETERS
*	capture_delay_frame
*
* RETURNS
*	redeye strobe or not
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool FlashlightRedeyeProcess(kal_uint32  CaptureDelayFrame) 
{
	return KAL_FALSE;
}

/*************************************************************************
* FUNCTION
*	FlashlightPostProcess
*
* DESCRIPTION
*	Flash light post control for capture timing switch
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPostProcess(void)  //called after sensor switch to capture mode setting
{
}

/*************************************************************************
* FUNCTION
*	FlashlightTriggerProcess
*
* DESCRIPTION
*	Flash light trigger process. 
*   This function should right before open image data base.
*   From trigger process to enable capture should < 1 frame time.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightTriggerProcess(void)
{    
}

/*************************************************************************
* FUNCTION
*	FlashlightStopProcess
*
* DESCRIPTION
*	Flash light stop process. 
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightStopProcess(void) 
{    
}


/*************************************************************************
* FUNCTION
*	flashlight_update_ae (Xenon Flash)
*
* DESCRIPTION
*	Xenon Pre strobe restore AE index
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool flashlight_update_ae(kal_uint8 TargetIdx) 
{
  			
   	return KAL_FALSE;
}

void FlashlightPreStrobe(kal_uint8 SutterDelayFrame) 
{  
 
}

/*************************************************************************
* FUNCTION
*	FlashlightPostStrobe (Xenon Flash)
*
* DESCRIPTION
*	Xenon Post strobe AE/AWB control
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightPostStrobe(void)  // Xenon Flash Post Strobe
{      
}

/*************************************************************************
* FUNCTION
*	FlashlightRedeyeTrigger (Xenon Flash)
*
* DESCRIPTION
*	Xenon flash strobe for anti-red-eye
*
* PARAMETERS
*	None
*
* RETURNS
*	anti-red-eye/pre-strobe
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool FlashlightRedeyeTrigger(void)  // IGBT red eye           
{
    return KAL_FALSE;           
}

kal_bool FlashlightRedeye(kal_uint32  CaptureDelayFrame)
{ 
		return KAL_FALSE;
}
/*************************************************************************
* FUNCTION
*	FlashlightTrigger (Xenon Flash)
*
* DESCRIPTION
*	LED strobe
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightTrigger(void) 
{
}

/*************************************************************************
* FUNCTION
*	FlashlightHisr (Xenon Flash)
*
* DESCRIPTION
*	Entry function of Flash light HISR
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FlashlightHisr(void)
{    
}

/*****************************************************************************/
/*                       Customize Sensor Interface                          */
/*****************************************************************************/
void FlashlightSensorPreviewConfig( FLASHLIGHT_PREVIEW_PARA_STURCT *Para,
                                FLASHLIGHT_INFO_WITH_SENSOR_STRUCT *SensorConfigData) 
{
}


kal_uint32 FlashlightSensorCaptureConfig(   FLASHLIGHT_CAPTURE_PARA_STRUCT *Para,
                                        FLASHLIGHT_INFO_WITH_SENSOR_STRUCT *SensorConfigData)
{ 
    return 0;
}

#endif


#endif /* ISP_SUPPORT && MT6238 */


#if (defined(ISP_SUPPORT) && defined(__ISP_YUV_SENSOR_SUPPORT__))
#include "mm_comm_def.h"
#include "image_sensor.h"
//#include "isp_device.h"

#include "sensor_comm_def.h"
#include "cal_if.h"
#include "isp_if.h"
#include "isp_comm_if.h"
#include "cal_comm_def.h"
#include "isp_flashlight_if.h"

#ifdef LED_FLASHLIGHT_SUPPORT
kal_bool IspYUVFlashlightTriggerEnable = KAL_FALSE;  /// Indicate for this capture process, if trigger flashlight
kal_bool IspYUVFlashlightTriggerModeSupport = FLASHLIGHT_NULL_MODE;
#endif
/*************************************************************************
* FUNCTION
*    IspYuvFlashlightConfig
*
* DESCRIPTION
*    Config the flashlight
* RETURNS
*
*
* GLOBALS AFFECTED
*
*************************************************************************/
void IspYuvFlashlightConfig(P_ISP_FEATURE_CONFIG_3A_BEFORE_CAPTURE_STRUCT pIspPara)
{
#ifdef LED_FLASHLIGHT_SUPPORT
    //static kal_uint32 IspCaptureStableFrameBak = 0;     ///when flashlight, capture stable frame will be changed
    IMAGE_SENSOR_COMM_DATA_STRUCT SensorCommData;
    IMAGE_SENSOR_COMM_DATA_STRUCT SensorOutData;

    SensorOutData.FeatureValue = KAL_FALSE;

    ASSERT(pIspPara!=NULL);
    ASSERT(pIspPara->MultishotInfo<=MULTISHOT_FIRST);  ///MULTISHOT_NOT_FIRST should not be here

    if(IspPreviewConfigData.pCalIspPreviewPara->FlashMode == CAM_FLASH_OFF)
    {
        IspYUVFlashlightTriggerEnable = KAL_FALSE;
    }
    else if(pIspPara->StillCaptureMode == CAM_STILL_CAPTURE_EV_BRACKETING ||
            pIspPara->StillCaptureMode == CAM_STILL_CAPTURE_PANO_VIEW ||
            pIspPara->StillCaptureMode == CAM_STILL_CAPTURE_AUTO_RAMA       ///smile shot?? should be blocked from Camcorder
            )
    {
        IspYUVFlashlightTriggerEnable = KAL_FALSE;
    } 
    else if(IspPreviewConfigData.pCalIspPreviewPara->FlashMode == CAM_FLASH_ON)
    {
        IspYUVFlashlightTriggerEnable = KAL_TRUE;
    }
    else
    {
        /// check YUV sensor condition  if really need to trigger flashlight
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_YUV_CHECK_IF_TRIGGER_FLASHLIGHT, NULL, &SensorOutData, sizeof(IMAGE_SENSOR_COMM_DATA_STRUCT), NULL);
        IspYUVFlashlightTriggerEnable = (kal_bool)SensorOutData.FeatureValue;
    }

    ///Query it each time, because we may have two sensors...
    CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_QUERY_YUV_CTRL_FLASHLIGHT_TRIGGER_MODE, NULL, &SensorCommData, sizeof(IMAGE_SENSOR_COMM_DATA_STRUCT), NULL);
    IspYUVFlashlightTriggerModeSupport = SensorCommData.FeatureValue;

#endif

    return ;
}


/*************************************************************************
* FUNCTION
*    IspYuvFlashlightPrestrobeProcess
*
* RETURNS
*    
*
* GLOBALS AFFECTED
*
*************************************************************************/
void IspYuvFlashlightPrestrobeProcess()
{
#ifdef LED_FLASHLIGHT_SUPPORT
    if(IspYUVFlashlightTriggerEnable)
    {
        /// MULTISHOT_NOT_FIRST will not get here
        IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
        IMAGE_SENSOR_COMM_DATA_STRUCT SensorCommData;
        IMAGE_SENSOR_COMM_DATA_STRUCT Bypass3A;
        kal_uint32 FeatureParaOutLen = 0;

        Bypass3A.FeatureValue = KAL_TRUE;
        SensorCommData.FeatureValue = FLASHLIGHT_CONTINUOUS_MODE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_YUV_CTRL_FLASHLIGHT_TRIGGER_MODE, &SensorCommData, NULL, 0, NULL);

        SensorCommCtrl.FeatureEnable=KAL_TRUE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);

        /// wait for pre-strobe delay frame
        IspWaitNVsync(0, CameraDriverOperParaBuffer.flashlight_prestrobe_frame);

        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_AE_BYPASS, &Bypass3A, NULL, 0, &FeatureParaOutLen);
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_AWB_BYPASS, &Bypass3A, NULL, 0, &FeatureParaOutLen);

        SensorCommCtrl.FeatureEnable=KAL_FALSE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);
        CameraCaptureProfilingWrapper(CAL_CAMERA_PROFILING_PREFLASH_COMPLETE);
    }
#endif
}


/*************************************************************************
* FUNCTION
*    IspYuvFlashlightRedeyeStrobeProcess
*
* RETURNS
*    
*
* GLOBALS AFFECTED
*
*************************************************************************/
void IspYuvFlashlightRedeyeStrobeProcess()
{
#ifdef LED_FLASHLIGHT_SUPPORT
    if(IspYUVFlashlightTriggerEnable && (IspPreviewConfigData.pCalIspPreviewPara->FlashMode == CAM_FLASH_REDEYE))
    {
        /// MULTISHOT_NOT_FIRST will not get here
        IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
        IMAGE_SENSOR_COMM_DATA_STRUCT SensorCommData;
        kal_uint32 FeatureParaOutLen = 0;

        /// wait for pre-strobe to redeye-strobe delay frame
        IspWaitNVsync(0, CameraDriverOperParaBuffer.flashlight_prestrobe_to_redeyestrobe_delay_frame);
        
        SensorCommData.FeatureValue = FLASHLIGHT_CONTINUOUS_MODE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_YUV_CTRL_FLASHLIGHT_TRIGGER_MODE, &SensorCommData, NULL, 0, NULL);

        SensorCommCtrl.FeatureEnable=KAL_TRUE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);

        /// wait for redeye-strobe  frame
        IspWaitNVsync(0, CameraDriverOperParaBuffer.flashlight_redeyestrobe_frame);

        SensorCommCtrl.FeatureEnable=KAL_FALSE;
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);

        CameraCaptureProfilingWrapper(CAL_CAMERA_PROFILING_POST_PROCESS_FLASH_COMPLETE);
    }
#endif
}



/*************************************************************************
* FUNCTION
*    IspYuvFlashlightMainStrobeProcess
* RETURNS
*    
*
* GLOBALS AFFECTED
*
*************************************************************************/
void IspYuvFlashlightMainStrobeProcess(kal_bool On)
{
#ifdef LED_FLASHLIGHT_SUPPORT
    if(IspYUVFlashlightTriggerEnable)
    {
        IMAGE_SENSOR_COMM_DATA_STRUCT SensorCommData;
        IMAGE_SENSOR_COMM_CTRL_STRUCT SensorCommCtrl;
        kal_uint32 FeatureParaOutLen = 0;

        if(On)
        {
            if(IspYUVFlashlightTriggerModeSupport & FLASHLIGHT_PULSE_MODE)
            {
                SensorCommData.FeatureValue = FLASHLIGHT_PULSE_MODE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_YUV_CTRL_FLASHLIGHT_TRIGGER_MODE, &SensorCommData, NULL, 0, NULL);
            
                SensorCommCtrl.FeatureEnable=KAL_TRUE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);

                /// wait for main-strobe frame
                IspWaitNVsync(0, CameraDriverOperParaBuffer.flashlight_mainstrobe_frame);

                SensorCommCtrl.FeatureEnable=KAL_FALSE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);
            }
            else
            {
                SensorCommData.FeatureValue = FLASHLIGHT_CONTINUOUS_MODE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_YUV_CTRL_FLASHLIGHT_TRIGGER_MODE, &SensorCommData, NULL, 0, NULL);
            
                SensorCommCtrl.FeatureEnable=KAL_TRUE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);

                /// wait for main-strobe frame
                IspWaitNVsync(0, CameraDriverOperParaBuffer.flashlight_mainstrobe_frame);
            }
        }
        else
        {
            ///only for FLASHLIGHT_CONTINUOUS_MODE
            if(!(IspYUVFlashlightTriggerModeSupport & FLASHLIGHT_PULSE_MODE))
            {
                SensorCommCtrl.FeatureEnable=KAL_FALSE;
                CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_CTRL_FLASHLIGHT, &SensorCommCtrl, NULL, 0, &FeatureParaOutLen);
            }
        }
    }
#endif
}

/*************************************************************************
* FUNCTION
*    IspYuvFlashlightRestore3AProcess
* RETURNS
*    
*
* GLOBALS AFFECTED
*
*************************************************************************/
void IspYuvFlashlightRestore3AProcess()
{
#if defined(LED_FLASHLIGHT_SUPPORT)
    if(IspYUVFlashlightTriggerEnable)
    {
        ///if flashlight enable, 3A will be bypassed in capture pre-strobe process, therefore always enable 3A here
        IMAGE_SENSOR_COMM_DATA_STRUCT Bypass3A;
        Bypass3A.FeatureValue = KAL_FALSE;
        
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_AE_BYPASS, &Bypass3A, NULL, 0, NULL);
        CustomDriverFunc.SensorFeatureCtrl(IMAGE_SENSOR_FEATURE_SET_AWB_BYPASS, &Bypass3A, NULL, 0, NULL);

        IspYUVFlashlightTriggerEnable = KAL_FALSE;
    }
#endif
}

#endif ///(defined(ISP_SUPPORT)&&defined(__ISP_YUV_SENSOR_SUPPORT__))
